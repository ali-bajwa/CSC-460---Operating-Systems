/*============================================================
DESCRIPTION:		This file contains the prototype for the 
					read functionality of the pipes used for
					communication between the master and
					slave processes.
============================================================*/
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

// Read information from a pipe
string readPipe(char* pipePath);