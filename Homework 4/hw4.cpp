/*============================================================
COURSE:				CSC460, hw4
 (OUT-DATED BELOW)
PROGRAMMERS:		Ali Bajwa (ab) 			__% Contribution
						_
						_
						(Also contributed with:
						 _,
						 _)
						
					Noah Bumgardner (nb)	__% Contribution
						hw4.cpp (CL-parameter checking)
						hw4.cpp (project documentation)
						makefile
						_
						(Also contributed with:
						 _,
						 _)

 (OUT-DATED BELOW, Will be.)
LAST MODIFIED DATE:	2015-10-21
DESCRIPTION:		PURPOSE:
					This program approximates an area under the curve
				f(x). More technically, this program concurrently
				approximates the integral of f(x) for the interval
				[left, right] using n trapezoids calculated by m
				slaves working concurrently. Once finished, the
				master prints the final result along with the number
				of trapezoids computed by each slave.
					PROCEDURE (For master and slaves):
					This program is the master. It generates m
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
					f(x): An arbitrary function. All values are
				positive.
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
					ARBITRARY BOUNDS (To ensure better performance):
 (OUT-DATED BELOW, Will be.)
NOTE:				The program is implemented using the data type
				'float' to hold trapezoid and summed areas.
					Exceedingly large, absolute values of 'left' and 'right'
				may cause an overflow, and thus erroneous results.
 (OUT-DATED BELOW, Will be.)
FILES:				hw4.cpp, makefile
IDE/COMPILER/INTERPRETER:		GNU g++
INSTRUCTION FOR COMPILATION AND EXECUTION:		(Use makefile)
 1. type:  make									to COMPILE
 2. type:  ./trapezoid  left  right  n  m		to EXECUTE
	(where 'left right n m' are four integer parameters)
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <string.h>
#include <error.h>
#include <sstream>
#include "pfErr.h"
#include "pipeReader.h"
#include "pipeWriter.h"
using namespace std;

int main(int argc, char* argv[])
{
	string errMsg = "";
	int left, right, n, m, s;

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
	
	int childPID, trapsAssigned = 0, index = 0;
	float le, increment;
	bool terminate = false, slaveAlive = true;
	string masterToSlave, pipeName;
	vector<float> trap_endpoints, traps_calculated (s, 0), child_PIDs(s, 0);
	vector<string> pipePaths;
	
	// Calculate the increment of x per trapezoid
	increment = (right - left)/(float)n;
	le = (float)left;
	
	// Add interval information to endpoints vector
	for (int i = 0; i <= n; i++ ) {
		trap_endpoints.push_back(le);
		le += increment;
	}
	
	/* DEBUG
	cout << "Left: " << left << endl;
	cout << "Right: " << right << endl;
	cout << "n: " << n << endl;
	cout << "m: " << m << endl;
	cout << "increment: " << increment << endl;
	cout << "S is " << s << endl;
	for (int i = 0 ; i < trap_endpoints.size(); i++ ) {
		cout << trap_endpoints[i] << endl;
	}
	cout << argv[0] << endl;
	DEBUG */
	
	// Initialize char array for slave process args
	int const numChildArgs = 4;
	char *childargv[numChildArgs];
	
	// Initialize childargs array to NULL
	for(int i = 0; i < numChildArgs; i++) {
		childargv[i] = NULL;
	}
	
	// Argv[0] is binary name for slave executable
	childargv[0] = new char [15];	
	strcpy(childargv[0],"./childprocess");
	
	// Argv[1] is pipe name from Master to Slave
	childargv[1] = new char [7];
	
	// Argv[2] is return pipe name from Slave to Master
	childargv[2] = new char [10];
	string masterPipeName = "/masterpipe";
	strcpy(childargv[2], masterPipeName.c_str());
	pipePaths.push_back(masterPipeName);
	
	makePipe(childargv[2]); // Generate global return pipe
	
	// Loop to spawn s processes
	for (int i = 0; i < s; i++) {
		// Generate pipe name and assign to argv[1]
		pipeName = "/pipe" + to_string(i+1);
		strcpy(childargv[1], pipeName.c_str());
		makePipe(childargv[1]); // Make Master to Slave pipe
		pipePaths.push_back(pipeName);
		
		//masterToSlave = to_string(trap_endpoints[i]) + " " + to_string(trap_endpoints[i+1]);
		masterToSlave = "terminate\0";
		string stuff("terminate");
		
		sendInfo(stuff, childargv[1]);
		trapsAssigned += 1;
		
		childPID = fork();
		
		if (childPID == 0) {
			execvp(childargv[0], childargv);
			exit(1);
		}
		else if (childPID > 0) {
			child_PIDs[i] = childPID;
		}
		else {
			// Needs better cleanup - send terminate signals, unlink existing pipes
			
			// Output failure message, terminate program.
			cout << "ERROR:  " << "Slave" << i
			 << " failed to generate."
			 << " Terminating program." << endl;
			exit(1); // EXIT PROGRAM, ERR
		}
	}
	
	/*for (int i = 0; i < pipePaths.size(); i++) {
				cout << pipePaths[i] << endl;
			}*/
	
	/*while(slaveAlive) {
		slaveAlive = false;
		if (slaveAlive == false) {
			for (int i = 0; i < pipePaths.size(); i++) {
				unlinkPipe(pipePaths[i]);
			}
		}
		break;
	}*/
	
	return 0;
}
