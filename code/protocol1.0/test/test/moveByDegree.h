#pragma once
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

#include "dynamixel_sdk.h"  

using namespace std;
class moveByDegree
{
private:
	const int m1_min_degree = 0;
	const int m1_max_degree = 90;
	const int m2_min_degree = 5;
	const int m2_max_degree = 90;
	const int m3_min_degree = 0;
	const int m3_max_degree = 90;
	const int m4_min_degree = 0;
	const int m4_max_degree = 90;

	const int m1_min_value = 1100;
	const int m1_max_value = 2100;
	const int m1_min_value = 150;
	const int m1_max_value = 1020;
	const int m1_min_value = 220;
	const int m1_max_value = 500;
	const int m1_min_value = 200;
	const int m1_max_value = 500;

public:
	moveByDegree();
	~moveByDegree();

	int calculateUnit(int id);
	void checkDegree(int id, int degree);
	void move(int id, int degree);
};

