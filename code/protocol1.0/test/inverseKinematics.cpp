/* Written Ha Nguyen
*
*
* Function for calculating the inverse kinematics of the arm and return all theta degree
*
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/


#include "inverseKinematics.h"



inverseKinematics::inverseKinematics(float px, float py, float pz, float phi)
{
	_px = px;
	_py = py;
	_pz = pz;
	_phi = phi;
	z1 = _pz - l1 - l0;
	r1 = _px;
}

void inverseKinematics::init(logger *log) {
	log_ = log;
}

void inverseKinematics::calAll() {
	cal1();
	cal3();
	cal2();
	cal4();
}

void inverseKinematics::cal1() { //1st calculation
	theta[0] = atan2(_py, _px);

	theta[0] = deg(theta[0]);
	log_->Add(logDEBUG) << "Value of theta1 is " << theta[0] << endl;
}

void inverseKinematics::cal3() { //2nd calculation
	theta[2] = acos( (pow(z1, 2) + pow(r1, 2) - pow(l2, 2) - pow(l3, 2)) / (2 * l2*l3));

	theta[2] = deg(theta[2]);
	log_->Add(logDEBUG) << "Value of z1 is " << z1 << endl;
	log_->Add(logDEBUG) << "Value of r1 is " << r1 << endl;
	log_->Add(logDEBUG) << "Value of theta3 is " << theta[2] << endl;
}

void inverseKinematics::cal2() { //3rd calculation
	float beta = deg(atan2(z1, r1));
	float psi = deg(acos((pow(z1, 2) + pow(r1, 2) + pow(l2, 2) - pow(l3,2)) / (2*sqrt(pow(r1,2) + pow(z1,2))*l2)));


	if (theta[2] >= 0) {
		theta[1] = 90-(90 - beta - psi);
	}
	else if (theta[2] < 0) {
		theta[1] = 90-(90 - beta + psi);
	}
	log_->Add(logDEBUG) << "Value of psi is " << psi << endl;
	log_->Add(logDEBUG) << "Value of beta is " << beta << endl;
	log_->Add(logDEBUG) << "Value of theta2 is " << theta[1] << endl;
}

void inverseKinematics::cal4() {//4th calculation
	theta[3] = _phi - theta[1] - theta[2] + 90;
	log_->Add(logDEBUG) << "Value of theta4 is " << theta[3] << endl;
}

float inverseKinematics::deg(float rad) {
	return rad * 180 / 3.1416;
}

float inverseKinematics::getTheta(int id) {
	return theta[id - 1];
}


inverseKinematics::~inverseKinematics()
{
}
