import FWCore.ParameterSet.Config as cms

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

process.inputs = cms.PSet (
    fileNames = cms.vstring(
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_100_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_101_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_10_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_102_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_103_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_104_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_105_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_106_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_107_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_108_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_109_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_110_1.root',
    'dcap:///pnfs/cms/WAX/11/store/user/rappocc/MinimumBias/shyft_357_data_v1/8f2ebcaed76069a836d8d452483e47c8/shyft_7TeV_firstdata_357_pat_111_1.root'
    ),

    lumisToProcess = cms.untracked.VLuminosityBlockRange(
    '132440:157-132440:401',
    '132596:382-132596:383',
    '132596:447-132596:453',
    '132598:80-132598:82',
    '132598:174-132598:188',
    '132599:1-132599:379',
    '132599:381-132599:538',
    '132601:1-132601:207',
    '132601:209-132601:259',
    '132601:261-132601:1131',
    '132602:1-132602:83',
    '132605:1-132605:444',
    '132605:446-132605:622',
    '132605:624-132605:829',
    '132605:831-132605:968',
    '132606:1-132606:37',
    '132656:1-132656:140',
    '132658:1-132658:177',
    '132659:1-132659:84',
    '132661:1-132661:130',
    '132662:1-132662:130',
    '132662:132-132662:165',
    '132716:220-132716:591',
    '132716:593-132716:640',
    '132959:1-132959:276',
    '132959:278-132959:417',
    '132960:1-132960:190',
    '132961:1-132961:427',
    '132965:1-132965:107',
    '132968:1-132968:173',
    '133029:101-133029:115',
    '133029:129-133029:350',
    '133031:1-133031:18',
    '133034:131-133034:325',
    '133035:1-133035:306',
    '133036:1-133036:225',
    '133046:1-133046:43',
    '133046:45-133046:323',
    '133158:65-133158:786',
    '133321:1-133321:383',
    )

    )


process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag_data.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(1)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(5)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0
process.shyftAnalysis.muPtMin = cms.double( '10' )
process.shyftAnalysis.muonIdTight.cutsToIgnore.append('RelIso')
process.shyftAnalysis.cutsToIgnore = cms.vstring('Trigger')


