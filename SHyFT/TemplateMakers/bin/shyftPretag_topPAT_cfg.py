import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


#process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')
process.load('SHyFT.TemplateMakers.sqwat_PF2PAT_cfi')

#turn WPJets Sel off
process.shyftAnalysis.skipSelection = cms.bool(True)

#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(27.0)

#PFIso minimum cut
process.shyftAnalysis.PFIsoMin =  cms.double(0.0)

#btag requirement
#btag -1 = pre-tag, 1= 1 btag, 0 = 0 btags
process.shyftAnalysis.btag = cms.double(1)

#Trigger Cut  use 'HLT*' for no trigger cut
process.shyftAnalysis.muTrig = cms.string('HLT*')

#Set the PU reweighting
process.shyftAnalysis.pileupWeightsFile = cms.string("PileupDist_SQWaT_Muon_2012_3D.root") 

##JES
process.shyftAnalysis.JECUncertaintyFile = cms.string('JEC11_V12_AK5PF_UncertaintySources.txt');

##JES
process.shyftAnalysis.JES = cms.string('none')
   
process.inputs = cms.PSet (
    fileNames = cms.vstring(

'file:skim_52X_PF2PAT.root'

    )
)

process.outputs = cms.PSet (
	outputName = cms.string('test.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(0)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
