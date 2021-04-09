import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

# Change input tags for older version of PAT
# For MC only
#process.shyftAnalysis.muonSrc = cms.InputTag('selectedPatMuonsStd')
#process.shyftAnalysis.electronSrc = cms.InputTag('selectedPatElectronsStd')
#process.shyftAnalysis.jetSrc = cms.InputTag('selectedPatJetsStd')
#process.shyftAnalysis.metSrc = cms.InputTag('patMETsStd')
#process.shyftAnalysis.trigSrc = cms.InputTag('patTriggerEventStd')

process.shyftAnalysis.jetScale = cms.double(1.0)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_10_1_jAw.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_11_1_oMM.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_12_1_kRf.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_13_1_rlx.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_14_1_Lb5.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_15_1_DiP.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_16_1_MDi.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_17_1_I4H.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_18_1_Kd2.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_19_1_wNm.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_1_1_MBM.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_20_1_WGZ.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_21_1_5ur.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_22_1_kOL.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_23_1_eW6.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_24_1_xO2.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_25_1_b4n.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_26_1_n5Q.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_27_1_tfh.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_28_1_uXu.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_29_1_Ohd.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_2_1_fW6.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_30_1_TvE.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_31_1_Puf.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_32_1_F6S.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_33_1_NAM.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_34_1_4QR.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_35_1_bON.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_36_1_fwz.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_37_1_eFL.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_38_1_jsx.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_39_1_LPR.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_3_1_DQZ.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_40_1_TZY.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_41_1_iwS.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_42_1_enx.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_43_1_qce.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_44_1_2jC.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_4_1_yMH.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_5_1_bLn.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_6_1_PgQ.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_7_1_Tcx.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_8_1_Ceh.root',
'file:/data/ndpc2/b/sboutle/skims_362/InclusiveMu15_noIso/ljmet_skim_9_1_oD3.root',

)
)

process.outputs = cms.PSet (
    outputName = cms.string('qcdHistos_non_jes100.root')
)

process.shyftAnalysis.muonIdTight.cutsToIgnore.append("RelIso")

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
