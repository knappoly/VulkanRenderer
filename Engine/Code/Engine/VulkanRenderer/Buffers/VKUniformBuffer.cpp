#include "Engine/VulkanRenderer/Buffers/VKUniformBuffer.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VkRenderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard Includes
#include <memory.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor 
//
VKUniformBuffer::VKUniformBuffer(void* logicalDevice, void* physicalDevice)
	: VKRenderBuffer(logicalDevice, physicalDevice)
{

}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKUniformBuffer::VKUniformBuffer(VKRenderer* renderer)
	: VKRenderBuffer(renderer->GetLogicalDevice(), renderer->GetPhysicalDevice())
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKUniformBuffer::~VKUniformBuffer()
{
	delete m_cpuBuffer;
	m_cpuBuffer = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Copy data to the CPU and dirty the buffer
//
void VKUniformBuffer::SetCPUData(size_t byteSize, const void* data)
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
void VKUniformBuffer::UpdateGPU()
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
void VKUniformBuffer::SetGPUData(size_t byte_size, const void* data)
{
	SetCPUData(byte_size, data);
	UpdateGPU();
}

//-----------------------------------------------------------------------------------------------
// Copies data to GPU 
//
bool VKUniformBuffer::CopyToGPU(size_t byteCount, const void* data)
{
	if(byteCount == 0)
		return true;

	GUARANTEE_OR_DIE(byteCount > 0, "Bad byteCount. Cannot allocate memory");

	VKRenderer* rend = VKRenderer::GetInstance();
	if(m_bufferHandle != VK_NULL_HANDLE && m_deviceMemoryHandle != VK_NULL_HANDLE || byteCount != m_bufferSize)
	{
		Cleanup();
		// Creates a high performance device buffer
		rend->CreateAndGetBuffer((VkBuffer*) &m_bufferHandle, (VkDeviceMemory*) &m_deviceMemoryHandle, byteCount, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		m_bufferSize = byteCount;
	}

	// Copy data to uniform buffer
	void* mappedMemHandle;
	vkMapMemory((VkDevice) m_logicalDevice, (VkDeviceMemory) m_deviceMemoryHandle, 0, m_bufferSize, 0, &mappedMemHandle);
	memcpy(mappedMemHandle, data, m_bufferSize);
	vkUnmapMemory((VkDevice) m_logicalDevice, (VkDeviceMemory) m_deviceMemoryHandle);

	return true;
}
