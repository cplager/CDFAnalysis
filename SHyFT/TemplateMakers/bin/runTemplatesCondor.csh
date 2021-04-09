#!/bin/csh -f

set listFileName = $1
set iJob = $2
set nJobs = $3

set sample = ${listFileName:r}
set outDirName = templatesCondor/$sample/log

if (! -e $outDirName) then
	mkdir -p $outDirName
endif

shyft_pretag shyftPretag_main_condor.py $listFileName $iJob $nJobs \
	>&! $outDirName/shyftPretag_${sample}_${iJob}.log
