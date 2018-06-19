/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* Function for initializing the motors as well as moving them by degree
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#include <stdio.h>
#include <stdlib.h>
#include "dynamixel_sdk.h"  

// Control table address
#define ADDR_MX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_PRESENT_POSITION        36
#define ADDR_MX_MAX_TORQUE				34		


// Data Byte Length
#define LEN_MX_GOAL_POSITION            2
#define LEN_MX_PRESENT_POSITION         2

// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define BAUDRATE                        1000000
#define DEVICENAME                      "COM4"      

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold

#define ESC_ASCII_VALUE                 0x1b

using namespace std;
class moveByDegree
{
private:
	const int min_degree[4] = {0, 5, 0, 0};
	const int max_degree[4] = {90, 90, 90, 90};

	const int min_value[4] = { 1100, 150, 220, 200 };
	const int max_value[4] = { 2100, 1020, 500, 500 };

	int dxl_goal_position[4] = { 200, 200, 200, 200 };  // Goal position
	uint16_t dxl_present_position[4] = { 0, 0, 0, 0 };
	int max_torque[4];
	int index = 0;
	int dxl_comm_result = COMM_TX_FAIL;            
	bool dxl_addparam_result = false;               


	uint8_t dxl_error = 0;                          // Dynamixel error
	uint8_t param_goal_position[2];

	//dynamixel initialization
	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	dynamixel::GroupSyncWrite group = dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION);


public:
	moveByDegree();
	~moveByDegree();

	int calculateUnit(int id);
	int checkDegree(int id, int degree);
	int move(int id, int degree);

	//setting up function
	int checkPort(void); //open port
	int setBaudRate(void); //set Baudrate
	int setMaxTorque(int id, int torque); //set the torque for EACH motor
	int enableTorque(int id); //enable torque of EACH motor
	int writeAll(void); //write all goal value into syncwrite
	int read(int id); //read the current value of EACH motor
	void print(void); //print current and goal value of ALL motor
	int disableTorque(int id); //disable torque of EACH motor
	void closePort(void); //close communication port


	//getter
	int getGoal(int id) const;
	int getPresent(int id) const;
	int getThreshold() const;
	int getMaxTorque(int id) const;

	//setter
	void setGoal(int id, int pos);
};

