#include "stdafx.h"
#include "Camera.h"
#include "Object.h"

//Direction of light source for shading.
Vector3D sunDirection = Vector3D(0, -2, 1);

//toggles a shading method which I personally prefer.
bool lighterShading = false;

// Initialises the camera at the given position
void Camera::init(const Point3D& pos)
{
	m_position = pos;
	m_pixelBuf.init(m_viewPlane.resolutionX, m_viewPlane.resolutionY);

//--------------------------------------------------------------------------------------------------------------------//
	//Task 1
	//Gets the width and height of each pixel using the size of the viewplane and the resolution of it.
	m_pixelWidth = (2*m_viewPlane.halfWidth) / m_viewPlane.resolutionX;
	m_pixelHeight = (2*m_viewPlane.halfHeight) / m_viewPlane.resolutionY;
//--------------------------------------------------------------------------------------------------------------------//
}

// Cast rays through the view plane and set colours based on what they intersect with
bool Camera::updatePixelBuffer(const std::vector<Object*>& objects)
{
	if (m_pixelBuf.isInitialised())
	{
		m_pixelBuf.clear();

		// Make sure our cached values are up to date
		if (m_worldTransformChanged)
		{
			updateWorldTransform();
			m_worldTransformChanged = false;
		}

		// Transform the objects to the camera's coordinate system
		const Matrix3D worldToCameraTransform = m_cameraToWorldTransform.inverseTransform();
		for (auto obj : objects)
			obj->applyTransformation(worldToCameraTransform);

		// Fill the pixel buffer with pointers to the closest object for each pixel
		Point3D origin;
		Vector3D rayDir;
		float distToIntersection;

		//Task 4 normalDirection
		Vector3D normalDirection;

		for (const auto obj : objects)
		{
			// Find the pixel that's intersected by the line from the
			// camera to the object's centre
			Vector3D toCentre = obj->position().asVector();
			toCentre.normalise();
				
			// Centre line intersects the view plane when the z value
			// is the distance to the view plane
			const float t = m_viewPlane.distance / toCentre.z;
			const float viewPlaneX = toCentre.x * t + m_viewPlane.halfWidth,
					viewPlaneY = toCentre.y * t + m_viewPlane.halfHeight;
				
			// Find the pixel indices of the centre line intersection point
			const int pixelX = static_cast<int>(viewPlaneX / m_pixelWidth),
					pixelY = static_cast<int>(viewPlaneY / m_pixelHeight);

			// Find the largest range of pixels that the object might cover,
			// based on its maximum 'radius'.
			const float objectRad = fabsf(obj->getMaxRadius());
			const int pixelRadiusX = static_cast<int>(objectRad / m_pixelWidth) + 1,
					pixelRadiusY = static_cast<int>(objectRad / m_pixelHeight) + 1;
				
			// Find the range of pixels that might be covered by the object,
			// clamped to the view plane resolution
			const unsigned startX = max(pixelX - pixelRadiusX, 0), endX = min(pixelX + pixelRadiusX, m_viewPlane.resolutionX),
					startY = max(pixelY - pixelRadiusY, 0), endY = min(pixelY + pixelRadiusY, m_viewPlane.resolutionY);
			
			// For each of the pixels that might be covered by the object, find the direction
			// of the ray passing through it and test whether it intersects with the object
			for (unsigned i = startX; i < endX; ++i)
			{
				for (unsigned j = startY; j < endY; ++j)
				{
//--------------------------------------------------------------------------------------------------------------------//
					// TODO: if you want to pass through any extra information from the intersection test
					// for Task 4, this is the place to do so. 
					rayDir = getRayDirectionThroughPixel(i, j);

					//Task 4 - passed normalDirection
					// Perform the intersection test between the ray through this pixel and the object,
					// and check whether the intersection point is closer than that of previously tested objects
					if (obj->getIntersection(origin, rayDir, distToIntersection, normalDirection)	
						&& distToIntersection < m_pixelBuf.getObjectInfoForPixel(i, j).distanceToIntersection)
					{
						m_pixelBuf.setObjectInfoForPixel(i, j, ObjectInfo(obj, distToIntersection,normalDirection));
					}
//--------------------------------------------------------------------------------------------------------------------//
				}
			}
		}

		// Now put the objects back!
		for (auto obj : objects)
			obj->applyTransformation(m_cameraToWorldTransform);

		return true;
	}
	
	return false;
}


//--------------------------------------------------------------------------------------------------------------------//

// Calculates the normalised direction in camera space of a ray from
// the camera through the view-plane pixel at index (i, j),
// where 0 <= i < m_viewPlane.resolutionX and 0 <= j < m_viewPlane.resolutionY.
Vector3D Camera::getRayDirectionThroughPixel(int i, int j)
{
	//Task 1 - we get the direction the ray that needs to be fired in from the camera for each pixel.

	Vector3D rayDir;
	float halfResolutionX = m_viewPlane.resolutionX / 2;
	float halfResolutionY = m_viewPlane.resolutionY / 2;

	//finds the centre 0,0 then multiplies the pixels dimesions by the displacement from that centre.
	rayDir.x = (i - halfResolutionX) * m_pixelWidth;
	rayDir.y = (j - halfResolutionY) * m_pixelHeight;


	rayDir.z = m_viewPlane.distance;
	

	rayDir.normalise();
	return rayDir;
}

void Camera::updateWorldTransform()
{
	//Task 3
	//Swapped the inputing for controlling the Z and X axis to make controlls feel better.
	Matrix3D rotationX = Matrix3D();
	rotationX(1, 1) =  cos(m_rotation.x);
	rotationX(1, 2) =  sin(m_rotation.x);
	rotationX(2, 1) = -sin(m_rotation.x);
	rotationX(2, 2) =  cos(m_rotation.x);

	Matrix3D rotationY = Matrix3D();
	rotationY(0, 0) =  cos(m_rotation.y);
	rotationY(2, 0) =  sin(m_rotation.y);
	rotationY(0, 2) = -sin(m_rotation.y);
	rotationY(2, 2) =  cos(m_rotation.y);

	Matrix3D rotationZ = Matrix3D();
	rotationZ(0, 0) =  cos(m_rotation.z);
	rotationZ(1, 0) = -sin(m_rotation.z);
	rotationZ(0, 1) =  sin(m_rotation.z);
	rotationZ(1, 1) =  cos(m_rotation.z);

	
	Matrix3D matrices = Matrix3D();

	matrices(0, 3) = m_position.x;
	matrices(1, 3) = m_position.y;
	matrices(2, 3) = m_position.z;
	matrices(2, 2) = -1;

	
	m_cameraToWorldTransform = rotationX * rotationY * rotationZ * matrices;
}

//Task 4 I compare the directions of the normals to my sun direction to see if its facing the same way and shade it based on that
Colour Camera::getColourAtPixel(unsigned i, unsigned j)
{
	Colour colour;

	const ObjectInfo& objInfo = m_pixelBuf.getObjectInfoForPixel(i, j);

	const Object* object = objInfo.object;
	if (object != nullptr)
		colour = object->m_colour;


	//Task 4 - Normal shading

	Vector3D normal = objInfo.normalAtIntersectionPoint;


	sunDirection.normalise();
	normal.normalise();


	//makes the normals world space.
	normal = m_cameraToWorldTransform * normal;


	float directionComparrisons = sunDirection.dot(normal);


	//could not find a way to identify if the object was a plane through the object class, so i identifed it by colour.
	if (colour.r == 245&& colour.g == 121 && colour.b == 58) {
		return colour;
	}


	//stops spheres from being shaded lighter. (visual preference)
	if (directionComparrisons > 0 && !lighterShading) {
		return colour;
	}


	//darken/lightens the colour based on the dot product of both directions
	colour.r = adjustColour(colour.r, directionComparrisons);
	colour.g = adjustColour(colour.g, directionComparrisons);
	colour.b = adjustColour(colour.b, directionComparrisons);


	return colour;
}


//Calculates based dotProd how much a colour should be darkened/lightened by.
float Camera::adjustColour(float colour, float dotProd)
{
	//increased or decreases the current rbg values by the dotproduct which ranges from -1 to 1, 0 being no change to colour at all.
	float amountThatCanBeChanged = 255 - colour;
	float amountToBeChanged = amountThatCanBeChanged * dotProd;
	float finalColour = colour + amountToBeChanged;


	if (finalColour > 255)  finalColour = 255; 
	else if (finalColour < 0)  finalColour = 0; 
	
	return finalColour;
}
