/*============================================================
DESCRIPTION:		This file contains the source code of the
					 fileOutput function. Writes a formatted
					 table to 'record.txt'.
============================================================*/
#include "fileoutput.h"

void fileOutput(vector<PCB> processes) {
	// Writes a formatted table to 'record.txt'
	int const COL = 16; // Column width
	ofstream myfile;
	myfile.open("record.txt");
	
	myfile << setw(COL) << "Arrival Time";
	myfile << setw(COL) << "Process";
	myfile << setw(COL) << "CPU burst";
	myfile << setw(COL) << "Waiting Time";
	myfile << endl;
	
	for (int i = 0; i < processes.size(); i++) {
		myfile << setw(COL) << processes[i].arrivalTime;
		myfile << setw(COL) << i+1;
		myfile << setw(COL) << processes[i].CPUburst;
		myfile << setw(COL) << processes[i].waitingTime;
		myfile << endl;
	}
}
