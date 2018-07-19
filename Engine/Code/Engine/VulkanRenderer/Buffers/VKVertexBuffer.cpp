#include "Engine/VulkanRenderer/Buffers/VKVertexBuffer.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKVertexBuffer::VKVertexBuffer(void* logicalDevice, void* physicalDevice)
	: VKRenderBuffer(logicalDevice, physicalDevice)
{
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKVertexBuffer::VKVertexBuffer(VKRenderer* renderer)
	: VKRenderBuffer(renderer->GetLogicalDevice(), renderer->GetPhysicalDevice())
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKVertexBuffer::~VKVertexBuffer()
{
}

//-----------------------------------------------------------------------------------------------
// Copies data to GPU
//
bool VKVertexBuffer::CopyToGPU(size_t byteCount, const void* data)
{
	if(byteCount == 0)
		return true;

	GUARANTEE_OR_DIE(byteCount > 0, "Bad byteCount. Cannot allocate memory");

	VKRenderer* rend = VKRenderer::GetInstance();
	if(m_bufferHandle != VK_NULL_HANDLE && m_deviceMemoryHandle != VK_NULL_HANDLE || byteCount != m_bufferSize)
	{
		Cleanup();
		// Creates a high performance device buffer
		rend->CreateAndGetBuffer((VkBuffer*) &m_bufferHandle, (VkDeviceMemory*) &m_deviceMemoryHandle, byteCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_bufferSize = byteCount;
	}

	// Create a low-performance staging buffer to copy data from CPU -> GPU
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;
	rend->CreateAndGetBuffer(&stagingBuffer, &stagingMemory, byteCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy data to staging buffer
	void* mappedMemHandle;
	vkMapMemory((VkDevice) m_logicalDevice, stagingMemory, 0, m_bufferSize, 0, &mappedMemHandle);
	memcpy(mappedMemHandle, data, m_bufferSize);
	vkUnmapMemory((VkDevice) m_logicalDevice, (VkDeviceMemory) stagingMemory);

	// Copy staging buffer to vertex buffer
	rend->CopyBuffers((VkBuffer) m_bufferHandle, stagingBuffer, byteCount);

	// Destroy the temporary staging buffer
	vkDestroyBuffer((VkDevice) m_logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory((VkDevice) m_logicalDevice, stagingMemory, nullptr);

	return true;
}
