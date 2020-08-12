#include "VulkanApplication.h"

std::unique_ptr<VulkanApplication> VulkanApplication::instance;
std::once_flag VulkanApplication::onlyOnce;

extern std::vector<const char *> instanceExtensionNames;
extern std::vector<const char *> layerNames;
extern std::vector<const char *> deviceExtensionNames;


// 类的构造函数负责枚举层
VulkanApplication::VulkanApplication() 
{
	// 应用程序启动时，开始枚举实例层
	instanceObj.layerExtension.getInstanceLayerProperties();

	deviceObj = NULL;
	debugFlag = true;
}

VulkanApplication::~VulkanApplication()
{

}

// 返回VulkanApplication的单例对象
VulkanApplication* VulkanApplication::GetInstance(){
    std::call_once(onlyOnce, [](){instance.reset(new VulkanApplication()); });
    return instance.get();
}

// 用于创建Vulkan实例的成员函数
VkResult VulkanApplication::createVulkanInstance( std::vector<const char *>& layers, std::vector<const char *>& extensionNames, const char* applicationName)
{
	return instanceObj.createInstance(layers, extensionNames, applicationName);
}

// This function is responsible for creating the logical device.
// The process of logical device creation requires the following steps:-
// 1. Get the physical device specific layer and corresponding extensions [Optional]
// 2. Create user define VulkanDevice object
// 3. Provide the list of layer and extension that needs to enabled in this physical device
// 4. Get the physical device or GPU properties
// 5. Get the memory properties from the physical device or GPU
// 6. Query the physical device exposed queues and related properties
// 7. Get the handle of graphics queue
// 8. Create the logical device, connect it to the graphics queue.

// 用于创建设备和队列的高级封装函数
VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions )
{

	// 用户定义了Vulkan的设备对象
	// 它负责管理物理设备和逻辑设备，也负责管理它所包含的队列和属性
	deviceObj = new VulkanDevice(gpu);
	if (!deviceObj){
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}
	
	// 打印设备端可用的层以及它们的扩展信息
	deviceObj->layerExtension.getDeviceExtensionProperties(gpu);

	// 获取物理设备或GPU的属性
	vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProps);

	// 获取物理设备或者GPU的内存属性
	vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

	// 查询物理设备端可用的队列并返回它们的属性
	deviceObj->getPhysicalDeviceQueuesAndProperties();

	// 获取能够支持图形操作流水线的队列
	deviceObj->getGraphicsQueueHandle();

	// 创建逻辑设备，确保设备已经关联到一个图形队列当中了
	return deviceObj->createDevice(layers, extensions);
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList)
{
	// 记录GPU的数量
	uint32_t gpuDeviceCount;

	// 获取GPU数量
	VkResult result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, NULL);
	assert(result == VK_SUCCESS);

	assert(gpuDeviceCount);

	gpuList.resize(gpuDeviceCount);

	// 获取物理设备对象
	result = vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
	assert(result == VK_SUCCESS);

	return result;
}

// 应用程序的构造函数负责层的枚举
void VulkanApplication::initialize()
{
	char title[] = "Hello World!!!";

	// Check if the supplied layer are support or not
	if (debugFlag) {
		instanceObj.layerExtension.areLayersSupported(layerNames);
	}

	// 创建Vulkan实例，并启用指定的层和扩展名称
	createVulkanInstance(layerNames, instanceExtensionNames, title);

	// Create the debugging report if debugging is enabled
	if (debugFlag) {
		instanceObj.layerExtension.createDebugReportCallback();
	}

	// 获取系统中物理设备的列表
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);

	// 这个例子中只使用第一个可用的物理设备
	if (gpuList.size() > 0) {
		handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
	}
}

void VulkanApplication::deInitialize()
{
	deviceObj->destroyDevice();
	if (debugFlag) {
		instanceObj.layerExtension.destroyDebugReportCallback();
	}
	instanceObj.destroyInstance();
}

void VulkanApplication::prepare()
{
	// Place holder, this will be utilized in the upcoming chapters
}

void VulkanApplication::update()
{
	// Place holder, this will be utilized in the upcoming chapters
}

bool VulkanApplication::render() 
{
	// Place holder, this will be utilized in the upcoming chapters
	return true;
}
