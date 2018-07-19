#pragma once
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class MaterialProperty_Int : public MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit MaterialProperty_Int( const char* name ) : MaterialProperty(name, DATA_INT) {}
	explicit MaterialProperty_Int( const char* name, int data ) : MaterialProperty(name, DATA_INT), m_data(data) {}
	~MaterialProperty_Int(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	int		GetData() const { return m_data; }
	void	SetData( int data ) { m_data = data; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void				Bind( unsigned int programHandle ) override;
	virtual MaterialProperty*	Clone() override;

	//-----------------------------------------------------------------------------------------------
	// Members
	int m_data;
};

