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
JSONfile = 'json/Cert_132440-141961_7TeV_StreamExpress_Collisions10_JSON_TopAug9_Filtered_138564-141961_Extended_141962-142537.txt'
myList = LumiList.LumiList (filename = JSONfile).getCMSSWString().split(',')
for entry in myList :
    process.source.lumisToProcess.append(str(entry))


process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_140399_c.root"),
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




