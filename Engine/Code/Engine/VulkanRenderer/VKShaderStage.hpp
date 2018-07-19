#pragma once
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Enumerations/ShaderStageSlot.hpp"
#include <vector>

#define VK_NO_PROTOTYPES
#include "External/Vulkan/vulkan_core.h"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;

//-----------------------------------------------------------------------------------------------
typedef std::vector<VkDescriptorSetLayoutBinding> BindingList;

//-----------------------------------------------------------------------------------------------
class VKShaderStage
{

public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit VKShaderStage( const tinyxml2::XMLElement& element, VKRenderer* renderer );
	explicit VKShaderStage( const std::string& name, const std::string& stageName, const std::string& src, VKRenderer* renderer, const char* defines = nullptr );
	~VKShaderStage(); 
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void*			GetShaderModule() const;
	ShaderStageSlot	GetStage() const { return m_stage; }
	BindingList		GetBindingList( int setIndex ) const;
	size_t			GetBindingListSetCount() const { return m_bindingLists.size(); }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void*			CreateShaderModule(void* byteCode, size_t size);
	bool			LoadShaderFromSource(const std::string& path, const std::string& src, const char* defines = nullptr ); // Loads the shader stage from the source
	bool			ReflectAndCreateBindings( std::vector<uint32_t>& byteCode );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	VKRenderer*					m_renderer;
	std::string					m_path = "INVALID";
	ShaderStageSlot				m_stage = SHADER_STAGE_INVALID;
	std::vector<BindingList>	m_bindingLists;
	void*						m_shaderModule;
};

