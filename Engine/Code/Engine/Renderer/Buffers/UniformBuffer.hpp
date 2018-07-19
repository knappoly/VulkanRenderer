#pragma once
#include "Engine/Renderer/RenderBuffer.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class UniformBuffer : public RenderBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	UniformBuffer();
	~UniformBuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators

	// copy data to the CPU and dirty the buffer
	void SetCPUData( size_t byteSize, const void* data ); 

	// update the gpu buffer from the local cpu buffer if dirty
	// and clears the dirty flag
	void UpdateGPU(); 

	// sets the cpu and gpu buffers - clears the dirty flag
	void SetGPUData( size_t byte_size, const void* data ); 

	// gets a pointer to the cpu buffer (const - so does not dirty)
	const void* GetCPUBuffer() const { return m_cpuBuffer; }

	// get a mutable pointer to the cpu buffer.  Sets the dirty flag
	// as it expects the user to change it.
	void* GetCPUBuffer() { m_isDirty = true; return m_cpuBuffer; }

	// get this buffer's size in bytes
	size_t GetSize() const { return m_bufferSize; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods

	//-----------------------------------------------------------------------------------------------
	// Templates

	// Let's me just set a structure, and it'll figure out the size
	template <typename T> 
	void Set( const T& v ) 
	{ 
		SetCPUData( sizeof(T), &v ); 
	}

	// get's a constant reference to the CPU buffer as known struct 
	// would get the same as saying (T*) uniform_buffer->get_cpu_buffer(); 
	template <typename T>
	const T* As() const
	{
		return (const T*) GetCPUBuffer(); 
	}

	template <typename T>
	T* As() 
	{
		return (T*) GetCPUBuffer();
	}

	// Convenience contructor to create a uniform buffer for a given struct
	template<typename T>
	static UniformBuffer* For( const T& v )
	{
		UniformBuffer* buffer = new UniformBuffer();
		buffer->Set<T>(v);

		return buffer;
	}

	//-----------------------------------------------------------------------------------------------
	// Members
	bool	m_isDirty = true;
	size_t	m_cpuByteSize;
	void*	m_cpuBuffer = nullptr;
};

