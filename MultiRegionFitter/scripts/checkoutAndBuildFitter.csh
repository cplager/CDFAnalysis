# Usage: source checkoutAndBuildFitter.csh

echo Edit this file to remove these three lines as well as make sure the
echo following three variables below are correct.
exit

# You need to have a CMSSW Project setup somewhere (for Root) and you
# need to decide where the fitter will end up.  Please set these two
# variables appropriately.

set myCMSDir=~/work/cmssw/CMSSW_3_6_1
set FitterDir=~/work/standalone/SHyFTFitter
set latestTag=cplager100917

# I always do this at the LPC by hand -- you can skip it if it's in
# your .login/.cshrc or you are running at CERN
source /uscmst1/prod/sw/cms/cshrc prod

# Setup CMSSW to get Root
cd $myCMSDir
cmsenv

#Make the working directory
mkdir -p $FitterDir

# Setup CDF cvs
setenv CVS_RSH ssh
setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
# If you have write access to CDF CVS, you can uncomment the next line
# setenv CVSROOT cdfcvs@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2


cd $FitterDir

# Check out CDF code
cvs co -r $latestTag -d scripts           Jhu/Top/scripts
cvs co -r $latestTag -d CLPTemplateMorph  Jhu/Top/CLPTemplateMorph
cvs co -r $latestTag -d CLPTemplateFitter Jhu/Top/CLPTemplateFitter
cvs co -r $latestTag -d OptionUtils       Jhu/Top/OptionUtils
cvs co -r $latestTag -d multiRegionFitter Jhu/Top/multiRegionFitter

# make directory for shlibs
mkdir shlib


# make a file called scripts/setup.env to setup environment
echo setenv LD_LIBRARY_PATH $FitterDir/shlib:\$LD_LIBRARY_PATH > scripts/setup.env
echo setenv PATH $FitterDir/scripts:\$PATH >> scripts/setup.env
echo rehash >> scripts/setup.env

# you want to source this script everytime you log in
source scripts/setup.env

# Compile
cd CLPTemplateMorph/ ; gmake -j8
cd ../CLPTemplateFitter/ ; gmake -j8 
cd ../OptionUtils/ ; gmake -j8
cd ../multiRegionFitter; gmake -j8 

