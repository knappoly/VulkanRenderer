#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum ShaderStageSlot
{
	SHADER_STAGE_INVALID = -1,
	SHADER_STAGE_VERTEX,
	SHADER_STAGE_TESSELLATION_CONTROL,
	SHADER_STAGE_TESSELLATION_EVALUATION,
	SHADER_STAGE_GEOMETRY,
	SHADER_STAGE_FRAGMENT,
	SHADER_STAGE_COMPUTE,
	NUM_SHADER_STAGES
};

// Parse dictionary
static const std::map<std::string, ShaderStageSlot> ParseShaderStageSlot =
{
	{"vertex", SHADER_STAGE_VERTEX},
	{"fragment", SHADER_STAGE_FRAGMENT}
};

