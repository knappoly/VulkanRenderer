#include "Engine/VulkanRenderer/VKFunctions.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/Core/EngineCommon.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Template for binding Global VK functions 
// 
template <typename T>
bool vkGetGlobalProcAddress( T *out, char const *name ) 
{
	// Grab the function from the global dll
	*out = (T) GetProcAddress( (HMODULE) g_vkLibrary, name); 
	return (*out != nullptr); 
}

//-----------------------------------------------------------------------------------------------
// Template for binding Device specific VK functions 
// 
template <typename T>
bool vkGetDevProcAddress( void* device, T *out, char const *name ) 
{
	// Grab the function specific to the device
	*out = (T) vkGetDeviceProcAddr((VkDevice) device, name); 

// 	if ((*out) == nullptr) {
// 		// if it is not part of the device, then attempt to get it from the VK library
// 		*out = (T) GetProcAddress( (HMODULE) g_vkLibrary, name); 
// 	}

	return (*out != nullptr); 
}

//-----------------------------------------------------------------------------------------------
// Template for binding instace specific VK functions 
// 
template <typename T>
bool vkGetInstanceProcAddress( void* instance, T *out, char const *name ) 
{
	// Grab the function specific to the device
	*out = (T) vkGetInstanceProcAddr((VkInstance) instance, name); 

// 	if ((*out) == nullptr) {
// 		// if it is not part of the instance, then attempt to get it from the VK library
// 		*out = (T) GetProcAddress( (HMODULE) g_vkLibrary, name); 
// 	}

	return (*out != nullptr); 
}

//-----------------------------------------------------------------------------------------------
// Binding Macros
#define VK_GLOBAL_BIND(f)			vkGetGlobalProcAddress( &f, #f )
#define VK_INSTANCE_BIND(inst, f)	vkGetInstanceProcAddress( inst, &f, #f )
#define VK_DEVICE_BIND(dev, f)		vkGetDevProcAddress( dev, &f, #f )

//-----------------------------------------------------------------------------------------------
// Vulkan Library
void*	   g_vkLibrary = nullptr;

//-----------------------------------------------------------------------------------------------
// Global Functions
PFN_vkCreateInstance							vkCreateInstance = nullptr;
PFN_vkEnumerateInstanceLayerProperties			vkEnumerateInstanceLayerProperties = nullptr;
PFN_vkEnumerateInstanceExtensionProperties		vkEnumerateInstanceExtensionProperties;
PFN_vkGetInstanceProcAddr						vkGetInstanceProcAddr = nullptr;

//-----------------------------------------------------------------------------------------------
// Instance specific VK Functions
PFN_vkCreateDebugReportCallbackEXT				vkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT				vkDestroyDebugReportCallbackEXT = nullptr;
PFN_vkGetDeviceProcAddr							vkGetDeviceProcAddr = nullptr;
PFN_vkEnumeratePhysicalDevices					vkEnumeratePhysicalDevices = nullptr;
PFN_vkGetPhysicalDeviceProperties				vkGetPhysicalDeviceProperties = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties	vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkDestroyInstance							vkDestroyInstance = nullptr;
PFN_vkCreateDevice								vkCreateDevice = nullptr;
PFN_vkEnumerateDeviceExtensionProperties		vkEnumerateDeviceExtensionProperties = nullptr;
PFN_vkCreateWin32SurfaceKHR						vkCreateWin32SurfaceKHR = nullptr; // Windows specific function
PFN_vkDestroySurfaceKHR							vkDestroySurfaceKHR = nullptr; 
PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR = nullptr; 
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
PFN_vkGetPhysicalDeviceMemoryProperties			vkGetPhysicalDeviceMemoryProperties = nullptr;

//-----------------------------------------------------------------------------------------------
// Device specific VK Functions
PFN_vkGetDeviceQueue							vkGetDeviceQueue = nullptr;
PFN_vkDestroyDevice								vkDestroyDevice = nullptr;
PFN_vkDeviceWaitIdle							vkDeviceWaitIdle = nullptr;
PFN_vkCreateSwapchainKHR						vkCreateSwapchainKHR = nullptr;
PFN_vkDestroySwapchainKHR						vkDestroySwapchainKHR = nullptr;
PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR = nullptr;
PFN_vkCreateImageView							vkCreateImageView = nullptr;
PFN_vkDestroyImageView							vkDestroyImageView = nullptr;
PFN_vkCreateShaderModule						vkCreateShaderModule = nullptr;
PFN_vkDestroyShaderModule						vkDestroyShaderModule = nullptr;
PFN_vkCreatePipelineLayout						vkCreatePipelineLayout = nullptr;
PFN_vkDestroyPipelineLayout						vkDestroyPipelineLayout = nullptr;
PFN_vkCreateRenderPass							vkCreateRenderPass = nullptr;
PFN_vkDestroyRenderPass							vkDestroyRenderPass = nullptr;
PFN_vkCreateGraphicsPipelines					vkCreateGraphicsPipelines = nullptr;
PFN_vkDestroyPipeline							vkDestroyPipeline = nullptr;
PFN_vkCreateFramebuffer							vkCreateFramebuffer  = nullptr;
PFN_vkDestroyFramebuffer						vkDestroyFramebuffer = nullptr;
PFN_vkCreateCommandPool							vkCreateCommandPool = nullptr;
PFN_vkDestroyCommandPool						vkDestroyCommandPool = nullptr;
PFN_vkAllocateCommandBuffers					vkAllocateCommandBuffers = nullptr;
PFN_vkFreeCommandBuffers						vkFreeCommandBuffers = nullptr;
PFN_vkBeginCommandBuffer						vkBeginCommandBuffer = nullptr;
PFN_vkCmdBeginRenderPass						vkCmdBeginRenderPass = nullptr;
PFN_vkCmdBindPipeline							vkCmdBindPipeline = nullptr;
PFN_vkCmdDraw									vkCmdDraw = nullptr;
PFN_vkCmdEndRenderPass							vkCmdEndRenderPass = nullptr;
PFN_vkEndCommandBuffer							vkEndCommandBuffer = nullptr;
PFN_vkCreateSemaphore							vkCreateSemaphore = nullptr;
PFN_vkDestroySemaphore							vkDestroySemaphore = nullptr;
PFN_vkAcquireNextImageKHR						vkAcquireNextImageKHR = nullptr;
PFN_vkQueueSubmit								vkQueueSubmit = nullptr;
PFN_vkQueuePresentKHR							vkQueuePresentKHR = nullptr;
PFN_vkQueueWaitIdle								vkQueueWaitIdle = nullptr;
PFN_vkCreateFence								vkCreateFence = nullptr;
PFN_vkDestroyFence								vkDestroyFence = nullptr;
PFN_vkResetFences								vkResetFences = nullptr;
PFN_vkWaitForFences								vkWaitForFences = nullptr;
PFN_vkCreateBuffer								vkCreateBuffer = nullptr;
PFN_vkDestroyBuffer								vkDestroyBuffer = nullptr;
PFN_vkGetBufferMemoryRequirements				vkGetBufferMemoryRequirements = nullptr;
PFN_vkAllocateMemory							vkAllocateMemory = nullptr;
PFN_vkFreeMemory								vkFreeMemory = nullptr;
PFN_vkBindBufferMemory							vkBindBufferMemory = nullptr;
PFN_vkMapMemory									vkMapMemory = nullptr;
PFN_vkUnmapMemory								vkUnmapMemory = nullptr;
PFN_vkCmdBindVertexBuffers						vkCmdBindVertexBuffers = nullptr;
PFN_vkCmdCopyBuffer								vkCmdCopyBuffer = nullptr;
PFN_vkCmdBindIndexBuffer						vkCmdBindIndexBuffer = nullptr;
PFN_vkCmdDrawIndexed							vkCmdDrawIndexed = nullptr;
PFN_vkCreateImage								vkCreateImage = nullptr;
PFN_vkBindImageMemory							vkBindImageMemory = nullptr;
PFN_vkCmdCopyBufferToImage						vkCmdCopyBufferToImage = nullptr;
PFN_vkCmdPipelineBarrier						vkCmdPipelineBarrier = nullptr;
PFN_vkGetImageMemoryRequirements				vkGetImageMemoryRequirements = nullptr;
PFN_vkDestroyImage								vkDestroyImage = nullptr;
PFN_vkCreateSampler								vkCreateSampler = nullptr;
PFN_vkDestroySampler							vkDestroySampler = nullptr;
PFN_vkCreateDescriptorSetLayout					vkCreateDescriptorSetLayout = nullptr;
PFN_vkDestroyDescriptorSetLayout				vkDestroyDescriptorSetLayout = nullptr;
PFN_vkCreateDescriptorPool						vkCreateDescriptorPool = nullptr;
PFN_vkDestroyDescriptorPool						vkDestroyDescriptorPool = nullptr;
PFN_vkAllocateDescriptorSets					vkAllocateDescriptorSets = nullptr;
PFN_vkUpdateDescriptorSets						vkUpdateDescriptorSets = nullptr;
PFN_vkCmdBindDescriptorSets						vkCmdBindDescriptorSets = nullptr;
PFN_vkCmdCopyImage								vkCmdCopyImage = nullptr;
									
//-----------------------------------------------------------------------------------------------
// Loads the vulkan library 
//
void LoadVulkanLibrary()
{
	// Load the vk library
	g_vkLibrary = ::LoadLibraryA("vulkan-1-999-0-0-0.dll");
}

//-----------------------------------------------------------------------------------------------
// Loads all the global functions
//
void LoadGlobalVkFunctions()
{
	VK_GLOBAL_BIND(vkCreateInstance);
	VK_GLOBAL_BIND(vkEnumerateInstanceLayerProperties);
	VK_GLOBAL_BIND(vkEnumerateInstanceExtensionProperties);
	VK_GLOBAL_BIND(vkGetInstanceProcAddr);
}

//-----------------------------------------------------------------------------------------------
// Loads all device specific functions 
//
void LoadDeviceVkFunctions(void* vkDevice)
{
	VK_DEVICE_BIND(vkDevice, vkGetDeviceQueue);
	VK_DEVICE_BIND(vkDevice, vkDestroyDevice);
	VK_DEVICE_BIND(vkDevice, vkDeviceWaitIdle);
	VK_DEVICE_BIND(vkDevice, vkCreateSwapchainKHR);
	VK_DEVICE_BIND(vkDevice, vkDestroySwapchainKHR);
	VK_DEVICE_BIND(vkDevice, vkGetSwapchainImagesKHR);
	VK_DEVICE_BIND(vkDevice, vkCreateImageView);
	VK_DEVICE_BIND(vkDevice, vkDestroyImageView);
	VK_DEVICE_BIND(vkDevice, vkCreateShaderModule);
	VK_DEVICE_BIND(vkDevice, vkDestroyShaderModule);
	VK_DEVICE_BIND(vkDevice, vkCreatePipelineLayout);
	VK_DEVICE_BIND(vkDevice, vkDestroyPipelineLayout);
	VK_DEVICE_BIND(vkDevice, vkCreateRenderPass);
	VK_DEVICE_BIND(vkDevice, vkDestroyRenderPass);
	VK_DEVICE_BIND(vkDevice, vkCreateGraphicsPipelines);
	VK_DEVICE_BIND(vkDevice, vkDestroyPipeline);
	VK_DEVICE_BIND(vkDevice, vkCreateFramebuffer);
	VK_DEVICE_BIND(vkDevice, vkDestroyFramebuffer);
	VK_DEVICE_BIND(vkDevice, vkCreateCommandPool);
	VK_DEVICE_BIND(vkDevice, vkDestroyCommandPool);
	VK_DEVICE_BIND(vkDevice, vkAllocateCommandBuffers);
	VK_DEVICE_BIND(vkDevice, vkFreeCommandBuffers);
	VK_DEVICE_BIND(vkDevice, vkBeginCommandBuffer);
	VK_DEVICE_BIND(vkDevice, vkCmdBeginRenderPass);
	VK_DEVICE_BIND(vkDevice, vkCmdBindPipeline);
	VK_DEVICE_BIND(vkDevice, vkCmdDraw);
	VK_DEVICE_BIND(vkDevice, vkCmdEndRenderPass);
	VK_DEVICE_BIND(vkDevice, vkEndCommandBuffer);
	VK_DEVICE_BIND(vkDevice, vkCreateSemaphore);
	VK_DEVICE_BIND(vkDevice, vkDestroySemaphore);
	VK_DEVICE_BIND(vkDevice, vkAcquireNextImageKHR);
	VK_DEVICE_BIND(vkDevice, vkQueueSubmit);
	VK_DEVICE_BIND(vkDevice, vkQueuePresentKHR);
	VK_DEVICE_BIND(vkDevice, vkQueueWaitIdle);
	VK_DEVICE_BIND(vkDevice, vkCreateFence);
	VK_DEVICE_BIND(vkDevice, vkDestroyFence);
	VK_DEVICE_BIND(vkDevice, vkResetFences);
	VK_DEVICE_BIND(vkDevice, vkWaitForFences);
	VK_DEVICE_BIND(vkDevice, vkCreateBuffer);
	VK_DEVICE_BIND(vkDevice, vkDestroyBuffer);
	VK_DEVICE_BIND(vkDevice, vkGetBufferMemoryRequirements);
	VK_DEVICE_BIND(vkDevice, vkAllocateMemory);
	VK_DEVICE_BIND(vkDevice, vkFreeMemory);
	VK_DEVICE_BIND(vkDevice, vkBindBufferMemory);
	VK_DEVICE_BIND(vkDevice, vkMapMemory);
	VK_DEVICE_BIND(vkDevice, vkUnmapMemory);
	VK_DEVICE_BIND(vkDevice, vkCmdBindVertexBuffers);
	VK_DEVICE_BIND(vkDevice, vkCmdCopyBuffer);
	VK_DEVICE_BIND(vkDevice, vkCmdBindIndexBuffer);
	VK_DEVICE_BIND(vkDevice, vkCmdDrawIndexed);
	VK_DEVICE_BIND(vkDevice, vkCreateImage);
	VK_DEVICE_BIND(vkDevice, vkBindImageMemory);
	VK_DEVICE_BIND(vkDevice, vkCmdCopyBufferToImage);
	VK_DEVICE_BIND(vkDevice, vkCmdPipelineBarrier);
	VK_DEVICE_BIND(vkDevice, vkGetImageMemoryRequirements);
	VK_DEVICE_BIND(vkDevice, vkDestroyImage);
	VK_DEVICE_BIND(vkDevice, vkCreateSampler);
	VK_DEVICE_BIND(vkDevice, vkDestroySampler);
	VK_DEVICE_BIND(vkDevice, vkCreateDescriptorSetLayout);
	VK_DEVICE_BIND(vkDevice, vkDestroyDescriptorSetLayout);
	VK_DEVICE_BIND(vkDevice, vkCreateDescriptorPool);
	VK_DEVICE_BIND(vkDevice, vkDestroyDescriptorPool);
	VK_DEVICE_BIND(vkDevice, vkAllocateDescriptorSets);
	VK_DEVICE_BIND(vkDevice, vkUpdateDescriptorSets);
	VK_DEVICE_BIND(vkDevice, vkCmdBindDescriptorSets);
	VK_DEVICE_BIND(vkDevice, vkCmdCopyImage);
}

//-----------------------------------------------------------------------------------------------
// Loads all instance specific functions
//
void LoadInstanceVkFunctions(void* vkInstance)
{
	VK_INSTANCE_BIND(vkInstance, vkCreateDebugReportCallbackEXT);
	VK_INSTANCE_BIND(vkInstance, vkDestroyDebugReportCallbackEXT);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceProperties);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceQueueFamilyProperties);
	VK_INSTANCE_BIND(vkInstance, vkEnumeratePhysicalDevices);
	VK_INSTANCE_BIND(vkInstance, vkGetDeviceProcAddr);
	VK_INSTANCE_BIND(vkInstance, vkDestroyInstance);
	VK_INSTANCE_BIND(vkInstance, vkCreateDevice);
	VK_INSTANCE_BIND(vkInstance, vkEnumerateDeviceExtensionProperties);
	VK_INSTANCE_BIND(vkInstance, vkCreateWin32SurfaceKHR);
	VK_INSTANCE_BIND(vkInstance, vkDestroySurfaceKHR);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceSurfaceSupportKHR);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceSurfaceFormatsKHR);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceSurfacePresentModesKHR);
	VK_INSTANCE_BIND(vkInstance, vkGetPhysicalDeviceMemoryProperties);
}
