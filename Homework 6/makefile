#=====================================================
# COURSE:				CSC460, hw4
# PROGRAMMERS:			Ali Bajwa (ab)
#						Noah Bumgardner (nb)
# LAST MODIFIED DATE:	2015-10-28
# DESCRIPTION:			This make file is for hw4.cpp
#					 	 and its dependencies.
# FILE:					makefile
#=====================================================

generate: hw4.cpp pfErr.h pfErr.o
	c++11 hw4.cpp pfErr.o -o trapezoid
	c++11 slaveProcess.cpp -o slaveProcess
	
pfErr.o: pfErr.cpp
	c++11 -c pfErr.cpp -o pfErr.o
	
clean:
	trapezoid slaveProcess *.o
