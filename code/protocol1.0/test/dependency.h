/* Written Ha Nguyen
* Based on the original by Ryu Woon Jung (Leon)
*
* All the library and extra needed dependencies, including
* functions for switching between different operating systems
*
* DXL motors used: 1 MX64-AT, 1 MX-28 and 2 AX-12A's
* Tested using USB2Dynamixel and SMPS2Dynamixel with a 12V/5A Powersource
*/

#pragma once

#ifndef DEPENDENCY_H
#define DEPENDENCY_H
#endif

#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include "dynamixel_sdk.h"  

#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#include <chrono>
#include <thread>
#endif


#define ESC_ASCII_VALUE                 0x1b

int getchar();
int kbhit(void);