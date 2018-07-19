#include "Engine/Renderer/Buffers/UniformBuffer.hpp"
#include <stdlib.h>
//-----------------------------------------------------------------------------------------------
// Engine Includes

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
UniformBuffer::UniformBuffer()
	: RenderBuffer()
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
UniformBuffer::~UniformBuffer()
{
	delete m_cpuBuffer;
	m_cpuBuffer = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Copy data to the CPU and dirty the buffer
//
void UniformBuffer::SetCPUData(size_t byteSize, const void* data)
{
	if(m_cpuBuffer)
	{
		free(m_cpuBuffer);
	}
	m_cpuBuffer = malloc(byteSize);
	memcpy(m_cpuBuffer, data, byteSize);
	m_cpuByteSize = byteSize;
	m_isDirty = true;
}

//-----------------------------------------------------------------------------------------------
// Copies CPU data to GPU if its dirty
//
void UniformBuffer::UpdateGPU()
{
	if(m_isDirty)
	{
		CopyToGPU(m_cpuByteSize, m_cpuBuffer);
		m_isDirty = false;
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the cpu and gpu buffers - clears the dirty flag
//
void UniformBuffer::SetGPUData(size_t byte_size, const void* data)
{
	SetCPUData(byte_size, data);
	UpdateGPU();
}
