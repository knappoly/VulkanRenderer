#pragma once
#include "Engine/Math/IntVector2.hpp"
#include <vector>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;

//-----------------------------------------------------------------------------------------------
struct GIFFrame
{
	Texture*	frameTexture;
	float		delay = -1.f;
};

//-----------------------------------------------------------------------------------------------
class GIFAnimation
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	GIFAnimation() {}
	GIFAnimation( const std::string& filePath ); 
	~GIFAnimation(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			unsigned int	GetCurrentHandle() const;
			IntVector2		GetDimensions() const { return m_dimensions; }
	const	Texture*		GetCurrentTexture() const;
			float			GetElapsedSeconds() const { return m_elapsedSeconds; }
			float			GetElapsedFraction() const;
			float			GetDuration() const { return m_duration; }
	const	Texture*		GetTextureAtTime( float seconds ) const;
			void			SetLoopMode( bool mode ) { m_isLooping = mode; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			void			CreateTextureFromData( unsigned char* data, int* delays, int frames );
			void			Update( float deltaSeconds );
			void			PlayFromStart();

private:
	//-----------------------------------------------------------------------------------------------
	// Members
			std::string					m_imageFilePath;
			std::vector<GIFFrame>		m_frames;
			float						m_elapsedSeconds = 0.f;
			float						m_duration = 0.f;
			IntVector2					m_dimensions;
	const	Texture*					m_currentTexture = nullptr;
			bool						m_isFinished = false;
			bool						m_isPlaying = true;
			bool						m_isLooping = true;
};

