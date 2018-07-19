#include "Engine/Renderer/Sprite.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Sprite::Sprite(const SpriteDefinition& spriteDef): m_spriteDef(&spriteDef)
{
	
}

//-----------------------------------------------------------------------------------------------
// Returns the texture
//
const Texture* Sprite::GetTexture() const
{
	return m_spriteDef->GetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the UVs for the sprite
//
AABB2 Sprite::GetUV() const
{
	return m_spriteDef->GetUV();
}

//-----------------------------------------------------------------------------------------------
// Returns the Pivot of the sprite
//
Vector2 Sprite::GetPivot() const
{
	return m_spriteDef->GetPivot();
}

//-----------------------------------------------------------------------------------------------
// Returns the dimensions of the sprite
//
IntVector2 Sprite::GetDimensions() const
{
	return m_spriteDef->GetDimensions();
}

//-----------------------------------------------------------------------------------------------
// Returns the local bounds calclated in the sprite definition
//
AABB2 Sprite::GetBounds() const
{
	return m_spriteDef->GetSpriteLocalBounds();
}

//-----------------------------------------------------------------------------------------------
// Returns the world dimensions calculated in the sprite definition
//
Vector2 Sprite::GetWorldDimensions() const
{
	return m_spriteDef->GetWorldDimensions();
}

//-----------------------------------------------------------------------------------------------
// Sets the definition for the sprite
//
void Sprite::SetDefinition(const SpriteDefinition& spriteDef)
{
	m_spriteDef = &spriteDef;
}
