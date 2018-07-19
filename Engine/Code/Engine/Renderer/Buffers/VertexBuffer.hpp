#pragma once
#include "Engine/Renderer/RenderBuffer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------
class VertexBuffer : public RenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VertexBuffer();
	~VertexBuffer(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	uint GetCount() const { return m_vertexCount; }
	uint GetStride() const { return m_vertexStride; }
	void SetCount( uint count ) { m_vertexCount = count; }
	void SetStride( uint stride ) { m_vertexStride = stride; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
	uint m_vertexCount;  
	uint m_vertexStride;
	
};