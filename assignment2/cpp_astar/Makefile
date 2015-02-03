CC=g++
CFLAGS=-std=c++11 -g -Wall -O2
LDFLAGS=
SOURCES=main.cpp DomainT.cpp NodeT.cpp astar.cpp rbfs.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	  $(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	  $(CC) $(CFLAGS) $< -c
clean:
	$(RM) count *.o *~ 
