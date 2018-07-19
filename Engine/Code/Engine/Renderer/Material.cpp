#include "Engine/Renderer/Material.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty_Float.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty_Int.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty_Matrix44.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty_Rgba.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty_Vector3.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"

//-----------------------------------------------------------------------------------------------

typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Material::Material(Shader* shader)
{
	m_shader = shader;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Material::Material(const std::string& path)
{
	XMLDocument doc;
	doc.LoadFile(path.c_str());
	XMLElement* root = doc.FirstChildElement();
	m_name = ParseXmlAttribute(*root, "id", m_name);
	SetFromXML(*root);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Material::~Material()
{
	if(m_shaderInstance)
	{
		delete m_shaderInstance;
		m_shaderInstance = nullptr;
	}

	for(std::map<std::string, MaterialProperty*>::iterator iter = m_properties.begin(); iter != m_properties.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	m_properties.clear();
}

//-----------------------------------------------------------------------------------------------
// Returns true if the object is opaque
//
bool Material::IsOpaque() const
{
	Shader* shader = GetShader();
	return shader->IsOpaque();
}

//-----------------------------------------------------------------------------------------------
// Sets the texture at the bindPoint with its sampler
//
void Material::SetTexture(int bind, Texture* resource, Sampler* sampler /*= nullptr */)
{
	if(bind < 0)
	{
		GUARANTEE_OR_DIE(false, "Invalid bind point for texture in material");
	}

	if(bind >= m_textures.size())
	{
		int diff = bind - (int) m_textures.size() + 1;
		int index = 0;
		do // Runs once if the diff is 0
		{
			m_textures.push_back(nullptr);
			index++;
		} while (index < diff);
	}

	m_textures[bind] = resource;
	resource->SetSampler(sampler);
}

//-----------------------------------------------------------------------------------------------
// Returns the texture at the bindPoint
//
Texture* Material::GetTexture(int bind)
{
	return m_textures[bind];
}

//-----------------------------------------------------------------------------------------------
// Returns the sampler at the bindPoint
//
Sampler* Material::GetSampler(int bind)
{
	return m_textures[bind]->GetSampler();
}

//-----------------------------------------------------------------------------------------------
// Sets the sampler for the texture at the given bind point
//
void Material::SetSampler(int bind, Sampler* sampler /*= nullptr */)
{
	if(bind < -1 || bind >= m_textures.size())
	{
		return;
	}

	Texture* tex = m_textures[bind];
	if(tex)
	{
		tex->SetSampler(sampler);
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the shader
//
void Material::SetShader(Shader *shader)
{
	m_shader = shader;

	if(m_shaderInstance)
	{
		delete m_shaderInstance;
		m_shaderInstance = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the shader from file
//
void Material::SetShader(const std::string& filePath)
{
	SetShader(Shader::CreateOrGetResource(filePath));
}

//-----------------------------------------------------------------------------------------------
// Returns an editable form of the shader by creating an instance
//
Shader* Material::GetEditableShader()
{
	if(!m_shaderInstance)
	{
		m_shaderInstance = new Shader(*m_shader);
	}

	return m_shaderInstance;
}

//-----------------------------------------------------------------------------------------------
// Returns the currently used shader, if instanced is available then that takes priority
//
Shader* Material::GetShader() const
{
	if(m_shaderInstance)
	{
		return m_shaderInstance;
	}

	return m_shader;
}

//-----------------------------------------------------------------------------------------------
// Deletes the shader instance and creates a fresh instance from the shared version
//
Shader* Material::ResetEditableShader()
{
	if(m_shaderInstance)
	{
		delete m_shaderInstance;
		m_shaderInstance = new Shader(*m_shader);
	}

	return m_shaderInstance;
}

//-----------------------------------------------------------------------------------------------
// Sets the cull mode on the shader instance
//
void Material::SetCullMode(CullMode mode)
{
	Shader* shader = GetEditableShader();
	shader->SetCullMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode on the shader instance
//
void Material::SetFillMode(FillMode mode)
{
	Shader* shader = GetEditableShader();
	shader->SetFillMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Sets the winding order on the shader instance
//
void Material::SetFrontFace(WindOrder order)
{
	Shader* shader = GetEditableShader();
	shader->SetFrontFace(order);
}

//-----------------------------------------------------------------------------------------------
// Sets the alpha blending mode on the shader instance
//
void Material::SetAlphaBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	Shader* shader = GetEditableShader();
	shader->SetAlphaBlending(op, src, dst);
}

//-----------------------------------------------------------------------------------------------
// Disables alpha blending mode on the shader instance
//
void Material::DisableAlphaBlending()
{
	SetAlphaBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ONE);
}

//-----------------------------------------------------------------------------------------------
// Sets the color blending mode on the shader instance
//
void Material::SetColorBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	Shader* shader = GetEditableShader();
	shader->SetColorBlending(op, src, dst);
}

//-----------------------------------------------------------------------------------------------
// Disables color blending mode on the shader instance
//
void Material::DisableColorBlending()
{
	SetColorBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ZERO);
}

//-----------------------------------------------------------------------------------------------
// Sets the depth test mode on the shader instance
//
void Material::SetDepthTest(DepthTestOp compare, bool write)
{
	Shader* shader = GetEditableShader();
	shader->SetDepthTest(compare, write);
}

//-----------------------------------------------------------------------------------------------
// Returns the sort order from the shader
//
int Material::GetSortOrder() const
{
	Shader* shader = GetShader();
	return shader->GetSortOrder();
}

//-----------------------------------------------------------------------------------------------
// Returns the render queue from the shader
//
int Material::GetRenderQueue() const
{
	Shader* shader = GetShader();
	return shader->GetRenderQueue();
}

//-----------------------------------------------------------------------------------------------
// Returns the property if it exists or returns nullptr
//
MaterialProperty* Material::GetProperty(const char* name) const
{
	std::map<std::string, MaterialProperty*>::const_iterator found = m_properties.find(name);
	if(found != m_properties.end())
	{
		return found->second;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
// Checks the type of the existing property (if any) and modifies accordingly
//
void Material::SetProperty(const char* name, float value)
{
	MaterialProperty* prop = GetProperty(name);

	if(prop == nullptr)
	{
		prop = new MaterialProperty_Float(name, value);
	}
	else
	{
		// Already exists and data types match
		if(prop->GetType() == DATA_FLOAT)
		{
			MaterialProperty_Float* propFloat = (MaterialProperty_Float*) prop;
			propFloat->SetData(value);
			return;
		}
		else
		{
			RemoveProperty(name);
			prop = new MaterialProperty_Float(name, value);
		}
	}

	m_properties[name] = prop;
}

//-----------------------------------------------------------------------------------------------
// Checks the type of the existing property (if any) and modifies accordingly
//
void Material::SetProperty(const char* name, int value)
{
	MaterialProperty* prop = GetProperty(name);

	if(prop == nullptr)
	{
		prop = new MaterialProperty_Int(name, value);
	}
	else
	{
		// Already exists and data types match
		if(prop->GetType() == DATA_INT)
		{
			MaterialProperty_Int* propInt = (MaterialProperty_Int*) prop;
			propInt->SetData(value);
			return;
		}
		else
		{
			RemoveProperty(name);
			prop = new MaterialProperty_Int(name, value);
		}
	}

	m_properties[name] = prop;
}

//-----------------------------------------------------------------------------------------------
// Checks the type of the existing property (if any) and modifies accordingly
//
void Material::SetProperty(const char* name, const Rgba& color)
{
	MaterialProperty* prop = GetProperty(name);

	if(prop == nullptr)
	{
		prop = new MaterialProperty_Rgba(name, color);
	}
	else
	{
		// Already exists and data types match
		if(prop->GetType() == DATA_RGBA)
		{
			MaterialProperty_Rgba* propColor = (MaterialProperty_Rgba*) prop;
			propColor->SetData(color);
			return;
		}
		else
		{
			RemoveProperty(name);
			prop = new MaterialProperty_Rgba(name, color);
		}
	}

	m_properties[name] = prop;
}

//-----------------------------------------------------------------------------------------------
// Checks the type of the existing property (if any) and modifies accordingly
//
void Material::SetProperty(const char* name, const Matrix44& matrix, bool transpose /*= false */)
{
	MaterialProperty* prop = GetProperty(name);

	if(prop == nullptr)
	{
		prop = new MaterialProperty_Matrix44(name, matrix, transpose);
	}
	else
	{
		// Already exists and data types match
		if(prop->GetType() == DATA_MATRIX44)
		{
			MaterialProperty_Matrix44* propColor = (MaterialProperty_Matrix44*) prop;
			propColor->SetData(matrix);
			propColor->SetTransposeMode(transpose);
			return;
		}
		else
		{
			RemoveProperty(name);
			prop = new MaterialProperty_Matrix44(name, matrix, transpose);
		}
	}

	m_properties[name] = prop;
}

//-----------------------------------------------------------------------------------------------
// Checks the type of the existing property (if any) and modifies accordingly
//
void Material::SetProperty(const char* name, const Vector3& value)
{
	MaterialProperty* prop = GetProperty(name);

	if(prop == nullptr)
	{
		prop = new MaterialProperty_Vector3(name, value);
	}
	else
	{
		// Already exists and data types match
		if(prop->GetType() == DATA_VECTOR3)
		{
			MaterialProperty_Vector3* propInt = (MaterialProperty_Vector3*) prop;
			propInt->SetData(value);
			return;
		}
		else
		{
			RemoveProperty(name);
			prop = new MaterialProperty_Vector3(name, value);
		}
	}

	m_properties[name] = prop;
}

//-----------------------------------------------------------------------------------------------
// Sets the property for TINT on the material
//
void Material::SetTint(const Rgba& tint)
{
	SetProperty("TINT", tint);
}

//-----------------------------------------------------------------------------------------------
// Sets the spec amount and power on the material
//
void Material::SetSpecular(float amount /*= 1.f*/, float power /*= 8.f */)
{
	m_specPower = power;
	m_specFactor = amount;
}

//-----------------------------------------------------------------------------------------------
// Creates and returns a material from shader specified
//
STATIC Material* Material::FromShader(Shader* shader)
{
	return new Material(shader);
}

//-----------------------------------------------------------------------------------------------
// Creates and returns a material from shader file path 
//
STATIC Material* Material::FromShader(const std::string& filePath)
{
	return new Material(Shader::CreateOrGetResource(filePath));
}

//-----------------------------------------------------------------------------------------------
// Sets the data from XML file
//
void Material::SetFromXML(const XMLElement& root)
{
	const XMLElement* element = root.FirstChildElement("shader");
	GUARANTEE_OR_DIE(element, "Shader not specified in the material");

	// Parse shader source
	std::string shaderSrc = "INVALID";
	shaderSrc = ParseXmlAttribute(*element, "src", shaderSrc);
	GUARANTEE_OR_DIE(shaderSrc != "INVALID", "Shader not specified in the material");
	SetShader(shaderSrc);

	// Parse textures
	for(element = root.FirstChildElement("texture"); element; element = element->NextSiblingElement("texture"))
	{
		std::string texPath = "INVALID";
		texPath = ParseXmlAttribute(*element, "src", texPath);
		GUARANTEE_OR_DIE(texPath != "INVALID", "Texture src not specified");

		int bind = -1;
		bind = ParseXmlAttribute(*element, "bind", bind);
		GUARANTEE_OR_DIE(bind != -1, "Bind point not specified for texture");
		Texture* texture;
		if(texPath == "default")
		{
			texture = Renderer::GetInstance()->m_defaultTexture;
		}
		else
		{
			texture = Renderer::GetInstance()->CreateOrGetTexture(texPath);
		}
		SetTexture(bind, texture);
	}

	ParseFloatsFromXML(root);
	ParseColorsFromXML(root);
	ParseIntsFromXML(root);
	ParseVec3sFromXML(root);
}

//-----------------------------------------------------------------------------------------------
// Parse float properties
//
void Material::ParseFloatsFromXML(const XMLElement& root)
{
	for(const XMLElement* element = root.FirstChildElement("float"); element; element = element->NextSiblingElement("float"))
	{
		std::string name = "INVALID";
		name = ParseXmlAttribute(*element, "name", name);
		GUARANTEE_OR_DIE(name != "INVALID", "Float property name not specified");

		float data = 0.f;
		data = ParseXmlAttribute(*element, "value", data);

		SetProperty(name.c_str(), data);
	}
}

//-----------------------------------------------------------------------------------------------
// Parse color properties
//
void Material::ParseColorsFromXML(const XMLElement& root)
{
	for(const XMLElement* element = root.FirstChildElement("color"); element; element = element->NextSiblingElement("color"))
	{
		std::string name = "INVALID";
		name = ParseXmlAttribute(*element, "name", name);
		GUARANTEE_OR_DIE(name != "INVALID", "Color property name not specified");

		Rgba data;
		data = ParseXmlAttribute(*element, "value", data);

		SetProperty(name.c_str(), data);
	}
}

//-----------------------------------------------------------------------------------------------
// Parse int properties from XML
//
void Material::ParseIntsFromXML(const XMLElement& root)
{
	for(const XMLElement* element = root.FirstChildElement("int"); element; element = element->NextSiblingElement("int"))
	{
		std::string name = "INVALID";
		name = ParseXmlAttribute(*element, "name", name);
		GUARANTEE_OR_DIE(name != "INVALID", "int property name not specified");

		int data = 0;
		data = ParseXmlAttribute(*element, "value", data);

		SetProperty(name.c_str(), data);
	}
}

//-----------------------------------------------------------------------------------------------
// Parses Vector3s properties
//
void Material::ParseVec3sFromXML(const XMLElement& root)
{
	for(const XMLElement* element = root.FirstChildElement("vector3"); element; element = element->NextSiblingElement("vector3"))
	{
		std::string name = "INVALID";
		name = ParseXmlAttribute(*element, "name", name);
		GUARANTEE_OR_DIE(name != "INVALID", "Vector3 property name not specified");

		Vector3 data;
		data = ParseXmlAttribute(*element, "value", data);

		SetProperty(name.c_str(), data);
	}
}

//-----------------------------------------------------------------------------------------------
// Removes the property from the material
//
void Material::RemoveProperty(const char* name)
{
	MaterialProperty* prop = GetProperty(name);
	if(prop)
	{
		m_properties.erase(name);
		delete prop;
		prop = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Clones this material and returns the instance
//
Material* Material::Clone() const
{
	Material* clone = FromShader(GetSharedShader());
	
	if(m_shaderInstance) // Create copy of the shader instance if it's instanced
	{
		clone->m_shaderInstance = new Shader(*m_shaderInstance);
	}

	if(m_properties.size() > 0) // Create copy of properties to avoid same changing same memory locations
	{
		std::map<std::string, MaterialProperty*>::const_iterator iter = m_properties.begin();
		while(iter != m_properties.end())
		{
			clone->m_properties[iter->first] = iter->second->Clone();
			++iter;
		}
	}
	
	clone->m_textures = m_textures;

	return clone;
}
