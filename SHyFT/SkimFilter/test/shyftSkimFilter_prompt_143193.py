import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.minJets = cms.int32(0)
process.shyftAnalyzer.selectionDetails.pvSelector.maxZ = 24.0
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
process.shyftAnalyzer.trigRequired = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(-1)
	)

process.MessageLogger = cms.Service(
	"MessageLogger",
	cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING')
				  ),
	categories     = cms.untracked.vstring('SHyFTSkimFilter'),	
	destinations   = cms.untracked.vstring('cout')
	)


process.source = cms.Source("PoolSource",
			    skipEvents = cms.untracked.uint32(0),
			    fileNames = cms.untracked.vstring(),
                            lumisToProcess = cms.untracked.VLuminosityBlockRange()
			    )

readFiles = []

readFiles.extend( [
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_dg2.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_99_1_XUK.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_98_1_cqd.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_97_1_8BD.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_96_1_ydB.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_95_1_Ic7.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_94_1_UV7.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_93_1_P4x.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_1_aFE.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_1_EGL.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_1_7ci.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_7fh.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_1_VZJ.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_1_3nY.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_g3d.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_hVX.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_ZOn.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_1_TXa.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_1_TKB.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_1_yqy.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_1_v6t.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_xBI.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_B6i.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_UOS.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_1_3fq.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_2_dS1.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_5LK.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_Bmy.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_1_kEO.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_2_QUo.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_1_OMY.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_1_uZG.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_EQa.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_1_5Bi.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_Al5.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_1_u6O.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_I7p.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_1_qVg.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_1_kmN.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_lSP.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_2_zv9.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_1_1JA.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_j7f.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_zfN.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_1_UCg.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_1_08d.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_Cva.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_Gaq.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_2_0wj.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_2_ls3.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_1_d8A.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_1_Hmy.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_1_tv2.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_WzY.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_XxY.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_2rS.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_Y0n.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_61P.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_72G.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_1_0pI.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_1_9eT.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_2_z4R.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_1_sGr.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_1_eN0.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_Dfh.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_3_YL8.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_1_x3P.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_mnN.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_KY8.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_1_AuD.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_aFv.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_2_dtH.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_1_tHI.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_D2s.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_uT2.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_2_mNo.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_xtd.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_zvw.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_6qK.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_2_klr.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_2_3hp.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_1_m1e.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_nbX.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_2_Ahl.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_Lua.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_gg7.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_ZWC.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_u3X.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_2_Hcp.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_2_GcZ.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_2_lfE.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_2_28a.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_wxl.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_2_eAI.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_pw7.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_nnz.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_rvN.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_1qR.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_105_1_PrI.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_104_1_Mfl.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_103_1_hBo.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_102_1_YIC.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_101_1_H77.root',
       '/store/user/slaunwhj/Mu/SHYFT_pat362_muskim_selv3_Aug19/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_100_1_wGG.root' ] );



for file in readFiles:
#    process.source.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-143336_7TeV_StreamExpress_Collisions10_JSON_TopAug26_Filtered_138564-143336_Extend_143337-143835_Recover_noESDCS.txt'
#JSONfile = 'json/Cert_132440-143179_7TeV_StreamExpress_Collisions10_JSON.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/sboutle/dataskim_selv3/dataSkim_selv3_prompt_143193_Aug26.root"),
			       maxSize = cms.untracked.int32(1048576),

			       # This is how you require a filter
			       # for the path
			       SelectEvents = cms.untracked.PSet(
	SelectEvents = cms.vstring('p1')
	)
			       )

process.p1 = cms.Path(
	process.shyftAnalyzer
	)

process.outpath = cms.EndPath(process.out)




