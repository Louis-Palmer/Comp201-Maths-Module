#include "stdafx.h"
#include "Bezier.h"
#include "iostream"
#include <cmath>
Bezier::Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3)
	: p0(p0), p1(p1), p2(p2), p3(p3)
{
}

//Controls smoothness of curve (higher is smoother).
const int n = 20;


/// <summary>
/// Task 1-2
/// Returns a point along the bezier curve based on T.
/// </summary>
/// <param name="T"> is the decimal along the curve which is taken as a float 0-1, 0 being the start and 1 being the end.</param>
/// <returns> Vector2 Position | Position along the Curve.</returns>
Vector2 Bezier::curveEquation(float t) const
{
	Vector2 position = pow(1 - t, 3)* p0 + 3 * pow(1 - t, 2) * t * p1 + 3 * (1.0 - t) * pow(t, 2) * p2 + pow(t, 3) * p3;
	return position;
}


/// <summary>
/// Task 4
/// Calculates the tangent based on T along the curve.
/// </summary>
/// <param name="T">is the decimal along the curve which is taken as a float 0-1, 0 being the start and 1 being the end.</param>
/// <returns> Vector2 Tangent | Tangent at the current point T along the curve. </returns>
Vector2 Bezier::tangentEquation(float t) const
{
	Vector2 tangent = 3 * pow(1 - t, 2) * (p1 - p0) + (6 * (1 - t)) * t * (p2 - p1) + 3 * pow(t, 2) * (p3 - p2);
	return tangent;
}


/// <summary>
/// Task 3
/// Function to calculate the rotation from T using tangents which gets returned as a float.
/// </summary>
/// <param name="T">is the decimal along the curve which is taken as a float 0-1, 0 being the start and 1 being the end.</param>
/// <returns>Returns a rotation in radians as a float</returns>
float Bezier::rotationEquation(float t) const
{
	Vector2 tangent = tangentEquation(t);

	//Converting tangent to radians to degrees (90 offset is fixed in DrawCar()).
	return atan2f(tangent.y, tangent.x) * 180 / M_PI;

}



/// <summary>
/// Task 1
/// Function that renders all the track segments based on the amount of points N is set too. 
/// </summary>
/// <param name="renderer">The SDL_Renderer used to render the lines</param>
void Bezier::draw(SDL_Renderer *renderer) const
{
	//for 20 lines the array needs 21 points.
	SDL_Point pointArray[n+1] = {};

	for (int i = 0; i <= n; i ++)
	{
		//Gets the specific point based on I/N (which is the equivalent of T).
		Vector2 point = curveEquation((float)i / (float)n);

		pointArray[i] = { int(point.x),int(point.y) };
	}

	SDL_RenderDrawLines(renderer, pointArray, n+1);
	
}


