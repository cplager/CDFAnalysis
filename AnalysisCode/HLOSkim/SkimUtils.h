// -*- C++ -*-

#include "TChain.h"
#include "HighLevelObjects/HighLevelObjects.hh"
#include "CLPClasses/CLPClasses.h"
#include "JetUser/JetEnergyCorrections.hh"

extern void setUserString (int index, const std::string& value);
//extern TChain* chainFromList (const std::string& filename);
//extern TChain* chainOneFile (const std::string& filename);
extern void isData(bool data);
extern CLPElectron clpElectron (int index, TClonesArray *elecTcaPtr);
extern CLPMuon clpMuon (int index, TClonesArray *muonTcaPtr);
extern CLPTrack clpTrack (int index, TClonesArray *trackTcaPtr);
extern CLPJet clpJet (int index, TClonesArray *jetTcaPtr,
                      JetEnergyCorrections &corrections,
                      JetEnergyCorrections &corrections7);
extern CLPHEPGObject clpHEPGObject (int index, TClonesArray *hepgTcaPtr);
// returns a vector of daughter indicies
extern CLPNamespace::IVec getHepgDaughters (int index, 
                                            TClonesArray *hepgTcaPtr);
extern void dumpHepg (TClonesArray *hepgTcaPtr, int depth = 30);
extern void fillHEPGCont (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr);
extern void fillHEPGContWithZs (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr);
extern void doHEPGMatching (CLPHEPGCont &cont, TClonesArray &elecTca,
                            TClonesArray &muonTca, TClonesArray &jetTca,
                            TClonesArray &trackTca);

extern bool hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
                       TClonesArray &trackTca, TClonesArray &jetTca);

