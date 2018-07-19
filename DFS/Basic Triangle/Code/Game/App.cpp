#include "Game/App.hpp"

//-----------------------------------------------------------------------------------------------
// Game Includes
#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/VulkanRenderer/VKRenderer.hpp"
#include "Engine/VulkanRenderer/VKCamera.hpp"
#include "Engine/VulkanRenderer/VkTexture.hpp"
#include "Engine/VulkanRenderer/VKMaterial.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Math/Transform.hpp"
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
// Static globals
static App* g_theApp = nullptr;

//-----------------------------------------------------------------------------------------------
// Constructor
//
App::App( const char* appName )
{
	m_isQuitting = false;
	m_appName = appName;
	

	Clock::CreateMasterClock();

	InputSystem::CreateInstance();
	AudioSystem::CreateInstance();
	VKRenderer::CreateInstance(appName);
	Game::CreateInstance();

	InputSystem::ShowCursor(false);
	InputSystem::SetMouseMode(MOUSE_RELATIVE);
	InputSystem::LockMouseToWindow(true);
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
App::~App()
{
	Game::DestroyInstance();
	VKRenderer::DestroyInstance();
	InputSystem::DestroyInstance();
	AudioSystem::DestroyInstance();
}

//-----------------------------------------------------------------------------------------------
// Updates the App state
void App::Update()
{
	if(m_firstFrame)
	{
		VKRenderer* renderer = VKRenderer::GetInstance();
		m_camera = new VKCamera(renderer);
		m_camera->SetColorTarget(renderer->GetDefaultColorTarget());
		m_camera->SetDepthTarget(renderer->GetDefaultDepthTarget());
		m_camera->SetPerspective(90.f, 1.f, 0.f, 1000.f);

		MeshBuilder builder;
		builder.Begin(PRIMITIVE_TRIANGLES, true);
		builder.AddCube(Vector3::ZERO, Vector3::ONE);
		builder.End();

		//m_cube = new VKMesh(renderer);
		//m_cube->FromBuilder<Vertex_3DPCU>(builder);
		m_cube = renderer->CreateOrGetMesh("Data/Models/scifi_fighter_mk6/scifi_fighter_mk6.obj");

		
		m_material = renderer->CreateOrGetMaterial("Data/Materials/vulkantest.mat");


		m_firstFrame = false;
	}

	float deltaSeconds = (float) Clock::GetMasterDeltaSeconds();
	ClampFloat(deltaSeconds, 0.f, 1.f/MINIMUM_FPS_SUPPORTED);

	// Input handlers
	HandleKeyboardInput();
	HandleMouseInput();

	// Updates the game logic with the calculated deltaSeconds
	Game::GetInstance()->Update();
}

//-----------------------------------------------------------------------------------------------
// Renders the App 
//
void App::Render()
{
	
	Transform model;
	model.SetScaleUniform(0.06f);

	VKRenderer* rend = VKRenderer::GetInstance();
	rend->SetCamera(m_camera);
	rend->SetMaterial(m_material);
	rend->DrawMesh(*m_cube, model.GetWorldMatrix());

// 	Game* gameInstance = Game::GetInstance();
// 	gameInstance->Render();
	
}

//-----------------------------------------------------------------------------------------------
// Starts the begin frame chain on the engine components and sets up screen
//
void App::BeginFrame()
{
	Clock::GetMasterClock()->BeginFrame(); // Ticks the master clock
	InputSystem::GetInstance()->BeginFrame();
	AudioSystem::GetInstance()->BeginFrame();
	VKRenderer::GetInstance()->BeginFrame();
}

//-----------------------------------------------------------------------------------------------
// Starts the end frame chain on the engine components
//
void App::EndFrame()
{
	AudioSystem::GetInstance()->EndFrame();
	InputSystem::GetInstance()->EndFrame();
	Clock::GetMasterClock()->EndFrame();
	VKRenderer::GetInstance()->EndFrame();
	Sleep(1); // For CPU Usage 
}

//-----------------------------------------------------------------------------------------------
// Starts the update chain on the game components and calculates delta time 
//
void App::RunFrame()
{
	BeginFrame();
	Update();
	Render();
	EndFrame();
}

//-----------------------------------------------------------------------------------------------
// Handles keyboard inputs
//
void App::HandleKeyboardInput()
{
// 	if(input->WasKeyJustPressed(KEYCODE_TILDE))
// 	{
// 		if(!IsDevConsoleOpen())
// 		{
// 			DevConsole::GetInstance()->Open();
// 		}
// 
// 		else
// 		{
// 			DevConsole::GetInstance()->Close();
// 		}
// 	}

	InputSystem* input = InputSystem::GetInstance();
	float deltaSeconds = (float) Clock::GetMasterDeltaSeconds();
	float moveSpeed = 2.f;

	if(input->IsKeyDown(KEYCODE_W))
	{
		m_camera->Translate(m_camera->GetForward() * deltaSeconds * moveSpeed);
	}
	if(input->IsKeyDown(KEYCODE_A))
	{
		m_camera->Translate(m_camera->GetRight() * deltaSeconds * -1.f * moveSpeed);
	}
	if(input->IsKeyDown(KEYCODE_S))
	{
		m_camera->Translate(m_camera->GetForward() * deltaSeconds * -1.f * moveSpeed);
	}
	if(input->IsKeyDown(KEYCODE_D))
	{
		m_camera->Translate(m_camera->GetRight() * deltaSeconds * moveSpeed);
	}
	if(input->IsKeyDown(KEYCODE_SPACE))
	{
		m_camera->Translate(m_camera->GetUp() * deltaSeconds * moveSpeed);
	}
	if(input->IsKeyDown(KEYCODE_C))
	{
		m_camera->Translate(m_camera->GetUp() * deltaSeconds * -1.f * moveSpeed);
	}

}

//-----------------------------------------------------------------------------------------------
// Handles mouse input 
//
void App::HandleMouseInput()
{
	float deltaSeconds = (float) Clock::GetMasterDeltaSeconds();
	Vector2 mouseDelta = InputSystem::GetMouseDelta();
	std::swap(mouseDelta.y, mouseDelta.x);
	m_camera->RotateByEuler(mouseDelta * deltaSeconds * 2.f);

	Vector3 camEuler = m_camera->m_transform->GetEulerAngles();
	camEuler.x = ClampFloat(camEuler.x, -90.f, 90.f);
	camEuler.y = fmodf(camEuler.y, 360.f);

	m_camera->m_transform->SetEulerAngles(camEuler);
	m_camera->UpdateMatrices();
}

//-----------------------------------------------------------------------------------------------
// Creates an app instance
//
App* App::CreateInstance( const char* appName )
{
	if(g_theApp == nullptr)
	{
		g_theApp = new App( appName );
	}

	return g_theApp;
}

//-----------------------------------------------------------------------------------------------
// Returns the app instance
//
App* App::GetInstance()
{
	return g_theApp;
}

//-----------------------------------------------------------------------------------------------
// destroys the app instance
//
void App::DestroyInstance()
{
	if(g_theApp)
	{
		delete g_theApp;
		g_theApp = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------
// Returns true if the app is quitting
//
bool App::IsQuitting()
{
	return g_theApp->m_isQuitting;
}

//-----------------------------------------------------------------------------------------------
// Quits the App
//
void App::RequestQuit()
{
	m_isQuitting = true;
}
