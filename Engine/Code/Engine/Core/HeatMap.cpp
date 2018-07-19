#include "Engine/Core/HeatMap.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor 
//
HeatMap::HeatMap(const IntVector2& dimensions):m_dimensions(dimensions)
{
	InitializeHeatValues();
}

//-----------------------------------------------------------------------------------------------
// Sets the heat of the object at the coordinates specified (Default: RowMajor)
//
void HeatMap::SetHeat(const IntVector2& coords, float newHeatValue, bool rowMajor /*= true */) 
{	
	int index = GetIndex(coords, rowMajor);
	if(index == -1) 
		return;

	m_heatValues[index] = newHeatValue;
}

//-----------------------------------------------------------------------------------------------
// Returns the heat value of the object at the coordinates specified (Default: RowMajor)
//
float HeatMap::GetHeat(const IntVector2& coords, bool rowMajor /*= true */) const
{
	int index = GetIndex(coords, rowMajor);
	if(index == -1) 
		return 9999.f;
	return m_heatValues[index];
}

//-----------------------------------------------------------------------------------------------
// Returns the heat value of the object at the coordinates specified (Default: RowMajor)
//
float HeatMap::GetHeat(int index, bool rowMajor) const
{
	return GetHeat(GetCoords(index, rowMajor), rowMajor);
}

//-----------------------------------------------------------------------------------------------
// Adds the heat value to the current heat value of the object (Default: RowMajor)
//
void HeatMap::AddHeat(const IntVector2& coords, float heatToAdd, bool rowMajor /*= true */)
{
	int index = GetIndex(coords, rowMajor);
	if(index == -1) 
		return;
	m_heatValues[index] += heatToAdd;
}

//-----------------------------------------------------------------------------------------------
// Returns the index depending on RowMajor or ColMajor (Default: RowMajor)
//
int HeatMap::GetIndex(const IntVector2& coords, bool rowMajor /*= true */) const
{
	int index;
	if(coords.x < 0 || coords.y < 0 || coords.x > m_dimensions.x || coords.y > m_dimensions.y)
		return -1;
	if(rowMajor)
		index = (coords.y * m_dimensions.x) + coords.x;
	else
		index = (coords.x * m_dimensions.y) + coords.y;

	return index;
}

//-----------------------------------------------------------------------------------------------
// Returns the index from coords  (Default: RowMajor)
//
int HeatMap::GetIndex(int x, int y, bool rowMajor /*= true */) const
{
	return GetIndex(IntVector2(x,y), rowMajor);
}

//-----------------------------------------------------------------------------------------------
// Returns the coordinates for the index given (Default:RowMajor)
//
IntVector2 HeatMap::GetCoords(int index, bool rowMajor /*= true */) const
{
	IntVector2 coords;
	if(rowMajor)
	{
		coords.x = index % m_dimensions.x;
		coords.y = index / m_dimensions.x;
	}
	else
	{
		coords.y = index % m_dimensions.x;
		coords.x = index / m_dimensions.x;
	}
	return coords;
}

//-----------------------------------------------------------------------------------------------
// Initializes the heat values to 9999.f 
//
void HeatMap::InitializeHeatValues()
{
	if((int) m_heatValues.size() < GetNumValues())
	{
		// Initialize heat values to 9999.f
		for (int index = 0; index < GetNumValues(); ++index)
		{
			m_heatValues.push_back(9999.f);
		}
	}
	else
	{
		for (int index = 0; index < GetNumValues(); ++index)
		{
			m_heatValues[index] = 9999.f;
		}
	}
}
