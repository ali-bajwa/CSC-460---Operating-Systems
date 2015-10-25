/*============================================================
DESCRIPTION:		This file contains code for the slave
					child processes that are generated
					from the master process
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
using namespace std;

float f(float x) {
	// Function to determine the area of
	return ((pow(x, 2)) + 2*x + 4);
}

float calcArea(float h, float s1, float s2) {
	return 0.5*(h)*(s1 + s2);
}

int main(int argc, char* argv[]) {
	// Run infinitely till terminate call received
	float a;
	float b;
	
	while (1) {
		string input = readPipe(argv[1]])
		if (input = "terminate") {
			break;
		}
		else {
			stringstream ss(input);
			ss >> a;
			ss >> b;
		}
	}
	
	return 0;
}