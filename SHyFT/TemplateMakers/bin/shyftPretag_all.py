import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("FWLitePlots")


process.load('SHyFT.TemplateMakers.shyftPretag_cfi')

process.inputs = cms.PSet (
    fileNames = cms.vstring(
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_rereco_v2.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_jul16.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_prompt_140399_Aug13.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_prompt_142188_Aug13.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_prompt_142537_Aug13.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_prompt_142664_Aug13.root',
    '/uscms/home/lannon/nobackup/dataskim_selv3/dataSkim_selv3_prompt_missing_Aug13.root'
    ),
    # The above file already has good lumi selection applied.
    #    lumisToProcess = cms.untracked.VLuminosityBlockRange()
    )

process.outputs = cms.PSet (
    outputName = cms.string('shyftPretag_all.root')
)

#This parameter controls how many events are processed (-1 = all)
process.shyftAnalysis.maxInclusive = cms.int32(-1)

#This parameter controls how many accepted events we get (-1 = all)
#process.shyftAnalysis.maxPassed = cms.int32(1)

#This parameter controls how many events are dumped in text format to the log file
process.shyftAnalysis.maxDump = cms.int32(-1)

#This parameter controls how often we output a "processed event #" message.
process.shyftAnalysis.printEvery = cms.int32(1000)

process.shyftAnalysis.minJets = 0

#Needed for Selv3 (only for data)
process.shyftAnalysis.pvSelector.maxZ = 24.0


