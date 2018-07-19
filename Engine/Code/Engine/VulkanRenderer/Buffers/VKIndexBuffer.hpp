#pragma once
#include "Engine/VulkanRenderer/Buffers/VKRenderBuffer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKIndexBuffer : public VKRenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKIndexBuffer( VKRenderer* renderer );
	VKIndexBuffer( void* logicalDevice, void* physicalDevice );
	~VKIndexBuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			uint32_t	GetStride() const { return m_indexStride; }
			uint32_t	GetIndexCount() const { return m_indexCount; }
			void		SetStride( uint32_t stride ) { m_indexStride = stride; }
			void		SetCount( uint32_t count ) { m_indexCount = count; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual bool		CopyToGPU( size_t byteCount, const void* data ) override; // Copies data to GPU 
	
	//-----------------------------------------------------------------------------------------------
	// Members
private:
	uint32_t m_indexStride = 0;
	uint32_t m_indexCount = 0;
};

