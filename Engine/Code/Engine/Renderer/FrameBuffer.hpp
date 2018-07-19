#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Texture;

//-----------------------------------------------------------------------------------------------
class FrameBuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	FrameBuffer();
	~FrameBuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	unsigned int	GetHandle() { return m_handle; }
	unsigned int	GetWidth();
	unsigned int	GetHeight(); 
	void			SetColorTarget(Texture* colorTarget);
	void			SetDepthStencilTarget(Texture* depthTarget);

	//-----------------------------------------------------------------------------------------------
	// Methods
	void Finalize();

	//-----------------------------------------------------------------------------------------------
	// Members
	unsigned int	m_handle;
	Texture*		m_colorTarget;
	Texture*		m_depthStencilTarget;
	unsigned int	m_width;
	unsigned int	m_height;
};

