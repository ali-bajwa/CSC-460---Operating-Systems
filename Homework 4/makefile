#=====================================================
# COURSE:				CSC460, hw4
# PROGRAMMERS:			Ali Bajwa (ab)
#						Noah Bumgardner (nb)
# LAST MODIFIED DATE:	2015-10-21
# DESCRIPTION:			This make file is for hw4.cpp
#					 	 and its dependencies.
# FILE:					makefile
#=====================================================

generate: hw4.cpp pfErr.h pfErr.o pipeReader.h pipeReader.o pipeWriter.h pipeWriter.o
	c++11 hw4.cpp pfErr.o pipeReader.o pipeWriter.o -o trapezoid
	c++11 childProcess.cpp pipeReader.o pipeWriter.o -o childprocess
	c++11 childProcess.c -o childProg
	c++11 hw4.c -o trap
pfErr.o: pfErr.cpp
	c++11 -c pfErr.cpp -o pfErr.o
pipeReader.o: pipeReader.cpp
	c++11 -c pipeReader.cpp -o pipeReader.o
pipeWriter.o: pipeWriter.cpp
	c++11 -c pipeWriter.cpp -o pipeWriter.o