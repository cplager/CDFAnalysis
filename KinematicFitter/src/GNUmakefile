# SoftRelTools GNUmakefile for KinematicFitter source directory 

# Build a library an executable.
LIB = libKinematicFitter.a

# Compile all the .cc files in this directory except
# KinematicFitterExample.cc into the library.
LIBCCFILES = $(filter-out $(skip_files), $(wildcard *.cc))

################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_root.mk
include SoftRelTools/arch_spec_STL.mk
