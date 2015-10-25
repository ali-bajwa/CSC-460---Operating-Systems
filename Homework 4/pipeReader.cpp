/*============================================================
DESCRIPTION:		This file contains code for the read
					functionality of the pipes used for
					communication between the master and
					slave processes.
============================================================*/
#include "pipeReader.h"

string readPipe(char * mypipe)
{
	int const MAX_BUF = 1024;
    int fd;
    char buf[MAX_BUF];

    fd = open(mypipe, O_RDONLY);
    read(fd, buf, MAX_BUF);
    close(fd);

    return string(buf);
}