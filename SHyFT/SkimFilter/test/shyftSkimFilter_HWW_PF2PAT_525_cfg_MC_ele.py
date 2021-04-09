import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.muTrig = cms.string('HLT_IsoMu24_eta2p1_v*')
process.shyftAnalyzer.selectionDetails.eleTrig = cms.string('HLT_Ele27_WP80_v*')

process.shyftAnalyzer.selectionDetails.muPlusJets     = cms.bool( False )
process.shyftAnalyzer.selectionDetails.ePlusJets      = cms.bool( True )
        
process.shyftAnalyzer.selectionDetails.minJets = cms.int32(2)
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
#process.shyftAnalyzer.trigRequired = cms.untracked.bool(False)

process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0
process.shyftAnalyzer.selectionDetails.pvSelector.minNdof = cms.double(4.0)
process.shyftAnalyzer.selectionDetails.pvSelector.maxRho = cms.double(2.0)

process.shyftAnalyzer.selectionDetails.muPtMin        = cms.double( 20.0 )
process.shyftAnalyzer.selectionDetails.muEtaMax        = cms.double( 2.1 )

process.shyftAnalyzer.selectionDetails.muonIdTight.version = cms.string('SPRING12')
process.shyftAnalyzer.selectionDetails.muonIdTight.Chi2 = cms.double(10.0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NValMuHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NHits = cms.int32(6)
process.shyftAnalyzer.selectionDetails.muonIdTight.nPixelHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.nMatchedStations = cms.int32(1)
process.shyftAnalyzer.selectionDetails.muonIdTight.LepZ =  cms.double( 0.5 )
process.shyftAnalyzer.selectionDetails.muonIdTight.D0 =  cms.double( 0.02 )
process.shyftAnalyzer.selectionDetails.muonIdTight.RecalcFromBeamSpot = cms.bool( False )

process.shyftAnalyzer.selectionDetails.eleEtMin        = cms.double( 20.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMax        = cms.double( 2.5 )

process.shyftAnalyzer.selectionDetails.muonIdLoose.version = cms.string('SPRING12')
process.shyftAnalyzer.selectionDetails.muPtMinLoose        = cms.double( 10.0 )
process.shyftAnalyzer.selectionDetails.muEtaMaxLoose        = cms.double( 2.5 )

process.shyftAnalyzer.selectionDetails.eleEtMinLoose        = cms.double( 15.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMaxLoose        = cms.double( 2.5 )

################    PF     ############################

process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(20.0)
process.shyftAnalyzer.selectionDetails.jetEtaMax = cms.double(2.4)
process.shyftAnalyzer.selectionDetails.eleJetDR          =cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.0 )

process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsPFlow')
process.shyftAnalyzer.selectionDetails.muonSrc = cms.InputTag('selectedPatMuonsPFlow')
process.shyftAnalyzer.selectionDetails.electronSrc = cms.InputTag('selectedPatElectronsPFlow')
process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPFlow')

#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("Trigger")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring(">= 1 Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("== 1 Tight Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton")
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
    'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_v2/ajkumar/WW_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_52X_Summer12_v2//290326670ba15ca0752d90668da7d2ec/pat_52x_test_100_1_eAu.root',
    'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_v2/ajkumar/WW_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_52X_Summer12_v2//290326670ba15ca0752d90668da7d2ec/pat_52x_test_102_1_DEr.root',
    'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_v2/ajkumar/WW_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_52X_Summer12_v2//290326670ba15ca0752d90668da7d2ec/pat_52x_test_105_1_ngX.root',
    'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_v2/ajkumar/WW_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_52X_Summer12_v2//290326670ba15ca0752d90668da7d2ec/pat_52x_test_101_1_TYw.root',
    'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PatTuples_8TeV_v2/ajkumar/WW_TuneZ2star_8TeV_pythia6_tauola/SQWaT_PAT_52X_Summer12_v2//290326670ba15ca0752d90668da7d2ec/pat_52x_test_108_1_LXA.root'
    
    ]

process.source = cms.Source("PoolSource",
			    skipEvents = cms.untracked.uint32(0),
			    fileNames = cms.untracked.vstring(fileNames)
			    )



process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       #fileName = cms.untracked.string("/uscms_data/d2/slaunwhj/scratch0/skim_fullSelection.root"),
                               fileName = cms.untracked.string("ljmet_skim.root") , 

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




