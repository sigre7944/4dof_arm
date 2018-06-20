/* Written Ha Nguyen
*
* Function for initializing the motors as well as moving them by degree value
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#pragma once

#include "./moveByDegree.h"

class manualAdjust
{
private:
	int mode = 0; //0 for normal mode, 1 for record mode
	int goal_position[4] = { 200, 200, 200, 200 };  // Goal position
	moveByDegree *m_;

public:
	manualAdjust();
	~manualAdjust();

	void init(moveByDegree *m);

	int loosenGrip();
	int tightenGrip();
	int checkForInput(int value);
	void newGoal();
	void sendGoal();
};

