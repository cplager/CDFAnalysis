import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')

#For running on skims
process.shyftAnalysis.gen = cms.string("prunedGenParticles")

#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(27.0)

#Set the PU reweighting
process.shyftAnalysis.pileupWeightsFile = cms.string("PileupDist_SQWaT_Muon_2012_3D.root")

#Now, unpack the command line arguments for this script
#listFileName = sys.argv[2]
#iJob = int(sys.argv[3])
#nJobs = int(sys.argv[4])
label = 'ttbar'
jesShift = 'none'
jerShift = 0.0


#Set the JES and JER shifts based on commmand line
#process.shyftAnalysis.fancyJES = cms.string(jesShift)
#process.shyftAnalysis.jerFactor = cms.double(jerShift)

#btag requirement
#btag -1 = pre-tag, 1= 1 btag, 0 = 0 btags
process.shyftAnalysis.btag = cms.double(1) 

#Determine which input files to run on
process.inputs = cms.PSet (
    fileNames = cms.vstring(
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_10_1_MXV.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_11_1_Ww2.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_12_1_LTL.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_13_1_Qa3.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_14_1_Dcz.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_15_1_MU9.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_16_1_3WL.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_17_1_Iyv.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_18_1_fRA.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_19_1_dCt.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_1_1_fY3.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_20_1_XIj.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_21_1_9Pc.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_22_1_ZeI.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_23_1_AM3.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_24_1_pxY.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_25_1_izo.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_26_1_SWq.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_27_1_C8r.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_28_1_OpR.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_2_1_EOW.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_3_1_9Xl.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_4_1_HPM.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_5_1_ayh.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_6_1_BgL.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_7_1_Irw.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_8_1_nMT.root',
'file:/store/user/lannon/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/SQWaT_muskim_42X/965481212accb396ee0392939c644670/skim_42X_PF2PAT_9_1_kBJ.root',

)
	)


#listFile = open(listFileName)
#readFiles = []
#for file in listFile:
#	if len(file.strip()) > 0:
#		readFiles.append(file.strip())

#Add just the right files from the list
#for file in readFiles[iJob::nJobs]:
#	print "Adding file:",file
#	process.inputs.fileNames.append('file:/pscratch/ndcms/bestman/storage/cms'+file.strip())


#label = listFileName.rsplit('.',1)[0]
#outFileName = 'mttbarCondor/%s/ttbarTree_%s_jes-%s_jer-%0.1f_job%03d.root' % (label,label,jesShift,jerShift,iJob)

#outFileName = 'mttbarTree_ttbar.root' #% (label,jesShift, jerShift)
#print "Output name will be %s " % outFileName

process.outputs = cms.PSet (
    outputName = cms.string('mttbarTree_wjets.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(1000)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)





process.shyftAnalysis.minJets = 3
process.shyftAnalysis.maxJets = 8
