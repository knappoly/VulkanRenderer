#pragma once
#include "Engine\Core\XMLUtils.hpp"
#include <map>
#include <string>
#include "Engine\Math\IntVector2.hpp"
#include "Engine\Math\Vector2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
struct IsoSpriteParams
{
	std::string m_name;
	Vector2		m_scale;
};

//-----------------------------------------------------------------------------------------------
class IsoSpriteDefinition
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	IsoSpriteDefinition( const XMLElement& element );
	~IsoSpriteDefinition(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			IsoSpriteParams*		GetSpriteParamForDirection( const IntVector2& direction ) const;
			IsoSpriteParams*		GetDefaultFacingSpriteParams() const;
			IsoSpriteParams*		GetSpriteParamForCamera( const Vector3& cameraForward, const Vector3& cameraRight, const Vector3& objectForward ) const;
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	static	void					InitializeDefinitions( const std::string& filePath );
	static	IsoSpriteDefinition*	GetDefinition( const std::string& name );

	//-----------------------------------------------------------------------------------------------
	// Members
	static	std::map<std::string, IsoSpriteDefinition*>	s_definitions;

			std::string									m_name = "Invalid ID";
			std::map<std::string, IsoSpriteParams*>		m_facingSprites; // Stores the sprite name for the direction(key)
};

