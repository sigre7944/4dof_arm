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
	logger log1 = logger(); //initate for logging
	inverseKinematics ik = inverseKinematics(250, 50, 200, 50);

	m1.init(&log1);
	man.init(&m1, &log1); //parse the moving functions to manual 
	con.init(&m1, &man, &log1, &ik); //parse the moving and manual functions to CONTROL
	ik.init(&log1); //give logging about inverse kinematics calculation

	con.startup(); //connect to the motors and basic setting

	while (1) {
		if (con.getState() == 1)
		{
			break;
		};

		m1.clearAll();
		sleep_for(500ms);
		
	}

	log1.output2File(); //write log file

	con.shutDown();

	//Sleep so there's time to read the error log
	sleep_for(1s);
	return 0;
}

