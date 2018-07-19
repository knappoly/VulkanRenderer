#pragma once
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class MaterialProperty_Float : public MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit MaterialProperty_Float( const char* name ) : MaterialProperty(name, DATA_FLOAT){}
	explicit MaterialProperty_Float( const char* name, float data ) : MaterialProperty(name, DATA_FLOAT), m_data(data) {}
	~MaterialProperty_Float(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			float	GetData() const { return m_data; }
			void	SetData( float data ) { m_data = data; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void				Bind( unsigned int programHandle ) override;
	virtual MaterialProperty*	Clone() override;								
	
	//-----------------------------------------------------------------------------------------------
	// Members
	float	m_data;
};

