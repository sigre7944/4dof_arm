/* Written Ha Nguyen
*
*
* Test code for the four motors of the arm
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#include "./test/inverseKinematics.h" // for inverse kinematics computation
#include "./test/control.h" //for manipulating all action using the STATE input

int main()
{
	moveByDegree m1 = moveByDegree(); //initiate for setting up functions and move motor by degree value
	manualAdjust man = manualAdjust(); //initate for manual adjusting to eating position
	control con = control(); //initate for all controlling command
	man.init(&m1);
	con.init(&m1, &man);

	int thres = m1.getThreshold();

	con.startup(); //connect to the motors and basic setting

	while (1) {
		if (con.getState() == 1)
		{
			break;
		};
		m1.clearAll();
		printf("Press any key to continue! (or press ESC to quit!)\n");
		getchar();
		if (getchar() == ESC_ASCII_VALUE) return 0;
	}

	

	//write each value for moving the motor in form of id-theta value in degree to move to
	if (m1.move(1, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.move(2, 5) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.move(3, 90) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.move(4, 0) == 1) {
		printf("Move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};

	//Set Max Torque
	if (m1.setMaxTorque(1, 150) == 1) {
		printf("Press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.setMaxTorque(2, 500) == 1) {
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
	



	// Syncwrite goal position
	if (m1.writeAll() == 1) {
		printf("Syncwrite, press any key to terminate...\n");;
		getchar();
		return 0;
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
	while ((abs(m1.getGoal(1) - m1.getPresent(1)) > thres) || (abs(m1.getGoal(2) - m1.getPresent(2)) >thres)
			|| (abs(m1.getGoal(3) - m1.getPresent(3)) > thres) || (abs(m1.getGoal(4) - m1.getPresent(4)) > thres));


	m1.clearAll();
	sleep_for(1s);
	man.sendGoal();
	printf("Press any key to continue! (or press ESC to quit!)\n");
	getchar();
	if (getchar() == ESC_ASCII_VALUE) return 0;

	//Move according to sent position
	if (m1.autoMove(1) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.autoMove(2) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.autoMove(3) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};
	if (m1.autoMove(4) == 1) {
		printf("Auto move failed, press any key to terminate...\n");
		getchar();
		return 0;
	};

	// Syncwrite goal position
	if (m1.writeAll() == 1) {
		printf("Syncwrite, press any key to terminate...\n");;
		getchar();
		return 0;
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
	} while ((abs(m1.getGoal(1) - m1.getPresent(1)) > thres) || (abs(m1.getGoal(2) - m1.getPresent(2)) >thres)
			|| (abs(m1.getGoal(3) - m1.getPresent(3)) > thres) || (abs(m1.getGoal(4) - m1.getPresent(4)) > thres));

	printf("Press any key to continue! (or press ESC to quit!)\n");
	getchar();
	if (getchar() == ESC_ASCII_VALUE) return 0;

	//disable torque
	if (m1.disableTorque(1) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (m1.disableTorque(2) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (m1.disableTorque(3) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		getchar();
		return 1;
	};
	if (m1.disableTorque(4) == 1) {
		printf("Cannot disabble torque, press any key to terminate...\n");
		getchar();
		return 1;
	};

	// Close port
	m1.closePort();

	//Sleep so there's time to read the error log
	sleep_for(1s);
	return 0;
}

