#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum SamplerWrapMode
{
	REPEAT,
	CLAMP_TO_EDGE,
	MIRRORED_REPEAT,
	CLAMP_TO_BORDER,
	MIRROR_CLAMP_TO_EDGE
};


//-----------------------------------------------------------------------------------------------
enum FilterMode
{
	NONE,
	LINEAR,
	NEAREST
};

//-----------------------------------------------------------------------------------------------
struct SamplerDesc
{
	SamplerWrapMode wrapS = REPEAT;
	SamplerWrapMode wrapT = REPEAT;
	SamplerWrapMode wrapR = REPEAT;
	FilterMode		mipMinFilter = LINEAR;
	FilterMode		mipMagFilter = LINEAR;
	FilterMode		minFilter = NEAREST;
	FilterMode		magFilter = NEAREST;
	float			minMipLevel = -1000.f;
	float			maxMipLevel = 1000.f;
};

