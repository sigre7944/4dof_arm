/* Written Ha Nguyen
*
* Function for teaching the arm the eating position
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
	if (m_->setMaxTorque(2, 50) == 1) {
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
		return 1;
	};
	if (m_->setMaxTorque(2, m_->getMaxTorque(2)) == 1) {
		return 1;
	};
	if (m_->setMaxTorque(3, m_->getMaxTorque(3)) == 1) {
		return 1;
	};
	if (m_->setMaxTorque(4, m_->getMaxTorque(4)) == 1) {
		return 1;
	}
	printf("Tightening Grip\n");
	return 0;
}


void manualAdjust::newGoal() {
	printf("Reading in new goal\n");
	m_->read(1);
	goal_position[0] = m_->getPresent(1);
	printf("[ID:%03d] GoalPos:%03d\n", 1, m_->getPresent(1));

	m_->read(2);
	goal_position[1] = m_->getPresent(2);
	printf("[ID:%03d] GoalPos:%03d\n", 2, m_->getPresent(2));

	m_->read(3);
	goal_position[2] = m_->getPresent(3);
	printf("[ID:%03d] GoalPos:%03d\n", 3, m_->getPresent(3));

	m_->read(4);
	goal_position[3] = m_->getPresent(4);
	printf("[ID:%03d] GoalPos:%03d\n", 4, m_->getPresent(4));

}

void manualAdjust::sendGoal() {
	m_->setGoal(1, goal_position[0]);
	m_->setGoal(2, goal_position[1]);
	m_->setGoal(3, goal_position[2]);
	m_->setGoal(4, goal_position[3]);
	printf("Sending new goal\n");
}

manualAdjust::~manualAdjust()
{
}
