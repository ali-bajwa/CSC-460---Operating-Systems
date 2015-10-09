/*============================================================
COURSE:				CSC460, hw3
PROGRAMMERS:		Ali Bajwa (ab) 			50% Contribution
						hw3.cpp (draft - complete version)
						hw3.cpp (final version)
						(Also contributed with:
						 compiling hw3.cpp drafts based on
						 meeting notes, correcting/improving
						 documentation and source code as
						 necessary/discussed)
						
					Noah Bumgardner (nb)	50% Contribution
						hw3.cpp (draft - complete version)
						makefile
						(Also contributed with:
						 encouraged group meetings,
						 printed final turnin copy)
					
LAST MODIFIED DATE:	2015-10-09
DESCRIPTION:		This program, generates n duplicated child
				processes concurrently where n is an integer
				input by the user in the range 1 <= n <= 20.
				Every even child generated creates a duplicate
				grandchild process. The program outputs to the
				iostream as specified in the assignment
				description.
FILES:							hw3.cpp, makefile
IDE/COMPILER/INTERPRETER:		GNU g++
INSTRUCTION FOR COMPILATION AND EXECUTION:		(Use makefile)
 1. type:  make					to COMPILE
 2. type:  ./generate			to EXECUTE
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;


int main(int argc, char* argv[])
{
	int const CHILDREN_MIN = 1;  // Lowest valid value of n
	int const CHILDREN_MAX = 20; // Highest valid value of n
	int inputChildMax = -1;
	// Only relevant to the initial parent.
	int child_pid = -999;
	int grandchild_pid = -999;

	// Check number of parameters
	if (argc != 2)
	{
		cout << "ERROR. The correct usage of this program is:"
		 << " ./generate [n] (where 1 <= n <= 20)" << endl;
		exit(1); // EXIT PROGRAM, ERR
	}

	// Check first parameter
	inputChildMax = atoi(argv[1]);
	// Check if inputChildMax (n) is out of bounds
	if (inputChildMax < CHILDREN_MIN or inputChildMax > CHILDREN_MAX)
	{
		cout << "ERROR: The number n provided is invalid "
		 << "or out of bounds" << endl;
		cout << "The correct usage of this program is:"
		 << " ./generate [n] (where 1 <= n <= 20)" << endl;
		exit(2); // EXIT PROGRAM, ERR
	}

	cout << "PARENT:  " << "This process's pid is " << getpid() << endl;

	// Iterate inputChildMax times to generate duplicate processes
	for (int i = 1; i <= inputChildMax; i++) {
		child_pid = fork(); // Create child
		
		if (child_pid > 0)  // Is initial parent
		{
			// Output success message, describe child.
			cout << "PARENT:  " << "Child" << i
			 << " has been created successfully."
			 << " Child" << i
			 << " has a pid of " << child_pid << endl;
		}
		else if (child_pid == 0) // Is child
		{
			cout << "  CHILD" << i << ":  "
			 << "This process has a pid of " << getpid()
			 << " and its parent's pid is " << getppid()
			 << endl;
			 
			if (i % 2 == 0) // Even child
			{
				cout << "  CHILD" << i
				 << " is about to make a (grand) child." << endl;
				 
				grandchild_pid = fork();
				
				if (grandchild_pid > 0) // Is new parent (Has parent and grandchild)
				{
					// Output success message, describe child.
					cout << "  CHILD" << i << ":  "
					 << "Grandchild" << i
					 << " has been created successfully."
					 << " Grandchild" << i
					 << " has a pid of " << grandchild_pid << endl;
				}
				else if (grandchild_pid == 0) // Is grandchild
				{
					// (Grand) child successfully generated.
					cout << "    GRANDCHILD" << i << ":  "
					 << "This process has a pid of " << getpid()
					 << " and its parent's pid is " << getppid()
					 << endl;
				}
				else 
				{
					// Output failure message, terminate program.
					cout << "ERROR:  " << "Grandchild" << i
					 << " failed to generate."
					 << " Terminating program." << endl;
					exit(3); // EXIT PROGRAM, ERR
				}
			}
			return 0;
		}
		else
		{
			// Output failure message, terminate program.
			cout << "ERROR:  " << "Child" << i
			 << " failed to generate."
			 << " Terminating program." << endl;
			exit(4); // EXIT PROGRAM, ERR
		}
	}

	return 0;
}
