#pragma once
#include "Game/GameState/GameState.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Mesh;
class Shader;
class Transform;
class Material;
class Command;
class ForwardRenderPath;
class RenderScene;
class GameObject;
class ParticleEmitter;

//-----------------------------------------------------------------------------------------------
class GameState_Playing : public GameState
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	GameState_Playing();
	~GameState_Playing();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			bool	DropLightCommand( Command& cmd );
	
	//-----------------------------------------------------------------------------------------------
	// Methods
			void	CreateScene();
	virtual void	ProcessInput() override;
			void	ProcessMouseInput();
	virtual void	Update( float deltaSeconds ) override;
	virtual void	Render() const override;

	//-----------------------------------------------------------------------------------------------
	// Members
	ForwardRenderPath*				m_renderPath;
};

