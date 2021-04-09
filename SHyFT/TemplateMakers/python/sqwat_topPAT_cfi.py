import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.wplusjetsAnalysis_cfi import wplusjetsAnalysis

shyftAnalysis = cms.PSet(

    wplusjetsAnalysis.clone(     
    muonSrc = cms.InputTag('tightPatMuonsPF'),
    electronSrc = cms.InputTag('selectedPatElectronsPF'),
    jetSrc = cms.InputTag('goodPatJetsPF'),
    metSrc = cms.InputTag('patMETsPF'),
    trigSrc = cms.InputTag('patTriggerEvent'),
    jetPtMin = cms.double(30.0),
    jetEtaMax = cms.double(2.4)
    ) ,
    
    # Additional parameters specific to shyft_pretag
    maxJets        = cms.int32( 5 ),

    # -1 means all
    maxInclusive = cms.int32(-1),
    maxPassed = cms.int32(-1),
    maxDump   = cms.int32(0),
    printEvery = cms.int32(1000),

    fitNorm        = cms.bool(False),

	skipSelection = cms.bool(True)
    
)
