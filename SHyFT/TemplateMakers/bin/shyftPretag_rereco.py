import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

process = cms.Process("FWLitePlots")

process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

process.inputs = cms.PSet (
    fileNames = cms.vstring(),

    lumisToProcess = cms.untracked.VLuminosityBlockRange()
    )


readFiles = []

readFiles.extend( [
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_bTE.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_0_PWH.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_1_7Bw.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_0_Jbd.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_0_OAg.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_1_gaa.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_1_Aax.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_uz8.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_vPN.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_BSl.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_1_0KS.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_1_jA9.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_1_dzm.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_0_64F.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_xt9.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_0_sE3.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_zID.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_0_HR3.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_6aP.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_pjE.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_r5j.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_1_yfy.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_1_UdI.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_1_I5h.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_0_tm3.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_P2A.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_cuP.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_1_gO3.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_SRL.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_1_uGU.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_0jj.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_0_quc.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_1_2cH.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_3yf.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_1_Nre.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_1_lfJ.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_0_xmT.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_Pkn.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_0_8DK.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_0_Jxz.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_0_PkX.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_0_UQB.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_0_NUX.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_1_TEJ.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_1_j4P.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_1_Vhm.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_1_EiO.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_0_FPv.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_cIK.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_pfZ.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_Fsv.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_7kU.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_4dh.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_1_ifm.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_1_H80.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_1_V9R.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_0_eyN.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_0_Wpr.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_UbV.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_gEN.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_1_Jbk.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_j5A.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_tWi.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_0_7Jf.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_SiM.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_1_kcr.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_0_T4r.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_0_7va.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_49e.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_1_08S.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_iHN.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_0_9sn.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_uIh.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_0_Wsl.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_1_KUR.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_1_3y3.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_0_cdt.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_0_7mA.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_VaV.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_0_m9y.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_gd0.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_QWN.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_0_aCZ.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_29g.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_0_a8i.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_0_2Jc.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_OgI.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_8oS.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_0_hCt.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_6rt.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_t5T.root',
       '/store/user/lannon/MinimumBias/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_IgT.root' ] );

readFiles.extend( [
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_UHW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_Y7e.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_KXu.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_7CE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_VK5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_swK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_9oB.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_qj2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_EIf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_vIz.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_SmG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_2Vw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_1_XYG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_Oel.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_XRU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_0cH.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_N0A.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_Zbk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_yaE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jun14_132440_137028/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_Uog.root' ] );


for file in readFiles:
    process.inputs.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)

    
# get JSON file correctly parced
JSONfile = 'json/Cert_132440-137028_7TeV_June14thReReco_Collisions10_JSON_v2.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.inputs.lumisToProcess.append(str(entry))
    


process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag_rereco.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(1)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 4

#Needed for Selv3 (only for data)
process.shyftAnalysis.pvSelector.maxZ = 24.0

