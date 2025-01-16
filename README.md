# comp270-worksheet-A
Base repository for COMP270 Assignment 1, worksheet 1

## This document is likely built using an older version of C++
When loading this document, you will be asked if you want to update, agree to this!

Doing so will automatically update your document to whatever version you are using and will not work if you disagree!
## Task 1
Task 1 is to calculate the track. Originally I calculated the Beizer curve using De Casteljau's algorithm which gets point T on each line then draws lines from those points and recursively does this until you have a have an exact point. I then added the calculated point to an SDL_Points array which allowed me to draw lines between each point easily.
```c++
int getPT(int C1, int C2, float T)
{
	//Equation for getting Point T along a line
	return C1 + ((C2 - C1)* T);
}
```
```c++
for (float i = 0; i < 1; i += Increment)
{
	// The Green Lines. Points for these would normally be for a linear Beizer curve
	int xa = getPT(p0.x, p1.x, i);
	int ya = getPT(p0.y, p1.y, i);
	int xb = getPT(p1.x, p2.x, i);
	int yb = getPT(p1.y, p2.y, i);
	int xc = getPT(p2.x, p3.x, i);
	int yc = getPT(p2.y, p3.y, i);

	// The Blue Line. Points for These would normally be the points for a Quadratic beizer curve
	int xm = getPT(xa, xb, i);
	int ym = getPT(ya, yb, i);
	int xn = getPT(xb, xc, i);
	int yn = getPT(yb, yc, i);

	// The Black Dot. Points for Cubic beizer curve
	int x = getPT(xm, xn, i);
	int y = getPT(ym, yn, i);

	//two lines below just add the final cordinates for the cubic curve to an array.
	int ArrayNumber = i * N;
	PointArray[ArrayNumber] = { x,y };
}
```
   
   
Later I switch to Beizer's method which we got given the equation for in the assigment brief. I also turned it into a function this was very usefull later on and allowed me to solve the rest of the tasks with it.  
 ```C++
 Vector2 Bezier::EquationCurve(float T) CONST
{
	Vector2 pos = pow(1 - T, 3)* p0 + 3 * pow(1 - T, 2) * T * p1 + 3 * (1.0 - T) * pow(T, 2) * p2 + pow(T, 3) * p3;
	return pos;
}
```

## Task 2
Task 2 was getting the car to follow the beizer curve. Before i switched the the calculation of the beizer curve to a function and while i was still trying to figure out how the whole worksheet worked I tried to create a whole new 2d array of all the point of the beizer curve. My intention was to go through and calculate the whole curve beforehand and store it, then get where the car position is along the track and match it to a cordinate in my stored curve. I quickly leanred that this was very difficult and coudlnt figure out how to do it.   
I then decided to just calculate a new position on the curve each time the drawCarOnTrack() function is called. I still used the 2d array i created and just calculated the x and y of the postion seperatley then return them both as a vector 2 as seen below.
```c++
Vector2 Bezier::GetCarPos(float position)
{
	Vector2 PointArray[6][4] = { Vector2(400, 50), Vector2(600, 50), Vector2(700, 100), Vector2(700, 300),
							   Vector2(700, 300), Vector2(700, 400), Vector2(700, 500), Vector2(600, 500),
							   Vector2(600, 500), Vector2(400, 500), Vector2(100, 600), Vector2(100, 500),
							   Vector2(100, 500), Vector2(100, 400), Vector2(200, 400), Vector2(300, 300),
							   Vector2(300, 300), Vector2(400, 200), Vector2(100, 200), Vector2(100, 150),
							   Vector2(100, 150), Vector2(100, 100), Vector2(200, 50), Vector2(400, 50) };


	int TrackNumber = static_cast<int>(position);
	float T = fmod(position, 1.0f);
	float X = pow(1 - T, 3) * PointArray[TrackNumber][0].x + 3 * pow(1 - T, 2) * T * PointArray[TrackNumber][1].x + 3 * (1.0 - T) * pow(T, 2) * 	PointArray[TrackNumber][2].x + pow(T, 3) * PointArray[TrackNumber][3].x;
	float Y = pow(1 - T, 3) * PointArray[TrackNumber][0].y + 3 * pow(1 - T, 2) * T * PointArray[TrackNumber][1].y + 3 * (1.0 - T) * pow(T, 2) * PointArray[TrackNumber][2].y + pow(T, 3) * PointArray[TrackNumber][3].y;
		
	return Vector2{X,Y};
}
```
Later on when I made the function for the beizer curve i switched it to that and figured out i can just use it for the specific part of the track the car is on and produced this below.
```c++
void Application::drawCarOnTrack(float position)
{
	int TrackNumber = static_cast<int>(position);
	float T = fmod(position, 1.0f);
	Bezier Curve = m_track[TrackNumber];

	Vector2 P = Curve.EquationCurve(T);

	drawCar(P, 0);

}

//the function below is in beizer.cpp
Vector2 Bezier::EquationCurve(float T)
{
	Vector2 pos = pow(1 - T, 3)* p0 + 3 * pow(1 - T, 2) * T * p1 + 3 * (1.0 - T) * pow(T, 2) * p2 + pow(T, 3) * p3;
	return pos;
}
```

## Task 3
Task 3 was to get the car to follow the rotation of the curve. 
Same with Task 2 I started by creating a 2D array containing. Which worked but same issue as in task 2, which I changed the same time I changed it in task 2.
```c++
Vector2 PointArray[6][4] = { Vector2(400, 50), Vector2(600, 50), Vector2(700, 100), Vector2(700, 300),
						   Vector2(700, 300), Vector2(700, 400), Vector2(700, 500), Vector2(600, 500),
						   Vector2(600, 500), Vector2(400, 500), Vector2(100, 600), Vector2(100, 500),
						   Vector2(100, 500), Vector2(100, 400), Vector2(200, 400), Vector2(300, 300),
						   Vector2(300, 300), Vector2(400, 200), Vector2(100, 200), Vector2(100, 150),
						   Vector2(100, 150), Vector2(100, 100), Vector2(200, 50), Vector2(400, 50) };
int TrackNumber = static_cast<int>(position);
float T = fmod(position, 1.0f);

Vector2 Rot = 3 * pow(1 - T, 2) * (PointArray[TrackNumber][1] - PointArray[TrackNumber][0]) + 6 * pow(1 - T, T) * (PointArray[TrackNumber][2] - PointArray[TrackNumber][1]) + pow(3 * T, 2) * (PointArray[TrackNumber][3] - PointArray[TrackNumber][2]);
```
   
Once I changed task 2 Bezier function I created  a similar equation but instead it would calculate the tangent using the equation given in the assignment brief below.  
$b'(t)=3(1-t)^2(p.1-p.0)+6(1-t)t(p.2-p1)+3t^2(p.3-p.2)$
```c++
Vector2 Rot = 3 * pow(1 - T, 2) * (p1 - p0) + 6 * pow(1 - T, T) * (p2 - p1) + pow(3 * T, 2) * (p3 - p2);
```
   
A problem with the code above is that I entered it wrong, this caused the car to follow the track but at each new segment of track the car would slightly drift. I wasn’t aware of the problem with the equation until later when I fixed it with the correct equation below.
```c++
Vector2 Rot = 3 * pow(1 - T, 2) * (p1 - p0) + (6 * (1-T))*T * (p2 - p1) + 3 * pow(T, 2) * (p3 - p2);
```
   
This returns the tangent as a vector2 but DrawCar() takes angle in degrees so I need to convert that. A function called atan2f() takes the y and x of the tangent and returns it as a angle radians and the conversion from radians to degrees is simple you multiply it by 180 then divide by PI. (The sprite was set 90degrees off the correct direction, so I had to add 90 degrees to get it facing the right way.)
```c++
	float radians = atan2f(Rot.y, Rot.x);
	return((radians * 180 / M_PI) + 90);
```


## Task 4
Task 4 was to get the car to move along the track at an equal speed.  
The cars speed varies as it takes the corners most of the time slower than the rest of the track, this was because the space between the dots was smaller so the car would travel less of a distance in the same amount of time.
I started off by trying to get the distance or magnitude from the points from each other by making a function that would save the last point and take the next point, this had a few problems one being that it would calculate the distance from its last point to the current one (,not the distance from its current point to the next one); so how I planned on doing it would mean that the intended speed it travels at would always be delayed (I was hoping that the car would be updating position fast enough that it wouldn’t be noticeable).
```c++
float GetDistance(Vector2 Point1, Vector2 Point2) 
{
	Vector2 Distance = Vector2{ fabs(Point1.x - Point2.x),fabs(Point1.y - Point2.y) };
	return sqrt(pow(Distance.x, 2) + pow(Distance.y, 2) * 1.0);

}
```
  
Eventually I settled on using the tangent at the point on the curve. This meant that I could get the magnitude at that point which would dictate how much i need to speed the car up by.

