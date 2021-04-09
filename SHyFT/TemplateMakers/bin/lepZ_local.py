import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.shyftPretag_cfi')
process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0


#####################################
#   DEBUG OUTPUT 
#####################################

# values: True or False
# True is very verbose
process.shyftAnalysis.debugOutput = cms.bool(False)


#Now, unpack the command line arguments for this script
#listFileName = sys.argv[2]
#iJob = int(sys.argv[3])
#nJobs = int(sys.argv[4])
label = 'ttbar'
jesShift = 'none'
jerShift = 0.0

#Set the JES and JER shifts based on commmand line
process.shyftAnalysis.fancyJES = cms.string(jesShift)
process.shyftAnalysis.jerFactor = cms.double(jerShift)


########## IGNORE SOME CUTS ###################

process.shyftAnalysis.cutsToIgnore = cms.vstring("== 1 Tight Lepton",
                                                 "== 1 Tight Lepton, Mu Veto",
                                                 "== 1 Lepton",
                                                 "Z Veto")

#Determine which input files to run on
process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'file:/data/ndpc2/c/sboutle/skims_386/zJets_ge1mu/skim_fullSelection_10_1_9dM.root'
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

outFileName = 'plotsLepZStudies_%s_jes-%s_jer-%0.1f.root' % (label,jesShift, jerShift)
print "Output name will be %s " % outFileName

process.outputs = cms.PSet (
    outputName = cms.string(outFileName)
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
process.shyftAnalysis.maxJets = 8
