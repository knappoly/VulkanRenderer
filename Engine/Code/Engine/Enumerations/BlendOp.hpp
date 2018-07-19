#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum BlendOp
{
	BLEND_OP_ADD,
	BLEND_OP_SUBTRACT,
	BLEND_OP_REVERSE_SUBTRACT,
	BLEND_OP_MIN,
	BLEND_OP_MAX
};
static const std::map<std::string, BlendOp> ParseBlendOp =
{
	{"add",				BLEND_OP_ADD},
	{"subtract",		BLEND_OP_SUBTRACT},
	{"rev_subtract",	BLEND_OP_REVERSE_SUBTRACT},
	{"min",				BLEND_OP_MIN},
	{"max",				BLEND_OP_MAX}
};

