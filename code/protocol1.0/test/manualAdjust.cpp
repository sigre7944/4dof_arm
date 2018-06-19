/* Written Ha Nguyen
*
* Function for initializing the motors as well as moving them by d
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#include "manualAdjust.h"

manualAdjust::manualAdjust()
{
}

void manualAdjust::init(moveByDegree *m) {
	m_ = m;
}

int manualAdjust::loosenGrip() {
	if (m_->setMaxTorque(1, 0) == 1) {
		return 1;
	};
	if (m_->setMaxTorque(2, 100) == 1) {
		return 1;
	};
	if (m_->setMaxTorque(3, 50) == 1) {
		return 1;
	};
	if (m_->setMaxTorque(4, 50) == 1) {
		return 1;
	}
	printf("Loosening Grip\n");
	return 0;
}

int manualAdjust::tightenGrip() {
	if (m_->setMaxTorque(1, m_->getMaxTorque(1)) == 1) {
		printf("Press any key to terminate...\n");
		return 1;
	};
	if (m_->setMaxTorque(2, m_->getMaxTorque(2)) == 1) {
		printf("Press any key to terminate...\n");
		return 1;
	};
	if (m_->setMaxTorque(3, m_->getMaxTorque(3)) == 1) {
		printf("Press any key to terminate...\n");
		return 1;
	};
	if (m_->setMaxTorque(4, m_->getMaxTorque(4)) == 1) {
		printf("Press any key to terminate...\n");
		return 1;
	}
	printf("Tightening Grip\n");
	return 0;
}

int manualAdjust::checkForInput(int value) {
	mode = value;
	return 0;
}

void manualAdjust::newGoal() {
	dxl_goal_position[0] = m_->read(1);
	dxl_goal_position[1] = m_->read(2);
	dxl_goal_position[2] = m_->read(3);
	dxl_goal_position[3] = m_->read(4);
	printf("Reading in new goal\n");
}

void manualAdjust::sendGoal() {
	m_->setGoal(1, dxl_goal_position[0]);
	m_->setGoal(2, dxl_goal_position[1]);
	m_->setGoal(3, dxl_goal_position[2]);
	m_->setGoal(4, dxl_goal_position[3]);
	printf("Sending new goal\n");
}

manualAdjust::~manualAdjust()
{
}
