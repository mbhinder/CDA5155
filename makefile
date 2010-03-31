CC=g++
CFLAGS=-c -g -Wall
LDFLAGS= -g
SOURCES=dis.cc sim.cc main.cc 
OBJECTS=$(SOURCES:.cc=.o)
	EXECUTABLE=MIPSsim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	        $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	        $(CC) $(CFLAGS) $< -o $@

clean:
	        rm -rf *.o MIPSsim
