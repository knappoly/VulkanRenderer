#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Image;
enum eTextureFormat;

//-----------------------------------------------------------------------------------------------
enum eTextureCubeSide
{
	TEXCUBE_RIGHT, 
	TEXCUBE_LEFT,  
	TEXCUBE_TOP,   
	TEXCUBE_BOTTOM,
	TEXCUBE_FRONT, 
	TEXCUBE_BACK,  
};

//-----------------------------------------------------------------------------------------------
class TextureCube
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	TextureCube();
	TextureCube( const std::string& path );
	~TextureCube();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	// cube maps should be square on each face
	inline int	GetWidth() const { return m_size; }
	inline int	GetHeight() const { return m_size; }
	inline bool IsValid() const { return (m_handle != 0); }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void Cleanup();
			bool MakeFromImages( const Image* images );
			bool MakeFromImage( const Image& image );
			bool MakeFromImage( const char* filename ); 
	
	//-----------------------------------------------------------------------------------------------
	// Members
	unsigned int	m_handle = 0;
	eTextureFormat	m_format;
	int				m_size;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
unsigned int ToGLCubeSide( eTextureCubeSide side );