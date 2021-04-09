# I always do this at the LPC by hand -- you can skip it if it's in
# your .login/.cshrc or you are running at CERN
#source /uscmst1/prod/sw/cms/cshrc prod

set FitterDir=${PWD}
#set myCMSDir=~/work/CMSSW_3_6_1


#Setup CDF cvs
#setenv CVSROOT :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2
#setenv CVS_RSH ssh

cmsenv

setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH":"$FitterDir/shlib




setenv PATH $FitterDir/scripts:${PATH}


