
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

	// 应用程序中所需要用到的层名称列表
	std::vector<const char *>			appRequestedLayerNames;
	
	// 应用程序中所需要用到的扩展名称列表
	std::vector<const char *>			appRequestedExtensionNames;
	
	// 层和相关的扩展列表
	std::vector<LayerProperties>		        layerPropertyList;

	// 全局层
	VkResult getInstanceLayerProperties();
	
	// 全局扩展
	VkResult getExtensionProperties(LayerProperties &layerProps, VkPhysicalDevice* gpu = NULL);

	// 基于设备的扩展
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);

	/******* VULKAN DEBUGGING MEMBER FUNCTION AND VARAIBLES *******/
	VkBool32 areLayersSupported(std::vector<const char *> &layerNames);
	VkResult createDebugReportCallback();
	void	destroyDebugReportCallback();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugFunction( VkFlags msgFlags, 
														 VkDebugReportObjectTypeEXT objType,
														 uint64_t srcObject, 
														 size_t location, 
														 int32_t msgCode,
														 const char *layerPrefix,
														 const char *msg, 
														 void *userData);

private:
	// 创建和删除函数指针的声明
	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	
	// 调试报告回调的句柄
	VkDebugReportCallbackEXT debugReportCallback;
public:
	// 创建调试报告的参数结构体
	VkDebugReportCallbackCreateInfoEXT dbgReportCreateInfo = {};
};
