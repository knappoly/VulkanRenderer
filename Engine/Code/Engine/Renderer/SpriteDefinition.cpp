#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "../Console/DevConsole.hpp"

typedef tinyxml2::XMLDocument XMLDocument;
//-----------------------------------------------------------------------------------------------
// Static member initialization
std::map<std::string, SpriteDefinition*> SpriteDefinition::s_definitions;

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteDefinition::SpriteDefinition(const XMLElement& element)
{
	// Load the id
	m_name = ParseXmlAttribute(element, "id", "Invalid ID");
	GUARANTEE_OR_DIE( m_name != "Invalid ID", Stringf("ID not found for Sprite at line:%d", element.GetLineNum()).c_str());

	// Load the texture data
	const XMLElement* defElement = element.FirstChildElement("diffuse");
	GUARANTEE_OR_DIE(defElement != nullptr, Stringf("Texture path invalid for Sprite:%s", m_name.c_str()).c_str());
	std::string texturePath =	ParseXmlAttribute(*defElement, "src", "Invalid Path");
	GUARANTEE_OR_DIE(texturePath != "Invalid Path", Stringf("Texture path not found for Sprite:%s", m_name.c_str()).c_str());
	m_texture = Renderer::GetInstance()->CreateOrGetTexture(texturePath);
	GUARANTEE_OR_DIE(m_texture->GetDimensions().x > 0, Stringf("Texture path not found for Sprite:%s", m_name.c_str()).c_str());

	// Load the uv data
	defElement = element.FirstChildElement("uv");
	std::string uvLayoutType =	ParseXmlAttribute(*defElement, "layout", "normalized");
	m_uvs = ParseXmlInnerText(*defElement, m_uvs); // Default layout is normalized

	if(uvLayoutType == "pixel")
	{
		SetUVsFromPixels(m_uvs);
	}
	
	// Load the ppu data
	defElement = element.FirstChildElement("ppu");
	if(defElement)
	{
		m_ppu = ParseXmlInnerText(*defElement, m_ppu);
	}

	// Load the pivot data
	defElement = element.FirstChildElement("pivot");
	if(defElement)
	{
		m_pivot = ParseXmlInnerText(*defElement, m_pivot);
	}

	m_uvs.maxs.y = 1.f - m_uvs.maxs.y;
	m_uvs.mins.y = 1.f - m_uvs.mins.y;
	SetDimensionsFromUVs(m_uvs);
	SetLocalBoundsFromDimensions(m_worldDimensions, m_pivot);
}

//-----------------------------------------------------------------------------------------------
// Returns the texture
//
const Texture* SpriteDefinition::GetTexture() const
{
	return m_texture;
}

//-----------------------------------------------------------------------------------------------
// Returns the definition for the name
//
SpriteDefinition* SpriteDefinition::GetDefinition(const std::string& name)
{
	std::map<std::string, SpriteDefinition*>::iterator found = SpriteDefinition::s_definitions.find(name);
	GUARANTEE_OR_DIE( found != s_definitions.end(), Stringf("Illegal sprite definition name:%s", name.c_str()).c_str());

	return found->second;
}

//-----------------------------------------------------------------------------------------------
// Calculates the dimensions from UVs using the texture dimensions
//
void SpriteDefinition::SetDimensionsFromUVs(const AABB2& uvs)
{
	IntVector2 mins, maxs;
	IntVector2 texDimensions = m_texture->GetDimensions();
	mins.x = (int) (uvs.mins.x * (float) texDimensions.x);
	maxs.x = (int) (uvs.maxs.x * (float) texDimensions.x);
	mins.y = (int) (uvs.mins.y * (float) texDimensions.y);
	maxs.y = (int) (uvs.maxs.y * (float) texDimensions.y);

	m_dimensions.x = abs(maxs.x - mins.x);
	m_dimensions.y = abs(maxs.y - mins.y);

	m_worldDimensions.x = (float) m_dimensions.x / (float) m_ppu;
	m_worldDimensions.y = (float) m_dimensions.y / (float) m_ppu;
}

//-----------------------------------------------------------------------------------------------
// Calculates the uvs from the texture dimensions
//
void SpriteDefinition::SetUVsFromPixels(const AABB2& pixels)
{
	AABB2 uvs;
	IntVector2 texDimensions = m_texture->GetDimensions();
	uvs.mins.x = pixels.mins.x / (float) texDimensions.x;
	uvs.mins.y = pixels.mins.y / (float) texDimensions.y;

	uvs.maxs.x = pixels.maxs.x / (float) texDimensions.x;
	uvs.maxs.y = pixels.maxs.y / (float) texDimensions.y;

	m_uvs = uvs;
}

//-----------------------------------------------------------------------------------------------
// Calculates the AABB bounds for the sprite dimensions and pivot specified
//
void SpriteDefinition::SetLocalBoundsFromDimensions(const Vector2& dimensions, const Vector2& pivot)
{
	float left = -1.f * pivot.x * dimensions.x;
	float right = left + dimensions.x;
	float bottom = -1.f * pivot.y * dimensions.y;
	float top = bottom + dimensions.y;

	m_spriteBounds.maxs = Vector2(right,top);
	m_spriteBounds.mins = Vector2(left, bottom);
}

//-----------------------------------------------------------------------------------------------
// Initializes the definitions from the xml file
//
void SpriteDefinition::InitializeDefinitions(const std::string& filePath)
{
	DebuggerPrintf("\nLoading sprite definitions");
	ConsolePrintf("Loading sprite definitions");
	if(filePath.find(".xml") != std::string::npos)
	{
		XMLDocument definitionsDoc;
		definitionsDoc.LoadFile(filePath.c_str());
		XMLElement* root = definitionsDoc.FirstChildElement();
		for(const XMLElement* definitionElement = root->FirstChildElement(); definitionElement; definitionElement = definitionElement->NextSiblingElement())
		{
			DebuggerPrintf("\n\"%s\" definition loading...", definitionElement->Attribute("id"));
			ConsolePrintf("\"%s\" definition loading...", definitionElement->Attribute("id"));
			std::map<std::string, SpriteDefinition*>::iterator found = SpriteDefinition::s_definitions.find(definitionElement->Attribute("id"));
			if(found != SpriteDefinition::s_definitions.end())
			{
				// Definition already exists
				GUARANTEE_OR_DIE(false, Stringf("SpriteDefinition already exists for \"%s\". Check the file for errors.", definitionElement->Attribute("id")).c_str());
			}
			else
			{
				// New Definition
				SpriteDefinition* newDefinition = new SpriteDefinition(*definitionElement);
				s_definitions[newDefinition->m_name] = newDefinition;
				DebuggerPrintf("\nDefinition loaded.");
				ConsolePrintf("Definition loaded.");
			}
		}
	}

	else
	{
		GUARANTEE_OR_DIE(false, "Check sprite definitions path. Missing extension .xml");
	}
}
