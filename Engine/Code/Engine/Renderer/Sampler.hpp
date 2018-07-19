#pragma once
//#include "Engine/Renderer/External/GL/glcorearb.h"
#include "Engine/Renderer/SamplerDesc.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class Sampler
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	Sampler();
	~Sampler();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	unsigned int	GetHandle() { return m_samplerHandle; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	bool	CreateSampler( const SamplerDesc& description );
	bool	CreatePointSampler();
	bool	CreateLinearSampler();
	bool	CreateShadowSampler();
	void	DestroySampler();

	//-----------------------------------------------------------------------------------------------
	// Static Methods
	static void		InitializeSamplers();
	static void		DestroySamplers();
	static Sampler*	GetPointSampler();
	static Sampler*	GetLinearSampler();
	static Sampler*	GetShadowSampler();
	static Sampler*	FromDescription( const SamplerDesc& desc );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	unsigned int m_samplerHandle;
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
int GetGLSamplerWrapMode( SamplerWrapMode mode );
int GetGLFilterMode( FilterMode mipFilter, FilterMode texFilter );