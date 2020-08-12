#
# Chapter 3 - 连接硬件设备.
#

cmake_minimum_required(VERSION 3.10.2)

set(Recipe_Name "3_0_DeviceHandshake")

# AUTO_LOCATE_VULKAN - 可用值是ON/OFF
# ON  - 使用CMake自动查找 Vulkan SDK。
# OFF - 手动设置 Vulkan SDK 的路径。
option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if(AUTO_LOCATE_VULKAN)
	message(STATUS "Attempting auto locate Vulkan using CMake......")
	
	# 使用CMake自动查找 Vulkan SDK
	# 自动返回一个布尔量 'Vulkan_FOUND'，它表示成功找到(ON)或者失败(OFF).
	# 头文件路径 - 'Vulkan_INCLUDE_DIRS' 
	# 库文件路径 - 'Vulkan_LIBRARY' 
	find_package(Vulkan)
	
	# 尝试从 ${Vulkan_INCLUDE_DIRS} 抽取 VulkanSDK路径
	if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
	endif()
		 
	if(NOT Vulkan_FOUND)
		# CMake可能不能自动定位 VulkanSDK 路径 
		# 但是，我们可以提供一些 VulkanSDk的文件位置来确定 'Vulkan_INCLUDE_DIRS'
		# 进而抽取正确的路径
		message(STATUS "Failed to locate Vulkan SDK, retrying again...")
		if(EXISTS "${VULKAN_PATH}")
			message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
		else()
			message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
			message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
			return()
		endif()
	endif()
else()
	message(STATUS "Attempting to locate Vulkan SDK using manual path......")
	set(VULKAN_SDK "C:/VulkanSDK/1.2.141.2")
	set(VULKAN_VERSION "1.2.141.2")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using manual specified path: ${VULKAN_PATH}")

	# 检查用户是否手动设置了路径
	if(NOT EXISTS "${VULKAN_PATH}")
		message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
	   return()
	endif()
endif()

# 设置一个合适的工程名
project(${Recipe_Name})

# 添加预编译宏
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

# 添加 ‘vulkan-1’ 库文件类编译Vulkan应用程序
set(VULKAN_LIB_LIST "vulkan-1")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# 包含 VulkanSDK 头文件路径
	include_directories(AFTER ${VULKAN_PATH}/Include)

	# 设置链接库 vulkan-1 所在目录
	link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
endif()


# 将头文件和源代码分组管理
if(WIN32)
    source_group("include" REGULAR_EXPRESSION "include/*")
    source_group("source" REGULAR_EXPRESSION "source/*")
endif(WIN32)

# 定义头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 收集所有需要编译的头文件和源代码文件
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.*)

# 构建工程，需要设置一个名字，以及提供所需要编译的文件列表
add_executable(${Recipe_Name} ${CPP_FILES} ${HPP_FILES})

# 将工程所需要的debug和release依赖库链接进来
target_link_libraries( ${Recipe_Name} ${VULKAN_LIB_LIST} )

# 定义工程属性
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set_property(TARGET ${Recipe_Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

# 定义工程中使用的C++版本
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${Recipe_Name} PROPERTY CXX_STANDARD_REQUIRED ON)

# 定义工程中C使用的版本
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD 99)
set_property(TARGET ${Recipe_Name} PROPERTY C_STANDARD_REQUIRED ON)
