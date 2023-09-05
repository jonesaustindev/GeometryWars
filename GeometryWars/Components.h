#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 position = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	float angle = 0.0f;

	CTransform(const Vec2& pos, const Vec2& vel, float angle)
		: position(pos), velocity(vel), angle(angle) {}
};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float outlineThickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(outlineThickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision
{
public:
	float radius = 0.0f;
	CCollision(float radius)
		: radius(radius) {}
};

class CInput
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;

	CInput() {}
};

class CScore
{
public:
	int score = 0;
};

class CLifespan
{
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int total)
		: remaining(total), total(total) {}
};
