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
    char * mypipe = pipePath;

    fd = open(mypipe, O_WRONLY);
    write(fd, input.c_str(), input.size());
    close(fd);
}

// Create a new pipe
void makePipe(char * mypipe) {	
	mkfifo(mypipe, 0666);
}

// Unlink a created pipe
void unlinkPipe(char * mypipe) {
	unlink(mypipe);
}