/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* Function for all action and communication between the users and the program
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#pragma once

#include "manualAdjust.h"
#include "inverseKinematics.h"

class control
{
private:
	unsigned char state = 1; 
	int input = -1;
	moveByDegree *move_;
	manualAdjust *man_;
	inverseKinematics *ik_;
	logger *log_;

public:
	control();
	~control();

	void init(moveByDegree *move, manualAdjust *man, logger *log, inverseKinematics *ik);
	int startup(); // all work needed for initializing the systems
	int getState(); //get the command from the user
	void printState(); //print the command from the user

	int manual(); //manual mode
	int move(); //auto move mode
	int run(); //run till reach destination
	int eat(); //run to eating destination
	void stayStill(); //stay at its current position
	int shutDown(); //disable torque
};

