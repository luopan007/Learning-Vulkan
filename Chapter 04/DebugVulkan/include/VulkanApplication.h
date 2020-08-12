
#pragma once
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanLED.h"

class VulkanApplication
{
private:
	// CTOR: Application constructor responsible for layer enumeration.
	VulkanApplication();

public:
	// DTOR
	~VulkanApplication();

private:
	// Variable for Single Ton implementation
    static std::unique_ptr<VulkanApplication> instance;
    static std::once_flag onlyOnce;

public:
    static VulkanApplication* GetInstance();

	// Simple program life cycle
	void initialize();				// Initialize and allocate resources
	void prepare();					// Prepare resource
	void update();					// Update data
	bool render();					// Render primitives
	void deInitialize();			// Release resources

private:
	// 创建Vulkan实例对象
	VkResult createVulkanInstance(std::vector<const char *>& layers, std::vector<const char *>& extensions,  const char* applicationName);
	
	VkResult handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char *>& layers, std::vector<const char *>& extensions);
	
	VkResult enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpus);

public:
    VulkanInstance  instanceObj;	// Vulkan实例对象

	VulkanDevice*      deviceObj;
private:
	bool debugFlag;
};
