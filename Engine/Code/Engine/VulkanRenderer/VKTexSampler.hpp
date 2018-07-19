#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKRenderer;

//-----------------------------------------------------------------------------------------------
class VKTexSampler
{
	friend class VKTexture;

private:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	VKTexSampler( VKRenderer& renderer );
	~VKTexSampler();

	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			void*		GetHandle() const { return m_samplerHandle; }

	//-----------------------------------------------------------------------------------------------
	// Methods
			void			CreatePointSampler();
			void			CreateLinearSampler();

	//-----------------------------------------------------------------------------------------------
	// Static methods
public:
	static	void			InitalizeSamplers( VKRenderer& renderer );
	static	void			DestroySamplers();
	static	VKTexSampler*	GetPointSampler();
	static	VKTexSampler*	GetLinearSampler();

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	VKRenderer&	m_renderer;
	void*		m_samplerHandle;
};

