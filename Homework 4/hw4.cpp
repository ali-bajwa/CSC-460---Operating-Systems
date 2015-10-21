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
using namespace std;
#include "pfErr.h"


int main(int argc, char* argv[])
{
	string errMsg = "";
	int in_left, in_right, in_n, in_m;

	// Check number of parameters.
	if (argc != 5)
		return pfErr();

	// Assume parameters are integers.
	in_left =	atoi(argv[1]);
	in_right =	atoi(argv[2]);
	in_n =		atoi(argv[3]);
	in_m =		atoi(argv[4]);

	// Check whether parameter values are valid.
	if (in_right <= in_left)
		errMsg += "\n 'left' must be less than 'right'";
	if (in_n <= 0)
		errMsg += "\n 'n' must be positive";
	if (in_m <= 0 or in_m > 8)
		errMsg += "\n 'm' must be positive and cannot exceed 8";

	// If any invalid parameters, print reasons and return.
	if (errMsg != "")
	{
		errMsg = "\nInvalid Parameter(s):" + errMsg;
		return pfErr(errMsg);
	}

	// Begin solving the problem.
	////// INSERT CODE HERE

	return 0;
}
