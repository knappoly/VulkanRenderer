#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint32_t;

//-----------------------------------------------------------------------------------------------
class VKRenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKRenderBuffer( void* logicalDevice, void* physicalDevice );
	virtual ~VKRenderBuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			void*		GetBufferHandle() const { return m_bufferHandle; }
			void*		GetMemoryHandle() const { return m_deviceMemoryHandle; }
			size_t		GetSize() const { return m_bufferSize; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void		Cleanup(); // Destroys the buffer and frees memory
	virtual bool		CopyToGPU( size_t byteCount, const void* data ) = 0; // Copies data to GPU 
	
	//-----------------------------------------------------------------------------------------------
	// Members
	size_t		m_bufferSize = 0;
	void*		m_physicalDevice;
	void*		m_logicalDevice;
	void*		m_bufferHandle;
	void*		m_deviceMemoryHandle;
};



