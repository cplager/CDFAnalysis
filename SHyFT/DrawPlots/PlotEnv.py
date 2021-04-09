

# Import some potential useful things
import sys, ConfigParser, os, string, commands, time
import math

from optparse import OptionParser


# import the root libraries
# with this kind of import
# you won't need to prefix your root objects with ROOT

from ROOT import *




# This is the class that stores all the information
# for the 

class PlotInfo:
	
	# Constructor
	
    def __init__(self, initDict) :
		
        # Parse the dictionary that was handed to you
		# as the only constructor argument
		# Default values not supported yet
		# No checking to see if this is valid yet
		# it will only store information it
		# understands, and will ignore everything else
		
		
		
        for (pname,pval) in initDict.iteritems() :
            if (pname == 'name'):
                self.name = pval

            if (pname == 'file'):                
                self.fileName = pval
                self.rootFile = TFile(self.fileName)

            if (pname == 'xsec'):                
                self.xsec = pval

            if (pname == 'ngen'):
                self.ngen = pval

            if (pname == 'fillColor'):
                self.fillColor = pval

            if (pname == 'fillPattern') :
                self.fillPattern = pval

            if (pname == 'isNormPerJetBin') :
                self.isNormPerJetBin = pval

            if (pname == 'normsPerJetBin') :
                self.normsPerJetBin = pval

            
        
        #done with loop over iter items
        
    #done with init

    
	# print out some information about this PlotInfo object
    def show(self):
        print 'PlotsInfo object named %s, xsec = %s, ngen = %s' % (self.name, self.xsec, self.ngen)
        print "   file = %s" % self.fileName


	# Get a histogram that is properly normalized
	# The PlotInfo will know how to normalize the histo
	# according to the config

    def getHist(self, histName, lumi, jetTreatment) :
        # add a namecycle to histName
        namePlusCycle = "%s;1" % (histName)
        # Get the histogram
        targetHist = self.rootFile.Get(namePlusCycle)
        
		# Is it a null pointer?
		
        if (targetHist == None) :
            print "Oops! Error looking for histo %s, exiting..." % (histName)
            sys.exit()

        targetHist.UseCurrentStyle()


        partialHistName = histName
        if ( histName.endswith('0j')
             or histName.endswith('1j')
             or histName.endswith('2j')
             or histName.endswith('3j')
             or histName.endswith('4j')
             or histName.endswith('5j')
             or histName.endswith('6j')
             or histName.endswith('7j')
             or histName.endswith('8j')) :
            partitionedString = histName.partition('_')     ##----------------
            
            # the partial hist name is the piece before
            # the underscore
            
            partialHistName = partitionedString[0]
            
        # end if 
                
                
                
        

		# Is it data? Nothing special for data
        # except that you should add all jet bins
        # together
        
        if (self.name == 'data'):        
            
            if (jetTreatment == '>=0j') :
                # sum up all _nj histos
                # targetHist.Reset("ICE")
                targetHist = self.sumHistoByJetBin(partialHistName, range(0,9), lumi)
                
            elif (jetTreatment == '>=1j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(1,9), lumi)
                
            elif (jetTreatment == '>=2j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(2,9), lumi)
                
            elif (jetTreatment == '>=3j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(3,9), lumi)
                
            elif (jetTreatment == '>=4j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(4,9), lumi)

            elif (jetTreatment == '>=5j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(5,9), lumi)
                
            elif (jetTreatment == '>=6j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(6,9), lumi)

            elif (jetTreatment == '>=7j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(7,9), lumi)

            elif (jetTreatment == '>=8j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(8,9), lumi)

            elif (jetTreatment == 'nJets'):
                print "Recognized request for njets in data, returning histo"

            elif (jetTreatment == 'nBTag'):
                print "Recognized request for nBTag in data, returning histo"
            
            # end if jetTreament
            targetHist.SetLineColor(1)
            targetHist.SetMarkerColor(1)
            targetHist.SetFillColor(0)
            targetHist.SetLineWidth(2)
            targetHist.SetMarkerStyle(20)

            return targetHist
        else :

			# targetHist.SetLineColor(self.fillColor)

			# normalize by xsec or jet bin
			# qcd is by jet bin,
			# wjets is by xsec
			
            
				
            # how do you know what jet bin you are in?
            # look at the distribution
            # maybe we should make this more explicit                
            
            # Big case statement
            #
            # Basic algorithm:
            #
            #   If the plot is for a specific jet bin
            #   then just use the norm for that jet bin
            #
            #   If the plot is for inclusive, then sum
            #   up all jet bins
            #
            #   If the plot is for >= njets, then
            #   start with njet and sum the rest
            #
            #
            
            
            if (jetTreatment == '0j' ) :
                    
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[0] / targetHist.Integral())
                        
            elif (jetTreatment == '1j') :
                
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[1] / targetHist.Integral())
                    
            elif (jetTreatment == '2j') :
                
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[2] / targetHist.Integral())
                    
            elif (jetTreatment == '3j') :
                
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[3] / targetHist.Integral())
                    
            elif (jetTreatment == '4j') :
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[4] / targetHist.Integral())
                    
            elif (jetTreatment == '5j') :
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[5] / targetHist.Integral())

            elif (jetTreatment == '6j') :
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[6] / targetHist.Integral())

            elif (jetTreatment == '7j') :
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[7] / targetHist.Integral())
                    
            elif (jetTreatment == '8j') :
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else :
                    targetHist.Scale(self.normsPerJetBin[8] / targetHist.Integral())
 #########################################
                    
            elif (jetTreatment == '>=0j') :
                # sum up all _nj histos
                # targetHist.Reset("ICE")
                targetHist = self.sumHistoByJetBin(partialHistName, range(0,9), lumi)

            elif (jetTreatment == '>=1j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(1,9), lumi)
                
            elif (jetTreatment == '>=2j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(2,9), lumi)
                
            elif (jetTreatment == '>=3j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(3,9), lumi)
                
            elif (jetTreatment == '>=4j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(4,9), lumi)
                
            elif (jetTreatment == '>=5j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(5,9), lumi)

            elif (jetTreatment == '>=6j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(6,9), lumi)
                
            elif (jetTreatment == '>=7j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(7,9), lumi)

            elif (jetTreatment == '>=8j') :
                targetHist = self.sumHistoByJetBin(partialHistName, range(8,9), lumi)
                

            elif (jetTreatment == "nJets") :
                print "Regonized njets treatment for MC"
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else:
                    print "...Not doing anything"

            elif (jetTreatment == "nBTag") :
                print "Regonized nBTag treatment for MC"
                if (self.isNormPerJetBin == 0) :
                    targetHist.Scale(self.xsec * lumi / self.ngen)
                else:
                    print "...Not doing anything"                    
                
            # Ok, you've decided how to handle it with jet bins    
            # now adjust colors
            
            targetHist.SetFillColor(self.fillColor)            
            targetHist.SetFillStyle(self.fillPattern)
                
                                                                    
        return targetHist
    # end of getHist

    def sumHistoByJetBin (self, partialHistName, rangeOfJetBins, lumi) :
        print "===Distribution %s, summing from %s" %( partialHistName, rangeOfJetBins)
        foundFirstHisto = false
        for iJets in rangeOfJetBins:
            tempHistName = "%s_%dj;1" % (partialHistName, iJets)
            print "Looking for sub histogram %s" % (tempHistName)
            tempHistiJet = self.rootFile.Get(tempHistName)
            
            if (tempHistiJet == None) :
                print "Oops! Error looking for histo %s, exiting..." % (tempHistName)
                sys.exit()

            if( self.name != "data"): 
                if (self.isNormPerJetBin == 1) :
                    print "sample %s histo %s norm %f" % (self.name, tempHistName, self.normsPerJetBin[iJets])
                    if (tempHistiJet.Integral() != 0):
                        tempHistiJet.Scale(self.normsPerJetBin[iJets]/tempHistiJet.Integral())                        
                    elif (tempHistiJet.Integral() == 0 and self.normsPerJetBin[iJets] == 0):
                        print "WARNING: Zero entry MC histogram for %s to be scaled to 0 norm... this is probably fine" % (self.name)
                    else :
                        print "ERROR: You are trying to scale a zero-entry histogram to a non-zero norm"
                        print "ERROR: Exiting program"
                        exit (10)
                        
                    
                else :
                    tempHistiJet.Scale(self.xsec * lumi / self.ngen)
                    print "sample %s histo %s norm %f" % (self.name, tempHistName, tempHistiJet.Integral())
            # end normalization if not data
            

            if (not foundFirstHisto):
                tempTotalHist = tempHistiJet.Clone("sumJetBin")
                foundFirstHisto = true
                
                print "tempTotalHist is called %s" % (tempTotalHist.GetName())
                
            else:
                tempTotalHist.Add(tempHistiJet)
            
        # end for
        print "Total normalization after summing over jet bins = %f " % (tempTotalHist.Integral())
        return tempTotalHist
    #end sumHistoByJetBin

        

class PlotGroup :
    def __init__ (self, plotList, lumi, plotGroupName, directoryName) :
        self.plotList = plotList
        self.lumi = lumi
        self.groupName = plotGroupName
        self.directoryName = directoryName

        if (not os.path.exists(self.directoryName)) :
            print "Creating directory named %s" % (directoryName)
            os.mkdir(directoryName)

    def show (self):
        print "Plot Group Name = %s, Lumi = %f, Output Dir = %s" % (self.groupName, self.lumi, self.directoryName)



def getJetTitleString ( jetTreatment ):
    returnString = jetTreatment
    if (jetTreatment == ">=0j") :
        returnString = "ge0j"
    elif (jetTreatment == ">=1j") :
        returnString = "ge1j"
    elif (jetTreatment == ">=2j") :
        returnString = "ge2j"
    elif (jetTreatment == ">=3j"):
        returnString = "ge3j"
    elif (jetTreatment == ">=4j"):
        returnString = "ge4j"
    elif (jetTreatment == ">=5j"):
        returnString = "ge5j"
    elif (jetTreatment == ">=6j"):
        returnString = "ge6j"
    elif (jetTreatment == ">=7j"):
        returnString = "ge7j"
    elif (jetTreatment == ">=8j"):
        returnString = "ge8j"

    return returnString
    
    


        
########################################
#
# Function to draw plots
# Does not allow sophisticated rebin
#
########################################

def drawStackPlot(dist, myPlotGroup, plotTitle, plotXLabel, nBins, xMin, xMax, jetTreatment) :

    stackList = myPlotGroup.plotList
    lumi = myPlotGroup.lumi
    directoryName = myPlotGroup.directoryName
    groupName = myPlotGroup.groupName
	
    print "drawing distribution %s " % dist
    myStack = THStack("theStack", plotTitle)

    # Make a legend
    legForStack = TLegend(0.155,0.8,0.92,0.92)
    legForStack.SetFillColor(0)
    legForStack.SetBorderSize(0)
    legForStack.SetNColumns(3)

    myLumiString = "CMS Preliminary 36 pb^{-1} at #sqrt{s} = 7 TeV" 
    myLumiTex = TLatex()
    myLumiTex.SetNDC()
    myLumiTex.SetTextFont(42)
    myLumiTex.SetTextAlign(13)
    myLumiTex.SetTextSize(0.045)


	# loop over the list of plots
	# save the data plot for later
	# put all other plots into the stack
	# and into a legend
    
    for iplot in stackList:	
       # Input histogram: unmodified binning
       origHist = iplot.getHist(dist,lumi, jetTreatment)

       # This rebinning also adds in overflow
       resultHist = rebinHistManual (origHist, nBins, xMin, xMax)
          
       # Save data for later
            
       if (iplot.name == "data"):                 
           myData = resultHist.Clone("data")
           legForStack.AddEntry(myData, iplot.name, "lpe")       
       else :
           iHist = resultHist.Clone("stack")
           legForStack.AddEntry(iHist, iplot.name, "f")
           myStack.Add(iHist, "hist")

	
	# Check for the maximum between the stack and
	# the data, then make sure the stack
	# has the maximum set properly.
	# You could also scale the max here
	
    if (myStack.GetMaximum() > myData.GetMaximum()) :
        plotMax = myStack.GetMaximum()
    else :
        plotMax = myData.GetMaximum()

    myStack.SetMaximum(plotMax*1.3)

    # Set the labels & title

    titleString = "%s;%s;%s" % ("", plotXLabel, "Events")

    myStack.SetTitle(titleString)

	# Create a canvas
	# put the plots on the canvas
	# then save the canvas
	
    myCanvas = TCanvas(dist, dist, 600, 600)
    if (dist == "nJets"):
        myCanvas.cd()
        gPad.SetLogy()
    
    myStack.Draw()
    myData.Draw('pesame')
    legForStack.Draw()
    myLumiTex.DrawLatex(0.25, 0.98, myLumiString)
    
    #  pngName = "%s_afterFit_300.png" % dist
    jetTitleString = getJetTitleString(jetTreatment)
    pngName = "%s/%s_%s_%s.png" % (directoryName, dist, jetTitleString, groupName)
    myCanvas.SaveAs(pngName)
##     pdfName = "%s/%s_%s_%s.pdf" % (directoryName, dist, jetTitleString, groupName)
##     myCanvas.SaveAs(pdfName)



# done with stack plot drawing


def drawNjetsPlot (dist, myPlotGroup, jetTreatment):

    stackList = myPlotGroup.plotList
    lumi = myPlotGroup.lumi
    directoryName = myPlotGroup.directoryName
    groupName = myPlotGroup.groupName

    print "Drawing nJets or nBTags"
    myStack = THStack("theStack", "Inclusive")

    # Make a legend
    legForStack = TLegend(0.155,0.8,0.92,0.92)
    legForStack.SetFillColor(0)
    legForStack.SetBorderSize(0)
    legForStack.SetNColumns(3)

    myLumiString = "CMS Preliminary 36 pb^{-1} at #sqrt{s} = 7 TeV"
    myLumiTex = TLatex()
    myLumiTex.SetNDC()
    myLumiTex.SetTextFont(42)
    myLumiTex.SetTextAlign(13)
    myLumiTex.SetTextSize(0.045)

    
##     if (jetTreatment == ">=1j"):
##         myBins = [2, 1.5, 1.5]
    if ( dist == "nJets" ):
        if (jetTreatment == ">=2j"):        
            myBins = [1, 1.5, 2.5]
            
        elif (jetTreatment == ">=3j"):
            myBins = [2, 1.5, 3.5]
            
        elif (jetTreatment == ">=4j"):
            myBins = [3, 1.5, 4.5]
        
        elif (jetTreatment == ">=5j"):
            myBins = [4, 1.5, 5.5]
            
        elif (jetTreatment == ">=6j"):   ##-----------------
            myBins = [5, 1.5, 6.5]
            
            nJetsQcd = TH1F ("nJetsQcd", "Number of Jets", 7, 1.5, 8.5)
    else:
        myBins = [6, -0.5, 5.5]
        
    for iplot in stackList:
        # now you have a normalized xsec*lumi njets plot
        # for everything except qcd

        
        if (iplot.isNormPerJetBin == 1):
            for iJet in range(2,9):   ##----------------
                normThisBin = iplot.normsPerJetBin[iJet]
                nJetsQcd.SetBinContent(iJet-1, normThisBin)  ##----------------
            # end for each jet bin
            nJetsQcd.SetFillColor(iplot.fillColor)
            nJetsQcd.SetFillStyle(iplot.fillPattern)
            resultHist = rebinHistManual(nJetsQcd, myBins[0], myBins[1], myBins[2])
            legForStack.AddEntry(resultHist, iplot.name, "f")
            myStack.Add(resultHist, "hist")
        else: 
            if ( dist == "nJets" ):
                origHist = iplot.getHist(dist, lumi, dist)   ##----------------
            else :
                origHist = iplot.getHist(dist, lumi, "nBTag")   ##----------------
            resultHist = rebinHistManual(origHist, myBins[0], myBins[1], myBins[2])
            if (iplot.name == "data"):
                myData = resultHist.Clone("data")
                legForStack.AddEntry(myData, iplot.name, "lpe")
                print "sample %s events %f" % (iplot.name, resultHist.Integral())
            else :
                legForStack.AddEntry(resultHist, iplot.name, "f")
                myStack.Add(resultHist, "hist")
                print "sample %s events %f" % (iplot.name, resultHist.Integral())


        if (iplot.name == "data" and dist == "nJets"):
 ##            print "events in bin 0 %f " % resultHist.GetBinContent(0)
##             print "events in bin 1 %f " % resultHist.GetBinContent(1)    ##----------------
            print "events in bin 2 %f " % resultHist.GetBinContent(2)
            print "events in bin 3 %f " % resultHist.GetBinContent(3)
            print "events in bin 4 %f " % resultHist.GetBinContent(4)
            print "events in bin 5 %f " % resultHist.GetBinContent(5)
            print "events in bin 6 %f " % resultHist.GetBinContent(6)
            print "events in bin 7 %f " % resultHist.GetBinContent(7)
            print "events in bin 8 %f " % resultHist.GetBinContent(8)

    # end if you are a regular plot
    # end for each plot

    if (myStack.GetMaximum() > myData.GetMaximum()) :
        plotMax = myStack.GetMaximum()
    else :
        plotMax = myData.GetMaximum()

    myStack.SetMaximum(plotMax*100)
    myStack.SetMinimum(0.1)
           
    if ( dist == "nJets" ):
        titleString = "%s;%s;%s" % ("", "Number of Jets", "Events")
    else :
        titleString = "%s;%s;%s" % ("", "Number of BTags", "Events")
    myStack.SetTitle(titleString)

    # Create a canvas
	# put the plots on the canvas
	# then save the canvas
	
    myCanvas = TCanvas(dist, dist, 600, 600)
    
    myCanvas.cd()
    gPad.SetLogy()
    
    myStack.Draw()
    myData.Draw('pesame')
    legForStack.Draw()
    myLumiTex.DrawLatex(0.25, 0.98, myLumiString)
    
    jetTitleString = getJetTitleString(jetTreatment)

##     if (jetTreatment == ">=2j"):        
##         plotxmax = myStack.GetXaxis().SetLimits(1.5,8.5)  ##--------------
##         myStack.SetMaximum(100000)
##         myStack.SetMinimum(10)
##     if (jetTreatment == ">=3j"):        
##         plotxmax = myStack.GetXaxis().SetLimits(2.5,8.5) ##--------------
##         myStack.SetMaximum(10000)
##         myStack.SetMinimum(10)

    pngName = "%s/%s_%s_%s.png" % (directoryName, dist, jetTitleString, groupName)
    myCanvas.SaveAs(pngName)
##     pdfName = "%s/%s_%s_%s.pdf" % (directoryName, dist, jetTitleString, groupName)
##     myCanvas.SaveAs(pdfName)



        


def moveExtraIntoHist (hist):
	"""Move over/underflow bins into the histogram"""
	#
	numBins = hist.GetNbinsX()
	hist.SetBinContent(1, hist.GetBinContent(0) + hist.GetBinContent(1))
	hist.SetBinContent(0,0)
	hist.SetBinContent(numBins, hist.GetBinContent(numBins) + hist.GetBinContent(numBins+1))
	hist.SetBinContent(numBins+1, 0)
	
	return hist




def rebinHistManual (origHist, nBins, xMin, xMax):
    
    nBinsOrig = int(origHist.GetNbinsX())
    xMinOrig = origHist.GetXaxis().GetXmin()
    xMaxOrig = origHist.GetXaxis().GetXmax()
    binSizeOrig = (float(xMaxOrig)-float(xMinOrig))/float(nBinsOrig)
    entries = origHist.GetEntries()
    totalEntries =0
    totalEntries += entries
    
    # Output histogram
    hist = origHist.Clone("temp")
    binSize = (float(xMax)-float(xMin))/float(nBins)



    #print out debugging info

    #print "======Rebin====="
    #print "Original Num Bins = %d from %f to %f" %(nBinsOrig, xMinOrig, xMaxOrig)
    #print "New num bins = %d from %f to %f" % (nBins, xMin, xMax)
    #print "What is the orignal binSize? %f " % (binSizeOrig)
    #print "What is the new bin size? %f" % (binSize)
    
    

    
    # We'll copy the bin contents in (regardless of resizing)
    hist.Reset()
    
    # Check that the starting bin aligns
    # There are lots and lots of problems with this
    # binary vs decimal matching
    # etc

    tolerance = 0.00001
    
    firstBinOrigHi = origHist.FindBin(xMin+tolerance)
    firstBinOrigLo = origHist.FindBin(xMin-tolerance)

    # There are two cases here, but both have the
    # same result
    #
    # case 1: your xMin value is not a boundary,
    # in which case lo==hi, so no problem
    #
    # case 2: your xMin value is a boundary.
    # in this case, you want to take the bin
    # where xMin is the *lower* boundary
    
    firstBinOrig = firstBinOrigHi

        

    
    #print "What are the high and low tolerance bins indices? hi = %d, lo = %d" % (firstBinOrigHi, firstBinOrigLo)
    #print "Which bin will you use? %d " % firstBinOrig 
    #print "What is value of the low edge in the bin you will use (sanity)? %f " % origHist.GetBinLowEdge(firstBinOrig)
    
    
    if ( not ((xMin < origHist.GetBinLowEdge(firstBinOrig)+tolerance) and (xMin > origHist.GetBinLowEdge(firstBinOrig)-tolerance ) ))  :
        print ("Bins don't align!  Requested min = %f.  Available min = %f" %
               (xMin,origHist.GetBinLowEdge(firstBinOrig)))
        print "Original: nBins = %d, xMin = %f, xMax = %f" % (nBinsOrig,xMinOrig,xMaxOrig)
        print "Rebinned: nBins = %d, xMin = %f, xMax = %f" % (nBins,xMin,xMax)
        sys.exit(2)
        
    # Make sure the original bins divide evenly into the new bins
    binRatio =  binSize / binSizeOrig
    binGroup = int(binRatio)

    #  if (binSize % binSizeOrig != 0) :
    if (binRatio - binGroup > 1e-10) :     
        print ("Bins don't align!  Requested bin size = %f.  Original bin size = %f" %
               (binSize,binSizeOrig))
        print "Original: nBins = %d, xMin = %f, xMax = %f" % (nBinsOrig,xMinOrig,xMaxOrig)
        print "Rebinned: nBins = %d, xMin = %f, xMax = %f" % (nBins,xMin,xMax)
        sys.exit(2)

    
    # OK, let's go forward with this binning
    hist.SetBins(nBins,xMin,xMax)
    
    
    # Handle the first bin (including underflow in original)
    binCont = hist.GetBinContent(1)
    binSumW2 = hist.GetBinError(1)**2
    origStart = 0  #Include the underflow in the original
    firstBinOrig = origHist.FindBin(xMin)
    origEnd = firstBinOrig + int(binGroup)

    # first loop
    for origBin in range(origStart,origEnd):
        binCont += origHist.GetBinContent(origBin)
        binSumW2 += (origHist.GetBinError(origBin)**2)
        hist.SetBinContent(1,binCont)
        hist.SetBinError(1,math.sqrt(binSumW2))
    # end loop over origBin
    
    # Do the bulk of the distribution
    for newBin in range(2,nBins):
        origStart = origEnd
        origEnd = origStart + int(binGroup)
        
        binCont = hist.GetBinContent(newBin)
        binSumW2 = hist.GetBinError(newBin)**2
        for origBin in range(origStart,origEnd):
            binCont += origHist.GetBinContent(origBin)
            binSumW2 += (origHist.GetBinError(origBin)**2)
        # end loop over orig bin
        hist.SetBinContent(newBin,binCont)
        hist.SetBinError(newBin,math.sqrt(binSumW2))
    # end loop over newBin
                
    # Do any remaining bins past the end of the new range (including overflow in original)
    origStart = origEnd
    origEnd = 2+nBinsOrig #Include the overflow in the original
    binCont = hist.GetBinContent(nBins)
    binSumW2 = hist.GetBinError(nBins)**2
    for origBin in range(origStart,origEnd):
        binCont += origHist.GetBinContent(origBin)
        binSumW2 += (origHist.GetBinError(origBin)**2)
    # end loop over origBin

    hist.SetBinContent(nBins,binCont)
    hist.SetBinError(nBins,math.sqrt(binSumW2))
    
    hist.SetEntries(totalEntries)

                 
    return hist


 ###### ##rebinning End
                 



# This was a first pass
# Might be useful as a reference
# Not currently in use now


def oldDrawInit():

	print "Hello World"

	parser = OptionParser()	
	parser.add_option('-f', '--normFile', dest='normFile', default='none', help='normalization file')
	parser.add_option('-L', '--Lumi', dest='lumi', default=0.000253, help='integrated lumi')

	(options, args) = parser.parse_args()

	if options.normFile == 'none' :
		print "Please provide a normFile"
		sys.exit()

	print "You specified norm file %s" % (options.normFile)

	

	inputFile = open(options.normFile,'r')

	plotInfoList = []

	for line in inputFile:

		#print "line is %s" %(line)
		if (line.startswith('#')):
			print "Line is comment, skip it!"
			continue
		
		if (line == "\n" ):
			print "Line is empty, skip it!"
			continue

		
		lineFields = line.split(';')

		print "This line has %d" % (len(lineFields))
		
		strippedFields = []
		for iobj in lineFields :
			print "Found object:"
			print "%s" % (iobj.strip())
			strippedFields.append(iobj.strip())
			
		temp = PlotInfo(strippedFields[0], strippedFields[1], strippedFields[2], strippedFields[3])

		plotInfoList.append(temp)

	# Done with loop over lines in the file

	print "Listing plot objects"

	for ip in plotInfoList :
		ip.show()
	
			
	return (plotInfoList, options.lumi)




