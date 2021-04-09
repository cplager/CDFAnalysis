import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0
    
process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_10_1_zaW.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_11_1_410.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_12_1_cNl.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_13_1_2ne.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_14_1_bO7.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_15_1_vWU.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_16_1_Vkg.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_17_1_CEy.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_18_1_vG4.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_19_1_OD5.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_1_1_j1d.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_20_1_DCY.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_2_1_B16.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_3_1_Snr.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_4_1_7qO.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_5_1_Gix.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_6_1_4tp.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_7_1_vk5.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_8_1_MgX.root',
    'file:/uscms/home/lannon/nobackup/CMSSW_3_6_2_crab/sync/res/shyft_361_pat_9_1_zQo.root'
    
    )
)

process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(100)

process.shyftAnalysis.minJets = 0
