#pragma once
#include "Engine/Renderer/SpriteAnimSetDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <string>

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteAnimSetDefinition::SpriteAnimSetDefinition( const XMLElement& animSetElement, Renderer& renderer )
{
	std::string spriteSheetName;
	spriteSheetName			= ParseXmlAttribute(animSetElement, "spriteSheet", "Invalid");
	IntVector2 spriteLayout = ParseXmlAttribute(animSetElement, "spriteLayout", IntVector2(0,0));
	float defaultFps = ParseXmlAttribute(animSetElement, "fps", 10.f);
	GUARANTEE_OR_DIE(spriteSheetName != "Invalid", "SpriteSheet name not specified in the spriteAnimSet element");

	std::string path = Stringf("Data/Images/%s", spriteSheetName.c_str());
	SpriteSheet* spriteSheet = new SpriteSheet(*renderer.CreateOrGetTexture(path), spriteLayout.x, spriteLayout.y);

	for(const XMLElement* animElement = animSetElement.FirstChildElement(); animElement; animElement = animElement->NextSiblingElement())
	{
		SpriteAnimDefinition* animDef = new SpriteAnimDefinition(*animElement, *spriteSheet, defaultFps, renderer);
		std::map<std::string, SpriteAnimDefinition*>::iterator found = m_namedAnimDefs.find(animDef->m_name);
		
		GUARANTEE_OR_DIE(found == m_namedAnimDefs.end(), Stringf("Sprite Animation: %s already exists for SpriteAnimSet:%s", animDef->m_name.c_str(), spriteSheetName.c_str()).c_str());
		m_namedAnimDefs[animDef->m_name] = animDef; // Add to the registry of anim defs if its unique
	}
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
SpriteAnimSetDefinition::~SpriteAnimSetDefinition()
{
	m_namedAnimDefs.clear();
	m_defaultAnimName = "";
}