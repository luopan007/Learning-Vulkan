
#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanApplication.h"
#include "DataStructures.h"

VulkanDevice::VulkanDevice(VkPhysicalDevice* physicalDevice) 
{
	gpu = physicalDevice;
}

VulkanDevice::~VulkanDevice() 
{
}

// Note: This function requires queue object to be in existence before
VkResult VulkanDevice::createDevice(std::vector<const char *>& layers, std::vector<const char *>& extensions)
{
	layerExtension.appRequestedLayerNames		= layers;
	layerExtension.appRequestedExtensionNames	= extensions;

	// Create Device with available queue information.

	VkResult result;
	float queuePriorities[1]			= { 0.0 };
	
	// 创建对象的信息
	VkDeviceQueueCreateInfo queueInfo	        = {};
	queueInfo.queueFamilyIndex			= graphicsQueueIndex;  
	queueInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext					= NULL;
	queueInfo.queueCount				= 1;
	queueInfo.pQueuePriorities			= queuePriorities;

	VkDeviceCreateInfo deviceInfo		= {};
	deviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext					= NULL;
	deviceInfo.queueCreateInfoCount		= 1;
	deviceInfo.pQueueCreateInfos		= &queueInfo;
	deviceInfo.enabledLayerCount		= 0;
	deviceInfo.ppEnabledLayerNames		= NULL; // 设备的层属性信息已经被废弃
	deviceInfo.enabledExtensionCount	= (uint32_t)extensions.size();
	deviceInfo.ppEnabledExtensionNames	= extensions.size() ? extensions.data() : NULL;
	deviceInfo.pEnabledFeatures		= NULL;

	result = vkCreateDevice(*gpu, &deviceInfo, NULL, &device);
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanDevice::getPhysicalDeviceQueuesAndProperties()
{
	// 第二个参数传入NULL，从而查询队列族的数量
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, NULL);
	
	// 分配数组空间保存队列属性信息
	queueFamilyProps.resize(queueFamilyCount);

	// 获取队列族的属性
	vkGetPhysicalDeviceQueueFamilyProperties(*gpu, &queueFamilyCount, queueFamilyProps.data());
}

uint32_t VulkanDevice::getGraphicsQueueHandle()
{
	//	1. 迭代查找物理设备所支持的所有队列
	//	2. 获取图形队列或队列族的类型
	//			一共支持4种类型：
	//			图形队列		- VK_QUEUE_GRAPHICS_BIT 
	//			计算队列		- VK_QUEUE_COMPUTE_BIT
	//			传输队列	        - VK_QUEUE_TRANSFER_BIT
	//			稀疏队列		- VK_QUEUE_SPARSE_BINDING_BIT
	//	3. 获取图形队列族的句柄或索引ID，这个ID将会在接下来的程序中使用

	bool found = false;
	// 1. 迭代查找物理设备所支持的所有队列
	for (unsigned int i = 0; i < queueFamilyCount; i++){
		// 2. 获取图形队列的类型
		//		一共支持4种类型：
		//		图形队列		- VK_QUEUE_GRAPHICS_BIT 
		//		计算队列		- VK_QUEUE_COMPUTE_BIT
		//		传输队列	        - VK_QUEUE_TRANSFER_BIT
		//		稀疏队列		- VK_QUEUE_SPARSE_BINDING_BIT

		if (queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
			// 3. 获取图形队列族的句柄或索引ID
			found			= true;
			graphicsQueueIndex	= i;
			break;
		}
	}

	// 检查是否有发现可用队列
	assert(found);

	return 0;
}

void VulkanDevice::destroyDevice()
{
	vkDestroyDevice(device, NULL);
}
