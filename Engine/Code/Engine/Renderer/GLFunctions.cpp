#include "Engine/Renderer/GLFunctions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "../Core/StringUtils.hpp"


//-----------------------------------------------------------------------------------------------
// General GL Function pointers
PFNGLCLEARPROC glClear = nullptr;
PFNGLCLEARCOLORPROC glClearColor = nullptr;
PFNGLORTHOFOESPROC glOrtho = nullptr;
PFNGLENABLEPROC glEnable = nullptr;
PFNGLDISABLEPROC glDisable = nullptr;
PFNGLGETERRORPROC glGetError = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;
PFNGLREADBUFFERPROC glReadBuffer = nullptr;
PFNGLREADPIXELSPROC glReadPixels = nullptr;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC glNamedFramebufferReadBuffer = nullptr;
PFNGLVIEWPORTPROC glViewport = nullptr;

// Draw function Pointers
PFNGLDRAWARRAYSPROC glDrawArrays = nullptr;
PFNGLBLENDFUNCPROC glBlendFunc = nullptr;
PFNGLDEPTHMASKPROC glDepthMask = nullptr;
PFNGLDEPTHFUNCPROC glDepthFunc = nullptr;
PFNGLCLEARDEPTHFPROC glClearDepthf = nullptr;
PFNGLLINEWIDTHPROC glLineWidth = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLDRAWELEMENTSPROC glDrawElements = nullptr;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLDRAWBUFFERSPROC glDrawBuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = nullptr;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer = nullptr;
PFNGLPOLYGONMODEPROC glPolygonMode = nullptr;

// Texture function pointers
PFNGLPIXELSTOREIPROC glPixelStorei = nullptr;
PFNGLGENTEXTURESPROC glGenTextures = nullptr;
PFNGLBINDTEXTUREPROC glBindTexture = nullptr;
PFNGLTEXPARAMETERIPROC glTexParameteri = nullptr;
PFNGLTEXIMAGE2DPROC glTexImage2D = nullptr;
PFNGLGENSAMPLERSPROC glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri = nullptr;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf = nullptr;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv = nullptr;
PFNGLDELETESAMPLERSPROC glDeleteSamplers = nullptr;
PFNGLBINDSAMPLERPROC glBindSampler = nullptr;
PFNGLBINDSAMPLERSPROC glBindSamplers = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLGETTEXIMAGEPROC glGetTexImage = nullptr;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D = nullptr;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = nullptr;
PFNGLDELETETEXTURESPROC glDeleteTextures = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;

// Shader function pointers
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = nullptr;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = nullptr;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate = nullptr;
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = nullptr;
PFNGLFRONTFACEPROC glFrontFace = nullptr;
PFNGLCULLFACEPROC glCullFace = nullptr;

//-----------------------------------------------------------------------------------------------
// WGL Function pointers
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

//-----------------------------------------------------------------------------------------------
// Checks for errors thrown by GL functions
//
bool GLCheckError(const char* fileName, int line)
{
#if defined(_DEBUG)
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		/*GUARANTEE_RECOVERABLE( false, Stringf("\nGL ERROR [0x%04x] at [%s(%i)]\n\n", error, fileName, line).c_str() );*/
		DebuggerPrintf("\nGL ERROR [0x%04x] at [%s(%i)]\n\n", error, fileName, line);
		return true; 
	}
#endif
	return false; 
}

//-----------------------------------------------------------------------------------------------
// Returns false 
//
bool GLFailed()
{
	return false;
}

//-----------------------------------------------------------------------------------------------
// Returns true
//
bool GLSucceeded()
{
	return !GLFailed();
}

//-----------------------------------------------------------------------------------------------
// Binds all the OpenGL functions from the library
//
void BindGLFunctions()
{
	GL_BIND_FUNCTION(glClear);
	GL_BIND_FUNCTION(glClearColor);
	GL_BIND_FUNCTION(glOrtho);
	GL_BIND_FUNCTION(glEnable);
	GL_BIND_FUNCTION(glDisable);
	GL_BIND_FUNCTION(glGetError);
	GL_BIND_FUNCTION(glCheckFramebufferStatus);
	GL_BIND_FUNCTION(glReadBuffer);
	GL_BIND_FUNCTION(glReadPixels);
	GL_BIND_FUNCTION(glNamedFramebufferReadBuffer);
	GL_BIND_FUNCTION(glViewport);

	// Texture Stuff
	GL_BIND_FUNCTION(glPixelStorei);
	GL_BIND_FUNCTION(glGenTextures);
	GL_BIND_FUNCTION(glBindTexture);
	GL_BIND_FUNCTION(glTexParameteri);
	GL_BIND_FUNCTION(glTexImage2D);
	GL_BIND_FUNCTION(glGenSamplers);
	GL_BIND_FUNCTION(glSamplerParameteri);
	GL_BIND_FUNCTION(glSamplerParameterf);
	GL_BIND_FUNCTION(glSamplerParameterfv);
	GL_BIND_FUNCTION(glDeleteSamplers);
	GL_BIND_FUNCTION(glBindSamplers);
	GL_BIND_FUNCTION(glBindSampler);
	GL_BIND_FUNCTION(glActiveTexture);
	GL_BIND_FUNCTION(glGetTexImage);
	GL_BIND_FUNCTION(glTexStorage2D);
	GL_BIND_FUNCTION(glTexSubImage2D);
	GL_BIND_FUNCTION(glDeleteTextures);
	GL_BIND_FUNCTION(glGenerateMipmap);
	
	// Draw Stuff
	GL_BIND_FUNCTION(glDrawArrays);
	GL_BIND_FUNCTION(glDrawElements);
	GL_BIND_FUNCTION(glLineWidth);
	GL_BIND_FUNCTION(glBlendFunc);
	GL_BIND_FUNCTION(glDepthFunc);
	GL_BIND_FUNCTION(glDepthMask);
	GL_BIND_FUNCTION(glClearDepthf);
	GL_BIND_FUNCTION(glGenVertexArrays);
	GL_BIND_FUNCTION(glBindVertexArray);
	GL_BIND_FUNCTION(glVertexAttribPointer);
	GL_BIND_FUNCTION(glEnableVertexAttribArray);
	GL_BIND_FUNCTION(glGetAttribLocation);
	GL_BIND_FUNCTION(glBindBuffer);
	GL_BIND_FUNCTION(glBufferData);
	GL_BIND_FUNCTION(glGenBuffers);
	GL_BIND_FUNCTION(glDeleteBuffers);
	GL_BIND_FUNCTION(glGenFramebuffers);
	GL_BIND_FUNCTION(glDeleteFramebuffers);
	GL_BIND_FUNCTION(glDrawBuffers);
	GL_BIND_FUNCTION(glFramebufferTexture);
	GL_BIND_FUNCTION(glBindFramebuffer);
	GL_BIND_FUNCTION(glBlitFramebuffer);
	GL_BIND_FUNCTION(glPolygonMode);

	// Shader Stuff
	GL_BIND_FUNCTION(glCreateShader);
	GL_BIND_FUNCTION(glDeleteShader);
	GL_BIND_FUNCTION(glShaderSource);
	GL_BIND_FUNCTION(glCompileShader);
	GL_BIND_FUNCTION(glAttachShader);
	GL_BIND_FUNCTION(glDetachShader);
	GL_BIND_FUNCTION(glGetShaderiv);
	GL_BIND_FUNCTION(glCreateProgram);
	GL_BIND_FUNCTION(glDeleteProgram);
	GL_BIND_FUNCTION(glLinkProgram);
	GL_BIND_FUNCTION(glUseProgram);
	GL_BIND_FUNCTION(glGetProgramiv);
	GL_BIND_FUNCTION(glGetShaderInfoLog);
	GL_BIND_FUNCTION(glGetProgramInfoLog);
	GL_BIND_FUNCTION(glGetUniformLocation);
	GL_BIND_FUNCTION(glGetUniformBlockIndex);
	GL_BIND_FUNCTION(glUniformBlockBinding);
	GL_BIND_FUNCTION(glBindBufferBase);
	GL_BIND_FUNCTION(glUniformMatrix4fv);
	GL_BIND_FUNCTION(glUniform1f);
	GL_BIND_FUNCTION(glUniform1fv);
	GL_BIND_FUNCTION(glUniform1iv);
	GL_BIND_FUNCTION(glUniform4fv);
	GL_BIND_FUNCTION(glUniform3fv);
	GL_BIND_FUNCTION(glCullFace);
	GL_BIND_FUNCTION(glBlendEquationSeparate);
	GL_BIND_FUNCTION(glBlendFuncSeparate);
	GL_BIND_FUNCTION(glFrontFace);
}

//-----------------------------------------------------------------------------------------------
// Binds the new GL Functions to start the context
//
void BindNewGLFunctions()
{
	GL_BIND_FUNCTION( wglGetExtensionsStringARB ); 
	GUARANTEE_OR_DIE(wglGetExtensionsStringARB != nullptr, "wglGetExtensionsStringARB could not be bound");

	GL_BIND_FUNCTION( wglChoosePixelFormatARB ); 
	GUARANTEE_OR_DIE(wglGetExtensionsStringARB != nullptr, "wglGetExtensionsStringARB could not be bound");
	
	GL_BIND_FUNCTION( wglCreateContextAttribsARB );
	GUARANTEE_OR_DIE(wglGetExtensionsStringARB != nullptr, "wglGetExtensionsStringARB could not be bound");
}


