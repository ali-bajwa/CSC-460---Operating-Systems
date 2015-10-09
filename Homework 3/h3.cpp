#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
	int const UPPER_BOUND = 20; // Greatest valid value of n
	int const LOWER_BOUND = 1;  // Smallest valid value for name
	
	// Check user input for valid n
	if ( argc != 2) // argc should be 2 for correct execution
	{
		cout << "ERROR. The correct usage of this program is: ./generate [n] (where 1 <= n <= 20)." << endl;
		
		return 0;
	}
    else if (atoi (argv[1]) < LOWER_BOUND || atoi (argv[1]) > UPPER_BOUND) {
		cout << "ERROR. The number n provided is invalid or out of bounds." << endl;
		cout << "The correct usage of this program is: ./generate [n] (where 1 <= n <= 20)." << endl;
		
		return 0;
	}
	
	// Correct input is given
	int n = atoi (argv[1]);
	
	cout << "Parent process id is: " << getpid() << endl;
	for (int i = 1; i <= n; i++)
	{
		pid_t pid = fork();
		if (pid == 0) { // Child process
			cout << "	This is child " << i << ". My process id is " << getpid() << " and my parent process id is " << getppid() << "." << endl;
			if ((i%2) == 0) { // Odd child
				pid_t pid_c = fork();
				
				if (pid == 0) {
					cout << "		This is a grandchild of child " << i << ". My process id is " << getpid() << " and my parent process id is " << getppid() << "."
				}
				else if (pid > 0) {
					cout << "	Generated child " << i << "successfully. The child process id is: " << pid << endl;
				}
				else {
					cout << "	ERROR: Generating child " << i << " failed. Terminating." << endl;
			return 0;
				}
			}
		}
		else if (pid > 0) { // Parent process
			cout << "Generated child " << i << "successfully. The child process id is: " << pid << endl;
		}
		else { // Fork failed
			cout << "ERROR: Generating child " << i << " failed. Terminating." << endl;
			return 0;
		}
	}
	return 0;
}