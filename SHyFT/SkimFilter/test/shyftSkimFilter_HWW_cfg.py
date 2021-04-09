import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.muTrig = cms.string('HLT_IsoMu17_v4')
process.shyftAnalyzer.selectionDetails.eleTrig = cms.string('HLT_Ele17_SW_TighterEleIdIsol_L1R_v3')

process.shyftAnalyzer.selectionDetails.muPlusJets     = cms.bool( True )
process.shyftAnalyzer.selectionDetails.ePlusJets      = cms.bool( False )
process.shyftAnalyzer.selectionDetails.isHWW          = cms.bool( True )
process.shyftAnalyzer.selectionDetails.isLoose          = cms.bool( True )
        
process.shyftAnalyzer.selectionDetails.minJets = cms.int32(2)
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
#process.shyftAnalyzer.trigRequired = cms.untracked.bool(False)

process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0
process.shyftAnalyzer.selectionDetails.pvSelector.minNdof = cms.double(4.0)
process.shyftAnalyzer.selectionDetails.pvSelector.maxRho = cms.double(2.0)

process.shyftAnalyzer.selectionDetails.muPtMin        = cms.double( 20.0 )
process.shyftAnalyzer.selectionDetails.muEtaMax        = cms.double( 2.1 )

process.shyftAnalyzer.selectionDetails.muonIdTight.version = cms.string('SPRING11')
process.shyftAnalyzer.selectionDetails.muonIdTight.RelIso = cms.double(999.0)
process.shyftAnalyzer.selectionDetails.muonIdTight.TrkIso = cms.double(0.05)
process.shyftAnalyzer.selectionDetails.muonIdTight.Chi2 = cms.double(10.0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NValMuHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.NHits = cms.int32(11)
process.shyftAnalyzer.selectionDetails.muonIdTight.nPixelHits = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.nMatchedStations = cms.int32(1)
process.shyftAnalyzer.selectionDetails.muonIdTight.LepZ =  cms.double( 1.0 )
process.shyftAnalyzer.selectionDetails.muonIdTight.D0 =  cms.double( 0.02 )
process.shyftAnalyzer.selectionDetails.muonIdTight.RecalcFromBeamSpot = cms.bool( True  )

process.shyftAnalyzer.selectionDetails.eleEtMin        = cms.double( 20.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMax        = cms.double( 2.5 )
process.shyftAnalyzer.selectionDetails.electronIdTight.RelIso = cms.double( 0.1 )
process.shyftAnalyzer.selectionDetails.electronIdTight.D0  = cms.double( 0.02 )

process.shyftAnalyzer.selectionDetails.muonIdLoose.version = cms.string('SPRING11')
process.shyftAnalyzer.selectionDetails.muonIdLoose.RelIso = cms.double(999.0)
process.shyftAnalyzer.selectionDetails.muonIdLoose.TrkIso = cms.double(0.2)
process.shyftAnalyzer.selectionDetails.muPtMinLoose        = cms.double( 10.0 )
process.shyftAnalyzer.selectionDetails.muEtaMaxLoose        = cms.double( 2.5 )

process.shyftAnalyzer.selectionDetails.eleEtMinLoose        = cms.double( 15.0 )
process.shyftAnalyzer.selectionDetails.eleEtaMaxLoose        = cms.double( 2.5 )
process.shyftAnalyzer.selectionDetails.electronIdLoose.RelIso = cms.double( 0.2 )

#Synchronization exercise (selection v4): these options can be changed to get different rows of the table

################    CALO     ############################
#process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.0 )
#process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
#process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJets')
#process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETs')
#process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)


################    PF     ############################

process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(20.0)
process.shyftAnalyzer.selectionDetails.jetEtaMax = cms.double(2.4)
process.shyftAnalyzer.selectionDetails.eleJetDR          =cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.0 )

process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPF')

#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("Trigger")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring(">= 1 Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("== 1 Tight Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton, Mu Veto")
#process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("MET Cut")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Z Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Conversion Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Cosmic Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("VBF crosscheck")



process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(10)
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
    'dcap:///pnfs/cms/WAX/11/store/user/sboutle/GluGluToHToWWToLNuQQ_M-170_7TeV-powheg-pythia6/SQWAT_413_v1/4ac1ba3a100cbf183f4cddf26f688eb6/SQWaT_41x_1_1_gIe.root'
    #'dcap:///pnfs/cms/WAX/11/store/user/lnujj/PATtuples/sboutle/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/SQWaT_PAT_416_v4/bc68ae99685e6316adbf5579a3572f5c/SQWaT_PF2ATtuple_41X_9_1_9M9.root'
    
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




