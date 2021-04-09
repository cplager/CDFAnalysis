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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_152_0_9fE.root'] );


for file in readFiles:
    process.source.fileNames.append(file)


# get JSON file correctly parced
JSONfile = 'json/Cert_132440-141961_7TeV_StreamExpress_Collisions10_JSON_TopAug9_Filtered_138564-141961_Extended_141962-142537.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_140399_b.root"),
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




