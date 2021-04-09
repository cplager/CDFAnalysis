import FWCore.ParameterSet.Config as cms
import os
import sys

#Default job configuration
process = cms.Process("FWLitePlots")
process.load('SHyFT.TemplateMakers.sqwat_topPAT_cfi')

#tighten the muon pt cut
process.shyftAnalysis.muPtMin = cms.double(0.0)

# add in a mtMin cut
process.shyftAnalysis.mtMin = cms.untracked.double(-1.0)

# add in a met cut

process.shyftAnalysis.metMin = cms.double(-1.0)


#Set the PU reweighting
process.shyftAnalysis.pileupHistData = cms.string("data_pileup_160404-180252_kevin.root")

#Now, unpack the command line arguments for this script
listFileName = sys.argv[2]
iJob = int(sys.argv[3])
nJobs = int(sys.argv[4])
## jesShift = sys.argv[5]
## jerShift = float(sys.argv[6])

print "*** Beginning Condor Job ***"
print "Arguments are: list = %s, iJob = %d, nJobs = %d\n\n" % (listFileName, iJob, nJobs)

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


label = listFileName.rsplit('.',1)[0]
#outFileName = 'templatesCondor/%s/shyftPretag_%s_jes-%s_jer-%0.1f_job%03d.root' % (label,label,jesShift,jerShift,iJob)
outFileName = 'batchMttbarTree/%s/mttbarTree_%s_job%03d.root' % (label,label,iJob)
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
