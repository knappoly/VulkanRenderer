#pragma once
#include <string>
#include <vector>
#include "Engine\Core\XMLUtils.hpp"
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef std::vector<int> Ints;
class SpriteSheet;
class IsoSprite;

//-----------------------------------------------------------------------------------------------
struct IsoSpriteAnimFrame
{
	IsoSprite*	m_isoSprite = nullptr;
	float		m_frameDuration = -1.f;
};

//-----------------------------------------------------------------------------------------------
class IsoSpriteAnimDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSpriteAnimDefinition( const XMLElement& element );
	~IsoSpriteAnimDefinition(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
				float								GetDuration() const { return m_animDuration; } 
				IsoSprite*							GetIsoSpriteAtTime( float elapsedSeconds ) const;

	//-----------------------------------------------------------------------------------------------
	// Methods
	static		void								InitializeDefinitions( const std::string& filePath );
	static		IsoSpriteAnimDefinition*			GetDefinition( const std::string& name );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	static	std::map<std::string, IsoSpriteAnimDefinition*> s_definitions;

			std::string										m_name = "Invalid ID";
			float											m_animDuration = 0.f;
			std::vector<IsoSpriteAnimFrame*>				m_isoSprites;
			bool											m_isLooping = true;
			bool											m_spriteSheetOverride = false;
};

