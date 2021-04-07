# Usage: source checkoutAndBuildFitter.csh

echo Edit this file to remove these three lines as well as make sure the
echo following three variables below are correct.
exit

# You need to have a CMSSW Project setup somewhere (for Root) and you
# need to decide where the fitter will end up.  Please set these two
# variables appropriately.

set BLUEDir=~/work/standalone/blue2
set latestTag=cplager101214c

## - Comment out following code if you are not running with CMSSW but
## - you already have ROOT setup

set myCMSDir=~/work/cmssw/CMSSW_3_9_1


# I always do this at the LPC by hand -- you can skip it if it's in
# your .login/.cshrc or you are running at CERN
source /uscmst1/prod/sw/cms/cshrc prod

# Setup CMSSW to get Root
cd $myCMSDir
cmsenv

## - Stop commenting for CMSSW

#Make the working directory
mkdir -p $BLUEDir

# Setup CDF cvs
setenv CVS_RSH ssh
setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
# If you have write access to CDF CVS, you can uncomment the next line
# setenv CVSROOT cdfcvs@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2


cd $BLUEDir

# Check out CDF code
cvs co -r $latestTag -d CLPBlue           cplager/CLPBlue

# make directory for shlibs
mkdir shlib


# make a file called scripts/setup.env to setup environment
echo setenv LD_LIBRARY_PATH $BLUEDir/shlib:\$LD_LIBRARY_PATH > setup.env
echo setenv PATH $BLUEDir/bin:\$PATH >> setup.env
echo rehash >> setup.env

# you want to source this script everytime you log in
source setup.env

cd CLPBlue
make
cd examples
make BASE=calcAIB.cc


