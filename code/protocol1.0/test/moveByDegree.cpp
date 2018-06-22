/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* Function for initializing the motors as well as moving them by degree value
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/
#include "moveByDegree.h"

moveByDegree::moveByDegree()
{
}

int moveByDegree::checkDegree(int id, int degree)
{
	if (degree > max_degree[id-1] || degree < min_degree[id-1]) return 0;
	else return 1;
}

int moveByDegree::calculateUnit(int id) {
	return (max_value[id-1] - min_value[id-1]) / (max_degree[id-1] - min_degree[id-1]);
}


int moveByDegree::move(int id, int degree) {
	if (checkDegree(id, degree) == false) {
		fprintf(stderr, "[ID:%03d] The degree is wrong", id);

		return 1;
	}
	else
	{	
		dxl_goal_position[id-1] = degree * calculateUnit(id)+min_value[id-1];
		// Allocate goal position value into byte array
		param_goal_position[0] = DXL_LOBYTE(dxl_goal_position[id-1]);
		param_goal_position[1] = DXL_HIBYTE(dxl_goal_position[id-1]);

		dxl_addparam_result = group.addParam(id, param_goal_position);//already made initialize
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", id);
			return 1;
		}
		return 0;
	}
}

int moveByDegree::autoMove(int id) {
		// Allocate goal position value into byte array
		param_goal_position[0] = DXL_LOBYTE(dxl_goal_position[id - 1]);
		param_goal_position[1] = DXL_HIBYTE(dxl_goal_position[id - 1]);

		dxl_addparam_result = group.addParam(id, param_goal_position);//already made initialize
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", id);
			return 1;
		}
		return 0;
}


int moveByDegree::writeAll(void) {
	dxl_comm_result = group.txPacket();
	if (dxl_comm_result != COMM_SUCCESS) { 
		packetHandler->printTxRxResult(dxl_comm_result); 
		return 1;
	}
	return 0;
}

void moveByDegree::clearAll() {
	group.clearParam();
}

int moveByDegree::checkPort(void) {
	if (portHandler->openPort())
	{
		printf("Succeeded to open the port!\n");
		return 0;
	}
	else
	{
		printf("Failed to open the port!\n");
		return 1;
	}
}

int moveByDegree::setBaudRate(void) {
	if (portHandler->openPort())
	{
		printf("Succeeded to set baudrate!\n");
		return 0;
	}
	else
	{
		printf("Failed to open the port!\n");
		return 1;
	}
}

int moveByDegree::setMaxTorque(int id, int torque){
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MAX_TORQUE, torque, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		max_torque[id - 1] = torque;
		//printf("The Max Torque has been set successfully\n");
		return 0;
	}
}

int moveByDegree::enableTorque(int id) {
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	else
	{
		printf("Dynamixel#%d has been successfully connected \n", id);
		return 0;
	}
}

int moveByDegree::read(int id) {
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_PRESENT_POSITION, &dxl_present_position[id-1], &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	//printf("Reading...");
	return 0;
}

void moveByDegree::print(void) {
	printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n",
		1, dxl_goal_position[0], dxl_present_position[0], 2, dxl_goal_position[1], dxl_present_position[1],
		3, dxl_goal_position[2], dxl_present_position[2], 4, dxl_goal_position[3], dxl_present_position[3]);
}

int moveByDegree::getGoal(int id) const {
	return dxl_goal_position[id-1];
}

int moveByDegree::getPresent(int id) const {
	return dxl_present_position[id-1];
}

int moveByDegree::getThreshold() const {
	return DXL_MOVING_STATUS_THRESHOLD;
}

int moveByDegree::getMaxTorque(int id) const{
	return max_torque[id - 1];
}
	
void moveByDegree::setGoal(int id, int pos){
	dxl_goal_position[id - 1] = pos;
	printf("[ID:%03d] Goal Changed:%03d\n", id, pos);
}

int moveByDegree::disableTorque(int id) {
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return 1;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return 1;
	}
	printf("[ID:%03d] Torque disabled\n", id);
	return 0;
}

void moveByDegree::closePort(void) {
	portHandler->closePort();
}

moveByDegree::~moveByDegree()
{
}
