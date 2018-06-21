#include "control.h"

control::control()
{
}

void control::init (moveByDegree *move, manualAdjust *man)
{
	move_ = move;
	man_ = man;
}

int control::startup() 
{
	// Open port
	if (move_->checkPort() == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	//Set Baudrate
	if (move_->setBaudRate() == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	// Enable Torque
	if (move_->enableTorque(1) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (move_->enableTorque(2) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (move_->enableTorque(3) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (move_->enableTorque(4) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
}

int control::getState()
{
	printf("Please enter from 0 to 2 for your desired state\n");
	printf("Press ESC to quit the program\n");
	printf("0 for automatically move to the plate position\n");
	printf("1 for locating the eating position\n");
	printf("2 for changing position between plate and eating\n");

	state = getchar();
	if (state == ESC_ASCII_VALUE) return 1;
	else return 0;
}

void control::printState()
{
}

int control::manual()
{
	return 0;
}

int control::move() {
	return 0;
}

void control::askForInput() {

}

control::~control()
{
}
