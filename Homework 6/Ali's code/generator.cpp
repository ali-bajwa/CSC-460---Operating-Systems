/*============================================================
DESCRIPTION:		This file contains the source code of the
					 generator process. Should be created as
					 a non-duplicated child so it can generate
					 PCB processes and send them to the parent
					 process.
============================================================*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "PCB.h"
using namespace std;

int main(int argc, char* argv[]) {
	/*
	Simulates the creation of argv[1] processes,
	 written to argv[2] pipe. Each process is
	 represented with two integer values:
		timeLapse	within	[0, 49]
		CPUburst	within	[1, 1000]
	Each pair of integers is used to make a PCB,
	 which is written to the pipe.
	*/
	int CPUburst, numProcesses, timeLapse,
		pipeWrite = 0, prevArrivalTime = 0;
	
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
