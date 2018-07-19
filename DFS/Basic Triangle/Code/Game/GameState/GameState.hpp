#pragma once
#include <vector>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class RenderScene;
class GameObject;
class ParticleEmitter;

//-----------------------------------------------------------------------------------------------
class GameState
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	GameState(){}
	virtual ~GameState(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void Update( float deltaSeconds ); 
	virtual void Render() const = 0; // pure virtual
	virtual void ProcessInput() = 0;
	
	//-----------------------------------------------------------------------------------------------
	// Members
	float m_timeSinceStateChange = 0.f;
	RenderScene*					m_scene;
	std::vector<GameObject*>		m_gameObjects;
	std::vector<ParticleEmitter*>	m_emitters;
};

