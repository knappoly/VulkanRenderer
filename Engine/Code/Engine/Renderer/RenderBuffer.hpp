#pragma once
#include "Engine/Renderer/External/GL/glcorearb.h"

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
class RenderBuffer
{
public:
	  //-----------------------------------------------------------------------------------------------
	  // Constructor/Destructor
      RenderBuffer();   // initialize data
      virtual ~RenderBuffer();  // cleanup OpenGL resource 

	  //-----------------------------------------------------------------------------------------------
	  // Methods
      bool		CopyToGPU( size_t const byte_count, void const *data ); // copies data to the GPU
	  GLuint	GetHandle() const{ return m_handle; } // returns the handle

	  //-----------------------------------------------------------------------------------------------
	  // Members
      GLuint m_handle;       // OpenGL handle to the GPU buffer, default = NULL; 
      size_t m_bufferSize;		 // how many bytes are in this buffer, default = 0
};