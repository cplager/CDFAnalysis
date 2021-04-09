
import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

process.shyftAnalysis.cutsToIgnore = cms.vstring("Trigger")

process.shyftAnalysis.muPlusJets     = cms.bool( True )
process.shyftAnalysis.ePlusJets      = cms.bool( False )
process.shyftAnalysis.isHWW          = cms.bool( True )
process.shyftAnalysis.isLoose          = cms.bool( True )

#process.shyftAnalysis.muTrig = cms.string('HLT_IsoMu17_v4')
#process.shyftAnalysis.eleTrig = cms.string('HLT_Ele17_SW_TighterEleIdIsol_L1R_v3')

process.shyftAnalysis.eleJetDR= cms.double( 0.5 )
process.shyftAnalysis.muJetDRJets=cms.double( 0.5 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.0 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
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

    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/SQWaT_pat416_eleskim_v5/61f8461568baf8d521244f510560bb17/ljmet_skim_11_1_eTA.root',
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/SQWaT_pat416_eleskim_v5/61f8461568baf8d521244f510560bb17/ljmet_skim_10_1_tMY.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/TTJets_TuneD6T_7TeV-madgraph-tauola/SQWaT_pat416_Muskim_v1/f84896de6281eb3c58af2b9d1fc8a5a2/ljmet_skim_3_1_TR1.root',
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/TTJets_TuneD6T_7TeV-madgraph-tauola/SQWaT_pat416_Muskim_v1/f84896de6281eb3c58af2b9d1fc8a5a2/ljmet_skim_2_1_PVD.root',
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/TTJets_TuneD6T_7TeV-madgraph-tauola/SQWaT_pat416_Muskim_v1/f84896de6281eb3c58af2b9d1fc8a5a2/ljmet_skim_1_1_HZK.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_pat416_Muskim_v12/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_9_1_uVY.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/goodell/Mu/pat416_skim_Mu2010Bmu15_tagp/5fde07adf7cdf191ec588f280b4757ab/data_Mu2010B_tag_9_1_0HU.root'

'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_10_1_KNo.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_1_1_q0z.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_2_1_C7L.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_3_1_PzN.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_4_1_CfU.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_5_1_Lty.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_6_1_qtD.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_7_1_6Dv.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_8_1_wxs.root',
'dcap:///pnfs/cms/WAX/11/store/user/lwming/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/SQWaT_416_singleTop_t_muskim_v4/8020d647196527f8af9e3cfca2e4f43b/ljmet_skim_9_1_19Y.root'
)
)

process.outputs = cms.PSet (
    outputName = cms.string('sqwatAnatree_singleTop_t_muskim.root')
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
