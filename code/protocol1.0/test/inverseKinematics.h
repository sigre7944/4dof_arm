/* Written Ha Nguyen
*
*
* Function for calculating the inverse kinematics of the arm and return all theta degree
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#pragma once
#include <math.h>
#include "log.h"

class inverseKinematics
{
private:
	const float l0 = 50;
	const float l1 = 65;
	const float l2 = 185;
	const float l3 = 115;
	const float l4 = 80;
	const float alpha2 = -90;
	float theta[4] = {0 ,0, 0, 0};

	float _px, _py, _pz, _phi;
	float z1, r1;


	logger *log_;

public:
	inverseKinematics(float px, float py, float pz, float phi);
	~inverseKinematics();

	float deg(float rad);
	float getTheta(int id);

	void init(logger *log);

	void calAll();
	void cal1();
	void cal2();
	void cal3();
	void cal4();

};
