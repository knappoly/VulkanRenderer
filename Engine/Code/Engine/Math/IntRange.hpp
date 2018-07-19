#pragma once

//-----------------------------------------------------------------------------------------------
class IntRange
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors
	IntRange(){}
	explicit IntRange(int minInclusive, int maxInclusive);
	explicit IntRange(int rangeMinMax);
	~IntRange(){}
	

	//-----------------------------------------------------------------------------------------------
	// Methods
	int GetRandomInRange() const;
	void SetFromText( const char* text );

	//-----------------------------------------------------------------------------------------------
	// Members
	int min;
	int max;
};

bool DoRangesOverlap( const IntRange& a, const IntRange& b );
const IntRange Interpolate( const IntRange& start, const IntRange& end, float fractionTowardEnd );
