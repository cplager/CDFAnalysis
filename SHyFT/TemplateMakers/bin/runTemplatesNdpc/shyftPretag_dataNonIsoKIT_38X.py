import FWCore.ParameterSet.Config as cms
import os


process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

########################## For JES nonsense


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

muonIDString = "KITQCD"
process.shyftAnalysis.muonIdTight.version = cms.string(muonIDString)

process.shyftAnalysis.jetScale = cms.double(1.0)
jesShiftString = 'none'
process.shyftAnalysis.fancyJES = cms.string(jesShiftString)

jerShift = 0.0
process.shyftAnalysis.jerFactor = cms.double(jerShift)

outFileName = "shyftPretag_dataNonIsoKIT_38X_muon-%s_v2.root" % (muonIDString)

print "\n\n-----> Output file name is %s\n\n" % (outFileName)

process.outputs = cms.PSet (
    outputName = cms.string(outFileName)
)



process.inputs = cms.PSet (
    fileNames = cms.vstring(

'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_1_1_8y4.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_2_1_DgC.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_3_1_oQk.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_4_1_8Wi.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_5_1_42C.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_6_1_KJ6.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_7_1_BJa.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9/dataSkim_selv4_386_mu9_8_1_Zgn.root',

'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_10_1_w9D.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_11_1_f4y.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_1_1_rY1.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_12_1_Nb5.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_13_2_JEL.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_14_2_BqL.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_15_2_0zX.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_16_2_arp.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_17_2_jSA.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_18_2_xOP.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_19_2_7ld.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_20_2_2dk.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_21_1_i9W.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_2_1_iaT.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_22_2_4Vw.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_23_1_VHH.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_3_1_xXN.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_4_1_AWy.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_5_1_BJ0.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_6_1_Jve.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_7_1_fXA.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_8_1_TPz.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu15/dataSkim_selv4_386_mu15_9_1_kAu.root',


'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_10_1_7Fp.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_11_1_ieM.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_12_1_yth.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_13_1_mQP.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_14_1_cqn.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_15_1_w3s.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_16_1_fWu.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_17_1_6n6.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_18_1_4WV.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_19_1_sV0.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_1_1_xwt.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_2_1_3wc.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_3_1_PYj.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_4_1_RsC.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_5_1_eZT.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_6_1_Pkk.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_7_1_4TH.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_8_1_wS7.root',
'file:/data/ndpc2/c/sboutle/skims_386/dataskims_Nov4_noiso/mu9preTS/dataSkim_selv4_386_mu9_9_1_F5v.root',

    
)
)


# Turn off/on reliso cut
#process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 2
process.shyftAnalysis.maxJets = 8
