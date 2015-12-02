#=====================================================
# COURSE:				CSC460, hw6
# PROGRAMMERS:			Ali Bajwa (ab)
#						Noah Bumgardner (nb)
# LAST MODIFIED DATE:	2015-12-2
# DESCRIPTION:			This make file is for hw6.cpp
#					 	 and its dependencies.
# FILE:					makefile
#=====================================================

generate: scheduler.cpp scheduler.h fileoutput.h fileoutput.o
	c++11 scheduler.cpp fileoutput.o -o scheduler
	c++11 generator.cpp -o generator
	
pfErr.o: fileoutput.cpp
	c++11 -c fileoutput.cpp -o fileoutput.o
	
clean:
	scheduler generator *.o
