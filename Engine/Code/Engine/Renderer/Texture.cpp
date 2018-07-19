//-----------------------------------------------------------------------------------------------
// Texture.cpp
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Renderer/External/GL/glcorearb.h"
#include "Engine/Renderer/External/GL/glext.h"
#include "Engine/Renderer/External/GL/wglext.h"
#include "Engine/Renderer/Texture.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Core/Image.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Sampler.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
Texture::Texture()
{
	m_sampler = Sampler::GetPointSampler();
}

//-----------------------------------------------------------------------------------------------
// Called only by the Renderer.  Use renderer->CreateOrGetTexture() to instantiate textures.
//
Texture::Texture( const std::string& imageFilePath, Sampler* sampler /*= nullptr*/, bool generateMipMap)
	: m_textureID( 0 )
	, m_dimensions( 0, 0 )
{
	Image image(imageFilePath);
	int numComponents = 4; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	unsigned char* imageData = image.GetTexelsAsByteArray();
	PopulateFromData( imageData, image.GetDimensions(), numComponents, generateMipMap );
	
	if(!sampler)
	{
		m_sampler = Sampler::GetPointSampler();
	}
	else
	{
		m_sampler = sampler;
	}
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
Texture::Texture(const Image& image, Sampler* sampler /*= POINTSAMPLER*/, bool generateMipMap)
{
	int numComponents = 4;
	unsigned char* imageData = image.GetTexelsAsByteArray();
	PopulateFromData(imageData, image.GetDimensions(), numComponents, generateMipMap);
	
	if(!sampler)
	{
		m_sampler = Sampler::GetPointSampler();
	}
	else
	{
		m_sampler = sampler;
	}
}

//-----------------------------------------------------------------------------------------------
// Creates a texture identity on the video card, and populates it with the given image texel data
//
void Texture::PopulateFromData( unsigned char* imageData, const IntVector2& texelSize, int numComponents, bool generateMipMap )
{
	m_dimensions = texelSize;
	GLenum format;
	GLenum channels;
	GLenum pixelLayout;
	
	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	
	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_textureID );
	
	// Tell OpenGL to bind (set) this as the currently active texture
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_textureID );
	
	
	if( numComponents == 3 )
	{
		m_format = TEXTURE_FORMAT_RGB8;
	}
	GetGLFormats(&format, &channels, &pixelLayout, m_format);

	int mipCount = 1;
	if(generateMipMap)
	{
		mipCount = CalculateMipCount(Max(m_dimensions.x, m_dimensions.y));
	}

	// create the gpu buffer
	// note: only this is needed for render targets
	glTexStorage2D(GL_TEXTURE_2D, 
		mipCount,
		format,
		m_dimensions.x, m_dimensions.y);

	GL_CHECK_ERROR();

	glTexSubImage2D( GL_TEXTURE_2D,
		0,
		0, 0,
		m_dimensions.x, m_dimensions.y,
		channels,
		pixelLayout,
		imageData);

	if(generateMipMap)
	{
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	GL_CHECK_ERROR();
}

//-----------------------------------------------------------------------------------------------
// Creates a render target texture with null data
//
bool Texture::CreateRenderTarget(unsigned int width, unsigned int height, eTextureFormat fmt)
{
	GLenum format ;
	GLenum channels;
	GLenum pixelLayout;
	GetGLFormats(&format, &channels, &pixelLayout, fmt);

	// generate the link to this texture
	glGenTextures( 1, &m_textureID ); 
	if (m_textureID == NULL) {
		return false; 
	}

	// Copy the texture - first, get use to be using texture unit 0 for this; 
	glActiveTexture( GL_TEXTURE0 ); 
	glBindTexture( GL_TEXTURE_2D, m_textureID );    // bind our texture to our current texture unit (0)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	//GL_CHECK_ERROR();

	// Copy data into it;
	glTexImage2D( GL_TEXTURE_2D, 0, 
		format,		// what's the format OpenGL should use
		width, 
		height,        
		0,             // border, use 0
		channels,      // how many channels are there?
		pixelLayout,  // how is the data laid out
		nullptr );     // don't need to pass it initialization data 
	GL_CHECK_ERROR();

	glBindTexture( GL_TEXTURE_2D, NULL ); // unset it; 

	// Save this all off
	m_dimensions.x = width;  
	m_dimensions.y = height; 
	m_format = fmt; // I save the format with the texture
					// for sanity checking.
					// great, success
	return true; 
}

//-----------------------------------------------------------------------------------------------
// Creates a compatible render target that matches dimensions and format
//
Texture* Texture::CreateCompatibleTarget(const Texture* src)
{
	Texture* texture = new Texture();
	texture->CreateRenderTarget(src->m_dimensions.x, src->m_dimensions.y, src->m_format);
	return texture;
}

//-----------------------------------------------------------------------------------------------
// Returns the sampler handle
//
unsigned int Texture::GetSamplerHandle() const
{
	return m_sampler->GetHandle();
}

//-----------------------------------------------------------------------------------------------
// Sets the sampler if passed or defaults to the existing sampler
//
void Texture::SetSampler(Sampler* sampler)
{
	if(sampler)
	{
		m_sampler = sampler;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the GLenum of the texture format specified
//
void GetGLFormats(GLenum* out_texFormat, GLenum* out_channels, GLenum* out_pixelLayout, eTextureFormat fmt)
{
	switch (fmt)
	{
	case TEXTURE_FORMAT_RGBA8:
		*out_texFormat = GL_RGBA8;
		*out_channels = GL_RGBA;
		*out_pixelLayout = GL_UNSIGNED_BYTE;
		break;

	case TEXTURE_FORMAT_RGB8:
		*out_texFormat = GL_RGB8;
		*out_channels = GL_RGB;
		*out_pixelLayout = GL_UNSIGNED_BYTE;
		break;

	case TEXTURE_FORMAT_D24S8:
		*out_texFormat = GL_DEPTH_STENCIL; 
		*out_channels = GL_DEPTH_STENCIL; 
		*out_pixelLayout = GL_UNSIGNED_INT_24_8;
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Calculates the mip count for the given dimension (in pixels)
//
int CalculateMipCount(int dimension)
{
	int count = 0;
	while(dimension > 0)
	{
		count++;
		dimension >>= 1;
	}

	return count;
}

