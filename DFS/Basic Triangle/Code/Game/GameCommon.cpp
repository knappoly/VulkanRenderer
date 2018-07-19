#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"

//-----------------------------------------------------------------------------------------------
bool		 g_isPaused = false;
bool		 g_isTimeSlow = false;

//-----------------------------------------------------------------------------------------------
const	float ORTHO_NEAR = 0.f;
const	float ORTHO_FAR = 1.f;

//-----------------------------------------------------------------------------------------------
GameState*	g_curState = nullptr;

