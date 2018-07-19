#include "Engine/VulkanRenderer/VKPipeline.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/Core/Vertex.hpp"
#include "Engine/VulkanRenderer/VKShaderStage.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard Includes

//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
VKPipeline::VKPipeline(VKRenderer* renderer)
{
	m_renderer = renderer;
	Initialize();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKPipeline::~VKPipeline()
{
	DestroyPipeline();
	DestroyPipelineLayout();
}

//-----------------------------------------------------------------------------------------------
// Initializes all the data structures
//
void VKPipeline::Initialize()
{
	m_pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	m_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

	m_pipelineLayoutInfo.pSetLayouts = 0;
	m_pipelineLayoutInfo.pPushConstantRanges = nullptr;
	m_pipelineLayoutInfo.pSetLayouts = nullptr;
	m_pipelineLayoutInfo.pushConstantRangeCount = 0;

// 	// Vulkan can support dynamic state changes but very limited 
// 	VkDynamicState dynamicStates[] = {
// 		VK_DYNAMIC_STATE_VIEWPORT,
// 		VK_DYNAMIC_STATE_LINE_WIDTH
// 	};
// 	m_dynamicStateInfo.dynamicStateCount = 2;
// 	m_dynamicStateInfo.pDynamicStates = dynamicStates;

	// Rasterizer defaults
	m_rasterStateInfo.depthClampEnable = VK_FALSE; // If true, verts farther than clip planes are clamped to depth
	m_rasterStateInfo.rasterizerDiscardEnable = VK_FALSE; // If true, geometry does not pass through rasterizer stage
	m_rasterStateInfo.depthBiasEnable = VK_FALSE; // For z-fighting 
	m_rasterStateInfo.depthBiasConstantFactor = 0.f;
	m_rasterStateInfo.depthBiasClamp = 0.f;
	m_rasterStateInfo.depthBiasSlopeFactor = 0.f;

	// Setup blend create info struct
	m_blendingInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	m_blendingInfo.blendEnable = VK_TRUE;
	m_colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
	m_colorBlendStateInfo.logicOpEnable = VK_FALSE;
	m_colorBlendStateInfo.attachmentCount = 1;
	m_colorBlendStateInfo.pAttachments = &m_blendingInfo;
	m_colorBlendStateInfo.blendConstants[0] = 0.f;
	m_colorBlendStateInfo.blendConstants[1] = 0.f;
	m_colorBlendStateInfo.blendConstants[2] = 0.f;
	m_colorBlendStateInfo.blendConstants[3] = 0.f;

	// Setup multisampling struct
	m_multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	m_multisamplingInfo.sampleShadingEnable = VK_FALSE;
	m_multisamplingInfo.minSampleShading = 1.f;
	m_multisamplingInfo.pSampleMask = nullptr;
	m_multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
	m_multisamplingInfo.alphaToOneEnable = VK_FALSE;

}

//-----------------------------------------------------------------------------------------------
// Sets the vertex layout data
//
void VKPipeline::SetVertexLayout(const VertexLayout& layout)
{
	m_vertexBindingInfo.binding = 0;
	m_vertexBindingInfo.stride = layout.m_stride;
	m_vertexBindingInfo.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per vertex change. For instance VK_VERTEX_INPUT_RATE_INSTANCE

	size_t attribCount = layout.m_attributes.size();
	m_attribInfos.clear();
	m_attribInfos.resize(attribCount);
	for(int index = 0; index < attribCount; ++index)
	{
		const VertexAttribute* attrib = layout.m_attributes[index];

		VkVertexInputAttributeDescription attribInfo = {};
		attribInfo.binding = 0;
		attribInfo.location = index;
		attribInfo.format = GetVKDataType(attrib->m_vkType);
		attribInfo.offset = (uint32_t) attrib->m_memberOffset;

		m_attribInfos[index] = attribInfo;
	}

	// Setup the Vertex input state -> Holds vertex descriptions and attributes
	m_vertexInputInfo.vertexBindingDescriptionCount = 1;
	m_vertexInputInfo.pVertexBindingDescriptions = &m_vertexBindingInfo;
	m_vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t) attribCount;
	m_vertexInputInfo.pVertexAttributeDescriptions = m_attribInfos.data();
}

//-----------------------------------------------------------------------------------------------
// Sets the shader stages
//
void VKPipeline::SetShaderStages(const std::vector<VKShaderStage*>& stages)
{
	m_shaderStages.clear();

	for(VKShaderStage* stage : stages)
	{
		VkPipelineShaderStageCreateInfo stageInfo = {};
		stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stageInfo.stage = GetVKShaderStageFlag(stage->m_stage);
		stageInfo.module = (VkShaderModule) stage->GetShaderModule();
		stageInfo.pName = "main";

		m_shaderStages.push_back(stageInfo);
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the draw topology
//
void VKPipeline::SetDrawType(DrawPrimitiveType type)
{
	m_inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	m_inputAssemblyInfo.topology = GetVKDrawType(type);
}

//-----------------------------------------------------------------------------------------------
// Setup the viewport
//
void VKPipeline::SetViewport(const AABB2& extent, float minDepth, float maxDepth)
{
	m_viewport.width = extent.maxs.x;
	m_viewport.height = extent.maxs.y;
	m_viewport.x = extent.mins.x;
	m_viewport.y = extent.mins.y;
	m_viewport.minDepth = minDepth;
	m_viewport.maxDepth = maxDepth;

	SetScissorRect(extent);

	m_viewportInfo.viewportCount = 1;
	m_viewportInfo.pViewports = &m_viewport;
	m_viewportInfo.pScissors = &m_scissorRect;
	m_viewportInfo.scissorCount = 1;
}

//-----------------------------------------------------------------------------------------------
// Setup the scissor rectangle -> Clipping rectangle for the rasterizer
//
void VKPipeline::SetScissorRect(const AABB2& extent)
{
	VkExtent2D scissorExtent;
	scissorExtent.height = (uint32_t) extent.maxs.y;
	scissorExtent.width = (uint32_t) extent.maxs.x;

	m_scissorRect.extent = scissorExtent;
	m_scissorRect.offset = {(int) extent.mins.x, (int) extent.mins.y};

	m_viewportInfo.pScissors = &m_scissorRect;
	m_viewportInfo.scissorCount = 1;
}

//-----------------------------------------------------------------------------------------------
// Sets the render state enums
//
void VKPipeline::SetRenderState(const RenderState& state)
{
	SetCullMode(state.m_cullMode);
	SetFillMode(state.m_fillMode);
	SetWindOrder(state.m_frontFace);

	SetDepthState(state.m_depthCompare, state.m_depthWrite);

	SetColorBlending(state.m_colorBlendOp, state.m_colorSrcFactor, state.m_colorDstFactor);
	SetAlphaBlending(state.m_alphaBlendOp, state.m_alphaSrcFactor, state.m_alphaDstFactor);
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode (PolygonMode)
//
void VKPipeline::SetFillMode(FillMode mode)
{
	m_rasterStateInfo.polygonMode = GetVKPolygonMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Set the culling mode
//
void VKPipeline::SetCullMode(CullMode mode)
{
	m_rasterStateInfo.cullMode = GetVKCullMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Sets the winding order 
//
void VKPipeline::SetWindOrder(WindOrder order)
{
	m_rasterStateInfo.frontFace = GetVKWindOrder(order);
}

//-----------------------------------------------------------------------------------------------
// Sets the depth state
//
void VKPipeline::SetDepthState(DepthTestOp compare, bool shouldWrite)
{
	m_depthStencilInfo.depthTestEnable = VK_TRUE;
	m_depthStencilInfo.depthCompareOp = GetVKDepthOp(compare);
	m_depthStencilInfo.depthWriteEnable = shouldWrite;
}

//-----------------------------------------------------------------------------------------------
// Sets the color blending state
//
void VKPipeline::SetColorBlending(BlendOp op, BlendFactor sFactor, BlendFactor dFactor)
{
	m_blendingInfo.colorBlendOp = GetVKBlendOp(op);
	m_blendingInfo.srcColorBlendFactor = GetVKBlendFactor(sFactor);
	m_blendingInfo.dstColorBlendFactor = GetVKBlendFactor(dFactor);
}

//-----------------------------------------------------------------------------------------------
// Setup alpha blending state
//
void VKPipeline::SetAlphaBlending(BlendOp op, BlendFactor sFactor, BlendFactor dFactor)
{
	m_blendingInfo.alphaBlendOp = GetVKBlendOp(op);
	m_blendingInfo.srcAlphaBlendFactor = GetVKBlendFactor(sFactor);
	m_blendingInfo.dstAlphaBlendFactor = GetVKBlendFactor(dFactor);
}

//-----------------------------------------------------------------------------------------------
// Sets the descriptor set layouts on the pipeline layout info
//
void VKPipeline::SetDescriptorSetLayouts(size_t count, void* layouts)
{
	m_pipelineLayoutInfo.setLayoutCount = (uint32_t) count;
	m_pipelineLayoutInfo.pSetLayouts = (VkDescriptorSetLayout*) layouts;
}

//-----------------------------------------------------------------------------------------------
// Sets the render pass
//
void VKPipeline::SetRenderPass(VkRenderPass renderPass)
{
	m_renderPass = renderPass;
}

//-----------------------------------------------------------------------------------------------
// Destroys the pipeline
//
void VKPipeline::DestroyPipeline()
{
	vkDeviceWaitIdle(m_renderer->GetLogicalDevice());

	if(m_pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(m_renderer->GetLogicalDevice(), m_pipeline, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the pipeline layout
//
void VKPipeline::DestroyPipelineLayout()
{
	if(m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_renderer->GetLogicalDevice(), m_pipelineLayout, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the pipeline layout and destroys the existing one
//
void VKPipeline::CreatePipelineLayout()
{
	DestroyPipelineLayout();

	if(vkCreatePipelineLayout(m_renderer->GetLogicalDevice(), &m_pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Could not create the pipeline layout");
	}
}

//-----------------------------------------------------------------------------------------------
// Creates the pipeline and destroys the existing one
//
void VKPipeline::UpdatePipeline()
{
	DestroyPipeline();

	CreatePipelineLayout();

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = (uint32_t) m_shaderStages.size();
	pipelineInfo.pStages = m_shaderStages.data();
	pipelineInfo.pVertexInputState = &m_vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &m_inputAssemblyInfo;
	pipelineInfo.pViewportState = &m_viewportInfo;
	pipelineInfo.pRasterizationState = &m_rasterStateInfo;
	pipelineInfo.pMultisampleState = &m_multisamplingInfo;
	pipelineInfo.pDepthStencilState = &m_depthStencilInfo;
	pipelineInfo.pColorBlendState = &m_colorBlendStateInfo;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if(vkCreateGraphicsPipelines(m_renderer->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Couldn't create the pipeline");
	}
}
