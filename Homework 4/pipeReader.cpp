/*============================================================
DESCRIPTION:		This file contains code for the read
					functionality of the pipes used for
					communication between the master and
					slave processes.
============================================================*/
#include "pipeReader.h"

// Read information through a pipe
string readPipe(char* pipePath)
{
	int const MAX_BUF = 1024;
    int fd;
    char buf[MAX_BUF];
	
    fd = open(pipePath, O_RDONLY);
    read(fd, buf, MAX_BUF);
    close(fd);

    return string(buf);
}