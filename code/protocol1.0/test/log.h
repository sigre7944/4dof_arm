/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* Function for writing log file for the program
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#ifndef __LOG_H__
#define __LOG_H__

#define _CRT_SECURE_NO_WARNINGS	//for using sprintf

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "dependency.h"

#if defined(_WIN32) || defined(_WIN64)
// Win Socket Header File(s)
#include <Windows.h>
#include <process.h>
#else
// POSIX Socket Header File(s)
#include <errno.h>
#include <pthread.h>
#endif


using namespace std;

enum logLevel 
{ 
	logERROR = 0,
	logWARNING = 1,
	logINFO = 2,
	logDEBUG = 3 
};

class logger
{
public:
	logger();
	~logger();

	string mes_;

	std::ostringstream& Add(logLevel level = logINFO);
	static string format();
	static string getTime();
	static string getLevel(logLevel level);

	void output2Console();
	void output2File();
	logger(const logger&);

private:
	ostringstream os;
	

};
#endif