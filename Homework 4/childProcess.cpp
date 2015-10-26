/*============================================================
DESCRIPTION:		This file contains code for the slave
					child processes that are generated
					from the master process
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sstream>
#include "pipeReader.h"
#include "pipeWriter.h"
using namespace std;

float f(float x) {
	// Function to determine the area of
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

int main(int argc, char* argv[]) {
	// Run infinitely till terminate call received
	float a, b, area;
	string reply;
	
	while (1) {
		string input = readPipe(argv[1]);
		
		if (input == "terminate") {
			break;
		}
		else {
			stringstream ss(input);
			ss >> a;
			ss >> b;
			
			area = calcArea(a, b);
			reply = string(argv[1]) + " " + to_string(area);
			
			sendInfo(reply, argv[2]);
		}
	}
	
	return 0;
}