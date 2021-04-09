

# Comments like this 
#./multiRegionFitter.exe config/cms_jesShift-XCHECK.config output=output/checkFitter intlumi=0.001 numpes=20  randomseed=1 

./multiRegionFitter.exe config/cms_jesShift-XCHECK.config output=output/MC_357_100611_ intlumi=0.001 numpes=20  randomseed=1 

./plotPulls.py output/MC_357_100611__seed001.root

echo "Done!"
echo "Images are in directory images/"


