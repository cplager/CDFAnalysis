#! /usr/bin/env python

import optparse
import ROOT
import sys
import re

allowed = [ re.compile (r'secvtxMass', re.IGNORECASE) ]

lumFactor = 1. / 0.837
topXsec   = 0.1575

factorList = [ (re.compile (r'^W'), lumFactor),
               (re.compile (r'^Q'), 2 * lumFactor),
               (re.compile (r'^T'), lumFactor / topXsec),
               ]

fixList = [ (re.compile (r'^data'),      'Data'),
            (re.compile (r'secvtxMass'), 'svm' )]

def copyAndModify (oldHist, newName):
    bins    = oldHist.GetNbinsX() / 2
    maximum = oldHist.GetXaxis().GetXmax() / 2
    newHist = ROOT.TH1F (newName, newName, bins, 0, maximum)
    for binIndex in range (0, bins): # 0 to bins - 1
        newHist.SetBinContent( binIndex, hist.GetBinContent (binIndex) )
    total = 0
    for binIndex in range (bins, 2 * bins + 2): # bins to 2 * bins + 1
        total += hist.GetBinContent (binIndex)
    newHist.SetBinContent (bins, total)
    newHist.Rebin(2)
    return newHist

if __name__ == "__main__":
    # Setup options parser
    parser = optparse.OptionParser \
             ("usage: %prog [options] input.root output.root")
    options, args = parser.parse_args()
    ## if len (args) != 2:
    ##     parser.print_usage()
    ##     sys.exit()
    onej1tRE = re.compile (r'secvtxMass_1j_1t$')
    for filename in args:
        print filename
        source = ROOT.TFile.Open (filename) 
        nextKey = ROOT.TIter( source.GetListOfKeys() )
        key = nextKey()
        directories = []
        while key:
            tdir = key.ReadObj()
            if not isinstance (tdir, ROOT.TDirectory):
                key = nextKey()
                continue
            directories.append (tdir)
            key = nextKey()
        for tdir in directories:
            print "  %-20s : " % tdir.GetName(),
            nextKey = ROOT.TIter( tdir.GetListOfKeys() )
            key = nextKey()
            first = True
            while key:
                hist = key.ReadObj()
                if not isinstance (hist, ROOT.TH1):
                    # not a histogram, don't bother
                    key = nextKey()
                    continue
                name = hist.GetName()
                if onej1tRE.search (name):
                    if first:
                        print " %s " % name
                        first = False
                    else:
                        print " " * 26, name
                key = nextKey()
        source.Close()
    print "done"
    sys.exit()
            
       
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
        newHist.Write()        
        key = nextKey()

    target.Close()
    source.Close()
        
