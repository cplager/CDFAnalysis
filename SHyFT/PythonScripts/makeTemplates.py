#! /usr/bin/env python

import ROOT
import optparse, sys, re, pprint, os

def moveExtraIntoHist (hist):
    """Move over/underflow bins into the histogram"""
    numBins = hist.GetNbinsX()
    hist.SetBinContent(1, hist.GetBinContent(0) + hist.GetBinContent(1))
    hist.SetBinContent(0,0)
    hist.SetBinContent(numBins, hist.GetBinContent(numBins) + hist.GetBinContent(numBins+1))
    hist.SetBinContent(numBins+1, 0)
    
    return hist


# What Mark calls everything
histNames = ['Wqq', 'Wc', 'Wcc', 'Wbb', 'Top', 'sing']
# number of jet and tag bins
numJets = 1
numTags = 1
bottom = 'bottom'
charm  = 'charm'
light  = 'light'
# which samples should I use to get flavor?
bottomSampleName = 'Top'
charmSampleName  = 'Wc'
lightSampleName  = 'Wqq'

flavorMap = {
    'Wqq'  : light,
    'Wc'   : charm,
    'Wcc'  : charm,
    'Wbb'  : bottom,
    'Top'  : bottom,
    'sing' : bottom, 
    }

histoNameList = ['hT_1j', 'hT_2j', 'hT_3j', 'hT_4j', 'hT_5j']

if __name__ == "__main__":
    # Setup options parser
    parser = optparse.OptionParser \
             ("usage: %prog inputTTbar.root inputWjets.root output.root" \
              "Combines inputTTbar and inputWjets into output, unpacking 2d hist" \
              "The order is important")
	

    options, args = parser.parse_args()
    if len (args) < 3:
        print "Need to provide 3 file names. Aborting."
        sys.exit(1)

	
    inputTTbar  = args[0]
    inputWjets  = args[1]
    outputName  = args[2]
	
    if not os.path.exists (inputTTbar):            
        print "Can't find '%s'.  Aborting." % inputTTbar
        sys.exit(1)

	if not os.path.exists (inputWjets):
		print "Can't find '%s'. Aborting." % inputWjets
		sys.exit(1)
		
    ttbarFile = ROOT.TFile.Open (inputTTbar)
    wjetsFile = ROOT.TFile.Open (inputWjets)

    outFile = ROOT.TFile.Open (outputName, "RECREATE")

    histoNameFileMap = {
    'Top'  : ttbarFile,
    'Wjets'   : wjetsFile 
    }

    #testHist = ttbarFile.Get("hTvsNJet")
    #testHist.Draw()

    #oneJetData
    #twoJetData
    #threeJetData
    #fourJetData
    #fiveJetData
    #listOfDataHistos = [oneJetData, twoJetData, threeJetData, fourJetData, fiveJetData]
    #dataHistoMap = zip (histoNameList, listOfDataHistos)

    #print dataHistoMap

    histoName = "hTvsNJet"

    dataHists = dict()

    for (iSampleName, iFile) in histoNameFileMap.items():
        print "Considering sample %s" % iSampleName
        tempHist2D = iFile.Get(histoName)
        print "tempHist2D is - "
        print tempHist2D
        print "done printing"

        tempHist2D.SetName(iSampleName)
        tempHist2D.SetDirectory(outFile)
        tempHist2D.Write()

        for iHistName in histoNameList :
            print "Now looking for hist %s" % iHistName
            newName = "%s_%s" % (iSampleName, iHistName)
            tempHist1D = iFile.Get(iHistName)

            tempHist1D = moveExtraIntoHist ( tempHist1D )
            
            print tempHist1D

            print "Old integral %d " % (tempHist1D.Integral())
            # apply renorm factor
            if (iSampleName == "Top") :
                tempHist1D.Scale(4.518)
            elif (iSampleName == "Wjets") :
                tempHist1D.Scale(0.830)

            print "Histo named %s has new integral %d" % (newName, tempHist1D.Integral())
                
            


            # build data hist
            if iSampleName == "Top" :
                dataName = "Data_%s" % iHistName
                dataHists[iHistName] = tempHist1D.Clone(dataName)
            else :
                dataHists[iHistName].Add(tempHist1D.Clone())
            print "Saving your object as %s" % newName
            tempHist1D.SetName(newName)
            tempHist1D.SetDirectory(outFile)
            tempHist1D.Write()


    print dataHists

    for (iName, iHist) in dataHists.items() :
        print "Writing out %s" % iName
        iHist.SetDirectory(outFile)
        iHist.Write()
    

        

            

































#             # loop over  number of jets
#             # and max pt
#             jetBinMaxHt = {
#                 1 : [10, 50,  300],
#                 2 : [10, 80,  500],
#                 3 : [10, 110, 600],
#                 4 : [10, 140, 600],
#                 5 : [10, 170, 600]
#             }
            
#             for (iJets, iBins) in  jetBinMaxHt.items():
#                 newName = "%s_%s_%dj" % (iSampleName, histoName, iJets)
#                 print "Histogram name will be %s" % (newName)
#                 print "Binning will be "
#                 print iBins
#                 if (len(iBins) < 3):
#                     print "not enough bin info, skipping"
#                     continue
                
#                 oneDimHtHist = ROOT.TH1F (newName, newName, iBins[0], iBins[1], iBins[2])

#                 print "looking up the bin with value (%d, %f) = %d" % (iJets, iBins[1], tempHist2D.FindBin(iJets,iBins[1]))
#                 print "looking up the bin with value (%d, %f) = %d" % (iJets, iBins[2], tempHist2D.FindBin(iJets,iBins[2]))

#                 current1DBin = 1;
                
#                 for iHtBin in range(1, tempHist2D.GetNbinsY()+1):
#                     currentAbsBinNum = tempHist2D.GetBin(iJets, iHtBin)
#                     currentBinContent = tempHist2D.GetBinContent(currentAbsBinNum)
#                     print "Looking at bin number %d with value %d" % (currentAbsBinNum, currentBinContent)
#                     if (currentAbsBinNum >= tempHist2D.FindBin(iJets,iBins[1])) and (currentAbsBinNum <= tempHist2D.FindBin(iJets, iBins[2])) :
#                         print "Bin is within range!"
#                         oneDimHtHist.SetBinContent(current1DBin, currentBinContent)
#                         current1DBin += 1
#                         if current1DBin > 10 :
#                             break

                
                    
                    



                
                
        

#     bottomHist = ROOT.TH1F (bottom, bottom, 20, 0, 5);
#     charmHist  = ROOT.TH1F (charm,  charm,  20, 0, 5);
#     lightHist  = ROOT.TH1F (light,  light,  20, 0, 5);
#     pairMap = {
#         bottom : (bottomSampleName, bottomHist),
#         charm  : (charmSampleName,  charmHist),
#         light  : (lightSampleName,  lightHist)
#         }
#     tag = 1
#     # Since we want 1..numJets inclusive, don't forget the '+ 1'
#     for jet in range (1, numJets + 1):
#         for name, hist in pairMap.values():
#             histName = '%s_%dj_%dt' % (name, jet, tag)
#             tempHist = hfile.Get (histName)
#             if not hist:
#                 print "Zut!"
#                 sys.exit (1)
#             hist.Add (tempHist)
#     outputFile = ROOT.TFile (outputName, 'Recreate')
#     for name, hist in pairMap.values():
#         hist.Scale( 1. / hist.Integral() )
#         hist.Write()
#     for tag in range (1, numTags + 1):
#         for jet in range (1, numJets + 1):
#             if tag > jet: continue
#             dataHistName = 'Data_%dj_%dt' % (jet, tag)
#             dataHist = ROOT.TH1F (dataHistName, dataHistName, 20, 0, 5)
#             for baseHistName in histNames:
#                 histName = '%s_%dj_%dt' % (baseHistName, jet, tag)
#                 hist = hfile.Get (histName)
#                 if not hist:
#                     print "Zut!"
#                     sys.exit (1)
#                 size = hist.Integral()
#                 if overwriteMap.has_key (histName):
#                     size = overwriteMap[ histName ]
#                 del hist
#                 hist = pairMap[ flavorMap[baseHistName] ][1].Clone (histName)
#                 hist.SetTitle (histName)
#                 hist.Scale (size)
#                 hist.Write()
#                 if baseHistName == 'Top':
#                     dataHist.Add (hist, options.topXsec)
#                 else:
#                     dataHist.Add (hist)
#             # for baseHistName
#             dataHist.Write()
#         # for jet
#     # for tag
#     outputFile.Close()
