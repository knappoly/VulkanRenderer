#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <string>
#include "Engine/Core/XMLUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;
class SpriteDefinition;

//-----------------------------------------------------------------------------------------------
class Sprite
{
	friend class Renderer;
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Sprite(){}
	explicit Sprite( const SpriteDefinition& spriteDef );
	~Sprite(){}
	
public:
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	const		Texture*	GetTexture() const;
				AABB2		GetUV() const;
				Vector2		GetPivot() const;
				IntVector2	GetDimensions() const;
				AABB2		GetBounds() const;
				Vector2		GetWorldDimensions() const;
	
	//-----------------------------------------------------------------------------------------------
	// Methods
				void		SetDefinition( const SpriteDefinition& spriteDef );

protected:
	//-----------------------------------------------------------------------------------------------
	// Members
	const		SpriteDefinition* m_spriteDef = nullptr;
};

