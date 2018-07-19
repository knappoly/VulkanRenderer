#pragma once
#include <map>
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
enum RenderQueue
{
	RENDER_QUEUE_OPAQUE,
	RENDER_QUEUE_ADDITIVE,
	RENDER_QUEUE_ALPHA
};
static const std::map<std::string, RenderQueue> ParseRenderQueue =
{
	{"opaque",			RENDER_QUEUE_OPAQUE},
	{"additive",		RENDER_QUEUE_ADDITIVE},
	{"alpha",			RENDER_QUEUE_ALPHA}
};
