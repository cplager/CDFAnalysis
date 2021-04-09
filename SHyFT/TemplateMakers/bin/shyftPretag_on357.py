import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

#Change input tags for older version of PAT
process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsStd')
process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsStd')
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsStd')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsStd')
process.shyftAnalysis.trigSrc = cms.InputTag('patTriggerEventStd')


process.inputs = cms.PSet (
    fileNames = cms.vstring(
    ['dcap:///pnfs/cms/WAX/11/store/user/lannon/TTbarJets-madgraph/SHYFT_pat357_skim/0aa0bf8c0747525f693ade731a97f67d/ljmet_skim_8_1.root'
    ])
)

process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag_on357.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(2000)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1)
