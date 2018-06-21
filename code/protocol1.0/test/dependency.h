#pragma once

#ifndef DEPENDENCY_H
#define DEPENDENCY_H
#endif


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

#define ESC_ASCII_VALUE                 0x1b

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

int getchar();
int kbhit(void);