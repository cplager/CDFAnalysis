#! /bin/csh -f

set samples = ( \
data_2011A-05Aug2011-v1 \
data_2011A-May10ReReco-v1 \
data_2011A-PromptReco-v4 \
data_2011A-PromptReco-v6 \
data_2011B-PromptReco-v1 \
t_schan \
t_tWchan \
t_tchan \
tbar_schan \
tbar_tWchan \
tbar_tchan \
ttbar \
wjets \
ww \
wz \
zjets \
)

if (! -e templatesCondor) mkdir templatesCondor
if (! -e templatesCondor/condorLogs) mkdir templatesCondor/condorLogs
if (! -e templatesCondor/submit) mkdir templatesCondor/submit

foreach sample ($samples)

	echo
	echo "-------------------------------------------"
	echo Submitting sample $sample

	set nJobs = `wc -l $sample.list | awk '{print $1}'`
	echo    "==>$nJobs to submit"

	#Creating condor submit file
	cat > templatesCondor/submit/$sample.submit <<EOF
universe = vanilla
executable = runTemplatesCondor.csh
notification = Never
log = templatesCondor/templates.logfile
getenv = True
arguments = ${sample}.list \$(Process) $nJobs
output = templatesCondor/condorLogs/condor_${sample}_\$(Process).stdout
error = templatesCondor/condorLogs/condor_${sample}_\$(Process).stderr
queue $nJobs
EOF

	condor_submit templatesCondor/submit/$sample.submit

	echo Job submitted
	echo "-------------------------------------------"


end
