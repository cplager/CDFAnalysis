#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "TSystem.h"
#include "TROOT.h"
#include "TString.h"
#include "CLPClasses/CLPClasses.h"
#include "SkimFCNC.h"
#include "TClonesArray.h"

using namespace std;

bool hasGoodZs (TClonesArray &elecTca, TClonesArray &muonTca, 
                TClonesArray &trackTca, TClonesArray &jetTca);



int main(int argc, char** argv)
{
   int howMany = 10;
   int which = 12;
   if (argc >= 2)
   {
      howMany = atol (argv[1]);
   }
   if (argc >= 3)
   {
      which = atoi (argv[2]);
   }      
   // set skim name
   TString listname;
   string outputname;
   CLPFunctions::setDataType( CLPNamespace::kMC );
   if (11 == which)
   {
      // all signal via rootd
      CLPFunctions::setUserBitOn (0); // signal MC
      outputname = "/cdf/scratch/cplager/fcnc/all_signal_nt5";
      listname = "all_signal_rootd.list";
      
   } else if (12 == which)
   {
      // Ingyin's ttbar skim
      outputname = "/cdf/scratch/cplager/fcnc/all_smttbar_nt5";
      listname = "all_smttbar.list";
      CLPFunctions::setUserBitOn (0); // signal MC
      CLPFunctions::setUserBitOn (2); // require at least 4 jets
      
   } else if (112 == which)
   {
      // Full ttbar skim
      // I should probably require a tight lepton
      outputname = "/cdf/scratch/cplager/fcnc/all_smttbar_nt5_2";
      listname = "ttopel_rootd.list";
      CLPFunctions::setUserBitOn (2); // require at least 4 jets
      
   } else if (14 == which)
   {
      // Peter Wittich's Z MC
      CLPFunctions::setUserBitOn (3); // find and match HEPG Zs
      outputname = "/cdf/scratch/cplager/fcnc/zmc";
      listname = "Zmc.list";
      
   } else if (13 == which)
   {
      // Z + n parton mc
      CLPFunctions::setUserBitOn (1); // weights according to filename
      CLPFunctions::setUserBitOn (2); // require at least 4 jets
      CLPFunctions::setUserBitOn (3); // find and match HEPG Zs
      outputname =  "/cdf/scratch/cplager/fcnc/all_znjet_nt5m";
      listname = "Znjet_rootd_nt5.list";
   } else if (15 == which)
   {
      // Red's Z + n parton mc
      CLPFunctions::setUserBitOn (1); // weights according to filename
      CLPFunctions::setUserBitOn (2); // require at least 4 jets
      CLPFunctions::setUserBitOn (3); // find and match HEPG Zs
      outputname =  "/cdf/scratch/cplager/fcnc/all_z4jet_red";
      listname = "red_Z4Jet.list";
   } else if (16 == which)
   {
      // Alpgen + Herwig V2
      CLPFunctions::setUserBitOn (1); // weights according to filename
      CLPFunctions::setUserBitOn (2); // require at least 4 jets
      CLPFunctions::setUserBitOn (3); // find and match HEPG Zs
      outputname =  "/cdf/scratch/cplager/fcnc/all_z4jet_alpher2_red";
      listname = "alpher2.list";
   } else if (51 == which)
   {
      // Ingyin's flat SM ttbar
      CLPFunctions::setUserBitOn (0); // signal MC
      outputname =  "/cdf/scratch/cplager/fcnc/all_flat_smttbar";
      listname = "flat.list";
   } else if (201 == which)
   {
      // Electron data
      CLPFunctions::setDataType( CLPNamespace::kData ); // Data, not MC
      CLPFunctions::setUserBitOn (4); // require good Z
      CLPFunctions::setUserBitOn (5); // signal veto
      outputname =  "/cdf/scratch/cplager/fcnc/all_bhel0d";
      listname = "bhel0d.list";
   } else if (202 == which)
   {
      // Muon data
      CLPFunctions::setDataType( CLPNamespace::kData ); // Data, not MC
      CLPFunctions::setUserBitOn (4); // require good Z
      CLPFunctions::setUserBitOn (5); // signal veto
      outputname =  "/cdf/scratch/cplager/fcnc/all_bhmu0d";
      listname = "bhmu0d.list";
   } else {
      //lets not get in trouble overwritting things we don't want to...
      exit(0);
   }

   // modify the output root file appropriately
   if (CLPFunctions::isUserBitOn(2))
   {
      outputname += "_4j";
   }
   if (CLPFunctions::isUserBitOn(4))
   {
      outputname += "_goodZ";
   }
   if (CLPFunctions::isUserBitOn(5))
   {
      outputname += "_nosig";
   }
   if (howMany)
   {
      outputname += Form ("_%d", howMany);
   }

   outputname += ".root";
   
   CLPFunctions::setUserString(0, outputname);

   cout << "howMany " << howMany << endl << "list " << listname << endl;

   SkimFCNC tt( SkimFCNC::chainFromFile ( (const char *) listname ) );
   
   // () to run on everything, (500) to run on 500 events
   // (500, true) to run on 500 events with debug statements
   tt.Loop(howMany, false);

   cout << "Done." << endl;

   return 0;
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
 
