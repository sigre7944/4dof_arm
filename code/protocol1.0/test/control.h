#pragma once

#include "./manualAdjust.h"
#include "dependency.h"

class control
{
private:
	int state = 0; 
	moveByDegree *move_;
	manualAdjust *man_;

public:
	control();
	~control();

	void init(moveByDegree *move, manualAdjust *man);
	int startup();
	int getState();
	void printState();
	int manual();
	int move();
	void askForInput();

};

