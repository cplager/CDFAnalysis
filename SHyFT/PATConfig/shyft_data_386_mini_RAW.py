# The SHyFT Configuration for PAT-tuples

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# -------- Common section for data and MC --------

process.GlobalTag.globaltag = cms.string('GR_R_38X_V14::All')

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


##########################   Adding PFlow Info ########################

switchJetCollection(process,cms.InputTag('ak5CaloJets'),
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('AK5Calo', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])),
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
                                                   jetCorrLabel = ('AK5PF', cms.vstring(['L2Relative', 'L3Absolute', 'L2L3Residual'])),
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


# ---- Only applicable to samples with V+jets ----

# add the flavor history
# JMS 38X
#  re-include flavor history
# process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

# --- Only applicable to ttbar sample ----

## produce ttGenEvent
#process.load("TopQuarkAnalysis.TopEventProducers.sequences.ttGenEvent_cff")




# Add the files 
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()

readFiles.extend( [

#     '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0010/8E202869-33EC-DF11-AD37-485B39800B8A.root',
#     '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0008/F0EE61DC-1AEB-DF11-B771-90E6BA19A1FD.root',
#     '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0008/C2BB9AEB-1AEB-DF11-AE19-E0CB4E29C4BE.root',
#     '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0008/B819F1CC-1BEB-DF11-B097-E0CB4E1A1147.root',
#     '/store/data/Run2010B/Mu/AOD/Nov4ReReco_v1/0008/B6A96268-1CEB-DF11-BFDE-485B39800B96.root'
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/661FA9D6-BBC6-DF11-A0B0-0030487CD77E.root',
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/40EDD56A-C6C6-DF11-936E-0030487C778E.root',
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/436/22B2052B-DFC6-DF11-8D3F-0030487CBD0A.root',
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/431/6CB7C6F1-ADC6-DF11-A96E-0030487C5CFA.root',
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/430/D47EAE08-ADC6-DF11-B1D8-0030487CD6D8.root',
##     '/store/data/Run2010B/Mu/RECO/PromptReco-v2/000/146/428/9A7CDCF5-AAC6-DF11-8BC3-001D09F276CF.root'

        'file:/data/ndpc0/c/slaunwhj/datasets/ggHWW/160/step2_RAW2DIGI_L1Reco_RECO_101_1_vbU.root',

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
    process.ak5PFJets *
    process.scrapingVeto*
    process.HBHENoiseFilter *	
    process.patDefaultSequence* 
    process.shyftAnalyzer
    
)

process.p = cms.Path(process.patseq)

# rename output file
# standard file name
process.out.fileName = cms.untracked.string('SQWaT_386_RAW_PAT.root')
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
