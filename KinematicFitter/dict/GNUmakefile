# SoftRelTools GNUmakefile for KinematicFitter dict
################################################################

ifdef LINK_SHARED_ROOT
  SHAREDLIB = libKinematicFitter_forRoot.so
  LIBCXXFILES = $(wildcard $(sharedlib_o_dir)*_dict.cxx)
else
ifdef LINK_SHARED
        LINK_SHARED_MODULES = yes
        SHAREDLIB       = libKinematicFitter.so
else
        sharedlib_o_dir = $(staticlib_o_dir)
        LIB             = libKinematicFitter.a
        ifdef USESHLIBS
                override NO_GENERATE_DEPENDS = yes
        endif
endif
LIBCXXFILES = $(filter-out $(skip_files), $(wildcard $(sharedlib_o_dir)*_dict.cxx))
endif

################################################################
include SoftRelTools/standard.mk
include SoftRelTools/arch_spec_root_minedm.mk
include SoftRelTools/arch_spec_rootcint.mk
