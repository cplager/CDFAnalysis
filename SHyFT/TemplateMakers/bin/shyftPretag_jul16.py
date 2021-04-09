import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')


process.inputs = cms.PSet (
    fileNames = cms.vstring(),

    lumisToProcess = cms.untracked.VLuminosityBlockRange()
    )


readFiles = []

readFiles.extend( [
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_Qb9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_0_lQJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_0_9Db.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_1_3Gt.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_unQ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_1_kpi.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_1_2Qo.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_MDP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_RX7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_zZp.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_0_jib.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_1_hej.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_0_wp1.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_0_Lhf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_hMB.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_1_0zO.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_YT9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_1_lBV.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_wWV.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_4wK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_RgW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_1_Wfr.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_0_Eyv.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_1_XZr.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_1_AOe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_kRM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_fVl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_1_sLJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_wU9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_1_tcQ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_0_L1A.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_1_mR9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_0_Udl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_csJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_0_bf5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_1_nM4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_pHU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_Ol6.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_1_E7o.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_0_Jsf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_zSL.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_JGv.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_0_lxd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_1_pvC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_0_Urt.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_1_tR7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_1_4se.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_vij.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_W0q.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_lTs.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_r41.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_JEw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_gJS.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_1_2AM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_1_lh1.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_1_cJ2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_1_yUq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_1_t5o.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_llB.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_nKA.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_0_NWD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_MiJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_U05.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_1_EmJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_8Lt.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_1_sHd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_1_ft5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_Si8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_1_Zpx.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_1_qRP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_0_4bD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_maN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_J4n.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_1_QTA.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_1_raK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_1_0Kb.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_N8g.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_1_CgJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_eDK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_nrR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_npw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_mmc.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_PI9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_ezJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_2_xgv.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_CVD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_e3X.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_OUy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_5zR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_WrM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_JlU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_jul16_139779_140159/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_0Wi.root' ] );


for file in readFiles:
    process.inputs.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)


# get JSON file correctly parced
JSONfile = 'json/Cert_139779-1340159_7TeV_July16thReReco_Collisions10_JSON.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.inputs.lumisToProcess.append(str(entry))


process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag_jul16.root')
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
