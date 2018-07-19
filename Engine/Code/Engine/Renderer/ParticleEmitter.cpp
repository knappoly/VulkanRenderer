#include "Engine/Renderer/ParticleEmitter.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Renderer/Renderable.hpp"
#include "Engine/Math/Transform.hpp"
#include "Engine/Renderer/Mesh/Mesh.hpp"
#include "Engine/Renderer/Mesh/MeshBuilder.hpp"
#include "Engine/Renderer/Mesh/MeshUtils.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Material.hpp"
#include "Engine/Core/StopWatch.hpp"
#include "DebugRenderUtils.hpp"

//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Updates the particle
//
void Particle::Update(float deltaSeconds)
{
	Vector3 acceleration = force / mass; 
	velocity += deltaSeconds * acceleration; 
	position += deltaSeconds * velocity; 

	force = Vector3::ZERO;
}

//-----------------------------------------------------------------------------------------------
// Returns the normalized age
//
float Particle::GetNormalizedAge(float time)
{
	float duration = timeWillDie - timeBorn;
	float elapsed = time - timeBorn;
	
	return elapsed * 1.f / duration;
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
ParticleEmitter::ParticleEmitter()
{
	m_renderable = new Renderable();
	m_transform = new Transform();
	m_builder = new MeshBuilder();
	m_clock = Clock::GetMasterClock();
	m_mesh = new Mesh();
	m_renderable->SetMesh(m_mesh);
	m_renderable->SetWatchTransform(nullptr);
	m_interval = new StopWatch();
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
ParticleEmitter::~ParticleEmitter()
{
	delete m_builder;
	m_builder = nullptr;

	delete m_transform;
	m_transform = nullptr;

	delete m_renderable;
	m_renderable = nullptr;

	m_clock = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Sets the spawn rate
//
void ParticleEmitter::SetSpawnRate(float particlesPerSecond)
{
	m_spawnRate = particlesPerSecond;
	m_spawnsOverTime = m_spawnRate;
	
	if(m_spawnsOverTime)
	{
		m_interval->SetTimer(1.f / m_spawnRate);
	}
}

//-----------------------------------------------------------------------------------------------
// Sets the material on the renderable
//
void ParticleEmitter::SetMaterial(const Material* mat)
{
	m_renderable->SetMaterial(*mat);
}

//-----------------------------------------------------------------------------------------------
// Destroy when the num particles is 0 and spawnRate is 0
//
bool ParticleEmitter::IsReadyToDestroy() const
{
	bool ready = !m_spawnsOverTime && m_particles.size();
	return !ready;
}

//-----------------------------------------------------------------------------------------------
// Sets the size range
//
void ParticleEmitter::SetSizeRange(float min, float max)
{
	m_sizeRange.min = min;
	m_sizeRange.max = max;
}

//-----------------------------------------------------------------------------------------------
// Sets the lifetime range
//
void ParticleEmitter::SetLifeRange(float min, float max)
{
	m_lifeTimeRange.min = min;
	m_lifeTimeRange.max = max;
}

//-----------------------------------------------------------------------------------------------
// Sets the callback that returns velocity vector
//
void ParticleEmitter::SetVelocityCB(VelocityCB cb)
{
	m_velocityCB = cb;
}

//-----------------------------------------------------------------------------------------------
// Sets the color range
//
void ParticleEmitter::SetColorRange(const Rgba& min, const Rgba& max)
{
	m_color1 = min;
	m_color2 = max;
}

//-----------------------------------------------------------------------------------------------
// Aligns the particles towards the camera
//
void ParticleEmitter::PreRender(Camera* cam)
{
	Vector3 right = cam->GetRight(); 
	Vector3 up = cam->GetUp(); 

	m_builder->Flush();
	m_builder->Begin(PRIMITIVE_TRIANGLES, true);
	for (size_t index = 0; index < m_particles.size(); ++index) 
	{
		Particle& p = m_particles[index]; 
		m_builder->SetColor(p.color);
		m_builder->AddPlane(p.position, right, up, Vector2::ONE * p.size);
	}
	m_builder->End();
	m_mesh->FromBuilder<Vertex_3DPCU>(*m_builder);
}

//-----------------------------------------------------------------------------------------------
// Aligns particles towards camera and updates the emitter
//
void ParticleEmitter::Update(float deltaSeconds)
{
	int count = m_spawnsOverTime ? m_interval->DecrementAll() : 0;
	SpawnBurst(count);

	float time = (float) m_clock->GetTime(); 

	for (size_t index = 0; index < m_particles.size(); ++index) 
	{
		Particle& p = m_particles[index]; 
		p.force = m_force;
		p.Update(deltaSeconds); 

		float normalAge = p.GetNormalizedAge(time);
		if(m_sizeOverTime)
		{
			p.size = Interpolate(m_sizeRange.max, m_sizeRange.min, normalAge);
		}
		if(m_colorOverTime)
		{
			p.color = Interpolate(m_color1, m_color2, normalAge);
		}

		if (p.IsDead(time)) 
		{
			m_particles[index] = m_particles[m_particles.size() - 1];
			m_particles.pop_back();
			index--;
		} 
	}

}

//-----------------------------------------------------------------------------------------------
// Spawns a particle
//
void ParticleEmitter::SpawnParticle()
{
	Particle p; 
	p.position = m_transform->GetWorldPosition(); // Model matrix moves it to the correct position
	
	if(m_velocityCB)
	{
		p.velocity = m_velocityCB();
	}
	else
	{
		p.velocity = m_velocity;
	}
	p.size = m_sizeRange.GetRandomInRange();
	

	float lifetime = m_lifeTimeRange.GetRandomInRange();

	p.timeBorn = (float) m_clock->GetTime();
	p.timeWillDie = p.timeBorn + lifetime; 

	p.force = Vector3::ZERO; 
	p.mass = 1.0f; 
	p.color = Interpolate(m_color1, m_color2, GetRandomFloatZeroToOne());
	m_particles.push_back( p ); 
}

//-----------------------------------------------------------------------------------------------
// Spawns a burst of particles
//
void ParticleEmitter::SpawnBurst(int count)
{
	for(int index = 0; index < count; ++index)
	{
		SpawnParticle();
	}
}
