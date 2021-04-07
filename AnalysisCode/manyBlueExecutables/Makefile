WORKDIR=./
MAIN=$(shell selectCCfiles.pl -main $(BASE))
EXECNAME=$(MAIN).exe

LIBDIR=../shlib/
FCNCLIBS=-L$(LIBDIR) -lCLPBlue -lRootFileCollection -lMinuit

ARCH=$(shell root-config --arch)
INCS = -Iinclude -I../include -I$(ROOTSYS)/include

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

# KAI compiler
ifeq ($(ARCH),linuxkcc)
CXX = KCC
CXXFLAGS  = +K1 -g -fpic $(shell root-config --cflags) $(INCS)
LD  = KCC
LDFLAGS = --no_exceptions 
BINTYPE=$(BFARCH)
SOEXT=so
ROOTLIB=$(ROOTSYS)/Linux+2.4/lib
endif

# Linux with gcc 3.4.3
ifeq ($(ARCH),linux)
CXX=g++
CXXFLAGS=-pipe -Wall -W -Woverloaded-virtual -fPIC -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO -DG__OSFDLL $(shell root-config --cflags) $(INCS)
LD=g++
LDFLAGS=
BINTYPE=$(BFARCH)
SOEXT=so
ROOTLIB=$(ROOTSYS)/lib
endif

# MacOS X with gcc 3.3
ifeq ($(ARCH),macosx)
CXX=g++
CXXFLAGS=-pipe -W -Wall -fsigned-char -fno-common -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO -DG__OSFDLL $(shell root-config --cflags) $(INCS)
LD=g++
LDFLAGS=-flat_namespace #-undefined suppress
BINTYPE=MacOSX-gcc_3_3
SOEXT=dylib
ROOTSYS=/sw
ROOTLIB=$(ROOTSYS)/lib/root
endif

INCFLAGS =-Iinclude -I../include
CXXFLAGS += $(shell root-config --cflags) $(INCFLAGS)

# Compile with TSam or not
ifndef NO_CDF_SAM
# With TSam (NO_CDF_SAM is NOT defined)
CXXFLAGS += -D__NO_CDFSOFT__ $(INCS)
SAMLIBS=-L$(DISKCACHE_I_DIR)/lib -ldiskcache_samroot \
	    -L$(SAM_CPP_API_DIR)/lib/so -lsam_cpp_api
else
# Without TSam  (NO_CDF_SAM IS defined)
# NO Tsam
CXXFLAGS += -D__NO_CDFSOFT__ -D__NO_TSAM__ $(INCS)
endif         


ROOTLIBS=$(shell root-config --glibs)


LIBS =  $(FCNCLIBS) $(ROOTLIBS) $(SAMLIBS)
#LIBS =  $(ROOTLIBS)
#LIBS =  $(FCNCLIBS) 


SRCS =  $(shell selectCCfiles.pl -src $(BASE)) 

DEPS =  $(patsubst %.cc, %.d, $(SRCS)) 

OBJS =  $(patsubst %.cc, %.o, $(SRCS))

$(EXECNAME): $(OBJS)
	@echo "making stuff with $(OBJS) : $(SRCS)!"
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXECNAME)  $(EXPLLINKLIBS)


%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cc
	@echo "Generating dependencies for $<"
	@set -e; $(CXX) -M $(CXXFLAGS) $< \
	| sed 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
	[ -s $@ ] || rm -f $@

clean:
	@rm -f *.d *.o core* $(EXECNAME)

tar:
	@echo "Making Tar file..."
	@tar czvf $(MAIN).tgz -X exclude.txt *

echo:
	@echo SRCS : $(SRCS)
	@echo DEPS : $(DEPS)
	@echo OBJS : $(OBJS)

include $(DEPS)
