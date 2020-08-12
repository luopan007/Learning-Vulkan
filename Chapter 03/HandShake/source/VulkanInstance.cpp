
#include "VulkanInstance.h"

VkResult VulkanInstance::createInstance(std::vector<const char *>& layers, std::vector<const char *>& extensionNames, char const*const appName)
{
	// 设置实例相关的层和扩展信息
	layerExtension.appRequestedExtensionNames	= extensionNames;
	layerExtension.appRequestedLayerNames		= layers;
	
	// 定义Vulkan应用程序的结构体
	VkApplicationInfo appInfo	= {};
	appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext				= NULL;
	appInfo.pApplicationName	= appName;
	appInfo.applicationVersion	= 1;
	appInfo.pEngineName			= appName;
	appInfo.engineVersion		= 1;
	appInfo.apiVersion			= VK_MAKE_VERSION(1, 2, 0);

	// 定义Vulkan实例创建的参数结构体
	VkInstanceCreateInfo instInfo	= {};
	instInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext					= VK_NULL_HANDLE;
	instInfo.flags					= 0;
	instInfo.pApplicationInfo		= &appInfo;

	// 设置需要启用的层名称的列表
	instInfo.enabledLayerCount		= static_cast<uint32_t>(layers.size());
	instInfo.ppEnabledLayerNames	= layers.size() ? layers.data() : NULL;

	// 设置需要启用的扩展名称列表
	instInfo.enabledExtensionCount	= static_cast<uint32_t>(extensionNames.size());
	instInfo.ppEnabledExtensionNames = extensionNames.size() ? extensionNames.data() : NULL;

	VkResult result = vkCreateInstance(&instInfo, NULL, &instance);
	assert(result == VK_SUCCESS);

	return result;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(instance, NULL);
}
