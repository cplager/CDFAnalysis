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

# different jet colletions
process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.0 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

process.shyftAnalysis.jetScale = cms.double(1.0)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_10_1_fU2.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_12_1_65D.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_13_1_EWM.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_14_1_3co.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_15_1_Hgy.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_16_1_h0H.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_17_1_FFJ.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_18_1_StG.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_19_1_8XA.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_1_1_ouG.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_20_1_YaO.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_21_1_cy0.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_22_1_Lpy.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_2_1_cvI.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_3_1_xh3.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_4_1_65x.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_5_1_cen.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_6_1_iXC.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_7_1_KJr.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_8_1_alA.root',
'file:/data/ndpc1/c/slaunwhj/datasets/InclusiveMu15_NonIso/skim_fullSelection_9_1_u9w.root',
    
    
)
)

process.outputs = cms.PSet (
    outputName = cms.string('/data/ndpc1/c/slaunwhj/scratch0/anaTree_qcdNonIso_PF_Pt30.root')
)

# Turn off/on reliso cut
#process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
