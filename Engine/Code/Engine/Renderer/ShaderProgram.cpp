#include "Engine/Renderer/ShaderProgram.hpp"
#include "Engine/Renderer/GLFunctions.hpp"
#include <string>
#include "Engine/File/File.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"

//#define ToString(x) #x

ShaderProgram*	g_defaultProgram = nullptr;
ShaderProgram*	g_invalidProgram = nullptr;
ShaderProgram*	g_defDiffuseProgram = nullptr;

const char* defaultVS =		"#version 420 core\n									\
							 uniform mat4 PROJECTION;\n								\
							 uniform mat4 VIEW; \n									\
							 uniform mat4 MODEL; \n									\
							 in vec3 POSITION;\n									\
							 in vec4 COLOR;\n 										\
							 in vec2 UV;\n											\
							 out vec4 passColor;\n 									\
							 out vec2 passUV;\n										\
							 void main( void )\n									\
							 {\n													\
							 	vec4 localPos = vec4(POSITION,1);\n					\
								vec4 clipPos = PROJECTION * VIEW * MODEL * localPos;\n		\
								gl_Position = clipPos;\n							\
								passColor = COLOR;\n								\
								passUV = UV;\n										\
							 }\n									";

const char* defaultFS =		"#version 420 core\n									\
							in vec4 passColor;\n									\
							out vec4 outColor;\n									\
							void main(void)\n										\
							{\n														\
								outColor = passColor;\n								\
							}\n";

const char* diffuseFS =		"#version 420 core\n									\
							layout(binding = 0) uniform sampler2D gTexDiffuse;\n	\
							in vec2 passUV;\n										\
							in vec4 passColor;\n									\
							out vec4 outColor;\n									\
							void main(void)\n										\
							{\n														\
								vec4 diffuse = texture( gTexDiffuse, passUV );		\
								outColor = diffuse * passColor;\n					\
							}\n";

const char* invalidFS =		"#version 420 core\n									\
							 out vec4 outColor;\n 									\
							 void main(void)\n 										\
							 {\n 													\
							 	outColor = vec4(1,0,1,1);\n							\
							 }\n";

//-----------------------------------------------------------------------------------------------
// Constructor
//
ShaderProgram::ShaderProgram()
{

}

//-----------------------------------------------------------------------------------------------
// Destructor
//
ShaderProgram::~ShaderProgram()
{
	
}

//-----------------------------------------------------------------------------------------------
// Compiles and links shader program from a given src string
//
bool ShaderProgram::LoadShaderFromSource(const char* name, const char* vsSource, const char* fsSource)
{
	GLuint vertShader = CompileShader(vsSource, GL_VERTEX_SHADER, name);
	GLuint fragShader = CompileShader(fsSource, GL_FRAGMENT_SHADER, name);
	m_programHandle = CreateAndLinkProgram( vertShader, fragShader, name ); 
	return (m_programHandle != NULL); 
}

//-----------------------------------------------------------------------------------------------
// Loads the shader from files and links the shaders
//
bool ShaderProgram::LoadFromFiles(const char* vsPath, const char* fsPath /*= nullptr*/, const char* defines /*= nullptr*/)
{
	std::string vsFile = vsPath;
	vsFile += ".vs"; 

	std::string fsFile = vsPath; 
	if(fsPath)
	{
		// If the fs file path is specified it loads the other path
		fsFile = fsPath;
	}
	fsFile += ".fs"; 

	// Compile the two stages we're using (all shaders will implement the vertex and fragment stages)
	// later on, we can add in more stages;
	GLuint vertShader = LoadShader( vsFile.c_str(), GL_VERTEX_SHADER, defines ); 
	if(vertShader == NULL)
	{
		return false; // Invalid vertex shader
	}

	GLuint fragShader = LoadShader( fsFile.c_str(), GL_FRAGMENT_SHADER, defines ); 
	if(fragShader == NULL)
	{
		return false; // Invalid fragment shader
	}

	// Link the program
	// program_handle is a member GLuint. 
	m_programHandle = CreateAndLinkProgram( vertShader, fragShader, vsPath ); 
	glDeleteShader( vertShader ); 
	glDeleteShader( fragShader ); 

	return (m_programHandle != NULL); 
}

//-----------------------------------------------------------------------------------------------
// Compiles the shader and returns a shader_id
//
GLuint CompileShader(const char* src, GLenum type, const char* fileName)
{
	// Create a shader
	GLuint shader_id = glCreateShader(type);
	GUARANTEE_OR_DIE(shader_id != NULL, "Shader could not be created");

	// Bind source to it, and compile
	// You can add multiple strings to a shader – they will 
	// be concatenated together to form the actual source object.
	GLint shader_length = (GLint)strlen(src);
	glShaderSource(shader_id, 1, &src, &shader_length);
	glCompileShader(shader_id);

	// Check status
	GLint status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		DebuggerPrintf("Shader Compiler Error\n");
		LogShaderError(shader_id, fileName); // function we write
		glDeleteShader(shader_id);
		shader_id = NULL;
	}

	return shader_id;
}

//-----------------------------------------------------------------------------------------------
// Compiles and links built-in shaders 
//
void ShaderProgram::InitializeBuiltInShaders()
{
	g_defaultProgram = new ShaderProgram();
	g_invalidProgram = new ShaderProgram();
	g_defDiffuseProgram = new ShaderProgram();

	// Default vertex shader
	g_defaultProgram->LoadShaderFromSource("default shader", defaultVS, defaultFS);
	g_invalidProgram->LoadShaderFromSource("invalid shader", defaultVS, invalidFS);
	g_defDiffuseProgram->LoadShaderFromSource("diffuse shader", defaultVS, diffuseFS);
}

//-----------------------------------------------------------------------------------------------
// Loads shader from file
//
GLuint LoadShader( const char* filename, GLenum type, const char* defines /*= nullptr*/ )
{
	std::string src = (char*)FileReadToNewBuffer(filename);
	GUARANTEE_OR_DIE(!src.empty(), "File could not be loaded");

	if(defines)
	{
		size_t versionIndex = src.find("#version");
		size_t insertIndex = src.find("\n", versionIndex + 1);
		
		StringTokenizer tokenizer(defines, ";");
		tokenizer.Tokenize();

		Strings defineTokens = tokenizer.GetTokens();

		for(std::string token : defineTokens)
		{
			size_t equalIndex = token.find("=");
			if(equalIndex != token.npos)
			{
				// Replace '=' with ' ' 
				token[equalIndex] = ' ';
			}
			token.insert(0, "#define ");
			token.append("\n");
			src.insert(insertIndex + 1, token);
			insertIndex = src.find_first_of("\n", insertIndex + 1);
		}
	}

	GLuint shader_id = CompileShader(src.c_str(), type, filename);

	return shader_id;
}

//-----------------------------------------------------------------------------------------------
// Logs shader compile errors
//
void LogShaderError(GLuint shader_id, const char* fileName)
{
	// figure out how large the buffer needs to be
	GLint length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

	// make a buffer, and copy the log to it. 
	char *buffer = new char[length + 1];
	glGetShaderInfoLog(shader_id, length, &length, buffer);
	
	// Print it out (may want to do some additional formatting)
	buffer[length] = NULL;

	StringTokenizer* allErrorsToks = new StringTokenizer(buffer, "ERROR");
	allErrorsToks->Tokenize();
	Strings allErrors = allErrorsToks->GetTokens();
	for(size_t errorID = 1; errorID < allErrors.size(); errorID++)
	{
		StringTokenizer* errorString = new StringTokenizer(allErrors[errorID], ":");
		errorString->Tokenize();
		Strings errors = errorString->GetTokens();
		if(errors[0] != "ERROR")
		{
			errors[0] = "ERROR";
		}
		std::string solutionDir = SOLUTION_DIR;
		solutionDir.append(fileName);
		
		DebuggerPrintf("%s(%s): %s - %s", solutionDir.c_str(), errors[2].c_str(), errors[3].c_str(), errors[4].c_str());
		if (errors.size() > 5)
		{
			DebuggerPrintf("%s", errors[5].c_str());
		}

	}
	
	// free up the memory we used. 
	delete buffer;
}

//-----------------------------------------------------------------------------------------------
// Creates and links the shader program
//
GLuint CreateAndLinkProgram( GLint vs, GLint fs, const char* fileName )
{
	// create the program handle - how you will reference
	// this program within OpenGL, like a texture handle
	GLuint program_id = glCreateProgram();
	GUARANTEE_OR_DIE( program_id != 0, "Shader could not be created" );

	// Attach the shaders you want to use
	glAttachShader( program_id, vs );
	glAttachShader( program_id, fs );

	// Link the program (create the GPU program)
	glLinkProgram( program_id );

	// Check for link errors - usually a result
	// of incompatibility between stages.
	GLint link_status;
	glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) {
		LogProgramError(program_id, fileName);
		glDeleteProgram(program_id);
		program_id = 0;
	} 

	// no longer need the shaders, you can detach them if you want
	// (not necessary)
	glDetachShader( program_id, vs );
	glDetachShader( program_id, fs );

	return program_id;
}

//-----------------------------------------------------------------------------------------------
// Log shader linking errors
//
void LogProgramError(GLuint program_id, const char* fileName)
{
	// get the buffer length
	GLint length;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);

	// copy the log into a new buffer
	char *buffer = new char[length + 1];
	glGetProgramInfoLog(program_id, length, &length, buffer);

	// print it to the output pane
	buffer[length] = NULL;
	DebuggerPrintf("Shader Linker Error:%s -- %s\n",fileName, buffer);
	        
	// cleanup
	delete buffer;
}


