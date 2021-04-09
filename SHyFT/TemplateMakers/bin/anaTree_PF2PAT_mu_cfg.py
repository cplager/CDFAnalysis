import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

#process.shyftAnalysis.cutsToIgnore = cms.vstring("Trigger")
#process.shyftAnalysis.cutsToIgnore  = cms.vstring(">=2 Jets")
#process.shyftAnalysis.cutsToIgnore.append(">= 1 Tight Lepton")
#process.shyftAnalysis.cutsToIgnore.append("== 1 Tight Lepton")
#process.shyftAnalysis.cutsToIgnore.append("== 1 Tight Lepton, Mu Veto")

process.shyftAnalysis.muPlusJets     = cms.bool( True )
process.shyftAnalysis.ePlusJets      = cms.bool( False )
process.shyftAnalysis.isHWW          = cms.bool( True )
process.shyftAnalysis.isLoose        = cms.bool( True )

process.shyftAnalysis.fancyJES       = cms.string('none')

process.shyftAnalysis.muTrig = cms.string('HLT_IsoMu17_v.*')
#process.shyftAnalysis.eleTrig = cms.string('HLT_Ele17_SW_TighterEleIdIsol_L1R_v3')
process.shyftAnalysis.eleTrig = cms.string('HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2')

process.shyftAnalysis.eleJetDR= cms.double( 0.3 )
process.shyftAnalysis.muJetDRJets=cms.double( 0.3 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.0 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsPF')
process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsPF')
process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsPF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(20.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0

process.shyftAnalysis.muonIdTight.version = cms.string('SPRING11')
process.shyftAnalysis.muonIdTight.RelIso = cms.double(999.0)
process.shyftAnalysis.muonIdTight.TrkIso = cms.double(0.05)
process.shyftAnalysis.muonIdTight.Chi2 = cms.double(10.0)
process.shyftAnalysis.muonIdTight.NValMuHits = cms.int32(0)
process.shyftAnalysis.muonIdTight.NHits = cms.int32(11)
process.shyftAnalysis.muonIdTight.nPixelHits = cms.int32(0)
process.shyftAnalysis.muonIdTight.nMatchedStations = cms.int32(1)
process.shyftAnalysis.muonIdTight.LepZ =  cms.double( 1.0 )
process.shyftAnalysis.muonIdTight.D0 =  cms.double( 0.02 )
process.shyftAnalysis.muonIdTight.RecalcFromBeamSpot = cms.bool( True  )
process.shyftAnalysis.eleEtMin        = cms.double( 20.0 )
process.shyftAnalysis.eleEtaMax        = cms.double( 2.5 )
process.shyftAnalysis.electronIdTight.RelIso = cms.double( 0.1 )
process.shyftAnalysis.electronIdTight.D0  = cms.double( 0.02 )
process.shyftAnalysis.eleEtMinLoose        = cms.double( 15.0 )
process.shyftAnalysis.eleEtaMaxLoose        = cms.double( 2.5 )
process.shyftAnalysis.electronIdLoose.RelIso = cms.double( 0.2 )
    
process.inputs = cms.PSet (
    fileNames = cms.vstring(

'

)
)

process.outputs = cms.PSet (
    outputName = cms.string('anaTree_ttbar_SUMMER_PF2PAT_PURW.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(100)

process.shyftAnalysis.minJets = 2
