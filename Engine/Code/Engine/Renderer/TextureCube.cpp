#include "Engine/Renderer/TextureCube.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/Image.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Binds an image to a side
//
static void BindImageToSide( eTextureCubeSide side, const Image& img, int size, int ox, int oy, GLenum channels, GLenum pixel_layout ) 
{
	void const *ptr = img.GetTexelReference( ox, oy ); 
	glTexSubImage2D( ToGLCubeSide(side),
		0,          // mip_level
		0, 0,       // offset
		size, size, 
		channels, 
		pixel_layout, 
		ptr ); 

	GL_CHECK_ERROR(); 
}

//------------------------------------------------------------------------
// Flips the image and binds it
//
static void FlipAndBindImage( eTextureCubeSide side, Image const &img, GLenum channels, GLenum pixel_layout ) 
{
	BindImageToSide( side, img, img.GetDimensions().x, 0, 0, channels, pixel_layout ); 
}

//-----------------------------------------------------------------------------------------------
// Converts eTextureFormat to GL format
//
unsigned int ToGLCubeSide(eTextureCubeSide side)
{
	switch (side)
	{
	case TEXCUBE_RIGHT:		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case TEXCUBE_LEFT:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case TEXCUBE_TOP:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case TEXCUBE_BOTTOM:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case TEXCUBE_FRONT:		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case TEXCUBE_BACK:		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	default:
		GUARANTEE_OR_DIE(false, "Bad TEXCUBE SIDE");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
TextureCube::TextureCube()
{
	m_format = TEXTURE_FORMAT_RGBA8;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
TextureCube::TextureCube(const std::string& path)
{
	m_format = TEXTURE_FORMAT_RGBA8;
	MakeFromImage(path.c_str());
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
TextureCube::~TextureCube()
{
	Cleanup();
}

//-----------------------------------------------------------------------------------------------
// Tidy up the cube
//
void TextureCube::Cleanup()
{
	if (IsValid()) {
		glDeleteTextures( 1, &m_handle );
		m_handle = NULL; 
	}

	m_size = 0; 
	m_format = TEXTURE_FORMAT_UNKNOWN; 
}

//-----------------------------------------------------------------------------------------------
// make a cube map from 6 images +x -x +y -y +z -z
//
bool TextureCube::MakeFromImages(const Image* images)
{
	if (m_handle == NULL) {
		glGenTextures( 1, &m_handle ); 
	}

	m_size = images[0].GetDimensions().x; 
	m_format = TEXTURE_FORMAT_RGBA8;

	GLenum internal_format; 
	GLenum channels; 
	GLenum pixel_layout; 
	GetGLFormats(&internal_format, &channels, &pixel_layout, m_format);

	// bind it; 
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle ); 

	glTexStorage2D( GL_TEXTURE_CUBE_MAP, 1, internal_format, m_size, m_size ); 
	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 ); 


	// bind the image to the side; 
	FlipAndBindImage( TEXCUBE_RIGHT,  images[0], channels, pixel_layout ); 
	FlipAndBindImage( TEXCUBE_LEFT,   images[1], channels, pixel_layout ); 
	FlipAndBindImage( TEXCUBE_TOP,    images[2], channels, pixel_layout ); 
	FlipAndBindImage( TEXCUBE_BOTTOM, images[3], channels, pixel_layout ); 
	FlipAndBindImage( TEXCUBE_FRONT,  images[4], channels, pixel_layout ); 
	FlipAndBindImage( TEXCUBE_BACK,   images[5], channels, pixel_layout ); 

	return GLSucceeded(); 
}

//-----------------------------------------------------------------------------------------------
// make a cube map from a single image
//
bool TextureCube::MakeFromImage(const Image& image)
{
	int width = image.GetDimensions().x; 
	int size = width / 4; 

	if (m_handle == NULL) {
		glGenTextures( 1, &m_handle ); 
	}

	m_size = size; 
	m_format = TEXTURE_FORMAT_RGBA8;

	GLenum internal_format; 
	GLenum channels; 
	GLenum pixel_layout; 
	GetGLFormats( &internal_format, &channels, &pixel_layout, m_format ); 

	// bind it; 
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle ); 
	glTexStorage2D( GL_TEXTURE_CUBE_MAP, 1, internal_format, m_size, m_size ); 
	GL_CHECK_ERROR(); 

	glPixelStorei( GL_UNPACK_ROW_LENGTH, image.GetDimensions().x ); 
	GL_CHECK_ERROR(); 

	// bind the image to the side; 
	BindImageToSide( TEXCUBE_RIGHT,  image, m_size, m_size * 2, m_size * 1, channels, pixel_layout ); 
	BindImageToSide( TEXCUBE_LEFT,   image, m_size, m_size * 0, m_size * 1, channels, pixel_layout ); 
	BindImageToSide( TEXCUBE_TOP,    image, m_size, m_size * 1, m_size * 0, channels, pixel_layout ); 
	BindImageToSide( TEXCUBE_BOTTOM, image, m_size, m_size * 1, m_size * 2, channels, pixel_layout ); 
	BindImageToSide( TEXCUBE_FRONT,  image, m_size, m_size * 1, m_size * 1, channels, pixel_layout ); 
	BindImageToSide( TEXCUBE_BACK,   image, m_size, m_size * 3, m_size * 1, channels, pixel_layout ); 

	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 ); 

	return GLSucceeded(); 
}

//-----------------------------------------------------------------------------------------------
// Loads cube map from file
//
bool TextureCube::MakeFromImage(const char* filename)
{
	Image img(filename, false);

	return MakeFromImage(img);
}
