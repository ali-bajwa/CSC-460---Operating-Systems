#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include "PCB.h"
using namespace std;

bool compareByArrivalTime(const PCB &a, const PCB &b) {
	return a.arrivalTime < b.arrivalTime;
}

int main(int argc, char* argv[]) {
	int const MAX_SIZE = 256;
	int const NUM_GEN_ARGS = 4;
	int size = 0;
	int timer = 0;
	int numProcesses;
	int processesRemaining = 0;
	PCB processControl;
	priority_queue<PCB> readyQueue;
	vector<PCB> arrivalQueue;
	vector<PCB> processes;
	
	char numProcessBuffer[MAX_SIZE];
	char generatorProgName[MAX_SIZE];
	char *generatorargv[NUM_GEN_ARGS];
	char pipeArgvWrite[MAX_SIZE];
	pid_t generatorPID;
	
	int pipefd[2]; // Generator to Scheduler pipe
	
	numProcesses = atoi(argv[1]);
	processesRemaining = numProcesses;
	
	strcpy(numProcessBuffer, to_string(numProcesses).c_str());
	strcpy(generatorProgName, "./generator");
	// Initialize slaveargs array to NULL
	for(int i = 0; i < NUM_GEN_ARGS; i++) {
		generatorargv[i] = NULL;
	}
	
	generatorargv[0] = generatorProgName;
	generatorargv[1] = numProcessBuffer;  // Number of processes to simulate
	generatorargv[2] = pipeArgvWrite; // Generator pipe write info
	generatorargv[3] = NULL;
	
	// If Generator to Scheduler pipe creation fails, throw an error and exit
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(1);
	}
	
	generatorPID = fork();
	if (generatorPID < 0){
		// Fork error
		printf("Fork failed");
		exit(2);
	}
	else if (generatorPID == 0) {
		// Child process
		size = sprintf (pipeArgvWrite, "%d", pipefd[1]);
		if (execvp(generatorargv[0], generatorargv) == -1) {
			// Exec command failed, print error and exit
			printf("Failure to execute because %s\n", strerror(errno));
			exit(3);
		}
		exit(4);
	}
	
	while (readyQueue.size() != 0 || arrivalQueue.size() != 0 || processesRemaining != 0) {
		if (processesRemaining > 0) {
			read(pipefd[0], (void*) &processControl, sizeof(processControl));
			processesRemaining -= 1;
			if (processControl.arrivalTime > timer) {
				arrivalQueue.push_back(processControl);
			}
			else {
				readyQueue.push(processControl);
			}
		}
		if (arrivalQueue.size() != 0) {
			for (int i = 0; i < arrivalQueue.size(); i++) {
				if (arrivalQueue[i].arrivalTime < timer) {
					readyQueue.push(arrivalQueue[i]);
					arrivalQueue.erase(arrivalQueue.begin() + i);
				}
			}
		}
		if (readyQueue.size() != 0) {
			PCB selectProcess = readyQueue.top();
			readyQueue.pop();
			timer += selectProcess.CPUburst;
			selectProcess.waitingTime = timer - selectProcess.arrivalTime;
			processes.push_back(selectProcess);
		}
		timer++;
	}
	
	sort(processes.begin(), processes.end(), compareByArrivalTime);
	
	for(int i = 0; i < processes.size(); i++) {
		cout << processes[i].arrivalTime << " " << i+1 << " " << processes[i].CPUburst << " " << processes[i].waitingTime << endl;
	}
	
	return 0;
}