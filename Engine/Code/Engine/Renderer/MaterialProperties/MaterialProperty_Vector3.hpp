#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class MaterialProperty_Vector3 : public MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit MaterialProperty_Vector3( const char* name ) : MaterialProperty(name, DATA_VECTOR3) {}
	explicit MaterialProperty_Vector3( const char* name, const Vector3& data ) : MaterialProperty(name, DATA_VECTOR3), m_data(data) {}
	~MaterialProperty_Vector3(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Vector3	GetData() const { return m_data; }
	void	SetData( const Vector3& data ) { m_data = data; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void				Bind( unsigned int programHandle );
	virtual MaterialProperty*	Clone() override;

	//-----------------------------------------------------------------------------------------------
	// Members
	Vector3	m_data;
};

