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
	if (state == -1) {
		printf("Please enter from 0 to 1 for your desired state\n");
		printf("Press ESC to quit the program\n");
		printf("0 for moving between plate and eating locations\n");
		printf("1 for locating the eating position\n");
	}

	state = getchar();
	if (state == ESC_ASCII_VALUE) return 1;
	switch (state) {
	case 0x30 : 
		break;
	case 0x31 : manual();
		printf("Press any key to continue\n");
		getchar();
		getchar();
		break;
	default: printf("Try another input\n");
		printf("%03d\n", state);
		break;
	}
	return 1;
}

void control::printState()
{
}

int control::manual()
{
	man_->loosenGrip();
	printf("Enter '1' when you have found the right position\n");
	getchar();
	if (getchar() == 0x31) {
		man_->newGoal();
		man_->sendGoal();
		move_->stayStill();
		printf("Eating position set\n");
		return 1;
	}

}

int control::move() {
	if (move_->move(1, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->move(2, 5) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->move(3, 90) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->move(4, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 1;
	};

	if (move_->setMaxTorque(1, 150) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->setMaxTorque(2, 500) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->setMaxTorque(3, 200) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (move_->setMaxTorque(4, 150) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 1;
	};

	if (move_->writeAll() == 1) {
		printf("Syncwrite, press any key to terminate...\n");;
		getchar();
		return 0;
	};

	run();
	return 0;
}

int control::run() {
	int thres = move_->getThreshold();

	do
	{
		// Read goal position
		if (move_->read(1) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (move_->read(2) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (move_->read(3) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (move_->read(4) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};

		//print the status
		move_->print();

		//check if the motor has reached destination
	} while ((abs(move_->getGoal(1) - move_->getPresent(1)) > thres) || (abs(move_->getGoal(2) - move_->getPresent(2)) >thres)
		|| (abs(move_->getGoal(3) - move_->getPresent(3)) > thres) || (abs(move_->getGoal(4) - move_->getPresent(4)) > thres));

	return 0;
}

control::~control()
{
}
