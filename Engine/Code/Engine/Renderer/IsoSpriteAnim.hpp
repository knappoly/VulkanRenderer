#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;
class AABB2;
class IsoSpriteAnimDefinition;
class IsoSprite;

//-----------------------------------------------------------------------------------------------
class IsoSpriteAnim
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSpriteAnim( const IsoSpriteAnimDefinition& animDef );
	~IsoSpriteAnim(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void		Update( float deltaSeconds );
			void		PlayFromStart();
			bool		IsFinished() const { return m_isFinished; } 
			float		GetDuration() const; 
			float		GetElapsedSeconds() const { return m_elapsedSeconds; } // Returns the amount of time that has elapsed since animation started
			float		GetElapsedFraction() const; // Returns the elapsed seconds as a fraction of duration
			float		GetRemainingSeconds() const;
			float		GetRemainingFraction() const;
	const	Texture*	GetTexture() const;
			IsoSprite*	GetCurrentIsoSprite() const;
			std::string GetName() const;

	//-----------------------------------------------------------------------------------------------
	// Members
	const	IsoSpriteAnimDefinition*	m_animDef = nullptr;
			IsoSprite*					m_currentIsoSprite = nullptr;
			bool						m_isPlaying = true;
			bool						m_isFinished = false;
			float						m_elapsedSeconds = 0.f;
};

