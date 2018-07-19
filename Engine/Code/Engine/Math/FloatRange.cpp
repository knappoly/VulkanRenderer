#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <string>
#include "Engine/Core/ErrorWarningAssert.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
FloatRange::FloatRange(float minInclusive, float maxInclusive): min(minInclusive), max(maxInclusive)
{
}

//-----------------------------------------------------------------------------------------------
// Constructor
//
FloatRange::FloatRange(float rangeMinMax): min(rangeMinMax), max(rangeMinMax)
{
}

//-----------------------------------------------------------------------------------------------
// Returns a random float in the range
//
float FloatRange::GetRandomInRange() const
{
	return GetRandomFloatInRange(min, max);
}

//-----------------------------------------------------------------------------------------------
// String to floatrange parser
//
void FloatRange::SetFromText(const char* text)
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
		min = static_cast<float>(atof(newMin.c_str()));
		max = static_cast<float>(atof(newMax.c_str()));
	}
	else
	{
		newMin = std::string(range, 0, tildeIndex);
		newMax = newMin;
		GUARANTEE_OR_DIE(!newMin.empty(), "Range value requires atleast one value");
		min = static_cast<float>(atof(newMin.c_str()));
		max = min;
	}
	//DebuggerPrintf("Range: %f ~ %f ", min,max);
}

//-----------------------------------------------------------------------------------------------
//
// Interpolates a range from the given two ranges 
// 
const FloatRange Interpolate(const FloatRange& start, const FloatRange& end, float fractionTowardEnd)
{
	FloatRange temp;
	temp.min = Interpolate(start.min, end.min, fractionTowardEnd);
	temp.max = Interpolate(start.max, end.max, fractionTowardEnd);
	return temp;
}