#pragma once
#include <string>

//-----------------------------------------------------------------------------------------------
// Forward Declarations

//-----------------------------------------------------------------------------------------------
enum DataType
{
	DATA_INVALID = -1,
	DATA_INT,
	DATA_FLOAT,
	DATA_MATRIX44,
	DATA_RGBA,
	DATA_VECTOR3,
	NUM_DATA_TYPES
};

//-----------------------------------------------------------------------------------------------
class MaterialProperty
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	MaterialProperty( const char* name, DataType type ) : m_name(name), m_type(type) {}
	virtual ~MaterialProperty(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
			std::string	GetName() const { return m_name; }
			void		SetType( DataType type ) { m_type = type; }
			DataType	GetType() const { return m_type; }
	
	//-----------------------------------------------------------------------------------------------
	// Methods
	virtual void					Bind( unsigned int programHandle ) = 0; // Pure virtual
	virtual MaterialProperty*		Clone() = 0;

	//-----------------------------------------------------------------------------------------------
	// Members
	std::string	m_name = "";
	DataType	m_type;
};

