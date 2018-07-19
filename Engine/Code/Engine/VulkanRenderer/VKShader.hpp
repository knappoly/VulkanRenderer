#pragma once
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Enumerations/RenderQueue.hpp"
#include "Engine/Structures/RenderState.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKShaderProgram;
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKShader
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit VKShader( VKShaderProgram* program, VKRenderer* renderer );
	explicit VKShader( const tinyxml2::XMLElement& element, VKRenderer* renderer );
	~VKShader(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool				IsOpaque() const { return m_renderQueue == RENDER_QUEUE_OPAQUE; }
			void				SetProgram( VKShaderProgram* program ) { m_program = program; }
			void				SetCullMode( CullMode mode ) { m_renderState.m_cullMode = mode; }
			void				SetFillMode( FillMode mode ) { m_renderState.m_fillMode = mode; }
			void				SetFrontFace( WindOrder order ) { m_renderState.m_frontFace = order; }
			void				SetRenderQueue( RenderQueue queue ) { m_renderQueue = queue; }
			void				SetSortOrder( int order ) { m_sortOrder = order; }
			VKShaderProgram*	GetProgram() const { return m_program; }
			int					GetSortOrder() const { return m_sortOrder; }
			RenderQueue			GetRenderQueue() const { return m_renderQueue; }
			std::vector<void*>& GetDescriptorSets() { return m_descriptorSets; }
	const	std::vector<void*>& GetDescriptorSets() const { return m_descriptorSets; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			void				CreateDescriptorSets();
			void				SetAlphaBlending( BlendOp op, BlendFactor src, BlendFactor dst ); 
			void				DisableAlphaBlending();
			void				SetColorBlending( BlendOp op, BlendFactor src, BlendFactor dst );
			void				DisableColorBlending();

			void				SetDepthTest( DepthTestOp compare, bool write ); 
			void				DisableDepth() { SetDepthTest( COMPARE_ALWAYS, false ); }
	
	//-----------------------------------------------------------------------------------------------
	// Static Methods
	static	VKShader*			AcquireResource( const std::string& path );
	static	VKShader*			CreateOrGetResource( const std::string& path );

	//-----------------------------------------------------------------------------------------------
	// Members
	VKRenderer*			m_renderer;
	VKShaderProgram*	m_program;
	RenderState			m_renderState;
	RenderQueue			m_renderQueue = RENDER_QUEUE_OPAQUE;
	int					m_sortOrder = 0;
	std::vector<void*>	m_descriptorSets;

	//-----------------------------------------------------------------------------------------------
	// Static members
	static	std::map<std::string, VKShader*> m_loadedShaders;
};

