#include "Engine/VulkanRenderer/Buffers/VKRenderBuffer.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKRenderBuffer::VKRenderBuffer(void* logicalDevice, void* physicalDevice)
{
	m_logicalDevice = logicalDevice;
	m_physicalDevice = physicalDevice;
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKRenderBuffer::~VKRenderBuffer()
{
	Cleanup();

	m_physicalDevice = nullptr;
	m_logicalDevice = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Destroys the buffer and frees the memory
//
void VKRenderBuffer::Cleanup()
{
	if(m_bufferHandle != VK_NULL_HANDLE)
	{
		vkDestroyBuffer((VkDevice)m_logicalDevice, (VkBuffer)m_bufferHandle, nullptr);
		m_bufferHandle = VK_NULL_HANDLE;

		vkFreeMemory((VkDevice)m_logicalDevice, (VkDeviceMemory)m_deviceMemoryHandle, nullptr);
	}
}

