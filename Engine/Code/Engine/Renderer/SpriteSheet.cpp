#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteSheet::SpriteSheet(const Texture& texture, int tilesWide, int tilesHigh): m_spriteSheetTexture(texture), m_spriteLayout(IntVector2(tilesWide,tilesHigh))
{

}

//-----------------------------------------------------------------------------------------------
// Returns the texture coords for the sprite from the sprite sheet
//
AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(const IntVector2& spriteCoords) const
{
	AABB2 texCoords;
	texCoords.mins.x = static_cast<float>(spriteCoords.x) * 1.f/static_cast<float>(m_spriteLayout.x);
	texCoords.mins.y = static_cast<float>(spriteCoords.y) * 1.f/static_cast<float>(m_spriteLayout.y);

	texCoords.maxs.x = static_cast<float>(spriteCoords.x + 1) * 1.f/static_cast<float>(m_spriteLayout.x);
	texCoords.maxs.y = static_cast<float>(spriteCoords.y + 1) * 1.f/static_cast<float>(m_spriteLayout.y);

	texCoords.mins.y = 1.f - texCoords.mins.y; // Offset the texture y-flip
	texCoords.maxs.y = 1.f - texCoords.maxs.y;
	return texCoords;
}

//-----------------------------------------------------------------------------------------------
// Returns the texture coordinates for the sprite at index 
// 
AABB2 SpriteSheet::GetTexCoordsForSpriteCoords(int spriteIndex) const
{
	IntVector2 spriteCoords;
	
	spriteCoords.x = spriteIndex % m_spriteLayout.y;
	spriteCoords.y = spriteIndex / m_spriteLayout.y;

	return GetTexCoordsForSpriteCoords(spriteCoords);
}

//-----------------------------------------------------------------------------------------------
// Returns the number of sprites in the sprite sheet
//
int SpriteSheet::GetNumSprites() const
{
	return m_spriteLayout.x * m_spriteLayout.y;
}



