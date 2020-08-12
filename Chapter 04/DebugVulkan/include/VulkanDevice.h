
#pragma once

#include "Headers.h"
//#include "VulkanQueue.h"
#include "VulkanLED.h"

class VulkanApplication;


// Vulkan exposes one or more devices, each of which exposes one or more queues which may process 
// work asynchronously to one another.The queues supported by a device are divided into families, 
// each of which supports one or more types of functionality and may contain multiple queues with 
// similar characteristics.Queues within a single family are considered compatible with one another, 
// and work produced for a family of queues can be executed on any queue within that family
class VulkanDevice{
public:
	VulkanDevice(VkPhysicalDevice* gpu);
	~VulkanDevice();

public:
    VkDevice							device;		// 逻辑设备
	VkPhysicalDevice*					gpu;		// 物理设备
	VkPhysicalDeviceProperties			gpuProps;	// 物理设备属性
    VkPhysicalDeviceMemoryProperties	memoryProperties;

public:
	// 队列相关属性
	VkQueue									queue;							// Vulkan队列属性
	std::vector<VkQueueFamilyProperties>	queueFamilyProps;				// 保存物理设备暴露的所有队列族
	uint32_t								graphicsQueueIndex;				// 保存图形队列的索引号
	uint32_t								graphicsQueueWithPresentIndex;  // 
	uint32_t								queueFamilyCount;				// 设备中队列的数量

	// 设备相关的扩展
	VulkanLayerAndExtension		layerExtension;

public:
	VkResult createDevice(std::vector<const char *>& layers, std::vector<const char *>& extensions);
	void destroyDevice();

	void getPhysicalDeviceQueuesAndProperties();

	uint32_t getGraphicsQueueHandle();
};
