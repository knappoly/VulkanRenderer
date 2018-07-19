#pragma once
#include "Engine\Core\XMLUtils.hpp"
#include <map>

//-----------------------------------------------------------------------------------------------
// Forward Declarations
class IsoSpriteAnimDefinition;

//-----------------------------------------------------------------------------------------------
class IsoSpriteAnimSetDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSpriteAnimSetDefinition( const XMLElement& element );
	~IsoSpriteAnimSetDefinition(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	static void							InitializeDefinitions( const std::string& filePath );
	static IsoSpriteAnimSetDefinition*	GetDefinition( const std::string& name );
	bool								IsValidAnimName( const std::string& name );
	
	//-----------------------------------------------------------------------------------------------
	// Members
	static std::map<std::string, IsoSpriteAnimSetDefinition*>	s_definitions;

	std::map< std::string, IsoSpriteAnimDefinition* >			m_namedAnimDefs;
	std::string													m_defaultAnimName = "idle";
	std::string													m_setName = "Invalid ID";
};

