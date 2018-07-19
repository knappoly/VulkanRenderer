#include "AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

const AABB2 AABB2::ZERO = AABB2(Vector2::ZERO, Vector2::ZERO);
const AABB2 AABB2::ZERO_TO_ONE = AABB2(Vector2(0.f, 0.f), Vector2(1.f, 1.f));
typedef std::vector<std::string> Strings;

AABB2::AABB2(const AABB2& copy)
{
	mins = copy.mins;
	maxs = copy.maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	mins = Vector2(minX, minY);
	maxs = Vector2(maxX, maxY);
}

AABB2::AABB2(const Vector2 & mins, const Vector2 & maxs)
{
	this->mins = mins;
	this->maxs = maxs;
}

AABB2::AABB2(const Vector2 & center, float radiusX, float radiusY)
{
	maxs.x = center.x + radiusX;
	maxs.y = center.y + radiusY;

	mins.x = center.x - radiusX;
	mins.y = center.y - radiusY;
}

void AABB2::StretchToIncludePoint(float x, float y)
{
	if(IsPointInside(Vector2(x,y)))
		return;

	StretchToIncludePoint(Vector2(x,y));
}

void AABB2::StretchToIncludePoint(const Vector2 & point)
{
	if(IsPointInside(Vector2(point.x,point.y)))
		return;

	if(point.x < mins.x )
	{
		mins.x = point.x;

		if(point.y < mins.y)
			mins.y = point.y;
		if(point.y>maxs.y)
			maxs.y = point.y;
	}
	if(point.x > mins.x && point.x < maxs.x)
	{
		if(point.y < mins.y)
			mins.y = point.y;
		if(point.y>maxs.y)
			maxs.y = point.y;
	}

	if(point.x > maxs.x )
	{
		maxs.x = point.x;
		if(point.y < mins.y)
			mins.y = point.y;
		if(point.y>maxs.y)
			maxs.y = point.y;
	}
}

void AABB2::AddPaddingToSides(float xPaddingRadius, float yPaddingRadius)
{
	mins -= Vector2(xPaddingRadius,yPaddingRadius);
	maxs += Vector2(xPaddingRadius,yPaddingRadius);
}

void AABB2::Translate(const Vector2 & translation)
{
	mins += translation;
	maxs += translation;
}

void AABB2::Translate(float translationX, float translationY)
{
	mins += Vector2(translationX,translationY);
	maxs += Vector2(translationX,translationY);
}

//-----------------------------------------------------------------------------------------------
// Sets the AABB2 members from text
//
void AABB2::SetFromText(const char* text)
{
	StringTokenizer* tokenizer = new StringTokenizer(text, ",");
	Strings tokens;
	tokenizer->Tokenize();
	
	GUARANTEE_OR_DIE(tokenizer->GetTokenCount() == 4, "Error in AABB2 string format. It should be (mins.x,mins.y,maxs.x,maxs.y)");
	tokens = tokenizer->GetTokens();
	
	mins.x = strtof(tokens[0].c_str(), nullptr);
	mins.y = strtof(tokens[1].c_str(), nullptr);
	maxs.x = strtof(tokens[2].c_str(), nullptr);
	maxs.y = strtof(tokens[3].c_str(), nullptr);
}

bool AABB2::IsPointInside(float x, float y) const
{
	if( (x >= mins.x && x <= maxs.x ) && (y >= mins.y && y <= maxs.y))
	{
		return true;
	}
	else
		return false;
}

bool AABB2::IsPointInside(const Vector2 & point) const
{
	return IsPointInside(point.x,point.y);
}

Vector2 AABB2::GetDimensions() const
{
	float width = maxs.x - mins.x;
	float height = maxs.y - mins.y;
	return Vector2(width,height);
}

Vector2 AABB2::GetCenter() const
{
	float x = (mins.x + maxs.x) * 0.5f;
	float y = (mins.y + maxs.y) * 0.5f;
	return Vector2(x,y);
}

void AABB2::operator+=(const Vector2 & translation)
{
	Translate(translation);
}

void AABB2::operator-=(const Vector2 & antiTranslation)
{
	
	Translate(antiTranslation * -1.f);
}

AABB2 AABB2::operator+(const Vector2 & translation) const
{
	AABB2 temp;
	temp.mins = mins + translation;
	temp.maxs = maxs + translation;
	return temp;
}

AABB2 AABB2::operator-(const Vector2 & antiTranslation) const
{
	AABB2 temp;
	temp.mins = mins - antiTranslation;
	temp.maxs = maxs - antiTranslation;
	return temp;
}

//-----------------------------------------------------------------------------------------------
// Interpolates the value for the mins and maxs vector
//
const AABB2 Interpolate( const AABB2& start, const AABB2& end, float fractionTowardEnd )
{
	AABB2 temp;
	temp.mins = Interpolate(start.mins, end.mins, fractionTowardEnd);
	temp.maxs = Interpolate(start.maxs, end.maxs, fractionTowardEnd);
	return temp;
}
