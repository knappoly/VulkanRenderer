#pragma once
#include <string>
#include <map>
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Enumerations/RenderQueue.hpp"
#include "Engine/Structures/RenderState.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class ShaderProgram;

//-----------------------------------------------------------------------------------------------
class Shader
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit Shader( ShaderProgram* program );
	explicit Shader( const tinyxml2::XMLElement& element );
	~Shader(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool			IsOpaque() const { return m_renderQueue == RENDER_QUEUE_OPAQUE; }
			void			SetProgram( ShaderProgram* program ) { m_program = program; }
			void			SetCullMode( CullMode mode ) { m_renderState.m_cullMode = mode; }
			void			SetFillMode( FillMode mode ) { m_renderState.m_fillMode = mode; }
			void			SetFrontFace( WindOrder order ) { m_renderState.m_frontFace = order; }
			void			SetRenderQueue( RenderQueue queue ) { m_renderQueue = queue; }
			void			SetSortOrder( int order ) { m_sortOrder = order; }
			ShaderProgram*	GetProgram() const { return m_program; }
			int				GetSortOrder() const { return m_sortOrder; }
			RenderQueue		GetRenderQueue() const { return m_renderQueue; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void			SetAlphaBlending( BlendOp op, BlendFactor src, BlendFactor dst ); 
			void			DisableAlphaBlending();
			void			SetColorBlending( BlendOp op, BlendFactor src, BlendFactor dst );
			void			DisableColorBlending();
			
			void			SetDepthTest( DepthTestOp compare, bool write ); 
			void			DisableDepth() { SetDepthTest( COMPARE_ALWAYS, false ); }

	//-----------------------------------------------------------------------------------------------
	// Static Methods
	static	Shader*			AcquireResource( const std::string& path );
	static	Shader*			CreateOrGetResource( const std::string& path );
	
	//-----------------------------------------------------------------------------------------------
	// Members
			ShaderProgram*		m_program;
			RenderState			m_renderState;
			RenderQueue			m_renderQueue = RENDER_QUEUE_OPAQUE;
			int					m_sortOrder = 0;

	//-----------------------------------------------------------------------------------------------
	// Static members
	static	std::map<std::string, Shader*> m_loadedShaders;
};

