#include "stdafx.h"
#include "Controller.h"


float Controller::calculateShotSpeed(const Vector2& tankPos, const Vector2& enemyPos, float shotAngleRadians, float gravity, float wind)
{
	//Commonly used variables are calculated here.
	float distance = enemyPos.x - tankPos.x;
	float heightDifference = enemyPos.y - tankPos.y;

	//make the equations more readable and smaller.
	float tanT = tan(shotAngleRadians);
	float sinT = sin(shotAngleRadians);
	float cosT = cos(shotAngleRadians);

	// Task 1 - Equation that can hit targets without a height difference or wind acceleration.
	if (!c_canHandleHeightDifference && !c_canHandleWind) {

																			 //                   Velocity_Numer
		float velocityNumer = sqrtf(gravity * tanT * distance / 2);          // U =    -----------------------------------------
		float initialVelocity = velocityNumer / sinT;						 //                     Sin(Theta)

		return initialVelocity;
	}


	//Equation mostly derived from https://www.youtube.com/watch?v=Dc34eSc8kPA
	// Task 2 - Equation that can handle height difference. See Task 2 answer in readme for full equation.
	if (c_canHandleHeightDifference && !c_canHandleWind) {

		float velocityNumer = gravity * pow(distance, 2);	

		//working out the denominators in two parts							                ___________________________________
		float velocityDenom1 = 2 * (pow(cosT, 2));								//         /        Velocity_Numer
		float velocityDenom2 = heightDifference * -1 - (distance * tanT);		// U =    /  ----------------------------------
																				//      \/   Velocity_Denom1 * Veclocity_Denom2

		float initialVelocity = sqrt((velocityNumer / (velocityDenom1 * velocityDenom2)) * -1);

		return initialVelocity;
	}

	// Task 3 - Equation that can handle both wind and height difference. See Task 3 answer in readme for full equation.
	else 
	{																			//            ___________________________________
																				//           / 2 * (Tan(Theta)*Distance) + Height
		float timeNumer = (2 * ((tanT * distance) + heightDifference));			// Time =   / ---------------------------------
		float timeDenom = gravity + (wind * tanT);								//   	  \/   gravity + (wind* Tan(Theta)

		//we work out time seperatley, we can substitute it into the equation later for t.
		float time = sqrt(timeNumer / timeDenom);

																			//                     Distance +0.5*-wind * Time^2
		float velocityNumer = distance + 0.5 * -wind * pow(time, 2);		//  Initial Velocity  = ----------------------------
		float velocityDenom = cosT * time;									//                        Cos(Theta) * Time

		float initialVelocity = velocityNumer / velocityDenom;
		return initialVelocity;
	}

}

float Controller::calculateShotAngle(const Vector2& tankPos, const Vector2& enemyPos, float shotSpeed, float gravity, float wind)
{
	//Commonly used variables are calculated here.
	float distance = ((enemyPos.x - tankPos.x)*-1);
	float heightDifference = (tankPos.y -enemyPos.y);

	gravity *= -1;


	//Equation from https://www.youtube.com/watch?v=bqYtNrhdDAY&ab_channel=MichelvanBiezen
	//See Task 4 answer in readme for full equation. (Does not include wind.)
	
	//we calculate phasangle and use it later on.
	float phaseAngle = atan(distance / heightDifference);
	
	
	float wholeNumer = ((gravity * pow(distance, 2)) / pow(shotSpeed, 2)) - heightDifference;
	float wholeDenom = sqrt(pow(heightDifference, 2) + pow(distance, 2));
	float majorityEquation = acos(wholeNumer / wholeDenom);

	float theta = (majorityEquation + phaseAngle)/2;

	//if the enemy was lower than the player tank then it would shift the correct angle by 90 degrees counter clockwise.
	if (heightDifference < 0) {
		theta -= 1.5708;
	}

	return theta ;
}


