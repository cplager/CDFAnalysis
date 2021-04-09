#! /bin/bash  
echo "Make submit and list files for pretag, 0tag and 1 and 2 btag"
echo "You must first have the files list samleSkim.list"

samples=(ttbar wjets zjets t-sChan t-tChan t-tWChan tbar-sChan tbar-tChan tbar-tWChan)
tags=(0Tag 2BTag BTag PreTag)

for i in 0 1 2 3 4 5 6 7 8 
do
### Get common variables
  nskims=`grep "${samples[i]}" templateCount.log | awk '{print $1}'`
  echo ""${samples[i]}" has "$nskims"  skims"
 
        ### Make the Skim Lists
        for j in 0 1 2 3
		do
		name=${samples[i]}${tags[j]}
		original=${samples[i]}
		echo $name
		#echo "cp "$original"Skim.list  "$name"Skim.list"
		cp "$original"Skim.list  "$name"Skim.list

        #set up the files
		rm -rf "$name"Condor.submit
		sw=${samples[i]}
        sed -e "s/30/$nskims/g" templatesCondor.submit > "$sw"Condor00.submit 
				
        #Go through each tag and make the correct Condor.submit
		if [ $j = 0 ] ; then
			#echo "0tag"
			eval "sed -e 's/wjets/"$name"Skim/g' "$sw"Condor00.submit > "$name"Condor0.submit"
			sed -e 's/Btag = -1/Btag = 0/g' "$name"Condor0.submit > "$name"Condor.submit
			rm -rf "$name"Condor0.submit
		fi
		if [ $j = 1 ] ; then
		    #echo "2Btag"
			eval "sed -e 's/wjets/"$name"Skim/g' "$sw"Condor00.submit > "$name"Condor0.submit"
			sed -e 's/Btag = -1/Btag = 2/g' "$name"Condor0.submit > "$name"Condor.submit
			rm -rf "$name"Condor0.submit
		fi     
		if [ $j = 2 ] ; then
			#echo "Btag"
			eval "sed -e 's/wjets/"$name"Skim/g' "$sw"Condor00.submit > "$name"Condor0.submit"
			sed -e 's/Btag = -1/Btag = 1/g' "$name"Condor0.submit > "$name"Condor.submit
			rm -rf "$name"Condor0.submit
		fi
		if [ $j = 3 ] ; then
			#echo "Pretag"
			eval "sed -e 's/wjets/"$name"Skim/g' "$sw"Condor00.submit > "$name"Condor.submit"
		fi 
		rm -rf "$sw"Condor0.submit 
	   	done
		rm -fr "$sw"Condor00.submit
done
