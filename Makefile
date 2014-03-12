# Makefile
progname = analyzer

libdir = ./lib/
libname = libwaveformanalyzer
staticlib = $(libdir)$(libname).a
dynamiclib = $(libdir)$(libname).so

CXX = g++
AR = ar
CXXFLAGS = -m64 -I./include/ -O0 -g -Wall -pedantic -fPIC --std=c++11

includes=$(wildcard ./include/*.hh)
objects=$(patsubst %.cc,%.o,$(wildcard ./src/*.cc))

all: $(progname) lib

lib: $(objects)
	@-mkdir -p $(libdir)
	$(AR) -rs $(staticlib) $^ 
	$(CXX) -o $(dynamiclib) $^ -shared

$(progname): lib $(progname).o
	$(CXX) -o $(progname) $(progname).o $(CXXFLAGS) $(LDFLAGS) $(staticlib)
#$(progname): $(objects) $(progname).o
#	$(CXX) -o $(progname) $^ $(CXXFLAGS) $(LDFLAGS)

%.o: ./src/%.cc %.cc $(includes)
	$(CXX) - $(CXXFLAGS)

clean:
	rm $(objects) $(progname).o
	rm $(progname)
	rm -R ./lib/
