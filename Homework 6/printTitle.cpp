/*============================================================
DESCRIPTION:		This file defines printTitle(). It
					 prints the title for the
					 card game WAR.
============================================================*/
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;


void printTitle() {
	// Prints title for card game WAR.
	int const WIDTH = 22;
	cout << setw(WIDTH+2) << setfill('=') << left << "  " << endl;
	cout << setw(WIDTH+4) << "|| The card game: \"WAR\" ||" << endl;
	cout << setw(WIDTH+2) << setfill('=') << left << "  " << endl;
    cout << setfill(' ');
}
