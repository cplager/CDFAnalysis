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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_gti.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_93_1_SR8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_92_1_jJj.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_91_1_IBy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_90_1_ugX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_1_ucU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_89_1_wfT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_88_1_QJT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_87_1_PHZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_86_1_0Ja.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_0_K4j.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_1_rHS.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_1_9Wm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_1_kQO.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_1_cAY.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_3sG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_0_U0x.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_GvM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_1_i0a.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_sJD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_UZd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_1i4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_0_Zz5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_0_KvD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_1_thc.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_0_vD2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_roD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_0_0MX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_0_BEk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_ixK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_0_kWx.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_bjG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_1_Sng.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_1_gSo.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_1_54Z.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_1_Iw8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_0_IwU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_NRr.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_0_xaf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_0_4jk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_1_KlZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_6RC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_A6D.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_1_1p4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_0_SHg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_1_tMH.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_1_onT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_1_2ms.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_6Du.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_0_DTd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_1_dAS.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_YA9.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_l4K.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_5tw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_1_oib.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_0_PWN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_1_6sq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_0_zTk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_0_Gdk.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_QBi.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_Uly.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_1_ovf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_0_Ofm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_8XT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_0_TeZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_Rwj.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_1_Nse.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_1_uPb.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_MxE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_0_2JP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_1_ZNZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_Wjy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_AOl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_q3N.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_1_3gg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_1_twl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_0_ogZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_0_bC4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_0_jee.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_ujz.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_dY0.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_xCd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_KZa.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_2yK.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_cXD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_1_Rno.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_OWy.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_1_jO4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_g9Y.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_hIp.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_Yo7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_1_8IM.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142189_142537/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_1_Tcp.root' ] );

for file in readFiles:
#    process.source.fileNames.append('dcap:///pnfs/cms/WAX/11'+file)
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-142664_7TeV_StreamExpress_Collisions10_JSON_TopAug13_Filtered_138564-142664.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_142537_Aug13.root"),
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




