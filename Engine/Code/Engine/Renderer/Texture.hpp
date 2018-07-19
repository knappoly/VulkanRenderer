//-----------------------------------------------------------------------------------------------
// Texture.hpp
//
#pragma once
#include "Engine/Math/IntVector2.hpp"
#include <string>
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Image;
class Sampler;
typedef unsigned int GLenum;

//-----------------------------------------------------------------------------------------------
enum eTextureFormat 
{
	TEXTURE_FORMAT_UNKNOWN,
	TEXTURE_FORMAT_RGBA8, // default color format
	TEXTURE_FORMAT_RGB8,
	TEXTURE_FORMAT_D24S8, 
}; 

//---------------------------------------------------------------------------
class Texture
{
	friend class Renderer; // Textures are managed by a Renderer instance
	friend class GIFAnimation; // Manages textures as frames

private:
	Texture();
	Texture( const std::string& imageFilePath, Sampler* sampler = nullptr, bool generateMipMap = true ); // Use renderer->CreateOrGetTexture() instead!
	Texture( const Image& image, Sampler* sampler = nullptr, bool generateMipMap = true );
	void PopulateFromData( unsigned char* imageData, const IntVector2& texelSize, int numComponents, bool generateMipMap = true );

public:
			bool			CreateRenderTarget( unsigned int width, unsigned int height, eTextureFormat fmt );
	static	Texture*		CreateCompatibleTarget( const Texture* src );
			unsigned int	GetHandle() const { return m_textureID; }
			unsigned int	GetSamplerHandle() const;
			IntVector2		GetDimensions() const { return m_dimensions; }
			void			SetSampler( Sampler* sampler );
			Sampler*		GetSampler() const { return m_sampler; }

private:
	unsigned int								m_textureID;
	IntVector2									m_dimensions;
	eTextureFormat								m_format = TEXTURE_FORMAT_RGBA8;
	Sampler*									m_sampler = nullptr;
};

//-----------------------------------------------------------------------------------------------
// Standalone Functions
void GetGLFormats(GLenum* out_texFormat, GLenum* out_channels, GLenum* out_pixelLayout, eTextureFormat fmt);
int	 CalculateMipCount(int dimension);