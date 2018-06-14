/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
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

#include "./test/moveByDegree.h"

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

	moveByDegree m1 = moveByDegree();
	int thres = m1.getThreshold();

	// Open port
	if (m1.checkPort() == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	//Set Baudrate
	if (m1.setBaudRate() == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	//Set Max Torque
	if (m1.setMaxTorque(1, 150) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.setMaxTorque(2, 700) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.setMaxTorque(3, 200) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.setMaxTorque(4, 150) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	// Enable Torque
	if (m1.enableTorque(1) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.enableTorque(2) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.enableTorque(3) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.enableTorque(4) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};

	
	while (1)
	{
		printf("Press any key to continue! (or press ESC to quit!)\n");
		if (getchar() == ESC_ASCII_VALUE)
			break;

		//write each value for moving the motor in form of id-theta value in degree to move to
		if (m1.move(1, 54) == 1) {
			printf("Move failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.move(2, 65) == 1) {
			printf("Move failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.move(3, 23) == 1) {
			printf("Move failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.move(4, 76) == 1) {
			printf("Move failed, press any key to terminate...\n");
			getchar();
			break;
		};

		// Syncwrite goal position
		if (m1.writeAll() == 1) {
			printf("Syncwrite, press any key to terminate...\n");;
			getchar();
			break;
		};

	do
		{
		// Read goal position
		if (m1.read(1) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.read(2) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.read(3) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};
		if (m1.read(4) == 1) {
			printf("Read failed, press any key to terminate...\n");
			getchar();
			break;
		};

		//print the status
		m1.print();

		//check if the motor has reached destination
		}
	while ((abs(m1.dxl_goal_position[0] - m1.dxl_present_position[0]) > thres) || (abs(m1.dxl_goal_position[1] - m1.dxl_present_position[1]) >thres)
			|| (abs(m1.dxl_goal_position[2] - m1.dxl_present_position[2]) > thres) || (abs(m1.dxl_goal_position[3] - m1.dxl_present_position[3]) > thres));

	}

	//disable torque
	m1.disableTorque(1);
	m1.disableTorque(2);
	m1.disableTorque(3);
	m1.disableTorque(4);

	// Close port
	m1.closePort();

	//Sleep so there's time to read the error log
	sleep_for(1s);
	return 0;
}

