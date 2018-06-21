#pragma once

#include "./manualAdjust.h"
#include "dependency.h"

class control
{
private:
	int state = -1; 
	int input = 2;
	moveByDegree *move_;
	manualAdjust *man_;

public:
	control();
	~control();

	void init(moveByDegree *move, manualAdjust *man);
	int startup(); // all work needed for initializing the systems
	int getState(); //get the command from the user
	void printState(); //print the command from the user

	int manual(); //manual mode
	int move(); //auto move mode
	int run(); //run till reach destination
	int eat(); //run to eating destination
};

