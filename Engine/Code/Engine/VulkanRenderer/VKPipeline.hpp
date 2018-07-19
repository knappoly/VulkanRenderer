#pragma once
#define VK_NO_PROTOTYPES
#include "External/Vulkan/vulkan_core.h"
#include "Engine/Enumerations/DrawPrimitiveType.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Enumerations/FillMode.hpp"
#include "Engine/Enumerations/CullMode.hpp"
#include "Engine/Enumerations/WindOrder.hpp"
#include "Engine/Enumerations/BlendOp.hpp"
#include "Engine/Enumerations/BlendFactor.hpp"
#include "Engine/Structures/RenderState.hpp"
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;
struct VertexLayout;
class VKShaderStage;

//-----------------------------------------------------------------------------------------------
class VKPipeline
{
	friend class VKRenderer;

private:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKPipeline( VKRenderer* renderer );
	~VKPipeline();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	VkPipeline	GetPipelineHandle() { return m_pipeline; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods

	// Pipeline object management
	void	Initialize();
	void	DestroyPipeline();
	void	DestroyPipelineLayout();
	void	CreatePipelineLayout();
	void	UpdatePipeline();

	// Pipeline state helpers
	void	SetVertexLayout( const VertexLayout& layout );
	void	SetShaderStages( const std::vector<VKShaderStage*>& stages );
	void	SetDrawType( DrawPrimitiveType type );
	void	SetViewport( const AABB2& extent, float minDepth = 0.f, float maxDepth = 1.f );
	void	SetScissorRect( const AABB2& extent );
	void	SetRenderState( const RenderState& state );
	void	SetFillMode( FillMode mode );
	void	SetCullMode( CullMode mode );
	void	SetWindOrder( WindOrder order );
	void	SetDepthState( DepthTestOp compare, bool shouldWrite );
	void	SetColorBlending( BlendOp op, BlendFactor sFactor, BlendFactor dFactor );
	void	SetAlphaBlending( BlendOp op, BlendFactor sFactor, BlendFactor dFactor );
	void	SetDescriptorSetLayouts( size_t count, void* layouts );
	void	SetRenderPass( VkRenderPass renderPass );

	//-----------------------------------------------------------------------------------------------
	// Members
	VKRenderer*								m_renderer = nullptr;
	VkPipelineLayoutCreateInfo				m_pipelineLayoutInfo = {};
	VkPipelineLayout						m_pipelineLayout;
	VkPipeline								m_pipeline;
	VkViewport								m_viewport;
	VkRect2D								m_scissorRect;
	VkRenderPass							m_renderPass;
	
	// Pipeline layout data
	uint32_t								m_descriptorSetCount;
	void*									m_descriptorSetLayouts;

	// Pipeline data
	VkVertexInputBindingDescription					m_vertexBindingInfo = {};
	std::vector<VkVertexInputAttributeDescription>	m_attribInfos;
	std::vector<VkPipelineShaderStageCreateInfo>	m_shaderStages;
	VkPipelineVertexInputStateCreateInfo			m_vertexInputInfo = {};
	VkPipelineInputAssemblyStateCreateInfo			m_inputAssemblyInfo = {};
	VkPipelineViewportStateCreateInfo				m_viewportInfo = {};
	VkPipelineRasterizationStateCreateInfo			m_rasterStateInfo = {};
	VkPipelineMultisampleStateCreateInfo			m_multisamplingInfo = {};
	VkPipelineDepthStencilStateCreateInfo			m_depthStencilInfo = {};
	VkPipelineColorBlendAttachmentState				m_blendingInfo = {};
	VkPipelineColorBlendStateCreateInfo				m_colorBlendStateInfo = {};
	VkPipelineDynamicStateCreateInfo				m_dynamicStateInfo = {};
	
};

