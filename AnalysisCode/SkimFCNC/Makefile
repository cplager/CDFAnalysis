WORKDIR=./
EXECNAME=$(shell basename `pwd`).exe

ARCH=$(shell root-config --arch)
INCS = -Iinclude -I../include -I$(ROOTSYS)/include

# Cygwin
ifeq ($(ARCH),win32gcc)
# Windows with gcc
DllSuf        = dll
CXX           = g++
CFLAGS        = -O2 -Wall -Woverloaded-virtual -I/usr/X11R6/include 
LD            = g++
LDFLAGS       = -O2 -Wl,--enable-auto-import
SOFLAGS       = -shared -D_DLL -Wl,--export-all-symbols
EXPLLINKLIBS  = $(ROOTLIBS) $(ROOTGLIBS)
endif

# KAI compiler
ifeq ($(ARCH),linuxkcc)
CXX = KCC
CFLAGS  = +K1 -g -fpic $(shell root-config --cflags) $(INCS)
LD  = KCC
LDFLAGS = --no_exceptions 
BINTYPE=$(BFARCH)
SOEXT=so
ROOTLIB=$(ROOTSYS)/Linux+2.4/lib
endif

# Linux with gcc 3.4.3
ifeq ($(ARCH),linux)
CXX=g++
CFLAGS=-pipe -Wall -W -Woverloaded-virtual -fPIC -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO -DG__OSFDLL $(shell root-config --cflags) $(INCS)
LD=g++
LDFLAGS=
BINTYPE=$(BFARCH)
SOEXT=so
ROOTLIB=$(ROOTSYS)/lib
endif

# MacOS X with gcc 3.3
ifeq ($(ARCH),macosx)
CXX=g++
CFLAGS=-pipe -W -Wall -fsigned-char -fno-common -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO -DG__OSFDLL $(shell root-config --cflags) $(INCS)
LD=g++
LDFLAGS=-flat_namespace #-undefined suppress
BINTYPE=MacOSX-gcc_3_3
SOEXT=dylib
ROOTSYS=/sw
ROOTLIB=$(ROOTSYS)/lib/root
endif

LIBDIR=../shlib/

ROOTLIBS=$(shell root-config --glibs)
FCNCLIBS=-L$(LIBDIR) -lJetUser -lCLPClasses

LIBS =  $(FCNCLIBS) $(ROOTLIBS) 
#LIBS =  $(ROOTLIBS)
#LIBS =  $(FCNCLIBS) 

CFLAGS += -D__NO_CDFSOFT__ $(INCS)


SRCS =  $(wildcard *.cc) 

DEPS =  $(patsubst %.cc, %.d, $(wildcard *.cc)) 

OBJS =  $(patsubst %.cc, %.o, $(wildcard *.cc))

$(EXECNAME): $(OBJS)
	@echo "making stuff with $(OBJS) : $(SRCS)!"
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXECNAME)  $(EXPLLINKLIBS)


%.o: %.cc
	$(CXX) $(CFLAGS) $(INCS) -c -o $@ $<

%.d: %.cc
	@echo "Generating dependencies for $<"
	@set -e; $(CXX) -M $(CFLAGS) $< \
	| sed 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
	[ -s $@ ] || rm -f $@

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
	@echo CFLAGS: $(CFLAGS)

clean:
	@rm -f *.d *.o core* $(EXECNAME)

include $(DEPS)
