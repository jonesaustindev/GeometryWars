#pragma once

class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vec2();
	Vec2(float x, float y);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator * (float rhs) const;
	Vec2 operator / (float rhs) const;

	Vec2 operator += (const Vec2& rhs);
	Vec2 operator -= (const Vec2& rhs);
	Vec2 operator *= (const float val);
	Vec2 operator /= (const float val);

	float dist(const Vec2& rhs) const;
	float length() const;
};