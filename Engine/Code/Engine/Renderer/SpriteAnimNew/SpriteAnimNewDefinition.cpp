#include "Engine/Renderer/SpriteAnimNew/SpriteAnimNewDefinition.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Renderer/Sprite.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Static globals
std::map<std::string, SpriteAnimNewDefinition*> SpriteAnimNewDefinition::s_definitions;

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteAnimNewDefinition::SpriteAnimNewDefinition(const XMLElement& element)
{
	m_name = ParseXmlAttribute(element, "id", m_name);
	GUARANTEE_OR_DIE( m_name != "Invalid ID", Stringf("SpriteAnim name not specified at line:%d", element.GetLineNum()).c_str());

	m_isLooping = ParseXmlAttribute(element, "loop", m_isLooping);

	for(const XMLElement* defElement = element.FirstChildElement("frame"); defElement; defElement = defElement->NextSiblingElement("frame"))
	{
		SpriteAnimFrame* frame = nullptr;
		std::string isoSpriteName = "Invalid ID";

		isoSpriteName = ParseXmlAttribute(*defElement, "src", isoSpriteName);
		GUARANTEE_OR_DIE(isoSpriteName != "Invalid ID", Stringf("AnimationFrame name(id) not specified at line:%d", defElement->GetLineNum()).c_str());

		frame = new SpriteAnimFrame();
		frame->m_sprite = new Sprite(*SpriteDefinition::GetDefinition(isoSpriteName));
		GUARANTEE_OR_DIE(frame->m_sprite != nullptr, Stringf("Specified animation frame name(id):%s cannot be found, Line:%d", isoSpriteName.c_str(),defElement->GetLineNum()).c_str());

		frame->m_frameDuration = ParseXmlAttribute(*defElement, "time", frame->m_frameDuration);
		GUARANTEE_OR_DIE(frame->m_frameDuration != -1.f, Stringf("Duration not specified for animation frame - name(id):%s, Line:%d", isoSpriteName.c_str(), defElement->GetLineNum()).c_str());

		m_sprites.push_back(frame);
		m_animDuration += frame->m_frameDuration;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the sprite at the given time
//
Sprite* SpriteAnimNewDefinition::GetSpriteAtTime(float elapsedSeconds) const
{
	if(m_sprites.size() == 1)
	{
		return m_sprites[0]->m_sprite;
	}

	float cumulativeTime = 0.f;
	for(size_t index = 0; index < m_sprites.size(); ++index)
	{
		if(elapsedSeconds < (m_sprites[index]->m_frameDuration + cumulativeTime))
		{
			return m_sprites[index]->m_sprite;
		}
		cumulativeTime += m_sprites[index]->m_frameDuration;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
// Initializes sprite anim definitions
//
void SpriteAnimNewDefinition::InitializeDefinitions(const std::string& filePath)
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
			std::map<std::string, SpriteAnimNewDefinition*>::iterator found = SpriteAnimNewDefinition::s_definitions.find(definitionElement->Attribute("id"));
			if(found != SpriteAnimNewDefinition::s_definitions.end())
			{
				// Definition already exists
				GUARANTEE_OR_DIE(false, Stringf("SpriteAnimDefinition already exists for \"%s\". Check the file for errors.", definitionElement->Attribute("id")).c_str());
			}
			else
			{
				// New Definition
				SpriteAnimNewDefinition* newDefinition = new SpriteAnimNewDefinition(*definitionElement);
				s_definitions[newDefinition->m_name] = newDefinition;
				DebuggerPrintf("\nDefinition loaded.");
				ConsolePrintf("Definition loaded.");
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the definition specified by name
//
SpriteAnimNewDefinition* SpriteAnimNewDefinition::GetDefinition(const std::string& name)
{
	std::map<std::string, SpriteAnimNewDefinition*>::iterator found = SpriteAnimNewDefinition::s_definitions.find(name);
	if(found != s_definitions.end())
	{
		return found->second;
	}
	
	return nullptr;
}
