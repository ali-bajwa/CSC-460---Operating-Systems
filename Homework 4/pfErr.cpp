/*============================================================
DESCRIPTION:		This file defines pfErr(). It
                     prints an error message, beginning
					 with proper printing format.
============================================================*/
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int pfErr(string reason="")
{
	/*
	Input:	Optional string.
			 -  Describes reason for error.
			 -  'reason' should begin with "\n ".
	Output:	Prints to standard output.
			Returns 1, so the function may be returned as an error.
	Name:	'pfErr' stands for 'print formatted Error'.
	*/
	cout << "ERROR. The correct usage of this program is:" << endl
		 << " ./trapezoid  left  right  n  m" << endl
		 << " (where 'left right n m' are four integer parameters)"
		 << endl << endl;
	if (reason != "")
		cout << "Reasons for ERROR:" << reason << endl << endl;
	return 1;
}
