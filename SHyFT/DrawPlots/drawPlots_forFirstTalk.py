#!/usr/bin/env python


# import the environment 
from PlotEnv import *

# import the plots you want
from myPlots import *

# import the root libraries
# with this kind of import
# you won't need to prefix your root objects with ROOT

from ROOT import *



# this is the function that is called
# when you type ./drawPlots.py
# there are some optional arguments

def main ():

	
	# Parse the options
    parser = OptionParser()
    parser.add_option('-L', '--Lumi', dest='lumi', default=0.0361, help='integrated lumi')
    parser.add_option('-b', dest="batch", action='store_true', default=false)
    # 30277
    
    (options, args) = parser.parse_args()
    
    myLumi = options.lumi

    print "Using lumi %f" % myLumi

	# This function is defined in
	# myPlots.py
	# it returns the plots you want
	
    defaultPlotGroup = getMyPlotsDefaultNorm()
##     metPlotGroup = getMyPlotsMETFittedNorm()
##    muptPlotGroup = getMyPlotsMUPTNorm()
##     mtPlotGroup = getMyPlotsMTFittedNorm()
##     htPlotGroup = getMyPlotsHTFittedNorm()
    mupt34PlotGroup = getMyPlotsMUPT34Norm()
##    muptL134PlotGroup = getMyPlotsMUPTL134Norm()


    defaultPlotGroup.lumi = myLumi*1e6
##    muptPlotGroup.lumi = myLumi*1e6
    mupt34PlotGroup.lumi = myLumi*1e6
##    muptL134PlotGroup.lumi = myLumi*1e6
##     metPlotGroup.lumi = myLumi*1e6
##     mtPlotGroup.lumi = myLumi*1e6
##     htPlotGroup.lumi = myLumi*1e6

    myPlotGroups = [defaultPlotGroup,mupt34PlotGroup]
##  myPlotGroups = [defaultPlotGroup,muptPlotGroup,mupt34PlotGroup,muptL134PlotGroup]
    #myPlotGroups = [defaultPlotGroup, metPlotGroup, mtPlotGroup, htPlotGroup]
    



    # Set the style for your plots

    ROOT.gROOT.SetStyle("Plain")
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetPadLeftMargin(0.15)
    ROOT.gStyle.SetPadRightMargin(0.07)
    ROOT.gStyle.SetPadTopMargin(0.07)
    ROOT.gStyle.SetPadBottomMargin(0.1)
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetTitleOffset(1.4, "y")


    #
	# This is the good stuff
	# draw the plots
	# Arguments are
	#   1. What distribution?
	#   2. Which plots?
	#   3. Title for the plots
	#   4. Title for the x-axis
    #   5,6,7. New binning, just like TH1F arguments (bins, xMin, xMax)
    #   9. Jet treatment: which jet bins to plot this for (0,1,2,>=0,>=1 etc)
    #

    	# Echo the current config
    for pg in myPlotGroups :
        pg.show()

        #drawStackPlot("met_0j", pg, "0j", "Missing E_{T}", 35, 0, 70, "0j")
        #drawStackPlot("met_1j", pg, "1j", "Missing E_{T}", 20, 0, 80, "1j")
##         drawStackPlot("met",    pg, ">=3j", "Missing E_{T}", 25, 0, 200, ">=3j")
##         drawStackPlot("met",    pg, "3j", "Missing E_{T}", 25, 0, 200, "3j")
##         #drawStackPlot("met",    pg, "Inclusive", "Missing E_{T}", 10, 0, 80, ">=0j")
##         drawStackPlot("ht",    pg, ">=3j", "H_{T} jets", 20, 0, 480, ">=3j")

##         drawStackPlot("met",    pg, ">=2j", "Missing E_{T}", 25, 0, 200, ">=2j")
##         drawStackPlot("met",    pg, "3j", "Missing E_{T}", 25, 0, 200, "3j")
##         #drawStackPlot("met",    pg, "Inclusive", "Missing E_{T}", 10, 0, 80, ">=0j")
##         drawStackPlot("ht",    pg, ">=2j", "H_{T} jets", 20, 0, 480, ">=2j")

##         #drawStackPlot("mT_0j", pg, "0j", "Transverse Mass", 40, 0, 120, "0j")
##         #drawStackPlot("mT_1j", pg, "1j", "Transverse Mass", 20, 0, 120, "1j")    
##         drawStackPlot("mT_2j", pg, "2j", "Transverse Mass", 10, 0, 120, "2j")
##         drawStackPlot("mT_3j", pg, "3j", "Transverse Mass", 10, 0, 120, "3j")    
##         drawStackPlot("mT",    pg, ">=3j", "Transverse Mass", 10, 0, 120, ">=3j")
##         drawStackPlot("mT",    pg, ">=4j", "Transverse Mass", 10, 0, 120, ">=4j")
        #drawStackPlot("mT",    pg, "Inclusive", "Transverse Mass", 10, 0, 120, ">=0j")

##         drawStackPlot("muEta", pg, "Inclusive", "Muon #eta", 20, -2.0, 2.0, ">=2j")
##         drawStackPlot("muEta", pg, "Inclusive", "Muon #eta", 20, -2.0, 2.0, ">=3j")

##         drawStackPlot("htLep_1j", pg, "1j", "H_{T,lep}", 40, 20, 180, "1j")
##         drawStackPlot("htLep_0j", pg, "0j", "H_{T,lep}", 50, 20, 120, "0j")
##         drawStackPlot("htLep", pg, ">=3j", "H_{T,lep}", 10, 20, 200, ">=3j")
##         drawStackPlot("htLep", pg , "Inclusive", "H_{T,lep}", 32,0, 160, ">=0j")
    
##         drawStackPlot("muPt", pg, "Inclusive", "Muon P_{T}", 36, 0, 180, ">=2j")
##         drawStackPlot("muPt", pg, "Inclusive", "Muon P_{T}", 36, 0, 180, ">=3j")
##         drawStackPlot("muPt_2j", pg, "2j", "Muon P_{T}", 28, 0, 140, "2j")
##         drawStackPlot("muPt_3j", pg, "3j", "Muon P_{T}", 14, 0, 140, "3j")
##         drawStackPlot("muPt", pg, "Inclusive", "Muon P_{T}", 18, 0, 180, ">=4j")

    ##----------------------------
##         drawStackPlot("Mttbar", pg, ">=2j", "Invariant Mass of TTbar", 30, 0, 1200, ">=2j")
##         drawStackPlot("Mttbar_3j", pg, "3j", "Invariant Mass of TTbar", 40, 0, 1600, "3j")
##         drawStackPlot("Mttbar_4j", pg, "4j", "Invariant Mass of TTbar", 40, 0, 1600, "4j")
##         drawStackPlot("Mttbar_5j", pg, "5j", "Invariant Mass of TTbar", 40, 0, 1600, "5j")
##         drawStackPlot("Mttbar", pg, ">=6j", "Invariant Mass of TTbar", 40, 0, 1600, ">=6j")
##         drawStackPlot("MttbarTop4jets", pg, ">=4j", "Invariant Mass of TTbar with 4 leading jets", 40, 0, 1600, ">=4j")

##         drawStackPlot("m2_dR", pg, ">=2j", "m2_dR", 60, 0, 600, ">=2j")
##         drawStackPlot("m3", pg, ">=3j", "m3", 40, 0, 800, ">=3j")
##         drawStackPlot("m3Low", pg, ">=3j", "m3Low", 40, 0, 800, ">=3j")
##        drawStackPlot("mW_had", pg, ">=4j", "mW_had", 50, 0, 500, ">=4j")
        drawStackPlot("m3Low_3j", pg, "3j", "m3Low", 40, 0, 800, "3j")
        drawStackPlot("m3Low_4j", pg, "4j", "m3Low", 40, 0, 800, "4j")
        drawStackPlot("m3Low_5j", pg, "5j", "m3Low", 40, 0, 800, "5j")
        drawStackPlot("m3Low", pg, ">=6j", "m3Low", 40, 0, 800, ">=6j")
        drawStackPlot("mW_had", pg, "4j", "mW_had", 50, 0, 500, "4j")
        drawStackPlot("mW_had_5j", pg, "5j", "mW_had", 50, 0, 500, "5j")
        drawStackPlot("mW_had", pg, ">=6j", "mW_had", 50, 0, 500, ">=6j")
        
        pival = 3.1416
        
##         drawStackPlot("jet1Eta", pg, ">=2j", "Lead Jet #eta", 10, -2.5, 2.5, ">=2j")
##         drawStackPlot("jet1Pt", pg, ">=2j", "Lead Jet P_{T}", 35, 30, 240, ">=2j")
    
##         drawStackPlot("jet2Eta", pg,">=2j", "Second Jet #eta",  10, -2.5, 2.5, ">=2j")
##         drawStackPlot("jet2Pt", pg, ">=2j", "Second Jet P_{T}", 14, 30, 114, ">=2j")

##         drawStackPlot("jet1Eta", pg, ">=3j", "Lead Jet #eta", 10, -2.5, 2.5, ">=3j")
##         drawStackPlot("jet1Pt", pg, ">=3j", "Lead Jet P_{T}", 35, 30, 240, ">=3j")
    
##         drawStackPlot("jet2Eta", pg,">=3j", "Second Jet #eta",  10, -2.5, 2.5, ">=3j")
##         drawStackPlot("jet2Pt", pg, ">=3j", "Second Jet P_{T}", 14, 30, 114, ">=3j")
        
 ##        drawNjetsPlot("nJets",pg, ">=4j")        
##         drawNjetsPlot("nJets",pg, ">=6j")
##         drawNjetsPlot("nBTagTC",pg, "")
##         drawNjetsPlot("nBTagSSV",pg, "")
        
        
    print "Done"

    return



	
	
# This allows you to run at the command line	
# tells you to call the main function defined above
if __name__ == '__main__':
	main()


