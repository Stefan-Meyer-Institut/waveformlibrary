# Makefile
progname = analyzer

libdir = ./lib/
libname = libwaveformanalyzer
staticlib = $(libdir)$(libname).a
dynamiclib = $(libdir)$(libname).so

CXX = g++
AR = ar
CXXFLAGS = -m64 -I./include/ -O0 -g -Wall -pedantic -fPIC #--std=c++11

# test if ROOT is available
ifdef ROOTSYS
	CXXFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags) -D__HAVEROOT
	LDFLAGS  += $(shell $(ROOTSYS)/bin/root-config --ldflags --libs)
endif

objects=$(patsubst %.cc,%.o,$(wildcard ./src/*.cc))

.PHONY : test

all: $(progname) lib

-include $(objects:.o=.d)
-include $(progname).d

lib: $(objects)
	@-mkdir -p $(libdir)
	$(AR) -rs $(staticlib) $^ 
	$(CXX) -o $(dynamiclib) $^ -shared

$(progname): lib $(progname).o
	$(CXX) -o $(progname) $(progname).o $(CXXFLAGS) $(LDFLAGS) $(staticlib)
#$(progname): $(objects) $(progname).o
#	$(CXX) -o $(progname) $^ $(CXXFLAGS) $(LDFLAGS)


%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cc > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

test: test-bin
	./test/test

test-bin: lib ./test/test.o
	$(CXX) -o ./test/test ./test/test.o $(CXXFLAGS) $(LDFLAGS) $(staticlib)

clean:
	rm $(objects) $(progname).o *.d ./src/*.d
	rm $(progname)
	rm -R ./lib/
	rm -R ./test/test
