#! /usr/bin/env python

import optparse
import ROOT
import sys
import re

allowed = [ re.compile (r'secvtxMass', re.IGNORECASE) ]

lumFactor = 1. / 0.837
topXsec   = 0.1575

factorList = []

fixList = [ (re.compile (r'^data'),      'Data'),
            (re.compile (r'secvtxMass'), 'svm' )]

addList = [ ('SingleTop', ('SingleTopSChan',
                           'SingleTopTChan',
                           'SingleTopTWChan') ) ]

def updateNumbers (options):
    global lumFactor, factorList
    lumFactor = 1 / options.lumi
    factorList = [ (re.compile (r'^W'), lumFactor),
                   (re.compile (r'^Q'), 2 * lumFactor),
                   (re.compile (r'^T'), lumFactor / topXsec),
                   ]
    

def copyAndModify (oldHist, newName):
    bins    = oldHist.GetNbinsX() / 2
    maximum = oldHist.GetXaxis().GetXmax() / 2
    newHist = ROOT.TH1F (newName, newName, bins, 0, maximum)
    for binIndex in range (0, bins): # 0 to bins - 1
        newHist.SetBinContent( binIndex, hist.GetBinContent (binIndex) )
    total = 0
    for binIndex in range (bins, 2 * bins + 2): # bi(ns to 2 * bins + 1
        total += hist.GetBinContent (binIndex)
    newHist.SetBinContent (bins, total)
    newHist.Rebin(2)
    return newHist


def addHistograms (tup, addHistsDict):
    base   = tup[0]
    others = tup[1]
    first  = others[0]
    keys   = addHistDict.get(first,{}).keys()
    retval = []
    for ending in keys:
        hist = None
        newName = base + ending
        problem = False
        for source in others:
            sourceHist = addHistDict.get(source,{}).get(ending)
            if not sourceHist:
                print "Warning: Can't find '%s,%s'" % (source, ending)
                problem = True
                break
            if hist:
                hist.Add (sourceHist)
            else:
                hist = sourceHist.Clone (newName)
        if problem:
            continue
        retval.append (hist)
    return retval
            
                
if __name__ == "__main__":
    # Setup options parser
    parser = optparse.OptionParser \
             ("usage: %prog [options] input.root output.root")
    parser.add_option ('--lumi', dest='lumi', type='float', default=2.89857,
                       help='integrated luminosity assumed for input files')
    options, args = parser.parse_args()
    if len (args) != 2:
        parser.print_usage()
        sys.exit()
    updateNumbers (options)

    # get list of regex of histograms not to save directly
    dontSaveList = []
    for piece in addList:
        for names in piece[1]:
            dontSaveList.append( re.compile( '^(%s)(\S+)$' % names) )
    addHistDict = {}
    target = ROOT.TFile.Open (args[1], 'RECREATE')
    source = ROOT.TFile.Open (args[0])
    nextKey = ROOT.TIter( source.GetListOfKeys() )
    key = nextKey()                                 
    while key:
        hist = key.ReadObj()
        if not isinstance (hist, ROOT.TH1):
            # not a histogram, don't bother
            key = nextKey()
            continue
        name = hist.GetName()
        found = False
        for regex in allowed:
            if regex.search (name):
                found = True
                break
        if not found:
            key = nextKey()
            continue
        scaled = False
        for regex, factor in factorList:
            if regex.search(name):
                hist.Scale (factor)
                print "%-25s - scaled by %f" % (name, factor)
                scaled = True
                break
        if not scaled:
            print "%-25s - not scaled" % name
        for regex, corrected in fixList:
            if regex.search (name):
                newname = regex.sub (corrected, name)
                #hist.SetName (newname)
                print "  changing %s to %s" % (name, newname)
                name = newname
        newHist = copyAndModify (hist, name)
        target.cd()
        found = False
        for regex in dontSaveList:
            match = regex.search (name)
            if match:
                found = True
                tempDict = addHistDict.setdefault (match.group(1), {})
                tempDict[match.group(2)] = newHist
                break
        if not found:
            newHist.Write()        
        key = nextKey()
    # add and write all histograms
    for tup in addList:
        hists = addHistograms (tup, addHistDict)
        for hist in hists:
            hist.Write()

    target.Close()
    source.Close()
        
