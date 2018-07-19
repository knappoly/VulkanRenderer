#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum DepthTestOp
{
	COMPARE_LESS,
	COMPARE_GREATER,
	COMPARE_GEQUAL,
	COMPARE_LEQUAL,
	COMPARE_NOTEQUAL,
	COMPARE_NEVER,
	COMPARE_ALWAYS
};
static const std::map<std::string, DepthTestOp> ParseDepthTest = 
{
	{"less",		COMPARE_LESS},
	{"greater",		COMPARE_GREATER},
	{"gequal",		COMPARE_GEQUAL},
	{"lequal",		COMPARE_LEQUAL},
	{"notequal",	COMPARE_NOTEQUAL},
	{"never",		COMPARE_NEVER},
	{"always",		COMPARE_ALWAYS}
};

