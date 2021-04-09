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
process.patTaus.isoDeposits = cms.PSet()

# ------- Determine whether to use PV or Beam Spot for muon selection

process.patMuons.usePV = False
process.patMuonsPFlow.usePV = False

##########################   Adding JPT Info ########################

# tcMET
#from PhysicsTools.PatAlgos.tools.metTools import *
#addTcMET(process, 'TC')

# JPT
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load('RecoJets.Configuration.RecoJPTJets_cff')

#addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
#                  'AK5', 'JPT',
#                  doJTA        = True,
#                  doBTagging   = True,
#                  jetCorrLabel = ('AK5','JPT'),
#                  doType1MET   = False,
#                  doL1Cleaning = False,
#                  doL1Counters = False,                 
#                  genJetCollection = cms.InputTag("ak5GenJets"),
#                  doJetID      = True,
#                  jetIdLabel   = "ak5"
#                  )


# JMS leave off all selected 
# process.selectedPatJetsAK5JPT.cut = cms.string('pt > 15 & abs(eta) < 2.4')




# -------- Shrink the event content by cutting objects --------

# Selection
# JMS 38X
# Do no apply selection to jets
# we need uncorrected jets around to systematic studies
#process.selectedPatJetsPFlow.cut = cms.string("pt > 15")
#process.selectedPatJets.cut = cms.string("pt > 20")
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
# re-include flabor history
process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

# --- Only applicable to ttbar sample ----

## produce ttGenEvent
process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")


# let it run
process.p = cms.Path(
 process.makeGenEvt *
 process.patDefaultSequence* 
 getattr(process,"patPF2PATSequence"+postfix)*
 process.flavorHistorySeq *
 process.prunedGenParticles
 )

# process.source.fileNames = [
#     #'/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/4C4A0E8D-C946-DF11-BCAC-003048D437D2.root'
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0016/6E7C4631-9D47-DF11-96CE-003048C69288.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0011/A4121AB4-0747-DF11-8984-0030487F171B.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/F6D86292-CD46-DF11-B2FD-003048C69312.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/E8A4F09C-CC46-DF11-97FC-003048C6903E.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/E8707C6F-CF46-DF11-8EF9-0030487E54B7.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/D87D77D2-C946-DF11-AD67-0030487D5E81.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/B4E69892-CB46-DF11-A646-0030487E4ED5.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/B47C6690-C946-DF11-8BC0-003048C692FA.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/A629B3AE-C946-DF11-8D0C-0030487F1A4F.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/9252E716-C846-DF11-A3CD-0030487F9297.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/7CD93B42-CD46-DF11-B99C-003048C6931C.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/7C45948C-CD46-DF11-B2DF-0030487F932D.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/5EAC389A-D246-DF11-8165-0030487E55B9.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/4C4A0E8D-C946-DF11-BCAC-003048D437D2.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/36602C16-C846-DF11-B0E5-0030487D5D6B.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/18DBE360-C846-DF11-81EC-003048D4397E.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/1283FB2C-CF46-DF11-B494-0030487EB003.root',
#     '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/04B06630-C946-DF11-8BA8-003048C6903C.root'    
#   ]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('shyft_382_pat.root')
#process.out.fileName = cms.untracked.string('/uscms_data/d2/slaunwhj/datasets/synch_38X/shyft_382_pat.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
#process.maxEvents.input = 100
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
    'keep *_prunedGenParticles_*_*'
]

