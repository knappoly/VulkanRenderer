#include "Engine/VulkanRenderer/VKMaterial.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/VulkanRenderer/VKTexSampler.hpp"
#include "Engine/VulkanRenderer/VKTexture.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard Includes

//-----------------------------------------------------------------------------------------------

typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKMaterial::VKMaterial(VKRenderer* renderer, VKShader* shader)
{
	m_renderer = renderer;
	m_shader = shader;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
VKMaterial::VKMaterial(VKRenderer* renderer, const std::string& path)
{
	m_renderer = renderer; 

	XMLDocument doc;
	doc.LoadFile(path.c_str());
	XMLElement* root = doc.FirstChildElement();
	m_name = ParseXmlAttribute(*root, "id", m_name);
	SetFromXML(*root);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
VKMaterial::~VKMaterial()
{
	if(m_shaderInstance)
	{
		delete m_shaderInstance;
		m_shaderInstance = nullptr;
	}

// 	for(std::map<std::string, MaterialProperty*>::iterator iter = m_properties.begin(); iter != m_properties.end(); ++iter)
// 	{
// 		delete iter->second;
// 		iter->second = nullptr;
// 	}
// 
// 	m_properties.clear();
}

//-----------------------------------------------------------------------------------------------
// Returns true if the object is opaque
//
bool VKMaterial::IsOpaque() const
{
	VKShader* shader = GetShader();
	return shader->IsOpaque();
}

//-----------------------------------------------------------------------------------------------
// Sets the texture at the bindPoint with its sampler
//
void VKMaterial::SetTexture(int bind, VKTexture* resource, VKTexSampler* sampler /*= nullptr */)
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
VKTexture* VKMaterial::GetTexture(int bind)
{
	return m_textures[bind];
}

//-----------------------------------------------------------------------------------------------
// Returns the sampler at the bindPoint
//
VKTexSampler* VKMaterial::GetSampler(int bind)
{
	return m_textures[bind]->GetSampler();
}

//-----------------------------------------------------------------------------------------------
// Sets the sampler for the texture at the given bind point
//
void VKMaterial::SetSampler(int bind, VKTexSampler* sampler /*= nullptr */)
{
	if(bind < -1 || bind >= m_textures.size())
	{
		return;
	}

	VKTexture* tex = m_textures[bind];
	if(tex)
	{
		tex->SetSampler(sampler);
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the shader
//
void VKMaterial::SetShader(VKShader *shader)
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
void VKMaterial::SetShader(const std::string& filePath)
{
	SetShader(VKShader::CreateOrGetResource(filePath));
}

//-----------------------------------------------------------------------------------------------
// Returns an editable form of the shader by creating an instance
//
VKShader* VKMaterial::GetEditableShader()
{
	if(!m_shaderInstance)
	{
		m_shaderInstance = new VKShader(*m_shader);
	}

	return m_shaderInstance;
}

//-----------------------------------------------------------------------------------------------
// Returns the currently used shader, if instanced is available then that takes priority
//
VKShader* VKMaterial::GetShader() const
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
VKShader* VKMaterial::ResetEditableShader()
{
	if(m_shaderInstance)
	{
		delete m_shaderInstance;
		m_shaderInstance = new VKShader(*m_shader);
	}

	return m_shaderInstance;
}

//-----------------------------------------------------------------------------------------------
// Sets the cull mode on the shader instance
//
void VKMaterial::SetCullMode(CullMode mode)
{
	VKShader* shader = GetEditableShader();
	shader->SetCullMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Sets the fill mode on the shader instance
//
void VKMaterial::SetFillMode(FillMode mode)
{
	VKShader* shader = GetEditableShader();
	shader->SetFillMode(mode);
}

//-----------------------------------------------------------------------------------------------
// Sets the winding order on the shader instance
//
void VKMaterial::SetFrontFace(WindOrder order)
{
	VKShader* shader = GetEditableShader();
	shader->SetFrontFace(order);
}

//-----------------------------------------------------------------------------------------------
// Sets the alpha blending mode on the shader instance
//
void VKMaterial::SetAlphaBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	VKShader* shader = GetEditableShader();
	shader->SetAlphaBlending(op, src, dst);
}

//-----------------------------------------------------------------------------------------------
// Disables alpha blending mode on the shader instance
//
void VKMaterial::DisableAlphaBlending()
{
	SetAlphaBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ONE);
}

//-----------------------------------------------------------------------------------------------
// Sets the color blending mode on the shader instance
//
void VKMaterial::SetColorBlending(BlendOp op, BlendFactor src, BlendFactor dst)
{
	VKShader* shader = GetEditableShader();
	shader->SetColorBlending(op, src, dst);
}

//-----------------------------------------------------------------------------------------------
// Disables color blending mode on the shader instance
//
void VKMaterial::DisableColorBlending()
{
	SetColorBlending(BLEND_OP_ADD, BLEND_ONE, BLEND_ZERO);
}

//-----------------------------------------------------------------------------------------------
// Sets the depth test mode on the shader instance
//
void VKMaterial::SetDepthTest(DepthTestOp compare, bool write)
{
	VKShader* shader = GetEditableShader();
	shader->SetDepthTest(compare, write);
}

//-----------------------------------------------------------------------------------------------
// Returns the sort order from the shader
//
int VKMaterial::GetSortOrder() const
{
	VKShader* shader = GetShader();
	return shader->GetSortOrder();
}

//-----------------------------------------------------------------------------------------------
// Returns the render queue from the shader
//
int VKMaterial::GetRenderQueue() const
{
	VKShader* shader = GetShader();
	return shader->GetRenderQueue();
}

//-----------------------------------------------------------------------------------------------
// Returns the descriptor sets from the shader
//
std::vector<void*>& VKMaterial::GetDescriptorSets() const
{
	VKShader* shader = GetShader();
	return shader->GetDescriptorSets();
}

//-----------------------------------------------------------------------------------------------
// Sets the data from XML file
//
void VKMaterial::SetFromXML(const XMLElement& root)
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
		VKTexture* texture;
		if(texPath == "default")
		{
			texture = VKRenderer::GetInstance()->GetDefaultTexture();
		}
		else
		{
			texture = VKRenderer::GetInstance()->CreateOrGetTexture(texPath);
		}
		SetTexture(bind, texture);
	}

// 	ParseFloatsFromXML(root);
// 	ParseColorsFromXML(root);
// 	ParseIntsFromXML(root);
// 	ParseVec3sFromXML(root);
}

//-----------------------------------------------------------------------------------------------
// Clones this material and returns the instance
//
VKMaterial* VKMaterial::Clone() const
{
	VKMaterial* clone = FromShader(m_renderer, GetSharedShader());

	if(m_shaderInstance) // Create copy of the shader instance if it's instanced
	{
		clone->m_shaderInstance = new VKShader(*m_shaderInstance);
	}

// 	if(m_properties.size() > 0) // Create copy of properties to avoid same changing same memory locations
// 	{
// 		std::map<std::string, MaterialProperty*>::const_iterator iter = m_properties.begin();
// 		while(iter != m_properties.end())
// 		{
// 			clone->m_properties[iter->first] = iter->second->Clone();
// 			++iter;
// 		}
// 	}

	clone->m_textures = m_textures;

	return clone;
}

//-----------------------------------------------------------------------------------------------
// Creates and returns a material from shader specified
//
VKMaterial* VKMaterial::FromShader(VKRenderer* renderer, VKShader* shader)
{
	return new VKMaterial(renderer, shader);
}

//-----------------------------------------------------------------------------------------------
// Creates and returns a material from shader specified
//
VKMaterial* VKMaterial::FromShader(VKRenderer* renderer, const std::string& filePath)
{
	return new VKMaterial(renderer, VKShader::CreateOrGetResource(filePath));
}
