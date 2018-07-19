#include "Engine/VulkanRenderer/VKTexSampler.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#define VK_NO_PROTOTYPES
#include "Engine/VulkanRenderer/External/Vulkan/vulkan_core.h"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Static globals
static VKTexSampler* g_pointSampler = nullptr;
static VKTexSampler* g_linearSampler = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKTexSampler::VKTexSampler( VKRenderer& renderer )
	: m_samplerHandle(VK_NULL_HANDLE)
	, m_renderer(renderer)
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKTexSampler::~VKTexSampler()
{
	vkDestroySampler(m_renderer.GetLogicalDevice(), (VkSampler) m_samplerHandle, nullptr);
}

//-----------------------------------------------------------------------------------------------
// Creates a point sampler
//
void VKTexSampler::CreatePointSampler()
{
	VkSamplerCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_NEAREST;
	createInfo.minFilter = VK_FILTER_NEAREST;
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	createInfo.anisotropyEnable = VK_FALSE;
	createInfo.maxAnisotropy = 1;
	createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	createInfo.unnormalizedCoordinates = VK_FALSE;
	createInfo.compareEnable = VK_FALSE;
	createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	createInfo.mipLodBias = 0.f;
	createInfo.minLod = 0.f;
	createInfo.maxLod = 0.f;

	if(vkCreateSampler(m_renderer.GetLogicalDevice(), &createInfo, nullptr, (VkSampler*) &m_samplerHandle) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create sampler");
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a bilinear sampler
//
void VKTexSampler::CreateLinearSampler()
{
	VkSamplerCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.magFilter = VK_FILTER_LINEAR;
	createInfo.minFilter = VK_FILTER_LINEAR;
	createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	createInfo.anisotropyEnable = VK_FALSE; 
	createInfo.maxAnisotropy = 1; // Disabled anisotropy needs to be 1
	createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	createInfo.unnormalizedCoordinates = VK_FALSE;
	createInfo.compareEnable = VK_FALSE;
	createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	createInfo.mipLodBias = 0.f;
	createInfo.minLod = 0.f;
	createInfo.maxLod = 0.f;

	if(vkCreateSampler(m_renderer.GetLogicalDevice(), &createInfo, nullptr, (VkSampler*) &m_samplerHandle) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create sampler");
	}
}

//-----------------------------------------------------------------------------------------------
// Initializes the different samplers needed 
//
void VKTexSampler::InitalizeSamplers(VKRenderer& renderer)
{
	if(!g_linearSampler)
	{
		g_linearSampler = new VKTexSampler(renderer);
		g_linearSampler->CreateLinearSampler();
	}

	if(!g_pointSampler)
	{
		g_pointSampler = new VKTexSampler(renderer);
		g_pointSampler->CreatePointSampler();
	}

}

//-----------------------------------------------------------------------------------------------
// Destroys the samplers
//
void VKTexSampler::DestroySamplers()
{
	if(g_linearSampler)
	{
		delete g_linearSampler;
		g_linearSampler = nullptr;
	}

	if(g_pointSampler)
	{
		delete g_pointSampler;
		g_pointSampler = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the point sampler
//
VKTexSampler* VKTexSampler::GetPointSampler()
{
	return g_pointSampler;
}

//-----------------------------------------------------------------------------------------------
// Returns the linear sampler
//
VKTexSampler* VKTexSampler::GetLinearSampler()
{
	return g_linearSampler;
}
