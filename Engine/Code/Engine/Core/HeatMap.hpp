#pragma once
#include <vector>
#include "Engine\Math\IntVector2.hpp"

//-----------------------------------------------------------------------------------------------
// Forward Declarations
typedef std::vector<float> Floats;

//-----------------------------------------------------------------------------------------------
class HeatMap
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructors
	HeatMap(){}
	HeatMap( const IntVector2& dimensions );
	~HeatMap(){}
	
	//-----------------------------------------------------------------------------------------------
	// Accessors/Mutators
	void		SetHeat( const IntVector2& coords, float newHeatValue, bool rowMajor = true );
	float		GetHeat( const IntVector2& coords, bool rowMajor = true ) const;
	float		GetHeat( int index, bool rowMajor = true ) const;
	void		AddHeat( const IntVector2& coords, float heatToAdd, bool rowMajor = true );
	int			GetIndex( const IntVector2& coords, bool rowMajor = true ) const;
	int			GetIndex( int x, int y, bool rowMajor = true ) const;
	IntVector2	GetCoords( int index, bool rowMajor = true ) const;
	int			GetNumValues() const { return m_dimensions.x * m_dimensions.y; } // Returns the total number of heat values
	IntVector2	GetDimensions() const { return m_dimensions; }

	//-----------------------------------------------------------------------------------------------
	// Methods
	void	InitializeHeatValues();

	//-----------------------------------------------------------------------------------------------
	// Members
private:
	IntVector2	m_dimensions;
	Floats		m_heatValues;
};

