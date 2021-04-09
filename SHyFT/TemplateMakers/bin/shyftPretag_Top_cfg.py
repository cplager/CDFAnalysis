import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.sqwat_PF2PAT_cfi')

###########  PF2PAT  configs ######################## 
## process.shyftAnalysis.selectionDetails.muJetDRJets=cms.double( 0.1 )
## process.shyftAnalysis.selectionDetails.muJetDRMuon=cms.double( 0.3 )
## process.shyftAnalysis.selectionDetails.jetSrc = cms.InputTag('selectedPatJetsPFlow')
## process.shyftAnalysis.selectionDetails.metSrc = cms.InputTag('patMETsPFlow')
## process.shyftAnalysis.selectionDetails.jetPtMin = cms.double(30.0)


#process.shyftAnalysis.muJetDRJets=cms.double( 0.1 )
process.shyftAnalysis.muJetDRJets=cms.double(0.1)
process.shyftAnalysis.muJetDRMuon=cms.double( 0.3 )
process.shyftAnalysis.jetSrc = cms.InputTag('goodPatJetsPF')
process.shyftAnalysis.eleEtaMaxLoose.jetSrc = cms.InputTag('goodPatJetsPF')
process.shyftAnalysis.muonSrc = cms.InputTag('tightPatMuonsPF')
process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsPF')
process.shyftAnalysis.metSrc = cms.InputTag('patMETsPF')
process.shyftAnalysis.pvSelector.pvSrc  = cms.InputTag('goodOfflinePrimaryVertices')
process.shyftAnalysis.muonIdLoose.pvSrc = cms.InputTag('goodOfflinePrimaryVertices')
process.shyftAnalysis.muonIdTight.pvSrc = cms.InputTag('goodOfflinePrimaryVertices')
process.shyftAnalysis.jetPtMin = cms.double(30.0)
process.shyftAnalysis.pvSelector.maxZ = 24.0

#top group pfRelIso cuts
#process.shyftAnalysis.muonIdLoose.RelIso = cms.double(0.2)
#process.shyftAnalysis.electronIdLoose.RelIso = cms.double(0.2)
#process.shyftAnalysis.muonIdTight.RelIso = cms.double(0.125)

process.shyftAnalysis.muTrig = cms.string('HLT_IsoMu17_v.*')

#Turn the cuts off - we do not want to use Selector Utils
process.shyftAnalysis.cutsToIgnore = cms.vstring ("PV")
process.shyftAnalysis.cutsToIgnore.append ("PV")
process.shyftAnalysis.cutsToIgnore.append (">= 1 Lepton")
process.shyftAnalysis.cutsToIgnore.append ("== 1 Tight Lepton")
process.shyftAnalysis.cutsToIgnore.append ("== 1 Tight Lepton, Mu Veto")
process.shyftAnalysis.cutsToIgnore.append ("== 1 Lepton")
process.shyftAnalysis.cutsToIgnore.append ("Z Veto")
#process.shyftAnalysis.cutsToIgnore.append (">=1 Jets")
#process.shyftAnalysis.cutsToIgnore.append (">=2 Jets")
#process.shyftAnalysis.cutsToIgnore.append (">=3 Jets")
#process.shyftAnalysis.cutsToIgnore.append (">=4 Jets")
#process.shyftAnalysis.cutsToIgnore.append (">=5 Jets")

   
process.inputs = cms.PSet (
    fileNames = cms.vstring(
'file:SQWaT_PF2PATtuple_42X_synch1f_oldv4.root'

    )
)

process.outputs = cms.PSet (
	outputName = cms.string('scratch.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(2000)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(-1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(10000)

process.shyftAnalysis.minJets = 0


#outfile = open('cfg_shPre.py','w')
#print >> outfile,process.dumpPython()
#outfile.close()
