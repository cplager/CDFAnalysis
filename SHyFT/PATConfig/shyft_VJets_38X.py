# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# Get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Spring10")

# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

# switch to 8e29 menu. Note this is needed to match SD production
process.patTriggerEvent.processName = cms.string( 'REDIGI' )
process.patTrigger.processName = cms.string( 'REDIGI' )

# ----------- Adding PF2PAT ------------

from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = True
getattr(process, "patMuons"+postfix).embedGenMatch = True

# remove trigger matching for PF2PAT as that is currently broken
process.patPF2PATSequencePFlow.remove(process.patTriggerSequencePFlow)
process.patPF2PATSequencePFlow.remove(process.patTriggerEventPFlow)
process.patPF2PATSequencePFlow.remove(process.patTriggerPFlow)
process.patTaus.isoDeposits = cms.PSet()

# ------- Determine whether to use PV or Beam Spot for muon selection

process.patMuons.usePV = False
process.patMuonsPFlow.usePV = False
##########################   Adding PFlow Info ########################

# tcMET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

process.load("RecoJets.JetProducers.ak5PFJets_cfi")

# PF from RECO and not using PF2PAT
addJetCollection(process,cms.InputTag('ak5PFJets'),
                                                   'AK5', 'PF',
                                                   doJTA        = True,
                                                   doBTagging   = True,
                                                   jetCorrLabel = ('AK5','PF'),
                                                   doType1MET   = False,
                                                   doL1Cleaning = False,
                                                   doL1Counters = False,
                                                   genJetCollection = cms.InputTag("ak5GenJets"),
                                                   doJetID      = True,
                                                   jetIdLabel   = "ak5"
                                                   )

##########################   Adding JPT Info ########################

# tcMET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')

# JPT
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')

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
process.kludgedJPTJets = cms.EDProducer( 'KludgeJPTID',
                                         jetSrc = cms.InputTag( 'selectedPatJetsAK5JPT' )
                                         )




# -------- Shrink the event content by cutting objects --------

# Selection
# JMS 38X
# Do no apply selection to jets
# we need uncorrected jets around to systematic studies
#process.selectedPatJetsPFlow.cut = cms.string("pt > 1")
#process.selectedPatJetsAK5PF.cut = cms.string("pt > 1")
#process.selectedPatJets.cut = cms.string("pt > 15")
process.patJets.tagInfoSources = cms.VInputTag(
    cms.InputTag("secondaryVertexTagInfos")
    )
process.patJetsPFlow.tagInfoSources = cms.VInputTag(
    cms.InputTag("secondaryVertexTagInfos")
    )
process.selectedPatMuons.cut = cms.string("pt > 3")
process.selectedPatMuonsPFlow.cut = cms.string("pt > 3")
process.selectedPatElectrons.cut = cms.string("pt > 3")
process.selectedPatElectronsPFlow.cut = cms.string("pt > 3")


# -------- Common MC configuration section --------
# JMS 38X
# Global tag for MC
#process.GlobalTag.globaltag = cms.string('START36_V9::All')

process.GlobalTag.globaltag = cms.string('START38_V9::All')

# Re-run the gen jets
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *

# JMS 38X
# It looks like this runBlahOnBlah has not changed for 38X

run36xOn35xInput( process,
                  genJets = "ak5GenJets",
                  postfix="")

# Tuning of Monte Carlo matching 
# Also match with leptons of opposite charge
process.electronMatch.checkCharge = False
process.muonMatch.checkCharge = False

# prune gen particles

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("genParticles"),
  select = cms.vstring(
  "drop  *",
  "keep status = 3", #keeps all particles from the hard matrix element
  "+keep (abs(pdgId) = 11 | abs(pdgId) = 13) & status = 1" #keeps all stable muons and electrons and their (direct) mothers.
  )
)

# ---- Only applicable to samples with V+jets ----

# add the flavor history
# JMS 38X
# re-include flavor history
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

# --- Only applicable to ttbar sample ----

## produce ttGenEvent
#process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")


# let it run
process.p = cms.Path(
# process.makeGenEvt *
 process.recoJPTJets *
 process.ak5PFJets *
 process.patDefaultSequence * 
 getattr(process,"patPF2PATSequence"+postfix)*
 process.flavorHistorySeq *
 process.prunedGenParticles
 )

process.source.fileNames = [
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/4C4A0E8D-C946-DF11-BCAC-003048D437D2.root',
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/D87D77D2-C946-DF11-AD67-0030487D5E81.root',
    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/B47C6690-C946-DF11-8BC0-003048C692FA.root'
]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('shyft_384_pat.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = -1
process.options.wantSummary = True
process.out.dropMetaData = cms.untracked.string("DROPPED")

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
#process.out.outputCommands = patEventContentNoCleaning
#process.out.outputCommands += patExtraAodEventContent
#process.out.outputCommands += patTriggerEventContent

process.out.outputCommands = [
    'keep *_flavorHistoryFilter_*_*',
    'keep *_prunedGenParticles_*_*',
    'keep *_decaySubset_*_*',
    'keep *_initSubset_*_*',
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
    'drop *_MEtoEDMConverter_*_*',
    'keep *_prunedGenParticles_*_*',
    'keep *_kludgedJPTJets_*_*'    
]

