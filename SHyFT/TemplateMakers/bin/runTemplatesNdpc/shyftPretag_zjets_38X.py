import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')


print "Dumping some environment variables"
workingDir = os.environ['PWD']
listOfDirsInSearchPath = (os.environ['CMSSW_SEARCH_PATH']).rsplit(':')

print "PWD is %s" % (workingDir)
print "CMSSW_SEACH_PATH is %s" % (listOfDirsInSearchPath)

foundDir = 0

for iDir in listOfDirsInSearchPath:
    if iDir == workingDir:
        print "Found working directory in search path"
        foundDir = 1
# end loop over dirs

if foundDir == 1:
    print "Not modifying search path"
else :
    print "Adding directory to search path"
    newSearchPath = "%s:%s" %  (workingDir, os.environ['CMSSW_SEARCH_PATH'])
    os.environ['CMSSW_SEARCH_PATH'] = newSearchPath
    print "New path is %s" % os.environ['CMSSW_SEARCH_PATH']
    
    


process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0


process.shyftAnalysis.jetScale = cms.double(1.0)


jesShiftString = 'none'
process.shyftAnalysis.fancyJES = cms.string(jesShiftString)

jerShift = 0.0
process.shyftAnalysis.jerFactor = cms.double(jerShift)


process.inputs = cms.PSet (
    fileNames = cms.vstring(

'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_10_1_rpF.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_11_1_aDu.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_12_1_Spj.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_13_1_fYd.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_1_1_2wD.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_2_1_YCU.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_3_1_eek.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_4_1_AH2.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_5_1_epz.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_6_1_HhE.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_7_1_zXt.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_8_1_mEj.root',
'file:/data/ndpc2/c/sboutle/skims_386/zJets/skim_fullSelection_9_1_Yv5.root'

)
)


## outFileName = 'shyftPretag_ttbar_jes-%s_jer-%0.1f.root' % (jesShiftString, jerShift)
## print "Output name will be %s " % outFileName


process.outputs = cms.PSet (
    outputName = cms.string('zjets_test2.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(10)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 2
process.shyftAnalysis.maxJets = 8
