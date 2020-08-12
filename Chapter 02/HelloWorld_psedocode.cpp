
/**************** 1. 枚举实例的层属性 ****************/
// 获得实例层的数量
uint32_t instanceLayerCount;
	
//设置第二个参数为 NULL 即可返回层的数量
vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL); 

VkLayerProperties *layerProperty = NULL;
vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperty);

// 遍历每一个可用的实例层，获取功能扩展信息
foreach  layerProperty{
	VkExtensionProperties *instanceExtensions;
	res = vkEnumerateInstanceExtensionProperties(layer_name, 
			&instanceExtensionCount, instanceExtensions);
}

/**************** 2. 创建实例 ****************/
VkInstance instance;	// Vulkan 实例对象
VkInstanceCreateInfo instanceInfo	 = {};

// 设置实例中需要开启的层的名称
instanceInfo.ppEnabledLayerNames	 = {"VK_LAYER_LUNARG_standard_validation"};

// 设置实例中需要开启的功能扩展
instanceInfo.ppEnabledExtensionNames = {VK_KHR_SURFACE_EXTENSION_NAME, 
										VK_KHR_WIN32_SURFACE_EXTENSION_NAME};

// 创建实例对象
vkCreateInstance(&instanceInfo, NULL, &instance);

/**************** 3. 枚举物理设备 ****************/
VkPhysicalDevice				gpu;		// 物理设备对象
uint32_t						gpuCount;	// 物理设备数量
std::vector<VkPhysicalDevice>	gpuList;	// 物理设备列表
// 获取物理设备的数量，也就是GPU的数量
vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);

// 获取所有物理设备，并存储到列表之中
vkEnumeratePhysicalDevices(instance, &gpuCount, gpuList);

/**************** 4. 创建逻辑设备 ****************/
// 获取设备队列和队列属性
vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueCount, queueProperties);

// 获取物理设备或GPU的内存属性
vkGetPhysicalDeviceMemoryProperties(gpu, &memoryProperties);

// 获取物理设备或GPU的属性
vkGetPhysicalDeviceProperties(gpu, &gpuProps);

// 从物理设备创建逻辑设备
VkDevice			device;	// 逻辑设备对象
VkDeviceCreateInfo	deviceInfo = {};
vkCreateDevice(gpuList[0], &deviceInfo, NULL, &device);

/**************** 5. 显示初始化 ****************/
// 
// 创建空窗口
CreateWindowEx(...);		/*Windows平台*/
xcb_create_window(...); 	/*Linux平台*/

// 查询WSI扩展并保存为函数指针。例如：
// vkCreateSwapchainKHR, vkCreateSwapchainKHR .....

// 创建抽象表面对象
VkWin32SurfaceCreateInfoKHR createInfo = {};
vkCreateWin32SurfaceKHR(instance, &createInfo, NULL, &surface);

// 选择一个支持显示的队列
foreach Queue in All Queues{
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, queueIndex, surface, &isPresentaionSupported);
	// 存储显示队列的索引
	if (isPresentaionSupported) {
		graphicsQueueFamilyIndex = Queue.index;
		break;
	}
}

// 获取兼容显示层的队列，同时也是一个图形队列
vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);

// 分配内存空间用来记录绘制表面的格式总数
uint32_t formatCount;
vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, NULL);

VkSurfaceFormatKHR *surfaceFormats = allocate memory('formatCount' * VkSurfaceFormatKHR);

// 保存表面格式到 VkSurfaceFormatKHR 对象中
vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, surfaceFormats);

/**************** 6. 创建交换链 ****************/

//开始将指令记录到指令缓存中
vkBeginCommandBuffer(cmd, &cmdBufInfo);

// 获取表面性能参数
vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surfCapabilities);
		
// 获取表面显示模式
vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, NULL);
VkPresentModeKHR presentModes[presentModeCount];
vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes);
		
// 创建交换链
VkSwapchainCreateInfoKHR swapChainInfo = {};
vkCreateSwapchainKHR(device, &swapChainInfo, NULL, &swapChain);
		
// 创建交换链图像对应的图像视图
vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, NULL);
VkImage swapchainImages[swapchainImageCount];
vkGetSwapchainImagesKHR(device, swapChain, &swapchainImageCount, swapchainImages);
		
// 获取交换链中的图像
foreach swapchainImages{
	// 设置布局方式，与具体驱动的实现兼容
	SetImageLayout();

	// 插入流水线屏障
	VkImageMemoryBarrier imgMemoryBarrier = { ... };
	vkCmdPipelineBarrier(cmd,srcStages,destStages,0,0,NULL,0,NULL,1,&imgMemoryBarrier);

	// 为每一个交换链图像创建图像视图
	SwapChainBuffer scBuffer = {...};
	VkImageViewCreateInfo colorImageView = {};
	colorImageView.image = sc_buffer.image;
	vkCreateImageView(device, &colorImageView, NULL, &scBuffer.view);

	// 保存图像视图以供应用程序使用
	buffers.push_back(scBuffer);
}

/**************** 7. 创建深度图像 ****************/

// 查询当前物理设备所支持的格式
vkGetPhysicalDeviceFormatProperties(gpuList, depthFormat, &properties);

// 创建图像对象
vkCreateImage(device, &imageInfo, NULL, &imageObject);

// 获取图像资源所需要的内存空间---物理设备侧
vkGetImageMemoryRequirements(device, imageObject, &memRequirements);

// 分配内存--物理设备侧
VkDeviceMemory	mem;
vkAllocateMemory(device, &memAlloc, NULL, &mem);

// 绑定内存
vkBindImageMemory(device, imageObject, mem, 0);

// 设置图像布局，可用于当前设备
SetImageLayout(. . .)

// 插入新的流水线屏障，确保刚才设置的图像布局在图像被真正创建之前就已经创建了
vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgPipelineBarrier);

// 创建深度图像视图
VkImageViewCreateInfo imgViewInfo = { ... };
imgViewInfo.image = imageObject;
vkCreateImageView(device, &imgViewInfo, NULL, &view);


/**************** 8. 构建着色器 ****************/
#version 450
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;
layout (location = 0) out vec4 outColor;
out gl_PerVertex {
	vec4 gl_Position;
};
void main() {
	outColor = inColor;
	gl_Position = pos;
	gl_Position.y = -gl_Position.y;
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}

#version 450
layout (location = 0) in vec4 color;
layout (location = 0) out vec4 outColor;
void main() {
	outColor = color;
};

VkPipelineShaderStageCreateInfo vtxShdrStages = {....};
VkShaderModuleCreateInfo moduleCreateInfo = { ... };
moduleCreateInfo.pCode = spvVertexShaderData/*SPIRV form shader data*/;
// 在物理设备端创建着色器木块
vkCreateShaderModule(device, &moduleCreateInfo, NULL, &vtxShdrStages.module);

/**************** 9. 创建描述符和流水线布局 ****************/

// 描述符的定义类型和着色器中是对应的
VkDescriptorSetLayoutBinding layoutBindings[2];
layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
layoutBindings[0].binding		 = 0;
layoutBindings[0].stageFlags	 = VK_SHADER_STAGE_VERTEX_BIT;
layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
layoutBindings[1].binding		 = 0;
layoutBindings[1].stageFlags     = VK_SHADER_STAGE_FRAGMENT_BIT;

// 设置布局绑定，创建描述符
VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
descriptorLayout.pBindings					     = layoutBindings;
VkDescriptorSetLayout descLayout[2];
vkCreateDescriptorSetLayout(device, &descriptorLayout, NULL, descLayout.data());

// 使用描述符创建流水线布局
VkPipelineLayoutCreateInfo pipelineLayoutCI = { ... };
pipelineLayoutCI.pSetLayouts				= descLayout.data();
vkCreatePipelineLayout(device, &pipelineLayoutCI, NULL, &pipelineLayout);


/**************** 10. 渲染通道 ****************/

// 定义两个附件，分别对应颜色和深度缓存
VkAttachmentDescription attachments[2];
attachments[0].format = colorImageformat;
attachments[0].loadOp = clear ? VK_ATTACHMENT_LOAD_OP_CLEAR 
							: VK_ATTACHMENT_LOAD_OP_DONT_CARE;
attachments[1].format = depthImageformat;
attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

VkAttachmentReference colorReference, depthReference = {...};

// 使用颜色图像和深度图像创建子通道
VkSubpassDescription subpass	= {};
subpass.pColorAttachments		= &colorReference;
subpass.pDepthStencilAttachment = &depthReference;

// 定义渲染通道控制的结构体
VkRenderPassCreateInfo rpInfo	= { &attachments,&subpass ...};

VkRenderPass renderPass; // 创建渲染通道对象
vkCreateRenderPass(device, &rpInfo, NULL, &renderPass);

/**************** 11. 创建帧缓存 ****************/
VkImageView attachments[2];						// [0]表示颜色, [1]表示深度
attachments[1] = Depth.view;

VkFramebufferCreateInfo fbInfo = {};
fbInfo.renderPass				= renderPass;	// 渲染通道对象
fbInfo.pAttachments				= attachments;	// 图像视图附件
fbInfo.width					= width;		// 帧缓存宽度
fbInfo.height					= height;		// 帧缓存高度

// 为交换链中的每幅图像的帧缓存对象分配内存，每幅图像只有一个帧缓存
VkFramebuffer framebuffers[交换链中的绘制图像数量];

foreach(drawing buffer in swapchain) {
	attachments[0] = currentSwapChainDrawImage.view;
	vkCreateFramebuffer(device, &fbInfo, NULL, &framebuffers[i]);
}

/****************  12. 产生几何体，在GPU中保存顶点数据 ****************/

static const VertexWithColor triangleData[] = {
	/*{  x ,     y,    z,    w,    r,    g,    b,   a },*/
	{ 0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0 },
	{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0 },
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0 },
};

VkBuffer				buffer;
VkMemoryRequirements	mem_requirement;
VkDeviceMemory			deviceMemory;

// 创建缓存对象，查询所需要的内存空间，分配内存
VkBufferCreateInfo buffer_info = { ... };
vkCreateBuffer(device, &buffer_info, NULL, &buffer);

vkGetBufferMemoryRequirements(device, buffer, &mem_requirement);

VkMemoryAllocateInfo alloc_info = { ... };

vkAllocateMemory(device, &alloc_info, NULL, &(deviceMemory));

// 通过映射的方式将三角型数据复制到GPU侧
uint8_t *pData;
vkMapMemory(device, deviceMemory, 0, mem_requirement.size, 0, &pData);
memcpy(pData, triangleData, dataSize); /**** 拷贝数据：void *memcpy(void *dst, void *src, unsigned n); ****/
vkUnmapMemory(device, deviceMemory);

// 绑定分配后的数据
vkBindBufferMemory(device, buffer, deviceMemory, 0);

/****************  13. 顶点绑定 ****************/
VkVertexInputBindingDescription viBinding;
viBinding.binding = 0;
viBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
viBinding.stride = sizeof(triangleData) /* 数据间隔 */;

VkVertexInputAttributeDescription viAttribs[2];
viAttribs[0].binding = 0;
viAttribs[0].location = 0;
viAttribs[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
viAttribs[0].offset = 0;
viAttribs[1].binding = 0;
viAttribs[1].location = 1;
viAttribs[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
viAttribs[1].offset = 16;

/****************  14. 定义状态 ****************/
// 顶点输入状态
VkPipelineVertexInputStateCreateInfo vertexInputStateInfo	= { ... };
vertexInputStateInfo.vertexBindingDescriptionCount			= 1;
vertexInputStateInfo.pVertexBindingDescriptions				= &viBinding;
vertexInputStateInfo.vertexAttributeDescriptionCount		= 2;
vertexInputStateInfo.pVertexAttributeDescriptions			= viAttribs;

// 动态状态
VkPipelineDynamicStateCreateInfo dynamicState				= { ... };

// 输入组装状态控制结构体
VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo	= { ... };

// 光栅化状态控制结构体
VkPipelineRasterizationStateCreateInfo rasterStateInfo		= { ... };

// 颜色混合附件状态控制结构体
VkPipelineColorBlendAttachmentState colorBlendSI			= { ... };

// 颜色混合状态控制结构体
VkPipelineColorBlendStateCreateInfo colorBlendStateInfo		= { ... };

// 视口状态控制结构体
VkPipelineViewportStateCreateInfo viewportStateInfo			= { ... };

// 深度模板状态控制结构体
VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = { ... };

// 多重采样状态控制结构体
VkPipelineMultisampleStateCreateInfo   multiSampleStateInfo = { ... };

/****************  15. 创建图形管线 ****************/
VkPipelineCache	pipelineCache; 
VkPipelineCacheCreateInfo pipelineCacheInfo;
vkCreatePipelineCache(device, &pipelineCacheInfo, NULL, &pipelineCache);

// 定义图形管线的控制参数结构体
VkGraphicsPipelineCreateInfo pipelineInfo;
pipelineInfo.layout					= pipelineLayout;
pipelineInfo.pVertexInputState		= &vertexInputStateInfo;
pipelineInfo.pInputAssemblyState	= &inputAssemblyInfo;
pipelineInfo.pRasterizationState	= &rasterStateInfo;
pipelineInfo.pColorBlendState		= &colorBlendStateInfo;
pipelineInfo.pMultisampleState		= &multiSampleStateInfo;
pipelineInfo.pDynamicState			= &dynamicState;
pipelineInfo.pViewportState			= &viewportStateInfo;
pipelineInfo.pDepthStencilState		= &depthStencilStateInfo;
pipelineInfo.pStages				= shaderStages;
pipelineInfo.stageCount				= 2;
pipelineInfo.renderPass				= renderPass;

// 创建图像管线
vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, NULL, &pipeline);

/****************  16. 获取渲染图像 ****************/

// 定义图像获取操作的同步信号量
// 只有绘制完成后，我们才可以获取绘制图像
VkSemaphore imageAcquiredSemaphore ;
VkSemaphoreCreateInfo imageAcquiredSemaphore CreateInfo = { ... };
imageAcquiredSemaphore CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
vkCreateSemaphore(device, &imageAcquiredSemaphoreCreateInfo,
					NULL, &imageAcquiredSemaphore );

// 获取下一个可用的交换链图像的索引号
vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, 
						imageAcquiredSemaphore , NULL, &currentSwapChainImageIndex);

/****************  17. 准备渲染通道的控制结构体 ****************/

// 定义清屏颜色以及深度模板值
const VkClearValue clearValues[2] = {
	[0] = { .color.float32 = { 0.2f, 0.2f, 0.2f, 0.2f } },
	[1] = { .depthStencil = { 1.0f, 0 } },
};

// 帧缓存的渲染通道执行数据结构体
VkRenderPassBeginInfo renderPassBegin;
renderPassBegin.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
renderPassBegin.pNext		= NULL;
renderPassBegin.renderPass	= renderPass;
renderPassBegin.framebuffer = framebuffers[currentSwapchainImageIndex];
renderPassBegin.renderArea.offset.x			= 0;
renderPassBegin.renderArea.offset.y			= 0;
renderPassBegin.renderArea.extent.width		= width;
renderPassBegin.renderArea.extent.height	= height;
renderPassBegin.clearValueCount				= 2;
renderPassBegin.pClearValues				= clearValues;

		
/****************  18. 执行渲染通道 ****************/
/**** 开始渲染通道 ****/
vkCmdBeginRenderPass(cmd, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE); 
		
// 绑定图像管线
vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

// 绑定三角形缓存数据
const VkDeviceSize offsets[1] = { 0 };
vkCmdBindVertexBuffers(cmd, 0, 1, &buffer, offsets);

// 设置视口
vkCmdSetViewport(cmd, 0, NUM_VIEWPORTS, &viewport);

// 设置裁切器
vkCmdSetScissor(cmd, 0, NUM_SCISSORS, &scissor);

// 绘制指令--3个顶点，1个实例，起始索引号0
vkCmdDraw(cmd, 3, 1, 0, 0);

/**** 结束渲染通道 ****/
vkCmdEndRenderPass(cmd);

// 设置交换链的图像布局
setImageLayout()
vkCmdPipelineBarrier(cmd, ....);

/**** 结束指令缓存的录制 ****/
vkEndCommandBuffer(cmd);

/****************  19. 队列提交 ****************/

VkFenceCreateInfo fenceInfo = { ... }; VkFence drawFence;
// 创建指令屏障对象，以确保指令缓存的处理已经完成
vkCreateFence(device, &fenceInfo, NULL, &drawFence);

// 给指令缓存发送结构体赋值
VkSubmitInfo submitInfo[1]			= { ... };
submitInfo[0].pNext					= NULL;
submitInfo[0].sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
submitInfo[0].pWaitSemaphores		= &imageAcquiredSemaphore ;
submitInfo[0].commandBufferCount	= 1;
submitInfo[0].pCommandBuffers		= &cmd;

// 将指令缓存发送至队列中执行
vkQueueSubmit(queue, 1, submitInfo, NULL);

/****************  20. 使用显式层进行显示 ****************/

// 定义显示的控制结构体
VkPresentInfoKHR present = { ... };
present.sType			 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
present.pNext			 = NULL;
present.swapchainCount	 = 1;
present.pSwapchains		 = &swapChain;
present.pImageIndices	 = &swapChainObjCurrent_buffer;

// 检查是不是之前发送的指令缓存都已经执行完毕
do {
	res = vkWaitForFences(device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
} while (res == VK_TIMEOUT);

// 将当前的交换链图像切换到显示队列，准备展示到输出设备
vkQueuePresentKHR(queue, &present);

// 删除同步对象
vkDestroySemaphore(device, imageAcquiredSemaphore , NULL);
vkDestroyFence(device, drawFence, NULL);
