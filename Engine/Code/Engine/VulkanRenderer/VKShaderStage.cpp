#include "Engine/VulkanRenderer/VKShaderStage.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/VulkanRenderer/VKFunctions.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/ShaderCompiler.hpp"
#include "Engine/Enumerations/ShaderStageSlot.hpp"
#include "ThirdParty/spirvcross/spirv_cross.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard includes
#include <map>
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Descriptor Type Dictionary
static const std::map<std::string, VkDescriptorType>  ParseDescriptorType
{
	{"ubo", VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER},
	{"imgsampler", VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER}
};

//-----------------------------------------------------------------------------------------------
// Stage flag dictionary
static const std::map<std::string, VkShaderStageFlags> ParseStageFlags
{
	{"vertex", VK_SHADER_STAGE_VERTEX_BIT},
	{"fragment", VK_SHADER_STAGE_FRAGMENT_BIT}
};

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShaderStage::VKShaderStage(const tinyxml2::XMLElement& element, VKRenderer* renderer)
	: m_renderer(renderer)
{
	m_path = ParseXmlAttribute(element, "file", m_path);
	GUARANTEE_OR_DIE(m_path != "INVALID", "Bad shader stage path");

	std::string stageName = element.Name();
	GUARANTEE_OR_DIE(stageName == "INVALID" || stageName == "", "Bad shader stage name");
	m_stage = ParseShaderStageSlot.at(stageName);

	const XMLElement* descElement = element.FirstChildElement();
	for(const XMLElement* descriptor = descElement->FirstChildElement(); descriptor && descElement; descriptor = descriptor->NextSiblingElement())
	{
		std::string type = ParseXmlAttribute(*descriptor, "type", "INVALID");
		GUARANTEE_OR_DIE(type != "INVALID", "Descriptor type not specified in shader stage");

		VkDescriptorType descType = ParseDescriptorType.at(type);

		int set = -1;
		set = ParseXmlAttribute(*descriptor, "set", set);
		GUARANTEE_OR_DIE(set != -1, "Bad set specified in shader stage descriptors");

		int binding = -1;
		binding = ParseXmlAttribute(*descriptor, "binding", binding);
		GUARANTEE_OR_DIE(binding != -1, "Bad binding specified in shader stage descriptors");

		if(set >= m_bindingLists.size())
		{
			// Create the slots for the sets to populate
			int diff = set - (int) m_bindingLists.size();
			int index = 0;
			do 
			{
				m_bindingLists.push_back(BindingList());
				index++;
			} while (index < diff);
		}

		if(binding >= m_bindingLists[set].size())
		{
			// Create the slots for the bindings in the corresponding set to populate
			int diff = binding - (int) m_bindingLists[set].size();
			int index = 0;
			do 
			{
				VkDescriptorSetLayoutBinding bind = {};
				m_bindingLists[set].push_back(bind);
				index++;
			} while (index < diff);
		}

		m_bindingLists[set][binding].binding = binding;
		m_bindingLists[set][binding].descriptorType = descType;
		m_bindingLists[set][binding].pImmutableSamplers = nullptr;
		m_bindingLists[set][binding].stageFlags = GetVKShaderStageFlag(m_stage);
	}
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKShaderStage::VKShaderStage(const std::string& path, const std::string& stageName, const std::string& src, VKRenderer* renderer, const char* defines)
	: m_renderer(renderer)
{
	m_stage = ParseShaderStageSlot.at(stageName);
	LoadShaderFromSource(path, src, defines);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKShaderStage::~VKShaderStage()
{
// 	for(void* layout : m_descriptorSetLayouts)
// 	{
// 		vkDestroyDescriptorSetLayout(m_renderer->GetLogicalDevice(), (VkDescriptorSetLayout) layout, nullptr);
// 	}

	if(m_shaderModule)
	{
		vkDestroyShaderModule(m_renderer->GetLogicalDevice(), (VkShaderModule) m_shaderModule, nullptr);
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the shader module
//
void* VKShaderStage::GetShaderModule() const
{
	return m_shaderModule;
}

//-----------------------------------------------------------------------------------------------
// Returns the binding list for a given set
//
BindingList VKShaderStage::GetBindingList(int setIndex) const
{
	if(setIndex > m_bindingLists.size() - 1)
	{
		return BindingList(); 
	}

	//BindingList* bindingList = m_bindingLists[index];
	return m_bindingLists[setIndex];
}

//-----------------------------------------------------------------------------------------------
// Creates the shader module and returns the reference
//
void* VKShaderStage::CreateShaderModule(void* byteCode, size_t size)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = size;
	createInfo.pCode = (const uint32_t*) byteCode;

	VkShaderModule module;
	if(vkCreateShaderModule(m_renderer->GetLogicalDevice(), &createInfo, nullptr, &module) != VK_SUCCESS)
	{
		GUARANTEE_OR_DIE(false, "Coudn't create shader module");
	}

	return module;
}

//-----------------------------------------------------------------------------------------------
// Loads a shader from the source text 
//
bool VKShaderStage::LoadShaderFromSource(const std::string& path, const std::string& src, const char* defines)
{
	m_path = path;

	std::string processedShader = PreprocessShader(m_path, m_stage, src, defines);

	std::vector<uint32_t> byteCode = CompileGLSLToSPV(m_path, m_stage, processedShader, true);
	size_t totalSize = byteCode.size() * sizeof(uint32_t);

	m_shaderModule = (VkShaderModule) CreateShaderModule(byteCode.data(), totalSize);
	
	ReflectAndCreateBindings(byteCode);

	return true;
}

//-----------------------------------------------------------------------------------------------
// Uses the byte code to reflect and create binding lists
//
bool VKShaderStage::ReflectAndCreateBindings(std::vector<uint32_t>& byteCode)
{
	spirv_cross::Compiler compiler(std::move(byteCode));

	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	// Uniform buffers
	for(auto& ubo : resources.uniform_buffers)
	{
		uint32_t set = compiler.get_decoration(ubo.id, spv::DecorationDescriptorSet);
		uint32_t binding = compiler.get_decoration(ubo.id, spv::DecorationBinding);

		if(set >= m_bindingLists.size())
		{
			// Create the slots for the sets to populate
			int diff = set - (int) m_bindingLists.size() + 1;
			int index = 0;
			do 
			{
				m_bindingLists.push_back(BindingList());
				index++;
			} while (index < diff);
		}

		if(binding >= m_bindingLists[set].size())
		{
			// Create the slots for the bindings in the corresponding set to populate
			int diff = binding - (int) m_bindingLists[set].size() + 1;
			int index = 0;
			do 
			{
				VkDescriptorSetLayoutBinding bind = {};
				bind.binding = UINT32_MAX;
				m_bindingLists[set].push_back(bind);
				index++;
			} while (index < diff);
		}

		m_bindingLists[set][binding].binding = binding;
		m_bindingLists[set][binding].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		m_bindingLists[set][binding].descriptorCount = 1;
		m_bindingLists[set][binding].pImmutableSamplers = nullptr;
		m_bindingLists[set][binding].stageFlags = GetVKShaderStageFlag(m_stage);
	}

	// Combined samplers
	for(auto& combSampler : resources.sampled_images)
	{
		uint32_t set = compiler.get_decoration(combSampler.id, spv::DecorationDescriptorSet);
		uint32_t binding = compiler.get_decoration(combSampler.id, spv::DecorationBinding);

		if(set >= m_bindingLists.size())
		{
			// Create the slots for the sets to populate
			int diff = set - (int) m_bindingLists.size() + 1;
			int index = 0;
			do 
			{
				m_bindingLists.push_back(BindingList());
				index++;
			} while (index < diff);
		}

		if(binding >= m_bindingLists[set].size())
		{
			// Create the slots for the bindings in the corresponding set to populate
			int diff = binding - (int) m_bindingLists[set].size() + 1;
			int index = 0;
			do 
			{
				VkDescriptorSetLayoutBinding bind = {};
				bind.binding = UINT32_MAX;
				m_bindingLists[set].push_back(bind);
				index++;
			} while (index < diff);
		}

		m_bindingLists[set][binding].binding = binding;
		m_bindingLists[set][binding].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		m_bindingLists[set][binding].descriptorCount = 1;
		m_bindingLists[set][binding].pImmutableSamplers = nullptr;
		m_bindingLists[set][binding].stageFlags = GetVKShaderStageFlag(m_stage);
	}

	return true;
}

