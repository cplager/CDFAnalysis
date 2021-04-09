# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.GlobalTag.globaltag = cms.string('START38_V13::All')


process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# Get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Spring10")

# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent


process.patMuons.usePV = False

##########################   Adding PFlow Info ########################

switchJetCollection(process,cms.InputTag('ak5CaloJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5Calo', cms.vstring(['L2Relative', 'L3Absolute'])),
                 doType1MET   = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID      = True
                 )

# tcMET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

process.load("RecoJets.JetProducers.ak5PFJets_cfi")

# PF from RECO and not using PF2PAT
addJetCollection(process,cms.InputTag('ak5PFJets'),
                                                   'AK5', 'PF',
                                                   doJTA        = True,
                                                   doBTagging   = True,
                                                   jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
                                                   doType1MET   = False,
                                                   doL1Cleaning = False,
                                                   doL1Counters = False,
                                                   genJetCollection = cms.InputTag("ak5GenJets"),
                                                   doJetID      = True,
                                                   jetIdLabel   = "ak5"
                                                   )



# -------- Shrink the event content by cutting objects --------

# Selection
# Apply a hard cut on the jets to reduce output size

process.selectedPatJetsAK5PF.cut = cms.string("pt > 15")
process.selectedPatJets.cut = cms.string("pt > 15")
process.selectedPatMuons.cut = cms.string("pt > 3")
process.selectedPatElectrons.cut = cms.string("pt > 3")



# -------- Common MC configuration section --------

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
#  re-include flavor history
# process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

# --- Only applicable to ttbar sample ----

## produce ttGenEvent
#process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")


# let it run
process.p = cms.Path(
 process.ak5PFJets *
 process.patDefaultSequence * 
 process.prunedGenParticles
 )

process.source.fileNames = [

#    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/4C4A0E8D-C946-DF11-BCAC-003048D437D2.root',
#    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/D87D77D2-C946-DF11-AD67-0030487D5E81.root',
#    '/store/mc/Spring10/TTbarJets-madgraph/GEN-SIM-RECO/START3X_V26_S09-v1/0006/B47C6690-C946-DF11-8BC0-003048C692FA.root'
'/store/mc/Fall10/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0024/FE8F3659-A2E5-DF11-BF50-0024E8768D68.root'
]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('SQWaT_386_test_genevent.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = 10
process.options.wantSummary = True
process.out.dropMetaData = cms.untracked.string("DROPPED")

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
from PhysicsTools.PatAlgos.patEventContent_cff import patExtraAodEventContent
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
#process.out.outputCommands = patEventContentNoCleaning
#process.out.outputCommands += patExtraAodEventContent
#process.out.outputCommands += patTriggerEventContent

process.out.outputCommands = [
    'keep GenRunInfoProduct_generator_*_*',
    'keep GenEventInfoProduct_generator_*_*',
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
#    'keep *_kludgedJPTJets_*_*'    
]

#_____ dump the full unrolled python config _____________________________

outfile = open('config.py','w') 
print >> outfile,process.dumpPython() 
outfile.close() 
