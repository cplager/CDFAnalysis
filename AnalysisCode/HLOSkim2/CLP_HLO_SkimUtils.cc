// -*- C++ -*-
#include "TH2.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TAuthenticate.h"
#include "TChain.h"
#include "TTree.h"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>

#include "CLPClasses/dout.h"
#include "CLPClasses/CLPClasses.h"
#include "AnalysisNamespace/AnalysisNamespace.hh"
#include "RootFileCollection/TopTreeNamespace.h"
#include "JetUser/JetEnergyCorrections.hh"
#include "HighLevelObjects/HighLevelObjects.hh"
#include "CLP_HLO_SkimUtils.h"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;
using namespace AnalysisNamespace;
using namespace TopTreeNamespace;

double               CLP_HLO_SkimUtils::ns_sumEt              = 0.;
double               CLP_HLO_SkimUtils::ns_hardwiredPtCutoff  = -1.;
double               CLP_HLO_SkimUtils::ns_hardwiredEtaCut    = 10.;
bool                 CLP_HLO_SkimUtils::ns_useOldMistag       = false;
bool                 CLP_HLO_SkimUtils::ns_debugFlag          = false;
jetMistagWinter2004 *CLP_HLO_SkimUtils::ns_2004looseMistagPtr = 0;
jetMistagWinter2004 *CLP_HLO_SkimUtils::ns_2004tightMistagPtr = 0;
jetMistagJan2007    *CLP_HLO_SkimUtils::ns_2007looseMistagPtr = 0;
jetMistagJan2007    *CLP_HLO_SkimUtils::ns_2007tightMistagPtr = 0;
jetMistagJan2007    *CLP_HLO_SkimUtils::ns_2007ultraMistagPtr = 0;

int
CLP_HLO_SkimUtils::f2i (float value) 
{
   const float kEpsilon = 1e-6;
   int sign = 1;
   if (value < 0) 
   {
      sign = -1;
      value *= -1;
   }
   int retval = (int) (value + kEpsilon);
   return sign * retval;
}

CLPElectron 
CLP_HLO_SkimUtils::clpElectron (int index, TClonesArray *elecTcaPtr)
{
   electron *elecPtr = (electron *) elecTcaPtr->At(index);   
   CLPElectron myElec;
   // Initialize values
   myElec.setIndex          (          index           );
   myElec.setIsolation      ( elecPtr->Isol            );
   myElec.setZ0             ( elecPtr->TrkZ0           );
   myElec.setD0             ( elecPtr->TrkD0           );
   myElec.setFiducial       ( elecPtr->Fiducial        );
   myElec.setAxSeg          ( elecPtr->TrkAxSeg        );
   myElec.setAxHits         ( elecPtr->TrkAxHits       );
   myElec.setStSeg          ( elecPtr->TrkStSeg        );
   myElec.setStHits         ( elecPtr->TrkStHits       );
   myElec.setSiHits         ( elecPtr->TrkSiHits       );
   myElec.setEta            ( elecPtr->Eta             );
   myElec.setPhi            ( elecPtr->Phi             );
   myElec.setCollectionType ( elecPtr->CollType        );
   myElec.setRegion         ( elecPtr->Region          );
   myElec.setEMenergy       ( elecPtr->En              );
   myElec.setTransEnergy    ( elecPtr->Et              );
   myElec.setTransMom       ( elecPtr->TrkPt           );
   myElec.setEoverP         ( elecPtr->EP              );
   myElec.setHadOverEm      ( elecPtr->Hadem           );
   myElec.setLShrTrk        ( elecPtr->LshrTrk         );
   myElec.setCharge         ( f2i (elecPtr->Charge )   );
   myElec.setDeltaX         ( elecPtr->DeltaX          );
   myElec.setDeltaZ         ( elecPtr->DeltaZ          );
   myElec.setStripChi2      ( elecPtr->StripChi2       );
   myElec.setConversion     ( elecPtr->Conversion      );
   myElec.setPem3x3FitTower ( elecPtr->Pem3x3FitTow    );
   myElec.setPem3x3DetEta   ( elecPtr->Pem3x3DetEta    );
   myElec.setPem3x3Phi      ( elecPtr->Pem3x3Phi       );
   myElec.setPem3x3Chi2     ( elecPtr->Pem3x3Chisq     );
   myElec.setPes2dEta       ( elecPtr->Pes2dEta        );
   myElec.setPes2dPhi       ( elecPtr->Pes2dPhi        );
   myElec.setPes2d5x9U      ( elecPtr->Pes2d5by9U      );
   myElec.setPes2d5x9V      ( elecPtr->Pes2d5by9V      );
   myElec.setPes2dX         ( elecPtr->Pes2dX          );
   myElec.setPes2dY         ( elecPtr->Pes2dY          );
   myElec.setPhxTrkZ0       ( elecPtr->PhxTrkZ0        );
   myElec.setPhxCharge      ( f2i (elecPtr->PhxCharge) );
   myElec.setPhxMatch       ( elecPtr->PhxMatch        );
   myElec.setPhxSiHits      ( elecPtr->PhxTrkSiHits    );
   myElec.setPx             ( elecPtr->Px              );
   myElec.setPy             ( elecPtr->Py              );
   myElec.setPz             ( elecPtr->Pz              );
   myElec.setTrackIdOrig    ( elecPtr->TrkIDOrig       );
   myElec.setCutCode        ( elecPtr->CutCode         );
   myElec.clearPassesCuts ();
   // calculate what needs to be calculated
   myElec.calcQuantities();
   // check cuts
   if (! myElec.isUndefined())
   {
      CLPCuts::ElectronResult result = CLPCuts::electronCuts (myElec);
      // tight cuts
      if ( result.none() )
      {
         // Passed all cuts
         myElec.orSetPasssesCuts (1);
      } 
   } // electron is defined
   return myElec;
}

CLPMuon 
CLP_HLO_SkimUtils::clpMuon (int index, TClonesArray *muonTcaPtr)
{
   muon *muonPtr = (muon *) muonTcaPtr->At(index);
   CLPMuon myMuon;
   // Initialize values
   myMuon.setIndex       (          index        );
   myMuon.setUncorrPt    ( muonPtr->Pt           );
   myMuon.setTransMom    ( muonPtr->PtCorr       );
   myMuon.setCharge      ( f2i (muonPtr->Charge) );
   myMuon.setFiducial    ( muonPtr->Fiducial     );
   myMuon.setIsolation   ( muonPtr->Isol         );
   myMuon.setAxSeg       ( muonPtr->TrkAxSeg     );
   myMuon.setAxHits      ( muonPtr->TrkAxHits    );
   myMuon.setStSeg       ( muonPtr->TrkStSeg     );
   myMuon.setStHits      ( muonPtr->TrkStHits    );
   myMuon.setSiHits      ( muonPtr->TrkSiHits    );
   myMuon.setEta         ( muonPtr->Eta          );
   myMuon.setPhi         ( muonPtr->Phi0         );
   myMuon.setZ0          ( muonPtr->Z0           );
   myMuon.setD0          ( muonPtr->D0           );
   myMuon.setViewType    ( muonPtr->ViewType     );
   myMuon.setMuonType    ( muonPtr->muontype     );
   myMuon.setCmuDx       ( muonPtr->CmuDx        );
   myMuon.setCmpDx       ( muonPtr->CmpDx        );
   myMuon.setCmxDx       ( muonPtr->CmxDx        );
   myMuon.setEMenergy    ( muonPtr->EmEnergy     );
   myMuon.setHadEnergy   ( muonPtr->HadEnergy    );
   myMuon.setPx          ( muonPtr->PxCorr       );
   myMuon.setPy          ( muonPtr->PyCorr       );
   myMuon.setPz          ( muonPtr->PzCorr       );
   myMuon.setTrackIdOrig ( muonPtr->TrkIDOrig    );
   // new for Gen 5                              
   myMuon.setInBluebeam  ( muonPtr->inBluebeam   );
   myMuon.setInMiniskirt ( muonPtr->inMiniskirt  );
   myMuon.setInKeystone  ( muonPtr->inKeystone   );
   myMuon.setCmuFidZ     ( muonPtr->CmuFidZ      );
   myMuon.setCmpFidZ     ( muonPtr->CmpFidZ      );
   myMuon.setCmxFidZ     ( muonPtr->CmxFidZ      );
   myMuon.setCmuFidX     ( muonPtr->CmuFidX      );
   myMuon.setCmpFidX     ( muonPtr->CmpFidX      );
   myMuon.setCmxFidX     ( muonPtr->CmxFidX      );
   myMuon.setCutCode     ( muonPtr->CutCode      );
   myMuon.clearPassesCuts ();

   // calculate stuff
   myMuon.calcQuantities();
   // check cuts
   if (! myMuon.isUndefined())
   {
      CLPCuts::MuonResult result = CLPCuts::muonCuts (myMuon);
      // tight cuts
      if ( result.none() )
      {
         // Passed all cuts
         myMuon.orSetPasssesCuts (1);
      } 
   } // muon is defined
   return myMuon;
}

CLPTrack 
CLP_HLO_SkimUtils::clpTrack (int index, TClonesArray *trackTcaPtr)
{
   offltrack *trackPtr =  (offltrack *) trackTcaPtr->At(index);
   CLPTrack track;
   // Initialize values
   track.setIndex          (           index             );
   track.setTransMom       ( trackPtr->Pt                );
   track.setBCTransMom     ( trackPtr->BCPt              );
   track.setCharge         ( trackPtr->Charge            );
   track.setTrackIsol      ( trackPtr->trkIsol           );
   track.setAxSeg          ( trackPtr->NumAxSeg          );
   track.setAxHits         ( trackPtr->NumCTHitsAx       );
   track.setStSeg          ( trackPtr->NumStSeg          );
   track.setStHits         ( trackPtr->NumCTHitsSt       );
   track.setSiHits         ( trackPtr->SIHits            );
   track.setEta            ( trackPtr->Eta               );
   track.setPhi            ( trackPtr->Phi0              );
   track.setZ0             ( trackPtr->Z0                );
   track.setD0             ( trackPtr->D0                );
   track.setPx             ( trackPtr->BCPx              );
   track.setPy             ( trackPtr->BCPy              );
   track.setPz             ( trackPtr->BCPz              );
   track.setCollectionType ( trackPtr->CollType          );
   track.setCorrD0         ( trackPtr->corrD0            );
   track.setTrackIdOrig    ( trackPtr->IdOrig            );
   track.setExpectedSiHits ( trackPtr->SiExpectedLayers  );
   track.setCalEmTower     ( trackPtr->CalEmTower        );
   track.setCalHadTower    ( trackPtr->CalHadTower       );
   track.clearPassesCuts ();
   // calculate stuff
   track.calcQuantities();
   // check cuts
   if (! track.isUndefined())
   {
      CLPCuts::TrackResult result = CLPCuts::trackCuts (track);
      // tight cuts
      if ( result.none() )
      {
         // Passed all cuts
         track.orSetPasssesCuts (1);
      } 
   } // track is defined
   return track;
}

CLPJet 
CLP_HLO_SkimUtils::clpJet (int index, TClonesArray *jetTcaPtr,
                           JetEnergyCorrections *jetCorrArray[kTotalNumJetCorr])
{   
   jet *jetPtr = (jet *) jetTcaPtr->At(index);
   CLPJet myJet;
   myJet.setIndex            (         index        );
   myJet.setEtaDetector      ( jetPtr->EtaDetector  );
   myJet.setEmFraction       ( jetPtr->EmFraction   );
   myJet.setCollectionType   ( jetPtr->CollType     );
   // set tagging information
   myJet.setBTag ( CLPJet::kTight, jetPtr->secvTag           );
   myJet.setBTag ( CLPJet::kLoose, jetPtr->secvLooseTag      );
   myJet.setBTag ( CLPJet::kUltra, jetPtr->secvUltratightTag );
   // set both massive and massless jets
   myJet.setMassiveMom4Vec   (jetPtr->Px, jetPtr->Py, jetPtr->Pz, 
                              jetPtr->En, jetPtr->Et);
   myJet.setMasslessMom4Vec  (jetPtr->CentroidEt,
                              jetPtr->CentroidEta,
                              jetPtr->CentroidPhi);
   // calculate stuff
   myJet.calcQuantities();
   // Before going on, is this a jet we want
   if (myJet.isUndefined())
   {
      // Don't bother going on
      return myJet;
   }

   /////////////////////
   // Jet Corrections //
   /////////////////////
   int offset;
   if (CLPJet::k04 == myJet.jetCone())
   {
      offset = 0;      
   //} else if (CLPJet::k07 == myJet.jetCone())
   //{
   //   offset = CLPJet::kNumJetCorrections;      
   } else {
      dout << "Shouldn't be here" << endl;
      return myJet;
   }
   for (int loop = 0; loop < CLPJet::kNumJetCorrections; ++loop)
   {
      Double_t correctionScale = (*jetCorrArray[offset + loop])
         .doEnergyCorrections(jetPtr->Pt, 
                              jetPtr->EmFraction,
                              jetPtr->EtaDetector);
      myJet.setCorrectionScale ( loop, correctionScale);
   } // for loop

   // if this is data, don't bother doing mistag stuff
   if ( isData() )
   {
      return myJet;
   }

   ////////////////////////////////////
   // Loading BTagObject Information //
   ////////////////////////////////////
   if (CLPJet::k04 == myJet.jetCone())
   {
      // only count cone 0.4 jets
      ns_sumEt += jetPtr->Et;
   } else {
      return myJet;
   }
   // Same cuts for 2004 and 2007 parameterization
   if (jetPtr->Et > 10 * GEV && 
       std::abs (jetPtr->Eta) < 2.4)
   {
      if (ns_useOldMistag)
      {
         // 2004
         // Loose
         ns_2004looseMistagPtr->cache (jetPtr->secvLooseTag,
                                       jetPtr->secvLooseNtrk,
                                       jetPtr->Et,
                                       jetPtr->Eta,
                                       jetPtr->Phi,
                                       myJet.index());
         // Tight
         ns_2004tightMistagPtr->cache (jetPtr->secvTag,
                                       jetPtr->secvNtrk,
                                       jetPtr->Et,
                                       jetPtr->Eta,
                                       jetPtr->Phi,
                                       myJet.index());
      } 
      else
      {
         // 2007
         int nvert = ns_tfi.GetEntries (kZvtxsBranch);
         privertex * pvPtr = (privertex *) ns_tfi.At (kPrivertexBranch, 0);
         float pzv = pvPtr->SecVtxVz;
         // Loose
         ns_2007looseMistagPtr->cache (jetPtr->secvLooseTag, 
                                       jetPtr->secvLooseNtrk,
                                       jetPtr->Et,
                                       jetPtr->Eta,
                                       nvert,
                                       pzv,
                                       myJet.index());
         // Tight
         ns_2007tightMistagPtr->cache (jetPtr->secvTag, 
                                       jetPtr->secvNtrk,
                                       jetPtr->Et,
                                       jetPtr->Eta,
                                       nvert,
                                       pzv,
                                       myJet.index());
         // Ultratight
         ns_2007ultraMistagPtr->cache (jetPtr->secvUltratightTag, 
                                       jetPtr->secvUltratightNtrk,
                                       jetPtr->Et,
                                       jetPtr->Eta,
                                       nvert,
                                       pzv,
                                       myJet.index());
         if (ns_debugFlag)
         {
            cout << "  " << myJet << endl;
            // Loose
            cout << "    loose:"
                 << "  " << jetPtr->secvLooseTag 
                 << "  " << jetPtr->secvLooseNtrk
                 << "  " << jetPtr->Et
                 << "  " << jetPtr->Eta
                 << "  " << nvert
                 << "  " << pzv
                 << "  " << myJet.index() 
                 << endl;
            // Tight
            cout << "    tight:"
                 << "  " << jetPtr->secvTag 
                 << "  " << jetPtr->secvNtrk
                 << "  " << jetPtr->Et
                 << "  " << jetPtr->Eta
                 << "  " << nvert
                 << "  " << pzv
                 << "  " << myJet.index() 
                 << endl;
            // Ultratight
            cout << "    ultra:"
                 << "  " << jetPtr->secvUltratightTag 
                 << "  " << jetPtr->secvUltratightNtrk
                 << "  " << jetPtr->Et
                 << "  " << jetPtr->Eta
                 << "  " << nvert
                 << "  " << pzv
                 << "  " << myJet.index()
                 << endl;
         } // if debug
      } // else (2007)
   } // meets minimum jet requirements
   return myJet;
}

CLPHEPGObject 
CLP_HLO_SkimUtils::clpHEPGObject (int index, TClonesArray *hepgTcaPtr)
{
   hepg *hepgPtr = (hepg *) hepgTcaPtr->At(index);
   CLPHEPGObject object;
   CLP4Vector mom (hepgPtr->Px, hepgPtr->Py, hepgPtr->Pz, hepgPtr->E);
   object.setHEPGMom4Vec  (          mom    );
   object.setIndex        (          index  );
   object.setHEPGtype     ( hepgPtr->ID     );
   object.setCharge       ( hepgPtr->Charge );
   return object;
}

CLPNamespace::IVec 
CLP_HLO_SkimUtils::getHepgDaughters (int index, TClonesArray *hepgTcaPtr)
{
   CLPNamespace::IVec retval;
   int numHepgObjs = hepgTcaPtr->GetEntriesFast();
   // don't bother checking if index <= 0 or index >= numHepgObjs
   if ((index <= 0) || (index >= numHepgObjs))
   {
      // return an empty vector
      return retval;
   }
   // we're trying to find the daughters of particle 'index'.
   // let's start looking at particle 'index + 1'
   bool foundAny = false;
   for (int loop = index + 1; loop < numHepgObjs; ++loop)
   {
      hepg *hepgPtr = (hepg *) hepgTcaPtr->At(loop);
      // does this particle have 'index' as (one of) its mother(s)?
      if ( ( (index == hepgPtr->Mo1) || (index == hepgPtr->Mo2) ) &&
           (3 == hepgPtr->Stdhep) )
      {
         // we got one
         retval.push_back( loop );
         foundAny = true;
      }
      else
      {
         // if we haven't found any yet, then there's nothing to worry
         // about.  If we have previously found some daughters and
         // this one is is not one, we should stop looking now
         if (foundAny)
         {
            // this means we have found some
            // Let's stop looking
            break;
         } // if found any
      } // not a daughter
   } // for loop
   return retval;
}

void 
CLP_HLO_SkimUtils::dumpHepg (TClonesArray *hepgTcaPtr, int depth)
{
   int upto = hepgTcaPtr->GetEntriesFast();
   if (upto > depth)
   {
      upto = depth;
   }
   cout << " Num    ID    Mass   Charge  Stdhep IDparent Mo1   Mo2  "
        << " Da1   Da2   E        Px     Py     Pz    Ind" << endl;

   for(Int_t loop = 0; loop < upto; loop++)
   {
      hepg *hepgPtr = (hepg *) hepgTcaPtr->At(loop);
	   cout << std::setw(5) << loop 
            << " " << setw(5) << hepgPtr->ID
            << " " << setw(7) << hepgPtr->Mass
            << " " << Form ("%4.1f", hepgPtr->Charge)
            << " " << setw(6) << hepgPtr->Stdhep
            << " " << setw(5) << hepgPtr->IDparent
            << " " << setw(5) << hepgPtr->Mo1
            << " " << setw(5) << hepgPtr->Mo2
            << " " << setw(5) << hepgPtr->Da1
            << " " << setw(5) << hepgPtr->Da2
            << " " << Form ("%8.2f", hepgPtr->E)
            << " " << Form ("%8.2f", hepgPtr->Px)
            << " " << Form ("%8.2f", hepgPtr->Py)
            << " " << Form ("%8.2f", hepgPtr->Pz)
            << " " << setw(5) << hepgPtr->Ind
            << endl;
   } // for loop   
   cout << endl;
}

void 
CLP_HLO_SkimUtils::fillHEPGContWithZs (CLPHEPGCont &cont, 
                                       TClonesArray *hepgTcaPtr)
{
   cont.clear();
   int zIndex = -1;
   CLPNamespace::IVec dautVec;
   for (int index = 0; index <= 20; ++index)
   {
      hepg *hepgPtr =  (hepg *) hepgTcaPtr->At(index);
      if (!HEPGNames::isaTop ( hepgPtr->ID ))
      {
         dautVec = getHepgDaughters (index, hepgTcaPtr);
         if (2 == dautVec.size())
         {
            zIndex = index;
            break;
       }
      }
   } // for index
   if (zIndex < 0)
   {
      // Sorry, no suitable Z's here
      return;
   }
   // So, if we're still here this means that we:
   // - found a HEPG Z
   // - it has two daughters
   CLPHEPGObject zed = clpHEPGObject (zIndex, hepgTcaPtr);
   zed.setInternalIndex (0); // the Z is 0th
   zed.setFirstDaughter (1); // first daughter is 1st
   zed.setNumDaughters ( (int) dautVec.size() );
   cont.add (zed);
   for (int inner = 0; inner < (int) dautVec.size(); ++inner)
   {
      CLPHEPGObject part = clpHEPGObject (dautVec[inner], hepgTcaPtr);
      part.setInternalIndex ( 1 + inner); // 1st, 2nd, etc.     
      part.setParent (0); // to 0th particle, the Z
      cont.add (part);
   } // for inner
}

void 
CLP_HLO_SkimUtils::fillHEPGCont (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr)
{
   cont.clear();
   // do your thing
   CLPHEPGObject::HEPList alpha, beta;
   int hasZ = 0;
   int hasW = 0;
   int hasW2 = 0;
   ///////////////////////////////
   // Loop over both top quarks //
   ///////////////////////////////
   for (int index = 6; index <= 7; ++index)
   {
      CLPHEPGObject::HEPList *ptr = &alpha;
      if (7 == index)
      {
         ptr = &beta;
      }
      CLPHEPGObject::HEPList &hepList = *ptr;
      hepg *hepgPtr =  (hepg *) hepgTcaPtr->At(index);
      if (!HEPGNames::isaTop ( hepgPtr->ID ))
      {
         // if this isn't a top, not all is right with the world
         cout << "NOTA TOP" << endl;
         return;
      }
      CLPHEPGObject top = clpHEPGObject (index, hepgTcaPtr);
      IVec indexVec     = getHepgDaughters (index, hepgTcaPtr);
      int topIndex      = hepList.size(); 
      int numDaughters  = (int) indexVec.size();
      top.setInternalIndex( topIndex );
      if (numDaughters)
      {
         top.setFirstDaughter( topIndex + 1 );
         top.setNumDaughters( numDaughters );
      }
      hepList.push_back (top);
      int bosonIndex = -1;
      for (unsigned int inner = 0; inner < indexVec.size(); ++inner)
      {
         // make a new hepg object from hepg banks
         CLPHEPGObject daughter = clpHEPGObject (indexVec[inner], hepgTcaPtr);
         // make sure we set up the internal counting right
         daughter.setInternalIndex( (int) hepList.size() );
         // this daughter is from the top
         daughter.setParent (topIndex);
         // push it on
         hepList.push_back (daughter);
         // is this one of the bosons
         if (daughter.isaZ())
         {
            // tell hasZ which one has the Z
            hasZ = index;
            // tell bosonIndex 
            bosonIndex = (int) hepList.size() - 1;
         } // isa Z
         if (daughter.isaW())
         {
            // tell hasW which one has the W
            if (! hasW)
            {
               hasW  = index;
            } else {
               hasW2 = index;
            }
            // tell bosonIndex 
            bosonIndex = (int) hepList.size() - 1;
         } // isa W
         // '37' is a H+ in Ingyin's "flat" MC
         if (37 == abs(daughter.HEPGtype()))
         {
            // tell hasW which one has the W
            hasZ  = index;
            // tell bosonIndex 
            bosonIndex = (int) hepList.size() - 1;
         } // isa W
      } // for inner
        // Did we find a boson
      if (bosonIndex < 0)
      {
         // Yes, we have no bananas
         // Don't keep going on
         return;
      }
      // find the daughters of the boson
      indexVec = getHepgDaughters (hepList[bosonIndex].index(), hepgTcaPtr);
      numDaughters = (int) indexVec.size();
      if (numDaughters)
      {
         hepList[bosonIndex].setNumDaughters( numDaughters );
         hepList[bosonIndex].setFirstDaughter( (int) hepList.size() );
      }
      for (unsigned int inner = 0; inner < indexVec.size(); ++inner)
      {
         CLPHEPGObject daughter = clpHEPGObject (indexVec[inner], hepgTcaPtr);
         // make sure we set up the internal counting right
         daughter.setInternalIndex( (int) hepList.size() );
         // this daughter is from the boson
         daughter.setParent (bosonIndex);
         // push it on
         hepList.push_back ( daughter );
      } // for inner
   } // for index
  
   ///////////////////////////////////////////////////////////
   // Order the daughters (Z before W, leptons before jets) //
   ///////////////////////////////////////////////////////////
   CLPHEPGObject::HEPList *firstPtr  = (CLPHEPGObject::HEPList *) 0;
   CLPHEPGObject::HEPList *secondPtr = (CLPHEPGObject::HEPList *) 0;
   if ( (6 == hasZ) && (7 == hasW) )
   {
      // Z was first
      firstPtr  = &alpha;
      secondPtr = &beta;
   } // Z first
   else if ( (7 == hasZ) && (6 == hasW) )
   {
      // W was first
      firstPtr  = &beta;
      secondPtr = &alpha;
   } // W first
   else
   {
      // this should only happen when we use SM ttbar (WbWb) OR ZcZc MC
      firstPtr  = &alpha;
      secondPtr = &beta;
      // IFF secondPtr has leptons AND firstPtr has hadrons, switch them.
      if (firstPtr->size() >= 4 && secondPtr->size() >= 4)
      {
         CLPHEPGObject &firstObj = firstPtr->at(3);
         CLPHEPGObject &secondObj  = secondPtr->at(3);
         if (firstObj.isaQuark() &&
             (secondObj.isaLightLepton() || secondObj.isaLightLeptonNu()))
         {
            std::swap (firstPtr, secondPtr);
         }
      } // sizes o.k.
   }
   // make sure both first and second aren't empty
   if (! firstPtr->size() || ! secondPtr->size())
   {
      return;
   }
        
   CLPHEPGObject::HEPList &first  = *firstPtr;
   CLPHEPGObject::HEPList &second = *secondPtr;
   CLPHEPGObject::HEPList totalList;        
   // add the first entry from the second list
   totalList.push_back( second[0] );
   // adjust the first list by 1
   CLPHEPGObject::increaseAllInternal (first, 1);
   // add the first list to the total
   for (unsigned int loop = 0; loop < first.size(); ++loop)
   {
      totalList.push_back( first[loop] );
   } // for loop
     // calculate how much to increase
   int increase = (int) totalList.size() - 1;
   // increase the second list by 'increase'
   CLPHEPGObject::increaseAllInternal (second, increase);
   // Here's the deal, we're not adding all of them to the list,
   // but rather starting from the second item (index 1) and adding
   // the rest.
   // The problem is that things that have the the 0th item as their
   // mother will now claim they have the 'increase'th as their mother.
   // add the second list, starting from the 2nd object [1]
   for (unsigned int loop = 1; loop < second.size(); ++loop)
   {
      // check to see if the parent is the top
      if (second[loop].parent() == increase)
      {
         // we need to point this to the 0th particle
         second[loop].setParent(0);
      }
      totalList.push_back( second[loop] );
   } // for loop
   // have the very first object point to the correct daughter
   totalList[0].setFirstDaughter( increase + 1 );
   cont.fill(totalList);
}

void 
CLP_HLO_SkimUtils::doLeptonHEPGMatching (CLPHEPGCont &cont, 
                                    TClonesArray &elecTca,
                                    TClonesArray &muonTca, 
                                    TClonesArray &trackTca)
{
   // Lets try to match all electrons, muons, and non-top quarks
   int size = cont.size();
   for (int outer = 0; outer < size; ++outer)
   {
      CLPHEPGObject &hepg = cont(outer);
      TClonesArray *tcaPtr = (TClonesArray *) 0;
      CLPObject::ObjectType type = CLPObject::kUndefined;
      // is this an electron?
      if (hepg.isaElectron())
      {         
         tcaPtr = &elecTca;
         type = CLPObject::kElectron;
      }
      // is this a muon?
      else if (hepg.isaMuon())
      {
         tcaPtr = &muonTca;
         type = CLPObject::kMuon;
      }
      // don't bother continuing unless we've satisfied one of the top
      // three categories
      if (! tcaPtr)
      {
         continue;
      }

      // get the appropriate three and four vectors
      CLP3Vector hepg4mom = hepg.HEPGMom4Vec();
      CLP3Vector hepg3mom = (CLP3Vector) hepg.HEPGMom4Vec();
      // vector of possible matches
      int tcaSize = tcaPtr->GetEntriesFast();
      // loop through the possible entries
      int match = -1;
      int numPossible = 0;
      double minDiff = 999.;
      for (int inner = 0; inner < tcaSize; ++inner)
      {
         CLPObject *objPtr = (CLPObject *) tcaPtr->At(inner);
         CLP3Vector objMom = objPtr->mom3Vec();
         double etaPhiDiff = objMom.etaPhiDiff (hepg3mom);
         if (etaPhiDiff < 0.4)
         {
            ++numPossible;
            double diff;
            diff = etaPhiDiff;            
            // if this is our first match OR
            // this is the best match so far, take it
            if ((match < 0) || (diff < minDiff))
            {
               match = inner;
               minDiff = diff;
            }
         } // within cone of 0.4
      } // for inner

      // do we have a match
      if (match >= 0)
      {
         // tell the CLPObject
         CLPObject *objPtr = (CLPObject *) tcaPtr->At(match);
         objPtr->setTaggedToHEPGIndex( hepg.internalIndex() );
         objPtr->setHEPGMom4Vec( hepg4mom );
         
         // Tell the HEPG object UNLESS we have a jet that is not a
         // cone size 0.4
         hepg.setNumPossibleTags (numPossible);
         hepg.setReconType (type);
         hepg.setTaggedToReconIndex (match);
         hepg.setMom3Vec( objPtr->mom3Vec() );
      }

      ///////////////////////////
      // Lepton/Track Matching //
      ///////////////////////////
      // new matching variables
      int matchTrack = -1;
      double minDiffTrack = 999.;
      int numPossibleTracks = 0;
      
      for (int inner = 0; inner < trackTca.GetEntriesFast(); ++inner)
      {
         CLPObject *objPtr = (CLPObject *) trackTca.At(inner);
         CLP3Vector objMom = objPtr->mom3Vec();
         double etaPhiDiff = objMom.etaPhiDiff (hepg3mom);
         if (etaPhiDiff < 0.4)
         {
            ++numPossibleTracks;
            // eta-phi diff
            double diff = etaPhiDiff;
            // if this is our first match OR
            // this is the best match so far, take it
            if ((matchTrack < 0) || (diff < minDiffTrack))
            {
               matchTrack = inner;
               minDiffTrack = diff;
            }
         } // within cone of 0.4
      } // for inner
      
      // do we have a match for tracks
      if (matchTrack >= 0)
      {
         // tell the CLPObject
         CLPObject *trackObjPtr = (CLPObject *) trackTca.At(matchTrack);
         trackObjPtr->setTaggedToHEPGIndex( hepg.internalIndex() );
         trackObjPtr->setHEPGMom4Vec( hepg4mom );
         
         // tell HEPG stuff
         hepg.setNumPossibleTrackTags (numPossibleTracks);
         hepg.setTaggedToTrackIndex (matchTrack); 
         hepg.setTrack3Vec( trackObjPtr->mom3Vec());
         
      } // if we matched a track
      // Let's check the status
      int status = 0;
      if (match >= 0 || matchTrack >= 0)
      {
         status |= 1 << CLPHEPGObject::kIsMatched; 
         CLPChargedParticle *lepPtr = (CLPChargedParticle *) 0;
         CLPTrack *trkPtr = (CLPTrack *) 0;
         if (match >= 0)
         {
            status |= 1 << CLPHEPGObject::kIsLeptonMatched; 
            // check charge
            lepPtr = (CLPChargedParticle *) tcaPtr->At(match);
            if (lepPtr->charge() == f2i( hepg.charge() ) )
            {
               status |= 1 << CLPHEPGObject::kChargesMatch;
            }
         } // if lepton matched
         if (matchTrack >= 0)
         {
            status |= 1 << CLPHEPGObject::kIsTrackMatched;
            trkPtr = (CLPTrack*) trackTca.At(matchTrack);
            if (trkPtr->charge() == f2i( hepg.charge() ) )
            {
               status |= 1 << CLPHEPGObject::kChargesMatch;
            }
         } // if track matched
         // Do we have a dual match?
         if (lepPtr && trkPtr)
         {
            // do they use the same track?
            if (lepPtr->trackIdOrig() == trkPtr->trackIdOrig())
            {
               status |=  1 << CLPHEPGObject::kTrackLeptonMatch;
            }
         }
      } // if either matched
      hepg.setMatchStatus (status);
   } // for outer
}

void 
CLP_HLO_SkimUtils::doJetHEPGMatching (CLPHEPGCont &cont,
                                      CLPJet::JetPtrList &jetPtrList)
{

   // Lets try to match all electrons, muons, and non-top quarks
   int size = cont.size();
   for (int outer = 0; outer < size; ++outer)
   {
      CLPHEPGObject &hepg = cont(outer);
      CLPObject::ObjectType type = CLPObject::kUndefined;
      if (! hepg.notaTopQuark())
      {
         continue;
      }

      // get the appropriate three and four vectors
      CLP3Vector hepg4mom = hepg.HEPGMom4Vec();
      CLP3Vector hepg3mom = (CLP3Vector) hepg.HEPGMom4Vec();
      // loop through the possible entries
      int match = -1;
      int numPossible = 0;
      double minDiff = 999.;
      int size = (int) jetPtrList.size();
      for (int inner = 0; inner < size; ++inner)
      {
         CLPJet *jetPtr = jetPtrList[inner];
         CLP3Vector jetMom = jetPtr->mom3Vec();
         double etaPhiDiff = jetMom.etaPhiDiff (hepg3mom);
         if (etaPhiDiff < 0.4)
         {
            ++numPossible;
            double diff;
            // jet uses |delta momentum vector|
            diff = (jetMom - hepg3mom).mag();
            // if this is our first match OR
            // this is the best match so far, take it
            if ((match < 0) || (diff < minDiff))
            {
               match = inner;
               minDiff = diff;
            }
         } // within cone of 0.4
      } // for inner

      // do we have a match
      if (match >= 0)
      {
         CLPJet *jetPtr = jetPtrList[match];
         jetPtr->setTaggedToHEPGIndex( hepg.internalIndex() );
         jetPtr->setHEPGMom4Vec( hepg4mom );
         
         // Tell the HEPG object UNLESS we have a jet that is not a
         // cone size 0.4.  We do tell 0.7 and 1.0 cone jets that they
         // are matched to HEPG particles.

         // is this the correct cone size?
         if (CLPJet::k04 != jetPtr->jetCone())
         {
            // Don't tell HEPG about this one
            continue;
         }
         hepg.setNumPossibleTags (numPossible);
         hepg.setReconType (type);
         hepg.setTaggedToReconIndex (match);
         hepg.setMom3Vec( jetPtr->mom3Vec() );
      }
   } // for outer
}

bool 
CLP_HLO_SkimUtils::hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
                              TClonesArray &trackTca, TClonesArray &jetTca)
{
   CLPElectron::ElectronList elecList;
   CLPMuon::MuonList muonList;
   CLPTrack::TrackList trackList;
   CLPJet::JetList jetList;

   ///////////////
   // Electrons //
   ///////////////
   elecList.clear();
   int numTCAelecs = elecTca.GetEntriesFast();
   for (int elecLoop = 0; elecLoop < numTCAelecs; ++elecLoop)
   {
      CLPElectron *elecPtr = (CLPElectron*) elecTca.At(elecLoop);
      if (elecPtr->isaTCE() || elecPtr->isaPHX())
      {
         CLPCuts::ElectronResult result = CLPCuts::electronCuts(*elecPtr);
         // tight cuts
         if ( result.none() )
         {
            elecList.push_back( *elecPtr );
         } 
      } // is a cem
   } // for elecLoop
   
   ///////////
   // Muons //
   ///////////
   muonList.clear();
   int numTCAmuons = muonTca.GetEntriesFast();
   for (int muonLoop = 0; muonLoop < numTCAmuons; ++muonLoop)
   {
      CLPMuon *muonPtr = (CLPMuon*) muonTca.At(muonLoop);
      CLPCuts::MuonResult result = CLPCuts::muonCuts( *muonPtr);
      // for now, we are only allowing tight leptons
      if ( result.none() && muonPtr->isaTightLepton())
      {
         muonList.push_back( *muonPtr );
      }
   } // for muonLoop
   

   ////////////
   // Tracks //
   ////////////
   trackList.clear();
   int numTCAtracks = trackTca.GetEntriesFast();
   for (int trackLoop = 0; trackLoop < numTCAtracks; ++trackLoop)
   {
      CLPTrack *trackPtr = (CLPTrack*) trackTca.At(trackLoop);
      CLPCuts::TrackResult result = CLPCuts::trackCuts( *trackPtr);
      if ( result.none() )
      {
         trackList.push_back( *trackPtr );
      } //if it passed all cuts
   } // for trackLoop

   //lets try to match any tracks to the electrons and muons that
   //have passed the buts above.
   CLPTrack::matchElectron (trackList, elecList); 
   CLPTrack::matchMuon     (trackList, muonList);
      
   //////////
   // Jets //
   //////////
   jetList.clear();
   int numTCAjets = jetTca.GetEntriesFast();
   for (int jetLoop = 0; jetLoop < numTCAjets; ++jetLoop)
   {
      CLPJet *jetPtr = (CLPJet*) jetTca.At(jetLoop);
      CLPCuts::JetResult result = CLPCuts::jetCuts( *jetPtr);
      if ( result.none() )
      {
         jetList.push_back( *jetPtr );
      }
   } // for jetLoop

   ///////////////////////////////
   // Remove Jet-Track Overlaps //
   ///////////////////////////////
   CLPJet::JetList tempList;
   int jetSize = (int) jetList.size();
   int trkSize = (int) trackList.size();
   for (int jetIndex = 0; jetIndex < jetSize; ++jetIndex)
   {
      CLP3Vector jetVec = jetList[jetIndex].mom3Vec();
      bool ok = true;
      for (int trkIndex = 0; trkIndex < trkSize; ++trkIndex)
      {
         CLP3Vector trkVec = trackList[trkIndex].mom3Vec();
         if ( jetVec.etaPhiDiff( trkVec ) < 0.4)
         {
            // it's too close to a track
            ok = false;
            break;
         }
      } // for trkIndex
      if (ok) 
      {
         tempList.push_back( jetList[jetIndex] );
      }
   } // for jetIndex
   jetList = tempList;

   //////////////////////////////////////////
   // Sort the jets from high Et to low Et //
   //////////////////////////////////////////

   // since we want them in decending order, we use a little trick
   // and sort 'rbegin' to 'rend' instead of 'begin' to 'end'.
 
   sort (jetList.rbegin(), jetList.rend());

   // veto signal
   if (CLPFunctions::isUserBitOn(5))
   {
      // We veto events IFF there are three or more "good" jets and
      // the leading jet Et > 30 GeV
      if ( (jetList.size() >= 3) &&
           (jetList[0].Et() > 30 * GEV) )
      {
         // we don't want this event
         return false;
      }
   }
  
   int size = (int) elecList.size();
   int trackSize = (int) trackList.size();
   ///////////////////////
   // Electron-Electron //
   ///////////////////////
   for (int outer = 0; outer < size - 1; ++outer)
   {
      CLP4Vector outer4mom = elecList[outer].mom4Vec();
      int outerSign = elecList[outer].charge();

      for (int inner = outer + 1; inner < size; ++inner)
      {
         // Is at least one of these tight?  We want to continue if
         // BOTH are NOT tight (i.e. ! && !).
         if (! elecList[outer].isaTightLepton() &&
             ! elecList[inner].isaTightLepton() )
         {
            // we currently want at least one tight
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = elecList[inner].charge();
         if ( (1 == outerSign * innerSign) )
         {
            // we don't want same sign (unless we have gNoQ set)
            continue;
         }
         CLP4Vector inner4mom = elecList[inner].mom4Vec();
         double zMass = (outer4mom + inner4mom).mass();
         if ((zMass > 76 * GEV) && (zMass < 106 * GEV))
         {
            return true;
         }
      } // for inner electron
   } // for outer electron

   ////////////////////
   // Electron-Track //
   ////////////////////
   for (int outer = 0; outer < size; ++outer)
   {
      // is this a tight lepton
      if (! elecList[outer].isaTightLepton())
      {
         // don't bother going any further
         continue;
      }
      CLP4Vector outer4mom = elecList[outer].mom4Vec();
      int outerSign = elecList[outer].charge();
      for (int inner = 0;inner < trackSize; ++inner)
      {
         // don't bother if this track is already matched to an
         // electron
         if (trackList[inner].isMatchedToElectron() || 
             trackList[inner].isMatchedToMuon() )
         {
            // Next!
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = trackList[inner].charge();
         if ( (1 == outerSign * innerSign) )
         {
            // we don't want same sign (unless we have gNoQ set)
            continue;
         }
         // we want to treat this track as an electron
         CLP4Vector inner4mom = 
            trackList[inner].mom4Vec(CLPTrack::kAsElectronMom);
         double zMass = (outer4mom + inner4mom).mass();
         if ((zMass > 76 * GEV) && (zMass < 106 * GEV))
         {
            return true;
         }
      } // for inner track
   } // for outer electron

   ///////////////
   // Muon-Muon //
   ///////////////
   size = (int) muonList.size();
   for (int outer = 0; outer + 1 < size; ++outer)
   {
      CLP4Vector outer4mom = muonList[outer].mom4Vec();
      int outerSign = muonList[outer].charge();
      for (int inner = outer + 1; inner < size; ++inner)
      {
         // check the signs (allowing for 0 sign)
         int innerSign = muonList[inner].charge();
         if ( 1 == outerSign * innerSign)
         {
            // we don't want same sign (unless we have gNoQ set)
            continue;
         }
         CLP4Vector inner4mom = muonList[inner].mom4Vec();
         double zMass = (outer4mom + inner4mom).mass();
         if ((zMass > 76 * GEV) && (zMass < 106 * GEV))
         {
            return true;
         }
      } // for inner muon
   } // for outer muon

   ////////////////
   // Muon-Track //
   ////////////////
   for (int outer = 0; outer < size; ++outer)
   {
      // is this a tight lepton
      if (! muonList[outer].isaTightLepton() )
      {
         // we currently want tight, tight, so don't bother
         continue;
      }
      CLP4Vector outer4mom = muonList[outer].mom4Vec();
      int outerSign = muonList[outer].charge();
      for (int inner = 0;inner < trackSize; ++inner)
      {
         // don't bother if this track is already matched to an
         // muon
         if (trackList[inner].isMatchedToElectron() || 
             trackList[inner].isMatchedToMuon() )
         {
            // Next!
            continue;
         }
         // check the signs (allowing for 0 sign)
         int innerSign = trackList[inner].charge();
         if (1 == outerSign * innerSign)
         {
            // we don't want same sign
            continue;
         }
         // we want to treat this track as an muon
         CLP4Vector inner4mom = 
            trackList[inner].mom4Vec(CLPTrack::kAsMuonMom);
         double zMass = (outer4mom + inner4mom).mass();
         if ((zMass > 76 * GEV) && (zMass < 106 * GEV))
         {
            return true;
         }
      } // for inner track
   } // for outer muon


   return false;
}

void 
CLP_HLO_SkimUtils::hepgHeavyFlavorCheck (bool &hasBottom, bool &hasCharm, 
                                         TClonesArray *hepgTcaPtr)
{
   // This routine searches through the HEPG bank and finds both charm
   // and bottom.  It is used to remove charm and bottom from
   // light-flavored samples and bottom from the charm samples.
   //
   // NOTE: For Gen 6 MC, there is a hardwired cut at 8 GeV in the
   // heavy flavor generation of the W + ccbar/bbar + jets MC.  There
   // is no such cut for Z + heavy flavor MC.  In addition, there is
   // an |eta| cut at 3.0 for W + jets MC and at 10 for Z + jets
   // MC. So:
   //
   // W + jets: ns_hardwiredPtCutoff =  8.; // GEV
   //           ns_hardwiredEtaCut   =  3.;
   // Z + jets: ns_hardwiredPtCutoff =  0.; // GEV
   //           ns_hardwiredEtaCut   = 10.;
   //
   // If you are taking this code to run without CLPClasses (if you
   // don't know, then you do), then you want to make the following
   // changes:
   // 'CLP3Vector' -> 'TVector3'

   hasBottom = hasCharm = false;
   // Even though these are STL vectors, I call them lists because I
   // don't want to confuse them with the 3 vectors.
   vector< CLP3Vector > bottomList, charmList;
   // loop over all HEPG particles
   int numHepg = hepgTcaPtr->GetEntries();
   for (int loop = 0; loop < numHepg; ++loop)
   {
      hepg *hepgPtr = (hepg*) hepgTcaPtr->At (loop);

      // is this charm?
      if ( ! hasCharm && (0 == hepgPtr->Interaction) && 
          (4 == abs (hepgPtr->ID)) && (2 == hepgPtr->Stdhep) )
      {
         CLP3Vector mom (hepgPtr->Px, hepgPtr->Py, hepgPtr->Pz);
         // // cout << "unconfirmed charm" << mom << endl;
         if ( (mom.Pt() > ns_hardwiredPtCutoff) &&
              (std::abs ( mom.eta() ) < ns_hardwiredEtaCut) )
         {
            hasCharm = true;
            // cout << "charm   " << mom << endl;
         } else {
            // put it on the list IFF it passes the eta cuts
            if (std::abs ( mom.eta() ) < ns_hardwiredEtaCut)
            {
               charmList.push_back (mom);
            }
         } // else if
      } // is charm
      
      // is this bottom?
      if ( ! hasBottom && (0 == hepgPtr->Interaction) && 
          (5 == abs (hepgPtr->ID)) && (2 == hepgPtr->Stdhep) )
      {
         CLP3Vector mom (hepgPtr->Px, hepgPtr->Py, hepgPtr->Pz);
         // cout << "unconfirmed bottom" << mom << endl;
         if ( mom.Pt() > ns_hardwiredPtCutoff &&
              std::abs ( mom.eta() ) < ns_hardwiredEtaCut )
         {
            hasBottom = true;
            // cout << "bottom  " << mom << endl;
         } else {
            // put it on the list IFF it passes the eta cuts
            if (std::abs ( mom.eta() ) < ns_hardwiredEtaCut)
            {
               bottomList.push_back (mom);
            }
         } // else if
      } // is bottom
      
      // If we've found both, stop looking
      if (hasBottom && hasCharm)
      {
         return;
      }
   } // for loop   

   // If we don't have a Pt cutoff, there's no reason to continue
   if (ns_hardwiredPtCutoff <= 0) return;


   // If we're still here, we need to check to see if we can make any
   // 8 GeV jets.  This is only allowed if the jets are relatively
   // co-linear (i.e., have a deltaR < 0.4)
   int numCharm  = (int) charmList.size();
   if (numCharm > 8)
   {
      hasCharm = true;
   }
   if (! hasCharm)
   {
      for (int outer = 0; outer < numCharm - 1; ++outer)
      {
         for (int inner = outer + 1; inner < numCharm; ++inner)
         {
            CLP3Vector twoVec = charmList[inner] + charmList[outer];
            double deltaRdiff = 
               charmList[inner].etaPhiDiff( charmList[outer] );            
            if ( (deltaRdiff < 0.4) &&
                 (twoVec.Pt() > ns_hardwiredPtCutoff) &&
                 (std::abs ( twoVec.eta() ) < ns_hardwiredEtaCut) )
            {
               hasCharm = true;
               // break out of both loops
               outer = numCharm + 1;
               break;
            } // if found charm
         } // for inner
      } // for oute
   } // no charm

   int numBottom  = (int) bottomList.size();
   if (numBottom > 8)
   {
      hasBottom = true;
   }
   if (! hasBottom)
   {
      for (int outer = 0; outer < numBottom - 1; ++outer)
      {
         for (int inner = outer + 1; inner < numBottom; ++inner)
         {
            CLP3Vector twoVec = bottomList[inner] + bottomList[outer];
            double deltaRdiff = 
               bottomList[inner].etaPhiDiff( bottomList[outer] );            
            if ( (deltaRdiff < 0.4) &&
                 (twoVec.Pt() > ns_hardwiredPtCutoff) &&
                 (std::abs ( twoVec.eta() ) < ns_hardwiredEtaCut) )
            {
               hasBottom = true;
               // break out of both loops
               outer = numBottom + 1;
               break;
            } // if found bottom
         } // for inner
      } // for oute
   } // no bottom
}

double 
CLP_HLO_SkimUtils::jetBtagObspAccounting (TClonesArray *clpjetTcaPtr,
                                          TClonesArray *obspTcaPtr,
                                          int runnumber)
{
   // if there are no jets, don't bother
   int numJets = clpjetTcaPtr->GetEntries();
   if (! numJets)
   {
      return 0.;
   }

   const double kSF_loose = 0.95; // check this
   const double kSF_tight = 0.95; // check this
   const double kSF_ultra = 0.88; // check this
   const float kBTagScaleFactors [CLPJet::kNumTagTypes] =
      { kSF_loose, kSF_tight, kSF_ultra };
   // Third term should be replaced by ns_ultraMistagPtr when it exists
   jetMistagWinter2004 *k2004MistagPtrArray [CLPJet::kNumTagTypes] =
      { ns_2004looseMistagPtr, ns_2004tightMistagPtr, ns_2004tightMistagPtr };
   jetMistagJan2007 *k2007MistagPtrArray [CLPJet::kNumTagTypes] =
      { ns_2007looseMistagPtr, ns_2007tightMistagPtr, ns_2007ultraMistagPtr };

   //////////////////////////////////
   // Match OBSP particles to jets //
   //////////////////////////////////
   int numObsp = obspTcaPtr->GetEntries();
   for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
   {
      CLPJet *jetPtr = (CLPJet*) clpjetTcaPtr->At (jetIndex);
      CLPJet &myJet = *jetPtr;
      CLP3Vector jetMom = myJet.mom3Vec();
      // loop over all OBSP particles
      for (int obspIndex = 0; obspIndex < numObsp; ++obspIndex)
      {
         obsp *obspPtr = (obsp*) obspTcaPtr->At (obspIndex);
         bool hasCharm =  hasCharmContent (obspPtr->cdfid);
         bool hasBottom =  hasBottomContent (obspPtr->cdfid);
         if ( !hasCharm && !hasBottom )
         {
            continue;
         }
         // make a three vector for the obsp particle
         CLP3Vector obsp3mom (obspPtr->px, obspPtr->py, obspPtr->pz);
         // make sure this has a Pt of at least 0.1 GeV (to be
         // consistent with the heavy flavor guys)
         if (obsp3mom.Pt() < 0.1 * GEV)
         {
            // Sorry, don't want to use this
            continue;
         }
         // obspIndex over the jets
         double etaPhiDiff = jetMom.etaPhiDiff (obsp3mom);
         // do we have a match?
         if (etaPhiDiff < 0.4)
         {
            // save the matches
            if (hasCharm)
            {
               myJet.setHeavyFlavor (CLPJet::kCharmFlavor);
               // if this jet already has bottom, we're done
               if (myJet.hasBottomFlavor())
               {
                  continue;
               }
            } // this jet is matched to charm
            if (hasBottom)
            {
               myJet.setHeavyFlavor (CLPJet::kBottomFlavor);
               // if this jet already has charm, we're done
               if (myJet.hasCharmFlavor())
               {
                  continue;
               }
            } // this jet is matched to bottom
         } // if etaPhiDiff < 0.4
      } // for obspIndex
   } // for jetIndex

   ////////////////////////////////////////////////
   // Calculate Sum Et and AlphaBeta Corrections //
   ////////////////////////////////////////////////
   // We don't have alphaBeta6 corrections for ultratight tagging, but
   // the conventional wisdom is that the mistag rate is an eighth of
   // that of the tight tagging.
   double alphaBeta[CLPJet::kNumTagTypes] = 
      {alphaBetaCorrLoose (runnumber, ns_sumEt),
       alphaBetaCorrTight (runnumber, ns_sumEt),
       alphaBetaCorrTight (runnumber, ns_sumEt) / 8.0};
       

   ///////////////////////////////////
   // Calculate B Tag probabilities //
   ///////////////////////////////////
   double weight = 1.;
   for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
   {
      CLPJet *jetPtr = (CLPJet*) clpjetTcaPtr->At (jetIndex);
      CLPJet &myJet = *jetPtr;
      // Remember, all jets start out with '0' tag probabilties
      // does this jet have heavy flavor?
      if (myJet.heavyFlavor())
      {
         for (int typeInt = (int) CLPJet::kLoose;
              typeInt < (int) CLPJet::kNumTagTypes;
              ++typeInt)
         {              
            CLPJet::SVXTagType type = (CLPJet::SVXTagType) typeInt;
            // This jet is matched to heavy flavor.  Is it tagged?
            //   - yes: probability to be tagged is  SF
            //   - no:  probability to be tagged is 0 
            if (myJet.isBTagged( type ))
            {
               // Jet is tagged.  Probability to NOT be tagged is 1 - SF
               myJet.setTagProb( type, kBTagScaleFactors[type] );
               // We are keeping track for loose jets, so
               if (CLPJet::kLoose ==  type)
               {
                  weight *= 1 - kBTagScaleFactors[type];
               }
            } // tagged jet
            else 
            {
               myJet.setTagProb( type, 0. );
            } // jet is not tagged
         } // for type
      } // Has heavy flavor
      else 
      {
         // This jet is NOT matched to HF.  So we use the mistag
         // parameters to get the mistag rate.
         // weight *= something;
         // There's only a probability to tag a jet if it has an
         // (uncorrectd) Et of 10 GeV and |eta| < 2.4
         if (myJet.Et (CLPJet::kUncorrected) > 10 * GEV &&
             std::abs (myJet.eta()) < 2.4)
         {
            for (int typeInt = (int) CLPJet::kLoose;
                 typeInt < (int) CLPJet::kNumTagTypes;
                 ++typeInt)
            {              
               CLPJet::SVXTagType type = (CLPJet::SVXTagType) typeInt;
               double mistagProb = 0;
               if (ns_useOldMistag)
               {
                  // 2004
                  double jetNegRate = 
                     (k2004MistagPtrArray[type])->negRate (myJet.index());
                  mistagProb = alphaBeta[typeInt] * jetNegRate;
               }
               else
               {
                  // 2007
                  mistagProb =
                     (k2007MistagPtrArray[type])->mistagRate (myJet.index());
               }
               myJet.setTagProb (type, mistagProb );
               if (CLPJet::kLoose == type) 
               {
                  weight *= 1 - mistagProb;
               } // if loose
            } // for type
         } // > 10 GeV
      } // no heavy flavor
   } // for jetIndex
   return 1 - weight;
}

void
CLP_HLO_SkimUtils::dumpMistagRates (TClonesArray *clpjetTcaPtr)
{
   jetMistagJan2007 *k2007MistagPtrArray [CLPJet::kNumTagTypes] =
      { ns_2007looseMistagPtr, ns_2007tightMistagPtr, ns_2007ultraMistagPtr };
   const string kMistagNames  [CLPJet::kNumTagTypes] =
      { "loose", "tight", "ultra"};
   int numJets = clpjetTcaPtr->GetEntries();
   for (int jetIndex = 0; jetIndex < numJets; ++jetIndex)
   {
      CLPJet *jetPtr = (CLPJet*) clpjetTcaPtr->At (jetIndex);
      CLPJet &myJet = *jetPtr;
      if (myJet.Et (CLPJet::kUncorrected) <= 10 * GEV ||
          std::abs (myJet.eta()) >= 2.4)
      {
         // don't bother
         continue;
      }
      cout << myJet << endl;
      for (int typeInt = 0; typeInt < CLPJet::kNumTagTypes; ++typeInt)
      {              
         CLPJet::SVXTagType type = (CLPJet::SVXTagType) typeInt;         
         cout << "    " << kMistagNames[typeInt] << ": "
              << k2007MistagPtrArray[typeInt]->mistagRate (myJet.index())
              << " : " << jetPtr->tagProb (type)
              << endl;
      } // for typeInt
   } // for jetIndex
} 

inline bool 
CLP_HLO_SkimUtils::hasBottomContent (int cdfid)
{
   const int kBottomID = 5; // fifth quark
   int third     = getThirdDigit  (cdfid);
   int fourth    = getFourthDigit (cdfid); 
   return (kBottomID == third || kBottomID == fourth);
}
 
inline bool 
CLP_HLO_SkimUtils::hasCharmContent (int cdfid)
{
   const int kCharmID = 4; // fourth quark
   int third     = getThirdDigit  (cdfid);
   int fourth    = getFourthDigit (cdfid); 
   return (kCharmID == third || kCharmID == fourth);
}
 
int 
CLP_HLO_SkimUtils::getThirdDigit (int number)
{                                               
   // make it positive
   number = abs (number);
   // get rid of larger digits
   number %= 1000;
   return number / 100;
}

int 
CLP_HLO_SkimUtils::getFourthDigit (int number)
{                                               
   // make it positive
   number = abs (number);
   // get rid of larger digits
   number %= 10000;
   return number / 1000;
}

double 
CLP_HLO_SkimUtils::alphaBetaCorrTight (int runnumber, double sumEt)
{
   // Stolen from Ulrich's FCNCMistagMatrix::alphaBetaCorrTight().
   // Will not be necessary with 1.2 fb^-1 mistag parameterization.
   const double kABcorr[ 42 ] = {
    0.00000, 1.10350, 1.10329, 1.10498, 1.10694, 
    1.11629, 1.14005, 1.19928, 1.27941, 1.32729, 
    1.34660, 1.35511, 1.35677, 1.35550, 1.35550, 
    1.35935, 1.36845, 1.38362, 1.40131, 1.41607, 
    1.42629, 1.43181, 1.43480, 1.43686, 1.43813, 
    1.43866, 1.43937, 1.43977, 1.44012, 1.44020, 
    1.44106, 1.44077, 1.44088, 1.44102, 1.44094,
    1.44106, 1.44164, 1.44101, 1.44073, 1.44131, 
    1.44067, 1.44093
  };
  int bin = (int) (std::abs (sumEt) ) / 10 + 1;
   if (bin > 41)
   {
      bin = 41;
   }
   double factor = 1.;
   if (runnumber > 190696)
   {
      factor = 1.05;
   }
   return kABcorr[ bin ];
}

double 
CLP_HLO_SkimUtils::alphaBetaCorrLoose (int runnumber, double sumEt)
{
   // Stolen from Ulrich's FCNCMistagMatrix::alphaBetaCorrLoose()
   // Will not be necessary with 1.2 fb^-1 mistag parameterization.
   const double kABcorr[ 42 ] = {
    0.00000, 1.19925, 1.19912, 1.20015, 1.20135, 
    1.20705, 1.22151, 1.25753, 1.30638, 1.33541, 
    1.34792, 1.35555, 1.36058, 1.36440, 1.36786, 
    1.37190, 1.37768, 1.38622, 1.39576, 1.40365, 
    1.40907, 1.41197, 1.41352, 1.41462, 1.41529, 
    1.41554, 1.41594, 1.41614, 1.41634, 1.41638, 
    1.41685, 1.41667, 1.41676, 1.41676, 1.41673, 
    1.41683, 1.41716, 1.41678, 1.41657, 1.41679, 
    1.41669, 1.41669
  };
  int bin = (int) (std::abs (sumEt) ) / 10 + 1;
   if (bin > 41)
   {
      bin = 41;
   }
   double factor = 1.;
   if (runnumber > 190696)
   {
      factor = 1.05;
   }
   return kABcorr[ bin ];
}

unsigned int 
CLP_HLO_SkimUtils::checkHeavyFlavor (TClonesArray *hepgTcaPtr,
                                     const TVector3Vec &jet3vecVec)
{
   // Harvard-style ALPGEN overlap removal:
   // -> check if ccbar or bbbar pairs end up in the same reconstructed jet
   // for old-style ALPGEN overlap removal:
   // -> flag events with HEPG charm & beauty after parton shower
   unsigned int heavyFlavorContent = 0;
   vector< hepg* > hepgPtrVec;
   TVector3Vec     hepg3VecVec;
   int numHepg = hepgTcaPtr->GetEntries();
   for (int hepgIndex = 0; hepgIndex < numHepg; ++hepgIndex)
   {
      hepg *hepgPtr = (hepg*) hepgTcaPtr->At (hepgIndex);
      if ( hepgPtr->Interaction != 0 ) 
      {
         continue;
      }
      // store all HEPG entries of interest
      if ( kStdHep_ShowerOrME    == hepgPtr->Stdhep || 
           kStdHep_MatrixElement == hepgPtr->Stdhep ) 
      {
         if ( 4 == fabs( hepgPtr->ID ) || 5 == fabs( hepgPtr->ID ) ) 
         {
            hepgPtrVec.push_back  ( hepgPtr );
            hepg3VecVec.push_back ( TVector3 (hepgPtr->Px, 
                                              hepgPtr->Py, 
                                              hepgPtr->Pz) );
         }
      } else {
         // Nothing else to see here folks.
         continue;
      }
      // check HF content for both matrix element (stdhep=3)
      // and parton shower (stdhep=2)
      if ( kStdHep_ShowerOrME == hepgPtr->Stdhep ) 
      {
         if ( 4 == hepgPtr->ID) 
         {
            heavyFlavorContent |= 1 << kHasHEPGCharm;
            heavyFlavorContent |= 1 << kHasCPartonShower;
         }
         else if ( -4 == hepgPtr->ID) 
         {
            heavyFlavorContent |= 1 << kHasHEPGCharm;
            heavyFlavorContent |= 1 << kHasCbarPartonShower;
         }
         else if ( 5 == hepgPtr->ID) 
         {
            heavyFlavorContent |= 1 << kHasHEPGBottom;
            heavyFlavorContent |= 1 << kHasBPartonShower;
         }
         else if ( -5 == hepgPtr->ID) 
         {
            heavyFlavorContent |= 1 << kHasHEPGBottom;
            heavyFlavorContent |= 1 << kHasBbarPartonShower;
         }
      } //parton shower
      else if ( kStdHep_MatrixElement == hepgPtr->Stdhep ) 
      {
         if ( 4 == hepgPtr->ID ) 
         {
            heavyFlavorContent |= 1 << kHasCMatrixElement;
         }
         else if ( -4 == hepgPtr->ID ) 
         {
            heavyFlavorContent |= 1 << kHasCbarMatrixElement;
         }
         else if ( 5 == hepgPtr->ID  ) 
         {
            heavyFlavorContent |= 1 << kHasBMatrixElement;
         }
         else if ( -5 == hepgPtr->ID ) 
         {
            heavyFlavorContent |= 1 << kHasBbarMatrixElement;
         }
      } // matrix element
   } // for hepgIndex
   
   // check if bbbar or ccbar end up in the same jet
   for (TVector3VecConstIter iter = jet3vecVec.begin();
        jet3vecVec.end() != iter;
        ++iter)
   {
      const TVector3 &jet3Vec = *iter;
      // count number of HF from ME and PS with two maps
      map<int,int> nhfme_map;
      map<int,int> nhfps_map;
      for (int hepgIndex = 0; hepgIndex < (int) hepgPtrVec.size(); ++hepgIndex) 
      {
         hepg *hepgPtr = hepgPtrVec [hepgIndex];
         const TVector3 &hepg3Vec = hepg3VecVec [hepgIndex];

         if ( hepg3Vec.DeltaR( jet3Vec ) < 0.4 ) 
         {
            if ( kStdHep_ShowerOrME == hepgPtr->Stdhep ) 
            {
               ++nhfps_map[hepgPtr->ID];
               // cout << j_list[ij] << endl;
               // cout << "--------> matches to PS quark " 
               //      << hepgPtr->index()
               //      << ", ID:" << hepgPtr->ID << ", IDparent:" 
               //      << hepgPtr->IDparent
               //      << ", Mo1:" << hepgPtr->Mo1
               //      << " with deltaR=" 
               //      << hepgPtr->mom4Vec().DeltaR( j ) << endl;
            }
            else if ( kStdHep_MatrixElement == hepgPtr->Stdhep ) 
            {
               ++nhfme_map[hepgPtr->ID];
               // cout << j_list[ij] << endl;
               // cout << "--------> matches to ME quark " 
               //      << hepgPtr->index()
               //      << ", ID:" << hepgPtr->ID << ", IDparent:" 
               //      << hepgPtr->IDparent
               //      << ", Mo1:" << hepgPtr->Mo1
               //      << " with deltaR=" 
               //      << hepgPtr->mom4Vec().DeltaR( j ) <<  endl;
            }
         } // matches jet
      } // for hepgIndex
      if ( nhfps_map[4] > 0 && nhfps_map[-4] > 0 ) 
      {
         heavyFlavorContent |= 1 << kHasCcbarSameJetPartonShower;
         //cout << "PS ccbar same jet" << endl;
      }
      if ( nhfps_map[5] > 0 && nhfps_map[-5] > 0 ) 
      {
         heavyFlavorContent |= 1 << kHasBbbarSameJetPartonShower;
         //cout << "PS bbbar same jet" << endl;
      }
      if ( nhfme_map[4] > 0 && nhfme_map[-4] > 0 ) 
      {
         heavyFlavorContent |= 1 << kHasCcbarSameJetMatrixElement;
         //cout << "ME ccbar same jet" << endl;
      }
      if ( nhfme_map[5] > 0 && nhfme_map[-5] > 0 ) 
      {
         heavyFlavorContent |= 1 << kHasBbbarSameJetMatrixElement;
         //cout << "ME bbbar same jet" << endl;
      }
   } // for iter

   return heavyFlavorContent;
}


void
CLP_HLO_SkimUtils::initialize2004BTagObjects (const char* tight_mistag_file,
                                              const char* loose_mistag_file)
{
   cout << "Initializing OLD 2004 BTag Objects" << endl 
        << "tight: " << tight_mistag_file << endl
        << "loose: " << loose_mistag_file << endl;
   ns_2004tightMistagPtr = new jetMistagWinter2004 (tight_mistag_file, 
                                                    "tagrates");
   if (! ns_2004tightMistagPtr)
   {
      cerr << "Tight mistag initialization failed" << endl;
      exit (1);
   }
   ns_2004looseMistagPtr = new jetMistagWinter2004 (loose_mistag_file, 
                                                    "tagrates");
   if (! ns_2004looseMistagPtr)
   {
      cerr << "Loose mistag initialization failed" << endl;
      exit (1);
   }
}

void
CLP_HLO_SkimUtils::initialize2007BTagObjects (const char* ultra_mistag_file,
                                              const char* tight_mistag_file,
                                              const char* loose_mistag_file)
{
   cout << "Initializing 2007 BTag Objects" << endl 
        << "ultra: " << ultra_mistag_file << endl
        << "tight: " << tight_mistag_file << endl
        << "loose: " << loose_mistag_file << endl;
   ns_2007ultraMistagPtr = new jetMistagJan2007 (ultra_mistag_file, 
                                                 "tagrates");
   if (! ns_2007ultraMistagPtr)
   {
      cerr << "Ultra mistag initialization failed" << endl;
      exit (1);
   }
   ns_2007tightMistagPtr = new jetMistagJan2007 (tight_mistag_file, 
                                                 "tagrates");
   if (! ns_2007tightMistagPtr)
   {
      cerr << "Tight mistag initialization failed" << endl;
      exit (1);
   }
   ns_2007looseMistagPtr = new jetMistagJan2007 (loose_mistag_file, 
                                                 "tagrates");
   if (! ns_2007looseMistagPtr)
   {
      cerr << "Loose mistag initialization failed" << endl;
      exit (1);
   }
}

void 
CLP_HLO_SkimUtils::clearBTagObjects()
{
   ns_sumEt = 0.;
   if (ns_useOldMistag)
   {
      // 2004
      assert (ns_2004tightMistagPtr);
      ns_2004tightMistagPtr->clear();
      ns_2004looseMistagPtr->clear();
   } 
   else 
   {
      // 2007
      assert (ns_2007tightMistagPtr);
      ns_2007ultraMistagPtr->clear();
      ns_2007tightMistagPtr->clear();
      ns_2007looseMistagPtr->clear();
   }
}

void 
CLP_HLO_SkimUtils::applyMistagMatricies()
{
   if (ns_useOldMistag)
   {
      // 2004
      ns_2004tightMistagPtr->applyMatrix();
      ns_2004looseMistagPtr->applyMatrix();
   }
   else
   {
      // 2007
      ns_2007ultraMistagPtr->SetTagType("ultratight");
      ns_2007tightMistagPtr->SetTagType("tight");
      ns_2007looseMistagPtr->SetTagType("loose");
      ns_2007ultraMistagPtr->applyMatrix();
      ns_2007tightMistagPtr->applyMatrix();
      ns_2007looseMistagPtr->applyMatrix();
      if (ns_debugFlag)
      {
         cout << "Applying 2007 Mistag Stuff" << endl;
         // cout << "ultra:" << endl;
         // ns_2007ultraMistagPtr->print();
         // cout << endl << "tight:" << endl;
         // ns_2007tightMistagPtr->print();
         // cout << endl << "loose:" << endl;
         // ns_2007looseMistagPtr->print();
      }

   } // if 2007
}
