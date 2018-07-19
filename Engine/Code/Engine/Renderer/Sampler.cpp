#include "Engine/Renderer/Sampler.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Rgba.hpp"

//-----------------------------------------------------------------------------------------------
// Static globals
static Sampler*	s_pointSampler = nullptr;
static Sampler*	s_linearSampler = nullptr;
static Sampler* s_shadowSampler = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
Sampler::Sampler(): m_samplerHandle(NULL)
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
Sampler::~Sampler()
{
	DestroySampler();
}

//-----------------------------------------------------------------------------------------------
// Creates the sampler using the options from the description
//
bool Sampler::CreateSampler(const SamplerDesc& description)
{
	// create the sampler handle if needed; 
	if (m_samplerHandle == NULL) {
		glGenSamplers( 1, &m_samplerHandle ); 
		if (m_samplerHandle == NULL) {
			return false; 
		}
	}

	// setup wrapping
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_WRAP_S, GetGLSamplerWrapMode(description.wrapS) );  
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_WRAP_T, GetGLSamplerWrapMode(description.wrapT) );  
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_WRAP_R, GetGLSamplerWrapMode(description.wrapR) );  


	GLint minFilter = GetGLFilterMode(description.mipMinFilter, description.minFilter);
	GLint magFilter = GetGLFilterMode(description.mipMagFilter, description.magFilter);

	// filtering; 
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_MIN_FILTER, minFilter );
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_MAG_FILTER, magFilter );

	// Limit where on the LOD I can actually fetch (if you want to control it through the sampler)
	// defaults to -1000.0f to 1000.0f
	glSamplerParameterf( m_samplerHandle, GL_TEXTURE_MIN_LOD, (float)description.minMipLevel ); 
	glSamplerParameterf( m_samplerHandle, GL_TEXTURE_MAX_LOD, (float)description.maxMipLevel ); 
	return true; 
}

//-----------------------------------------------------------------------------------------------
// Creates a point sampler and sets its properties
//
bool Sampler::CreatePointSampler()
{
	
	SamplerDesc pointSamplerDesc;
	pointSamplerDesc.magFilter = NEAREST;
	pointSamplerDesc.minFilter = NEAREST;
	pointSamplerDesc.wrapS = REPEAT;
	pointSamplerDesc.wrapT = REPEAT;
	pointSamplerDesc.wrapR = REPEAT;

	return CreateSampler(pointSamplerDesc); 
}

//-----------------------------------------------------------------------------------------------
// Creates a linear sampler and sets its properties
//
bool Sampler::CreateLinearSampler()
{
	SamplerDesc linearSamplerDesc;
	linearSamplerDesc.magFilter = LINEAR;
	linearSamplerDesc.minFilter = LINEAR;
	linearSamplerDesc.wrapS = REPEAT;
	linearSamplerDesc.wrapT = REPEAT;
	linearSamplerDesc.wrapR = REPEAT;

	return CreateSampler(linearSamplerDesc); 
}

//-----------------------------------------------------------------------------------------------
// Creates a shadow sampler that's internally a linear sampler
//
bool Sampler::CreateShadowSampler()
{
	SamplerDesc linearSamplerDesc;
	linearSamplerDesc.magFilter = LINEAR;
	linearSamplerDesc.minFilter = LINEAR;
	linearSamplerDesc.wrapS = CLAMP_TO_BORDER;
	linearSamplerDesc.wrapT = CLAMP_TO_BORDER;
	linearSamplerDesc.wrapR = CLAMP_TO_BORDER;

	bool create = CreateSampler(linearSamplerDesc);

	glSamplerParameterfv( m_samplerHandle, GL_TEXTURE_BORDER_COLOR, Rgba::WHITE.GetAsFloats() ); 
	
	// compare
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE ); 
	glSamplerParameteri( m_samplerHandle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL ); 

	return create;
}

//-----------------------------------------------------------------------------------------------
// Destroys the sampler and its handle
//
void Sampler::DestroySampler()
{
	if (m_samplerHandle != NULL) {
		glDeleteSamplers( 1, &m_samplerHandle ); 
		m_samplerHandle = NULL; 
	}
}

//-----------------------------------------------------------------------------------------------
// Initializes the various kinds of samplers
//
void Sampler::InitializeSamplers()
{
	if(!s_pointSampler)
	{
		s_pointSampler = new Sampler();
		s_pointSampler->CreatePointSampler();
	}

	if(!s_linearSampler)
	{
		s_linearSampler = new Sampler();
		s_linearSampler->CreateLinearSampler();
	}

	if(!s_shadowSampler)
	{
		s_shadowSampler = new Sampler();
		s_shadowSampler->CreateShadowSampler();
	}
}

//-----------------------------------------------------------------------------------------------
// Destroys the samplers
//
void Sampler::DestroySamplers()
{
	delete s_linearSampler;
	s_linearSampler = nullptr;

	delete s_pointSampler;
	s_pointSampler = nullptr;

	delete s_shadowSampler;
	s_shadowSampler = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Returns the point sampler
//
Sampler* Sampler::GetPointSampler()
{
	return s_pointSampler;
}

//-----------------------------------------------------------------------------------------------
// Returns the linear sampler
//
Sampler* Sampler::GetLinearSampler()
{
	return s_linearSampler;
}

//-----------------------------------------------------------------------------------------------
// Returns the shadow sampler
//
Sampler* Sampler::GetShadowSampler()
{
	return s_shadowSampler;
}

//-----------------------------------------------------------------------------------------------
// Creates a sampler from the given description and returns it
//
Sampler* Sampler::FromDescription(const SamplerDesc& desc)
{
	Sampler* sampler = new Sampler();
	sampler->CreateSampler(desc);

	return sampler;
}

//-----------------------------------------------------------------------------------------------
// Returns the Glint for the sampler wrapmode
//
int GetGLSamplerWrapMode(SamplerWrapMode mode)
{
	switch (mode)
	{
	case REPEAT:				return GL_REPEAT;
	case CLAMP_TO_EDGE:			return GL_CLAMP_TO_EDGE;
	case MIRRORED_REPEAT:		return GL_MIRRORED_REPEAT;
	case CLAMP_TO_BORDER:		return GL_CLAMP_TO_BORDER;
	case MIRROR_CLAMP_TO_EDGE:	return GL_MIRROR_CLAMP_TO_EDGE;
	}

	GUARANTEE_OR_DIE(false, "Unsupported wrap mode");
	return -1;
}

//-----------------------------------------------------------------------------------------------
// Returns the GL filter mode
//
int GetGLFilterMode(FilterMode mipFilter, FilterMode texFilter)
{
	switch (mipFilter)
	{
	case NONE:
		switch (texFilter)
		{
		case LINEAR:	return GL_LINEAR;
		case NEAREST:	return GL_NEAREST;
		}
	case LINEAR:
		switch (texFilter)
		{
		case LINEAR:	return GL_LINEAR_MIPMAP_LINEAR;
		case NEAREST:	return GL_LINEAR_MIPMAP_NEAREST;
		}
	case NEAREST:
		switch (texFilter)
		{
		case LINEAR:	return GL_NEAREST_MIPMAP_LINEAR;
		case NEAREST:	return GL_NEAREST_MIPMAP_NEAREST;
		}
	}

	GUARANTEE_OR_DIE(false, "Unsupported filter mode");
	return -1;
}
