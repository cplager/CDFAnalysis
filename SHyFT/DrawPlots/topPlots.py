from ROOT import *
from TOPPlotEnv import *


def getMyPlotsDefaultNorm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
##     plotList.append(
##         PlotInfo(
##             {'name': 'qcd',
## 		     'file': 'templates/sqwatTop_qcd_Model_v2_42X.root',
## 		     'file': 'template_histos/sqwatTop_qcd_Model_42X.root',
## 			 #'file': 'templates/ReW_177515/sqwatTop_qcd_177515rew_42X.root',
##              #'file': 'sqwatTop_qcd_Model_42X.root',
##              #'file': 'template_histos/shyftPretag_qcd.root',
##              'xsec': 84.6793*2,
##              #'ngen': 140648660,
##              'ngen': 25064715,
##              'fillColor': ROOT.kYellow,
##              'fillPattern': 1001,
##              'isNormPerJetBin': 0,
##             # 'isNormPerJetBin': 1,
##       # MET Fit result    
##            #  'normsPerJetBin': [240.988, 85.7213, 17.2649, 0, 0, 0]
##       # M_T Fit result    
##            #  'normsPerJetBin': [222.048, 81.5971, 24.4153, 0, 0, 0]         
##              }
##             )
##     )

    plotList.append(
        PlotInfo(
            {'name': 'zjets',
             #'file': 'templates/sqwatTop_zjets_full_cRW_42X.root',
			 'file': '../RooFitter/templates/zjets/sqwatTop_zjets_52X_1btag_epu.root',
			 #'file': 'templates/sqwatTop_zjets_full_177515rew_42X.root',
             'xsec': 3.503,
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 1082838,
             'fillColor': ROOT.kAzure-2,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             #'file': 'templates/sqwatTop_wjets_full_cRW_42X.root',
			 'file': '../RooFitter/templates/wjets/sqwatTop_wjets_52X_1btag_epu.root',
             #'file': 'templates/sqwatTop_wjets_177515rew_v2_42X.root',
			 #'file': 'templates/sqwatTop_wjets_full_177515rew_42X.root',
             'xsec': 36.257,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 18393090,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
        {'name': 'single top',
         #'file': 'templates/sqwatTop_singleTop_42X.root',
         'file': '../RooFitter/templates/singletop/sqwatTop_singleTop_t_52X_1btag_epu.root',
         #'file': 'templates/sqwatTop_singleTop_177515rew_v2_42X.root',
         'xsec': 1,
         'ngen': 1,
         'fillColor': ROOT.kPink+10,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )

    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             #'file': 'templates/sqwatTop_ttbar_42X.root',
			 'file': '../RooFitter/templates/ttbar/sqwatTop_ttbar_52X_1btag_epu.root',
             #'file': 'templates/sqwatTop_ttbar_177515rew_v2_42X.root',
             'xsec': 0.225,
             'ngen': 6736135,
             'fillColor': ROOT.kRed+1,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'data',
			 'file':'../RooFitter/templates/data/sqwatTop_data2012_52X_1btag_epu.root',
             #'file': 'templates/sqwatTop_data_42X.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 3900
    groupName = "beforeFit"
    directoryName = "beforeFit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup



def getMyPlotsAfterFitNorm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             'file': 'templates/sqwatTop_qcd_Model_42X.root',
             #'file': '../RooFitter/templates/qcd_iso/sqwatTop_qcd_42X.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
             'xsec': 84.6793*2,
             #'ngen': 140648660,
             'ngen': 25064715,
             'fillColor': ROOT.kYellow,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,
            # 'isNormPerJetBin': 1,
      # MET Fit result    
           #  'normsPerJetBin': [240.988, 85.7213, 17.2649, 0, 0, 0]
      # M_T Fit result    
           #  'normsPerJetBin': [222.048, 81.5971, 24.4153, 0, 0, 0]         
             }
            )
    )

    plotList.append(
        PlotInfo(
            {'name': 'zjets',
             'file': 'templates/sqwatTop_zjets_full_cRW_42X.root',
             'xsec': 3.9405,  #mult fit ratio by fitted wjets xsec
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 36277961,
             'fillColor': ROOT.kAzure-2,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    plotList.append(
        PlotInfo(
        {'name': 'single top',
         'file': 'templates/sqwatTop_singleTop_42X.root',
         'xsec': 0.90343,
         'ngen': 1,
         'fillColor': ROOT.kPink+10,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )	
    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             'file': 'templates/sqwatTop_wjets_full_cRW_42X.root',
             'xsec': 30.108,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 81352581,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
##     plotList.append(
##         PlotInfo(
##         {'name': 'single top',
##          'file': '../TemplateMakers/bin/sqwatTop_singleTop_t_42X.root',
##          'xsec': 0.073206,
##          'ngen': 1,
##          'fillColor': ROOT.kPink+10,
##          'fillPattern': 1001,
##          'isNormPerJetBin': 0,}
##           )
##         )
   
    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': 'templates/sqwatTop_ttbar_42X.root',
             'xsec': 0.16126,
             'ngen': 3701947,
             'fillColor': ROOT.kRed+1,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': 'templates/sqwatTop_data_42X.root',
			 #'file': 'sqwatTop_data_42X.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 2.084
    groupName = "afterFit"
    directoryName = "afterFit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup

def getQCDPlots () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             'file': 'templates/sqwatTop_qcd_Model_42X.root',
             #'file': '../RooFitter/templates/qcd_iso/sqwatTop_qcd_42X.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
             'xsec': 84.6793*2,
             #'ngen': 140648660,
             'ngen': 25064715,
             'fillColor': ROOT.kYellow,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,
            # 'isNormPerJetBin': 1,
      # MET Fit result    
           #  'normsPerJetBin': [240.988, 85.7213, 17.2649, 0, 0, 0]
      # M_T Fit result    
           #  'normsPerJetBin': [222.048, 81.5971, 24.4153, 0, 0, 0]         
             }
            )
    )

    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': 'templates/sqwatTop_data_42X.root',
			 #'file': 'sqwatTop_data_42X.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 2.878
    groupName = "afterFit"
    directoryName = "afterFit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup


def getMyPlotsDataMinusBckgrd () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': 'templates/sqwatTop_ttbar_42X.root',
             'xsec': 0.17483,
             'ngen': 3701947,
             'fillColor': ROOT.kRed+1,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': 'templates/sqwatTop_data_noBkgr_42X.root',
			 #'file': 'sqwatTop_data_42X.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )



    # this lumi value will be overwritten
    defaultLumi = 1.4920
    groupName = "bckgrdSubData"
    directoryName = "bckgrdSubData"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup
def getMyPlotsDminBoverD () :

	##to draw these plots you need to go to PlotEnv.py
	## in drawStackplots turn off drawStackPlot and just have drawData("") no options

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': 'sqwatTop_data_noBkgr_42X.root',
             'xsec': 1,
             'ngen': 1,
             'fillColor': kBlue,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': 'sqwatTop_DmBoBRatios.root',
			 #'file': 'sqwatTop_data_42X.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )



    # this lumi value will be overwritten
    defaultLumi = 1.4920
    groupName = "delta"
    directoryName = "deltaPlots"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup
