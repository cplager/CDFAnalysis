Welcome to CLPBlue.  To use this package, this requires two steps:

1) Build the CLPBlue library

2) Use this library either with standalone executables or inside Root.
   I have extensively tested the former; I don't guarantee that
   everything will work inside Root (although it _should_).

I. Building CLPBlue shared object.

Building the CLPBlue shared object requires Root (>= 4.02; see note at
bottom), but not CDFSoft.  It builds in the same environment as the
Top analysis code
(http://www-cdf.fnal.gov/~cplager/internal/Analysis/Tools/TopAnalysisTools/tools.html).

 Here's what you need to do:

  0) pick a parent directory and go there
  > cd parentDir/
  
  1) Check out CLPBlue.
  > cvs co -d CLPBlue cplager/CLPBlue
  
  2) Setup directory structure (if it doesn't already exist)
  > mkdir shlib; mkdir include
  > cd include; ln -s ../CLPBlue/CLPBlue; cd -
  
  3) Make the library
  > cd CLPBlue; gmake

II. Building a standalone executable

  1) Pick an executable, make a directory, and get the necessary files
  > cd parentDir
  > mkdir testThreeSingleTop
  > cp CLPBlue/examples/testThreeSingleTop.cc testThreeSingleTop/
  > cp CLPBlue/examples/_Makefile testThreeSingleTop/Makefile

  2) Compile executable
  > cd testThreeSingleTop; gmake

  3) Setup your environment.  Here, you will have to either link
     'libCLPBlue.so' in your directory (once) or set LD_LIBRARYPATH
     (every time)
  > cd parentDir/testThreeSingleTop
  > setenv LD_LIBRARY_PATH "$LD_LIBRARY_PATH":"parentdir/shlib"
    -- or --
  > ln -s ../shlib/libCLPBlue.so .  

  4) Run the code
  > cd parentDir/testThreeSingleTop
  > ./testThreeSingleTop.exe

III. Using in Root

  1) Load library
  root> gSystem->load("parentdir/shlib/libCLPBlue.so")

  2) Use it
  root> CLPBlue blue;

Written 061215 by Charles Plager

-------------------------------------------

Root >= 4.02

There is a bug in root 4.00.08 that doesn't allow the program to
generate correlated sets of random numbers.  This does work with Root
4.02.  Unfortunately, most (all) versions of CDFSoft stop at Root
4.00.08.  To setup version 4.02 of root in the CDF framework, you need
run the following command (after having sourced ~cdfsoft2/cdf2.cshrc):

setup root v4_02_00a -q GCC_3_4_3
