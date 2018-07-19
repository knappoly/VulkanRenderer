#pragma once
#include <string>
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum BlendFactor
{
	BLEND_ZERO,
	BLEND_ONE,
	BLEND_ONE_MINUS_SRC_ALPHA,
	BLEND_SRC_ALPHA,
	BLEND_SRC_COLOR,
	BLEND_ONE_MINUS_SRC_COLOR,
	BLEND_DST_ALPHA,
	BLEND_ONE_MINUS_DST_ALPHA,
	BLEND_DST_COLOR,
	BLEND_ONE_MINUS_DST_COLOR,
	BLEND_CONSTANT_COLOR,
	BLEND_ONE_MINUS_CONSTANT_COLOR
};
static const std::map<std::string, BlendFactor> ParseBlendFactor = 
{
	{"zero",			BLEND_ZERO},
	{"one",				BLEND_ONE},
	{"inv_src_alpha",	BLEND_ONE_MINUS_SRC_ALPHA},	
	{"src_alpha",		BLEND_SRC_ALPHA},
	{"src_color",		BLEND_SRC_COLOR},
	{"inv_src_color",	BLEND_ONE_MINUS_SRC_COLOR},
	{"dst_alpha",		BLEND_DST_ALPHA},
	{"inv_dst_alpha",	BLEND_ONE_MINUS_DST_ALPHA},
	{"dst_color",		BLEND_DST_COLOR},
	{"inv_dst_color",	BLEND_ONE_MINUS_DST_COLOR},
	{"constant",		BLEND_CONSTANT_COLOR},
	{"inv_constant",	BLEND_ONE_MINUS_CONSTANT_COLOR}
};

