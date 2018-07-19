#pragma once
#include "Engine/Renderer/Sprite.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class IsoSpriteDefinition;
struct IsoSpriteParams;
class Vector3;

//-----------------------------------------------------------------------------------------------
class IsoSprite : public Sprite
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSprite( const IsoSpriteDefinition& definition );
	~IsoSprite(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Vector2 GetScale() const;
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	SetDefinition( const IsoSpriteDefinition& definition );
	void	SetCurrentSpriteForCamera( const Vector3& camForward, const Vector3& camRight, const Vector3& objectForward );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	const	IsoSpriteDefinition* m_isoSpriteDef = nullptr;
			IsoSpriteParams*	 m_currentSpriteParams = nullptr;
};

