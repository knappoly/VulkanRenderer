#pragma once
#include "Engine/Renderer/RenderBuffer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------
class IndexBuffer : public RenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IndexBuffer();
	~IndexBuffer(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	uint GetCount() const { return m_indexCount; }
	uint GetStride() const { return m_indexStride; }
	void SetCount( uint count ) { m_indexCount = count; }
	void SetStride( uint stride ) { m_indexStride = stride; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	
	//-----------------------------------------------------------------------------------------------
	// Members
	uint m_indexCount;
	uint m_indexStride;
};

