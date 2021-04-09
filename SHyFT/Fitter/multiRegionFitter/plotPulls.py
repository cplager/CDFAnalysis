#! /usr/bin/env python

import ROOT
import glob
import optparse
import math
import re
import time

# plots to make
plotsList = [
    # name     hist range       draw string
    ('Pull',   (100, -5, 5),   "(%(meas)s-%(gen)s)/%(err)s", 'gaus'),
    ('Err',    (),   "%(err)s"),
    ('Meas',   (),   "%(meas)s-%(gen)s", 'gaus'),
    ('Gen',    (),  "%(gen)s", 'gaus'),
    ('RelUnc', (), "%(err)s/abs(%(meas)s)"),
    ]

# Get the labels for the x-axis
axisDict = {'Pull' : "Pull Value",
            'Meas' : "Measured Value",
            'Gen'  : "Generated Value",
            'Err'  : "Expected Error",
            'RelUnc' : "Relative Uncertainty"}

# Get the units for the x-axis
unitDict = {'Pull' : "",
            'Meas' : " (pb)",
            'Gen'  : " (pb)",
            'Err'  : " (pb)",
            'RelUnc' : ""}

ggRE = re.compile (r'>>')

def prettyUnit (origEdge, unit, asSmall):
    sign = 1
    if origEdge < 0:
        sign = -1
    edge = abs (origEdge) / unit
    if asSmall:
        return int( math.floor (edge * 0.9) * sign )
    else:      
        return int( math.ceil  (edge * 1.2) * sign )


def calcHistRange (chain, drawString, cuts = ''):
    """Calculates a 'pretty' range for a given draw command"""
    if cuts:
        chain.Draw (drawString, cuts)
    else:
        chain.Draw (drawString, cuts)
    htemp   = ROOT.MakeNullPointer( ROOT.TH1F )
    htemp   = chain.GetHistogram()
    entries = htemp.GetEntries()
    bins    = htemp.GetNbinsX()
    lowerBin = bins
    for bin in range (1, bins):
        percent = htemp.Integral (1, bin) / entries
        if percent > 0.025:
            lowerBin = bin 
            break
    upperBin = 1
    for bin in range (bins, 1, -1):
        percent = htemp.Integral (bin, bins) / entries
        if percent > 0.025:
            upperBin = bin
            break
    lowerEdge = htemp.GetBinLowEdge (lowerBin)
    upperEdge = htemp.GetBinLowEdge (upperBin) + htemp.GetBinWidth (upperBin)
    if upperBin <= 2:
        variable = ggRE.sub ('', drawString)
        cut = "%s < %f" % (variable, upperEdge)
        return calcHistRange (chain, drawString, cut)
    lowAsSmall = False
    uppAsSmall = False
    if lowerEdge > 0:
        lowAsSmall = True
    elif upperEdge < 0:
        uppAsSmall = True
    maxValue = max( abs(lowerEdge), abs(upperEdge))
    unit = 10 ** math.floor( math.log10( maxValue ) )    
    lowUnit = prettyUnit (lowerEdge, unit, lowAsSmall)
    uppUnit = prettyUnit (upperEdge, unit, uppAsSmall)
    diff = lowUnit - uppUnit
    bins = int (math.ceil (100 / diff)) * diff
    return bins, lowUnit * unit, uppUnit * unit


if __name__ == "__main__":
    # Setup options parser
    parser = optparse.OptionParser \
             ("usage: %prog [options] input.root output.root" \
              "Plots pull distributions, etc.")
    parser.add_option ('--output', dest = 'output', type='string',
                       default='images/',
                       help='output name for plots');
    parser.add_option ("--eps", dest='eps',
                       action='store_true', default=False,
                       help="Create eps files")
    parser.add_option ("--prelim", dest='pre',
                       action='store_true', default=False,
                       help="Add CDF Run II Preliminary 2.7 fb-1 to the plots")
    options, args = parser.parse_args()
    ROOT.gROOT.SetStyle ('Plain')
    ROOT.gROOT.SetBatch()
    ROOT.gStyle.SetOptFit (1111)
    files = []
    for arg in args:
        files.append (arg)
    rootfile = ROOT.TFile.Open (files[0])
    namesVec    = ROOT.MakeNullPointer( ROOT.vector("string") )
    bscNamesVec = ROOT.MakeNullPointer( ROOT.vector("string") ) 
    rootfile.GetObject ("names", namesVec)
    try:
        rootfile.GetObject ("bscNames", bscNamesVec)
    except:
        bscNamesVec = []
    names = []
    bscNames = []
    for name in namesVec:
        names.append (name)
    for name in bscNamesVec:
        bscNames.append (name)
    print 'names', names, '\nbsc  ', bscNames
    plotVars = []
    for index, name in enumerate (names):
        plotVars.append( (name,"measVec[%d]" % index,
                          "genVec[%d]" % index, "errVec[%d]" % index) )
    for index, name in enumerate (bscNames):
        plotVars.append( (name,"measBsc[%d]" % index,
                          "genBsc[%d]" % index, "errBsc[%d]" % index) )
    c1 = ROOT.TCanvas('c1', 'c1', 600, 400)
    chain = ROOT.TChain ('PEtree')
    for rootfile in files:
        chain.AddFile (rootfile)

    for tup in plotVars:
        ## if tup[0] != 'sing':
        ##     continue
        ## #print tup
        print tup;
        stringDict = { 'name' : tup[0],
                       'meas' : tup[1],
                       'gen'  : tup[2],
                       'err'  : tup[3]}
        for plot in plotsList:
            plotName = stringDict['name'] + plot[0]
            axisLabel = axisDict[plot[0]]
            if stringDict['name'] == "Top":
                axisLabel += unitDict[plot[0]]
            print 'name', plotName
            drawString = plot[2] % stringDict
            if len (plot[1]):
                tup = plot[1]
            else:
                tup = calcHistRange (chain, drawString)
            drawString = plot[2] % stringDict + ">>" + plotName
            print drawString
            hist = ROOT.TH1F (plotName, plotName,
                              tup[0], tup[1], tup[2])
            hist.GetXaxis().SetTitle(axisLabel)
            hist.GetYaxis().SetTitle("# Events")
            chain.Draw (drawString)
            if len (plot) > 3:
                hist.Fit( plot[3] )
            if options.pre:
                # Add "CDF Preliminary" to the plot
                prelim = ROOT.TPaveText(0.18,0.93,0.59,0.98,"NDC");
                prelim.SetTextSize(0.04);
                prelim.SetFillColor(0);
                prelim.SetShadowColor(0);
                prelim.SetTextAlign(12);
                prelim.AddText("CDF Run II Preliminary 2.7 fb^{-1}");
                prelim.Draw();
            if options.eps:
                c1.Print ('%s%s.eps' % (options.output, plotName))
            else:
                c1.Print ('%s%s.png' % (options.output, plotName))
    print "Done."

    del chain
