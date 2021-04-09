# SHyFT configuration
from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.PatAlgos.tools.coreTools import *

## global tag for data (Note: Different from MC!!!)
## KPL: Updated Jul 29
## JMS: Updated for 38X on Sept 29
##  Updated to global tag for 383
## process.GlobalTag.globaltag = cms.string('GR_R_36X_V12::All')

process.GlobalTag.globaltag = cms.string('GR_R_38X_V13::All')

# get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Spring10")


# Update July 6
# Do not require physics declared
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
#process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# require scraping filter
# Update July 6
# threshold from 0.2 to 0.25
# this means a tighter cut
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.25)
                                    )

# Run b-tagging sequences on 35x inputs
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run36xOn35xInput(process)

# configure HLT
#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND NOT (36 OR 37 OR 38 OR 39)')

# switch on PAT trigger
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger( process )

# JMS July 6 Correct ele misalignment
# Add process.eIdSequence after the gsf electrons and before calling PAT
# JMS remmoved for 38X
# from RecoEgamma.EgammaTools.correctedElectronsProducer_cfi import *

process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')



# JMS load noise filter
# add noise filter to sequence below

process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')


#No point since we select on PV later and doesn't remove that much
#Also makes it harder to compare to other groups.
## process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
##                                            vertexCollection = cms.InputTag('offlinePrimaryVertices'),
##                                            minimumNDOF = cms.uint32(4) ,
##                                            maxAbsZ = cms.double(15), 
##                                            maxd0 = cms.double(2) 
##                                            )


from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=False, postfix=postfix) 

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = False
getattr(process, "patMuons"+postfix).embedGenMatch = False

# ------- Determine whether to use PV or Beam Spot for muon selection

process.patMuons.usePV = False
process.patMuonsPFlow.usePV = False


# turn off MC matching for the process
removeMCMatching(process, ['All'])


##########################   Adding JPT Info ########################

# tcMET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')

# JPT
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')

# Not sure why removing 
addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                  'AK5', 'JPT',
                  doJTA        = True,
                  doBTagging   = True,
                  jetCorrLabel = ('AK5','JPT'),
                  doType1MET   = False,
                  doL1Cleaning = False,
                  doL1Counters = False,                 
                  genJetCollection = cms.InputTag("ak5GenJets"),
                  doJetID      = True,
                  jetIdLabel   = "ak5"
                  )

# JMS leave off all selected 
# process.selectedPatJetsAK5JPT.cut = cms.string('pt > 15 & abs(eta) < 2.4')

# apply my kludge for JPT Jet ID
# this is loaded but never put into a path
process.kludgedJPTJets = cms.EDProducer( 'KludgeJPTID',
                                         jetSrc = cms.InputTag( 'selectedPatJetsAK5JPT' )
                                         )







# remove trigger matching for PF2PAT as that is currently broken
process.patPF2PATSequencePFlow.remove(process.patTriggerSequencePFlow)
process.patTaus.isoDeposits = cms.PSet()





# Set up the SV info?
process.patJets.tagInfoSources = cms.VInputTag(
    cms.InputTag("secondaryVertexTagInfos")
    )
process.patJetsPFlow.tagInfoSources = cms.VInputTag(
    cms.InputTag("secondaryVertexTagInfosAOD")
    )

# Is something need for JPT here?

process.patJetsAK5JPT.tagInfoSources = cms.VInputTag(
    cms.InputTag("secondaryVertexTagInfosAK5JPT")
    )



#KPL 29-Jul-2010:  Turn off filtering on objects since we're filtering on trigger paths now.
# Selection
## process.selectedPatJetsPFlow.cut = cms.string("pt > 15")
## process.selectedPatJets.cut = cms.string("pt > 20")
## process.selectedPatMuons.cut = cms.string("pt > 3")
## process.selectedPatMuonsPFlow.cut = cms.string("pt > 3")
## process.selectedPatElectrons.cut = cms.string("pt > 3")
## process.selectedPatElectronsPFlow.cut = cms.string("pt > 3")

## # FILTERS:
## # One of : >=1 mu (PF or std)
## #          >=1 e (PF or std)
## #          >=2 jet (PF or std)
## process.muonFilter = cms.EDFilter("CandViewCountFilter",
##                                   src = cms.InputTag("selectedPatMuons"),
##                                   minNumber = cms.uint32(1),
##                                   )

## process.pfMuonFilter = cms.EDFilter("CandViewCountFilter",
##                                     src = cms.InputTag("selectedPatMuonsPFlow"),
##                                     minNumber = cms.uint32(1),
##                                     )

## process.electronFilter = cms.EDFilter("CandViewCountFilter",
##                                   src = cms.InputTag("selectedPatElectrons"),
##                                   minNumber = cms.uint32(1),
##                                   )

## process.pfElectronFilter = cms.EDFilter("CandViewCountFilter",
##                                     src = cms.InputTag("selectedPatElectronsPFlow"),
##                                     minNumber = cms.uint32(1),
##                                     )

## process.jetFilter = cms.EDFilter("CandViewCountFilter",
##                                   src = cms.InputTag("selectedPatJets"),
##                                   minNumber = cms.uint32(2),
##                                   )

## process.pfJetFilter = cms.EDFilter("CandViewCountFilter",
##                                     src = cms.InputTag("selectedPatJetsPFlow"),
##                                     minNumber = cms.uint32(2),
##                                     )



# Add the files 
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [

    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/661FA9D6-BBC6-DF11-A0B0-0030487CD77E.root',
    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/40EDD56A-C6C6-DF11-936E-0030487C778E.root',
    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/22B2052B-DFC6-DF11-8D3F-0030487CBD0A.root',
    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/431/6CB7C6F1-ADC6-DF11-A96E-0030487C5CFA.root',
    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/430/D47EAE08-ADC6-DF11-B1D8-0030487CD6D8.root',
    '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/428/9A7CDCF5-AAC6-DF11-8BC3-001D09F276CF.root'    

    ] );

process.source.fileNames = readFiles

# let it run

#print
#print "============== Warning =============="
#print "technical trigger filter:    DISABLED"
#print "physics declare bit filter:  DISABLED"
#print "primary vertex filter:       DISABLED"

# no physics declared


# Also, add in some simple selection filtering
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring('PV','>= 1 Lepton','== 1 Tight Lepton','== 1 Tight Lepton, Mu Veto','== 1 Lepton','MET Cut','Z Veto','Conversion Veto','Cosmic Veto','>=1 Jets','>=2 Jets','>=3 Jets','>=4 Jets','>=5 Jets')


process.patseq = cms.Sequence(    
    process.scrapingVeto*
#    process.primaryVertexFilter*
    process.HBHENoiseFilter *	
#    process.eIdSequence *
    process.recoJPTJets *
    process.patDefaultSequence* 
    getattr(process,"patPF2PATSequence"+postfix) *
    process.shyftAnalyzer
    
)

process.p = cms.Path(process.patseq)

## process.p1 = cms.Path(
##     process.patseq*process.muonFilter
##     )

## process.p2 = cms.Path(
##     process.patseq*process.pfMuonFilter
##     )

## process.p3 = cms.Path(
##     process.patseq*process.electronFilter
##     )

## process.p4 = cms.Path(
##     process.patseq*process.pfElectronFilter
##     )

## process.p5 = cms.Path(
##     process.patseq*process.jetFilter
##     )

## process.p6 = cms.Path(
##     process.patseq*process.pfJetFilter
##     )

## # "or" the preceeding paths
## # process.out.SelectEvents.SelectEvents = cms.vstring('p1', 'p2', 'p3', 'p4', 'p5', 'p6')
## # Only keep muon events

## process.out.SelectEvents.SelectEvents = cms.vstring('p1', 'p2')


# rename output file
# standard file name
process.out.fileName = cms.untracked.string('shyft_362_justMuons_calo_pf_jpt.root')
#process.out.fileName = cms.untracked.string('/uscms_data/d2/slaunwhj/datasets/synch_38X/shyft_362_justMuons_calo_pf_jpt.root')

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.default.limit = 1000

process.MessageLogger.categories.append('HLTPrescaleRecorder')
process.MessageLogger.categories.append('hltPrescaleTable')
process.MessageLogger.categories.append('errorL1AlgoDecision')
process.MessageLogger.categories.append('hltPrescaleSet')

process.MessageLogger.cerr.HLTPrescaleRecorder = cms.untracked.PSet(
        limit = cms.untracked.int32(1000)
            )

process.MessageLogger.cerr.hltPrescaleTable = cms.untracked.PSet(
        limit = cms.untracked.int32(1000)
            )
process.MessageLogger.cerr.errorL1AlgoDecision = cms.untracked.PSet(
        limit = cms.untracked.int32(1000)
            )

process.MessageLogger.cerr.hltPrescaleSet = cms.untracked.PSet(
        limit = cms.untracked.int32(1000)
            )




# process all the events
#process.maxEvents.input = 10
process.options.wantSummary = True
process.out.dropMetaData = cms.untracked.string("DROPPED")

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
#process.out.outputCommands = patEventContentNoCleaning
#process.out.outputCommands += patExtraAodEventContent
#process.out.outputCommands += patTriggerEventContent
process.out.outputCommands = [
    'drop *_cleanPat*_*_*',
    'keep *_selectedPat*_*_*',
    'keep *_patMETs*_*_*',
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
    'keep *_hltGtDigis_*_*',
    'keep *_l1GtObjectMap_*_*',
    'keep *_l1extraParticles_*_*',   
    'keep *_ecalRecHit_EcalRecHitsEB_*',
    'drop *_MEtoEDMConverter_*_*',
    'keep *_kludgedJPTJets_*_*'    
    ]

