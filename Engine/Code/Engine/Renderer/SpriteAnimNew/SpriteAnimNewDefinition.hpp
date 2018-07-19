#pragma once
#include <string>
#include <map>
#include <vector>
#include "Engine\Core\XMLUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Sprite;

//-----------------------------------------------------------------------------------------------
struct SpriteAnimFrame
{
	Sprite*		m_sprite = nullptr;
	float		m_frameDuration = -1.f;
};

//-----------------------------------------------------------------------------------------------
class SpriteAnimNewDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	SpriteAnimNewDefinition( const XMLElement& element );
	~SpriteAnimNewDefinition(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
				float								GetDuration() const { return m_animDuration; } 
				Sprite*								GetSpriteAtTime( float elapsedSeconds ) const;

	//-----------------------------------------------------------------------------------------------
	// Methods
	static		void								InitializeDefinitions( const std::string& filePath );
	static		SpriteAnimNewDefinition*			GetDefinition( const std::string& name );

	//-----------------------------------------------------------------------------------------------
	// Members
	static	std::map<std::string, SpriteAnimNewDefinition*> s_definitions;

			std::string										m_name = "Invalid ID";
			float											m_animDuration = 0.f;
			std::vector<SpriteAnimFrame*>					m_sprites;
			bool											m_isLooping = true;
			bool											m_spriteSheetOverride = false;
};

