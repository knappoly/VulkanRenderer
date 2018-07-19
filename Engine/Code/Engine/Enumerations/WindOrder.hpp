#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum WindOrder 
{
	WIND_CLOCKWISE,         // GL_CW       glFrontFace( GL_CW ); 
	WIND_COUNTER_CLOCKWISE, // GL_CCW      glFrontFace( GL_CCW ); 
};
static const std::map<std::string, WindOrder> ParseWindOrder = 
{
	{"cw",	WIND_CLOCKWISE},
	{"ccw",	WIND_COUNTER_CLOCKWISE}
};

