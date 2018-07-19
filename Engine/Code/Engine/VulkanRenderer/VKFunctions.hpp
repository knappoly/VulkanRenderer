#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define VK_NO_PROTOTYPES
#include "External\Vulkan\vulkan_core.h"
#include "External\Vulkan\vulkan_win32.h"

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
// Vulkan Library
extern	void*	g_vkLibrary;

//-----------------------------------------------------------------------------------------------
// Global VK Functions
extern PFN_vkCreateInstance								vkCreateInstance;
extern PFN_vkEnumerateInstanceLayerProperties			vkEnumerateInstanceLayerProperties;
extern PFN_vkEnumerateInstanceExtensionProperties		vkEnumerateInstanceExtensionProperties;
extern PFN_vkGetInstanceProcAddr						vkGetInstanceProcAddr;

//-----------------------------------------------------------------------------------------------
// Instance specific VK Functions
extern PFN_vkCreateDebugReportCallbackEXT				vkCreateDebugReportCallbackEXT;
extern PFN_vkDestroyDebugReportCallbackEXT				vkDestroyDebugReportCallbackEXT;
extern PFN_vkDestroyInstance							vkDestroyInstance;
extern PFN_vkEnumeratePhysicalDevices					vkEnumeratePhysicalDevices;
extern PFN_vkGetPhysicalDeviceProperties				vkGetPhysicalDeviceProperties;
extern PFN_vkGetPhysicalDeviceQueueFamilyProperties		vkGetPhysicalDeviceQueueFamilyProperties;
extern PFN_vkGetDeviceProcAddr							vkGetDeviceProcAddr;
extern PFN_vkCreateDevice								vkCreateDevice;
extern PFN_vkCreateWin32SurfaceKHR						vkCreateWin32SurfaceKHR;
extern PFN_vkDestroySurfaceKHR							vkDestroySurfaceKHR;
extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR			vkGetPhysicalDeviceSurfaceSupportKHR;
extern PFN_vkEnumerateDeviceExtensionProperties			vkEnumerateDeviceExtensionProperties;
extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR			vkGetPhysicalDeviceSurfaceFormatsKHR;
extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR;
extern PFN_vkGetPhysicalDeviceMemoryProperties			vkGetPhysicalDeviceMemoryProperties;

//-----------------------------------------------------------------------------------------------
// Device specific VK Functions
extern PFN_vkGetDeviceQueue								vkGetDeviceQueue;
extern PFN_vkDeviceWaitIdle								vkDeviceWaitIdle;
extern PFN_vkDestroyDevice								vkDestroyDevice;
extern PFN_vkCreateSwapchainKHR							vkCreateSwapchainKHR;
extern PFN_vkDestroySwapchainKHR						vkDestroySwapchainKHR;
extern PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR;
extern PFN_vkCreateImageView							vkCreateImageView;
extern PFN_vkDestroyImageView							vkDestroyImageView;
extern PFN_vkCreateShaderModule							vkCreateShaderModule;
extern PFN_vkDestroyShaderModule						vkDestroyShaderModule;
extern PFN_vkCreatePipelineLayout						vkCreatePipelineLayout;
extern PFN_vkDestroyPipelineLayout						vkDestroyPipelineLayout;
extern PFN_vkCreateRenderPass							vkCreateRenderPass;
extern PFN_vkDestroyRenderPass							vkDestroyRenderPass;
extern PFN_vkCreateGraphicsPipelines					vkCreateGraphicsPipelines;
extern PFN_vkDestroyPipeline							vkDestroyPipeline;
extern PFN_vkCreateFramebuffer							vkCreateFramebuffer;
extern PFN_vkDestroyFramebuffer							vkDestroyFramebuffer;
extern PFN_vkCreateCommandPool							vkCreateCommandPool;
extern PFN_vkDestroyCommandPool							vkDestroyCommandPool;
extern PFN_vkAllocateCommandBuffers						vkAllocateCommandBuffers;
extern PFN_vkFreeCommandBuffers							vkFreeCommandBuffers;
extern PFN_vkBeginCommandBuffer							vkBeginCommandBuffer;
extern PFN_vkCmdBeginRenderPass							vkCmdBeginRenderPass;
extern PFN_vkCmdBindPipeline							vkCmdBindPipeline;
extern PFN_vkCmdDraw									vkCmdDraw;
extern PFN_vkCmdEndRenderPass							vkCmdEndRenderPass;
extern PFN_vkEndCommandBuffer							vkEndCommandBuffer;
extern PFN_vkCreateSemaphore							vkCreateSemaphore;
extern PFN_vkDestroySemaphore							vkDestroySemaphore;
extern PFN_vkAcquireNextImageKHR						vkAcquireNextImageKHR;
extern PFN_vkQueueSubmit								vkQueueSubmit;
extern PFN_vkQueuePresentKHR							vkQueuePresentKHR;
extern PFN_vkQueueWaitIdle								vkQueueWaitIdle;
extern PFN_vkCreateFence								vkCreateFence;
extern PFN_vkDestroyFence								vkDestroyFence;
extern PFN_vkWaitForFences								vkWaitForFences;
extern PFN_vkResetFences								vkResetFences;
extern PFN_vkCreateBuffer								vkCreateBuffer;
extern PFN_vkDestroyBuffer								vkDestroyBuffer;
extern PFN_vkGetBufferMemoryRequirements				vkGetBufferMemoryRequirements;
extern PFN_vkAllocateMemory								vkAllocateMemory;
extern PFN_vkFreeMemory									vkFreeMemory;
extern PFN_vkBindBufferMemory							vkBindBufferMemory;
extern PFN_vkMapMemory									vkMapMemory;
extern PFN_vkUnmapMemory								vkUnmapMemory;
extern PFN_vkCmdBindVertexBuffers						vkCmdBindVertexBuffers;
extern PFN_vkCmdCopyBuffer								vkCmdCopyBuffer;
extern PFN_vkCmdBindIndexBuffer							vkCmdBindIndexBuffer;
extern PFN_vkCmdDrawIndexed								vkCmdDrawIndexed;
extern PFN_vkCreateImage								vkCreateImage;
extern PFN_vkBindImageMemory							vkBindImageMemory;
extern PFN_vkCmdCopyBufferToImage						vkCmdCopyBufferToImage;
extern PFN_vkCmdPipelineBarrier							vkCmdPipelineBarrier;
extern PFN_vkGetImageMemoryRequirements					vkGetImageMemoryRequirements;
extern PFN_vkDestroyImage								vkDestroyImage;
extern PFN_vkCreateSampler								vkCreateSampler;
extern PFN_vkDestroySampler								vkDestroySampler;
extern PFN_vkCreateDescriptorSetLayout					vkCreateDescriptorSetLayout;
extern PFN_vkDestroyDescriptorSetLayout					vkDestroyDescriptorSetLayout;
extern PFN_vkCreateDescriptorPool						vkCreateDescriptorPool;
extern PFN_vkDestroyDescriptorPool						vkDestroyDescriptorPool;
extern PFN_vkAllocateDescriptorSets						vkAllocateDescriptorSets;
extern PFN_vkUpdateDescriptorSets						vkUpdateDescriptorSets;
extern PFN_vkCmdBindDescriptorSets						vkCmdBindDescriptorSets;
extern PFN_vkCmdCopyImage								vkCmdCopyImage;

//-----------------------------------------------------------------------------------------------
// Standalone functions - Specific loaders
void	LoadVulkanLibrary();
void	LoadGlobalVkFunctions();
void	LoadDeviceVkFunctions( void* vkDevice );
void	LoadInstanceVkFunctions( void* vkInstance );


