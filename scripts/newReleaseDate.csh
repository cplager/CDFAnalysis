#! /bin/tcsh -f
# $1 is release (e.g. 4.11.2)
setup cdfsoft2 $1
unsetup perl
newrel -t $1 testrel_$1_$2_$3
cd testrel_$1_$2_$3
~cplager/scripts/addpkgDate CalTrigger $2 $3
~cplager/scripts/addpkgDate Consumer $2 $3
~cplager/scripts/addpkgDate ConsumerFramework $2 $3
~cplager/scripts/addpkgDate L1GlobalTrigger $2 $3
~cplager/scripts/addpkgDate PackageList $2 $3
~cplager/scripts/addpkgDate XFTMods $2 $3
~cplager/scripts/addpkgDate XFTSim $2 $3
~cplager/scripts/addpkgDate XTRPSim $2 $3
echo "gmake nobin ; gmake Consumer.xmon ; gmake ConsumerFramework.display"
