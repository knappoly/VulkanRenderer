#pragma once
#include <map>
#include <string>
#include <vector>
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Core/XMLUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;
class Sampler;
class Rgba;
class Matrix44;
class Vector3;
class MaterialProperty;

//-----------------------------------------------------------------------------------------------
class Material
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit Material( Shader* shader );
	explicit Material( const std::string& path );
	explicit Material( const Material* ) = delete; // Invalid copy constructor
	~Material();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	bool				IsValid() const { return m_shader != nullptr; }
	bool				IsLit() const { return m_isLit; }
	bool				IsOpaque() const;

	// Texture functions
	void				SetTexture( int bind, Texture* resource, Sampler* sampler = nullptr );
	Texture*			GetTexture( int bind );
	Sampler*			GetSampler( int bind );
	void				SetSampler( int bind, Sampler* sampler = nullptr );

	// Shader functions
	void				SetShader( Shader *shader ); 
	void				SetShader( const std::string& filePath );
	Shader*				GetSharedShader() const { return m_shader; }
	Shader*				GetEditableShader();
	Shader*				GetShader() const;
	Shader*				ResetEditableShader();

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

	// Property functions - Check current DataType enum and modify if types match else create new
	MaterialProperty*	GetProperty( const char* name ) const;
	void				SetProperty( const char* name, float value );
	void				SetProperty( const char* name, int value );
	void				SetProperty( const char* name, const Rgba& color );
	void				SetProperty( const char* name, const Matrix44& matrix, bool transpose = false );
	void				SetProperty( const char* name, const Vector3& value );

	// Special functions
	void				SetTint( const Rgba& tint ); 
	void				SetSpecular( float amount = 1.f, float power = 8.f ); 
	void				SetSpecFactor( float amount = 1.f ) { m_specFactor = amount; }
	void				SetSpecPower( float power = 8.f ) { m_specPower = power ; }

	//-----------------------------------------------------------------------------------------------
	// Static methods
	static	Material*	FromShader( Shader* shader );
	static	Material*	FromShader( const std::string& filePath );
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void				SetFromXML( const XMLElement& root );
	void				ParseFloatsFromXML( const XMLElement& root );
	void				ParseColorsFromXML( const XMLElement& root );
	void				ParseIntsFromXML( const XMLElement& root );
	void				ParseVec3sFromXML( const XMLElement& root );
	void				RemoveProperty( const char* name ); 
	Material*			Clone() const;

	//-----------------------------------------------------------------------------------------------
	// Members
	std::string									m_name = "";
	Shader*										m_shader = nullptr;
	Shader*										m_shaderInstance = nullptr;
	std::map<std::string, MaterialProperty*>	m_properties;
	std::vector<Texture*>						m_textures; 
	float										m_specFactor = 0.f;
	float										m_specPower = 8.f;
	bool										m_isLit = true;
};

