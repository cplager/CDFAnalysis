# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.GlobalTag.globaltag = cms.string( 'START41_V0::All' )
#for 42X :
#process.GlobalTag.globaltag = cms.string( 'START42_V12::All' )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

###**********
# load the PAT config  #tessa -from PF2PAT example
process.load("PhysicsTools.PatAlgos.patSequences_cff")
###**********


# Get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Spring10")

# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.patTriggerEvent.processName = cms.string( 'REDIGI311X' )
process.patTrigger.processName = cms.string( 'REDIGI311X' )


process.patMuons.usePV = False

#FastJet
# Get a list of good primary vertices, in 42x, these are DAF vertices
#in 42X will change midNdof cut to 7.0
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
	"PrimaryVertexObjectFilter",
	filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
	src=cms.InputTag('offlinePrimaryVertices')
	)


# ----------- Adding PF2PAT ------------

from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = True
getattr(process, "patMuons"+postfix).embedGenMatch = True

## # remove trigger matching for PF2PAT as that is currently broken
process.patPF2PATSequencePFlow.remove(process.patTriggerSequencePFlow)
process.patPF2PATSequencePFlow.remove(process.patTriggerEventPFlow)
process.patPF2PATSequencePFlow.remove(process.patTriggerPFlow)
process.patTaus.isoDeposits = cms.PSet()

#use different versions with PF2PAT on and off
#inputJetCorrLabel = ('AK5PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
#inputJetCorrLabelnoPF2PAT = ('AK5PF',['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
inputJetCorrLabelnoPF2PAT = ('AK5PF',['L1FastJet', 'L2Relative', 'L3Absolute'])
inputJetCorrLabel = ('AK5PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'])


#FastJet
process.pfPileUpPFlow.Enable = True
#I think the following line is only for 42X
#process.pfPileUpPFlow.checkClosestZVertex = cms.bool(False)
process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
process.pfJetsPFlow.doAreaFastjet = True
process.pfJetsPFlow.doRhoFastjet = False
process.patJetCorrFactorsPFlow.payload = inputJetCorrLabel[0]
process.patJetCorrFactorsPFlow.levels = inputJetCorrLabel[1]

## # ------- Determine whether to use PV or Beam Spot for muon selection

process.patMuons.usePV = False
process.patMuonsPFlow.usePV = False

#FastJet
# Compute the mean pt per unit area (rho) from the
# PFchs inputs
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsPFlow = kt4PFJets.clone(
                rParam = cms.double(0.6),
                src = cms.InputTag('pfNoElectron'+postfix),
                doAreaFastjet = cms.bool(True),
                doRhoFastjet = cms.bool(True),
                voronoiRfact = cms.double(0.9)
                )

process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")
process.patJetCorrFactorsPFlow.primaryVertices = cms.InputTag('goodOfflinePrimaryVertices')

process.kt6PFJetsforJets = kt4PFJets.clone(
                        rParam = cms.double(0.6),
                        doAreaFastjet = cms.bool(True),
                        doRhoFastjet = cms.bool(True),
                        voronoiRfact = cms.double(0.9)
                        )

## ##########################   Adding PFlow Info ######################## 
 # tcMET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')

#process.load("RecoJets.JetProducers.ak5PFJets_cfi")
 # PF from RECO and not using PF2PAT

addJetCollection(process,cms.InputTag('ak5PFJets'),
                  'AK5', 'PF',
                  doJTA        = True,
                  doBTagging   = True,
                  #jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
                  jetCorrLabel = inputJetCorrLabelnoPF2PAT,
                  doType1MET   = False,
                  doL1Cleaning = False,
                  doL1Counters = False,
                  genJetCollection = cms.InputTag("ak5GenJets"),
                  doJetID      = True,
                  jetIdLabel   = "ak5"
                 )

process.load('RecoJets.Configuration.RecoPFJets_cff')
process.patJetCorrFactorsAK5PF.rho = cms.InputTag("kt6PFJetsforJets", "rho")
#process.patJetCorrFactors.primaryVertices = cms.InputTag('goodOfflinePrimaryVertices')
process.patJetCorrFactorsAK5PF.primaryVertices = cms.InputTag('goodOfflinePrimaryVertices')


process.selectedPatJetsAK5PF.cut = cms.string("pt > 15")
process.selectedPatJets.cut = cms.string("pt > 15")
process.selectedPatMuons.cut = cms.string("pt > 5")
process.selectedPatElectrons.cut = cms.string("pt > 5")


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

#FastJet
process.patseq = cms.Sequence(
	process.goodOfflinePrimaryVertices*
	getattr(process,"patPF2PATSequence"+postfix)
	)


# let it run
process.p = cms.Path(
 #FastJet for PF2PAT
 process.patseq *
 process.kt6PFJetsforJets *
 process.ak5PFJets *
 process.patDefaultSequence *
 getattr(process,"patPF2PATSequence"+postfix)*
 #process.flavorHistorySeq *
 process.prunedGenParticles
 )


###********** //tessa this is from the PF2PAT example  
# top projections in PF2PAT:
getattr(process,"pfNoPileUp"+postfix).enable = True
getattr(process,"pfNoMuon"+postfix).enable = True 
getattr(process,"pfNoElectron"+postfix).enable = True 
getattr(process,"pfNoTau"+postfix).enable = False 
getattr(process,"pfNoJet"+postfix).enable = True

# verbose flags for the PF2PAT modules
getattr(process,"pfNoMuon"+postfix).verbose = False
###********** 

#FastJet
# Add the PV selector and KT6 producer to the sequence
getattr(process,"patPF2PATSequence"+postfix).replace(
	getattr(process,"pfNoElectron"+postfix),
	getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow
	)


process.source.fileNames = [
    '/store/mc/Spring11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0008/3CDF8681-5F4F-E011-9293-E0CB4E1A118A.root',
    '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0048/9037E764-715C-E011-B693-00A0D1EEA584.root',
    '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0047/F840AD8D-355B-E011-8FE3-0024E876635F.root',
    '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0047/F46BE1B5-395B-E011-B12F-0026B94E2831.root',
    '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/AODSIM/PU_S1_START311_V1G1-v1/0047/9CDCC9F1-065B-E011-9355-0015178C4C7C.root'
]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('SQWaT_PF2PATtuple_41X.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.default.limit = 1
process.MessageLogger.categories.append('AdaptiveVertexFitter')
process.MessageLogger.cerr.AdaptiveVertexFitter = cms.untracked.PSet(
               limit = cms.untracked.int32(1)
               )
process.MessageLogger.categories.append('NullTransverseMomentum')
process.MessageLogger.cerr.NullTransverseMomentum  = cms.untracked.PSet(
                   limit = cms.untracked.int32(1)
                                  )
# process all the events
process.maxEvents.input = 100
process.options.wantSummary = False
process.out.dropMetaData = cms.untracked.string("DROPPED")

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
###********** //tessa this is from the PF2PAT example
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   *patEventContentNoCleaning )
###*********

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
    'keep *_prunedGenParticles_*_*',
    'keep *_goodOfflinePrimaryVertices*_*_*',#FastJet
    'keep double_*PFlow*_*_PAT',	#FastJet
    'keep *_addPileupInfo_*_*'
#	'keep *_kludgedJPTJets_*_*'    
]

#_____ dump the full unrolled python config _____________________________

outfile = open('config.py','w') 
print >> outfile,process.dumpPython() 
outfile.close() 
