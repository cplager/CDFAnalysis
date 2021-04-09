#! /bin/csh -f

set samples = ( \
data_2011A-05Aug2011-v1 \
data_2011A-May10ReReco-v1 \
data_2011A-PromptReco-v4 \
data_2011A-PromptReco-v6 \
data_2011B-PromptReco-v1 \
)

if (! -e runEventCondor) mkdir runEventCondor
if (! -e runEventCondor/condorLogs) mkdir runEventCondor/condorLogs
if (! -e runEventCondor/submit) mkdir runEventCondor/submit

foreach sample ($samples)

	echo
	echo "-------------------------------------------"
	echo Submitting sample $sample

	set nJobs = `wc -l $sample.list | awk '{print $1}'`
	echo    "==>$nJobs to submit"

	#Creating condor submit file
	cat > runEventCondor/submit/$sample.submit <<EOF
universe = vanilla
executable = runRunEventCondor.csh
notification = Never
log = runEventCondor/runEvent.logfile
getenv = True
arguments = ${sample}.list \$(Process) $nJobs
output = runEventCondor/condorLogs/condor_${sample}_\$(Process).stdout
error = runEventCondor/condorLogs/condor_${sample}_\$(Process).stderr
queue $nJobs
EOF

	condor_submit runEventCondor/submit/$sample.submit

	echo Job submitted
	echo "-------------------------------------------"


end
