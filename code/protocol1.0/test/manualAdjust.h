/* Written Ha Nguyen
*
* Function for teaching the arm the eating position
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#pragma once

#include "moveByDegree.h"

class manualAdjust
{
private:
	int mode = 0; //0 for normal mode, 1 for record mode
	int goal_position[4] = { 1500, 500, 250, 250 };  // Goal position
	moveByDegree *m_;
	logger *log_;

public:
	manualAdjust();
	~manualAdjust();

	void init(moveByDegree *m, logger *log);

	int loosenGrip();
	int tightenGrip();
	void newGoal();
	void sendGoal();
};

