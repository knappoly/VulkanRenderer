#include "Engine/VulkanRenderer/VKTexture.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/Image.hpp"
#define VK_NO_PROTOTYPES
#include "External/Vulkan/vulkan_core.h"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/VKTexSampler.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKTexture::VKTexture(VKRenderer& renderer)
	: m_renderer(renderer)
{
	m_sampler = VKTexSampler::GetPointSampler();
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKTexture::VKTexture(VKRenderer& renderer, const std::string& imageFilePath, VKTexSampler* sampler /*= nullptr */)
	: m_renderer(renderer)
	, m_dimensions(0,0)
	, m_texHandle(VK_NULL_HANDLE)
	, m_sampler(sampler)
{
	UNUSED(sampler);

	Image image(imageFilePath, true);
	LoadFromImage(image);

	if(!sampler)
	{
		m_sampler = VKTexSampler::GetPointSampler();
	}
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKTexture::VKTexture(VKRenderer& renderer, const Image& image, VKTexSampler* sampler /*= nullptr */)
	: m_renderer(renderer)
	, m_dimensions(0,0)
	, m_texHandle(VK_NULL_HANDLE)
	, m_sampler(sampler)
{
	UNUSED(sampler);
	LoadFromImage(image);

	if(!sampler)
	{
		m_sampler = VKTexSampler::GetPointSampler();
	}
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKTexture::~VKTexture()
{
	VkDevice device =  m_renderer.GetLogicalDevice();

	vkDestroyImageView(device, (VkImageView) m_viewHandle, nullptr);
	vkDestroyImage(device, (VkImage) m_texHandle, nullptr);
	vkFreeMemory(device, (VkDeviceMemory) m_memHandle, nullptr);
}

//-----------------------------------------------------------------------------------------------
// Returns the sampler handle
//
void* VKTexture::GetSamplerHandle() const
{
	return m_sampler->GetHandle();
}

//-----------------------------------------------------------------------------------------------
// Returns the image layout
//
int VKTexture::GetLayout() const
{
	return (VkImageLayout) m_imageLayout;
}

//-----------------------------------------------------------------------------------------------
// Returns the VK format type equivalent of the store format
//
int VKTexture::GetVulkanFormat() const
{
	return GetVkFormat(m_format);
}

//-----------------------------------------------------------------------------------------------
// Sets the sampler if passed or defaults to the existing sampler
//
void VKTexture::SetSampler(VKTexSampler* sampler)
{
	if(sampler)
	{
		m_sampler = sampler;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a render target for the given format and width and height
//
void VKTexture::CreateRenderTarget(int width, int height, eTextureFormat format)
{
	if(format == TEXTURE_FORMAT_D24S8)
	{
		CreateDepthTarget(width, height, format);
	}
	else
	{
		CreateColorTarget(width, height, format);
	}

	m_dimensions = IntVector2(width, height);
}

//-----------------------------------------------------------------------------------------------
// Creates a color target for the width and height
//
void VKTexture::CreateColorTarget(int width, int height, eTextureFormat format)
{
	VkFormat colorFormat = GetVkFormat(format);
	VkImage* colorTarget = (VkImage*) &m_texHandle;
	VkDeviceMemory* memHandle = (VkDeviceMemory*) &m_memHandle;

	m_renderer.CreateAndGetImage(colorTarget, memHandle, width, height, colorFormat, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_IMAGE_TILING_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_renderer.TransitionImageLayout(
		(VkImage) m_texHandle,					
		VK_IMAGE_ASPECT_COLOR_BIT,													// Aspect mask												
		VK_IMAGE_LAYOUT_UNDEFINED,													// Old Layout
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,									// New Layout
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,											// Src Pipeline stage
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// Dst Pipeline stage
		0,																			// Dont care bout source access mask
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT  // Destination access mask
	);

	m_imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	m_viewHandle = m_renderer.CreateAndGetImageView((VkImage) m_texHandle, GetVkFormat(format), VK_IMAGE_ASPECT_COLOR_BIT);
}

//-----------------------------------------------------------------------------------------------
// Creates a depth target for the width and height
//
void VKTexture::CreateDepthTarget(int width, int height, eTextureFormat format)
{
	m_format = format;
	VkFormat depthFormat = GetVkFormat(format);
	VkImage* depthTarget = (VkImage*) &m_texHandle;
	VkDeviceMemory* memHandle = (VkDeviceMemory*) &m_memHandle;
	m_renderer.CreateAndGetImage(depthTarget, memHandle, width, height, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_TILING_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_renderer.TransitionImageLayout(
		(VkImage) m_texHandle,					
		VK_IMAGE_ASPECT_STENCIL_BIT | VK_IMAGE_ASPECT_DEPTH_BIT,						// Aspect mask												
		VK_IMAGE_LAYOUT_UNDEFINED,							// Old Layout
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,	// New Layout
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,					// Src Pipeline stage
		VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,			// Dst Pipeline stage
		0,													// Dont care bout source access mask
		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT // Destination access mask
	);

	m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	m_viewHandle = m_renderer.CreateAndGetImageView((VkImage) m_texHandle, GetVkFormat(format), VK_IMAGE_ASPECT_DEPTH_BIT);
}

//-----------------------------------------------------------------------------------------------
// Creates the texture from an image
//
void VKTexture::LoadFromImage(const Image& image)
{
	int numComponents = 4;
	m_dimensions = image.GetDimensions();
	unsigned char* imageData = image.GetTexelsAsByteArray();
	
	PopulateFromData(imageData, m_dimensions, numComponents);

	free(imageData);
}

//-----------------------------------------------------------------------------------------------
// Copies the pixel data into GPU memory
//
void VKTexture::PopulateFromData(unsigned char* imageData, const IntVector2& texelSize, int numComponents)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory; 
	VkDeviceSize totalSize = texelSize.x * texelSize.y * numComponents;
	VkDevice device = m_renderer.GetLogicalDevice();
	VkFormat format = GetVkFormat(m_format);

	m_renderer.CreateAndGetBuffer(&stagingBuffer, &stagingMemory, totalSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy data to staging buffer using mapped memory
	void* mappedMemHandle;
	vkMapMemory(device, stagingMemory, 0, totalSize, 0, &mappedMemHandle);
	memcpy(mappedMemHandle, imageData, totalSize);
	vkUnmapMemory(device, stagingMemory);

	m_renderer.CreateAndGetImage((VkImage*)&m_texHandle, (VkDeviceMemory*)&m_memHandle, texelSize.x, texelSize.y, format, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_TILING_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	// Transition to a layout for copying data
	m_renderer.TransitionImageLayout(
		(VkImage) m_texHandle,					// Texture handle
		VK_IMAGE_ASPECT_COLOR_BIT,				// Aspect mask
		VK_IMAGE_LAYOUT_UNDEFINED,				// Old
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,	// New layout
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,		// Any pipeline stage 
		VK_PIPELINE_STAGE_TRANSFER_BIT,			// Specified transfer stage of the pipeline
		0,										// Don't care about the src mask coz there's nothing there
		VK_ACCESS_TRANSFER_WRITE_BIT			// Enables writing to the image
	);
	m_renderer.CopyBufferToImage(stagingBuffer, (VkImage) m_texHandle, texelSize.x, texelSize.y);

	// Get the layout ready for shader reading
	m_renderer.TransitionImageLayout(
		(VkImage) m_texHandle,						// Texture handle
		VK_IMAGE_ASPECT_COLOR_BIT,					// Aspect mask
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 		// Old layout
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,	// New layout
		VK_PIPELINE_STAGE_TRANSFER_BIT,				// Was already in Transfer stage from prev transition
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,		// Read from fragment shader stage
		VK_ACCESS_TRANSFER_WRITE_BIT,				// Was previously in write mode
		VK_ACCESS_SHADER_READ_BIT					// Goes into shader read mode
	);

	vkDestroyBuffer(m_renderer.GetLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(m_renderer.GetLogicalDevice(), stagingMemory, nullptr);

	m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	m_viewHandle = m_renderer.CreateAndGetImageView((VkImage) m_texHandle, format, VK_IMAGE_ASPECT_COLOR_BIT);
}
