#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (float val) const
{
	return Vec2(x / val, y / val);
}

bool Vec2::operator == (const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator += (const Vec2& rhs)
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator -= (const Vec2& rhs)
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator *= (const float val)
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator /= (const float val)
{
	return Vec2(x / val, y / val);
}

float Vec2::dist(const Vec2& rhs) const
{
	// TODO come back to this and implement the optimized version from notes
	//return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	return (x * rhs.x) + (y * rhs.y);
}

float Vec2::length() const
{
	// TODO come back to this and implement the optimized version from notes
	//return sqrtf(x * x + y * y);
	return x * x + y * y;
}

Vec2 Vec2::difference(const Vec2& rhs)
{
	return Vec2(rhs.x - x, rhs.y - y);
}

Vec2 Vec2::normalize(const Vec2& rhs)
{
	Vec2 differenceVector = difference(rhs);
	float squaredLength = differenceVector.length();

	if (squaredLength > 0)
	{
		// Doing this rather than using std::sqrt to save some complexity in computation
		float inverseSquaredLength = 1.0f / squaredLength;

		// Scale the vector components using the inverse squared length
		return Vec2(differenceVector.x * inverseSquaredLength, differenceVector.y * inverseSquaredLength);
	}
	else
	{
		// Handle the case where squaredLength is not greater than 0
		return Vec2(0.0f, 0.0f);
	}
}

//Vec2 Vec2::normalize(const Vec2& rhs)
//{
//	Vec2 differenceVector = difference(rhs);
//	float squaredLength = differenceVector.length();
//
//	if (squaredLength > 0)
//	{
//		// Doing this rather than using std::sqrt to save some complexity in computation
//		float inverseSquaredLength = 1.0f / squaredLength;
//
//		// Scale the vector components using the inverse squared length
//		return Vec2(differenceVector.x * inverseSquaredLength, differenceVector.y * inverseSquaredLength);
//	}
//	else
//	{
//		return Vec2(0.0f, 0.0f);
//	}
//}
