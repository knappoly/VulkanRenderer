#pragma once

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class VKCamera;
class VKMaterial;
class VKMesh;

//-----------------------------------------------------------------------------------------------
class App
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	App( const char* appName );
	~App();

	//-----------------------------------------------------------------------------------------------
	// Methods
	void Update();
	void Render();
	void BeginFrame();
	void EndFrame();
	void RunFrame();
	void RequestQuit();
	void HandleKeyboardInput();
	void HandleMouseInput();

	//-----------------------------------------------------------------------------------------------
	// Static methods
	static App* CreateInstance( const char* appName );
	static App* GetInstance();
	static void DestroyInstance();
	static bool IsQuitting();

	//-----------------------------------------------------------------------------------------------
	// Member Variables
			bool	m_isQuitting;
	const	char*	m_appName = nullptr;
			VKCamera* m_camera = nullptr;
			VKMaterial* m_material = nullptr;
			VKMesh*		m_cube = nullptr;
			bool		m_firstFrame = true;
};


