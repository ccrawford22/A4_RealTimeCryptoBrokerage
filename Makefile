# CS480 - Spring 2023
# Cody Crawford
# RedID:824167663
# Program 4

# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = log.o cryptoexchange.o

# Program name
PROGRAM = cryptoexc

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
$(PROGRAM) : $(OBJS)
	$(CXX) -pthread -o $(PROGRAM) $^

log.o : cryptoexchange.h log.h log.cpp 
	$(CXX) $(CXXFLAGS) log.cpp 

cryptoexchange.o : log.o consumer.h producer.h shared.h cryptoexchange.h cryptoexchange.cpp
	$(CXX) $(CXXFLAGS) cryptoexchange.cpp

clean :
	rm -f *.o *.exe $(PROGRAM)
