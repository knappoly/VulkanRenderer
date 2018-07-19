#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Renderer.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor (Constructs from xml element)
// 
SpriteAnimDefinition::SpriteAnimDefinition(const XMLElement& element, const SpriteSheet& spriteSheet, float defaultFps, Renderer& renderer)
	: m_spriteSheet(&spriteSheet)
{
	m_name				=	ParseXmlAttribute(element, "name", m_name.c_str());
	m_framesPerSconds	=	ParseXmlAttribute(element, "fps", defaultFps);
	m_spriteIndices		=	ParseXmlAttribute(element, "spriteIndexes", m_spriteIndices);
	m_isLooping			=	ParseXmlAttribute(element, "isLooping", m_isLooping);

	std::string spriteSheetName = ParseXmlAttribute(element, "spriteSheet", "None");
	if(spriteSheetName != "None")
	{
		// Different spritesheet for this animation
		std::string path = Stringf("Data/Images/%s", spriteSheetName.c_str());
		IntVector2 layout = ParseXmlAttribute(element, "spriteLayout", IntVector2(0,0));

		m_spriteSheet = new SpriteSheet(*renderer.CreateOrGetTexture(path), layout.x, layout.y);
		m_spriteSheetOverride = true;
	}
	
	
	//GUARANTEE_OR_DIE(!m_spriteIndices.empty(), Stringf("Sprite indices not specified for Animation:%s", m_name.c_str).c_str());
}

//-----------------------------------------------------------------------------------------------
// Returns the current index of the SpriteAnimation
//
int SpriteAnimDefinition::GetSpriteIndexAtTime( float elapsedSeconds ) const
{
	int index;
	index = static_cast<int>(elapsedSeconds * (float) m_spriteIndices.size() / GetDuration());
	return m_spriteIndices.at(index);
}