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
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_9_1_wCp.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_8_0_pcn.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_7_0_Bls.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_6_0_1g7.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_5_1_WJ4.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_4_1_kYh.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_3_1_3nG.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_2_1_f9G.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_1_1_wxR.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_14_1_RKo.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_13_0_sIU.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_12_1_cwI.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_11_0_BFl.root',
       '/store/user/lannon/Mu/SHYFT_pat362_muskim_selv3_prompt_142557_142664/7e088d3e802b6af7e359804cda70cbb7/shyft_362_justMuons_10_0_pEy.root' ] );

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
			       fileName = cms.untracked.string("/uscms_data/d2/lannon/dataskim_selv3/dataSkim_selv3_prompt_142664_Aug13.root"),
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




