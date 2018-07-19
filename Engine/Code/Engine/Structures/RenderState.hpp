#pragma once
#include "Engine/Enumerations/CullMode.hpp"
#include "Engine/Enumerations/FillMode.hpp"
#include "Engine/Enumerations/WindOrder.hpp"
#include "Engine/Enumerations/DepthTestOp.hpp"
#include "Engine/Enumerations/BlendOp.hpp"
#include "Engine/Enumerations/BlendFactor.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
struct RenderState 
{
	// Raster State Control									
	CullMode		m_cullMode			= CULLMODE_BACK;				
	FillMode		m_fillMode			= FILLMODE_SOLID;			
	WindOrder		m_frontFace			= WIND_COUNTER_CLOCKWISE;	

	// Depth State Control
	DepthTestOp		m_depthCompare		= COMPARE_LESS;			
	bool			m_depthWrite		= true;			

	// Blend
	BlendOp			m_colorBlendOp		= BLEND_OP_ADD;	
	BlendFactor		m_colorSrcFactor	= BLEND_ONE;	
	BlendFactor		m_colorDstFactor	= BLEND_ZERO;	

	BlendOp			m_alphaBlendOp		= BLEND_OP_ADD;	
	BlendFactor		m_alphaSrcFactor	= BLEND_ONE;	
	BlendFactor		m_alphaDstFactor	= BLEND_ONE;	
}; 

