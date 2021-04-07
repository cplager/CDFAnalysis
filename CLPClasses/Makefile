MAIN=CLPClasses

ARCH         := $(shell root-config --arch)

CXX           =
DllSuf        = so

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

# Cygwin
ifeq ($(ARCH),win32gcc)
# Windows with gcc
DllSuf        = dll
CXX           = g++
CXXFLAGS      = -g -O2 -Wall -Woverloaded-virtual -I/usr/X11R6/include 
LD            = g++
LDFLAGS       = -g -O2 -Wl,--enable-auto-import
SOFLAGS       = -shared -D_DLL -Wl,--export-all-symbols
EXPLLINKLIBS  = $(ROOTLIBS) $(ROOTGLIBS)
endif

# gcc compiler (CDF Gen 6)
ifeq ($(ARCH),linux)
# Linux with egcs, gcc 2.9x, gcc 3.x (>= RedHat 5.2)
CXX           = g++
CXXFLAGS      = -g -O -Wall -fPIC
LD            = g++
LDFLAGS       = -g -O
SOFLAGS       = -shared
endif

# Kai compiler (CDF Gen 5)
ifeq ($(ARCH),linuxkcc)
# Linux with the KAI compiler
CXX           = KCC --one_instantiation_per_object
CXXFLAGS      = -g -O -fPIC +K0
LD            = KCC
LDFLAGS       = -g -O $(shell root-config --cflags)
SOFLAGS       =
endif


ifeq ($(CXX),)
$(error $(ARCH) invalid architecture)
endif


SHARED=../shlib/lib$(MAIN).$(DllSuf)

INCFLAGS =-Iinclude -I../include
CXXFLAGS += $(shell root-config --cflags) $(INCFLAGS)

LDFLAGS = --no_exceptions -shared

SRCS =  $(wildcard src/*.cc) 

INCS = 	$(wildcard include/*.h)

DEPS =  $(patsubst src/%, objects/%, \
		  $(patsubst %.cc, %.d, $(wildcard src/*.cc)) )

# We want dictionaries only for classes that have _linkdef.h files
DICTOBS =  $(patsubst %_linkdef.h, %.o, \
		      $(patsubst dict/%, objects/dict_%, \
			  $(wildcard dict/*_linkdef.h) ) )

OBJS =   $(patsubst src/%, objects/%, \
		    $(patsubst %.cc, %.o, $(SRCS)) )


OBJS += $(DICTOBS)


all:	$(SHARED)

$(SHARED): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(SHARED) $(EXPLLINKLIBS)

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

objects/dict_%.o: include/%.h dict/%_linkdef.h
	@echo "Generating dictionary for $< $@"
	$(ROOTSYS)/bin/rootcint -f $(patsubst %.o, %.C, $@) -c -Idict -Iinclude -I../include $(notdir $^) 
	$(CXX) -c $(CXXFLAGS) -o $@ $(patsubst %.o, %.C, $@)

objects/%.o: src/%.cc include/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objects/%.d: src/%.cc
	@echo "Generating dependencies for $<"
	@set -e; $(CXX) -M $(CXXFLAGS) $< \
	| sed 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
	[ -s $@ ] || rm -f $@

clean:
	@echo "Cleaning everything..."
	@rm -f objects/*.d objects/*.o core* objects/dict_* $(SHARED)

tar:
	@echo "Making Tar file..."
	@cd ..;tar czvf $(MAIN)/$(MAIN).tgz -X CLPClasses/exclude.txt *

scp:
	@scp CLPClasses.tgz fcdflnx4.fnal.gov:backups/


include $(DEPS)
