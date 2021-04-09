

echo "Compiling"

cd OptionUtils
gmake

cd ../CLPTemplateMorph
gmake

cd ../CLPTemplateFitter
gmake


cd ../multiRegionFitter
gmake

cd ..

echo "\n\n\nDone compiling... run code in directory: multiRegionFitter/"
