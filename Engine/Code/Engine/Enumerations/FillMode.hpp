#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum FillMode 
{
	FILLMODE_SOLID,         // GL_FILL     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL )
	FILLMODE_WIRE,          // GL_LINE     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE )
};
static const std::map<std::string, FillMode> ParseFillMode = 
{
	{"solid",	FILLMODE_SOLID},
	{"wire",	FILLMODE_WIRE}
};

