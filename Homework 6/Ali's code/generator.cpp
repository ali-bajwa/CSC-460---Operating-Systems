/*============================================================
DESCRIPTION:		This file contains the source code of the
					 slave process executed by the master
					 program to calculate the area of
					 trapezoids given function f(x)
============================================================*/
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <string.h>
#include <iostream>
#include "PCB.h"
using namespace std;

int main(int argc, char* argv[]) {
	int numProcesses, timeLapse, CPUburst, prevArrivalTime = 0, pipeWrite = 0;
	
	numProcesses = atoi(argv[1]);
	pipeWrite = atoi(argv[2]);

	for (int i = 0; i < numProcesses; i++) {
		timeLapse = rand() % 50;
		CPUburst = rand() % 1000 + 1;
		prevArrivalTime += timeLapse;
		
		PCB processControl;
		processControl.CPUburst = CPUburst;
		processControl.arrivalTime = prevArrivalTime;
		
		write(pipeWrite, &processControl, sizeof(processControl));
	}

	return 0;
}
