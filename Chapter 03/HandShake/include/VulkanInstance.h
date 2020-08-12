
#pragma once

#include "VulkanLED.h"


class VulkanInstance{
public:
    VulkanInstance(){}
    ~VulkanInstance(){}

public:
    // Vulkan实例对象的变量
    VkInstance	instance;

	// Vulkan实例相关的层和扩展
	VulkanLayerAndExtension		layerExtension;

public:
	// 创建Vulkan实例的函数
	VkResult createInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions, const char* applicationName);
	
	// 销毁Vulkan实例的函数
	void destroyInstance();
};
