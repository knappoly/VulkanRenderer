#pragma once
#include <vector>
#include "Engine/Enumerations/ShaderStageSlot.hpp"
#include "Engine/Core/XMLUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKShaderStage;
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKShaderProgram
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKShaderProgram( VKRenderer* renderer );
	VKShaderProgram( VKRenderer* renderer, const tinyxml2::XMLElement& element );
	~VKShaderProgram();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			std::vector<VKShaderStage*>		GetActiveModules() const;
			void*							GetVertexModule() const;
			void*							GetFragmentModule() const;
	const	std::vector<void*>&				GetDescSetLayouts() const { return m_descriptorSetLayouts; }
			std::vector<void*>&				GetDescSetLayouts() { return m_descriptorSetLayouts; }
			std::vector<void*>&				GetDescPools() { return m_descriptorPools; }
	const	std::vector<void*>&				GetDescPools() const { return m_descriptorPools; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			bool	LoadShaderFromSource(const char* vsPath, const char* fsPath, const char* vsSource, const char* fsSource, const char* defines = nullptr ); // Loads a shader program from the source
			void	CreateDescriptorSetLayouts();
			bool	LoadFromFiles( const char* vsPath, const char* fsPath = nullptr, const char* defines = nullptr ); // load a shader from file

	//-----------------------------------------------------------------------------------------------
	// Members
	std::vector<VKShaderStage*>		m_shaderStages;
	std::vector<void*>				m_descriptorSetLayouts;
	std::vector<void*>				m_descriptorPools;
	VKRenderer*						m_renderer;
};

