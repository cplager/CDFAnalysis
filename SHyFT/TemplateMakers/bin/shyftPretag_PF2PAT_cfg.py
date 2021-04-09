import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.sqwat_PF2PAT_cfi')

###########  PF2PAT  configs ######################## 
process.shyftAnalysis.muJetDRJets=cms.double(0.0)
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsPF')
process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsPF')
process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsPF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0
process.shyftAnalysis.pvSelector.pvSrc = cms.InputTag('goodOfflinePrimaryVertices')

#top group pfRelIso cuts
process.shyftAnalysis.muonIdLoose.RelIso = cms.double(0.2)
process.shyftAnalysis.electronIdLoose.RelIso = cms.double(0.2)
process.shyftAnalysis.muonIdTight.RelIso = cms.double(0.125)

process.shyftAnalysis.muTrig = cms.string('HLT_IsoMu17_v.*')

   
process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'file:skim_42X_PF2PAT.root'
    )
)

process.outputs = cms.PSet (
       outputName = cms.string('scratch.root')
)
#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(10000)

process.shyftAnalysis.minJets = 0
