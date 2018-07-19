#include "Engine/Renderer/IsoSpriteAnimDefinition.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "IsoSprite.hpp"
#include "IsoSpriteDefinition.hpp"
#include "Engine/Console/DevConsole.hpp"

//-----------------------------------------------------------------------------------------------
typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Static globals
std::map<std::string, IsoSpriteAnimDefinition*> IsoSpriteAnimDefinition::s_definitions;

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSpriteAnimDefinition::IsoSpriteAnimDefinition(const XMLElement& element)
{
	m_name = ParseXmlAttribute(element, "id", m_name);
	GUARANTEE_OR_DIE( m_name != "Invalid ID", Stringf("IsoSpriteAnim name not specified at line:%d", element.GetLineNum()).c_str());

	m_isLooping = ParseXmlAttribute(element, "loop", m_isLooping);

	for(const XMLElement* defElement = element.FirstChildElement("frame"); defElement; defElement = defElement->NextSiblingElement("frame"))
	{
		IsoSpriteAnimFrame* frame = nullptr;
		std::string isoSpriteName = "Invalid ID";

		isoSpriteName = ParseXmlAttribute(*defElement, "src", isoSpriteName);
		GUARANTEE_OR_DIE(isoSpriteName != "Invalid ID", Stringf("AnimationFrame name(id) not specified at line:%d", defElement->GetLineNum()).c_str());

		frame = new IsoSpriteAnimFrame();
		frame->m_isoSprite = new IsoSprite(*IsoSpriteDefinition::GetDefinition(isoSpriteName));
		GUARANTEE_OR_DIE(frame->m_isoSprite != nullptr, Stringf("Specified animation frame name(id):%s cannot be found, Line:%d", isoSpriteName.c_str(),defElement->GetLineNum()).c_str());

		frame->m_frameDuration = ParseXmlAttribute(*defElement, "time", frame->m_frameDuration);
		GUARANTEE_OR_DIE(frame->m_frameDuration != -1.f, Stringf("Duration not specified for animation frame - name(id):%s, Line:%d", isoSpriteName.c_str(), defElement->GetLineNum()).c_str());

		m_isoSprites.push_back(frame);
		m_animDuration += frame->m_frameDuration;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the IsoSprite at the duration specified
//
IsoSprite* IsoSpriteAnimDefinition::GetIsoSpriteAtTime(float elapsedSeconds) const
{
	if(m_isoSprites.size() == 1)
	{
		return m_isoSprites[0]->m_isoSprite;
	}

	float cumulativeTime = 0.f;
	for(size_t index = 0; index < m_isoSprites.size(); ++index)
	{
		if(elapsedSeconds < (m_isoSprites[index]->m_frameDuration + cumulativeTime))
		{
			return m_isoSprites[index]->m_isoSprite;
		}
		cumulativeTime += m_isoSprites[index]->m_frameDuration;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
// Initializes definitions from xml file
//
void IsoSpriteAnimDefinition::InitializeDefinitions(const std::string& filePath)
{
	DebuggerPrintf("\nLoading iso-sprite anim definitions");
	ConsolePrintf("Loading iso-sprite anim definitions");
	if(filePath.find(".xml") != std::string::npos)
	{
		XMLDocument definitionsDoc;
		definitionsDoc.LoadFile(filePath.c_str());
		XMLElement* root = definitionsDoc.FirstChildElement();
		for(const XMLElement* definitionElement = root->FirstChildElement(); definitionElement; definitionElement = definitionElement->NextSiblingElement())
		{
			DebuggerPrintf("\n\"%s\" definition loading...", definitionElement->Attribute("id"));
			ConsolePrintf("\"%s\" definition loading...", definitionElement->Attribute("id"));
			std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = IsoSpriteAnimDefinition::s_definitions.find(definitionElement->Attribute("id"));
			if(found != IsoSpriteAnimDefinition::s_definitions.end())
			{
				// Definition already exists
				GUARANTEE_OR_DIE(false, Stringf("IsoSpriteAnimDefinition already exists for \"%s\". Check the file for errors.", definitionElement->Attribute("id")).c_str());
			}
			else
			{
				// New Definition
				IsoSpriteAnimDefinition* newDefinition = new IsoSpriteAnimDefinition(*definitionElement);
				s_definitions[newDefinition->m_name] = newDefinition;
				DebuggerPrintf("\nDefinition loaded.");
				ConsolePrintf("Definition loaded.");
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the definition
//
IsoSpriteAnimDefinition* IsoSpriteAnimDefinition::GetDefinition(const std::string& name)
{
	std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = IsoSpriteAnimDefinition::s_definitions.find(name);
	GUARANTEE_OR_DIE( found != s_definitions.end(), Stringf("Illegal isosprite animation name:%s", name.c_str()).c_str());

	return found->second;
}
