#pragma once

#include "Vector2.h"

class Bezier
{
public:
	Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);

	void draw(SDL_Renderer *renderer) const;

	//all functions that calculate parts of the beizer curve.
	Vector2 curveEquation(float t) const;
	float rotationEquation(float t) const;
	Vector2 tangentEquation(float t) const;

private:
	Vector2 p0, p1, p2, p3;

};
