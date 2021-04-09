#! /bin/tcsh -f
# $1 is release (e.g. 4.11.2)
setup cdfsoft2 $1
unsetup perl
newrel -t $1 testrel_$1
cd testrel_$1
addpkg -h CalTrigger
addpkg -h Consumer 
addpkg -h ConsumerFramework 
addpkg -h L1GlobalTrigger
addpkg -h PackageList
addpkg -h XFTMods 
addpkg -h XFTSim
addpkg -h XTRPSim
echo "gmake nobin ; gmake Consumer.xmon ; gmake ConsumerFramework.display"
