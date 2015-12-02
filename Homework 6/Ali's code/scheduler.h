/*============================================================
DESCRIPTION:		This header file is used by scheduler.cpp.
					This file defines compareByArrivalTime(),
					 a function to sort based on PCB arrival
					 times.
============================================================*/
#include <algorithm>
#include <errno.h>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include "PCB.h"

bool compareByArrivalTime(const PCB &a, const PCB &b) {
	// Function to sort based on arrival times of two PCBs
	return a.arrivalTime < b.arrivalTime;
}
