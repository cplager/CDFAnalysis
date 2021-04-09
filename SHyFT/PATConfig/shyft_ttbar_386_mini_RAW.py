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
#    'file:/data/ndpc0/b/slaunwhj/datasets/TTBar-38X-AOD/E8B2CA4D-42E7-DF11-988C-90E6BA442F16.root'
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0009/E8B2CA4D-42E7-DF11-988C-90E6BA442F16.root',
    'file:/data/ndpc0/c/slaunwhj/datasets/ggHWW/160/step2_RAW2DIGI_L1Reco_RECO_101_1_vbU.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0009/E847D402-12E7-DF11-97C5-003048D4EF1D.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0009/6EE559BE-11E7-DF11-B575-00145E5513C1.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/DC4963A1-E0E5-DF11-807E-00D0680BF898.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/D8F33E3F-58E5-DF11-9FCC-0026B9548CB5.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/B2D39D4C-63E6-DF11-8CFA-003048CEB070.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/B28EE7AE-48E5-DF11-9F45-001F29651428.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/9C7AD216-ACE5-DF11-BE50-001517255D36.root',
#     '/store/mc/Fall10/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/START38_V12-v2/0008/788BCB6C-ACE5-DF11-A13C-90E6BA442F1F.root',
#    '/store/mc/Fall10/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/AODSIM/START38_V12-v1/0017/D62F2154-32CB-DF11-B985-003048678FC6.root' 

]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('SQWaT_386_RAW_PAT.root')

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

process.load('Configuration.EventContent.EventContent_cff')

print "     Looping over RAW drop/keep commands  "

for iStatement in process.RAWEventContent.outputCommands:
    print "Current command is %s"  % iStatement
    stringCommand = "%s" % iStatement
    if (iStatement != 'drop *'):
        print "Keeping this command!"
        process.out.outputCommands.extend([stringCommand])
    else:
        print "Not using this command!"






#_____ dump the full unrolled python config _____________________________

outfile = open('config.py','w') 
print >> outfile,process.dumpPython() 
outfile.close() 
