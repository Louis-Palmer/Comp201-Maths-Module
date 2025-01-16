#pragma once

// Class representing a 2D vector
// This contains some of the most common vector operations -- feel free to extend it
class Vector2
{
public:
	// Default constructor, initialises vector to (0,0)
	Vector2() : x(0), y(0) { }

	// Constructor initialising the vector from given x and y components
	Vector2(float x, float y) : x(x), y(y) { }

	// X and Y components of the vector
	float x, y;

	// Addition operator
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	// Subtraction operator
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	// Scalar multiplication operator (vector * scalar)
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	// Vector magnitude
	float magnitude() const
	{
		return sqrtf(x * x + y * y);
	}
};

// Scalar multiplication operator (scalar * vector)
// NB since the Vector2 is the second operand, this can't be defined as a method inside the Vector2 class
inline Vector2 operator*(float scalar, const Vector2& vector)
{
	return Vector2(scalar * vector.x, scalar * vector.y);
}
