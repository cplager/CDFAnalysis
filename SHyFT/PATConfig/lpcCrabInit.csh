##
##
##

echo "Attempting to setup cms env"  
source /uscmst1/prod/sw/cms/cshrc prod

echo "Now setting up grid env"



echo "Done with proxy init"


source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.csh

grid-proxy-init -debug -verify
voms-proxy-init -voms cms

echo "Attempting to move to a CMSSW release and setup the software"
set targetRelease = "~/releases/CMSSW_3_6_1/"

pushd $targetRelease
cmsenv
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.csh
popd

echo "To submit a job, try the following"
echo "crab -create"
echo "crab -submit"


echo "Done!"
