import FWCore.ParameterSet.Config as cms

process = cms.Process("SHYFTFilterTheEvents")
process.load("SHyFT.SkimFilter.SHYFTAnalyzer_cfi")

process.shyftAnalyzer.selectionDetails.minJets = cms.int32(0)
process.shyftAnalyzer.selectionDetails.muonIdTight.cutsToIgnore.append('RelIso')
process.shyftAnalyzer.verbose = cms.untracked.bool(False)
process.shyftAnalyzer.trigRequired = cms.untracked.bool(True)

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(1000)
	)

process.MessageLogger = cms.Service(
	"MessageLogger",
	cout = cms.untracked.PSet(threshold = cms.untracked.string('WARNING')
				  ),
	categories     = cms.untracked.vstring('SHyFTSkimFilter'),	
	destinations   = cms.untracked.vstring('cout')
	)


# Declare some default names 
fileNames = [
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_1_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_2_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_3_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_4_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_5_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_6_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_7_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_8_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_9_1.root',
	'file:///uscms/home/lannon/nobackup/syncEx/ljmet_10_1.root'
	]

process.source = cms.Source("PoolSource",
			    skipEvents = cms.untracked.uint32(0),
			    fileNames = cms.untracked.vstring(fileNames)
			    )



process.out = cms.OutputModule("PoolOutputModule",
			       outputCommands = cms.untracked.vstring('keep *'),
			       # Disable fast cloning to resolve 34X issue
			       # fastCloning = cms.untracked.bool(False),
			       fileName = cms.untracked.string("ljmet_skim.root"),
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




