#!/bin/csh -f

set listFileName = $1
set iJob = $2
set nJobs = $3
set jesShift = $4
set jerShift = $5

set sample = ${listFileName:r}
set outDirName = mttbarCondor/$sample/log

if (! -e $outDirName) then
	mkdir -p $outDirName
endif

mttbarTree mttbarTree_condor.py $listFileName $iJob $nJobs $jesShift $jerShift \
	>&! $outDirName/ttbarTree_${sample}_jes-${jesShift}_jer-${jerShift}_${iJob}.log
