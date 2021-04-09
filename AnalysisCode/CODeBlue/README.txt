Welcome to CODeBLUE - C.O.D. evaluating B.L.U.E.

	Included here are my complete files for COD, BLUE, and AIB
(Asymmetric Iterative BLUE).  The included makefile is for compiling
different standalone executables.  Root and '$ROOTSYS' are
required.  I have this code working on both linux and cygwin.  I see
no reason why it shouldn't work on mac os 10 either.

####################################################################

	Makefile - Currently setup for GCC, can change to KAI by
uncommenting a few lines.  Setting up cdfsoft2 for development or
6.x.y will setup root 4.0 made with GCC.  Setting up 5.x.y will setup
root 3.z made with KAI.


#################
## Executables ##
#################

	codeBlue - main executable.  Compares COD to BLUE.  Start here as
an example if you want to use either COD or BLUE.
When you run this, you can tell it to do acceptance or background
errors simply by passing arguments at run time:
unix> ./codeBlue
 - statistical errors only
unix> ./codeBlue acc back
 - stat, acceptance, AND background errors

	calcCorrelations - Calculates statistical correlations between
different analyses.

	findStatErrors - Uses COD to find the statistical errors for
simple (i.e. 1 bin) analysis examples as a function of cross section.
See 'simpleDil.config' for more inforamtion how to set this up.

	runCod - Runs COD by itself

	testAsymBlue - Tests out Asymmetric Iterative Blue (AIB)

	correlationTuple - Makes a small ntuple so correlations can be studied.

	runBlue - Runs BLUE by itself

##################
## Root Scripts ##
##################

	display2d.c - displays a few two dimensional plots from the
output.root file from codeBlue

	fitPulls.c - displays and fits pulls from the output.root file
from codeBlue.

	fitSplitGauss.c - fits a split Gaussian to acceptance variation
and resulting cross section.


##################
## Perl Scripts ##
##################

The below perl scrips honor the '-h' flag for help.  

	makeTGraphErrors.pl - takes output from findStatErrors (cross section,
plus error, and minus error) and writes root scripts to fit the
statistical errors as a function of cross section with a third degree
polynomial.

	findErrors.pl - Takes a formula and scans over cross section space
to calculate errors.  It then tells root to fit this function as a
third degree polynomial.


#########################
## Configuration Files ##
#########################

All of the below files are designed to be human readable.

	*.cod - input files for COD Poisson likelihood fitter

	*.blue - input files for BLUE

    *.aib - input files for Asymmetric Iterative BLUE

-- Charles Plager