#!/usr/bin/env python


# import the environment 
from TOPPlotEnv import *

# import the plots you want
from topPlots import *

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
    
    #myLumi = float(options.lumi)
    myLumi = 5.088

    print "Using lumi %f" % myLumi

	# This function is defined in
	# myPlots.py
	# it returns the plots you want
	
    defaultPlotGroup = getMyPlotsDefaultNorm()
#    afterFitPlotGroup = getMyPlotsAfterFitNorm()
#    bckgrdSubDataPlotGroup = getMyPlotsDataMinusBckgrd()
#    qcdPlotGroup = getQCDPlots()
    #deltaPlotGroup = getMyPlotsDminBoverD();
    


    defaultPlotGroup.lumi = myLumi*1e6
#    afterFitPlotGroup.lumi = myLumi*1e6
#    bckgrdSubDataPlotGroup.lumi = myLumi*1e6
#    qcdPlotGroup.lumi = myLumi*1e6
    #deltaPlotGroup.lumi = myLumi*1e6
	

    myPlotGroups = [defaultPlotGroup]
    #myPlotGroups = [qcdPlotGroup]
    #myPlotGroups = [bckgrdSubDataPlotGroup]
    #myPlotGroups = [afterFitPlotGroup]
    #myPlotGroups = [deltaPlotGroup]
   

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
        #drawStackPlot("h_muMttbar_3j", pg, "3j", "Invariant Mass of TTbar", 60, 0, 3000, "3j")
        #drawStackPlot("h_muMttbar_4j", pg, "4j", "Invariant Mass of TTbar", 60, 0, 3000, "4j")		
        #drawStackPlot("h_muMttbar", pg, ">=4j", "Invariant Mass of TTbar", 60, 0, 3000, ">=4j")
        #drawStackPlot("h_muTTbarPt_3j", pg, "3j", "Pt of TTbar", 50, 0, 50, "3j")
        #drawStackPlot("h_muTTbarPt", pg, ">=4j", "Pt of TTbar", 50, 0, 50, ">=4j")

       ### drawStackPlot("h_nPV", pg, "Inclusive", "Number of PV", 50, -0.5, 49.5, "nJets")
##         drawStackPlot("nPV_0j", pg, "0j", "Number of PV", 25, -0.5, 24.5, "0j")
##         drawStackPlot("nPV_1j", pg, "1j", "Number of PV", 25, -0.5, 24.5, "1j")
##         drawStackPlot("nPV_2j", pg, "2j", "Number of PV", 25, -0.5, 24.5, "2j")
##         drawStackPlot("nPV_3j", pg, "3j", "Number of PV", 25, -0.5, 24.5, "3j")
        #drawStackPlot("h_nPV", pg, ">=4j", "Number of PV", 25, -0.5, 24.5, ">=4j")
##
#        drawStackPlot("nJets", pg, "Inclusive", "N Jets", 6, -0.5, 5.5, "nJets")

        #drawStackPlot("h_muMET_2j",    pg, "2j", "Missing E_{T}", 50, 0, 200, "2j")
        #drawStackPlot("h_muMET_4j",    pg, "4j", "Missing E_{T}", 50, 0, 200, "4j")
        #drawStackPlot("h_muMET",    pg, ">=4j", "Missing E_{T}", 50, 0, 200, ">=4j")
       # drawStackPlot("h_muMET",    pg, "Inclusive", "Missing E_{T}", 50, 0, 200, "nJets")
        #drawStackPlot("h_muMt_2j",  pg, "2j", "Transverse Mass", 50, 0, 200, "2j")
        #drawStackPlot("h_muMt_4j",  pg, "4j", "Transverse Mass", 50, 0, 200, "4j")
        #drawStackPlot("h_muMt",    pg, ">=4j", "Transverse Mass", 50, 0, 200, ">=4j")
        #drawStackPlot("h_muMt",    pg, "Inclusive", "Transverse Mass", 50, 0, 200, "nJets")

        drawStackPlot("h_muM3_4j", pg, "4j", "M3", 40, 0, 1200, "4j")
        drawStackPlot("h_muM3", pg, ">=4j", "M3", 40, 0, 1200, ">=4j")
        drawStackPlot("h_muHt_4j", pg, "4j", "H_{T,All}", 50, 0, 2000, "4j")
        drawStackPlot("h_muHt", pg, ">=4j", "H_{T,All}", 50, 0, 2000, ">=4j")

        #drawStackPlot("h_muEta_2j", pg, "2j", "Muon #eta",20, -2.4, 2.4,"2j")
        #drawStackPlot("h_muEta_4j", pg, "4j", "Muon #eta",20, -2.4, 2.4,"4j")
        #drawStackPlot("h_muEta", pg, ">=4j", "Muon #eta",20, -2.4, 2.4,">=4j")
        #drawStackPlot("h_muPt_2j", pg, "2j", "Muon P_{T}", 40, 27, 267, "2j")
        #drawStackPlot("h_muPt_4j", pg, "4j", "Muon P_{T}", 40, 27, 267, "4j")
        #drawStackPlot("h_muPt", pg, ">=4j", "Muon P_{T}", 40, 27, 267, ">=4j")

##         drawStackPlot("jet1Eta_3j", pg, "3j", "Lead Jet #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet1Pt_3j", pg, "3j", "Lead Jet P_{T}", 35, 30, 380, "3j")
##         drawStackPlot("jet1Phi_3j", pg, "3j", "Lead Jet #phi", 25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet1Eta", pg, ">=4j", "Lead Jet #eta", 20, -2.5, 2.5, ">=4j")
##        drawStackPlot("jet1Pt", pg, ">=4j", "Lead Jet P_{T}", 35, 30, 380, ">=4j")
##        drawStackPlot("jet1Phi", pg, ">=4j", "Lead Jet #phi", 25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet2Eta_3j", pg, "3j", "Jet 2 #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet2Pt_3j", pg, "3j", "Jet 2 P_{T}", 35, 30, 380, "3j")
##         drawStackPlot("jet2Phi_3j", pg, "3j", "Jet 2 #phi", 25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet2Eta", pg, ">=4j", "Jet 2 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet2Pt", pg, ">=4j", "Jet 2 P_{T}", 35, 30, 380, ">=4j")
##         drawStackPlot("jet2Phi", pg, ">=4j", "Jet 2 #phi", 25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet3Eta_3j", pg, "3j", "Jet 3 #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet3Pt_3j", pg, "3j", "Jet 3 P_{T}", 35, 30, 380, "3j")
##         drawStackPlot("jet3Phi_3j", pg, "3j", "Jet 3  #phi",25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet3Eta", pg, ">=4j", "Jet 3 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet3Pt", pg, ">=4j", "Jet 3 P_{T}", 35, 30, 380, ">=4j")
##         drawStackPlot("jet3Phi", pg, ">=4j", "Jet 3  #phi",25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet4Eta", pg, ">=4j", "Jet 4 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet4Pt", pg, ">=4j", "Jet 4 P_{T}", 35, 30, 380, ">=4j")
##         drawStackPlot("jet4Phi", pg, ">=4j", "Jet 4  #phi",25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet5Eta", pg, ">=5j", "Jet 5 #eta", 10, -2.5, 2.5, ">=5j")
##         drawStackPlot("jet5Pt", pg, ">=5j", "Jet 5 P_{T}", 35, 30, 380, ">=5j")
##         drawStackPlot("jet5Phi", pg, ">=5j", "Jet 5  #phi",25, -3.1416, 3.1416, ">=5j")

##         drawStackPlot("PFIso", pg, "Inclusive", "PF RelIso", 40, 0, 0.125, "nJets")

##         drawStackPlot("htLep_3j", pg, "3j", "H_{T,lep}", 60, 0, 600, "3j")
##         drawStackPlot("htLep", pg, ">=4j", "H_{T,lep}", 60, 0, 600, ">=4j")


##         ##Combined Mass Plots
##         drawStackPlot("Mttbar_3j", pg, "3j", "Invariant Mass of TTbar", 50, 0, 2000, "3j")
##        ##drawStackPlot("Mttbar_4j", pg, "4j", "Invariant Mass of TTbar", 20, 0, 2000, "4j")
##         ##drawStackPlot("Mttbar_5j", pg, "5j", "Invariant Mass of TTbar", 20, 0, 2000, "5j")
##      drawStackPlot("Mttbar", pg, ">=4j", "Invariant Mass of TTbar", 50, 0, 2500, ">=4j")
##         drawStackPlot("mTop_had_3j", pg, "3j", "Invariant Mass of Hadronic Top", 60, 0, 1200, "3j")
##         drawStackPlot("mTop_had", pg, ">=4j", "Invariant Mass of Hadronic Top", 60, 0, 1200, ">=4j")
##         drawStackPlot("mTop_lep_3j", pg, "3j", "Invariant Mass of Leptonic Top", 60, 0, 1200, "3j")
##         drawStackPlot("mTop_lep", pg, ">=4j", "Invariant Mass of Leptonic Top", 60, 0, 1200, ">=4j")
##         #drawStackPlot("m2_3j", pg, "3j", "M2", 20, 0, 1200, "3j")
##         #drawStackPlot("m2", pg, ">=4j", "M2", 20, 0, 1200, ">=4j")
## ################################       pival = 3.1416
## ##         ##Leading Jet Variables
## ## 		## need to e
##         drawStackPlot("jet1Eta_3j", pg, "3j", "Lead Jet #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet1Pt_3j", pg, "3j", "Lead Jet P_{T}", 35, 30, 240, "3j")
##         drawStackPlot("jet1Phi_3j", pg, "3j", "Lead Jet #phi", 25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet1Eta", pg, ">=4j", "Lead Jet #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet1Pt", pg, ">=4j", "Lead Jet P_{T}", 35, 30, 240, ">=4j")
##         drawStackPlot("jet1Phi", pg, ">=4j", "Lead Jet #phi", 25, -3.1416, 3.1416, ">=4j")
## 		##Other Jet Variables
##         drawStackPlot("jet2Eta_3j", pg, "3j", "Jet 2 #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet2Pt_3j", pg, "3j", "Jet 2 P_{T}", 35, 30, 240, "3j")
##         drawStackPlot("jet2Phi_3j", pg, "3j", "Jet 2 #phi", 25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet2Eta", pg, ">=4j", "Jet 2 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet2Pt", pg, ">=4j", "Jet 2 P_{T}", 35, 30, 240, ">=4j")
##         drawStackPlot("jet2Phi", pg, ">=4j", "Jet 2 #phi", 25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet3Eta_3j", pg, "3j", "Jet 3 #eta", 20, -2.5, 2.5, "3j")
##         drawStackPlot("jet3Pt_3j", pg, "3j", "Jet 3 P_{T}", 35, 30, 240, "3j")
##         drawStackPlot("jet3Phi_3j", pg, "3j", "Jet 3  #phi",25, -3.1416, 3.1416, "3j")
##         drawStackPlot("jet3Eta", pg, ">=4j", "Jet 3 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet3Pt", pg, ">=4j", "Jet 3 P_{T}", 35, 30, 240, ">=4j")
##         drawStackPlot("jet3Phi", pg, ">=4j", "Jet 3  #phi",25, -3.1416, 3.1416, ">=4j")
##         drawStackPlot("jet4Eta", pg, ">=4j", "Jet 4 #eta", 20, -2.5, 2.5, ">=4j")
##         drawStackPlot("jet4Pt", pg, ">=4j", "Jet 4 P_{T}", 35, 30, 240, ">=4j")
##         drawStackPlot("jet4Phi", pg, ">=4j", "Jet 4  #phi",25, -3.1416, 3.1416, ">=4j")
##         #drawStackPlot("jet5Eta", pg, ">=5j", "Jet 5 #eta", 10, -2.5, 2.5, ">=5j")
##         #drawStackPlot("jet5Pt", pg, ">=5j", "Jet 5 P_{T}", 35, 30, 240, ">=5j")
##         #drawStackPlot("jet5Phi", pg, ">=5j", "Jet 5  #phi",25, -3.1416, 3.1416, ">=5j")

##         ##dPhi Met and Jet or Lepton
##         drawStackPlot("dPhiMetJet1_3j", pg, "3j", "Leading Jet  d#phi (MET, jet1)",25, 0.0, 3.1416, "3j")
##         drawStackPlot("dPhiMetJet1", pg, ">=4j", "Leading Jet  d#phi (MET, jet1)",25, 0.0, 3.1416, ">=4j")
##         drawStackPlot("dPhiMetJet2_3j", pg, "3j", "d#phi (MET, jet2)",25, 0.0, 3.1416, "3j")
##         drawStackPlot("dPhiMetJet2", pg, ">=4j", "d#phi (MET, jet2)",25, 0.0, 3.1416, ">=4j")
##         drawStackPlot("dPhiMetJet3_3j", pg, "3j", "d#phi (MET, jet3)",25, 0.0, 3.1416, "3j")
##         drawStackPlot("dPhiMetJet3", pg, ">=4j", "d#phi (MET, jet3)",25, 0.0, 3.1416, ">=4j")
##         drawStackPlot("dPhiMetJet4", pg, ">=4j", "d#phi (MET, jet4)",25, 0.0, 3.1416, ">=4j")		
##         drawStackPlot("dPhiMetLep_3j", pg, "3j", "d#phi (MET, Lep)",25, 0.0, 3.1416, "3j")
##         drawStackPlot("dPhiMetLep", pg, ">=4j", "d#phi (MET, Lep)",25, 0.0, 3.1416, ">=4j")
		
##         #drawStackPlot("nPV", pg, "Inclusive", "number of PV", 15, -0.5, 14.5, "nJets")
       # drawStackPlot("nPV_3j", pg, "3j", "number of PV", 15, -0.5, 14.5, "3j")
##         #drawStackPlot("nPVvalid", pg, "Inclusive", "number of valid PV", 15, 0.5, 14.5, "nJets")
    ##   drawStackPlot("nPVvalid_3j", pg, "3j", "number of valid PV", 15, -0.5, 14.5, "3j")
    ##     drawStackPlot("nJets", pg, "Inclusive", "N Jets", 6, -0.5, 5.5, "nJets")
		
    print "Done"

    return



	
	
# This allows you to run at the command line	
# tells you to call the main function defined above
if __name__ == '__main__':
	main()


