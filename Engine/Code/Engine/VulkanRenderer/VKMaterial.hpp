#pragma once
#include "Engine/VulkanRenderer/VKShader.hpp"
#include "Engine/Core/XMLUtils.hpp"
#include <string>
#include <map>
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;
class VKTexture;
class VKTexSampler;

//-----------------------------------------------------------------------------------------------
class VKMaterial
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit VKMaterial( VKRenderer* renderer, VKShader* shader );
	explicit VKMaterial( VKRenderer* renderer, const std::string& path );
	explicit VKMaterial( VKRenderer* renderer, const VKMaterial* ) = delete; // Invalid copy constructor
	~VKMaterial();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	bool				IsValid() const { return m_shader != nullptr; }
	bool				IsLit() const { return m_isLit; }
	bool				IsOpaque() const;

	// Texture functions
	void				SetTexture( int bind, VKTexture* resource, VKTexSampler* sampler = nullptr );
	VKTexture*			GetTexture( int bind );
	VKTexSampler*		GetSampler( int bind );
	void				SetSampler( int bind, VKTexSampler* sampler = nullptr );

	// Shader functions
	void				SetShader( VKShader *shader ); 
	void				SetShader( const std::string& filePath );
	VKShader*			GetSharedShader() const { return m_shader; }
	VKShader*			GetEditableShader();
	VKShader*			GetShader() const;
	VKShader*			ResetEditableShader();

	// Shader Wrappers
	void				SetCullMode( CullMode mode );
	void				SetFillMode( FillMode mode );
	void				SetFrontFace( WindOrder order );
	void				SetAlphaBlending( BlendOp op, BlendFactor src, BlendFactor dst ); 
	void				DisableAlphaBlending();
	void				SetColorBlending( BlendOp op, BlendFactor src, BlendFactor dst );
	void				DisableColorBlending();
	void				SetDepthTest( DepthTestOp compare, bool write ); 
	void				DisableDepth() { SetDepthTest( COMPARE_ALWAYS, false ); }
	int					GetSortOrder() const;
	int					GetRenderQueue() const;
	std::vector<void*>&	GetDescriptorSets() const;

	//-----------------------------------------------------------------------------------------------
	// Methods
	void				SetFromXML( const XMLElement& root );
	void				ParseFloatsFromXML( const XMLElement& root );
	void				ParseColorsFromXML( const XMLElement& root );
	void				ParseIntsFromXML( const XMLElement& root );
	void				ParseVec3sFromXML( const XMLElement& root );
	void				RemoveProperty( const char* name ); 
	VKMaterial*			Clone() const;
	
	//-----------------------------------------------------------------------------------------------
	// Static methods
	static	VKMaterial*	FromShader( VKRenderer* renderer, VKShader* shader );
	static	VKMaterial*	FromShader( VKRenderer* renderer, const std::string& filePath );

	//-----------------------------------------------------------------------------------------------
	// Members
	VKRenderer*		m_renderer;
	std::string									m_name = "";
	VKShader*									m_shader = nullptr;
	VKShader*									m_shaderInstance = nullptr;
	//std::map<std::string, MaterialProperty*>	m_properties;
	std::vector<VKTexture*>						m_textures; 
	float										m_specFactor = 0.f;
	float										m_specPower = 8.f;
	bool										m_isLit = true;
};



