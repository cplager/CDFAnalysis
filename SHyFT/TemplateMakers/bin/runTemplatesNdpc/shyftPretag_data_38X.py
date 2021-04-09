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
    
# different jet colletions
process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsAK5PF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0

process.shyftAnalysis.cutsToIgnore = cms.vstring("Trigger")
#process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

process.shyftAnalysis.jetScale = cms.double(1.0)

jesShiftString = 'none'
process.shyftAnalysis.fancyJES = cms.string(jesShiftString)

jerShift = 0.0
process.shyftAnalysis.jerFactor = cms.double(jerShift)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
    
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_10_1_hoS.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_11_1_HCf.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_1_1_PAg.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_12_1_NP5.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_13_1_AkI.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_14_1_QJY.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_15_1_Ank.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_16_2_Kbr.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_17_1_ad8.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_18_1_I0x.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_19_1_SK2.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_20_1_smY.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_21_1_rmt.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_2_1_70U.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_22_1_j2A.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_3_1_Kh5.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_4_1_ild.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_5_1_oGb.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_6_1_1Fb.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_7_1_nUD.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_8_1_gxI.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9preTS/dataSkim_selv4_386_mu9_9_1_wBk.root',


'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9/ljmet_skim_1_1_Huf.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9/ljmet_skim_2_1_9Sf.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9/ljmet_skim_3_1_HCP.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9/ljmet_skim_4_1_IZN.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu9/ljmet_skim_5_1_V8e.root',

'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_10_1_n8T.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_11_1_0tY.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_1_1_tAK.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_12_1_hKy.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_13_1_6wJ.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_2_1_DML.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_3_1_aOf.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_4_1_Vhb.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_5_1_BIH.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_6_1_cr9.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_7_1_bZM.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_8_1_cOy.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Jan21/mu15/dataSkim_selv4_386_mu9_9_1_Bow.root'



    
)
)

process.outputs = cms.PSet (
    outputName = cms.string('data_test2.root')
)

# Turn off/on reliso cut
#process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

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
