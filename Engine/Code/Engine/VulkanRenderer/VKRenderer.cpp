#include "Engine/VulkanRenderer/VKRenderer.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/External/Vulkan/vulkan_win32.h"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Core/Vertex.hpp"
#include "Engine/VulkanRenderer/Buffers/VKVertexBuffer.hpp"
#include "Engine/VulkanRenderer/Buffers/VKIndexBuffer.hpp"
#include "Engine/Enumerations/TextureFormat.hpp"
#include "Engine/VulkanRenderer/VKTexture.hpp"
#include "Engine/VulkanRenderer/VKTexSampler.hpp"
#include "Engine/VulkanRenderer/VKShaderStage.hpp"
#include "Engine/VulkanRenderer/VKShaderProgram.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/VulkanRenderer/VKMaterial.hpp"
#include "Engine/VulkanRenderer/Mesh/VKMesh.hpp"
#include "Engine/VulkanRenderer/VKPipeline.hpp"
#include "Engine/VulkanRenderer/Mesh/VKMeshUtils.hpp"
#include "Engine/Enumerations/ReservedDescriptorSetSlot.hpp"
#include "Engine/VulkanRenderer/Buffers/VKUniformBuffer.hpp"
#include "Engine/VulkanRenderer/VKCamera.hpp"
#include "Engine/Enumerations/ReservedUniformBlock.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Static globals
static	VKRenderer* g_renderer = nullptr;

static	const std::vector<const char*> s_deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

#ifdef _DEBUG
static	bool	s_enableValidationLayers = true;
static	const std::vector<const char*> s_validationLayers = {
	"VK_LAYER_LUNARG_core_validation",
	"VK_LAYER_RENDERDOC_Capture"
};
static	const std::vector<const char*> s_globalExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};
#else
static	bool	s_enableValidationLayers = false;
static	const std::vector<const char*> s_validationLayers = {
// 	"VK_LAYER_LUNARG_core_validation",
// 	"VK_LAYER_RENDERDOC_Capture"
};
static	const std::vector<const char*> s_globalExtensions = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	/*VK_EXT_DEBUG_REPORT_EXTENSION_NAME*/
};
#endif

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKRenderer::VKRenderer( const char* appName )
{
	m_defaultPipeline = new VKPipeline(this);

	InitializeVulkanInstance(appName);
	SetupDebugCallback();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
	CreateCommandPool();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKRenderer::~VKRenderer()
{
	CleanupSwapchain();
	
	delete m_immediateVBO;
	m_immediateVBO = nullptr;

	delete m_immediateIBO;
	m_immediateIBO = nullptr;

	for(int index = 0; index < MAX_FRAMES_IN_FLIGHT; ++index)
	{
		vkDestroySemaphore(m_logicalDevice, m_renderFinishedSemaphore[index], nullptr);
		vkDestroySemaphore(m_logicalDevice, m_imageAvailableSemaphore[index], nullptr);
		vkDestroyFence(m_logicalDevice, m_fences[index], nullptr);
	}

	vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
	
	vkDestroyDevice(m_logicalDevice, nullptr);
	vkDestroySurfaceKHR(m_vkInstance, m_surface, nullptr);
	
	if(s_enableValidationLayers)
	{
		vkDestroyDebugReportCallbackEXT(m_vkInstance, m_debugCallback, nullptr);
	}
	vkDestroyInstance(m_vkInstance, nullptr);
}

//-----------------------------------------------------------------------------------------------
// Initializes the vulkan instance
//
void VKRenderer::InitializeVulkanInstance( const char* appName )
{
	// Check validation layer support
	if(s_enableValidationLayers)
	{
		GUARANTEE_OR_DIE(CheckValidationLayerSupport(), "Requested validation layer not supported.");
	}

	// Check Extensions support
	GUARANTEE_OR_DIE( CheckExtensionsSupport(), "Requested extensions are not supported.");

	// Create an app info struct for the instance
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	appInfo.pEngineName = ENGINE_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION(0,1,0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	// Create the instance info 
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = 0;
	createInfo.enabledExtensionCount = (uint32_t) s_globalExtensions.size();

	if(createInfo.enabledExtensionCount)
	{
		createInfo.ppEnabledExtensionNames = s_globalExtensions.data();
	}

	if(s_enableValidationLayers)
	{
		createInfo.enabledLayerCount = (uint32_t) s_validationLayers.size();
		createInfo.ppEnabledLayerNames = s_validationLayers.data();
	}

	if(vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Couldn't create Vulkan instance");
	}

	// Load all instance level functions
	LoadInstanceVkFunctions(m_vkInstance);
}

//-----------------------------------------------------------------------------------------------
// Picks the first physical device that's available on the computer
//
void VKRenderer::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

	GUARANTEE_OR_DIE(deviceCount, "No Vulkan supported GPUs found");
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

	for(VkPhysicalDevice device : devices)
	{
		if(IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			break;
		}
	}

	if(m_physicalDevice == VK_NULL_HANDLE)
	{
		GUARANTEE_OR_DIE(false, "No suitable GPU found.");
	}
}

//-----------------------------------------------------------------------------------------------
// Picks the format that's suitable for the surface
//
VkSurfaceFormatKHR VKRenderer::PickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	if(formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		// Choosing a color format if there is no preferred color format
		return {VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}
	
	for(VkSurfaceFormatKHR surfaceFormat : formats)
	{
		// Querying for particular type
		if(surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return surfaceFormat;
		}
	}

	return formats[0];
}

//-----------------------------------------------------------------------------------------------
// Picks the best present mode for the scenario
//
VkPresentModeKHR VKRenderer::PickPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
	/* Available modes 
		VK_PRESENT_MODE_IMMEDIATE
		VK_PRESENT_MODE_FIFO_KHR
		VK_PRESENT_MODE_FIFO_RELAXED_KHR
		VK_PRESENT_MODE_MAILBOX_KHR
	*/
	VkPresentModeKHR bestContigency = VK_PRESENT_MODE_FIFO_KHR;
	for(VkPresentModeKHR mode : presentModes)
	{
		// Looking for a specific type: Mailbox(triple buffering)
		if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return mode;
		}
		else if(mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestContigency = mode;
		}
	}

	return bestContigency; 
}

//-----------------------------------------------------------------------------------------------
// Pick the right resolution for the swap chain images
//
VkExtent2D VKRenderer::PickSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// Vulkan uses max value of uint32 to say that any extent is supported
	if(capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D windowExtent = {(uint32_t) Window::GetInstance()->m_width, (uint32_t) Window::GetInstance()->m_height};

		windowExtent.width = Max(capabilities.minImageExtent.width, Min(capabilities.maxImageExtent.width, windowExtent.width));
		windowExtent.height = Max(capabilities.minImageExtent.height, Min(capabilities.maxImageExtent.height, windowExtent.height));

		return windowExtent;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the swap chain with the settings previously picked
//
void VKRenderer::CreateSwapChain()
{
	// Fetching the details for the swapchain creation
	SwapChainDetails details = GetSwapChainDetails(m_physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = PickSurfaceFormat(details.formats);
	VkPresentModeKHR presentMode = PickPresentMode(details.presentModes);
	VkExtent2D extent = PickSwapExtent(details.capabilities);
	
	uint32_t imageCount = details.capabilities.minImageCount + 1;
	if(details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount)
	{
		// If max image count set to 0. The driver supports any number
		imageCount = details.capabilities.maxImageCount;
	}

	// Creating the swapchain
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.imageArrayLayers = 1; // More if application is using stereroscopic 3d
	createInfo.surface = m_surface;
	createInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.imageExtent = extent;
	createInfo.minImageCount = imageCount;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

	QueueFamilyIndices indices = GetQueueFamilyIndices(m_physicalDevice);
	uint32_t queueIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

	// Specify queue family usage over different queues
	if(indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Two different queues access the same image
		createInfo.queueFamilyIndexCount = 2; // For now 
		createInfo.pQueueFamilyIndices = queueIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Two different queues access the same image
		createInfo.queueFamilyIndexCount = 0; 
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// Setting default display orientation
	createInfo.preTransform = details.capabilities.currentTransform; // No rotation or horizontal flip

	// Set blending with other windows
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Ignores alpha over other windows
	
	// Old swapchain will be used when window is resized or changed similarly. Need to create a new swap chain in those cases
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// Actually create the swapchain
	if(vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Swapchain could not be created");
	}

	// Caching image data for later use
	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;

	// Get the swapchain image handles
	uint32_t swapChainImageCount = 0;
	vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &swapChainImageCount, nullptr);

	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &swapChainImageCount, m_swapChainImages.data());
}

//-----------------------------------------------------------------------------------------------
// Creates the image views for color targets
//
void VKRenderer::CreateImageViews()
{
	// Create the space for the views
	m_swapChainImageViews.resize(m_swapChainImages.size());

	// Create a view for each swapchain image
	for(int index = 0; index < m_swapChainImages.size(); ++index)
	{
		// Setup the struct for 2D Image
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = m_swapChainImages[index];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		// Create the view
		if(vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Could not create image view");
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the pool that manages buffer memory
//
void VKRenderer::CreateCommandPool()
{
	QueueFamilyIndices indices = GetQueueFamilyIndices(m_physicalDevice);
	
	VkCommandPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	createInfo.queueFamilyIndex = indices.graphicsFamily;
	createInfo.flags = 0;

	if(vkCreateCommandPool(m_logicalDevice, &createInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create command pool");
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the VBO for immediate drawing 
//
void VKRenderer::CreateVertexBuffer()
{
	m_immediateVBO = new VKVertexBuffer(m_logicalDevice, m_physicalDevice);
}

//-----------------------------------------------------------------------------------------------
// Creates the IBO for immediate drawing
//
void VKRenderer::CreateIndexBuffer()
{
	m_immediateIBO = new VKIndexBuffer(m_logicalDevice, m_physicalDevice);
}

//-----------------------------------------------------------------------------------------------
// Creates the semaphores for synchronization
//
void VKRenderer::CreateSyncStuff()
{
	m_imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	m_renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	m_colorTargetAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	m_fences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

 	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for(int index = 0; index < MAX_FRAMES_IN_FLIGHT; ++index)
	{
		if (vkCreateSemaphore(m_logicalDevice, &createInfo, nullptr, &m_imageAvailableSemaphore[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Image availability semaphore could not be created");
		}

		if (vkCreateSemaphore(m_logicalDevice, &createInfo, nullptr, &m_renderFinishedSemaphore[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Render finished semaphore could not be created");
		}

		if (vkCreateSemaphore(m_logicalDevice, &createInfo, nullptr, &m_colorTargetAvailableSemaphore[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Color target available semaphore could not be created");
		}

		if(vkCreateFence(m_logicalDevice, &fenceInfo, nullptr, &m_fences[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Could not create fence");
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the swapchain when the device is idle	
//
void VKRenderer::CleanupSwapchain()
{
	vkDeviceWaitIdle(m_logicalDevice);
	
	if(m_defaultPipeline)
	{
		delete m_defaultPipeline;
		m_defaultPipeline = nullptr;
	}

	for(VkImageView view : m_swapChainImageViews)
	{
		vkDestroyImageView(m_logicalDevice, view, nullptr);
	}

	vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
}

//-----------------------------------------------------------------------------------------------
// Recreates the swap chain when it is out-of-date
//
void VKRenderer::RecreateSwapchain()
{
	CleanupSwapchain();

	CreateSwapChain();
	CreateImageViews();
}
//-----------------------------------------------------------------------------------------------
// Check if the device is suitable
//
bool VKRenderer::IsDeviceSuitable(VkPhysicalDevice device)
{
	UNUSED(device);

	VkPhysicalDeviceProperties deviceProp;
	vkGetPhysicalDeviceProperties(device, &deviceProp);
	bool isDiscreteGPU = (deviceProp.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);

	QueueFamilyIndices indices = GetQueueFamilyIndices(device);

	bool extensionsSupported = CheckDeviceExtensionsSupport(device);

	bool isSwapChainSupportEnough = false;
	if(extensionsSupported) // Swapchain itself is an extension
	{
		SwapChainDetails details = GetSwapChainDetails(device);
		isSwapChainSupportEnough = !details.formats.empty() && !details.presentModes.empty();
	}

	return indices.IsComplete() && isDiscreteGPU && isSwapChainSupportEnough;
}

//-----------------------------------------------------------------------------------------------
// Checks if the device supports the requested device extensions
//
bool VKRenderer::CheckDeviceExtensionsSupport( VkPhysicalDevice device )
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for(const char* extensionName : s_deviceExtensions)
	{
		bool extensionFound = false;
		for(uint32_t index = 0; index < extensionCount; ++index)
		{
			
			if(strcmp(extensionName, availableExtensions[index].extensionName) == 0)
			{
				extensionFound = true;
				break;
			}
		}

		if(!extensionFound)
		{
			return false;
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------------------------
// Return the supported queue families and their corresponding queue index 
//
QueueFamilyIndices VKRenderer::GetQueueFamilyIndices(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

	// Get supported queue families
	VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[count];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies);

	
	for(uint32_t iterIndex = 0; iterIndex < count; ++iterIndex)
	{
		if( queueFamilies[iterIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT )
		{
			indices.graphicsFamily = iterIndex;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, iterIndex, m_surface, &presentSupport);

		if( presentSupport )
		{
			indices.presentFamily = iterIndex; 
		}

		if(indices.IsComplete())
		{
			break;
		}
	}

	delete[] queueFamilies; // cleanup

	return indices;
}

//-----------------------------------------------------------------------------------------------
// Returns the swapchain support details for the physical device specified
//
SwapChainDetails VKRenderer::GetSwapChainDetails(VkPhysicalDevice device)
{
	SwapChainDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	
	if(formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if(presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

//-----------------------------------------------------------------------------------------------
// Creates the virtual device that interfaces with the physical device
//
void VKRenderer::CreateLogicalDevice()
{
	// Get the queue indices supported by the physical device
	QueueFamilyIndices indices = GetQueueFamilyIndices(m_physicalDevice);
	
	// Multiple queues are needed
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::vector<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
	float queuePriority = 1.f;
	for( int queueIndex : uniqueQueueFamilies )
	{
		VkDeviceQueueCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		createInfo.queueFamilyIndex = queueIndex;
		createInfo.queueCount = 1;
		createInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(createInfo);
	}
	
	// Device create info takes this as a struct member to enable certain features
	VkPhysicalDeviceFeatures deviceFeatures = {};
	
	// Logical device creation info
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size();
	deviceCreateInfo.enabledExtensionCount = (uint32_t) s_deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = s_deviceExtensions.data();
	deviceCreateInfo.enabledLayerCount = 0;

	// Check if validation layers are enabled. Enable those layers on the logical device
	if(s_enableValidationLayers)
	{
		deviceCreateInfo.enabledLayerCount = (uint32_t) s_validationLayers.size();
		deviceCreateInfo.ppEnabledLayerNames = s_validationLayers.data();
	}

	// Actually create the device
	if(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "LOGICAL Device could not be created");
	}

	// Load all device level functions
	LoadDeviceVkFunctions(m_logicalDevice);

	// Get the queue handle for Graphics and Presentation
	vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, indices.presentFamily, 0, &m_presentQueue);
}

//-----------------------------------------------------------------------------------------------
// Creates the surface for Win32 platform
//
void VKRenderer::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hinstance = GetModuleHandle( NULL );
	createInfo.hwnd = (HWND) Window::GetInstance()->GetHandle();

	if( vkCreateWin32SurfaceKHR( m_vkInstance, &createInfo, nullptr, &m_surface) != VK_SUCCESS )
	{
		GUARANTEE_OR_DIE(false, "Could not create Win32 surface. ");
	}

}

//-----------------------------------------------------------------------------------------------
// Runs stuff after the renderer is initialized
//
void VKRenderer::PostStartup()
{
	m_testBuffer = VKUniformBuffer::For(this, MatrixBuffer());
	m_testBuffer->UpdateGPU();

	m_modelBuffer = VKUniformBuffer::For(this, ModelBuffer());
	m_modelBuffer->UpdateGPU();

	VKTexSampler::InitalizeSamplers(*this);

	// Create the default render targets
	m_defaultDepthTarget = new VKTexture(*this);
	m_defaultDepthTarget->CreateRenderTarget(m_swapChainExtent.width, m_swapChainExtent.height, TEXTURE_FORMAT_D24S8);
	m_defaultColorTarget = new VKTexture(*this);
	m_defaultColorTarget->CreateRenderTarget(m_swapChainExtent.width, m_swapChainExtent.height, TEXTURE_FORMAT_RGBA8);

	CreateSyncStuff();
}

//-----------------------------------------------------------------------------------------------
// Begins the frame
//
void VKRenderer::BeginFrame()
{
	vkAcquireNextImageKHR(m_logicalDevice, m_swapChain, UINT32_MAX, m_imageAvailableSemaphore[m_currentFrame], VK_NULL_HANDLE, &m_swapImageIndex);
}

//-----------------------------------------------------------------------------------------------
// Draws mesh vertices using PCU vertex layout
//
void VKRenderer::DrawMeshImmediate(const Vertex_3DPCU* vertices, int numVerts, DrawPrimitiveType mode, const Matrix44& modelMatrix)
{

	
}

//-----------------------------------------------------------------------------------------------
// Draws the mesh with the model matrix
//
void VKRenderer::DrawMesh(const VKMesh& mesh, const Matrix44& modelMatrix /*= Matrix44::IDENTITY */)
{
	const DrawInstruction& drawInstruct = mesh.m_drawInstruction;
	ModelBuffer* modelBuffer = m_modelBuffer->As<ModelBuffer>();
	modelBuffer->MODEL = modelMatrix;
	m_modelBuffer->UpdateGPU();
	

	// Sets the draw topology on the default pipeline
	m_defaultPipeline->SetDrawType(mesh.m_drawInstruction.m_drawType);
	
	// Binds the active material
	BindMaterial(m_activeMaterial);

	// Bind the mesh to the current pipeline
	BindMeshToProgram(&mesh);

	// Bind the camera buffer
	m_currentCamera->m_cameraUBO->UpdateGPU();
	BindUBO(0, m_currentCamera->m_cameraUBO);

	// Bind the model buffer
	BindUBO(1, m_modelBuffer);

	// Recreate the pipeline
	m_defaultPipeline->UpdatePipeline();

	VkExtent2D renderExtent = {};
	renderExtent.height = (uint32_t) m_currentCamera->GetViewportMaxs().y;
	renderExtent.width = (uint32_t) m_currentCamera->GetViewportMaxs().x;
	
	VkOffset2D renderOffset = {};
	renderOffset.x = (int32_t) m_currentCamera->GetViewportMins().x;
	renderOffset.y = (int32_t) m_currentCamera->GetViewportMins().y;

	VkCommandBuffer cmdBuffer = BeginTemporaryCommandBuffer();
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = (VkRenderPass) m_currentCamera->GetRenderPass();
	renderPassBeginInfo.framebuffer = (VkFramebuffer) m_currentCamera->GetFrameBufferHandle();
	renderPassBeginInfo.renderArea.extent = renderExtent;
	renderPassBeginInfo.renderArea.offset = renderOffset;

	VkClearValue clearColor = {0.f, 0.f, 0.f, 1.f};
	VkClearValue depthStencil = {1.f, 0.f};

	VkClearValue clearValues[2] = {clearColor, depthStencil};
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, (VkPipeline) m_defaultPipeline->GetPipelineHandle());

	VkBuffer vbo = (VkBuffer) mesh.m_vbo->GetBufferHandle();
	VkDeviceSize offsets[] = {0};
	VkBuffer ibo = (VkBuffer) mesh.m_ibo->GetBufferHandle();
	vkCmdBindVertexBuffers(cmdBuffer, (uint32_t) drawInstruct.m_startIndex, 1, &vbo, offsets );
	vkCmdBindIndexBuffer(cmdBuffer, ibo, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_defaultPipeline->m_pipelineLayout, 0, (uint32_t) m_activeMaterial->GetDescriptorSets().size(), (VkDescriptorSet*) m_activeMaterial->GetDescriptorSets().data(), 0, nullptr);
	
	if(drawInstruct.m_useIndices)
	{
		vkCmdDrawIndexed(cmdBuffer, mesh.m_ibo->GetIndexCount(), 1,(uint32_t) drawInstruct.m_startIndex, 0, 0);
	}
	else
	{
		vkCmdDraw(cmdBuffer, mesh.m_vbo->GetVertexCount(), 1, (uint32_t) drawInstruct.m_startIndex, 0);
	}
	vkCmdEndRenderPass(cmdBuffer);

	vkEndCommandBuffer(cmdBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmdBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &m_renderFinishedSemaphore[m_currentFrame];

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE); // For now no fences
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &cmdBuffer);

}

//-----------------------------------------------------------------------------------------------
// Creates a mesh or gets the existing instance of a mesh
//
VKMesh* VKRenderer::CreateOrGetMesh(const std::string& path)
{
	if(m_loadedMeshes.find(path) != m_loadedMeshes.end())
	{
		return m_loadedMeshes.at(path);
	}
	else
	{
		VKMesh* mesh = new VKMesh(this);
		mesh->FromFile<Vertex_3DPCU>(path.c_str());
		m_loadedMeshes[path] = mesh;
		return mesh;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates default meshes like sphere, cube, plane and stores it in the loaded meshes
//
void VKRenderer::InitializeDefaultMeshes()
{
	VKMesh* cube = CreateCube(Vector3::ZERO, Vector3::ONE);
	m_loadedMeshes["Cube"] = cube;

	VKMesh* sphere = CreateUVSphere(Vector3::ZERO, 1.f, 25, 25);
	m_loadedMeshes["Sphere"] = sphere;

	VKMesh* plane = CreateQuad3D(Vector2::ZERO, Vector3::RIGHT, Vector3::UP, Vector2::ONE);
	m_loadedMeshes["Quad"] = plane;
}

//-----------------------------------------------------------------------------------------------
// Begins a temporary command buffer recording and returns the handle
//
VkCommandBuffer VKRenderer::BeginTemporaryCommandBuffer()
{
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = m_commandPool;
	allocateInfo.commandBufferCount = 1;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkCommandBuffer tempCmdBuffer;
	if(vkAllocateCommandBuffers(m_logicalDevice, &allocateInfo, &tempCmdBuffer) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Cannot allocate temporary command buffer");
	}

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(tempCmdBuffer, &beginInfo);

	return tempCmdBuffer;
}

//-----------------------------------------------------------------------------------------------
// Ends the temp buffer recording, submits the buffer and frees the command buffer
//
void VKRenderer::EndTemporaryCommandBuffer(VkCommandBuffer tempBuffer)
{
	vkEndCommandBuffer(tempBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &tempBuffer;

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE); // For now no fences
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &tempBuffer);
}

//-----------------------------------------------------------------------------------------------
// Creates an image and returns the handle to it
//
void VKRenderer::CreateAndGetImage(VkImage* out_image, VkDeviceMemory* out_devMem, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImageTiling tiling, VkMemoryPropertyFlags props)
{
	VkImage image;
	VkDeviceMemory memory;
	
	VkImageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.extent.width = width;
	createInfo.extent.height = height;
	createInfo.extent.depth = 1;
	createInfo.arrayLayers = 1;
	createInfo.mipLevels = 1;
	createInfo.format = format;
	createInfo.tiling = tiling;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	createInfo.usage = usage;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT; // Not multisampling

	if(vkCreateImage(m_logicalDevice, &createInfo, nullptr, &image) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Can't create the image");
	}

	VkMemoryRequirements memoryReq = {};
	vkGetImageMemoryRequirements(m_logicalDevice, image, &memoryReq);

	VkMemoryAllocateInfo allocationInfo = {};
	allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocationInfo.allocationSize = memoryReq.size;
	allocationInfo.memoryTypeIndex = FindMemoryType(memoryReq.memoryTypeBits, props);

	if(vkAllocateMemory( m_logicalDevice, &allocationInfo, nullptr, &memory) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Cannot allocate memory for the image");
	}

	vkBindImageMemory(m_logicalDevice, image, memory, 0);

	*out_devMem = memory; 
	*out_image = image;
}

//-----------------------------------------------------------------------------------------------
// Creates and returns a image view handle
//
VkImageView VKRenderer::CreateAndGetImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.image = image;
	createInfo.format = format;
	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.layerCount = 1;
	createInfo.subresourceRange.levelCount = 1;

	VkImageView view;
	if(vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &view) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create image view");
	}

	return view;
}

//-----------------------------------------------------------------------------------------------
// Transitions the image between image layouts 
//
void VKRenderer::TransitionImageLayout(VkImage image, VkImageAspectFlags aspectFlags, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkAccessFlags srcMask, VkAccessFlags dstMask)
{
	VkCommandBuffer tempBuffer = BeginTemporaryCommandBuffer();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = aspectFlags;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = srcMask;
	barrier.dstAccessMask = dstMask;

	vkCmdPipelineBarrier(
		tempBuffer,				// Command buffer
		srcStage, dstStage,		// SrcStage and DstStage Mask
		0,						// Dependency flags
		0, nullptr,				// Memory barrier count and barriers
		0, nullptr,				// Buffer memory barrier count and barriers
		1U, &barrier			// Image barrier count and barriers
	);			

	EndTemporaryCommandBuffer(tempBuffer);
}

//-----------------------------------------------------------------------------------------------
// Copies the data from buffer into an image
//
void VKRenderer::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer tempBuffer = BeginTemporaryCommandBuffer();

	VkBufferImageCopy copyInfo = {};
	copyInfo.bufferOffset = 0;
	copyInfo.bufferRowLength = 0;
	copyInfo.bufferImageHeight = 0;
	copyInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyInfo.imageSubresource.baseArrayLayer = 0;
	copyInfo.imageSubresource.layerCount = 1;
	copyInfo.imageSubresource.mipLevel = 0;
	copyInfo.imageOffset = {0,0,0};
	copyInfo.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(tempBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);

	EndTemporaryCommandBuffer(tempBuffer);
}

//-----------------------------------------------------------------------------------------------
// Copies one image into another using the copy info
//
void VKRenderer::CopyImages(VkImage dst, VkImageLayout dstLayout, VkImage src, VkImageLayout srcLayout, VkImageCopy copyInfo, VkSemaphore* waitSemaphores, uint32_t waitCount, VkSemaphore* signalSemaphores, uint32_t signalCount)
{
	VkCommandBuffer tempBuffer = BeginTemporaryCommandBuffer();

	VkPipelineStageFlags waitStages[2] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &tempBuffer;
	submitInfo.signalSemaphoreCount = signalCount;
	submitInfo.pSignalSemaphores = signalSemaphores;
	submitInfo.waitSemaphoreCount = waitCount;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	vkCmdCopyImage(
		tempBuffer, 
		src, srcLayout,
		dst, dstLayout,
		1,	&copyInfo
	);

	vkEndCommandBuffer(tempBuffer);

	vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphicsQueue);

	vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &tempBuffer);
}

//-----------------------------------------------------------------------------------------------
// Sets the texture on the default material
//
void VKRenderer::SetTexture(VKTexture* texture)
{
	if(!texture)
	{
		SetTexture(0, m_defaultTexture);
	}

	SetTexture(0, texture);
}

//-----------------------------------------------------------------------------------------------
// Sets the texture on the default material
//
void VKRenderer::SetTexture(unsigned int index, VKTexture* texture, VKTexSampler* sampler /*= nullptr */)
{
	m_defaultMaterial->SetTexture(index, texture, sampler);
}

//-----------------------------------------------------------------------------------------------
// Binds the texture to the active material's descriptor set
//
void VKRenderer::BindTexture2D(const VKTexture* texture)
{
	if(texture == nullptr)
	{
		BindTexture2D(0, m_defaultTexture);
		return;
	}

	BindTexture2D(0U, texture);
}

//-----------------------------------------------------------------------------------------------
// Binds the texture to the active material's descriptor set
//
void VKRenderer::BindTexture2D(unsigned int index, const VKTexture* texture)
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = (VkImageView) texture->GetImageViewHandle();
	imageInfo.sampler = (VkSampler) texture->GetSamplerHandle();

	VkWriteDescriptorSet texWrite = {};
	texWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	texWrite.dstBinding = index;
	texWrite.dstArrayElement = 0;
	texWrite.dstSet = (VkDescriptorSet) m_activeMaterial->GetDescriptorSets()[RESERVED_SLOT_COMBINED_IMAGE_SAMPLER];
	texWrite.descriptorCount = 1;
	texWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	texWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(m_logicalDevice, 1, &texWrite, 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
// Returns the texture id
//
VKTexture* VKRenderer::CreateOrGetTexture(const std::string& path, bool genMipmaps /*= true*/)
{
	TODO("MIPMAPPING");
	UNUSED(genMipmaps);

	if(m_loadedTextures.find(path) != m_loadedTextures.end())
	{
		return m_loadedTextures.at(path);
	}
	else
	{
		VKTexture* newTexture = new VKTexture(*this, path, nullptr);
		m_loadedTextures[path] = newTexture;
		return newTexture;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns texture id 
//
VKTexture* VKRenderer::CreateOrGetTexture(const Image& image, bool genMipmaps /*= true*/)
{
	TODO("MIPMAPPING");
	UNUSED(genMipmaps);

	if(m_loadedTextures.find(image.GetPath()) != m_loadedTextures.end())
	{
		return m_loadedTextures.at(image.GetPath());
	}
	else
	{
		VKTexture* newTexture = new VKTexture(*this, image, nullptr);
		m_loadedTextures[image.GetPath()] = newTexture;
		return newTexture;
	}
}

//-----------------------------------------------------------------------------------------------
// Checks if texture is already loaded
//
bool VKRenderer::IsTextureLoaded(const std::string& path) const
{
	if(m_loadedTextures.find(path) != m_loadedTextures.end())
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
// Sets the default texture to the default material slot 0
//
void VKRenderer::SetDefaultTexture()
{
	SetTexture(m_defaultTexture);
}

//-----------------------------------------------------------------------------------------------
// Copies from src texture in the destination using frame buffers
//
void VKRenderer::CopyTexture2D(VKTexture* dst, VKTexture* src, VkSemaphore* waitSemaphores, uint32_t waitCount, VkSemaphore* signalSemaphores, uint32_t signalCount)
{
	IntVector2 dimensions = src->GetDimensions();
	VkExtent3D extent = {(uint32_t) dimensions.x, (uint32_t) dimensions.y, 1};

	VkImageCopy copyInfo = {};
	copyInfo.dstOffset = {0,0,0};
	copyInfo.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyInfo.dstSubresource.baseArrayLayer = 0;
	copyInfo.dstSubresource.layerCount = 1;
	copyInfo.dstSubresource.mipLevel = 1; 
	copyInfo.srcOffset = {0,0,0};
	copyInfo.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	copyInfo.srcSubresource.baseArrayLayer = 0;
	copyInfo.srcSubresource.layerCount = 1;
	copyInfo.srcSubresource.mipLevel = 1;
	copyInfo.extent = extent;

	CopyImages(
	(VkImage) dst->GetHandle(), (VkImageLayout) dst->GetLayout(),			// Dst Image and Layout
	(VkImage) src->GetHandle(), (VkImageLayout) src->GetLayout(),			// Src image and layout
	copyInfo,																// Copy region
	waitSemaphores, waitCount,												// waitSemaphores
	signalSemaphores, signalCount											// Signal semaphores
	);

}

//-----------------------------------------------------------------------------------------------
// Creates a render target 
//
VKTexture* VKRenderer::CreateRenderTarget(unsigned int width, unsigned int height, eTextureFormat fmt /*= TEXTURE_FORMAT_RGBA8*/)
{
	VKTexture* target = new VKTexture(*this);
	target->CreateRenderTarget(width, height, fmt);
	return target;
}

//-----------------------------------------------------------------------------------------------
// Creates a depth stencil target
//
VKTexture* VKRenderer::CreateDepthStencilTarget(unsigned int width, unsigned int height)
{
	return CreateRenderTarget(width, height, TEXTURE_FORMAT_D24S8);
}

//-----------------------------------------------------------------------------------------------
// Creates a color target
//
VKTexture* VKRenderer::CreateColorTarget(unsigned int width, unsigned int height)
{
	return CreateRenderTarget(width, height, TEXTURE_FORMAT_RGBA8);
}

//-----------------------------------------------------------------------------------------------
// Sets the shader to the default material
//
void VKRenderer::SetShader(VKShader* shader /*= nullptr */)
{
	if(shader == nullptr)
	{
		SetDefaultShader();
	}

	else
	{
		m_defaultMaterial->SetShader(shader);
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the camera if specified, else default camera
//
void VKRenderer::SetCamera(VKCamera* cam)
{
	if(cam == nullptr)
	{
		cam = m_defaultCamera;
	}
	IntVector2 viewMins = IntVector2(cam->GetViewportMins());
	IntVector2 viewMaxs = IntVector2(cam->GetViewportMaxs());
	m_defaultPipeline->SetViewport(cam->GetViewportExtents());

	cam->Finalize();
	m_currentCamera = cam;
	m_defaultPipeline->SetRenderPass( (VkRenderPass) cam->GetRenderPass());
}

//-----------------------------------------------------------------------------------------------
// Finds the memory type index 
//
uint32_t VKRenderer::FindMemoryType(uint32_t requiredTypes, uint32_t requiredProps) const
{
	VkPhysicalDeviceMemoryProperties memProps = {};
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProps);

	for(uint32_t index = 0; index < memProps.memoryTypeCount; ++index)
	{
		if((requiredTypes & 1 << index) && ((memProps.memoryTypes[index].propertyFlags & requiredProps) == requiredProps))
		{
			return index;
		}
	}

	GUARANTEE_OR_DIE(false, "Can't find suitable memory type");
	return UINT32_MAX;
}

//-----------------------------------------------------------------------------------------------
// Creates the buffer and allocates the memory to it
//
void VKRenderer::CreateAndGetBuffer(VkBuffer* out_buffer, VkDeviceMemory* out_deviceMem, VkDeviceSize byteCount, VkBufferUsageFlags usage, VkMemoryPropertyFlags props)
{
	VkBuffer buffer; 
	VkDeviceMemory memory; 

	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.usage = usage;
	createInfo.size = byteCount;

	if(vkCreateBuffer(m_logicalDevice, &createInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Cannot create the buffer");
	}

	VkMemoryRequirements memoryReq = {};
	vkGetBufferMemoryRequirements(m_logicalDevice, buffer, &memoryReq);

	VkMemoryAllocateInfo allocationInfo = {};
	allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocationInfo.allocationSize = memoryReq.size;
	allocationInfo.memoryTypeIndex = FindMemoryType(memoryReq.memoryTypeBits, props);

	if(vkAllocateMemory( m_logicalDevice, &allocationInfo, nullptr, &memory) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Cannot allocate memory for the buffer");
	}

	vkBindBufferMemory(m_logicalDevice, buffer, memory, 0);
	
	*out_buffer = buffer;
	*out_deviceMem = memory;
}

//-----------------------------------------------------------------------------------------------
// Creates a temporary command buffer and submits a copy command to copy buffers
//
void VKRenderer::CopyBuffers(VkBuffer dstBuffer, VkBuffer srcBuffer, VkDeviceSize byteCount) 
{
	VkCommandBuffer tempCmdBuffer = BeginTemporaryCommandBuffer();
	
	VkBufferCopy copyInfo = {};
	copyInfo.size = byteCount;

	vkCmdCopyBuffer(tempCmdBuffer, srcBuffer, dstBuffer, 1, &copyInfo);
	
	EndTemporaryCommandBuffer(tempCmdBuffer);
}

//-----------------------------------------------------------------------------------------------
// Creates or Gets the shader program
//
VKShaderProgram* VKRenderer::CreateOrGetShaderProgram(const std::string& path, const char* defines /*= nullptr */)
{
	if(path.compare("default") == 0)
	{
		//return g_defaultProgram; // Return default shader when requested
		return nullptr;
	}

	else if(path.compare("diffuse") == 0)
	{
		//return g_defDiffuseProgram;
		return nullptr;
	}
	else
	{
		VKShaderProgram* newShader = new VKShaderProgram(this);
		bool status = newShader->LoadFromFiles(path.c_str(), defines);
		if (status)
		{
			m_loadedShaderPrograms[path] = newShader;
			return newShader;
		}
		else
		{
			m_loadedShaderPrograms[path] = nullptr; // Puts it in the map so that it can be reloaded with the key
			delete newShader;
			//return g_invalidProgram;		 // Returns invalid shader when there is an error 
			return nullptr;
		}
	}
}
	
//-----------------------------------------------------------------------------------------------
// Uses the shader program to draw
//
void VKRenderer::UseShaderProgram(const VKShaderProgram* shaderProgram)
{
	m_defaultPipeline->SetShaderStages(shaderProgram->GetActiveModules());
	m_defaultPipeline->SetDescriptorSetLayouts( shaderProgram->GetDescSetLayouts().size(), (void*) shaderProgram->GetDescSetLayouts().data());
}

//-----------------------------------------------------------------------------------------------
// Sets the default shader to the default material
//
void VKRenderer::SetDefaultShader()
{
	m_defaultMaterial->SetShader(m_defaultShader);
}

//-----------------------------------------------------------------------------------------------
// Sets the vertex layout to the pipeline
//
void VKRenderer::BindMeshToProgram(const VKMesh* mesh)
{
	m_defaultPipeline->SetVertexLayout(*mesh->GetLayout());
}

//-----------------------------------------------------------------------------------------------
// Binds the render state on the pipeline
//
void VKRenderer::BindRenderState(RenderState state)
{
	m_defaultPipeline->SetRenderState(state);
}

//-----------------------------------------------------------------------------------------------
// Sets the alpha blend state on the default material
//
void VKRenderer::AlphaBlendFunction(BlendFactor sfactor, BlendFactor dfactor)
{
	m_defaultShader->SetAlphaBlending(BLEND_OP_ADD, sfactor, dfactor);
	m_defaultMaterial->SetAlphaBlending(BLEND_OP_ADD, sfactor, dfactor);
}

//-----------------------------------------------------------------------------------------------
// Sets the color blend state on the default material
//
void VKRenderer::ColorBlendFunction(BlendFactor sfactor, BlendFactor dfactor)
{
	m_defaultShader->SetColorBlending(BLEND_OP_ADD, sfactor, dfactor);
	m_defaultMaterial->SetColorBlending(BLEND_OP_ADD, sfactor, dfactor);
}

//-----------------------------------------------------------------------------------------------
// Sets the depth test mode on the default material
//
void VKRenderer::SetDepthTestMode(DepthTestOp mode, bool flag)
{
	m_defaultShader->SetDepthTest(mode, flag);
	m_defaultMaterial->SetDepthTest(mode, flag);
}

//-----------------------------------------------------------------------------------------------
// Disables depth on the default material 
//
void VKRenderer::DisableDepth()
{
	SetDepthTestMode(COMPARE_ALWAYS, false);
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode on the default material 
//
void VKRenderer::SetFillMode(FillMode fillMode)
{
	m_defaultShader->SetFillMode(fillMode);
	m_defaultMaterial->SetFillMode(fillMode);
}

//-----------------------------------------------------------------------------------------------
// Sets the cull mode on the default material
//
void VKRenderer::SetCullMode(CullMode cullMode)
{
	m_defaultShader->SetCullMode(cullMode);
	m_defaultMaterial->SetCullMode(cullMode);
}

//-----------------------------------------------------------------------------------------------
// Binds the material and its properties, textures, etc
//
void VKRenderer::BindMaterial(const VKMaterial* material /*= nullptr */)
{
	// Bind the shader
	UseShaderProgram(material->GetShader()->GetProgram());
	BindRenderState(material->GetShader()->m_renderState);


	// Bind the textures and samplers on the material
	for(size_t texIdx = 0; texIdx < material->m_textures.size(); ++texIdx)
	{
		BindTexture2D((int) texIdx, material->m_textures[texIdx]);
	}

// 	Bind the material properties
// 		std::map<std::string, MaterialProperty*>::const_iterator iter = material->m_properties.begin();
// 		for(; iter != material->m_properties.end(); ++iter )
// 		{
// 			iter->second->Bind( material->GetShader()->GetProgram()->GetHandle() );
// 		}

	// Specular block
// 	m_specularBlock->specFactor = material->m_specFactor;
// 	m_specularBlock->specPower = material->m_specPower;
}

//-----------------------------------------------------------------------------------------------
// Sets the active material
//
void VKRenderer::SetMaterial(const VKMaterial* material /*= nullptr */)
{
	m_activeMaterial = const_cast<VKMaterial*>(material);
}

//-----------------------------------------------------------------------------------------------
// Creates or returns the material 
//
VKMaterial* VKRenderer::CreateOrGetMaterial(const std::string& path)
{
	if(m_loadedMaterials.find(path) != m_loadedMaterials.end())
	{
		return m_loadedMaterials.at(path);
	}
	else
	{
		VKMaterial* newMaterial = new VKMaterial(this, path);
		m_loadedMaterials[path] = newMaterial;
		return newMaterial;
	}
}

//-----------------------------------------------------------------------------------------------
// Binds the default material 
//
void VKRenderer::SetDefaultMaterial()
{
	m_activeMaterial = m_defaultMaterial;
}

//-----------------------------------------------------------------------------------------------
// Resets the default material 
//
void VKRenderer::ResetDefaultMaterial()
{
	delete m_defaultMaterial;
	m_defaultMaterial = m_defaultMaterialShared->Clone();
}

//-----------------------------------------------------------------------------------------------
// Binds a Uniform Buffer to the active material
//
void VKRenderer::BindUBO(int bindPoint, const VKUniformBuffer* ubo)
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = (VkBuffer) ubo->GetBufferHandle();
	bufferInfo.offset = 0;
	bufferInfo.range = ubo->GetSize();

	VkWriteDescriptorSet uboWrite = {};
	uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	uboWrite.dstBinding = bindPoint;
	uboWrite.dstArrayElement = 0;
	uboWrite.dstSet = (VkDescriptorSet) m_activeMaterial->GetDescriptorSets()[RESERVED_SLOT_UNIFORM_BUFFER];
	uboWrite.descriptorCount = 1;
	uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboWrite.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(m_logicalDevice, 1, &uboWrite, 0, nullptr);
}

//-----------------------------------------------------------------------------------------------
// End of frame
//
void VKRenderer::EndFrame()
{
	IntVector2 dimensions = m_defaultColorTarget->GetDimensions();
	VkExtent3D extent = {(uint32_t) dimensions.x, (uint32_t) dimensions.y, 1};
	VkImageCopy swapCopyInfo = {};
	swapCopyInfo.dstOffset = {0,0,0};
	swapCopyInfo.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	swapCopyInfo.dstSubresource.baseArrayLayer = 0;
	swapCopyInfo.dstSubresource.layerCount = 1;
	swapCopyInfo.dstSubresource.mipLevel = 0;
	swapCopyInfo.srcOffset = {0,0,0};
	swapCopyInfo.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	swapCopyInfo.srcSubresource.baseArrayLayer = 0;
	swapCopyInfo.srcSubresource.layerCount = 1;
	swapCopyInfo.srcSubresource.mipLevel = 0;
	swapCopyInfo.extent = extent;

	TransitionImageLayout(
		m_swapChainImages[m_swapImageIndex], VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, VK_ACCESS_TRANSFER_WRITE_BIT
		);

	TransitionImageLayout(
		(VkImage) m_defaultColorTarget->GetHandle(), VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, VK_ACCESS_TRANSFER_READ_BIT
		);

	VkSemaphore waitSemaphores[2] = {m_imageAvailableSemaphore[m_currentFrame], m_renderFinishedSemaphore[m_currentFrame]};
	CopyImages(
		m_swapChainImages[m_swapImageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		(VkImage) m_defaultColorTarget->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		swapCopyInfo,
		waitSemaphores, 2,
		&m_colorTargetAvailableSemaphore[m_currentFrame], 1
	);

	TransitionImageLayout(
		m_swapChainImages[m_swapImageIndex], VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT,
		VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_HOST_READ_BIT
	);

	TransitionImageLayout(
		(VkImage) m_defaultColorTarget->GetHandle(), VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
	);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_colorTargetAvailableSemaphore[m_currentFrame];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_swapChain;
	presentInfo.pImageIndices = &m_swapImageIndex;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(m_presentQueue, &presentInfo);

	m_currentFrame = (m_currentFrame+1) % MAX_FRAMES_IN_FLIGHT;

}

//-----------------------------------------------------------------------------------------------
// Creates the VkRenderer Instance 
//
VKRenderer* VKRenderer::CreateInstance( const char* appName )
{
	if(!g_renderer)
	{
		g_renderer = new VKRenderer( appName );
	}

	return g_renderer;
}

//-----------------------------------------------------------------------------------------------
// Destroys the VkRenderer instance
//
void VKRenderer::DestroyInstance()
{
	if(g_renderer)
	{
		delete g_renderer;
		g_renderer = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the instance if exists
//
VKRenderer* VKRenderer::GetInstance()
{
	return g_renderer;
}

//-----------------------------------------------------------------------------------------------
// Debug callback when validation is enabled
//
VKAPI_ATTR VkBool32 VKAPI_CALL VKRenderer::DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData)
{
	UNUSED(flags);
	UNUSED(objType);
	UNUSED(obj);
	UNUSED(location);
	UNUSED(code);
	UNUSED(layerPrefix);
	UNUSED(userData);
	DebuggerPrintf("\n\nValidation layer: %s\n\n", msg);
	return VK_FALSE;
}

//-----------------------------------------------------------------------------------------------
// Checks if requested validation layers are supported
//
bool VKRenderer::CheckValidationLayerSupport()
{
	// Get the number of layers supported
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr); 

	// Get the actual layers that are supported
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); 

	// Iterate through requested validation layers
	for(const char* layerName : s_validationLayers)
	{
		// Check availability of requested layer
		bool layerFound = false; // To support multiple validation layers
		for(const VkLayerProperties& layerProp : availableLayers)
		{
			if(strcmp(layerProp.layerName, layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
		{
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------------------------
// Checks if requested extensions are supported by the platform
//
bool VKRenderer::CheckExtensionsSupport()
{
	// Get the number of extensions supported
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); 

	// Get the actual extensions that are supported
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()); 

	// Iterate through requested extensions layers
	for(const char* extensionName : s_globalExtensions)
	{
		// Check availability of requested extensions
		bool extensionFound = false; // To support multiple extensions layers
		for(const VkExtensionProperties& extensionProp : availableExtensions)
		{
			if(strcmp(extensionProp.extensionName, extensionName) == 0)
			{
				extensionFound = true;
				break;
			}
		}

		if(!extensionFound)
		{
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------------------------
// Sets up the debug callback when validation layers are enabled
//
void VKRenderer::SetupDebugCallback()
{
	if(!s_enableValidationLayers) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = DebugCallback;

	if(vkCreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_debugCallback) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create debug callback");
	}
}

//-----------------------------------------------------------------------------------------------
// Startup the vk renderer 
//
void VkRenderStartup()
{
	LoadVulkanLibrary();

	// Load the global vk functions
	LoadGlobalVkFunctions();
}

//-----------------------------------------------------------------------------------------------
// Shutdown the vk renderer instance
//
void VkShutdown()
{
	::FreeLibrary( (HMODULE) g_vkLibrary ); 
}

//-----------------------------------------------------------------------------------------------
// Get the Vulkan data type from RenderType
//
VkFormat GetVKDataType(VKRenderType type)
{
	switch (type)
	{
	case VKRT_INT:				return VK_FORMAT_R32_SINT;
	case VKRT_FLOAT:			return VK_FORMAT_R32_SFLOAT;
	case VKRT_UNSIGNED_INT:		return VK_FORMAT_R32_UINT;
	case VKRT_UNSIGNED_BYTE:	return VK_FORMAT_R8_UINT;
	case VKRT_VECTOR2:			return VK_FORMAT_R32G32_SFLOAT;
	case VKRT_VECTOR3:			return VK_FORMAT_R32G32B32_SFLOAT;
	case VKRT_VECTOR4:			return VK_FORMAT_R32G32B32A32_SFLOAT;
	default:
		GUARANTEE_OR_DIE(false, "Bad data type. Can't covert to vulkan data type");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Get the vulkan format from eTextureFormat
//
VkFormat GetVkFormat(eTextureFormat format)
{
	switch (format)
	{
	
	case TEXTURE_FORMAT_RGBA8:		return VK_FORMAT_R8G8B8A8_UNORM;
	case TEXTURE_FORMAT_RGB8:		return VK_FORMAT_R8G8B8_UNORM;
	case TEXTURE_FORMAT_D24S8:		return VK_FORMAT_D24_UNORM_S8_UINT;
	case TEXTURE_FORMAT_RGBA32:		return VK_FORMAT_R32G32B32A32_SFLOAT;
	case TEXTURE_FORMAT_UNKNOWN:

	default: 
		GUARANTEE_OR_DIE(false, "Bad texture format"); return VK_FORMAT_UNDEFINED;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the vulkan enum for shader stage
//
VkShaderStageFlagBits GetVKShaderStageFlag(ShaderStageSlot stage)
{
	switch (stage)
	{
	case SHADER_STAGE_VERTEX:					return VK_SHADER_STAGE_VERTEX_BIT;
	case SHADER_STAGE_TESSELLATION_CONTROL:		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	case SHADER_STAGE_TESSELLATION_EVALUATION:	return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	case SHADER_STAGE_GEOMETRY:					return VK_SHADER_STAGE_GEOMETRY_BIT;
	case SHADER_STAGE_FRAGMENT:					return VK_SHADER_STAGE_FRAGMENT_BIT;
	case SHADER_STAGE_COMPUTE:					return VK_SHADER_STAGE_COMPUTE_BIT;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported shader stage");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for draw type
//
VkPrimitiveTopology GetVKDrawType(DrawPrimitiveType type)
{
	switch (type)
	{
	case PRIMITIVE_LINES:			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case PRIMITIVE_TRIANGLES:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case PRIMITIVE_TRIANGLE_STRIP:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	case PRIMITIVE_POINT:			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported topology");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for fillmode -> polygon mode
//
VkPolygonMode GetVKPolygonMode(FillMode mode)
{
	switch (mode)
	{
	case FILLMODE_SOLID:	return VK_POLYGON_MODE_FILL;
	case FILLMODE_WIRE:		return VK_POLYGON_MODE_LINE;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported fill mode ");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the vkenum for cull mode
//
VkCullModeFlags GetVKCullMode(CullMode mode)
{
	switch (mode)
	{
	case CULLMODE_BACK:		return VK_CULL_MODE_BACK_BIT;
	case CULLMODE_FRONT:	return VK_CULL_MODE_FRONT_BIT;
	case CULLMODE_NONE:		return VK_CULL_MODE_NONE;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported cull mode");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for wind order -> front face
//
VkFrontFace GetVKWindOrder(WindOrder order)
{
	switch (order)
	{
	case WIND_CLOCKWISE:			return VK_FRONT_FACE_CLOCKWISE;
	case WIND_COUNTER_CLOCKWISE:	return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported wind order");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for depth test op
//
VkCompareOp GetVKDepthOp(DepthTestOp compare)
{
	switch (compare)
	{
	case COMPARE_LESS:		return VK_COMPARE_OP_LESS;
	case COMPARE_GREATER:	return VK_COMPARE_OP_GREATER;
	case COMPARE_GEQUAL:	return VK_COMPARE_OP_GREATER_OR_EQUAL;
	case COMPARE_LEQUAL:	return VK_COMPARE_OP_LESS_OR_EQUAL;
	case COMPARE_NOTEQUAL:	return VK_COMPARE_OP_NOT_EQUAL;
	case COMPARE_NEVER:		return VK_COMPARE_OP_NEVER;
	case COMPARE_ALWAYS:	return VK_COMPARE_OP_ALWAYS;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported depth test mode");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for blend op
//
VkBlendOp GetVKBlendOp(BlendOp op)
{
	switch (op)
	{
	case BLEND_OP_ADD:				return VK_BLEND_OP_ADD;
	case BLEND_OP_SUBTRACT:			return VK_BLEND_OP_SUBTRACT;
	case BLEND_OP_REVERSE_SUBTRACT:	return VK_BLEND_OP_REVERSE_SUBTRACT;
	case BLEND_OP_MIN:				return VK_BLEND_OP_MIN;
	case BLEND_OP_MAX:				return VK_BLEND_OP_MAX;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported blend operation");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the VK enum for blend factor
//
VkBlendFactor GetVKBlendFactor(BlendFactor factor)
{
	switch (factor)
	{
	case BLEND_ZERO:						return VK_BLEND_FACTOR_ZERO;
	case BLEND_ONE:							return VK_BLEND_FACTOR_ONE;
	case BLEND_ONE_MINUS_SRC_ALPHA:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	case BLEND_SRC_ALPHA:					return VK_BLEND_FACTOR_SRC_ALPHA;
	case BLEND_SRC_COLOR:					return VK_BLEND_FACTOR_SRC_COLOR;
	case BLEND_ONE_MINUS_SRC_COLOR:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
	case BLEND_DST_ALPHA:					return VK_BLEND_FACTOR_DST_ALPHA;
	case BLEND_ONE_MINUS_DST_ALPHA:			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
	case BLEND_DST_COLOR:					return VK_BLEND_FACTOR_DST_COLOR;
	case BLEND_ONE_MINUS_DST_COLOR:			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
	case BLEND_CONSTANT_COLOR:				return VK_BLEND_FACTOR_CONSTANT_COLOR;
	case BLEND_ONE_MINUS_CONSTANT_COLOR:	return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
	default:
		GUARANTEE_OR_DIE(false, "Unsupported blend factor");
		break;
	}
}
