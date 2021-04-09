#!/bin/csh -f

set listFileName = $1
set iJob = $2
set nJobs = $3

set sample = ${listFileName:r}
set outDirName = runEventCondor/$sample/log

if (! -e $outDirName) then
	mkdir -p $outDirName
endif

runEvent runEvent_condor.py $listFileName $iJob $nJobs \
	>&! $outDirName/runEvent_${sample}_${iJob}.log
