#pragma once
#include "Engine/Enumerations/ShaderStageSlot.hpp"
#include <string>
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef unsigned int uint32_t;
struct shaderc_include_result;

//-----------------------------------------------------------------------------------------------
// Standalone functions
std::string				PreprocessShader( const std::string& srcName, ShaderStageSlot stage, const std::string& src, const char* defines = nullptr );
std::vector<uint32_t>	CompileGLSLToSPV( const std::string& srcName, ShaderStageSlot stage, const std::string& src, bool shouldOptimize = false );



