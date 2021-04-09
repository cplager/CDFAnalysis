import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')
#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(27.0)

#Set the PU reweighting
process.shyftAnalysis.pileupWeightsFile = cms.string("data_pileup3D_160404-180252_kevin_mbxs68000.root")

#Now, unpack the command line arguments for this script
#listFileName = sys.argv[2]
#iJob = int(sys.argv[3])
#nJobs = int(sys.argv[4])
label = 'ttbar'
jesShift = 'none'
jerShift = 0.0


# OMG Change this

#set up to run on PAT tups
process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsPFlow')
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsPFlow')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPFlow')


#Set the JES and JER shifts based on commmand line
#process.shyftAnalysis.fancyJES = cms.string(jesShift)
#process.shyftAnalysis.jerFactor = cms.double(jerShift)

#Determine which input files to run on
process.inputs = cms.PSet (
    fileNames = cms.vstring(
'/store/user/lannon/TTJets_TuneZ2_7TeV-madgraph-tauola/SQWaT_PAT_42X_Fall11_v3/3156aed375cb9d84362f1cc2ea98a9c4/pat_42x_fall11_1925_2_RKV.root'
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
    outputName = cms.string('mttbarTree_ttbar.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)





process.shyftAnalysis.minJets = 3
process.shyftAnalysis.maxJets = 8
