import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.minJets = cms.int32(0)

process.shyftAnalyzer.selectionDetails.muonIdTight.cutsToIgnore.append("RelIso")
process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring("== 1 Tight Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Tight Lepton, Mu Veto")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("== 1 Lepton")
process.shyftAnalyzer.selectionDetails.cutsToIgnore.append("Z Veto")

process.shyftAnalyzer.verbose = cms.untracked.bool(False)
process.shyftAnalyzer.trigRequired = cms.untracked.bool(True)

#Synchronization exercise (selection v4): these options can be changed to get different rows of the table

################    CALO     ############################
#process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.0 )
#process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
#process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJets')
#process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETs')
#process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)

################    PF 30     ############################
process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.0 )
process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.0 )
process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)



################    PF2PAT     ############################
#process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.1 )
#process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
#process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsPFlow')
#process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPFlow')
#process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)


process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
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
    'file:/data/ndpc2/b/sboutle/syncEx_selv4/shyft_382_alljtypes.root'    
    
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
                   #fileName = cms.untracked.string("/data/ndpc0/b/slaunwhj/scratch0/skim_fullSelection_thinJets.root"),
			       fileName = cms.untracked.string("skim_fullSelectionPF30_thinJets_noIso_noDrCuts_noVetos.root"),

			       # This is how you require a filter
			       # for the path
			       SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p1')
	)
			       )


process.selectedPatJetsSkim = cms.EDFilter("PATJetSelector", cut = cms.string('pt > 15'), src = cms.InputTag("selectedPatJetsAK5PF"))


process.out.outputCommands = ['drop *',
                              'keep *_flavorHistoryFilter_*_*',
                              'keep *_prunedGenParticles_*_*',
                              'keep *_decaySubset_*_*',
                              'keep *_initSubset_*_*',
                              'drop *_cleanPat*_*_*',
                              'keep *_selectedPat*_*_*',
                              'drop *_selectedPat*PFlow_*_*',                              
                              'drop *_selectedPatJetsAK5JPT_*_*',
                              'drop *_selectedPatJetsAK5PF_*_*',
                              'keep *_selectedPatJetsSkim_*_*',
                              'keep *_patMETs*_*_*',
                              'drop *_patMETsTC*_*_*',
                              'drop *_patMETsPFlow*_*_*',
                              'keep recoPFCandidates_particleFlow_*_*',
                              'keep *_offlineBeamSpot_*_*',
                              'keep *_offlinePrimaryVertices_*_*',
                              'keep recoTracks_generalTracks_*_*',
                              'drop patPFParticles_*_*_*',
                              'keep patTriggerObjects_patTrigger_*_*',
                              'keep patTriggerFilters_patTrigger_*_*',
                              'keep patTriggerPaths_patTrigger_*_*',
                              'keep patTriggerEvent_patTriggerEvent_*_*',
                              'keep *_cleanPatPhotonsTriggerMatch_*_*',
                              'keep *_cleanPatElectronsTriggerMatch_*_*',
                              'keep *_cleanPatMuonsTriggerMatch_*_*',
                              'keep *_cleanPatTausTriggerMatch_*_*',
                              'keep *_cleanPatJetsTriggerMatch_*_*',
                              'keep *_patMETsTriggerMatch_*_*',
                              'drop *_MEtoEDMConverter_*_*',
                              'keep *_prunedGenParticles_*_*',
                              'keep *_kludgedJPTJets_*_*'    ]

    


process.p1 = cms.Path(
	process.shyftAnalyzer
    * process.selectedPatJetsSkim
	)

process.outpath = cms.EndPath(process.out)




