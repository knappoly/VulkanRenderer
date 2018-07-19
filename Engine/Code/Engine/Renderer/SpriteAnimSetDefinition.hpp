#pragma once
#include "Engine/Core/XMLUtils.hpp"
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class Renderer;
class SpriteAnimDefinition;

//-----------------------------------------------------------------------------------------------
class SpriteAnimSetDefinition
{
	friend class SpriteAnimSet;

	//-----------------------------------------------------------------------------------------------
	// Constructor/Destructor
public:
	SpriteAnimSetDefinition( const XMLElement& animSetElement, Renderer& renderer );
	~SpriteAnimSetDefinition();

	//-----------------------------------------------------------------------------------------------
	// Members
protected:
	std::map< std::string, SpriteAnimDefinition* >	m_namedAnimDefs;
	std::string										m_defaultAnimName = "Idle";
};
