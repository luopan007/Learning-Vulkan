
#include "VulkanLED.h"
#include "VulkanApplication.h"

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback	= NULL;
	dbgDestroyDebugReportCallback	= NULL;
	debugReportCallback = NULL;
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
	dbgCreateDebugReportCallback = NULL;
	dbgDestroyDebugReportCallback = NULL;
	debugReportCallback = NULL;
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	uint32_t						instanceLayerCount;		// 存储实例层的数目
	std::vector<VkLayerProperties>	layerProperties;		// 通过向量数组存储层的属性
	VkResult						result;				// 检查Vulkan API的执行结果

	// 查询所有的层
	do {
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);

		if (result)
		{
			return result;
		}

		if (instanceLayerCount == 0)
		{
			return VK_INCOMPLETE; // 获取失败
		}

		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	// 查询每个层的所有扩展信息，并进行保存
	std::cout << "\nInstanced Layers" << std::endl;
	std::cout << "===================" << std::endl;
	for (auto globalLayerProp: layerProperties) {
		// 显示层的名字和描述信息
		std::cout <<"\n"<< globalLayerProp.description <<"\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.layerName <<"\n";

		LayerProperties layerProps;
		layerProps.properties = globalLayerProp;

		// 根据层属性信息，获取基于实例的扩展
		result = getExtensionProperties(layerProps);

		if (result){
			continue;
		}

		layerPropertyList.push_back(layerProps);
		// 显示每个实例层的扩展名称
		// 每个层都可以支持一个或者多个扩展
		for (auto j : layerProps.extensions) {
			std::cout << "\t\t|\n\t\t|---[Layer Extension]--> " << j.extensionName << "\n";
		}
	}
	return result;
}

/*
* 获取基于设备的扩展
*/
VkResult VulkanLayerAndExtension::getDeviceExtensionProperties(VkPhysicalDevice* gpu)
{
	VkResult						result;					// 检查Vulkan API的执行结果

	// 查询每个层的所有扩展信息，并进行保存
	std::cout << "Device extensions" << std::endl;
	std::cout << "===================" << std::endl;
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	std::vector<LayerProperties>* instanceLayerProp = &appObj->GetInstance()->instanceObj.layerExtension.layerPropertyList;
	for (auto globalLayerProp : *instanceLayerProp) {
		LayerProperties layerProps;
		layerProps.properties = globalLayerProp.properties;

		if (result = getExtensionProperties(layerProps, gpu))
			continue;

		std::cout << "\n" << globalLayerProp.properties.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.properties.layerName << "\n";
		layerPropertyList.push_back(layerProps);

		if (layerProps.extensions.size()) {
			for (auto j : layerProps.extensions) {
				std::cout << "\t\t|\n\t\t|---[Device Extesion]--> " << j.extensionName << "\n";
			}
		}
		else {
			std::cout << "\t\t|\n\t\t|---[Device Extesion]--> No extension found \n";
		}
	}
	return result;
}

// 这个函数可以获取基于实例或者设备的扩展以及它们的属性信息
// 传入一个合法的物理设备指针gpu，即可获取基于物理设备的扩展
// 传入NULL得到的就是基于实例的扩展了
VkResult VulkanLayerAndExtension::getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu)
{
	uint32_t	extensionCount;								 // 保存每一层的扩展总数
	VkResult	result;								         // 检查Vulkan API的执行结果
	char*		layerName = layerProps.properties.layerName; // 层的名字

	do {
		// 获取当前层中的扩展的总数
		if (gpu)
		{
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, NULL);
		}
		else
		{
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, NULL);
		}

		if (result || extensionCount == 0)
		{
			continue;
		}

		layerProps.extensions.resize(extensionCount);

		// 获取所有的扩展属性
		if (gpu)
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
		else
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
	} while (result == VK_INCOMPLETE);

	return result;
}

void VulkanLayerAndExtension::destroyDebugReportCallback()
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	VkInstance& instance	= appObj->instanceObj.instance;
	dbgDestroyDebugReportCallback(instance, debugReportCallback, NULL);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanLayerAndExtension::debugFunction(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject, size_t location, int32_t msgCode,
	const char *layerPrefix, const char *msg, void *userData) {

	if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] ERROR: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] WARNING: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] INFORMATION: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] PERFORMANCE: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		std::cout << "[VK_DEBUG_REPORT] DEBUG: [" << layerPrefix << "] Code" << msgCode << ":" << msg << std::endl;
	}
	else {
		return VK_FALSE;
	}

	fflush(stdout);
	return VK_TRUE;
}

/*
Inspects the incoming layer names against system supported layers, theses layers are not supported
then this function removed it from layerNames allow
*/
VkBool32 VulkanLayerAndExtension::areLayersSupported(std::vector<const char *> &layerNames)
{
	uint32_t checkCount = (uint32_t)layerNames.size();
	uint32_t layerCount = (uint32_t)layerPropertyList.size();
	std::vector<const char*> unsupportLayerNames;
	for (uint32_t i = 0; i < checkCount; i++) {
		VkBool32 isSupported = 0;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (!strcmp(layerNames[i], layerPropertyList[j].properties.layerName)) {
				isSupported = 1;
			}
		}

		if (!isSupported) {
			std::cout << "No Layer support found, removed from layer: " << layerNames[i] << std::endl;
			unsupportLayerNames.push_back(layerNames[i]);
		}
		else {
			std::cout << "Layer supported: " << layerNames[i] << std::endl;
		}
	}

	for (auto i : unsupportLayerNames) {
		auto it = std::find(layerNames.begin(), layerNames.end(), i);
		if (it != layerNames.end()) layerNames.erase(it);
	}

	return true;
}



VkResult VulkanLayerAndExtension::createDebugReportCallback()
{
	VkResult result;

	VulkanApplication* appObj	= VulkanApplication::GetInstance();
	VkInstance* instance		= &appObj->instanceObj.instance;

	// 获取 vkCreateDebugReportCallbackEXT 函数指针
	dbgCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkCreateDebugReportCallbackEXT");
	if (!dbgCreateDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkCreateDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgCreateDebugReportCallback function\n";

	// 获取 vkDestroyDebugReportCallbackEXT 函数指针
	dbgDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(*instance, "vkDestroyDebugReportCallbackEXT");
	if (!dbgDestroyDebugReportCallback) {
		std::cout << "Error: GetInstanceProcAddr unable to locate vkDestroyDebugReportCallbackEXT function." << std::endl;
		return VK_ERROR_INITIALIZATION_FAILED;
	}
	std::cout << "GetInstanceProcAddr loaded dbgDestroyDebugReportCallback function\n";

	// 定义调试报告的结构体，并且设置 'debugFunction'，该函数用来打印调试信息到控制台
	dbgReportCreateInfo.sType		= VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	dbgReportCreateInfo.pfnCallback = debugFunction;
	dbgReportCreateInfo.pUserData	= NULL;
	dbgReportCreateInfo.pNext		= NULL;
	dbgReportCreateInfo.flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
									  VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
									  VK_DEBUG_REPORT_ERROR_BIT_EXT |
									  VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	// 创建一个调试报告对象，并且将它的句柄保存到 debugReportCallback 中
	result = dbgCreateDebugReportCallback(*instance, &dbgReportCreateInfo, NULL, &debugReportCallback);
	if (result == VK_SUCCESS) {
		std::cout << "Debug report callback object created successfully\n";
	}
	return result;
}
