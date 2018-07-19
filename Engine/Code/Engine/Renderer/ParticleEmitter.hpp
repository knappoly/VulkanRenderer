#pragma once
#include "Engine\Math\Vector3.hpp"
#include <vector>
#include "Engine\Core\Rgba.hpp"
#include "Engine\Math\FloatRange.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Camera;
class Mesh;
class MeshBuilder;
class Transform;
class Renderable;
class Clock;
class Material;
class StopWatch;

typedef Vector3 (*VelocityCB) ();

//-----------------------------------------------------------------------------------------------
struct Particle
{
	// Methods
			void	Update( float deltaSeconds );
	inline	bool	IsDead( float realTime ) { return realTime >= timeWillDie; }
			float	GetNormalizedAge( float realTime );

	// Members
	Vector3 position;
	Vector3	velocity;
	Vector3 force;
	Rgba	color = Rgba::WHITE;
	float	size;
	float	mass;
	float	timeBorn;
	float	timeWillDie;
};

//-----------------------------------------------------------------------------------------------
class ParticleEmitter
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	ParticleEmitter();
	~ParticleEmitter();
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void	SetSpawnRate( float particlesPerSecond );
	void	SetClock( Clock* clock ) { m_clock = clock; }
	void	SetMaterial( const Material* mat );
	void	SetKillAutomatic( bool flag ) { m_killWhenDone = flag; }
	bool	IsReadyToDestroy() const;
	void	SetSizeRange( float min, float max );
	void	SetLifeRange( float min, float max );
	void	SetVelocity( const Vector3& velocity ) { m_velocity = velocity; }
	void	SetVelocityCB( VelocityCB cb );
	void	SetColorRange( const Rgba& min, const Rgba& max );
	void	SetForce( const Vector3& force ) { m_force = force; }
	void	SetSizeOverTime( bool flag ) { m_sizeOverTime = flag; }
	void	SetColorOverTime( bool flag ) { m_colorOverTime = flag; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	void	PreRender( Camera* cam );
	void	Update( float deltaSeconds );
	void	SpawnParticle();
	void	SpawnBurst( int count );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Transform*				m_transform;
	Renderable*				m_renderable;
	Mesh*					m_mesh;
	MeshBuilder*			m_builder;
	bool					m_spawnsOverTime = false;
	bool					m_colorOverTime = false;
	bool					m_sizeOverTime = false;
	bool					m_killWhenDone = false;
	float					m_spawnRate = 0.f;
	StopWatch*				m_interval;
	std::vector<Particle>	m_particles;
	Clock*					m_clock = nullptr;
	FloatRange				m_sizeRange = FloatRange(.1f, .3f);
	FloatRange				m_lifeTimeRange = FloatRange(1.f, 3.f);
	VelocityCB				m_velocityCB;
	Vector3					m_velocity = Vector3::FORWARD;
	Vector3					m_force = Vector3::ZERO;
	Rgba					m_color1 = Rgba::WHITE;
	Rgba					m_color2 = Rgba::WHITE;
};

