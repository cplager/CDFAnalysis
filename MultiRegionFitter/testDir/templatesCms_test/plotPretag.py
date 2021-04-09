#! /usr/bin/env python

import ROOT
import re

nonAlphaRE   = re.compile (r'\W')
underscoreRE = re.compile (r'_')
jetRE        = re.compile (r'_(\d)j')

colorDict = { 'Top'   : 2,  
              'WJets' : 56,  
              'QCD'   : 33,   }

plotList = ['met_0j', 'met_1j', 'met_2j', 'met_3j', 'met_4j', 'met_5j', 
            'ht_0j' , 'ht_1j' , 'ht_2j' , 'ht_3j' , 'ht_4j' , 'ht_5j' ,]

def plotMany (title, histograms):
    # renormalize all three
    maxValues = []
    for hist in histograms:
        if hist.Integral():
            hist.Scale( 1./ hist.Integral() )
        hist.SetLineWidth (2)
        maxValues.append( hist.GetMaximum() )
    maximum = max (maxValues) * 1.2
    first = True
    c1 = ROOT.TCanvas()
    for hist in histograms:
        if first:
            hist.SetTitle (title)
            hist.SetMaximum (maximum)
            hist.Draw()
            first = False
        else:
            hist.Draw('Same')
    filename = nonAlphaRE.sub ('_', title) + '.png'
    c1.Print (filename)


ROOT.gROOT.SetStyle('Plain')
ROOT.gStyle.SetOptStat (0)
ROOT.gROOT.SetBatch()
rootfile = ROOT.TFile.Open ('shyftPretag.root')
for plot in plotList:
    pretty = jetRE.sub (r' \1 Jets', plot)
    pretty = underscoreRE.sub (' ', pretty)
    histList = []
    for sample, color in sorted (colorDict.iteritems()):
        histname = '%s_%s' % (sample, plot)
        hist = rootfile.Get (histname)
        hist.SetLineColor (color)
        histList.append (hist)
        print "hist", hist.Integral(), histname
    plotMany (pretty, histList)
