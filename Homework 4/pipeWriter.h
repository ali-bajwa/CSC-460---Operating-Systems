/*============================================================
DESCRIPTION:		This file contains the prototype for the 
					write functionality of the pipes used for
					communication between the master and
					slave processes.
============================================================*/
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

// Send information through the pipe
void sendInfo(string input, char* pipePath);

// Create a new pipe
void makePipe(char* pipePath);

// Unlink a created pipe
void unlinkPipe(char* pipePath);