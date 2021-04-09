#!/bin/csh -f

set listFileName = $1
set iJob = $2
set nJobs = $3

set sample = ${listFileName:r}
set outDirName = nIntCondor/$sample/log

if (! -e $outDirName) then
	mkdir -p $outDirName
endif

nInt nInt_condor.py $listFileName $iJob $nJobs \
	>&! $outDirName/nInt_${sample}_${iJob}.log
