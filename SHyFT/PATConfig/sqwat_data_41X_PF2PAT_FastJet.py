# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.GlobalTag.globaltag = cms.string( 'GR_R_41_V0::All' )
#for 42X :
#process.GlobalTag.globaltag = cms.string( 'GR_R_42_V12::All' )

#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# Get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Spring10")


process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.25)
                                    )

process.load('RecoEgamma.ElectronIdentification.electronIdSequence_cff')

# JMS load noise filter
# add noise filter to sequence below

process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

# add the trigger information to the configuration
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

process.patMuons.usePV = False

# turn off MC matching for the process
removeMCMatching(process, ['All'])

#FastJet
# Get a list of good primary vertices, in 42x, these are DAF vertices
from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
	"PrimaryVertexObjectFilter",
	filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
	src=cms.InputTag('offlinePrimaryVertices')
	)

###**********
# load the PAT config  #tessa -from PF2PAT example
process.load("PhysicsTools.PatAlgos.patSequences_cff")
###**********

# ----------- Adding PF2PAT ------------

from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = True
getattr(process, "patMuons"+postfix).embedGenMatch = True

## # remove trigger matching for PF2PAT as that is currently broken
#process.patPF2PATSequencePFlow.remove(process.patTriggerSequencePFlow)
#process.patPF2PATSequencePFlow.remove(process.patTriggerEventPFlow)
#process.patPF2PATSequencePFlow.remove(process.patTriggerPFlow)
process.patTaus.isoDeposits = cms.PSet()

#use different versions with PF2PAT on and off
inputJetCorrLabel = ('AK5PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
inputJetCorrLabelnoPF2PAT = ('AK5PF',['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])

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

#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.ak5PFL1Offset.useCondDB = False

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

########### Also, add in some simple selection filtering
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.cutsToIgnore = cms.vstring('PV','>= 1 Lepton','== 1 Tight Lepton','== 1 Tight Lepton, Mu Veto','== 1 Lepton','MET Cut','Z Veto','Conversion Veto','Cosmic Veto','>=1 Jets','>=2 Jets','>=3 Jets','>=4 Jets','>=5 Jets')
process.shyftAnalyzer.selectionDetails.muTrig = cms.string('HLT_Mu15_v2')

#FastJet
process.patseq = cms.Sequence(
    process.scrapingVeto*
    process.HBHENoiseFilter*    
    process.goodOfflinePrimaryVertices*
    getattr(process,"patPF2PATSequence"+postfix)
    )


# let it run
process.p = cms.Path(
 #FastJet for PF2PAT
 process.patseq *
 process.kt6PFJetsforJets *
####???? process.kt6PFJets *
 process.ak5PFJets *
 process.patDefaultSequence *
 ###loose isolation
 getattr(process,"patPF2PATSequence"+postfix)*
 process.shyftAnalyzer
 #process.looseLeptonSequence *
 ###loose isolation
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
    #'file:///uscms_data/d2/tessa/TTbar_Spring11_MC_DataSet/3CDF8681-5F4F-E011-9293-E0CB4E1A118A.root'
    #'file:///uscms_data/d2/tessa/QCD_Spring11MC_DataSet/26803E47-BC53-E011-873D-002618943933.root'
    #'file:////uscms_data/d2/tessa/WJets_Spring11MC_DataSet/A4ABC964-3155-E011-A680-003048678D86.root'
    #'file:///uscms_data/d1/tessa/TTbar_Spring11_MC_DataSet/3CDF8681-5F4F-E011-9293-E0CB4E1A118A.root'
    #'/store/mc/Spring11/TTJets_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0008/8890F217-334F-E011-BABA-0022198F5B49.root'
    '/store/data/Run2011A/SingleMu/AOD/PromptReco-v1/000/161/312/EE67057F-DC57-E011-866E-001617E30D0A.root'
]


# rename output file
# JMS update name to reflect 38X
process.out.fileName = cms.untracked.string('SQWaT_PF2ATtuple_41X.root')

# reduce verbosity
# JMS more suppression 10 -> 1000
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.default.limit = 1
process.MessageLogger.categories.append('AdaptiveVertexFitter')
process.MessageLogger.categories.append('NullTransverseMomentum')
process.MessageLogger.categories.append('HLTPrescaleRecorder')
process.MessageLogger.categories.append('hltPrescaleTable')
process.MessageLogger.categories.append('errorL1AlgoDecision')
process.MessageLogger.categories.append('hltPrescaleSet')

process.MessageLogger.cerr.AdaptiveVertexFitter = cms.untracked.PSet(
               limit = cms.untracked.int32(1)
               )
process.MessageLogger.cerr.NullTransverseMomentum  = cms.untracked.PSet(
                   limit = cms.untracked.int32(1)
                                  )
process.MessageLogger.cerr.HLTPrescaleRecorder = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
                        )

process.MessageLogger.cerr.hltPrescaleTable = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
                        )
process.MessageLogger.cerr.errorL1AlgoDecision = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
                        )

process.MessageLogger.cerr.hltPrescaleSet = cms.untracked.PSet(
            limit = cms.untracked.int32(10)
                        )


# process all the events
process.maxEvents.input = 1000
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
    'keep *_goodOfflinePrimaryVertices*_*_*',#FastJet
    'keep double_*PFlow*_*_PAT',	#FastJet
    'keep *_addPileupInfo_*_*'
#	'keep *_kludgedJPTJets_*_*'    
]

#_____ dump the full unrolled python config _____________________________

outfile = open('config_PF2PAT_FastJet.py','w') 
print >> outfile,process.dumpPython() 
outfile.close() 
