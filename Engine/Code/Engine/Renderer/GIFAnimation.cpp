#include "Engine/Renderer/GIFAnimation.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "ThirdParty/stb/stb_image.h"
#include <string.h>
#include "Engine/File/File.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
GIFAnimation::GIFAnimation(const std::string& filePath)
{
	int numComponents = 0;
	int numComponentsReq = 0;
	size_t length;
	void* imageData = FileReadToNewBuffer(filePath.c_str(), &length);

	int frames = 0;
	int* delays;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* gifData = stbi_load_gif_from_memory( (stbi_uc*)imageData,(int) length, &delays, &m_dimensions.x, &m_dimensions.y, &frames, &numComponents, numComponentsReq);
	stbi_set_flip_vertically_on_load(false);

	CreateTextureFromData(gifData, delays, frames);

	free(imageData);
	stbi_image_free(gifData);
	m_imageFilePath = filePath;
	m_currentTexture = GetTextureAtTime(m_elapsedSeconds);
}

//-----------------------------------------------------------------------------------------------
// Returns the handle of the current frame
//
unsigned int GIFAnimation::GetCurrentHandle() const
{
	return m_currentTexture->GetHandle();
}

//-----------------------------------------------------------------------------------------------
// Returns the current texture
//
const	Texture* GIFAnimation::GetCurrentTexture() const
{
	return m_currentTexture;
}

//-----------------------------------------------------------------------------------------------
// Returns the elapsed seconds as a fraction of duration
//
float GIFAnimation::GetElapsedFraction() const
{
	return m_elapsedSeconds * 1.f / GetDuration();
}

//-----------------------------------------------------------------------------------------------
// Returns the texture at the given time in the animation
//
const	Texture* GIFAnimation::GetTextureAtTime(float seconds) const
{
	if(m_frames.size() == 1)
	{
		return m_frames[0].frameTexture;
	}

	float cumulativeTime = 0.f;
	for(size_t index = 0; index < m_frames.size(); ++index)
	{
		if(seconds < (m_frames[index].delay + cumulativeTime))
		{
			return m_frames[index].frameTexture;
		}
		cumulativeTime += m_frames[index].delay;
	}

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
// Creates textures and appends it to the list of textures
//
void GIFAnimation::CreateTextureFromData(unsigned char* data, int* delays, int frames)
{
	size_t frameSize = m_dimensions.x * m_dimensions.y;
	unsigned char* subData = new unsigned char[frameSize * 4];
	for(int frameIndex = 0; frameIndex < frames; ++frameIndex)
	{
		GIFFrame frame;
		Texture* frameTex = new Texture();
		
		// Copy a frame each time and create a texture
		memcpy(subData, data + (frameIndex * frameSize * 4), frameSize * 4); 
		frameTex->PopulateFromData(subData, m_dimensions, 4);

		frame.frameTexture = frameTex;
		frame.delay = (float) delays[frameIndex] * 0.001f; // data is in milliseconds
		
		m_duration += frame.delay;
		m_frames.push_back(frame);
	}
}

//-----------------------------------------------------------------------------------------------
// Updates the gif animation
//
void GIFAnimation::Update(float deltaSeconds)
{
	if(!m_isPlaying)
		deltaSeconds = 0.f;

	m_elapsedSeconds += deltaSeconds;

	if(m_elapsedSeconds > GetDuration())
	{
		if(m_isLooping)
			PlayFromStart();

		else
		{
			m_isFinished = true;
			m_isPlaying = false;
			m_elapsedSeconds -= deltaSeconds;
		}
	}

	m_currentTexture = GetTextureAtTime(m_elapsedSeconds);
}

//-----------------------------------------------------------------------------------------------
// Plays the animation from the start
//
void GIFAnimation::PlayFromStart()
{
	m_isFinished = false;
	m_elapsedSeconds = 0.f;
	m_isPlaying = true;
}
