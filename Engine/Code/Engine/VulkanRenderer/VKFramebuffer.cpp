#include "Engine/VulkanRenderer/VKFramebuffer.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKTexture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
VKFramebuffer::VKFramebuffer(VKRenderer* renderer)
{
	m_renderer = renderer;
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKFramebuffer::~VKFramebuffer()
{
	DestroyRenderPass();
	DestroyFramebuffer();
}

//-----------------------------------------------------------------------------------------------
// Sets the color target on the framebuffer
//
void VKFramebuffer::SetColorTarget(VKTexture* colorTarget)
{
	m_colorTarget = colorTarget;
	m_isDirty = true;

	IntVector2 dimensions = colorTarget->GetDimensions();
	m_width = dimensions.x;
	m_height = dimensions.y;
}

//-----------------------------------------------------------------------------------------------
// Sets the depth stencil target on the framebuffer
//
void VKFramebuffer::SetDepthStencilTarget(VKTexture* depthTarget)
{
	m_depthStencilTarget = depthTarget;
	m_isDirty = true;

	IntVector2 dimensions = depthTarget->GetDimensions();
	m_width = dimensions.x;
	m_height = dimensions.y;
}

//-----------------------------------------------------------------------------------------------
// Creates the render pass for the framebuffer
//
void VKFramebuffer::CreateRenderPass()
{
	DestroyRenderPass(); // Cleanup the render pass if its already existing

	VkAttachmentDescription colorAttachment = {};
	VkAttachmentReference colorAttachRef = {};
	VkAttachmentDescription depthAttachment = {};
	VkAttachmentReference depthAttachRef = {};
	std::vector<VkAttachmentDescription> attachments;

	if(m_colorTarget)
	{
		// Attachment description
		colorAttachment.format = (VkFormat) m_colorTarget->GetVulkanFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // This is for multisampling
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Attachment reference
		colorAttachRef.attachment = 0; // Only one attachment so index is 0
		colorAttachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Add to attachments list
		attachments.push_back(colorAttachment);
	}

	if(m_depthStencilTarget)
	{
		// Attachment description
		depthAttachment.format = (VkFormat) m_depthStencilTarget->GetVulkanFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT; // This is for multisampling
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Attachment reference
		depthAttachRef.attachment = 1;
		depthAttachRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Add to attachments list
		attachments.push_back(depthAttachment);
	}

	VkSubpassDescription subPassDesc = {};
	subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPassDesc.colorAttachmentCount = 1; 
	subPassDesc.pColorAttachments = m_colorTarget ? &colorAttachRef : nullptr;
	subPassDesc.pDepthStencilAttachment = m_depthStencilTarget ? &depthAttachRef : nullptr;

	VkSubpassDependency subPassDependency = {};
	subPassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subPassDependency.dstSubpass = 0;
	subPassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subPassDependency.srcAccessMask = 0;
	subPassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subPassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

// 	VkSubpassDependency depthPassDependency = {};
// 	depthPassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
// 	depthPassDependency.dstSubpass = 0;
// 	depthPassDependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
// 	depthPassDependency.srcAccessMask = 0;
// 	depthPassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 	depthPassDependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

//	VkSubpassDependency dependencies[2] = {subPassDependency, depthPassDependency};

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = (uint32_t) attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subPassDesc;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &subPassDependency;

	if(vkCreateRenderPass(m_renderer->GetLogicalDevice(), &renderPassInfo, nullptr, (VkRenderPass*) &m_renderPass) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Couldn't create render pass");
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the render pass
//
void VKFramebuffer::DestroyRenderPass()
{
	if(m_renderPass)
	{
		vkDestroyRenderPass(m_renderer->GetLogicalDevice(), (VkRenderPass) m_renderPass, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the framebuffer
//
void VKFramebuffer::DestroyFramebuffer()
{
	if(m_handle)
	{
		vkDestroyFramebuffer(m_renderer->GetLogicalDevice(), (VkFramebuffer) m_handle, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Finalizes the framebuffer with the color target and the depth target specified
//
void VKFramebuffer::Finalize()
{
	if(m_isDirty)
	{
		CreateRenderPass();
		DestroyFramebuffer(); // Destroy the framebuffer first and create the new one when targets change

		std::vector<VkImageView> attachmentViews;
		IntVector2 dimensions;

		if (m_colorTarget)
		{
			dimensions = m_colorTarget->GetDimensions();
			attachmentViews.push_back((VkImageView)m_colorTarget->GetImageViewHandle());
		}

		if (m_depthStencilTarget)
		{
			dimensions = m_colorTarget->GetDimensions();
			attachmentViews.push_back((VkImageView)m_depthStencilTarget->GetImageViewHandle());
		}

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.attachmentCount = (uint32_t)attachmentViews.size();
		createInfo.pAttachments = attachmentViews.data();
		createInfo.renderPass = (VkRenderPass)m_renderPass;
		createInfo.width = (uint32_t)dimensions.x;
		createInfo.height = (uint32_t)dimensions.y;
		createInfo.layers = 1;

		if (vkCreateFramebuffer(m_renderer->GetLogicalDevice(), &createInfo, nullptr, (VkFramebuffer*)&m_handle) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Could not create framebuffer");
		}
	}
}
