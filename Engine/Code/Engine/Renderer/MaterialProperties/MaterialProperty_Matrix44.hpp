#pragma once
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"
#include "Engine/Math/Matrix44.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class MaterialProperty_Matrix44 : public MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit MaterialProperty_Matrix44( const char* name ) : MaterialProperty(name, DATA_MATRIX44) {}
	explicit MaterialProperty_Matrix44( const char* name, const Matrix44& data, bool transpose ) 
		: MaterialProperty(name, DATA_MATRIX44), m_data(data), m_isTranspose(transpose) {}
	~MaterialProperty_Matrix44(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Matrix44	GetData() const { return m_data; }
	void		SetData( const Matrix44& data ) { m_data = data; }
	bool		GetTransposeMode() const { return m_isTranspose; }
	void		SetTransposeMode ( bool mode ) { m_isTranspose = mode; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void				Bind( unsigned int programHandle ) override;
	virtual MaterialProperty*	Clone() override;
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Matrix44	m_data;
	bool		m_isTranspose = false;
};

