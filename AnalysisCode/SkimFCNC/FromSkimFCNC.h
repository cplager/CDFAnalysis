// -*- C++ -*-

// To be included in TopZ.h



static void setUserString (int index, const std::string& value);
static TChain* chainFromFile (const std::string& filename);
static void isData(bool data);
CLPElectron clpElectron (int index);
CLPMuon clpMuon (int index);
CLPTrack clpTrack (int index);
CLPJet clpJet (int index, JetEnergyCorrections &corrections,
               JetEnergyCorrections &corrections7);
CLPHEPGObject clpHEPGObject (int index);
// returns a vector of daughter indicies
CLPNamespace::IVec getHepgDaughters (int index);
void dumpHepg (int depth = 30);
void fillHEPGCont (CLPHEPGCont &cont);
void fillHEPGContWithZs (CLPHEPGCont &cont);
void doHEPGMatching (CLPHEPGCont &cont, TClonesArray &elecTca,
                     TClonesArray &muonTca, TClonesArray &jetTca,
                     TClonesArray &trackTca);
