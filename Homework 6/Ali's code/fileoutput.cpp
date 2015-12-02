/*============================================================
DESCRIPTION:		This file contains the source code of the
					 fileOutput function.
============================================================*/
#include "fileoutput.h"

void fileOutput(vector<PCB> processes) {
	ofstream myfile;
	myfile.open("record.txt");
	
	myfile << setw(12) << "Arrival Time";
	myfile << "\t";
	myfile << setw(7) << "Process";
	myfile << "\t";
	myfile << setw(9) << "CPU burst";
	myfile << "\t";
	myfile << setw(12) << "Waiting Time";
	myfile << endl;
	
	for (int i = 0; i < processes.size(); i++) {
		myfile << setw(12) << processes[i].arrivalTime;
		myfile << "\t";
		myfile << setw(7) << i;
		myfile << "\t";
		myfile << setw(9) << processes[i].CPUburst;
		myfile << "\t";
		myfile << setw(12) << processes[i].waitingTime;
		myfile << endl;
	}
}