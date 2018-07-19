#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKTexture;
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKFramebuffer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKFramebuffer( VKRenderer* renderer );
	~VKFramebuffer();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void*			GetHandle() const { return m_handle; }
	int				GetWidth() const { return m_width; }
	int				GetHeight() const { return m_height; }
	void*			GetRenderPass() const { return m_renderPass; }
	VKTexture*		GetColorTarget() const { return m_colorTarget; }
	VKTexture*		GetDepthTarget() const { return m_depthStencilTarget; }
	void			SetColorTarget(VKTexture* colorTarget);
	void			SetDepthStencilTarget(VKTexture* depthTarget);
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void			CreateRenderPass();
	void			DestroyRenderPass();
	void			DestroyFramebuffer();
	void			Finalize();

	//-----------------------------------------------------------------------------------------------
	// Members
	void*			m_handle;
	VKTexture*		m_colorTarget;
	VKTexture*		m_depthStencilTarget;
	int				m_width;
	int				m_height;
	VKRenderer*		m_renderer;
	void*			m_renderPass;
	bool			m_isDirty = true;
};

