
#pragma once
#include "Headers.h"

struct LayerProperties{
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension{
public:
	VulkanLayerAndExtension();
	~VulkanLayerAndExtension();

	// 层列表
	std::vector<const char *>			appRequestedLayerNames;
	
	// 扩展列表
	std::vector<const char *>			appRequestedExtensionNames;
	
	// 层和相关的扩展列表
	std::vector<LayerProperties>		layerPropertyList;

	// 全局层
	VkResult getInstanceLayerProperties();
	
	// 全局扩展
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu = NULL);

	// 基于设备的扩展
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);
};
