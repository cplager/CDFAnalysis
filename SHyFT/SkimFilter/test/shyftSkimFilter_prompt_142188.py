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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_0_Fqh.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_0_VcF.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_85_1_ogt.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_84_0_cOW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_83_0_D1I.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_82_1_0Ld.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_81_1_Omd.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_80_1_eOn.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_0_A8I.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_79_1_4JW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_78_1_vIN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_77_1_ihl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_76_1_Hk8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_75_1_n4y.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_74_1_huT.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_73_0_Wwb.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_72_0_02W.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_71_0_jZe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_70_1_SHg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_1_GsJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_69_1_udJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_68_1_aPg.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_67_1_pXs.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_66_1_LR8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_65_1_HOm.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_64_0_gWN.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_63_0_4I5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_62_0_HpY.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_61_0_atS.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_60_1_yCf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_0_LNe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_59_1_gvW.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_58_1_WzR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_57_1_PSw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_56_1_LaE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_55_0_CxP.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_54_0_8an.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_53_0_sB2.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_52_0_fau.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_51_0_vuG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_0_weD.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_0_x7a.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_49_0_jqX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_48_1_I50.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_47_1_rRu.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_46_1_GEV.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_45_0_7MG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_44_1_SpC.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_43_1_HF6.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_42_1_DM6.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_41_1_ib1.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_40_1_50S.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_0_Tq8.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_39_0_i2S.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_38_1_47E.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_37_1_ghp.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_36_1_vfE.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_35_1_9Cr.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_34_1_Vaj.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_33_1_2Tx.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_32_1_2Wc.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_31_0_qaX.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_30_0_CY5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_KIw.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_29_1_nCe.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_28_1_vSp.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_27_0_AQq.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_26_1_BU6.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_25_0_bIa.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_24_1_laZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_23_1_LWz.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_22_1_mYI.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_21_1_w5m.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_20_1_phF.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_nda.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_19_1_8EY.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_18_1_Q9v.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_17_1_EUJ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_16_1_fqf.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_15_0_lSV.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_UI5.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_1_Rek.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_0_okG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_0_zNZ.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_140401_142188/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_0_0EG.root' ] );



for file in readFiles:
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
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_142188_Aug13.root"),
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




