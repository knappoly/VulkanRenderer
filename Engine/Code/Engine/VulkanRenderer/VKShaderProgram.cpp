#include "Engine/VulkanRenderer/VKShaderProgram.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKShaderStage.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Static globals
VkDescriptorType descriptorTypes[] = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER};


//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShaderProgram::VKShaderProgram(VKRenderer* renderer)
{
	m_renderer = renderer;
	m_shaderStages.resize(NUM_SHADER_STAGES); // Reserving memory for all possible shader stages
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShaderProgram::VKShaderProgram(VKRenderer* renderer, const tinyxml2::XMLElement& element)
{
	m_renderer = renderer;
	m_shaderStages.resize(NUM_SHADER_STAGES);

	UNUSED(element);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKShaderProgram::~VKShaderProgram()
{
	for(VKShaderStage* stage : m_shaderStages)
	{
		delete stage;
		stage = nullptr;
	}
	
	m_shaderStages.clear();

	for(void* pool : m_descriptorPools)
	{
		vkDestroyDescriptorPool(m_renderer->GetLogicalDevice(), (VkDescriptorPool) pool, nullptr);
	}

	for(void* layout : m_descriptorSetLayouts)
	{
		vkDestroyDescriptorSetLayout(m_renderer->GetLogicalDevice(), (VkDescriptorSetLayout) layout, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the shader modules that are enabled
//
std::vector<VKShaderStage*> VKShaderProgram::GetActiveModules() const
{
	std::vector<VKShaderStage*> activeStages;
	for(VKShaderStage* stage : m_shaderStages)
	{
		if(stage != nullptr)
		{
			activeStages.push_back(stage);
		}
	}

	return activeStages;
}

//-----------------------------------------------------------------------------------------------
// Returns the vertex shader module
//
void* VKShaderProgram::GetVertexModule() const
{
	return m_shaderStages[SHADER_STAGE_VERTEX]->GetShaderModule();
}

//-----------------------------------------------------------------------------------------------
// Returns the fragment shader module
//
void* VKShaderProgram::GetFragmentModule() const
{
	return m_shaderStages[SHADER_STAGE_FRAGMENT]->GetShaderModule();
}

//-----------------------------------------------------------------------------------------------
// Loads a shader program from source string
//
bool VKShaderProgram::LoadShaderFromSource(const char* vsPath, const char* fsPath, const char* vsSource, const char* fsSource, const char* defines)
{

	VKShaderStage* vertexStage = new VKShaderStage(vsPath, "vertex", vsSource, m_renderer, defines);
	VKShaderStage* fragmentStage = new VKShaderStage(fsPath, "fragment", fsSource, m_renderer, defines);

	m_shaderStages[SHADER_STAGE_VERTEX] = vertexStage;
	m_shaderStages[SHADER_STAGE_FRAGMENT] = fragmentStage;

	CreateDescriptorSetLayouts();
	return true;
}

//-----------------------------------------------------------------------------------------------
// Creates the descriptor set layouts from the program stages
//
void VKShaderProgram::CreateDescriptorSetLayouts()
{
	VKShaderStage* vertStage = m_shaderStages[SHADER_STAGE_VERTEX];
	VKShaderStage* fragStage = m_shaderStages[SHADER_STAGE_FRAGMENT];
	int vertBindSetCount = (int) vertStage->GetBindingListSetCount();
	int fragBindSetCount = (int) fragStage->GetBindingListSetCount();

	int count = Max(vertBindSetCount, fragBindSetCount);

	std::vector<BindingList> combinedList(count);
	for(int setIndex = 0; setIndex < count; ++setIndex)
	{
		BindingList vertList = vertStage->GetBindingList(setIndex);
		BindingList fragList = fragStage->GetBindingList(setIndex);

		int vertBindCount = (int) vertList.size();
		int fragBindCount = (int) fragList.size();
		combinedList[setIndex].resize(Max(vertBindCount, fragBindCount));

		for(size_t bindIndex = 0; bindIndex < vertList.size(); ++bindIndex)
		{
			if (vertList[bindIndex].binding != UINT32_MAX) // To prevent overwriting
			{
				combinedList[setIndex][bindIndex] = vertList[bindIndex];
			}
		}
		for(size_t bindIndex = 0; bindIndex < fragList.size(); ++bindIndex)
		{
			if (fragList[bindIndex].binding != UINT32_MAX) // To prevent overwriting
			{
				combinedList[setIndex][bindIndex] = fragList[bindIndex];
			}
		}
	}

	m_descriptorSetLayouts.resize(combinedList.size());
	m_descriptorPools.resize(combinedList.size());

	//std::vector<VkDescriptorPoolSize> poolSizes;
	// Iterate over the combinedlist to create the DescriptorSetLayouts and pools
	for(size_t index = 0; index < combinedList.size(); ++index)
	{
		uint32_t descriptorCount = (uint32_t) combinedList[index].size();

		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = descriptorCount;
		createInfo.pBindings = combinedList[index].data();
	
		if(vkCreateDescriptorSetLayout(m_renderer->GetLogicalDevice(), &createInfo, nullptr, (VkDescriptorSetLayout*) &m_descriptorSetLayouts[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Can't create descriptor set layout");
		}

		VkDescriptorPoolSize poolSize = {};
		poolSize.type = descriptorTypes[index];
		poolSize.descriptorCount = descriptorCount;
		//poolSizes.push_back(poolSize);

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = 1;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT;

		if(vkCreateDescriptorPool(m_renderer->GetLogicalDevice(), &poolInfo, nullptr, (VkDescriptorPool*) &m_descriptorPools[index]) != VK_SUCCESS)
		{
			GUARANTEE_OR_DIE(false, "Can't create descriptor pools");
		}
	}

	
}

//-----------------------------------------------------------------------------------------------
// Loads a shader program from files
//
bool VKShaderProgram::LoadFromFiles(const char* vsPath, const char* fsPath /*= nullptr*/, const char* defines /*= nullptr */)
{
	std::string vsFile = vsPath;
	vsFile += ".vert";

	std::string fsFile = vsPath;
	if(fsPath)
	{
		// If the fs file path is specified it loads the other path
		fsFile = fsPath;
	}
	fsFile += ".frag"; 

	const char* vsSrc = (char*) FileReadToNewBuffer(vsFile.c_str());
	const char* fsSrc = (char*) FileReadToNewBuffer(fsFile.c_str());

	return LoadShaderFromSource(vsFile.c_str(), fsFile.c_str(), vsSrc, fsSrc, defines);
}

