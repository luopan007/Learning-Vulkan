# Learning Vulkan：《Vulkan 学习指南》

<img src="./images/LearningVulkan.PNG" alt="Learning Vulkan" height="256px" align="right">

This is the code repository for [Learning Vulkan](https://www.packtpub.com/application-development/learning-vulkan?utm_source=github&utm_medium=repository&utm_campaign=9781786469809), published by [Packt](www.packtpub.com). It contains all the supporting project files necessary to work through the book from start to finish.

这个代码仓是用于《Vulkan 学习指南》。

## About the Book：关于本书
Vulkan, the next generation graphics and compute API, is the latest offering by Khronos. This API is the successor of OpenGL and in contrast offers greatflexibility and high performance capabilities to control modern GPU devices. With this book, you'll get great insights into the working of Vulkan and how you can make stunning graphics run with minimum hardware requirements.

Vulkan作为下一代图形和计算的API接口，同时也是KHRonos组织最新的产品。这个API作为OpenGL的下一代产品，提供了更好的灵活性和高性能，可以控制GPU设备。借助本书，您将获得有关Vulkan工作原理的深刻见解，以及如何以最低的硬件开销获得满意的图像。

## Instructions and Navigations：说明和导航
All of the code is organized into folders. Each folder starts with a number followed by the application name. For example, Chapter02.

所有代码都处于独立的文件夹中。每个文件夹名都以章节名开头，数字紧跟。例如：Chapter02。


The code will look like the following:（代码看起来像会是这样：）
```
foreach layerProperty{
	VkExtensionProperties *instanceExtensions;
	res = vkEnumerateInstanceExtensionProperties(layer_name,
	&instanceExtensionCount, instanceExtensions);
}
```
Please follow through the hardware and software requirements provided with this book. The reader must have a decent knowledge of C/C++ programming. Coding experience is required.
[Click here](https://www.packtpub.com/sites/default/files/downloads/LearningVulkan_ColorImages.pdf) to get the colored images.

请遵循本书的硬件和软件要求：读者需要对C++编程有经验。

## 编译
```
 cmake.exe -G "Visual Studio 15 2017 Win64" ..
```
## Preface：前言

### Chapter 1, Getting Started with the NextGen 3D Graphics API：开始学习新一代3D图形API

This chapter begins with the fundamentals of the Vulkan API and provides an overview of all its distinct features compared to its predecessor OpenGL API. This chapter will cover the basics, concepts, application model, and technical jargon used in Vulkan programming that is extremely helpful for first-time learners. You will also walk through the Vulkan programming model and see an outline of each module and its role.

本章从Vulkan API基础知识开始，讲述其与OpenGL的区别。本章会涵盖基础知识和概念，应用模型和专业术语，这些对初学者非常有用。您将学习Vulkan的编程模型，了解其作用。

### Chapter 2, Your First Vulkan Pseudo Program：你的第一个Vulkan伪代码

This chapter will help you program a simple Hello World program using a pseudocode approach. This will help the beginners to get a flavor of Vulkan programming and learn the step-by-step process to build their first Vulkan application. You will also learn how to install the necessary software and the SDK.

本章使用伪代码编写Vulkan中的Hello World程序。这也将帮助初学者熟悉Vulkan编程，并逐步学习如何构建第一个Vulkan应用程序。还要学会如何安装SDK和必要软件包依赖。

### Chapter 3, Shaking Hands with the Device：与硬件设备握手

Will help you to set up the programming environment to start with building your very first Vulkan example. You will create the Vulkan instance and initialize the program. You will connect with the physical hardware device, explore different types of queues exposed by it, and query various available layers and extensions. This chapter will provide a detailed understanding of the device queue and queue family concept and its relation with logical devices.

设置环境变量，开始构建第一个Vulkan程序。您建初始化Vulkan实例，然后初始化程序。连接物理设备（GPU显卡设备），查询设备所支持的队列、可用层和扩展。本章将详细解释设备队列和队列族概念及其逻辑设备的关系。

### Chapter 4, Debugging in Vulkan：调试Vulkan程序

This chapter describes how to perform debugging in a Vulkan application. Vulkan allows debugging through validation layers. In this chapter, we will discuss the role of each validation layer and program a simple example to understand the debugging in action. In addition, we will also query the layer extensions to add extra features that may not be a part of the Vulkan specifications.

### Chapter 5, Command Buffer and Memory Management in Vulkan：Vulkan中的指令缓存和内存管理

As we approach in the middle, we will thoroughly discuss and implement command buffers in Vulkan. You will understand the role of the command pool and will learn how to record command buffers in Vulkan. The second half of the chapter will cover memory management in Vulkan; you will dig through device memory, and learn methods to allocate or deallocate GPU memory and understand the mapping of CPU and GPU memory.

### Chapter 6, Allocating Image Resources and Building a Swapchain with WSI：图像资源分配和交换链构建

Here we will shed light on image resources and discuss memory management concepts, such as image creation, allocation, binding, and mapping. Using this, we will create a depth image for depth testing. This chapter will also introduce the WSI swapchain, which is used for presentation and renders the drawing output onscreen. We will acquire the swapchain color images and create image views that will be used for drawing primitives.

### Chapter 7, Buffer Resource, Render Pass, Frame Buffer, and Shaders with SPIR-V：缓存资源、渲染通道、帧缓存和SPIR-V着色器

This chapter will discuss the buffer resource and its usage for implementing the vertex buffer containing a drawing object’s geometry information. This chapter will give a detailed introduction to using the Render Pass to define a single unit of work specifying drawing operations using various attachments and subpasses. We will use Render Pass and implement frame buffers in Vulkan and demonstrate simple example to clear the background. As the chapter closes, we will implement our first shader in Vulkan using SPIR-V; we learn about SDK tools that convert GLSL into SPIR-V intermediate representation.

### Chapter 8, Pipelines and Pipeline State Management：流水线和流水线状态管理

Will introduce Vulkan’s compute and graphics pipeline. This chapter will provide an overview of the graphic pipeline flow and cover the role of various modules from start to end. We will discuss pipeline state objects, pipeline cache objects, and pipeline layouts. This chapter will cover all the pipeline states thoroughly, also covering dynamics states, input assembly with drawing primitives, rasterization, blending, viewport, depth/stencil testing, and multisampling. We will use these state's objects and implement the graphics pipeline.

### Chapter 9, Drawing Objects：绘制对象

In this chapter, we will thoroughly understand the process of drawing objects in Vulkan. We will record and execute the drawing object command buffers. The recording associates the Render Pass, framebuffer, and pipeline together along with the viewport and geometry data. The command buffer execution involves the submission of the command buffer to the device queue and presenting the drawn swapchain image to the presentation engine. We will also discuss the Vulkan synchronization mechanisms and understand fences, semaphore, and memory barriers. Besides, we will also cover drawing APIs and demonstrate it through some easy-to-follow examples.

### Chapter 10, Descriptors and Push Constant：资源描述符和推送常数

This chapter describes, how to update shader resources from a Vulkan application using descriptors and push constants. In descriptors, we will discuss and create descriptor pools and descriptor set layout. You will learn how to use the pipeline layouts and use the descriptors to update the buffer resource residing on the device memory and render the updated geometry on screen. Unlike descriptors, push constant does not use the command buffer and provides an optimized path to update the resources. You will implement a small example to understand push constants in action.

### Chapter 11, Drawing Textures：绘制纹理

Will bring realism to our rendered 3D drawing object by adding textures. You will learn how to create the image resource and apply samplers to it. You will also learn how to apply textures using linear and optimal tiling. In optimal tiling implementation, you will learn to transfer buffer and image memory through staging.

## Related Products：相关产品
* [Vulkan Cookbook](https://www.packtpub.com/game-development/vulkan-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781786468154)

* [OpenGL 4 Shading Language Cookbook, Second Edition](https://www.packtpub.com/game-development/opengl-4-shading-language-cookbook-second-edition?utm_source=github&utm_medium=repository&utm_campaign=9781782167020)

* [OpenGL Development Cookbook](https://www.packtpub.com/game-development/opengl-development-cookbook?utm_source=github&utm_medium=repository&utm_campaign=9781849695046)

## Other books by the author：作者的其他书籍
* [OpenGLES 3.0 Cookbook](https://www.packtpub.com/application-development/opengl-es-30-cookbook)

### Suggestions and Feedback：建议与反馈
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSe5qwunkGf6PUvzPirPDtuy1Du5Rlzew23UBp2S-P3wB-GcwQ/viewform) if you have any feedback or suggestions.
