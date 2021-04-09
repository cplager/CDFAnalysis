#! /usr/bin/env python

import ROOT
import optparse, sys, re, os
import math


if __name__ == "__main__":

    # Setup options parser
    parser = optparse.OptionParser(
        usage = "usage: %prog [options] histBase files.list output.root",
        description = "Puts the histograms from different files into "\
        " one file with appropriate names for the fitter.  Files and names"\
        " are specified in files.list."
        )
	
    #Add some options
    parser.add_option("-r","--rebin",action="store",type="string",dest="binFileName",
                      help="bins.txt contains the histogram binning to use for each jet bin.  " +
                      "Format:  1 jet bin per line: [jet bin] [nbins] [min] [max].  "+
                      "E.g. 1j 100 100 500", metavar = "bins.txt")

    parser.add_option("-d","--add-data",action="store",type="string", dest="addData",
                      help="Duplicate template TEMPL to fake data.", metavar = "TEMPL")

    options, args = parser.parse_args()
    if len (args) < 3:
        parser.error("Must supply both histBase, files.list, and output.root")

    histBase      = args[0]
    fileListName  = args[1]
    outputName    = args[2]

    rebinDict = {}
    if (options.binFileName):
        print "\nReading binning information from " + options.binFileName + "..."
        binFile = open(options.binFileName)
        for line in binFile:

            #Clean up white space
            line = line.strip()

            #Skip comments
            if line.startswith("#") :
                continue

            #Skip blank lines
            if len(line) == 0:
                continue
            
            tokens = line.split()
            if len(tokens) != 4:
                print "Format error in file " + options.binFileName + ": " + line
                sys.exit(2)

            jetBin = tokens[0]
            nBins  = int(tokens[1])
            xMin   = float(tokens[2])
            xMax   = float(tokens[3])
            print ("  -->Jet bin " + jetBin + ": nBins = %d, xMin = %f, xMax = %f" % (nBins,xMin,xMax))
            rebinDict[jetBin] = (nBins,xMin,xMax)

    #Open the output file
    outFile = ROOT.TFile.Open (outputName, "RECREATE")

    #Open the list of files and store in a dictionary
    try:
        print "\nAttempting to open "+fileListName
        fileList = open(fileListName)
    except IOError, (errno, strerror):
        print "Cannot open %s: %s (%s)" % (fileListName,strerror,errno)
        sys.exit(2)

    #File format should be name path
    firstFile = True
    histList = []
    for line in fileList:

        #Clean up white space
        line = line.strip()
        
        #Skip comments
        if line.startswith("#") :
            continue

        #Skip blank lines
        if len(line) == 0:
            continue

        tokens = line.split()
        if len(tokens) != 2 and len(tokens) != 4:
            print "Format error on line " + line
            sys.exit(2)

        sampleName = tokens[0]
        inFileName = tokens[1]
        histScale = 1.0

        if len(tokens) == 4:
            # We have to normalize the histograms here
            crossSection = float(tokens[2])  #in nb
            genEvents    = float(tokens[3])
            # For the scaling, convert crossSection into fb (times 1E6)
            histScale    = 1e6*crossSection/genEvents;

        componentList = []
        totalWeight = 0

        if inFileName == "start_combine" :

            print "\nCopying histograms for " + sampleName + " from the following files :" 

            # Get the next line
            line = fileList.next().strip()
            numToken = -1

            while line != "end_combine" :
        
                # Skip comments
                if line.startswith("#") :
                    continue

                # Skip blank lines
                if len(line) == 0:
                    continue

                tokens = line.split()
                if len(tokens) != 2 and len(tokens) != 3:
                    print "Format error on line " + line
                    sys.exit(2)

                if numToken < 0:
                    numToken = len(tokens)
                else:
                    if numToken != len(tokens):
                        print "Format error on line " + line
                        print "Must have the same number of tokens on every line."
                        print "Previous lines had %d" % numToken
                        print "This line has %d" % len(tokens)
                        sys.exit(2)

                componentFile = tokens[0]
                if len(tokens) == 2:
                    histScale     = float(tokens[1])
                    totalWeight += weight
                elif len(tokens) == 3:
                    # We have to normalize the histograms here
                    crossSection = float(tokens[2])  #in nb
                    genEvents    = float(tokens[3])
                    # For the scaling, convert crossSection into fb (times 1E6)
                    histScale    = 1e6*crossSection/genEvents;
                    
                componentList.append((componentFile,histScale))
            
                print "  -->File = %s, Weight = %g" % (componentFile, histScale)

                # Get the next line
                line = fileList.next().strip()


        else:    
            print "\nCopying histograms for " + sampleName + " from " + inFileName+(" using scale factor %f" % histScale)
            componentList.append((inFileName, histScale))
    
        
        if firstFile:  #Find the list of histograms to copy

            matchExp = re.compile('^'+histBase+'_\d+j$')

            fileName = componentList[0][0]

            if not os.path.exists (fileName):            
                print "Can't find '%s'.  Aborting." % fileName
                sys.exit(2)
                
            inFile = ROOT.TFile.Open(fileName)

            list = inFile.GetListOfKeys()

            for key in list:
                match = matchExp.match(key.GetName())
                if match:
                    histList.append(match.group())

            firstFile = False
            inFile.Close()

        outFile.cd()
        for histName in histList:

            # Output histogram:  Will figure this out as I loop over the components
            hist = 0
            nBins = 0
            xMin = 0
            xMax = 0
            binSize = 0

            totalEntries = 0
            
            for component in componentList:
            
                fileName = component[0]
                weight = component[1]
            
                if not os.path.exists (fileName):            
                    print "Can't find '%s'.  Aborting." % fileName
                    sys.exit(2)
                
                inFile = ROOT.TFile.Open(fileName)

                # Input histogram: unmodified binning
                origHist = inFile.Get(histName)
                nBinsOrig = int(origHist.GetNbinsX())
                xMinOrig = origHist.GetXaxis().GetXmin()
                xMaxOrig = origHist.GetXaxis().GetXmax()
                binSizeOrig = (xMaxOrig-xMinOrig)/nBinsOrig
                entries = origHist.GetEntries()
                totalEntries += entries

                # Scale the input histogram by the weight
                if origHist.GetSumw2N() == 0:
                    origHist.Sumw2()   # Needed to track errors on scaled histogram.
                origHist.Scale(weight)

                if (not hist) :  # We need to initialize the output histogram

                    # Output histogram:  Start with initial binning, but may modify
                    hist = origHist.Clone("temp")
                    nBins = nBinsOrig
                    xMin = xMinOrig
                    xMax = xMaxOrig
                    binSize = (xMax-xMin)/nBins

                    # We'll copy the bin contents in (regardless of resizing)
                    hist.Reset()

                    # If we're rebinning, make some changes...
                    if (options.binFileName) :
                     
                        # Get the jet bin
                        jetBin = histName.split("_")[-1]

                        if jetBin not in rebinDict:
                            print ("JetBin = " + jetBin + " not specified in " + options.binFileName +
                                   ".  Exiting...")
                            sys.exit(2)
                    
                        nBins,xMin,xMax = rebinDict[jetBin]
                        binSize = (xMax-xMin)/nBins

                        # Check that the starting bin aligns
                        firstBinOrig = origHist.FindBin(xMin)
                        if xMin != origHist.GetBinLowEdge(firstBinOrig) :
                            print ("Bins don't align!  Requested min = %f.  Available min = %f" %
                                   (xMin,origHist.GetBinLowEdge(firstBinOrig)))
                            print "Original: nBins = %d, xMin = %f, xMax = %f" % (nBinsOrig,xMinOrig,xMaxOrig)
                            print "Rebinned: nBins = %d, xMin = %f, xMax = %f" % (nBins,xMin,xMax)
                            sys.exit(2)

                        # Make sure the original bins divide evenly into the new bins
                        if (binSize % binSizeOrig != 0) :
                            print ("Bins don't align!  Requested bin size = %f.  Original bin size = %f" %
                                   (binSize,binSizOrig))
                            print "Original: nBins = %d, xMin = %f, xMax = %f" % (nBinsOrig,xMinOrig,xMaxOrig)
                            print "Rebinned: nBins = %d, xMin = %f, xMax = %f" % (nBins,xMin,xMax)
                            sys.exit(2)
                
                        # OK, let's go forward with this binning
                        hist.SetBins(nBins,xMin,xMax)


                # Figure out how many bins to group together in the original histogram
                binGroup = binSize / binSizeOrig

                # Handle the first bin (including underflow in original)
                binCont = hist.GetBinContent(1)
                binSumW2 = hist.GetBinError(1)**2
                origStart = 0  #Include the underflow in the original
                firstBinOrig = origHist.FindBin(xMin)
                origEnd = firstBinOrig + int(binGroup)
                for origBin in range(origStart,origEnd):
                    binCont += origHist.GetBinContent(origBin)
                    binSumW2 += (origHist.GetBinError(origBin)**2)
                hist.SetBinContent(1,binCont)
                hist.SetBinError(1,math.sqrt(binSumW2))

                # Do the bulk of the distribution
                for newBin in range(2,nBins):
                    origStart = origEnd
                    origEnd = origStart + int(binGroup)

                    binCont = hist.GetBinContent(newBin)
                    binSumW2 = hist.GetBinError(newBin)**2
                    for origBin in range(origStart,origEnd):
                        binCont += origHist.GetBinContent(origBin)
                        binSumW2 += (origHist.GetBinError(origBin)**2)
                    hist.SetBinContent(newBin,binCont)
                    hist.SetBinError(newBin,math.sqrt(binSumW2))

                # Do any remaining bins past the end of the new range (including overflow in original)
                origStart = origEnd
                origEnd = 2+nBinsOrig #Include the overflow in the original
                binCont = hist.GetBinContent(nBins)
                binSumW2 = hist.GetBinError(nBins)**2
                for origBin in range(origStart,origEnd):
                    binCont += origHist.GetBinContent(origBin)
                    binSumW2 += (origHist.GetBinError(origBin)**2)
                hist.SetBinContent(nBins,binCont)
                hist.SetBinError(nBins,math.sqrt(binSumW2))


            # Done looping over the components for this template
            # Manually correct the entries, which get messed up by all the bin content setting
            hist.SetEntries(totalEntries)

            # If needed, unscale the total of all components (only needed for prenormalized histograms
            if totalWeight > 0 :
                hist.Scale(1./totalWeight)

            # Rename and write out
            newName = "%s_%s" % (sampleName,histName)
            hist.SetName(newName)
            hist.SetDirectory(outFile)
            outFile.cd()
            hist.Write()

            print "  -->" + newName + " written to " + outFile.GetName()

            if options.addData :
                if options.addData == sampleName:
                    dataName = "Data_%s" % histName
                    print "  ---->Duplicating newName as " + dataName
                    dataHist = hist.Clone(dataName)
                    dataHist.SetDirectory(outFile)
                    dataHist.Write()
                

    outFile.Close()
    
        
    

