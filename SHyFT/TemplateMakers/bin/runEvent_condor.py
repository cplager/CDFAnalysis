import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')
#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(27.0)


#Now, unpack the command line arguments for this script
listFileName = sys.argv[2]
iJob = int(sys.argv[3])
nJobs = int(sys.argv[4])
## jesShift = sys.argv[5]
## jerShift = float(sys.argv[6])

#Set the JES and JER shifts based on commmand line
## process.shyftAnalysis.fancyJES = cms.string(jesShift)
## process.shyftAnalysis.jerFactor = cms.double(jerShift)

#Determine which input files to run on
process.inputs = cms.PSet (
    fileNames = cms.vstring()
	)
listFile = open(listFileName)
readFiles = []
for file in listFile:
	if len(file.strip()) > 0:
		readFiles.append(file.strip())

#Add just the right files from the list
for file in readFiles[iJob::nJobs]:
	print "Adding file:",file
	process.inputs.fileNames.append('file:'+file.strip())



#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(10)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

