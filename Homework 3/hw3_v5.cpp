/*============================================================
COURSE:			CSC460, hw3
PROGRAMMER:		Noah Bumgardner (nb)
LAST MODIFIED DATE:	2015-10-02
DESCRIPTION:		This program, if it is the
			 initial parent, creates two
			 duplicate children processes.
			 Uses cout to describe activity.
FILE:			hw3_v3.cpp
IDE/COMPILER/INTERPRETER:		GNU g++
INSTRUCTION FOR COMPILATION AND EXECUTION:
 1. type:  c++11 hw3_v3.cpp		to COMPILE
 2. type:  ./a.out			to EXECUTE
  or
 1. type:  c++11 hw3_v3.cpp -o xxx	to COMPILE
 2. type:  ./xxx			to EXECUTE
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


int main(int argc, char* argv[])
{
	int const NUMS = 1;
	int const CHILDREN_MIN = 1;
	int const CHILDREN_MAX = 20;
	int inputChildMax = -1;
	// Only relevant to the inital parent.
	int children_curr = 0, child_pid = -999;

	// Check number of parameters
	if (argc != 1 + NUMS)
	{
		cout << "In the command line, give exactly "
		 << NUMS << " integer from 1 to 20 (inclusive)."
		 << std::endl;
		exit(1); // EXIT PROGRAM, ERR
	}

	// Check first parameter
	inputChildMax = atoi(argv[1]);
	if (CHILDREN_MIN > inputChildMax or inputChildMax > CHILDREN_MAX)
	{
		cout << "Invalid number. "
		 << "In the command line, give exactly "
		 << NUMS << " integer from 1 to 20 (inclusive)."
		 << std::endl;
		exit(2); // EXIT PROGRAM, ERR
	}

	cout << "PARENT:  " << "This process's pid is " << getpid()
	 << " and it's parent's pid is " << getppid() << endl;

	// Check number of children already made.
	while (children_curr < inputChildMax and children_curr < CHILDREN_MAX) {
		////// Create child
		child_pid = fork();
		children_curr++;
		//
		if (child_pid >= 0)
		{
			// Child successfully generated.
			if (child_pid != 0) // Is initial parent
			{
				// Output success message, describe child.
				cout << "PARENT:  " << "Child" << children_curr
				 << " has been created successfully."
				 << " Child" << children_curr
				 << " has a pid of " << child_pid << endl;
			}
			else // Is child
			{
				cout << "  CHILD" << children_curr << ":  "
				 << "This process has a pid of " << getpid()
				 << " and its parent's pid is " << getppid()
				 << endl;
				// ToDo: Remove '== 0' from test for conciseness ... Is 0 true?
				if (children_curr % 2 == 0)
				{
					cout << "  CHILD" << children_curr
					 << " is about to make a (grand) child." << endl;
					////// INSERT CODE HERE
					
					child_pid = fork();
					if (child_pid >= 0)
					{
						if (child_pid != 0) // Is new parent (Has parent and grandchild)
						{
							// Output success message, describe child.
							cout << "  CHILD" << children_curr << ":  "
							 << "Grandchild" << children_curr
							 << " has been created successfully."
							 << " Grandchild" << children_curr
							 << " has a pid of " << child_pid << endl;
						}
						else // Is grandchild
						{
							// (Grand) child successfully generated.
							cout << "    GRANDCHILD" << children_curr << ":  "
							 << "This process has a pid of " << getpid()
							 << " and its parent's pid is " << getppid()
							 << endl;
						}
					}
					else
					{
						// Output failure message, terminate program.
						cout << "ERROR:  " << "Grandchild" << children_curr
						 << " failed to generate."
						 << " Terminating program." << endl;
						exit(3); // EXIT PROGRAM, ERR
					}
					
				}
				return 0; // EXIT PROGRAM
			}
		}
		else
		{
			// Output failure message, terminate program.
			cout << "ERROR:  " << "Child" << children_curr
			 << " failed to generate."
			 << " Terminating program." << endl;
			exit(4); // EXIT PROGRAM, ERR
		}
	}

	return 0;
}
