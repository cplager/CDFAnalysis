import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.minJets = cms.int32(0)
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
process.shyftAnalyzer.trigRequired = cms.untracked.bool(True)
process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0

#Synchronization exercise (selection v4): these options can be changed to get different rows of the table

################    CALO     ############################
#process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.0 )
#process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
#process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJets')
#process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETs')
#process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)


################    PF     ############################
process.shyftAnalyzer.selectionDetails.muJetDRJets=cms.double( 0.1 )
process.shyftAnalyzer.selectionDetails.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalyzer.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalyzer.selectionDetails.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalyzer.selectionDetails.jetPtMin = cms.double(30.0)


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




