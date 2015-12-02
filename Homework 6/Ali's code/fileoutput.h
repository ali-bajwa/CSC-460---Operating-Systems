/*============================================================
DESCRIPTION:		This file contains the definition of the
					 fileOutput function. Writes a formatted
					 table to 'record.txt'.
============================================================*/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "PCB.h"
using namespace std;

// Writes a formatted table to 'record.txt'
void fileOutput(vector<PCB> processes);
