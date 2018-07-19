#include "Engine/Math/CubicSpline2D.hpp"

//-----------------------------------------------------------------------------------------------
// Constructor
//
CubicSpline2D::CubicSpline2D(const Vector2* positionsArray, int numPoints, const Vector2* velocitiesArray/*=nullptr */)
{
	for(int index = 0; index < numPoints; ++index)
	{
		m_positions.push_back(positionsArray[index]);
		
		if(velocitiesArray)
			m_velocities.push_back(velocitiesArray[index]);
		else
			m_velocities.push_back(Vector2::ZERO);
	}
}

//-----------------------------------------------------------------------------------------------
// Appends a point to the array
//
void CubicSpline2D::AppendPoint(const Vector2& position, const Vector2& velocity/*=Vector2::ZERO */)
{
	m_positions.push_back(position);
	m_velocities.push_back(velocity);
}

//-----------------------------------------------------------------------------------------------
// Appends points to the array
//
void CubicSpline2D::AppendPoints(const Vector2* positionsArray, int numPoints, const Vector2* velocitiesArray/*=nullptr */)
{
	for(int index = 0; index < numPoints; ++index)
	{
		m_positions.push_back(positionsArray[index]);

		if(velocitiesArray)
			m_velocities.push_back(velocitiesArray[index]);
		else
			m_velocities.push_back(Vector2::ZERO);
	}
}

//-----------------------------------------------------------------------------------------------
// Inserts a point in the middle
//
void CubicSpline2D::InsertPoint(int insertBeforeIndex, const Vector2& position, const Vector2& velocity/*=Vector2::ZERO */)
{
	std::vector<Vector2>::iterator posIter = m_positions.begin()  + insertBeforeIndex;
	std::vector<Vector2>::iterator velIter = m_velocities.begin() + insertBeforeIndex;
	m_positions.insert(posIter, position);
	m_velocities.insert(velIter, velocity);
}

//-----------------------------------------------------------------------------------------------
// Removes the point
//
void CubicSpline2D::RemovePoint(int pointIndex)
{
	std::vector<Vector2>::iterator iter = m_positions.begin() + pointIndex;
	m_positions.erase(iter);
}

//-----------------------------------------------------------------------------------------------
// Removes all points in the array
//
void CubicSpline2D::RemoveAllPoints()
{
	m_positions.clear();
	m_velocities.clear();
}

//-----------------------------------------------------------------------------------------------
// Mutates the point at the index
//
void CubicSpline2D::SetPoint(int pointIndex, const Vector2& newPosition, const Vector2& newVelocity)
{
	m_positions[pointIndex] = newPosition;
	m_velocities[pointIndex] = newVelocity;
}

//-----------------------------------------------------------------------------------------------
// Mutates the point (position only)
//
void CubicSpline2D::SetPosition(int pointIndex, const Vector2& newPosition)
{
	m_positions[pointIndex] = newPosition;
}

//-----------------------------------------------------------------------------------------------
// Mutates the point (velocity only)
//
void CubicSpline2D::SetVelocity(int pointIndex, const Vector2& newVelocity)
{
	m_velocities[pointIndex] = newVelocity;
}

//-----------------------------------------------------------------------------------------------
// 
//
void CubicSpline2D::SetCardinalVelocities(float tension/*=0.f*/, const Vector2& startVelocity/*=Vector2::ZERO*/, const Vector2& endVelocity/*=Vector2::ZERO */)
{
	m_velocities[0] = startVelocity;
	m_velocities[m_velocities.size() - 1] = endVelocity;

	for(size_t index = 1; index < m_positions.size() - 1; ++index) // Loop from the second point to the penultimate point
	{
		Vector2 velocityAverage = (m_positions[index + 1] - m_positions[index - 1]) * 0.5f;
		m_velocities[index] = velocityAverage * (1.f - tension);
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the positions
//
int CubicSpline2D::GetPositions(std::vector<Vector2>& out_positions) const
{
	out_positions = m_positions;
	return static_cast<int>(m_positions.size());
}

//-----------------------------------------------------------------------------------------------
// Returns the velocities
//
int CubicSpline2D::GetVelocities(std::vector<Vector2>& out_velocities) const
{
	out_velocities = m_velocities;
	return static_cast<int>(m_velocities.size());
}

//-----------------------------------------------------------------------------------------------
// Evaluates the spline on a cumulative time 
//
Vector2 CubicSpline2D::EvaluateAtCumulativeParametric(float t) const
{
	int curveSelector = static_cast<int>(floor(t));
	float normalizedTimeValue = t - (float) curveSelector;

	Vector2 startPos = m_positions[curveSelector];
	Vector2 endPos = m_positions[curveSelector + 1];
	Vector2 startVel = m_velocities[curveSelector];
	Vector2 endVel = m_velocities[curveSelector + 1];

	return EvaluateCubicHermite(startPos, startVel, endPos, endVel, normalizedTimeValue);
}

//-----------------------------------------------------------------------------------------------
// Evaluates the spline value on a normalized time value over the complete spline
//
Vector2 CubicSpline2D::EvaluateAtNormalizedParametric(float t) const
{
	float cumulativeTime = RangeMapFloat(t, 0.f, 1.f, 0.f, (float) m_positions.size() - 1);

	return EvaluateAtCumulativeParametric(cumulativeTime);
}
