#include "Engine/Renderer/External/GL/glcorearb.h"

//-----------------------------------------------------------------------------------------------
class ShaderProgram
{
public:

	  //-----------------------------------------------------------------------------------------------
	  // Constructor/Destructor
      ShaderProgram();
      ~ShaderProgram();

	  //-----------------------------------------------------------------------------------------------
	  // Methods
	  bool LoadShaderFromSource(const char* name, const char* vsSource, const char* fsSource ); // Loads a shader program from the source
      bool LoadFromFiles( const char* vsPath, const char* fsPath = nullptr, const char* defines = nullptr ); // load a shader from file
	  
	  GLuint GetHandle() const{ return m_programHandle; }
	  static void InitializeBuiltInShaders();

	  //-----------------------------------------------------------------------------------------------
	  // Members
      GLuint m_programHandle; // OpenGL handle for this program, default 0
};

//-----------------------------------------------------------------------------------------------
// Standalone functions
static GLuint LoadShader(const char* filename, GLenum type, const char* defines = nullptr);
static void LogShaderError(GLuint shader_id, const char* fileName);
static GLuint CreateAndLinkProgram(GLint vs, GLint fs, const char* fileName);
static void LogProgramError(GLuint program_id, const char* fileName);
static GLuint CompileShader(const char* src, GLenum type, const char* fileName);

//-----------------------------------------------------------------------------------------------
// Built-in Shaders

extern ShaderProgram*		g_defaultProgram;
extern ShaderProgram*		g_invalidProgram;
extern ShaderProgram*		g_defDiffuseProgram;

extern const char*			defaultFS;
extern const char*			defaultVS;
extern const char*			invalidFS;
