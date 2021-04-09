
import FWCore.ParameterSet.Config as cms

#print "You have loaded Analysis/FullFW_SHyFT/SHYFTAnalyzer_cfi.py\n"
                                     
from PhysicsTools.SelectorUtils.wplusjetsAnalysis_cfi import wplusjetsAnalysis

shyftAnalyzer = cms.EDFilter("SHYFTAnalyzer",

                             verbose = cms.untracked.bool(False),
                             
                             selectionDetails = wplusjetsAnalysis.clone(muonSrc = cms.InputTag('selectedPatMuons'),
                                                                        electronSrc = cms.InputTag('selectedPatElectrons'),
                                                                        jetSrc = cms.InputTag('selectedPatJets'),
                                                                        metSrc = cms.InputTag('patMETs'),
                                                                        trigSrc = cms.InputTag('patTriggerEvent'),
                                                                        jetPtMin = cms.double(30.0),
                                                                        jetEtaMax = cms.double(2.4)
                                                                        ) ,
                             );
