#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum CullMode 
{
	CULLMODE_BACK,			// GL_BACK     glEnable(GL_CULL_FACE); glCullFace(GL_BACK); 
	CULLMODE_FRONT,			// GL_FRONT    glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); 
	CULLMODE_NONE,			// GL_NONE     glDisable(GL_CULL_FACE)
};
static const std::map<std::string, CullMode> ParseCullMode = 
{
	{"back",	CULLMODE_BACK},
	{"front",	CULLMODE_FRONT},
	{"none",	CULLMODE_NONE}
};

