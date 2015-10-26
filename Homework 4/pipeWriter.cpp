/*============================================================
DESCRIPTION:		This file contains code for the write
					functionality of the pipes used for
					communication between the master and
					slave processes.
============================================================*/
#include "pipeWriter.h"

// Send information through the pipe
void sendInfo(string input, char* pipePath)
{
    int fd;
    fd = open(pipePath, O_WRONLY);
	cout <<"Input is: " << input << endl;
    write(fd, input.c_str(), sizeof(char)*(input.size()));
    close(fd);
}

// Create a new pipe
void makePipe(char* pipePath) {	
	mkfifo(pipePath, 0666);
}

// Unlink a created pipe
void unlinkPipe(char* pipePath) {
	unlink(pipePath);
}