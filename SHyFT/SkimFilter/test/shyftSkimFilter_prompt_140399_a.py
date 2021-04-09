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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_138564_139459_140160_140399/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_50_1_Aae.root'] );


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
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_140399_a.root"),
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




