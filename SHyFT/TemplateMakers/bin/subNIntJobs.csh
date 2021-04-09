#! /bin/csh -f

set samples = ( \
#wjets_inclusive \
#wjets \
ttbar_inclusive \
)

if (! -e nIntCondor) mkdir nIntCondor
if (! -e nIntCondor/condorLogs) mkdir nIntCondor/condorLogs
if (! -e nIntCondor/submit) mkdir nIntCondor/submit

foreach sample ($samples)

	echo
	echo "-------------------------------------------"
	echo Submitting sample $sample

	set nJobs = `wc -l $sample.list | awk '{print $1}'`
	echo    "==>$nJobs to submit"

	#Creating condor submit file
	cat > nIntCondor/submit/$sample.submit <<EOF
universe = vanilla
executable = runNIntCondor.csh
notification = Never
log = nIntCondor/nInt.logfile
getenv = True
arguments = ${sample}.list \$(Process) $nJobs
output = nIntCondor/condorLogs/condor_${sample}_\$(Process).stdout
error = nIntCondor/condorLogs/condor_${sample}_\$(Process).stderr
queue $nJobs
EOF

	condor_submit nIntCondor/submit/$sample.submit

	echo Job submitted
	echo "-------------------------------------------"


end
