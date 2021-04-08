/********************************************************************
* objects/dict_KinematicFitter.h
********************************************************************/
#ifdef __CINT__
#error objects/dict_KinematicFitter.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#include "G__ci.h"
extern "C" {
extern void G__cpp_setup_tagtabledict_KinematicFitter();
extern void G__cpp_setup_inheritancedict_KinematicFitter();
extern void G__cpp_setup_typetabledict_KinematicFitter();
extern void G__cpp_setup_memvardict_KinematicFitter();
extern void G__cpp_setup_globaldict_KinematicFitter();
extern void G__cpp_setup_memfuncdict_KinematicFitter();
extern void G__cpp_setup_funcdict_KinematicFitter();
extern void G__set_cpp_environmentdict_KinematicFitter();
}


#include "TROOT.h"
#include "TMemberInspector.h"
#include "KinematicFitter.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__dict_KinematicFitterLN_string;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEintcOallocatorlEintgRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlETStreamerInfomUcOallocatorlETStreamerInfomUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlETStringcOallocatorlETStringgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlETStringcOallocatorlETStringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_maplEintcOintcOlesslEintgRcOallocatorlEpairlEconstsPintcOintgRsPgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_maplEintcOintcOlesslEintgRcOallocatorlEpairlEconstsPintcOintgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_setlEintcOlesslEintgRcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_setlEintcOlesslEintgRcOallocatorlEintgRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_TMatrixD;
extern G__linked_taginfo G__dict_KinematicFitterLN_TLorentzVector;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFitter;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFittercLcLdA;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFittercLcLInOutType;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFittercLcLConstraintType;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFittercLcLCovarianceMode;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlETLorentzVectorcOallocatorlETLorentzVectorgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlETLorentzVectorcOallocatorlETLorentzVectorgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEvectorlEintcOallocatorlEintgRsPgRcOallocatorlEvectorlEintcOallocatorlEintgRsPgRsPgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEvectorlEintcOallocatorlEintgRsPgRcOallocatorlEvectorlEintcOallocatorlEintgRsPgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlEvectorlEintcOallocatorlEintgRsPgRcOallocatorlEvectorlEintcOallocatorlEintgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlETMatrixDcOallocatorlETMatrixDgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlETMatrixDcOallocatorlETMatrixDgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlETMatrixDmUcOallocatorlETMatrixDmUgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlETMatrixDmUcOallocatorlETMatrixDmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_KinematicFittercLcLConstraintStruct;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEKinematicFittercLcLConstraintStructcOallocatorlEKinematicFittercLcLConstraintStructgRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEKinematicFittercLcLConstraintStructcOallocatorlEKinematicFittercLcLConstraintStructgRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlEKinematicFittercLcLConstraintStructcOallocatorlEKinematicFittercLcLConstraintStructgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEKinematicFittercLcLVirtualParticlecOallocatorlEKinematicFittercLcLVirtualParticlegRsPgR;
extern G__linked_taginfo G__dict_KinematicFitterLN_vectorlEKinematicFittercLcLVirtualParticlecOallocatorlEKinematicFittercLcLVirtualParticlegRsPgRcLcLiterator;
extern G__linked_taginfo G__dict_KinematicFitterLN_reverse_iteratorlEvectorlEKinematicFittercLcLVirtualParticlecOallocatorlEKinematicFittercLcLVirtualParticlegRsPgRcLcLiteratorgR;

/* STUB derived class for protected member access */
