# Makefile
progname = analyzer

CXX = g++
CXXFLAGS = -m64 -I./include/ -O0 -g -Wall -pedantic

includes=$(wildcard ./include/*.hh)
objects=$(patsubst %.cc,%.o,$(wildcard ./src/*.cc))

$(progname): $(objects) $(progname).o
	$(CXX) -o $(progname) $^ $(CXXFLAGS) $(LDFLAGS)

%.o: ./src/%.cc %.cc ${includes}
	$(CXX) - $(CXXFLAGS)

clean:
	rm $(objects) $(progname).o
	rm $(progname)
