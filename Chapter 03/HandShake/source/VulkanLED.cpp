
#include "VulkanLED.h"
#include "VulkanApplication.h"

VulkanLayerAndExtension::VulkanLayerAndExtension()
{
}

VulkanLayerAndExtension::~VulkanLayerAndExtension()
{
}

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	uint32_t						instanceLayerCount;		// 存储实例层的数目
	std::vector<VkLayerProperties>	layerProperties;		// 通过向量数组存储层的属性
	VkResult						result;					// 检查Vulkan API的执行结果

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
	VkResult	result;										 // 检查Vulkan API的执行结果
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
		{
			result = vkEnumerateDeviceExtensionProperties(*gpu, layerName, &extensionCount, layerProps.extensions.data());
		}
		else
		{
			result = vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, layerProps.extensions.data());
		}
	} while (result == VK_INCOMPLETE);

	return result;
}
