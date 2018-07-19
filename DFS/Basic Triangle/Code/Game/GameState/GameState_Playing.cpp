#include "Game/GameState/GameState_Playing.hpp"

//-----------------------------------------------------------------------------------------------
// Game Includes
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Game/GameObject.hpp"

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Console/DevConsole.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/DebugRenderUtils.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Console/CommandDefinition.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/ForwardRenderPath.hpp"
#include "Engine/Renderer/Renderable.hpp"
#include "Engine/Renderer/RenderScene.hpp"
#include "Engine/Renderer/Lights/Light.hpp"
#include "Engine/Renderer/ParticleEmitter.hpp"
#include "Engine/Audio/AudioSystem.hpp"

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Constructor
//
GameState_Playing::GameState_Playing() : GameState()
{
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
GameState_Playing::~GameState_Playing()
{
}

//-----------------------------------------------------------------------------------------------
// Creates the game scene 
//
void GameState_Playing::CreateScene()
{
}

//-----------------------------------------------------------------------------------------------
// Processes input 
//
void GameState_Playing::ProcessInput()
{

}

//-----------------------------------------------------------------------------------------------
// Processes mouse input
//
void GameState_Playing::ProcessMouseInput()
{
	
}

//-----------------------------------------------------------------------------------------------
// Updates the game state
//
void GameState_Playing::Update(float deltaSeconds)
{
	GameState::Update(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------
// Renders the game state
//
void GameState_Playing::Render() const
{
}
