import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('Analysis.SHyFT.shyftAnalysis_cfi')

process.inputs = cms.PSet (
    fileNames = cms.vstring(
'dcap://cmsdca.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lannon/WJets-madgraph/SHYFT_pat357/cf923400bf995f0c270a21dc1a301307/ljmet_88_1.root',
'dcap://cmsdca.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/sboutle/TTbarJets-madgraph/SHYFT_ttbar_synch_384/a5f281b3597f7f372ceb0a9e8572897c/shyft_384_pat_9_0_cig.root',
'dcap://cmsdca.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/sboutle/TTbarJets-madgraph/SHYFT_ttbar_synch_384/a5f281b3597f7f372ceb0a9e8572897c/shyft_384_pat_99_0_Mop.root'

       )
)
process.shyftAnalysis.printEvery = cms.int32(100)
process.shyftAnalysis.mode = cms.int32(0)
process.shyftAnalysis.sampleName = cms.string('combinedTemplate')

process.outputs = cms.PSet (
    outputName = cms.string('shyftPlots.root')
)
