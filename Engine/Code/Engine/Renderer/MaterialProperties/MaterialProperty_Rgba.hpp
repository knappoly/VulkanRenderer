#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Engine/Renderer/MaterialProperties/MaterialProperty.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations


//-----------------------------------------------------------------------------------------------
class MaterialProperty_Rgba : public MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	explicit MaterialProperty_Rgba( const char* name ) : MaterialProperty(name, DATA_RGBA){}
	explicit MaterialProperty_Rgba( const char* name, const Rgba& data ) : MaterialProperty(name, DATA_RGBA), m_data(data) {}
	~MaterialProperty_Rgba(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	Rgba	GetData() const { return m_data; }
	void	SetData( const Rgba& data ) { m_data = data; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void				Bind( unsigned int programHandle ) override;
	virtual MaterialProperty*	Clone() override;
	
	//-----------------------------------------------------------------------------------------------
	// Members
	Rgba	m_data;
};


