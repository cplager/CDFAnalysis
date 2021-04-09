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
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_5LZ.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_99_0_Pls.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_98_0_ABm.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_97_0_b1Y.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_96_0_h8n.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_95_0_RvU.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_94_0_Bhd.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_93_0_y9B.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_0_Q1u.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_0_TR5.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_0_xTJ.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_4Hp.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_0_iKv.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_0_yS3.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_0_4Jd.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_0_yGX.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_0_MLJ.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_0_a7n.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_0_pnv.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_0_lCx.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_0_T3N.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_0_B1l.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_4wN.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_0_lWP.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_0_1ED.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_0_H5D.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_0_M3a.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_0_xJo.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_0_e3v.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_0_AoZ.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_0_nPT.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_0_Uuj.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_0_A42.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_sbR.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_0_QuA.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_0_bxJ.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_0_pyb.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_omW.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_0_R9A.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_0_ZdW.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_0_QG5.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_0_iLf.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_0_kls.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_MMk.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_2ql.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_0_spV.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_0_Gis.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_0_Ujf.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_0_FH6.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_0_VBD.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_0_42d.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_0_1iv.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_0_LR9.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_0_aEs.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_0_0Nm.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_ck0.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_0_Cbg.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_0_xuz.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_0_Mmz.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_0_DPz.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_0_pUm.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_0_7wg.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_0_WHr.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_0_IOC.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_0_vui.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_0_0Iy.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_5cl.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_0_gbk.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_0_hqF.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_0_BEl.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_0_fQd.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_0_bob.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_0_ZeP.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_0_X80.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_znU.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_GXx.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_0_Fiw.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_5jD.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_0_lpK.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_0_89O.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_0_4jb.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_0_jLV.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_0_J03.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_aWN.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_1_5pc.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_iWj.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_JMT.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_95X.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_3Bl.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_Z87.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_HQx.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_1_gCN.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_DUe.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_1LX.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_Ib4.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_3_54n.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_1gi.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_2_HTP.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_IKw.root',
           '/store/user/lwming/Mu/SHYFT_pat362_muskim_selv3_prompt_143953_144114/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_100_0_My4.root' ] );


for file in readFiles:
#    process.source.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-144114_7TeV_StreamExpress_Collisions10_JSON.txt'
#'json/Cert_132440-143336_7TeV_StreamExpress_Collisions10_JSON_TopAug26_Filtered_138564-143336_Extend_143337-143835_Recover_noESDCS.txt'
#JSONfile = 'json/Cert_132440-143179_7TeV_StreamExpress_Collisions10_JSON.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/lwming/dataskim_selv3/dataSkim_selv3_prompt_144114_Sep3.root"),
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




