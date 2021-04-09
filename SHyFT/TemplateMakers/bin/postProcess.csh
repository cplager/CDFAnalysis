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

if (! -e templatesCondor/merged) mkdir templatesCondor/merged

foreach sample ($samples)

	echo
	echo "-------------------------------------------"
	echo Combining histogram files for sample $sample

	set fileList = `ls -1 templatesCondor/$sample/shyftPretag_${sample}_job*.root`
	echo    "==>Found $#fileList files"
	hadd -f templatesCondor/merged/shyftPretag_${sample}.root $fileList

	echo "-------------------------------------------"


end

# Check for data
if (-e templatesCondor/merged/shyftPretag_data_all.root) rm -f templatesCondor/merged/shyftPretag_data_all.root
set dataList = `ls -1 templatesCondor/merged/shyftPretag_data_*.root`
if ($#dataList > 0) then
	echo Merge data files $dataList
	hadd -f templatesCondor/merged/shyftPretag_data_all.root $dataList
endif

# Check for Single Top
if (-e templatesCondor/merged/shyftPretag_t_schan.root \
 && -e templatesCondor/merged/shyftPretag_t_tWchan.root \
 && -e templatesCondor/merged/shyftPretag_t_tchan.root \
 && -e templatesCondor/merged/shyftPretag_tbar_schan.root \
 && -e templatesCondor/merged/shyftPretag_tbar_tWchan.root \
 && -e templatesCondor/merged/shyftPretag_tbar_tchan.root ) then

	echo Combining single top templates
	root -b -n -q combineSingleTopFall11.C >&! templatesCondor/merged/combineSingleTopFall11.log
	echo Done
	echo "------"
	tail -20 templatesCondor/merged/combineSingleTopFall11.log
	echo "------"
else 
	echo "Skipping combining single top samples.  Is that what you want?  Better double check!"
endif

