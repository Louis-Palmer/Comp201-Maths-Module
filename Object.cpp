#include "stdafx.h"
#include "Object.h"

// Plane constructor. Params are:
//	centrePoint		The point on the plane from which the width and height limits are measured
//	n				The unit vector that is normal to the plane (in world space)
//	up				The unit vector along which the plane height is measured (in world space; should be orthogonal to the normal)
//	w, h			The width and height of the plane (zero/negative for an infinite plane)
Plane::Plane(Point3D centrePoint, Vector3D n, Vector3D up, float w, float h) :
	Object(centrePoint),
	m_heightDirection(up),
	m_normal(n),
	m_halfWidth(w / 2.0f),
	m_halfHeight(h / 2.0f)
{
	m_normal.normalise();
	m_isBounded = m_halfWidth > 0.0f && m_halfHeight > 0.0f;
	if (m_isBounded)
	{
		m_widthDirection = m_heightDirection.cross(m_normal);
		m_widthDirection.normalise();
		m_heightDirection.normalise();
		m_halfDiagonal = sqrt(m_halfWidth * m_halfWidth + m_halfHeight * m_halfHeight);
	}
}

//--------------------------------------------------------------------------------------------------------------------//

// Returns true if the ray intersects with this plane.
// Params:
//	raySrc					source/starting point of the ray (input)
//	rayDir					direction of the ray (input)
//	distToFirstIntersection	distance along the ray from the starting point of the first intersection with the plane (output)
bool Plane::getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection, Vector3D& PlaneNormalDirection) const
{
	//equation to get to T which is distance from ray origin that intersects with the plane
	// 
	//        (planeCentre-rayOrigin) • planeNormal           (            pDistanceFromCentre            )
	//   t =  --------------------------------------      0 = (rayOrigin + rayDirection * t - planceCentre) • planeNormal
	//             rayDirection • planeNormal
	
	//calculates the distance from the camera to where it intersecs on the plane.
	float t = (m_normal.dot(m_centre - raySrc) / (m_normal.dot(rayDir)));
	distToFirstIntersection = t;

	//calculates the point it intersects at and how far from the centre that is
	Point3D p = raySrc + rayDir * t;
	Vector3D pFromCentre = p - m_centre;


	//checks if the point of intersection is within the plane bounds using the magnitude from the centre.
	if (abs(pFromCentre.dot(m_widthDirection)) <= m_halfWidth && abs(pFromCentre.dot(m_heightDirection)) <= m_halfHeight)
	{
		if (distToFirstIntersection >=0)
		{
			return true;
		}
		
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------//

// Transforms the object using the given matrix.
void Plane::applyTransformation(const Matrix3D & matrix)
{
	m_centre = matrix * m_centre;
	m_heightDirection = matrix * m_heightDirection;
	m_widthDirection = matrix * m_widthDirection;
	m_normal = matrix * m_normal;
}

// Returns true if the ray intersects with this sphere.
// Params:
//	raySrc					starting point of the ray (input)
//	rayDir					direction of the ray (input)
//	distToFirstIntersection	distance along the ray from the starting point of the first intersection with the sphere (output)
//  Task 4 - normalDirection is the normal for the intersection point on the sphere. (output)
bool Sphere::getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection, Vector3D& normalDirection) const
{
	// Find the point on the ray closest to the sphere's centre
	Vector3D srcToCentre = m_centre - raySrc;
	float tc = srcToCentre.dot(rayDir);

	//Task 4 this is where i calculate the normal for where im intersecting.
	
	//getting intersection point
	Vector3D intersectionPoint = raySrc.asVector() + (rayDir * distToFirstIntersection);
	//realised the direction from the centre of the circle to the intersection point will allways be the normal assuming its a perfect sphere.
	Vector3D intersectionPointNormal = m_centre.asVector() - intersectionPoint;
	
	// Check whether the closest point is inside the sphere
	if (tc > 0.0f)
	{
		float distSq = srcToCentre.dot(srcToCentre) - tc * tc;
		if (distSq < m_radius2)
		{
			distToFirstIntersection = tc - sqrt(m_radius2 - distSq);
			normalDirection = intersectionPointNormal;   //Task 4 setting the variable
			return true;
		}
	}

	return false;
}

// Transforms the object using the given matrix.
void Sphere::applyTransformation(const Matrix3D & matrix)
{
	m_centre = matrix * m_centre;
}
