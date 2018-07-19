#include "Engine/Renderer/SpriteAnimSet.hpp"
#include "Engine/Renderer/SpriteAnimSetDefinition.hpp"
#include "Engine/Renderer/SpriteAnim.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vector2.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
SpriteAnimSet::SpriteAnimSet(SpriteAnimSetDefinition* animSetDef): m_animSetDef(animSetDef)
{
	std::map<std::string, SpriteAnimDefinition*>::iterator found = m_animSetDef->m_namedAnimDefs.begin();
	while(found != m_animSetDef->m_namedAnimDefs.end())
	{
		SpriteAnim* animClip = new SpriteAnim(found->second);
		m_namedAnims[animClip->GetName()] = animClip;
		found++;
	}
	m_currentAnim = m_namedAnims[animSetDef->m_defaultAnimName]; // Sets the default animation as the start animation
}

//-----------------------------------------------------------------------------------------------
// Destructor
//
SpriteAnimSet::~SpriteAnimSet()
{
	m_namedAnims.clear();
	m_currentAnim = nullptr;
	m_animSetDef = nullptr;
}

//-----------------------------------------------------------------------------------------------
// Updates the AnimationSet's currentAnimation
//
void SpriteAnimSet::Update( float deltaSeconds )
{
	m_currentAnim->Update(deltaSeconds);
}

//-----------------------------------------------------------------------------------------------
// Sets the start animation 
//
void SpriteAnimSet::StartAnim( const std::string& animName )
{
	std::map<std::string, SpriteAnim*>::iterator found = m_namedAnims.find(animName);
	if(found == m_namedAnims.end())
		m_currentAnim = m_namedAnims["Idle"]; //Plays idle if the animation is not found
	else
		m_currentAnim = m_namedAnims[animName];
	
	m_currentAnim->PlayFromStart();
}

//-----------------------------------------------------------------------------------------------
// Sets the current animation
//
void SpriteAnimSet::SetCurrentAnim( const std::string& animName )
{
	m_currentAnim = m_namedAnims[animName];
}

//-----------------------------------------------------------------------------------------------
// Returns the current texture that is being used
//
const Texture& SpriteAnimSet::GetCurrentTexture() const
{
	return m_currentAnim->GetTexture();
}

//-----------------------------------------------------------------------------------------------
// Returns the current UVs for the texture
//
AABB2 SpriteAnimSet::GetCurrentUVs() const
{
	return m_currentAnim->GetCurrentUVs();
}

//-----------------------------------------------------------------------------------------------
// Returns the texture coordinates at mins
//
Vector2 SpriteAnimSet::GetTexCoordsAtMins() const
{
	AABB2 uvs = GetCurrentUVs();
	Vector2 coordsAtMins = uvs.mins;
	return coordsAtMins;
}

//-----------------------------------------------------------------------------------------------
// Returns the texture coordinates at maxs
//
Vector2 SpriteAnimSet::GetTexCoordsAtMaxs() const
{
	AABB2 uvs = GetCurrentUVs();
	Vector2 coordsAtMaxs = uvs.maxs;
	return coordsAtMaxs;
}

//-----------------------------------------------------------------------------------------------
// Returns the name of the current animation clip
//
std::string SpriteAnimSet::GetCurrentAnimName() const
{
	return m_currentAnim->GetName();
}

