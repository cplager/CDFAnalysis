#
# Makefile for rootsnns.
#

ARCH         := $(shell root-config --arch)

ObjSuf        = o
SrcSuf        = cc
DllSuf        = dll # so on linux
OutPutOpt     = -o

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


# gcc flags
CXX           = g++
LD            = g++
CXXFLAGS      = -O -Wall -Wno-deprecated

##  # Kai flags
##  CXX           = KCC
##  LD            = KCC
##  CXXFLAGS      = -O

LDFLAGS       = -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) -lMinuit $(SYSLIBS)
LDFLAGS      += $(shell root-config --nonew)

#------------------------------------------------------------------------------


OBJS          = CLPBlue.o CLPHisto.o CLPTuple.o CLPTypeDef.o CLPUtils.o StringUtils.o XsecComb.o CLPAsymBlue.o

# Code Blue
OBJS_CB          = ${OBJS} codeBlue.o
PROGRAM_CB       = codeBlue.exe

# Calculate Correlations
OBJS_CC          = ${OBJS} calcCorrelations.o
PROGRAM_CC       = calcCorrelations.exe

# Find statistical errors
OBJS_FSE          = ${OBJS} findStatErrors.o
PROGRAM_FSE       = findStatErrors.exe

# run COD by itself
OBJS_RC          = ${OBJS} runCod.o
PROGRAM_RC       = runCod.exe

# test Asymmetric BLUE
OBJS_AB          = ${OBJS} testAsymBlue.o
PROGRAM_AB       = testAsymBlue.exe

# run BLUE
OBJS_RB          = ${OBJS} runBlue.o
PROGRAM_RB       = runBlue.exe

# Correlation Tuple
OBJS_CT          = ${OBJS} correlationTuple.o
PROGRAM_CT       = correlationTuple.exe


SHLIB         = codeBlue.${DllSuf}

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:  $(PROGRAM_CB)

corr: $(PROGRAM_CC)

fse:  $(PROGRAM_FSE)

rc:   $(PROGRAM_RC)

ab:   $(PROGRAM_AB)

rb:   $(PROGRAM_RB)

ct:   $(PROGRAM_CT)

lib: $(OBJS)
		$(LD) $(SOFLAGS) $(OBJS) $(LIBS) $(OutPutOpt) $(SHLIB)
		@echo "$@ done"

$(PROGRAM_CB): $(OBJS_CB)
		$(LD) $(LDFLAGS) $(OBJS_CB) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_CC): $(OBJS_CC)
		$(LD) $(LDFLAGS) $(OBJS_CC) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_FSE): $(OBJS_FSE)
		$(LD) $(LDFLAGS) $(OBJS_FSE) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_RC): $(OBJS_RC)
		$(LD) $(LDFLAGS) $(OBJS_RC) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_AB): $(OBJS_AB)
		$(LD) $(LDFLAGS) $(OBJS_AB) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_RB): $(OBJS_RB)
		$(LD) $(LDFLAGS) $(OBJS_RB) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_CT): $(OBJS_CT)
		$(LD) $(LDFLAGS) $(OBJS_CT) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

clean:
		@rm -f $(OBJS) core *~ *.d *.o *.exe

distclean: clean
		@rm -f $(PROGRAM_CB) *_dict.* *.def *.exp \
		   *.root *.ps *.so .def *.o *.d so_locations
		@rm -rf cxx_repository


# tells how to compile .cc files into .o files
.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
