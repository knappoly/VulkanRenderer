#pragma once

//-----------------------------------------------------------------------------------------------
// Forward declarations
class GameState;

//-----------------------------------------------------------------------------------------------
// Extern declarations

extern	bool		g_isPaused;
extern	bool		g_isTimeSlow;

//-----------------------------------------------------------------------------------------------
// Constants
const		float MINIMUM_FPS_SUPPORTED = 30.f;
constexpr	float CLIENT_ASPECT = 1.0f;

//-----------------------------------------------------------------------------------------------
// Game Globals
extern GameState*	g_curState;

//-----------------------------------------------------------------------------------------------
// Constants
const float	GRAVITY = 9.8f;

