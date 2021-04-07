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

#include "time.h"
#include "CLPClasses/dout.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include "CLPClasses/CLPClasses.h"
#include "JetUser/JetEnergyCorrections.hh"
#include "HighLevelObjects/HighLevelObjects.hh"

using namespace std;
using namespace CLPNamespace;
using namespace CLPFunctions;




void setUserString (int index, const std::string& value)
{
   cout << "setting user string " << index << " to " << value << "." << endl;
   CLPFunctions::setUserString(index, value);
   cout << "Set to " << CLPFunctions::userString(index) << endl;
}

//TChain*
//chainOneFile (const std::string& filename)
//{
//   TAuthenticate::SetGlobalUser("cdfdata");
//   TChain *chainPtr =  new TChain("TopTree");
//   chainPtr->AddFile( filename.c_str(), TChain::kBigNumber );
//   return chainPtr;   
//}
//
//
//TChain*
//chainFromList (const std::string& filename)
//{
//   TAuthenticate::SetGlobalUser("cdfdata");
//   
//   TChain *chain =  new TChain("TopTree");
//
//   RunListClass runList (filename, "");
//   int size = (int) runList.size();
//   cout << "size " << size << endl;
//   for (int loop = 0; loop < size; ++loop)
//   {
//      chain->AddFile( runList.filename(loop), TChain::kBigNumber );
//   } // for loop
//
//   return chain;   
//}

void isData (bool data)
{
   if (data)
   {
      cout << "Is Data" << endl;
      CLPFunctions::setDataType( CLPNamespace::kData );
   } else {
      cout << "Is MC" << endl;
      CLPFunctions::setDataType( CLPNamespace::kMC );
   }
}

CLPElectron clpElectron (int index, TClonesArray *elecTcaPtr)
{
   electron *elecPtr = (electron *) elecTcaPtr->At(index);   
   CLPElectron myElec;
   // Initialize values
   myElec.setIndex          (          index         );
   myElec.setIsolation      ( elecPtr->Isol          );
   myElec.setZ0             ( elecPtr->TrkZ0         );
   myElec.setD0             ( elecPtr->TrkD0         );
   myElec.setFiducial       ( elecPtr->Fiducial      );
   myElec.setAxSeg          ( elecPtr->TrkAxSeg      );
   myElec.setAxHits         ( elecPtr->TrkAxHits     );
   myElec.setStSeg          ( elecPtr->TrkStSeg      );
   myElec.setStHits         ( elecPtr->TrkStHits     );
   myElec.setSiHits         ( elecPtr->PhxTrkSiHits  );
   myElec.setEta            ( elecPtr->Eta           );
   myElec.setPhi            ( elecPtr->Phi           );
   myElec.setCollectionType ( elecPtr->CollType      );
   myElec.setRegion         ( elecPtr->Region        );
   myElec.setEMenergy       ( elecPtr->En            );
   myElec.setTransEnergy    ( elecPtr->Et            );
   myElec.setTransMom       ( elecPtr->TrkPt         );
   myElec.setEoverP         ( elecPtr->EP            );
   myElec.setHadOverEm      ( elecPtr->Hadem         );
   myElec.setLShrTrk        ( elecPtr->LshrTrk       );
   myElec.setCharge         ( elecPtr->Charge        );
   myElec.setDeltaX         ( elecPtr->DeltaX        );
   myElec.setDeltaZ         ( elecPtr->DeltaZ        );
   myElec.setStripChi2      ( elecPtr->StripChi2     );
   myElec.setConversion     ( elecPtr->Conversion    );
   myElec.setPem3x3FitTower ( elecPtr->Pem3x3FitTow  );
   myElec.setPem3x3DetEta   ( elecPtr->Pem3x3DetEta  );
   myElec.setPem3x3Phi      ( elecPtr->Pem3x3Phi     );
   myElec.setPem3x3Chi2     ( elecPtr->Pem3x3Chisq   );
   myElec.setPes2dEta       ( elecPtr->Pes2dEta      );
   myElec.setPes2dPhi       ( elecPtr->Pes2dPhi      );
   myElec.setPes2d5x9U      ( elecPtr->Pes2d5by9U    );
   myElec.setPes2d5x9V      ( elecPtr->Pes2d5by9V    );
   myElec.setPes2dX         ( elecPtr->Pes2dX        );
   myElec.setPes2dY         ( elecPtr->Pes2dY        );
   myElec.setPhxTrkZ0       ( elecPtr->PhxTrkZ0      );
   myElec.setPhxCharge      ( elecPtr->PhxCharge     );
   myElec.setPhxMatch       ( elecPtr->PhxMatch      );
   myElec.setPhxSiHits      ( elecPtr->PhxTrkSiHits  );
   myElec.setPx             ( elecPtr->Px            );
   myElec.setPy             ( elecPtr->Py            );
   myElec.setPz             ( elecPtr->Pz            );
   myElec.setTrackIdOrig    ( elecPtr->TrkIDOrig     );
   // calculate what needs to be calculated
   myElec.calcQuantities();
   return myElec;
}

CLPMuon clpMuon (int index, TClonesArray *muonTcaPtr)
{
   muon *muonPtr = (muon *) muonTcaPtr->At(index);
   CLPMuon myMuon;
   // Initialize values
   myMuon.setIndex          (          index       );
   myMuon.setUncorrPt       ( muonPtr->Pt          );
   myMuon.setTransMom       ( muonPtr->PtCorr      );
   myMuon.setCharge         ( muonPtr->Charge      );
   myMuon.setFiducial       ( muonPtr->Fiducial    );
   myMuon.setIsolation      ( muonPtr->Isol        );
   myMuon.setAxSeg          ( muonPtr->TrkAxSeg    );
   myMuon.setAxHits         ( muonPtr->TrkAxHits   );
   myMuon.setStSeg          ( muonPtr->TrkStSeg    );
   myMuon.setStHits         ( muonPtr->TrkStHits   );
   myMuon.setSiHits         ( muonPtr->TrkSiHits   );
   myMuon.setEta            ( muonPtr->Eta         );
   myMuon.setPhi            ( muonPtr->Phi0        );
   myMuon.setZ0             ( muonPtr->Z0          );
   myMuon.setD0             ( muonPtr->D0          );
   myMuon.setViewType       ( muonPtr->ViewType    );
   myMuon.setMuonType       ( muonPtr->muontype    );
   myMuon.setCmuDx          ( muonPtr->CmuDx       );
   myMuon.setCmpDx          ( muonPtr->CmpDx       );
   myMuon.setCmxDx          ( muonPtr->CmxDx       );
   myMuon.setEMenergy       ( muonPtr->EmEnergy    );
   myMuon.setHadEnergy      ( muonPtr->HadEnergy   );
   myMuon.setPx             ( muonPtr->PxCorr      );
   myMuon.setPy             ( muonPtr->PyCorr      );
   myMuon.setPz             ( muonPtr->PzCorr      );
   myMuon.setTrackIdOrig    ( muonPtr->TrkIDOrig   );
   // new for Gen 5
   myMuon.setInBluebeam     ( muonPtr->inBluebeam  );
   myMuon.setInMiniskirt    ( muonPtr->inMiniskirt );
   myMuon.setInKeystone     ( muonPtr->inKeystone  );
   myMuon.setCmuFidZ        ( muonPtr->CmuFidZ     );
   myMuon.setCmpFidZ        ( muonPtr->CmpFidZ     );
   myMuon.setCmxFidZ        ( muonPtr->CmxFidZ     );
   myMuon.setCmuFidX        ( muonPtr->CmuFidX     );
   myMuon.setCmpFidX        ( muonPtr->CmpFidX     );
   myMuon.setCmxFidX        ( muonPtr->CmxFidX     );

   // calculate stuff
   myMuon.calcQuantities();
   return myMuon;
}

CLPTrack clpTrack (int index, TClonesArray *trackTcaPtr)
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
   // calculate stuff
   track.calcQuantities();
   return track;
}

CLPJet clpJet (int index, TClonesArray *jetTcaPtr,
               JetEnergyCorrections &corrections, 
               JetEnergyCorrections &corrections7)
{   
   jet *jetPtr = (jet *) jetTcaPtr->At(index);
   CLPJet myJet;
   Double_t correctionScale =
      corrections.doEnergyCorrections(jetPtr->Pt, jetPtr->EmFraction,
                                      jetPtr->EtaDetector);   
   Double_t correctionScale7 =
      corrections7.doEnergyCorrections(jetPtr->Pt, jetPtr->EmFraction,
                                       jetPtr->EtaDetector);   
   myJet.setIndex            (         index                     );
   myJet.setCorrectionScale  ( CLPJet::kLevel5, correctionScale  );
   myJet.setCorrectionScale  ( CLPJet::kLevel7, correctionScale7 );
   myJet.setEtaDetector      ( jetPtr->EtaDetector               );
   myJet.setEmFraction       ( jetPtr->EmFraction                );
   myJet.setCollectionType   ( jetPtr->CollType                  );
   myJet.setBTag             ( jetPtr->secvTag                   );
   myJet.setLooseBTag        ( jetPtr->secvLooseTag              );
   // set both massive and massless jets
   myJet.setMassiveMom4Vec   (jetPtr->Px, jetPtr->Py, jetPtr->Pz, 
                              jetPtr->E, jetPtr->Et);
   myJet.setMasslessMomVec   (jetPtr->CentroidEt,
                              jetPtr->CentroidEta,
                              jetPtr->CentroidPhi);
   // calculate stuff
   myJet.calcQuantities();
   return myJet;
}

CLPHEPGObject clpHEPGObject (int index, TClonesArray *hepgTcaPtr)
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

CLPNamespace::IVec getHepgDaughters (int index, TClonesArray *hepgTcaPtr)
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

void dumpHepg (TClonesArray *hepgTcaPtr, int depth)
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

void fillHEPGContWithZs (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr)
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
   //cout << endl << cont << endl;
}

void fillHEPGCont (CLPHEPGCont &cont, TClonesArray *hepgTcaPtr)
{
   cont.clear();
   // do your thing
   CLPHEPGObject::HEPList alpha, beta;
   int hasZ = 0;
   int hasW = 0;
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
      CLPNamespace::IVec indexVec = getHepgDaughters (index, hepgTcaPtr);
      int topIndex = hepList.size(); 
      int numDaughters = (int) indexVec.size();
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
            hasW  = index;
            // tell bosonIndex 
            bosonIndex = (int) hepList.size() - 1;
         } // isa W
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
  
   CLPHEPGObject::HEPList totalList;
        
   CLPHEPGObject::HEPList *firstPtr  = (CLPHEPGObject::HEPList *) 0;
   CLPHEPGObject::HEPList *secondPtr = (CLPHEPGObject::HEPList *) 0;
   if ( (6 == hasZ) && (7 == hasW) )
   {
      firstPtr = &alpha;
      secondPtr = &beta;
   } // Z first
   else if ( (7 == hasZ) && (6 == hasW) )
   {
      firstPtr = &beta;
      secondPtr = &alpha;
   } // W first
   else
   {
      // this should only happen when we use SM ttbar
      firstPtr = &alpha;
      secondPtr = &beta;
   }
   // make sure both first and second aren't empty
   if (! firstPtr->size() || ! secondPtr->size())
   {
      return;
   }
        
   CLPHEPGObject::HEPList &first  = *firstPtr;
   CLPHEPGObject::HEPList &second = *secondPtr;
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


void doHEPGMatching (CLPHEPGCont &cont, TClonesArray &elecTca,
                     TClonesArray &muonTca, TClonesArray &jetTca, 
                     TClonesArray &trackTca)
{
  
   //CLP3Vector::setOutput3Mode(CLP3Vector::kStandardOutputMode);

   // Lets try to match all electrons, muons, and non-top quarks
   int size = cont.size();
   for (int outer = 0; outer < size; ++outer)
   {
      CLPHEPGObject &hepg = cont(outer);
      TClonesArray *tcaPtr = (TClonesArray *) 0;
      CLPObject::ObjectType type = CLPObject::kUndefined;
      // is this an electron?
      bool isaLepton = false;
      if (hepg.isaElectron())
      {         
         tcaPtr = &elecTca;
         type = CLPObject::kElectron;
         isaLepton = true;
      }
      // is this a muon?
      else if (hepg.isaMuon())
      {
         tcaPtr = &muonTca;
         type = CLPObject::kMuon;
         isaLepton = true;
      }
      // is this a non-top quark
      else if (hepg.notaTopQuark())
      {
         tcaPtr = &jetTca;
         type = CLPObject::kJet;
      }
      // don't bother continuing unless we've satisfied one of the top
      // three categories
      if (! tcaPtr)
      {
         continue;
      }
      // get the appropriate three vector
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
            if (isaLepton)
            {
               // electron or muon uses eta-phi
               diff = etaPhiDiff;            
            } else {
               // jet uses |delta momentum vector|
               diff = (objMom - hepg3mom).mag();
            }
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
         // tell the HEPG object
         hepg.setNumPossibleTags (numPossible);
         hepg.setReconType (type);
         hepg.setTaggedToReconIndex (match);
         hepg.setMom3Vec( objPtr->mom3Vec() );
         //dout << "type " << (int) objPtr->type() << endl
         //     << "3mom " << objPtr->mom3Vec() << endl
         //     << "4mom " << objPtr->HEPGMom4Vec() << endl;
      }

      if (isaLepton) //we have an electron or muon
      {
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
               if (lepPtr->charge() == hepg.charge())
               {
                  status |= 1 << CLPHEPGObject::kChargesMatch;
               }
            } // if lepton matched
            if (matchTrack >= 0)
            {
               status |= 1 << CLPHEPGObject::kIsTrackMatched;
               trkPtr = (CLPTrack*) trackTca.At(matchTrack);
               if (trkPtr->charge() == hepg.charge())
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
      } // if (isaLepton)      
   } // for outer
}

bool hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
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
 
