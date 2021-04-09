* Fit PE distributions
	loadfits.c
	fitPEdists.c

* Extract information from the fits
	bAndMax.pl

* Find bBest for each b0 by rebinning histograms as b0 for a given bMeas
	loadbBestfromPE.c
	fitbBestFromPE.c
	stripbBestFromPE.pl
	fitBmaxFromPE.c

* Calculate FC bands
	loadlikeli.c
	likeliRat.c
	first.fc, stat_only.fc, sys.fc

# Get FC info
	getFCinfo.pl

* Draw FC bands
	(draw the fitted results)
	loadfcbands.c
	fcbands.c
	(draw the raw results)
	loadfcbands2.c
	fcbands2.c

-----------------

* Look at peak behavior as function of b0 looking at how the peaks move
	fitpN.c


* Find bMax as a function of b0
	fitBmax.c


* Find bBest and p (b | bBest) as functions of bMeas
	findBbest.c

* Fit FC bands
	fit68_95.c


------------------------------------------------------------

Detailed instructions

* Modify fitPEdists.c so it looks at your distributions

* run it
unix> rlq loadfits.c > pefits.out

* Modify fitbBestFromPE.c so it looks at your distributions

* run it
unix> rlq loadbBestfromPE.c > bbestfrompe.out

* make a file that has 3 columns: bmeasured, bmax, P(bMeas | bBest)
unix> bAndMax.pl -meas_gen_const  bbestfromPE.out > meas_gen_const.out

* make a new .fc file.  Delete all lines after '# start Gaussian parameters'
unix> cp sys.fc new.fc

* Get fits for: 
	- bBest as a function of bMeasured ('best_measured')
	- P(bMeasured | bBest) as a function of bMeasured ('Pmeas_best')
	- bMeasured as a funciton of bBest ('peakLR')

unix> fitToPolyN.pl -s 0 1 -p 5 -ns  -t "bBest vs bMeasured" meas_gen_const.out
unix> fitToPolyN.pl -s 1 0 -p 5 -ns  -t "bMeasured vs bBest" meas_gen_const.out
unix> fitToPolyN.pl -s 0 2 -p 5 -ns  -t "P-bMeas-bBest vs bMeasured" meas_gen_const.out


* Verify the fits are reaonsable, then enter them fits in the
appropriate places in new.fc.
	- You may need to comment out ('#') unreasonable points in
'meas_gen_const.out'

* Modify loadlikeli.c to use 'new.fc' and run it
unix> rlq loadlikeli.c > likeli.out

* Modify (change '$prefix' if desired) and run getFCinfo.pl
unix> getFCinfo.pl likeli.out

* Clean up fc{68,90,95}.txt.  Change into fc.out format
unix> makeReadyForFCbands.pl fc95.txt > fc95.out
... etc

# make FC plot
unix>  rlq loadfcbands2.c

NOTE: 'rlq' is an alias to 'root -l -b -q'