# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.GlobalTag.globaltag = cms.string('START311_V2::All')


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

## # ------- Determine whether to use PV or Beam Spot for muon selection

process.patMuons.usePV = False
process.patMuonsPFlow.usePV = False


## ### Loose Isolation stuff  ---- add back in later!!!!####################################
## process.pfIsolatedMuonsLoosePFlow = process.pfIsolatedMuonsPFlow.clone(
## 	    combinedIsolationCut = cms.double(999.0)
## 		    )

## process.patMuonsLoosePFlow = process.patMuonsPFlow.clone(
## 	    pfMuonSource = cms.InputTag("pfIsolatedMuonsLoosePFlow")
## 		    )
## adaptPFMuons( process, process.patMuonsLoosePFlow, "PFlow")
## process.muonMatchLoosePFlow = process.muonMatchPFlow.clone(
## 	    src = cms.InputTag("pfIsolatedMuonsLoosePFlow")
## 		    )
## process.muonMatchPFlow.src = "pfIsolatedMuonsPFlow"

## process.selectedPatMuonsLoosePFlow = process.selectedPatMuonsPFlow.clone(
## 	    src = cms.InputTag("patMuonsLoosePFlow")
## 		    )



## process.pfIsolatedElectronsLoosePFlow = process.pfIsolatedElectronsPFlow.clone(
## 	    combinedIsolationCut = cms.double(999.0)
## 		    )

## process.patElectronsLoosePFlow = process.patElectronsPFlow.clone(
## 	    pfElectronSource = cms.InputTag("pfIsolatedElectronsLoosePFlow")
## 		    )
## adaptPFElectrons( process, process.patElectronsLoosePFlow, "PFlow")

## process.selectedPatElectronsLoosePFlow = process.selectedPatElectronsPFlow.clone(
## 	    src = cms.InputTag("patElectronsLoosePFlow")
## 		    )


## process.looseLeptonSequence = cms.Sequence(
## 	    process.pfIsolatedMuonsLoosePFlow +
## 		    process.muonMatchLoosePFlow +
## 		    process.patMuonsLoosePFlow +
## 		    process.selectedPatMuonsLoosePFlow +
## 		    process.pfIsolatedElectronsLoosePFlow +
## 		    process.patElectronsLoosePFlow +
## 		    process.selectedPatElectronsLoosePFlow
## 		    )

#### End loose Isolation stuff ###############################################



## ##########################   Adding PFlow Info ########################

## switchJetCollection(process,cms.InputTag('ak5CaloJets'),
##                  doJTA        = True,
##                  doBTagging   = True,
##                  jetCorrLabel = ('AK5Calo', cms.vstring(['L2Relative', 'L3Absolute'])),
##                  doType1MET   = True,
##                  genJetCollection=cms.InputTag("ak5GenJets"),
##                  doJetID      = True
##                  )

## # tcMET
## from PhysicsTools.PatAlgos.tools.metTools import *
## addPfMET(process, 'PF')

## process.load("RecoJets.JetProducers.ak5PFJets_cfi")

## # PF from RECO and not using PF2PAT
## addJetCollection(process,cms.InputTag('ak5PFJets'),
##                                                    'AK5', 'PF',
##                                                    doJTA        = True,
##                                                    doBTagging   = True,
##                                                    jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute'])),
##                                                    doType1MET   = False,
##                                                    doL1Cleaning = False,
##                                                    doL1Counters = False,
##                                                    genJetCollection = cms.InputTag("ak5GenJets"),
##                                                    doJetID      = True,
##                                                    jetIdLabel   = "ak5"
##                                                    )




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
 #process.ak5PFJets *
 process.patDefaultSequence *
 ###loose isolation
  getattr(process,"patPF2PATSequence"+postfix)*
 # process.looseLeptonSequence *
 ###loose isolation
# process.flavorHistorySeq *
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
 

process.source.fileNames = [
'file:///uscms_data/d2/tessa/TTbar_Spring11_MC_DataSet/3CDF8681-5F4F-E011-9293-E0CB4E1A118A.root'
#		'file:///data/ndpc0/b/tpearson/SHyFT/Spring11_ttbar/3CDF8681-5F4F-E011-9293-E0CB4E1A118A.root'
#'/store/mc/Spring11/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0008/8890F217-334F-E011-BABA-0022198F5B49.root'
]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('SQWaT_PF2PAT_41X.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

# process all the events
process.maxEvents.input = -1
process.options.wantSummary = True
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
	'keep *_addPileupInfo_*_*'
#    'keep *_kludgedJPTJets_*_*'    
]

#_____ dump the full unrolled python config _____________________________

outfile = open('config_PF2PAT.py','w') 
print >> outfile,process.dumpPython() 
outfile.close() 
