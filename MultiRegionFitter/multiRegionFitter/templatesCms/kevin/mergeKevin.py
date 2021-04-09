#! /usr/bin/env python

import ROOT
import sys

## Normalizations from http://hep.pha.jhu.edu:8080/top/401
##           Counts   xsec(nb)  weight (1e6/N)
## top      1483404     0.165      0.674125188
## wjets   10068895    28          0.099315764
## incMu15  3948423    79.688      0.253265671
 

fileDict = { 'Data'  : ('shyftPretag_ISOCUTttbar_36X.root',   1.0        ),
             'Top'   : ('shyftPretag_ISOCUTttbar_36X.root',   0.674125188),  
             'WJets' : ('shyftPretag_ISOCUTwjets_36X.root',   0.099315764),  
             'QCD'   : ('shyftPretag_ISOCUTincmu15_36X.root', 0.253265671),   }

rangeDict = { 'met_0j' : (20,   0, 160),
              'met_1j' : (20,   0, 160),
              'met_2j' : (20,   0, 160),
              'met_3j' : (20,   0, 160),
              'met_4j' : ( 8,   0, 128),
              'met_5j' : ( 8,   0, 128),
              'ht_0j'  : (10,  20, 300),
              'ht_1j'  : (10,  50, 300),
              'ht_2j'  : (10,  80, 400),
              'ht_3j'  : (10, 110, 500),
              'ht_4j'  : (10, 140, 600),
              'ht_5j'  : (10, 170, 600),
              }

combineDict = { 'met_ge4j' : ('met_4j', 'met_5j') }
## h.SetDirectory(0);         // for the current histogram h
## TH1::AddDirectory(kFALSE);  // sets a global switch disabling the reference
ROOT.TH1.AddDirectory (False)

def copyHistogram (histogram, newName, rangeTuple, weight = 1.,
                   moveOverflow = True):
    '''Given an input histogram, it copies the bin contents to the new
    range'''
    # get the numbers first
    oldNum   = histogram.GetNbinsX()
    oldLower = histogram.GetXaxis().GetXmin()
    oldUpper = histogram.GetXaxis().GetXmax()
    oldStep  = (oldUpper - oldLower) / oldNum
    # convert range tuple into number of bins
    num   = rangeTuple[0]
    lower = rangeTuple[1]
    upper = rangeTuple[2]
    lowerBinFloat = (lower - oldLower) / oldStep
    upperBinFloat = (upper - oldLower) / oldStep
    lowerBin = int( round( lowerBinFloat ) )
    upperBin = int( round( upperBinFloat ) )
    lowerEpsilon = abs( lowerBin - lowerBinFloat )
    upperEpsilon = abs( upperBin - upperBinFloat )
    maxEpsilon = 1e-6
    if lowerEpsilon > maxEpsilon or upperEpsilon > maxEpsilon:
        raise RuntimeError, "lowerBinFloat (%f) or upperBinFloat (%f) are not close enough to integer values." % (lowerBinFloat, upperBinFloat)
    if (upperBin - lowerBin) % num:
        raise RuntimeError, "Number of bins is not evenly divisible (%d, %d)" \
              % (upperBin - lowerBin, num)
    step = (upperBin - lowerBin) / num
    newHist = ROOT.TH1F (newName, newName,
                         rangeTuple[0], rangeTuple[1], rangeTuple[2])
    for newIndex in range (num):
        contents = 0.
        for subindex in range (step):
            binIndex = lowerBin + newIndex * step + subindex + 1
            contents += histogram.GetBinContent (binIndex)
        newHist.SetBinContent (newIndex + 1, contents * weight)
    underflow = 0.
    # for 0 .. lowerBin - 1
    for binIndex in range (0, lowerBin):
        underflow += histogram.GetBinContent (binIndex)
    if moveOverflow:
        newHist.SetBinContent (1,
                               underflow * weight + newHist.GetBinContent (1))
    else:
        newHist.SetBinContent (0, underflow * weight)
    overflow = 0.
    # for upperBin + 1 .. oldNum + 1
    for binIndex in range (upperBin + 1, oldNum + 1 + 1):
        overflow += histogram.GetBinContent (binIndex)
    if moveOverflow:
        newHist.SetBinContent (num,
                               overflow * weight + newHist.GetBinContent (num))
    else:
        newHist.SetBinContent (num + 1, overflow * weight)
    ## print newName, "histogram", histogram.Integral (0, oldNum + 1) -\
    ##       newHist.Integral (0, num + 1) / weight
    
    return newHist
        

histograms = {}
for sample, sampleTuple in sorted (fileDict.iteritems()):
    filename = sampleTuple[0]
    weight   = sampleTuple[1]
    rootfile = ROOT.TFile.Open (filename)
    if not rootfile:
        raise RuntimeError, "Can't open file %s" % filename
    # copy histograms
    for histname, newRange in sorted (rangeDict.iteritems()):
        hist = rootfile.Get (histname)
        if not hist:
            raise RuntimeError, "Can't find '%s' in '%s'" % (histname, filename)
        newName = '%s_%s' % (sample, histname)
        histograms[newName] = copyHistogram (hist, newName, newRange, weight)
    rootfile.Close()
    # combine histograms (if requested)
    for target, pieces in combineDict.iteritems():
        newHist = 0
        newName = '%s_%s' % (sample, target)
        for index, name in enumerate (pieces):
            oldName = '%s_%s' % (sample, name)
            if not index:
                newHist = histograms[oldName].Clone (newName)
                newHist.SetTitle (newName)
            else:
                newHist.Add( histograms[oldName] )
        histograms[newName] = newHist
                          

out = ROOT.TFile.Open ('shyftPretag.root', 'RECREATE')
for name, hist in sorted (histograms.iteritems()):
    hist.Write()
out.Close()
