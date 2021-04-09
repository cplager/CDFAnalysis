import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

# Change input tags for older version of PAT
# For MC only
#process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsStd')
#process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsStd')
#process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsStd')
#process.shyftAnalysis.metSrc = cms.InputTag('patMETsStd')
#process.shyftAnalysis.trigSrc = cms.InputTag('patTriggerEventStd')

process.shyftAnalysis.jetScale = cms.double(1.1)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_10_1_sq2.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_11_1_08B.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_12_1_cLF.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_13_1_GEx.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_14_1_NEJ.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_15_1_rpD.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_1_1_unI.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_2_1_463.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_3_1_fi3.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_4_1_fea.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_5_1_Qra.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_6_1_cqm.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_7_1_UcH.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_8_1_b47.root',
    'file:/data/ndpc2/b/sboutle/skims_362/ttbar/ljmet_skim_9_1_Odf.root',

)
)

process.outputs = cms.PSet (
    outputName = cms.string('ttbarHistos_jes110.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
