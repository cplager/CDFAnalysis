MAIN=$(shell basename `pwd`)

ARCH         := $(shell root-config --arch)

CXX           =
DllSuf        = so

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

# Include Root's architecture file that has options for all of it's
# supported compilers
include ${ROOTSYS}/test/Makefile.arch

SHARED = ../shlib/lib$(MAIN).$(DllSuf)

INCFLAGS = -I.. -I. -I$(MAIN)
CXXFLAGS += $(shell root-config --cflags) $(INCFLAGS)

SRCS =  $(wildcard src/*.cc) 

INCS = 	$(wildcard $(MAIN)/*.hh)

DEPS =  $(patsubst src/%, objects/%, \
		  $(patsubst %.cc, %.d, $(wildcard src/*.cc)) )

# We want dictionaries only for classes that have _linkdef.h files
DICTOBS =  $(patsubst %_linkdef.h, %.o, \
		      $(patsubst dict/%, objects/dict_%, \
			  $(wildcard dict/*_linkdef.h) ) )

OBJS =   $(patsubst src/%, objects/%, \
		    $(patsubst %.cc, %.o, $(SRCS)) )

OBJS += $(DICTOBS)

# We want a shared object library
LDFLAGS += -shared

all:	shared

shared:	$(SHARED)
$(SHARED): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(EXPLLINKLIBS) -o $(SHARED) 


echo:
	@echo For Debugging:
	@echo .
	@echo SRCS: $(SRCS)
	@echo .
	@echo INCLUDES: $(INCS)
	@echo .
	@echo OBJECTS: $(OBJS)
	@echo .
	@echo DEPS: $(DEPS)
	@echo .
	@echo DICTOBS: $(DICTOBS)

objects/dict_%.o: $(MAIN)/%.hh dict/%_linkdef.h
	@echo "Generating dictionary for $< $@"
	$(ROOTSYS)/bin/rootcint -f $(patsubst %.o, %.C, $@) -c -Idict $(INCFLAGS) $(notdir $^) 
	$(CXX) -c $(CXXFLAGS) -o $@ $(patsubst %.o, %.C, $@)

objects/%.o: src/%.cc $(MAIN)/%.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objects/%.d: src/%.cc
	@echo "Generating dependencies for $<"
	@set -e; $(CXX) -M $(CXXFLAGS) $< \
	| sed 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
	[ -s $@ ] || rm -f $@

clean:
	@echo "Cleaning everything..."
	@rm -f objects/*.d objects/*.o core* objects/dict_* $(SHARED)


include $(DEPS)

