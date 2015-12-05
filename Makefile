CXX = u++
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD -O2

MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}			# makefile name

DEPENDS = ${OBJECTS:.o=.d}
#HEADERS = ${shell find . -name "*.h"}					# substitute ".o" with ".h" ???

#############################################################

.PHONY: all clean

.SUFFIXES:
.SUFFIXES: .cc .o
.cc.o:
	${CXX} -c $< ${CXXFLAGS} -o $@

OBJECTS = main.o config.o printer.o bank.o parent.o watcard.o groupoff.o watoff.o namesrv.o vending.o truck.o bottle.o
EXEC = soda

all: ${EXEC}

${EXEC}: ${OBJECTS}
	${CXX} $^ ${CXXFLAGS} -o $@

#############################################################

${OBJECTS}: ${MAKEFILE_NAME}							# OPTIONAL: changes to this file => recompile

-include ${DEPENDS}

clean:						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC}
