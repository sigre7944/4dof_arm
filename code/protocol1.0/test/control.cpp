/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* Function for all action and communication between the users and the program
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#include "control.h"

control::control()
{
}

void control::init (moveByDegree *move, manualAdjust *man, logger *log)
{
	move_ = move;
	man_ = man;
	log_ = log;
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
	if (state == 1) {
		printf("Please enter from 0 to 1 for your desired state\n");
		printf("Press ESC to quit the program\n");
		printf("1 for locating the eating position\n");
		printf("Any other key for moving between plate and eating locations\n");
	}

	input = getchar();
	if (input == ESC_ASCII_VALUE) return 1;
	switch (input) {
	case 0x31 : 
		state = 255; 
		manual();
		printf("Press any key but 1 to continue (or press ESC to quit!)\n");
		if (getchar() == ESC_ASCII_VALUE) return 1;
		break;
	default: 
		printState();
		if (state == 0) {
			eat();
		}
		else if (state == 255) {
			move();
		}
		state = ~state;
		printf("Press any key to continue(or press ESC to quit!)\n");
		if (getchar() == ESC_ASCII_VALUE) return 1;
		break;
	}
	return 0;
}

void control::printState()
{
	if (state == 0) printf("Eating\n");
	if (state == 255) printf("To the Plate\n");
}

int control::manual()
{
	man_->loosenGrip();
	printf("Enter '1' when you have found the right position\n");
	getchar();
	if (getchar() == 0x31) {
		man_->newGoal();
		man_->sendGoal();
		stayStill();
		printf("Eating position set\n");
		log_->Add(logINFO) << "Eating position set" << endl;
	}
	return 0;
}

int control::move() {
	if (move_->move(1, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->move(2, 5) == 1) {
		printf("Move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->move(3, 90) == 1) {
		printf("Move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->move(4, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Move Failed" << endl;
		getchar();
		return 1;
	};

	move_->writeAll();

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
			log_->Add(logERROR) << "Read Failed" << endl;
			getchar();
			break;
		};
		if (move_->read(2) == 1) {
			printf("Read failed, press any key to terminate...\n");
			log_->Add(logERROR) << "Read Failed" << endl;
			getchar();
			break;
		};
		if (move_->read(3) == 1) {
			printf("Read failed, press any key to terminate...\n");
			log_->Add(logERROR) << "Read Failed" << endl;
			getchar();
			break;
		};
		if (move_->read(4) == 1) {
			printf("Read failed, press any key to terminate...\n");
			log_->Add(logERROR) << "Read Failed" << endl;
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

int control::eat() {
	man_->sendGoal();

	if (move_->autoMove(1) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Auto Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->autoMove(2) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Auto Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->autoMove(3) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Auto Move Failed" << endl;
		getchar();
		return 1;
	};
	if (move_->autoMove(4) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		log_->Add(logERROR) << "Auto Move Failed" << endl;
		getchar();
		return 1;
	};

	move_->writeAll();
	run();
	return 0;
}

void control::stayStill() {
	move_->autoMove(1);
	move_->autoMove(2);
	move_->autoMove(3);
	move_->autoMove(4);

	move_->setMaxTorque(1, 150);
	move_->setMaxTorque(2, 500);
	move_->setMaxTorque(3, 200);
	move_->setMaxTorque(4, 150);

	move_->writeAll();
}

int control::shutDown() {
	if (move_->disableTorque(1) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		log_->Add(logERROR) << "Cannot disabble torque" << endl;
		getchar();
		return 1;
	};
	if (move_->disableTorque(2) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		log_->Add(logERROR) << "Cannot disabble torque" << endl;
		getchar();
		return 1;
	};
	if (move_->disableTorque(3) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		log_->Add(logERROR) << "Cannot disabble torque" << endl;
		getchar();
		return 1;
	};
	if (move_->disableTorque(4) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		log_->Add(logERROR) << "Cannot disabble torque" << endl;
		getchar();
		return 1;
	};

	move_->closePort();

}

control::~control()
{
}
