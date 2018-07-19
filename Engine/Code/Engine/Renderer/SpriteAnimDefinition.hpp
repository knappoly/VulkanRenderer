#pragma once
#include <vector>
#include "Engine/Core/XMLUtils.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declaration
class SpriteSheet;
class Renderer;
typedef std::vector<int> Ints;

//-----------------------------------------------------------------------------------------------
class SpriteAnimDefinition
{
	friend class SpriteAnim;
	friend class SpriteAnimSetDefinition;

public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructor
	SpriteAnimDefinition( const XMLElement& element, const SpriteSheet& spriteSheet, float defaultFps, Renderer& renderer ); 
	~SpriteAnimDefinition(){}

	//-----------------------------------------------------------------------------------------------
	// Accessors
	float			GetDuration() const { return m_spriteIndices.size() * 1.f / m_framesPerSconds; } 
	int				GetSpriteIndexAtTime( float elapsedSeconds ) const;

	//-----------------------------------------------------------------------------------------------
	// Members
protected:
			std::string		m_name = "UNNAMED";
	const	SpriteSheet*	m_spriteSheet = nullptr;
			float			m_framesPerSconds = 10.f;
			Ints			m_spriteIndices;
			bool			m_isLooping = true;
			bool			m_spriteSheetOverride = false;
};