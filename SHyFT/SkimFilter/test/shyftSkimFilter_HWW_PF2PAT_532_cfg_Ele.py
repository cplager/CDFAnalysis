import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.muTrig = cms.string('HLT_IsoMu24_eta2p1_v*')
process.shyftAnalyzer.selectionDetails.eleTrig = cms.string('HLT_Ele27_WP80_v*')

process.shyftAnalyzer.selectionDetails.muPlusJets     = cms.bool( False )
process.shyftAnalyzer.selectionDetails.ePlusJets      = cms.bool( True )
process.shyftAnalyzer.selectionDetails.isHWW          = cms.bool( True )
process.shyftAnalyzer.selectionDetails.isLoose          = cms.bool( False )
        
process.shyftAnalyzer.selectionDetails.minJets = cms.int32(2)
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
#process.shyftAnalyzer.trigRequired = cms.untracked.bool(False)

process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0
process.shyftAnalyzer.selectionDetails.pvSelector.minNdof = cms.double(4.0)
process.shyftAnalyzer.selectionDetails.pvSelector.maxRho = cms.double(2.0)

process.shyftAnalyzer.selectionDetails.muPtMin        = cms.double( 24.0 )
process.shyftAnalyzer.selectionDetails.muEtaMax        = cms.double( 2.1 )

process.shyftAnalyzer.selectionDetails.muonIdTight.version = cms.string('SPRING12')
process.shyftAnalyzer.selectionDetails.muonIdTight.RelIso = cms.double(0.12)
process.shyftAnalyzer.selectionDetails.muonIdTight.TrkIso = cms.double(999.0)
process.shyftAnalyzer.selectionDetails.muonIdTight.Chi2 = cms.double(10.0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NValMuHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NHits = cms.int32(6)
process.shyftAnalyzer.selectionDetails.muonIdTight.nPixelHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.nMatchedStations = cms.int32(1)
process.shyftAnalyzer.selectionDetails.muonIdTight.LepZ =  cms.double( 5.0 )
process.shyftAnalyzer.selectionDetails.muonIdTight.D0 =  cms.double( 0.2 )
process.shyftAnalyzer.selectionDetails.muonIdTight.RecalcFromBeamSpot = cms.bool( True  )

process.shyftAnalyzer.selectionDetails.eleEtMin        = cms.double( 27.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMax        = cms.double( 2.5 )

process.shyftAnalyzer.selectionDetails.muonIdLoose.version = cms.string('SPRING12')
process.shyftAnalyzer.selectionDetails.muonIdLoose.RelIso = cms.double(0.2)
process.shyftAnalyzer.selectionDetails.muonIdLoose.TrkIso = cms.double(999.0)
process.shyftAnalyzer.selectionDetails.muPtMinLoose        = cms.double( 10.0 )
process.shyftAnalyzer.selectionDetails.muEtaMaxLoose        = cms.double( 2.5 )

process.shyftAnalyzer.selectionDetails.eleEtMinLoose        = cms.double( 15.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMaxLoose        = cms.double( 2.5 )

#Synchronization exercise (selection v4): these options can be changed to get different rows of the table

################    CALO     ############################
#process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.0 )
#process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
#process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJets')
#process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETs')
#process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)


################    PF     ############################

process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(25.0)
process.shyftAnalyzer.selectionDetails.jetEtaMax = cms.double(2.4)
process.shyftAnalyzer.selectionDetails.eleJetDR          =cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.0 )

process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsPFlow')
process.shyftAnalyzer.selectionDetails.muonSrc = cms.InputTag('selectedPatMuonsPFlow')
process.shyftAnalyzer.selectionDetails.electronSrc = cms.InputTag('selectedPatElectronsPFlow')
process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPFlow')

process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("Trigger")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring(">= 1 Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("== 1 Tight Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton, Mu Veto")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("MET Cut")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Z Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Conversion Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Cosmic Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("VBF crosscheck")



process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(10000)
	)

process.MessageLogger = cms.Service(
	"MessageLogger",
	cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING')
				  ),
	categories     = cms.untracked.vstring('SHyFTSkimFilter'),	
	destinations   = cms.untracked.vstring('cout')
	)


# Declare some default names 
fileNames = [
    #10K PATtuple at LPC:
    #'file:/uscms_data/d2/sboutle/synch_38/shyft_382_alljtypes.root'
    #10K PATtuple at UVAPC:
    #'file:/data2/sboutle/SHyFT/syncEx_v4/shyft_382_alljtypes.root'
    #10K PATtuple at NDPC:
    #'file:/data/ndpc2/b/sboutle/syncEx_selv4/shyft_382_alljtypes.root'
    #'file:/uscms_data/d2/sboutle/SQWaT_387_1_1_4mh.root'

    # one file from sync ex for ggHWW
    #'dcap:///pnfs/cms/WAX/11/store/user/sboutle/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/SQWAT_413_v1/4ac1ba3a100cbf183f4cddf26f688eb6/SQWaT_41x_1_1_gIe.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PATtuples/sboutle/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/SQWaT_PAT_416_v4/bc68ae99685e6316adbf5579a3572f5c/SQWaT_PF2ATtuple_41X_9_1_9M9.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/tpearson/SingleMu/data_2011A-MayReReco_IsoMu17_v4/759ece08e614ebb9c58afa3d28b251de/pat_42x_9_1_4zY.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PATtuples/sboutle/TTJets_TuneZ2_7TeV-madgraph-tauola/SQWaT_PATtup_42X_ttbar_v3/a183b3818a52c9ec4737e1cba5f2d537/pat_42x_9_1_j7m.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/abrinke1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_PATtup_42X_WJets/a183b3818a52c9ec4737e1cba5f2d537/pat_42x_9_1_IQx.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/lwming/TTJets_TuneZ2_7TeV-madgraph-tauola/SQWaT_elePATtup_42X_ttbar/7327facc0949c2a030127b26734f841e/pat_42x_9_1_h9M.root',
'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_53X/jdg6y/goodell/WZ_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_43X_Fall12_v1/829f288d768dd564418efaaf3a8ab9aa/pat_53x_test_v03_335_1_LwN.root',
'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_53X/jdg6y/goodell/WZ_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_43X_Fall12_v1/829f288d768dd564418efaaf3a8ab9aa/pat_53x_test_v03_288_1_ZIw.root',
'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_53X/jdg6y/goodell/WZ_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_43X_Fall12_v1/829f288d768dd564418efaaf3a8ab9aa/pat_53x_test_v03_322_1_04p.root'

    ]

process.source = cms.Source("PoolSource",
			    skipEvents = cms.untracked.uint32(0),
			    fileNames = cms.untracked.vstring(fileNames)
			    )



process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       #fileName = cms.untracked.string("/uscms_data/d2/slaunwhj/scratch0/skim_fullSelection.root',"),
                               fileName = cms.untracked.string("ljmet_skim_53xtest.root") , 

			       # This is how you require a filter
			       # for the path
			       SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p1')
	)
			       )


#process.out.outputCommands = [
#    


process.p1 = cms.Path(
	process.shyftAnalyzer
	)

process.outpath = cms.EndPath(process.out)




