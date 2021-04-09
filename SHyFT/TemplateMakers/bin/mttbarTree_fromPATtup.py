import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')


#set up to run on PAT tups
process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsPFlow')
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsPFlow')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPFlow')


#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(27.0)

#Set the PU reweighting
process.shyftAnalysis.pileupWeightsFile = cms.string("PileupDist_SQWaT_Muon_2012_3D.root") 

#Now, unpack the command line arguments for this script
#listFileName = sys.argv[2]
#iJob = int(sys.argv[3])
#nJobs = int(sys.argv[4])
label = 'ttbar'
jesShift = 'none'
jerShift = 0.0


#Set the JES and JER shifts based on commmand line
#process.shyftAnalysis.fancyJES = cms.string(jesShift)
#process.shyftAnalysis.jerFactor = cms.double(jerShift)

#btag requirement
#btag -1 = pre-tag, 1= 1 btag, 0 = 0 btags
process.shyftAnalysis.btag = cms.double(1)  

#Determine which input files to run on

process.inputs = cms.PSet (
       fileNames = cms.vstring(
'file:/store/user/lannon/TTJets_TuneZ2_7TeV-madgraph-tauola/SQWaT_PAT_42X_Fall11_v3/3156aed375cb9d84362f1cc2ea98a9c4/pat_42x_fall11_1000_1_e9e.root'
   )
       )


process.outputs = cms.PSet (
    outputName = cms.string('mttbarTree_ttbar_NoCuts.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(100)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
process.shyftAnalysis.maxJets = 100
