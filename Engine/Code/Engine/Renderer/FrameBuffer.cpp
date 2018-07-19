#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Window.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
FrameBuffer::FrameBuffer()
{
	if(m_handle == NULL)
	{
		glGenFramebuffers( 1, &m_handle );
	}
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
FrameBuffer::~FrameBuffer()
{
	if (m_handle != NULL) {
		glDeleteFramebuffers( 1, &m_handle ); 
		m_handle = NULL; 
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the width on the color target
//
unsigned int FrameBuffer::GetWidth()
{
	return m_width;
}

//-----------------------------------------------------------------------------------------------
// Returns the height on the color target
//
unsigned int FrameBuffer::GetHeight()
{
	return m_height;
}

//-----------------------------------------------------------------------------------------------
// Sets the color target
//
void FrameBuffer::SetColorTarget(Texture* colorTarget)
{
	m_colorTarget = colorTarget;
	m_width = m_colorTarget->GetDimensions().x;
	m_height = m_colorTarget->GetDimensions().y;
}

//-----------------------------------------------------------------------------------------------
// Sets the depth stencil target
//
void FrameBuffer::SetDepthStencilTarget(Texture* depthTarget)
{
	m_depthStencilTarget = depthTarget;
}

//-----------------------------------------------------------------------------------------------
// Writes to the frame buffer
//
void FrameBuffer::Finalize()
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_handle ); 

	// keep track of which outputs go to which attachments; 
	GLenum targets[1]; 

	// Bind a color target to an attachment point
	// and keep track of which locations to to which attachments. 
	glFramebufferTexture( GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0 + 0, 
		 m_colorTarget->GetHandle(), 
		0 ); 
	GL_CHECK_ERROR();
	// 0 to to attachment 0
	targets[0] = GL_COLOR_ATTACHMENT0 + 0; 

	// Update target bindings
	glDrawBuffers( 1, targets ); 
	GL_CHECK_ERROR();

	// Bind depth if available;
	if (m_depthStencilTarget == nullptr) {
		glFramebufferTexture( GL_FRAMEBUFFER, 
			GL_DEPTH_STENCIL_ATTACHMENT, 
			NULL, 
			0 ); 
	} else {
		glFramebufferTexture( GL_FRAMEBUFFER, 
			GL_DEPTH_STENCIL_ATTACHMENT, 
			m_depthStencilTarget->GetHandle(), 
			0 ); 
	}
	GL_CHECK_ERROR();

	// Error Check - recommend only doing in debug
#if defined(_DEBUG)
	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		DebuggerPrintf( "Failed to create framebuffer:  %u", status );
	}
#endif
}
