
//-----------------------------------------------------------------------------------------------
class FloatRange
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructor
	FloatRange() {min = max = 0.f;}
	explicit FloatRange(float minInclusive, float maxInclusive);
	explicit FloatRange(float rangeMinMax);
	~FloatRange(){}

	//-----------------------------------------------------------------------------------------------
	// Methods
	float	GetRandomInRange() const;
	void	SetFromText( const char* text); // Parses the float range from the string
	
	//-----------------------------------------------------------------------------------------------
	// Members
	float min;
	float max;
};

const FloatRange Interpolate(const FloatRange& start, const FloatRange& end, float fractionTowardEnd);
