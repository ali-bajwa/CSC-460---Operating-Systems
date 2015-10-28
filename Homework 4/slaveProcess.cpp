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
#include <fcntl.h>
#include <string.h>
using namespace std;

float f(float x) {
	// Function to determine the integral of
	return ((pow(x, 2)) + 2*x + 4);
}

float calcArea(float a, float b) {
	// Calculate area of a trapezoid
	float h, side1, side2;
	side1 = f(a);
	side2 = f(b);
	h = b-a;
	
	return 0.5*(h)*(side1 + side2);
}

int readFromPipe(int fdRead, int fdWrite, int slaveNum)
{
	int const MAX_BUF = 1024;
	float a; 
	float b;
	float area = 0;
	char buf[MAX_BUF];
	int size = 0;
	
	// If there is something to read
	if((size = read(fdRead, buf, MAX_BUF))) 
	{
		// Check termination condition
		if(strcmp(buf, "terminate") == 0) {
			return 0;
		}
		sscanf(buf,"%f %f", &a, &b); // Assign a and b endpoint values
		area = calcArea(a, b);
		strcpy(buf, ""); // Clear buffer
		size = sprintf(buf, "%f %d,", area, slaveNum);
		write(fdWrite, buf, size);
	}
	return 1;
}

int main(int argc, char* argv[]) {
	int slaveNum = 0;
	int slaveRead = 0;
	int slaveWrite = 0;

	slaveNum = atoi(argv[1]);
	slaveRead = atoi(argv[2]);
	slaveWrite = atoi(argv[3]);

	while(1)
	{
		// Run infinitely until terminate call is received
		if(readFromPipe(slaveRead, slaveWrite, slaveNum) == 0)
		{
			// Read function returns 0 on terminate
			close(slaveRead); // Close read pipe head
			break;
		}
	}

	return 0;
}
