#include "Game/GameState/GameState.hpp"

//-----------------------------------------------------------------------------------------------
// Updates the state
//
void GameState::Update(float deltaSeconds)
{
	m_timeSinceStateChange += deltaSeconds;
}
