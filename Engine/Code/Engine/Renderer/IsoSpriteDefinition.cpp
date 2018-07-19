#include "Engine/Renderer/IsoSpriteDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Math/Vector3.hpp"

typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Static globals
std::map<std::string, IsoSpriteDefinition*> IsoSpriteDefinition::s_definitions;

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSpriteDefinition::IsoSpriteDefinition(const XMLElement& element)
{
	

	m_name = ParseXmlAttribute(element, "id", m_name);
	GUARANTEE_OR_DIE(m_name != "Invalid ID", Stringf("Name (id) not found for the isosprite at line:%d", element.GetLineNum()).c_str());

	for(const XMLElement* defElement = element.FirstChildElement("facing"); defElement; defElement = defElement->NextSiblingElement("facing"))
	{
		IsoSpriteParams* params = nullptr;
		IntVector2 direction = IntVector2::ZERO;
		std::string spriteName = "Invalid ID";
		Vector2 scale(1.f, 1.f);

		direction = ParseXmlAttribute(*defElement, "dir", direction);
		GUARANTEE_OR_DIE(direction != IntVector2::ZERO, Stringf("Direction not specified for one of the facing specification in isosprite:%s line:%d", m_name.c_str(), defElement->GetLineNum()).c_str());

		std::map<std::string, IsoSpriteParams*>::iterator foundDir = m_facingSprites.find(direction.ToString());
		GUARANTEE_OR_DIE(foundDir == m_facingSprites.end(), Stringf("Duplicate facing direction specification in isosprite:%s line:%d", m_name.c_str(), defElement->GetLineNum()).c_str());

		spriteName = ParseXmlAttribute(*defElement, "src", spriteName);
		GUARANTEE_OR_DIE(spriteName != "Invalid ID", Stringf("Name (id) not found for facing sprite in isosprite at line:%d", defElement->GetLineNum()).c_str());

		scale = ParseXmlAttribute(*defElement, "scale", scale);

		params = new IsoSpriteParams();
		params->m_name = spriteName;
		params->m_scale = scale;

		m_facingSprites[direction.ToString()] = params;
	}
	GUARANTEE_OR_DIE(m_facingSprites.size() > 0, Stringf("No facing direction specified for the isosprite:%s", m_name.c_str()).c_str());
}

//-----------------------------------------------------------------------------------------------
// Return the sprite name(id) for the direction specified
//
IsoSpriteParams* IsoSpriteDefinition::GetSpriteParamForDirection(const IntVector2& direction) const
{
	std::map<std::string, IsoSpriteParams*>::const_iterator foundDir = m_facingSprites.find(direction.ToString());
	GUARANTEE_OR_DIE(foundDir != m_facingSprites.end(), Stringf("Sprite not found for the direction specified:%d,%d", direction.x, direction.y).c_str());

	return foundDir->second;
}

//-----------------------------------------------------------------------------------------------
// Return the first sprite name on the map of sprites
//
IsoSpriteParams* IsoSpriteDefinition::GetDefaultFacingSpriteParams() const
{
	return m_facingSprites.begin()->second;
}

//-----------------------------------------------------------------------------------------------
// Computes the direction to use by camera forward and object's forward and returns the spriteParams
//
IsoSpriteParams* IsoSpriteDefinition::GetSpriteParamForCamera(const Vector3& cameraForward, const Vector3& cameraRight, const Vector3& objectForward) const
{
	IntVector2 direction;

	// Compute the away or toward
	if(DotProduct(cameraForward.xz(), objectForward.xz()) > 0.f)
	{
		direction.y = 1;
	}
	else
	{
		direction.y = -1;
	}

	// Compute the left or right
	if(DotProduct(cameraRight.xz(), objectForward.xz()) > 0.f)
	{
		direction.x = -1;
	}
	else
	{
		direction.x = 1;
	}
	return GetSpriteParamForDirection(direction);
}

//-----------------------------------------------------------------------------------------------
// Initializes the isosprite definitions
//
void IsoSpriteDefinition::InitializeDefinitions(const std::string& filePath)
{
	DebuggerPrintf("\nLoading iso-sprite definitions");
	ConsolePrintf("Loading iso-sprite definitions");
	if(filePath.find(".xml") != std::string::npos)
	{
		XMLDocument definitionsDoc;
		definitionsDoc.LoadFile(filePath.c_str());
		XMLElement* root = definitionsDoc.FirstChildElement();
		for(const XMLElement* definitionElement = root->FirstChildElement(); definitionElement; definitionElement = definitionElement->NextSiblingElement())
		{
			DebuggerPrintf("\n\"%s\" definition loading...", definitionElement->Attribute("id"));
			ConsolePrintf("\"%s\" definition loading...", definitionElement->Attribute("id"));
			std::map<std::string, IsoSpriteDefinition*>::iterator found = IsoSpriteDefinition::s_definitions.find(definitionElement->Attribute("id"));
			if(found != IsoSpriteDefinition::s_definitions.end())
			{
				// Definition already exists
				GUARANTEE_OR_DIE(false, Stringf("IsoSpriteDefinition already exists for \"%s\". Check the file for errors.", definitionElement->Attribute("id")).c_str());
			}
			else
			{
				// New Definition
				IsoSpriteDefinition* newDefinition = new IsoSpriteDefinition(*definitionElement);
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

//-----------------------------------------------------------------------------------------------
// Returns the definition 
//
IsoSpriteDefinition* IsoSpriteDefinition::GetDefinition(const std::string& name)
{
	std::map<std::string, IsoSpriteDefinition*>::iterator found = IsoSpriteDefinition::s_definitions.find(name);
	GUARANTEE_OR_DIE( found != s_definitions.end(), Stringf("Illegal isosprite definition name:%s", name.c_str()).c_str());

	return found->second;
}
