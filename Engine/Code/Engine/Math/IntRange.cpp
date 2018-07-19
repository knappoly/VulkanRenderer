#include "Engine/Math/IntRange.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
IntRange::IntRange(int minInclusive, int maxInclusive): min(minInclusive), max(maxInclusive)
{

}

//-----------------------------------------------------------------------------------------------
// Constructor 
//
IntRange::IntRange(int rangeMinMax):min(rangeMinMax), max(rangeMinMax)
{

}

//-----------------------------------------------------------------------------------------------
// Returns a random integer within the range
//
int IntRange::GetRandomInRange() const
{
	return GetRandomIntInRange(min, max);
}

//-----------------------------------------------------------------------------------------------
// Returns true if the ranges overlap
// 
bool DoRangesOverlap( const IntRange& a, const IntRange& b )
{
	UNUSED(a);
	UNUSED(b);
	return false;
}

//-----------------------------------------------------------------------------------------------
// Interpolates a range from the given two ranges 
//
const IntRange Interpolate( const IntRange& start, const IntRange& end, float fractionTowardEnd )
{
	IntRange temp;
	temp.min = Interpolate(start.min, end.min, fractionTowardEnd);
	temp.max = Interpolate(start.max, end.max, fractionTowardEnd);
	return temp;
}

void IntRange::SetFromText( const char* text )
{
	std::string range = text;
	std::string newMin, newMax;
	size_t tildeIndex = 0;

	//Find the '~' sign
	tildeIndex = range.find("~", tildeIndex);
	if(tildeIndex > 0)
	{
		newMin = std::string(range, 0, tildeIndex);
		newMax = std::string(range, tildeIndex+1);
		GUARANTEE_OR_DIE(!newMin.empty(), "Range value requires format min~max");
		GUARANTEE_OR_DIE(!newMax.empty(), "Range value requires format min~max");
		min = atoi(newMin.c_str());
		max = atoi(newMax.c_str());
	}
	else
	{
		newMin = std::string(range, 0, tildeIndex);
		newMax = newMin;
		GUARANTEE_OR_DIE(!newMin.empty(), "Range value requires atleast one value");
		min = atoi(newMin.c_str());
		max = min;
	}
	//DebuggerPrintf("Range: %d ~ %d ", min,max);
}