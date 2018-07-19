#pragma once
#include "Engine/VulkanRenderer/Buffers/VKRenderBuffer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKVertexBuffer : public VKRenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKVertexBuffer( VKRenderer* renderer );
	VKVertexBuffer( void* logicalDevice, void* physicalDevice );
	~VKVertexBuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
				uint32_t	GetStride() const { return m_vertexStride; }
				uint32_t	GetVertexCount() const { return m_vertexCount; }
				void		SetStride( uint32_t stride ) { m_vertexStride = stride; }
				void		SetCount( uint32_t count ) { m_vertexCount = count; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual		bool		CopyToGPU( size_t byteCount, const void* data ) override; 

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	uint32_t m_vertexStride = 0;
	uint32_t m_vertexCount = 0;
};

