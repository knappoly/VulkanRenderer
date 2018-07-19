#include "Engine/Core/ShaderCompiler.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#pragma comment(lib, "ThirdParty/shaderc/shaderc_shared.lib")
#include "ThirdParty/shaderc/shaderc.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/File/File.hpp"
#include "Engine/Core/EngineCommon.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Standard Includes

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Includer Class implementing the IncluderInterface
class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
{
public:
	virtual shaderc_include_result* GetInclude(const char* requested_source, shaderc_include_type type, const char* requesting_source, size_t include_depth) override
	{
		shaderc_include_result* includeData = new shaderc_include_result();
		size_t contentSize;
		std::string filePath;
		std::string includeFile = requested_source;
		std::string requesterPath = requesting_source;
		size_t lastSlashIndex;

		switch (type)
		{
		case shaderc_include_type_relative:
			lastSlashIndex = requesterPath.find_last_of("/");
			requesterPath.erase(lastSlashIndex, requesterPath.size() - lastSlashIndex);
			filePath = requesterPath + "/";
			break;

		case shaderc_include_type_standard:
			filePath = "Data/Shaders/Src/";
			break;
		}

		// Append the include file
		filePath += includeFile; 

		// File read the include file; throw error if the file does not exist
		const char* buffer = (char*) FileReadToNewBuffer(filePath.c_str(), &contentSize);

		char* sourceName = (char*) malloc(contentSize * filePath.size());
		memcpy(sourceName, filePath.c_str(), filePath.size());

		includeData->content = buffer;
		includeData->content_length = contentSize;
		includeData->source_name = sourceName;
		includeData->source_name_length = filePath.size();
		includeData->user_data = nullptr;
		
		UNUSED(include_depth);		
		return includeData;
	}

	virtual void ReleaseInclude(shaderc_include_result* data) override
	{
		free((void*) data->content); // char buffer
		free((void*) data->source_name);
		free((void*) data->user_data); // undefined for now

		delete data;
		data = nullptr;
	}
};


//-----------------------------------------------------------------------------------------------
// Returns the shaderc_stage_kind value for the given ShaderStageSlot value
//
shaderc_shader_kind GetShaderKind( ShaderStageSlot stage )
{
	switch (stage)
	{
	case SHADER_STAGE_VERTEX:					return shaderc_vertex_shader;			break;
	case SHADER_STAGE_TESSELLATION_CONTROL:		return shaderc_tess_control_shader;		break;
	case SHADER_STAGE_TESSELLATION_EVALUATION:	return shaderc_tess_evaluation_shader;	break;
	case SHADER_STAGE_GEOMETRY:					return shaderc_geometry_shader;			break;
	case SHADER_STAGE_FRAGMENT:					return shaderc_fragment_shader;			break;
	case SHADER_STAGE_COMPUTE:					return shaderc_compute_shader;			break;
	default:
		GUARANTEE_OR_DIE(false, "Bad shader stage value.");
		break;
	}
}

//-----------------------------------------------------------------------------------------------
// Preprocesses the shader source and returns the pre-processed shader source as a string
//
std::string PreprocessShader(const std::string& srcName, ShaderStageSlot stage, const std::string& src, const char* defines /*= nullptr */)
{
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	shaderc_shader_kind shaderKind = GetShaderKind(stage);

	std::unique_ptr<ShaderIncluder> includer(new ShaderIncluder());
	options.SetIncluder(std::move(includer));

	if(defines)
	{
		// Iterate through the defines and add macro definitions to options
		StringTokenizer tokenizer(defines, ";");
		tokenizer.Tokenize();

		Strings defineTokens = tokenizer.GetTokens();
		for(std::string define : defineTokens)
		{
			StringTokenizer valueTokenizer(define, "=");
			valueTokenizer.Tokenize();

			if(valueTokenizer.GetTokenCount() == 1) // Value less define
			{
				options.AddMacroDefinition(define);
			}
			else
			{
				Strings tokens = valueTokenizer.GetTokens();
				options.AddMacroDefinition(tokens[0], tokens[1]);
			}
		}
	}
	
	//options.SetOptimizationLevel(shaderc_optimization_level_size);
	shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(src, shaderKind, srcName.c_str(), options);
	
	if (result.GetCompilationStatus() != shaderc_compilation_status_success) 
	{
		GUARANTEE_OR_DIE(false, result.GetErrorMessage());
	}
	
	return {result.cbegin(), result.cend()};
}

//-----------------------------------------------------------------------------------------------
// Compiles the GLSL shader to Spir-V bytecode and returns as a vector of 32-bit words
//
std::vector<uint32_t> CompileGLSLToSPV(const std::string& srcName, ShaderStageSlot stage, const std::string& src, bool shouldOptimize /*= false */)
{
	// Assumes preprocess step does the macro n stuff like that.

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	shaderc_shader_kind shaderKind = GetShaderKind(stage);

	if(shouldOptimize)
	{
		options.SetOptimizationLevel(shaderc_optimization_level_size);
	}

	shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(src, shaderKind, srcName.c_str());
	if (result.GetCompilationStatus() != shaderc_compilation_status_success) 
	{
		GUARANTEE_OR_DIE(false, result.GetErrorMessage());
	}

	return {result.cbegin(), result.cend()};
}
