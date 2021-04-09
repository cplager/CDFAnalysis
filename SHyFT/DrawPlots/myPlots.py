

#############################################
#
# myPlots.py
#
# Think of this as a cmssw config file
#
# You will include it in your file that actually
# draws the plots
# 
# It has one function defined
# that returns a list of plot objects
# that have been properly configured.
# The function name is not important...
# you just need to know how to access
# the list of plots from the drawing script
#
# The plot objects are defined in PlotEnv
# and need a dictionary to intialize them
# 
#
#############################################



# load the usual stuff

from ROOT import *
from PlotEnv import *


def getMyPlotsDefaultNorm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             'file': '../TemplateMakers/bin/rootfiles/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD_v2.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
             'xsec': 84.6793*2,
             #'ngen': 140648660,
             'ngen': 186320560,#>=3 jets
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
             'file': '../TemplateMakers/bin/rootfiles/zjets_test2.root',
             'xsec': 3.048,
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 2543727,
             'fillColor': ROOT.kBlue-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             'file': '../TemplateMakers/bin/rootfiles/wjets_test2.root',
             'xsec': 31.314,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 14805546,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
        {'name': 'single top',
         'file': '../TemplateMakers/bin/rootfiles/singleTop_test2.root',
         'xsec': 1.0,
         'ngen': 1,
         'fillColor': ROOT.kOrange+1,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': '../TemplateMakers/bin/rootfiles/ttbar_test2.root',
             'xsec': 0.1575,
             'ngen': 1306182,
             'fillColor': ROOT.kRed-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )

    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': '../TemplateMakers/bin/rootfiles/data_test2.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 1.0
    groupName = "beforeFit"
    directoryName = "beforeFit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup


def getMyPlotsMUPTNorm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             #'file': '../../roofitter_new/templates/qcd_iso/shyftPretag_qcd_38X.root',
             'file': '../../roofitter_new/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
              'xsec': 84.6793*2,
             'ngen': 140648660,
             'fillColor': ROOT.kYellow,
             'fillPattern': 1001,
             'isNormPerJetBin': 1,
             'normsPerJetBin': [0,0,728.17, 135.99, 30.45,0]
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
             'file': '../../roofitter_new/templates/zjets/shyftPretag_zjets_38X.root',
             'xsec': 4.258,
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 2543727,
             'fillColor': ROOT.kBlue-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             'file': '../../roofitter_new/templates/wjets/shyftPretag_wjets_38X.root',
             'xsec': 37.668,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 14805546,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    plotList.append(
        PlotInfo(
        {'name': 'single top',
         'file': '../../roofitter_new/templates/singletop/shyftPretag_singleTop_merged_38X.root',
         'xsec': 1.07,
         'ngen': 1,
         'fillColor': ROOT.kOrange+1,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )

    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': '../../roofitter_new/templates/ttbar/shyftPretag_ttbar_38X.root',
             'xsec': 0.16149,
             'ngen': 1306182,
             'fillColor': ROOT.kRed-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': '../../roofitter_new/templates/data/shyftPretag_data_sync.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 1.0
    groupName = "afterMUPTFit"
    directoryName = "MUPTFit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)


    return myPlotGroup

def getMyPlotsMUPT34Norm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             #'file': '../../roofitter_new/templates/qcd_iso/shyftPretag_qcd_38X.root',
             'file': '../TemplateMakers/bin/rootfiles/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD_v2.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
              'xsec': 253.37, # 84.6793*2,
           #  'ngen': 140648660, # >=2j    ------------- ???
              'ngen': 186320560, # >=3j
             'fillColor': ROOT.kYellow,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,     # -----------
           #  'normsPerJetBin': [0,0,0, 148.4, 33.23,0]
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
             'file': '../TemplateMakers/bin/rootfiles/zjets_test2.root',
             'xsec': 3.87417,
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 2543727,
             'fillColor': ROOT.kBlue-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             'file': '../TemplateMakers/bin/rootfiles/wjets_test2.root',
             'xsec': 39.19,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 14805546,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    plotList.append(
        PlotInfo(
        {'name': 'single top',
         'file': '../TemplateMakers/bin/rootfiles/singleTop_test2.root',
         'xsec': 1.012,
         'ngen': 1,
         'fillColor': ROOT.kOrange+1,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )

    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': '../TemplateMakers/bin/rootfiles/ttbar_test2.root',
             'xsec': 0.14851,
             'ngen': 1306182,
             'fillColor': ROOT.kRed-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': '../TemplateMakers/bin/rootfiles/data_test2.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 1.0
    groupName = "afterMUPT34Fit"
    directoryName = "MUPT34Fit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup

def getMyPlotsMUPTL134Norm () :

    # Start with an empty list just to get things going
    plotList = []

    # append on an item to the list
    # you can pass it a dictionary with the pieces
    # that you want to intialize
    # yes, we are aware that it looks like cmssw
    
    plotList.append(
        PlotInfo(
            {'name': 'qcd',
             #'file': '../../roofitter_new/templates/qcd_iso/shyftPretag_qcd_38X.root',
             'file': '../../roofitter_new/templates/qcd_kit/shyftPretag_dataNonIsoKIT_38X_muon-KITQCD.root',
             #'file': 'template_histos/shyftPretag_qcd.root',
              'xsec': 84.6793*2,
             'ngen': 140648660,
             'fillColor': ROOT.kYellow,
             'fillPattern': 1001,
             'isNormPerJetBin': 1,
             'normsPerJetBin': [0,0,0, 139.15, 31.16,0]
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
             'file': '../../roofitter_new/templates/zjets/shyftPretag_zjets_38X.root',
             'xsec': 3.64,
      # MET Fit result
           #  'xsec': 3.08778,
      # M_T Fit result
           #  'xsec': 3.11705,
             'ngen': 2543727,
             'fillColor': ROOT.kBlue-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'wjets',
             'file': '../../roofitter_new/templates/wjets/shyftPretag_wjets_38X.root',
             'xsec': 36.979,
        #MET Fit result
           #  'xsec': 29.4183,
        #M_T Fit result
           #  'xsec': 29.5914,
             'ngen': 14805546,
             'fillColor': ROOT.kGreen-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    plotList.append(
        PlotInfo(
        {'name': 'single top',
         'file': '../../roofitter_new/templates/singletop/shyftPretag_singleTop_merged_38X.root',
         'xsec': 1.01,
         'ngen': 1,
         'fillColor': ROOT.kOrange+1,
         'fillPattern': 1001,
         'isNormPerJetBin': 0,}
          )
        )

    plotList.append(
        PlotInfo(
            {'name': 'ttbar',
             'file': '../../roofitter_new/templates/ttbar/shyftPretag_ttbar_38X.root',
             'xsec': 0.14552,
             'ngen': 1306182,
             'fillColor': ROOT.kRed-3,
             'fillPattern': 1001,
             'isNormPerJetBin': 0,}
            )
        )
    
    plotList.append(
        PlotInfo(
            {'name': 'data',
             'file': '../../roofitter_new/templates/data/shyftPretag_data_L1offset.root',
             'xsec': 1 ,
             'ngen': 1,
             'fillColor': 0,
             'fillPattern': 1001,
             'isNormPerJetBin': 0
             }
            )
        )

    # this lumi value will be overwritten
    defaultLumi = 1.0
    groupName = "afterMUPTL134Fit"
    directoryName = "MUPTL134Fit"
    myPlotGroup = PlotGroup (plotList, defaultLumi, groupName, directoryName)

    return myPlotGroup

         
        

