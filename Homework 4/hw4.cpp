/*============================================================
COURSE:				CSC460, hw4
PROGRAMMERS:		Ali Bajwa (ab) 			50% Contribution
						hw4.cpp (Computation/Piping code)
						slaveProcess.cpp

						(Also contributed with:
						 Documentation)
						
					Noah Bumgardner (nb)	50% Contribution
						hw4.cpp (CL-parameter checking)
						hw4.cpp (project documentation)
						makefile
						
LAST MODIFIED DATE:	2015-10-28
DESCRIPTION:		PURPOSE:
					This program approximates an area under the curve
				f(x). More technically, this program concurrently
				approximates the integral of f(x) for the interval
				[left, right] using n trapezoids calculated by s
				slaves working concurrently. Once finished, the
				master prints the final result along with the number
				of trapezoids computed by each slave.
					PROCEDURE (For master and slaves):
					This program is the master. It generates s
				non-duplicated slaves. It has one writing pipe to
				each slave. It has one reading pipe shared by all
				slaves. It can send a slave one pair of floats so
				the slave use it and send back the trapezoid's area.
				It can tell the slave to terminate, done once no
				trapezoids are left to be computed.
					Each slave is passed one pair of x-values at
				once, in order to calculate one trapezoid under the
				curve f(x). Slaves work concurrently, pass their
				result to the master via a shared pipe, and listen
				for the master via a personal pipe.
					n is an integer
					INITIAL VALUES:
					f(x): An arbitrary function.
					left: Integer. Bounds the integral along the
				vertical line (x = left) on the left side.
					right: Integer. Bounds the integral along the
				vertical line (x = right) on the right side.
					n: Number of trapezoids used to solve the
				integral. Ignoring round-off error, accuraccy is
				increased as 'n' increases.
					m: Number of slaves used to concurrently solve
				the integral.
					BOUNDS:
					f(x) = x**2 + 2*x + 4
					left < right
					0 < n
					0 < m <= 8
					
NOTE:				The program is implemented using the data type
				'float' to hold trapezoid and summed areas.
					Exceedingly large, absolute values of 'left' and 'right'
				may cause an overflow, and thus erroneous results.
				
				Ignore the warning on compile that the variable size is never used.
				 It is used to calculate the buffer size within conditional statements.
				
KNOWN ERRORS:		When the read on the child fails, the child may never return
					 the result of the trapezoid summation but the parent does not
					 account for this failure and keeps calculating the sum without
					 this result. Since the child process closes itself, this trapezoid
					 is left uncalculated.

FILES:				hw4.cpp, slaveProcess.cpp, pfError.cpp,
					 pfError.h makefile
IDE/COMPILER/INTERPRETER:		GNU g++
INSTRUCTION FOR COMPILATION AND EXECUTION:		(Use makefile)
 1. type:  make									to COMPILE
 2. type:  ./trapezoid  left  right  n  m		to EXECUTE
	(where 'left right n m' are four integer parameters)
============================================================*/
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
#include <vector>
#include "pfErr.h"
using namespace std;

void calcProcesses(int numSlaves, int numTraps, float left, float increment)
{
	int const NUM_SLAVE_ARGS = 5;
	int const MAX_SIZE = 256;
	
	int i = 0;
	pid_t slavePID;
	float answer = 0;
	float area = 0;
	int slaveNum = 0;
	int slavesRemaining = numSlaves;
	int trapsRemaining = numTraps;
	vector<int> TrapsCalculated (numSlaves, 0);

	char buf[MAX_SIZE];
	char msg[MAX_SIZE];
	char pipeName[MAX_SIZE];
	char slaveProgNum[MAX_SIZE];
	char pipeArgvRead[MAX_SIZE];
	char pipeArgvWrite[MAX_SIZE];
	char slaveProgName[MAX_SIZE];

	char *pch;

	int size = 0;
	char *slaveargv[NUM_SLAVE_ARGS];
	
	int pipefdSlaves[8][2]; // Array of slave pipes
	int pipefdMaster[2];    // Slave to Master shared pipe

	// Initialize slaveargs array to NULL
	for(i = 0; i < NUM_SLAVE_ARGS; i++) {
		slaveargv[i] = NULL;
	}

	slaveargv[0] = slaveProgName; // Executable slave prog file
	slaveargv[1] = slaveProgNum;  // Slave number
	slaveargv[2] = pipeArgvRead;  // Slave read pipe info
	slaveargv[3] = pipeArgvWrite; // Slave write pipe info

	strcpy(slaveProgName, "./slaveProcess");
	
	// If Slave to Master pipe creation fails, throw an error and exit
	if (pipe(pipefdMaster) == -1) {
		perror("pipe");
		exit(1);
	}
	
	// Generate s slaves and send initial calculations
	for(i = 0; i < numSlaves; i++)
	{
		strcpy(pipeArgvRead,pipeName);
		
		// If Master to Slave pipe creation fails, throw an error and exit
		if (pipe(pipefdSlaves[i]) == -1) {
			perror("pipe");
			exit(2);
		}
		
		slavePID = fork();

		if (slavePID < 0){
			// Fork error
			printf("Fork failed");
			exit(3);
		}
		else if (slavePID == 0) {
			// Child process
			size = sprintf (slaveProgNum, "%d" ,i);
			size = sprintf (pipeArgvRead, "%d" , pipefdSlaves[i][0]);
			size = sprintf (pipeArgvWrite, "%d", pipefdMaster[1]);

			if (execvp(slaveargv[0], slaveargv) == -1) {
				// Exec command failed, print error and exit
			    printf("Failure to execute because %s\n", strerror(errno));
			    exit(4);
			}
			exit(5);	
		}
		else {
			// Parent process
			trapsRemaining--;
			strcpy(msg, "");
			sprintf (msg, "%f %f", left + (increment * i), left + (increment * (i+1)));
			write(pipefdSlaves[i][1], msg, strlen(msg)+1);
		}
	}

	while(slavesRemaining > 0)
	{
		if(read(pipefdMaster[0], buf, MAX_SIZE) > 0)
		{
			pch = strtok (buf, ","); // Child writes are split by ',', hence tokenize
			
			while (pch != NULL && strlen(pch) > 4)
			{
				sscanf(pch, "%f %d", &answer, &slaveNum);
				area += answer; // Increment answer to total area
				TrapsCalculated[slaveNum] += 1; // Increment traps calculated by slave
				
				if(trapsRemaining > 0) // Trapezoids remaining to calculate
				{
					i++;
					sprintf (msg, "%f %f", left + (increment * i), left + (increment * (i+1)));
					write(pipefdSlaves[slaveNum][1], msg, strlen(msg)+1);
					trapsRemaining--;
				}
				else // No trapezoids left to calculate, terminate
				{
					strcpy(msg, "terminate");
					slavesRemaining--;
					write(pipefdSlaves[slaveNum][1], msg, strlen(msg)+1);
				}
				pch = strtok (NULL, ",");
			}
			strcpy(buf, "");
		}
	}

	wait(NULL);
	close(pipefdMaster[0]); // Close read head of Slave to Master pipe
	close(pipefdMaster[1]); // Close write head of Slave to Master pipe
	
	// Print required output
	printf("The area of the integral is: %f\n", area);
	for (i = 0; i < numSlaves; i++) {
		printf("Slave %d calculated %d trapezoid(s)\n", i+1, TrapsCalculated[i]);
	}
}

int main(int argc, char *argv[])
{
	string errMsg = "";
	int left, right, n, m, s;
	float increment;

	// Check number of parameters.
	if (argc != 5)
		return pfErr();

	// Assume parameters are integers.
	left =	atoi(argv[1]);
	right =	atoi(argv[2]);
	n =		atoi(argv[3]);
	m =		atoi(argv[4]);

	// Check whether parameter values are valid.
	if (right <= left)
		errMsg += "\n 'left' must be less than 'right'";
	if (n <= 0)
		errMsg += "\n 'n' must be positive";
	if (m <= 0 or m > 8)
		errMsg += "\n 'm' must be positive and cannot exceed 8";

	// If any invalid parameters, print reasons and return.
	if (errMsg != "")
	{
		errMsg = "\nInvalid Parameter(s):" + errMsg;
		return pfErr(errMsg);
	}
	
	// Calculate number of slaves(s) as specified
	s = min(n, m);
	
	// Calculate the increment of x per trapezoid
	increment = (right - left)/(float)n;
	
	calcProcesses(s, n, (float)left, increment);
	
	return 0;
}
