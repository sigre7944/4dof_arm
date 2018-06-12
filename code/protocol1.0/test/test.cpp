/* Rewritten by Ha Nguyen
* Original Author: Ryu Woon Jung (Leon)
*
* Test code for the four motors of the arm
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/


#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#include <chrono>
#include <thread>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "dynamixel_sdk.h"                                  // Uses Dynamixel SDK library

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
#define DXL1_ID                         1                   // Dynamixel#1
#define DXL2_ID                         2                   // Dynamixel#2
#define DXL3_ID                         3                   // Dynamixel#3
#define DXL4_ID                         4                   // Dynamixel#4
#define BAUDRATE                        1000000
#define DEVICENAME                      "COM4"      

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define MAX_TORQUE						0x120				//Value for maximum torque
#define DXL_1_MINIMUM_POSITION_VALUE    1100                 // Dynamixel will rotate between this value
#define DXL_1_MAXIMUM_POSITION_VALUE    1100                // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_2_MINIMUM_POSITION_VALUE    1020                
#define DXL_2_MAXIMUM_POSITION_VALUE    1020
#define DXL_3_MINIMUM_POSITION_VALUE    220                 // Dynamixel will rotate between this value
#define DXL_3_MAXIMUM_POSITION_VALUE    220
#define DXL_4_MINIMUM_POSITION_VALUE    200                 // Dynamixel will rotate between this value
#define DXL_4_MAXIMUM_POSITION_VALUE    500
#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold

#define ESC_ASCII_VALUE                 0x1b

int getchar() //defining getch for different builds
{
#if defined(__linux__) || defined(__APPLE__)
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
#elif defined(_WIN32) || defined(_WIN64)
	return _getch();
#endif
}

int kbhit(void) //defining kbhit for different builds
{
#if defined(__linux__) || defined(__APPLE__)
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
#elif defined(_WIN32) || defined(_WIN64)
	return _kbhit();
#endif
}

int main()
{
	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;

	// Initialize PortHandler instance
	// Set the port path
	// Get methods and members of PortHandlerLinux or PortHandlerWindows
	dynamixel::PortHandler *portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);

	// Initialize PacketHandler instance
	// Set the protocol version
	// Get methods and members of Protocol1PacketHandler or Protocol2PacketHandler
	dynamixel::PacketHandler *packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

	// Initialize GroupSyncWrite instance
	dynamixel::GroupSyncWrite groupSyncWrite(portHandler, packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION);

	int index = 0;
	int dxl_comm_result = COMM_TX_FAIL;             // Communication result
	bool dxl_addparam_result = false;               // addParam result
	int dxl_1_goal_position[2] = { DXL_1_MINIMUM_POSITION_VALUE, DXL_1_MAXIMUM_POSITION_VALUE };  // Goal position
	int dxl_2_goal_position[2] = { DXL_2_MINIMUM_POSITION_VALUE, DXL_2_MAXIMUM_POSITION_VALUE };  // Goal position
	int dxl_3_goal_position[2] = { DXL_3_MINIMUM_POSITION_VALUE, DXL_3_MAXIMUM_POSITION_VALUE };  // Goal position
	int dxl_4_goal_position[2] = { DXL_4_MINIMUM_POSITION_VALUE, DXL_4_MAXIMUM_POSITION_VALUE };  // Goal position

	uint8_t dxl_error = 0;                          // Dynamixel error
	uint8_t param_1_goal_position[2];
	uint8_t param_2_goal_position[2];
	uint8_t param_3_goal_position[2];
	uint8_t param_4_goal_position[2];
	uint16_t dxl1_present_position = 0, dxl2_present_position = 0, dxl3_present_position = 0, dxl4_present_position = 0;                        // Present position

																																				// Open port
	if (portHandler->openPort())
	{
		printf("Succeeded to open the port!\n");
	}
	else
	{
		printf("Failed to open the port!\n");
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	}

	// Set port baudrate
	if (portHandler->setBaudRate(BAUDRATE))
	{
		printf("Succeeded to change the baudrate!\n");
	}
	else
	{
		printf("Failed to change the baudrate!\n");
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	}

	// Set Maximum Torque #1
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL1_ID, ADDR_MX_MAX_TORQUE, 150, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("The Max Torque has been set successfully\n");
	}

	// Set Maximum Torque #2
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL2_ID, ADDR_MX_MAX_TORQUE, 700, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("The Max Torque has been set successfully\n");
	}

	// Set Maximum Torque #3
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL3_ID, ADDR_MX_MAX_TORQUE, 200, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("The Max Torque has been set successfully\n");
	}

	// Set Maximum Torque #4
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL4_ID, ADDR_MX_MAX_TORQUE, 150, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("The Max Torque has been set successfully\n");
	}

	// Enable Dynamixel#1 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("Dynamixel#%d has been successfully connected \n", DXL1_ID);
	}

	// Enable Dynamixel#2 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("Dynamixel#%d has been successfully connected \n", DXL2_ID);
	}

	// Enable Dynamixel#3 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL3_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("Dynamixel#%d has been successfully connected \n", DXL3_ID);
	}

	// Enable Dynamixel#4 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL4_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}
	else
	{
		printf("Dynamixel#%d has been successfully connected \n", DXL4_ID);
	}

	while (1)
	{
		printf("Press any key to continue! (or press ESC to quit!)\n");
		if (getchar() == ESC_ASCII_VALUE)
			break;

		// Allocate goal position value into byte array
		param_1_goal_position[0] = DXL_LOBYTE(dxl_1_goal_position[index]);
		param_1_goal_position[1] = DXL_HIBYTE(dxl_1_goal_position[index]);
		param_2_goal_position[0] = DXL_LOBYTE(dxl_2_goal_position[index]);
		param_2_goal_position[1] = DXL_HIBYTE(dxl_2_goal_position[index]);
		param_3_goal_position[0] = DXL_LOBYTE(dxl_3_goal_position[index]);
		param_3_goal_position[1] = DXL_HIBYTE(dxl_3_goal_position[index]);
		param_4_goal_position[0] = DXL_LOBYTE(dxl_4_goal_position[index]);
		param_4_goal_position[1] = DXL_HIBYTE(dxl_4_goal_position[index]);

		// Add Dynamixel#1 goal position value to the Syncwrite storage
		dxl_addparam_result = groupSyncWrite.addParam(DXL1_ID, param_1_goal_position);
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", DXL1_ID);
			return 0;
		}


		// Add Dynamixel#2 goal position value to the Syncwrite parameter storage
		dxl_addparam_result = groupSyncWrite.addParam(DXL2_ID, param_2_goal_position);
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", DXL2_ID);
			return 0;
		}

		// Add Dynamixel#3 goal position value to the Syncwrite parameter storage
		dxl_addparam_result = groupSyncWrite.addParam(DXL3_ID, param_3_goal_position);
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", DXL3_ID);
			return 0;
		}

		// Add Dynamixel#4 goal position value to the Syncwrite parameter storage
		dxl_addparam_result = groupSyncWrite.addParam(DXL4_ID, param_4_goal_position);
		if (dxl_addparam_result != true)
		{
			fprintf(stderr, "[ID:%03d] groupSyncWrite addparam failed", DXL4_ID);
			return 0;
		}

		// Syncwrite goal position
		dxl_comm_result = groupSyncWrite.txPacket();
		if (dxl_comm_result != COMM_SUCCESS) packetHandler->printTxRxResult(dxl_comm_result);

		// Clear syncwrite parameter storage
		groupSyncWrite.clearParam();

		do
		{
			// Read Dynamixel#1 present position
			dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL1_ID, ADDR_MX_PRESENT_POSITION, &dxl1_present_position, &dxl_error);
			if (dxl_comm_result != COMM_SUCCESS)
			{
				packetHandler->printTxRxResult(dxl_comm_result);
			}
			else if (dxl_error != 0)
			{
				packetHandler->printRxPacketError(dxl_error);
			}

			// Read Dynamixel#2 present position
			dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL2_ID, ADDR_MX_PRESENT_POSITION, &dxl2_present_position, &dxl_error);
			if (dxl_comm_result != COMM_SUCCESS)
			{
				packetHandler->printTxRxResult(dxl_comm_result);
			}
			else if (dxl_error != 0)
			{
				packetHandler->printRxPacketError(dxl_error);
			}

			// Read Dynamixel#3 present position
			dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL3_ID, ADDR_MX_PRESENT_POSITION, &dxl3_present_position, &dxl_error);
			if (dxl_comm_result != COMM_SUCCESS)
			{
				packetHandler->printTxRxResult(dxl_comm_result);
			}
			else if (dxl_error != 0)
			{
				packetHandler->printRxPacketError(dxl_error);
			}

			// Read Dynamixel#4 present position
			dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL4_ID, ADDR_MX_PRESENT_POSITION, &dxl4_present_position, &dxl_error);
			if (dxl_comm_result != COMM_SUCCESS)
			{
				packetHandler->printTxRxResult(dxl_comm_result);
			}
			else if (dxl_error != 0)
			{
				packetHandler->printRxPacketError(dxl_error);
			}

			//Print all position
			printf("[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n[ID:%03d] GoalPos:%03d  PresPos:%03d\t[ID:%03d] GoalPos:%03d  PresPos:%03d\n",
				DXL1_ID, dxl_1_goal_position[index], dxl1_present_position, DXL2_ID, dxl_2_goal_position[index], dxl2_present_position,
				DXL3_ID, dxl_3_goal_position[index], dxl3_present_position, DXL4_ID, dxl_4_goal_position[index], dxl4_present_position);

		} while ((abs(dxl_1_goal_position[index] - dxl1_present_position) > DXL_MOVING_STATUS_THRESHOLD) || (abs(dxl_2_goal_position[index] - dxl2_present_position) > DXL_MOVING_STATUS_THRESHOLD)
			|| (abs(dxl_3_goal_position[index] - dxl3_present_position) > DXL_MOVING_STATUS_THRESHOLD) || (abs(dxl_4_goal_position[index] - dxl4_present_position) > DXL_MOVING_STATUS_THRESHOLD));

		sleep_for(3s); //delay before changing position again
					   // Change goal position
		if (index == 0)
		{
			index = 1;
		}
		else
		{
			index = 0;
		}
	}

	// Disable Dynamixel#1 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}

	// Disable Dynamixel#2 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}

	// Disable Dynamixel#3 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL3_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}

	// Disable Dynamixel#4 Torque
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL4_ID, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
	}

	// Close port
	portHandler->closePort();

	return 0;
}

