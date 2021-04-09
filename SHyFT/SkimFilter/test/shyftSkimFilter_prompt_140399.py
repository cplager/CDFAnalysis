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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_8te.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_99_1_Sfj.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_98_1_D9C.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_97_1_mpQ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_96_1_2VD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_95_1_ksz.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_94_1_pxb.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_93_1_XwI.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_0_SrC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_0_F1r.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_1_zlK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_0_Y48.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_1_L0g.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_1_k1T.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_mjd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_x7M.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_KOU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_1_D0I.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_0_ms5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_0_SGJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_0_CgD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_zp4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_BXd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_0_wt3.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_1_RPn.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_ATZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_0_tuG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_0_4pD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_0_fuL.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_0_sla.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_0_xdn.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_0_lpZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_0_WzW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_22b.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_0_INB.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_0_ofX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_0_40P.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_0_kg5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_0_B1M.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_0_tsM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_gCy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_0_IVP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_1_Hjy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_4iR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_0_EVe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_1_kaS.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_1_LIm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_9jG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_MPv.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_1_zlm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_1_kIT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_1_Bnm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_1_BoL.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_0_6th.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_Aae.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_0_9I7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_6rq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_0_beo.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_0_fCN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_0_K3o.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_0_aFC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_0_29o.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_0_rvZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_0_9WH.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_0_YhN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_0_rY5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_EcW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_0_In3.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_iE2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_0_J34.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_0_nvg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_wA0.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_0_xW0.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_0_1iV.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_DC1.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_aoX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_1_rax.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_3Bh.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_T6q.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_0_iuT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_0_m11.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_0_Lsl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_0_UMy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_kiW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_0_fXL.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_0_m2b.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_0_eel.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_oxF.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_inm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_0_dj2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_0_zuT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_0_keP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_ZUQ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_167_1_S8j.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_166_0_T2o.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_165_0_FMJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_164_0_Q5b.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_163_0_BfN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_162_0_bSB.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_161_0_7WC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_160_0_YL4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_0_onj.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_159_0_gkR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_158_1_NoE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_157_1_G06.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_156_1_VCG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_155_0_1Qi.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_154_0_Vu1.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_153_0_ymh.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_152_0_9fE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_151_0_Mu3.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_150_0_C2y.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_qFh.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_149_1_Lmg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_148_0_AY0.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_147_0_Wma.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_146_0_doM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_145_0_bvY.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_144_0_q2y.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_143_0_uHr.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_142_0_iZe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_141_1_iRL.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_140_1_skk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_0_2wT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_139_0_zB0.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_138_0_Ego.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_137_0_AlX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_136_1_h1d.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_135_1_tLK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_134_1_1HY.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_133_1_wdW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_132_1_K10.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_131_0_TD5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_130_0_xra.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_iIR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_129_0_Ivq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_128_0_la3.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_127_0_vg7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_126_1_rEX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_125_1_t00.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_124_0_ziJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_123_0_cvE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_122_0_873.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_121_0_3pX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_120_0_SbT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_0_qZC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_119_0_nOC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_118_1_O0K.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_117_0_ClM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_116_0_qRq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_115_0_VzP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_114_0_CtE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_113_0_R3K.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_112_0_pC6.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_111_1_yYF.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_110_1_jXO.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_WjU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_109_1_hhd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_108_1_ywc.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_107_1_LLa.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_106_1_z7u.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_105_1_Z6D.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_104_1_D4T.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_103_1_JNQ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_102_1_vO3.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_101_1_cH9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_100_1_uat.root' ] );


for file in readFiles:
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-143336_7TeV_StreamExpress_Collisions10_JSON_TopAug26_Filtered_138564-143336_Extend_143337-143835_Recover_noESDCS.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/sboutle/dataskim_selv3/dataSkim_selv3_prompt_140399_Aug26.root"),
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




