#pragma once
#include "Engine/Math/IntVector2.hpp"
class Texture;
class AABB2;


//-----------------------------------------------------------------------------------------------
class SpriteSheet
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	SpriteSheet(const Texture& texture, int tilesWide, int tilesHigh);
	~SpriteSheet(){}

	//-----------------------------------------------------------------------------------------------
	// Methods
			AABB2		GetTexCoordsForSpriteCoords( const IntVector2& spriteCoords) const;
			AABB2		GetTexCoordsForSpriteCoords( int spriteIndex ) const;
			int			GetNumSprites() const;
	const	Texture&	GetSpriteSheetTexture() const { return m_spriteSheetTexture; }

	
private:
	const	Texture&	m_spriteSheetTexture;
			IntVector2	m_spriteLayout;


};