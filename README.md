# comp270-worksheet-3
Base repository for COMP270 worksheet 3

# Work in progress Readme
  
## Task 1
Task one was simply to get the spheres to render. This is the most important task since without it you cant do any others.
  
To start it off I needed to get the dimensions of each pixel on the viewplane. I did this by getting the height and width of the viewplane then dividing it by the resoultion meaning the quantity of pixels that needed to fit on the viewplane.
```c++
m_pixelWidth = (2*m_viewPlane.halfWidth) / m_viewPlane.resolutionX;
m_pixelHeight = (2*m_viewPlane.halfHeight) / m_viewPlane.resolutionY;
```

I then needed to get the direction of the ray for each pixel on the viewplane. Since we know the centre of the viewplane is the centre from the camera we can work from there making that point 0,0. We now need to find which pixel are on that point so we find the centre one by halfing the resolution for height and width. We now need to take these away from the pixel we are trying to aim at now means that if we have a pixel that is -10 on the x axis it is 10 to the left from the centre of the plane. Now we can multiply this by the pixel dimesions to get a distance from the centre of the plane.  For the z axis it is already given to us as the distance from the camera to the viewplane so we can just set that.

```c++
	Vector3D rayDir;
	float halfResolutionX = m_viewPlane.resolutionX / 2;
	float halfResolutionY = m_viewPlane.resolutionY / 2;

	rayDir.x = (i - halfResolutionX) * m_pixelWidth;
	rayDir.y = (j - halfResolutionY) * m_pixelHeight;

	rayDir.z = m_viewPlane.distance;
	
	rayDir.normalise();
	return rayDir;
```

## Task 2
Task 2 was to render an orange plane that is behind the spheres this plane has a width and height so its not an infinite plane.  
First part of the problem is to get the plane to render based on its normal and centre.
```
      (PlaneCentre-RayOrigin) • PlaneNormal 
 t =  --------------------------------------
           RayDirection • PlaneNormal
```
This is the equation we usedto get t which is the distance of the plane from the Camera. With this it allows us to render an infite plane behind the spheres. 
  
The next step was to make sure the plane was only rendered within its own height and width.
We do this by calculating the point of intersection along the plane and storing this as P using the equation below
```
p = raySrc + rayDir * t
```
We now need to check if P is within the bounds of the plane. The method I chose was to use half the magnitude of the height and width of the plane and then get the magnitude from that. Then getting the magnitude from the dot product of the distance of p from the centre, And the width and height direction. If the point of intersection is within the Plane bounds the magnitude from the dot product will always be less than the Half width and height.  
This is shown with the code below.  
```c++
float t = (m_normal.dot(m_centre - raySrc) / (m_normal.dot(rayDir)));
distToFirstIntersection = t;

Point3D p = raySrc + rayDir * t;
Vector3D pFromCentre = p - m_centre;

if (abs(pFromCentre.dot(m_widthDirection)) <= m_halfWidth && abs(pFromCentre.dot(m_heightDirection)) <= m_halfHeight)
{
	if (distToFirstIntersection >=0)
	{
		return true;
	}
}
return false;
```

## Task 3
Task 3 was to get the camera to rotate, two ways of doing this, first being around the world origin and second being like a first person camera. I chose to go with the World orgin option.

First step for this was getting the Rotation matrices for X,Y,Z axis using the m_rotation in each of them. Shown below.
```c++
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
```
The next step is to create a new matrix for holding the current position of the camera since it got a bit buggy when I was setting it directly to m_cameraToWorldTransform.
```c++
Matrix3D matrices = Matrix3D();

matrices(0, 3) = m_position.x;
matrices(1, 3) = m_position.y;
matrices(2, 3) = m_position.z;
matrices(2, 2) = -1;
```
From here all we need is to multiply everything together in the correct order.
```c++
m_cameraToWorldTransform = rotationX * rotationY * rotationZ * matrices;
```
The end result is a matrix that we can multiply vectors and matrices to convert anything to and from world to camera space.

## Task 4
For task 4 we had many option to chose from, I chose to go with normal shading. I had the logic for normal shading all down on paper before i started working on it. (Image to be added here)

First step was to get the normals for the spheres.  
I did this in the getIntersection() function. The first step was to get the point on the sphere that the ray intersetected it on.   
```c++
Vector3D intersectionPoint = raySrc.asVector() + (rayDir * distToFirstIntersection);
```
With this intersection point we can work out what the direction is by getting the distance from the centre of the sphere to the intersection point.  
```c++
Vector3D intersectionPointNormal = m_centre.asVector() - intersectionPoint;
```
  
Second step was to pass this data through to getColourAtPixel(), this was not too complicated thanks to the way the project was set up; all I had to do was change the variables the getIntersection() used to include the normal. 

The third step is to calculate the Dot product and shade the spheres based on that. I did this first by normalising both my SunDirection and normal. Then storing the dot prodcut as a float. With both being normalised we get to ignore the magnitude. We also use what we calculated in task 3 to move the normals from camera to world space.
```c++
Vector3D normal = objInfo.normalAtIntersectionPoint;

sunDirection.normalise();
normal.normalise();

normal = m_cameraToWorldTransform * normal;

float directionComparrisons = sunDirection.dot(normal);
```

I then made a function that would shade the current colour based on the dot product that ranges from -1 to 1.
```c++
float Camera::adjustColour(float colour, float dotProd)
{
float amountThatCanBeChanged = 255 - colour;
float amountToBeChanged = amountThatCanBeChanged * dotProd;
float finalColour = colour + amountToBeChanged;
if (finalColour > 255)  finalColour = 255; 
else if (finalColour < 0)  finalColour = 0; 

return finalColour;
}
```
