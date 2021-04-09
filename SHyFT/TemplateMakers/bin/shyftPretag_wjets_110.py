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

process.shyftAnalysis.jetScale = cms.double(1.1)

process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_100_1_vge.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_101_1_M4S.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_10_1_zwk.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_11_1_h9Q.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_12_1_awX.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_13_1_8F2.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_14_1_DL2.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_15_1_hoC.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_16_1_xrl.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_17_1_XMl.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_18_1_OVq.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_19_1_LY0.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_1_1_A2l.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_20_1_s4I.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_21_1_GMU.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_22_1_OLF.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_23_1_NbR.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_24_1_DYO.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_25_1_IvE.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_26_1_Oow.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_27_1_kG9.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_28_1_MIm.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_29_1_IJ4.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_2_1_IzJ.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_30_1_7Ki.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_31_1_6xS.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_32_1_WDp.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_33_1_FJ7.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_34_1_0aK.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_35_1_I5x.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_36_1_nnj.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_37_1_Hgk.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_38_1_a8S.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_39_1_SnJ.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_3_1_3pY.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_40_1_lgM.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_41_1_r7M.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_42_1_ayO.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_43_1_oQj.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_44_1_cuB.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_45_1_431.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_46_1_suQ.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_47_1_WDZ.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_48_1_OLc.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_49_1_Wux.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_4_1_s0x.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_50_1_17W.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_51_1_snM.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_52_1_Ao4.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_53_1_ZCB.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_54_1_9bN.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_55_1_Rqf.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_56_1_1wN.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_57_1_SnC.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_58_1_mVY.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_59_1_SWv.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_5_1_wIC.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_60_1_OGn.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_61_1_0C5.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_62_1_Wjc.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_63_1_Pgz.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_64_1_MzD.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_65_1_65i.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_66_1_Ctc.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_67_1_b58.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_68_1_0la.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_69_1_DlG.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_6_1_xJQ.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_70_1_aRc.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_71_1_oDW.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_72_1_aW2.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_73_1_SDr.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_74_1_d2I.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_75_1_bWz.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_76_1_vGs.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_77_1_dcw.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_78_1_K53.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_79_1_8AD.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_7_1_sh4.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_80_1_RWr.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_81_1_Kwc.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_82_1_Kcw.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_83_1_1kw.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_84_1_tNw.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_85_1_IXI.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_86_1_15S.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_87_1_RtT.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_88_1_thy.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_89_1_8u6.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_8_1_vu8.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_90_1_kdd.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_91_1_FyV.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_92_1_m7H.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_93_1_WBI.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_94_1_VCn.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_95_1_C0k.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_96_1_Jne.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_97_1_SC4.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_98_1_byh.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_99_1_4kb.root',
'file:/data/ndpc2/b/sboutle/skims_362/WJets/ljmet_skim_9_1_m1r.root'
)
)

process.outputs = cms.PSet (
    outputName = cms.string('wjetsHistos_jes110.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(200)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
