// -*- C++ -*-


// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/FlavorHistory.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Math/GenVector/PxPyPzM4D.h"

#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"

// Necessary for selector
#include "PhysicsTools/SelectorUtils/interface/WPlusJetsEventSelector.h"
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"


// Root includes
#include "TROOT.h"
#include "TH2F.h"

using namespace std;
using optutl::CommandLineParser;

enum
{
   kDataMode   = -1,
   kNormalMode =  0,
   kVqqMode,
   kLFMode,
   kWcMode
};

const string kElectron = "_el";
const string kMuon     = "_mu";

//////////////////////////
// Forward Declarations //
//////////////////////////

// This subroutine, written by you (below), uses the command line
// arguments and creates an output tag (if any).  This subroutine must
// exist.
void outputNameTagFunc (string &tag);

// Book all histograms to be filled this job.  If wanted, you can skip
// this subroutine and book all histograms in the main subroutine.
void bookHistograms (fwlite::ChainEvent &eventCont, 
                     const std::string &leptonType,
                     edm::ParameterSet const& shyftParameters,
                     fwlite::TFileService& fs,
                     std::map<TString, TH1*>& histograms);

// Calculate the name that should be used for this event based on the
// mode, the HF word, and (if necessary), whether or not it's a W or
// Z.  Returns false if the event should not be processed.
bool calcSampleName (fwlite::ChainEvent &eventCont, string &sampleName,
                     edm::ParameterSet const& shyftParameters );

///////////////////////////
// ///////////////////// //
// // Main Subroutine // //
// ///////////////////// //
///////////////////////////


int mainRoutine (int argc, char* argv[])
{

   ////////////////////////////////
   // ////////////////////////// //
   // // Command Line Options // //
   // ////////////////////////// //
   ////////////////////////////////
   
   // load framework libraries
// what does this do? I have no idea.
//   gSystem->Load( "libFWCoreFWLite" );
   AutoLibraryLoader::enable();

   if ( argc < 2 ) {
      std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
      return 2;
   }
   
   // Get the python configuration
   cout << "Unpacking configuation parameters." << endl;
   PythonProcessDesc builder(argv[1]);
   edm::ParameterSet const& shyftParameters = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
   edm::ParameterSet const& inputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
   edm::ParameterSet const& outputs = builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");

// removed   
//    // Tell people what this analysis code does and setup default options.
//    CommandLineParser parser ("Creates SecVtx Mass templates");
// 
//      //////////////////////////////////////////////////////
//    // Add any command line options you would like here //
//    //////////////////////////////////////////////////////
//    parser.addOption ("mode",           CommandLineParser::kInteger, 
//                      "Normal MC (0), VQQ (1), LF (2), Wc (3), Data (-1)", 
//                      0);   
//    parser.addOption ("sampleName",     CommandLineParser::kString, 
//                      "Sample name (e.g., top, Wqq, etc.)");   
//    parser.addOption ("selectorConfig", CommandLineParser::kString, 
//                      "Selector configuration file",
//                      "shyftSimple_cfg.py");   
// 
//    ////////////////////////////////////////////////
//    parser.stringValue ("outputFile") = "templates"; // .root added automatically
// 
//    // This object 'eventCont' is used both to get all information from the
//    // eventCont as well as to store histograms, etc.
//    // Parse the command line arguments
//    parser.parseArguments (argc, argv);
// 
   //////////////////////////////////
   // //////////////////////////// //
   // // Create Event Container // //
   // //////////////////////////// //
   //////////////////////////////////
   // removed
   // fwlite::EventContainer eventCont (parser);


   // load the chain event and do some validation on it
   std::vector<std::string> fileNames = inputs.getParameter<std::vector<std::string> > ("fileNames");
   fwlite::ChainEvent ev( fileNames );
   // 
   // Immediately check to see if you have no events in your chain
     
   cout << "Checking to see if we have any events in the files you requested... " << endl;
     
   bool chainIsValid = ev.isValid();
   Long64_t chainSize = ev.size();
     
   cout << "Is valid?  " << chainIsValid << endl
        << "Size?      " << chainSize << endl;
     
   if ( ! ( chainIsValid && (chainSize > 0) ) ) {
        
      cout << "Chain is not valid or size is not > 0... quitting"
           << endl;
        
      return -10;
        
   }
   // make the TFileService
   fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

   // make the histogram map
   std::map<TString, TH1*> histograms;

   // configure the selector
   cout << "Initialize WPlusJetsEventSelector" << endl;
   WPlusJetsEventSelector wPlusJetsSelector( shyftParameters );
   pat::strbitset ret = wPlusJetsSelector.getBitTemplate();
   cout << "Cuts defined:" << endl;
   wPlusJetsSelector.printActiveCuts(cout);

   ////////////////////////////////////////
   // ////////////////////////////////// //
   // //         Begin Run            // //
   // // (e.g., book histograms, etc) // //
   // ////////////////////////////////// //
   ////////////////////////////////////////

   // Setup a style
   gROOT->SetStyle ("Plain");

   // Book those histograms!
   bookHistograms (ev, kElectron, shyftParameters, fs,histograms);
   bookHistograms (ev, kMuon, shyftParameters, fs, histograms);

   ////////////////////
   // Selector setup //
   ////////////////////
//    PythonProcessDesc builder ( parser.stringValue ("selectorConfig").c_str() );
//    boost::shared_ptr<edm::ProcessDesc> b = builder.processDesc();
//    boost::shared_ptr<edm::ParameterSet> parameters = b->getProcessPSet();
//    parameters->registerIt(); 
//   edm::ParameterSet const& shyftParameters = 
//      builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
//    WPlusJetsEventSelector wPlusJetsSelector (shyftParameters);
//    std::strbitset ret = wPlusJetsSelector.getBitTemplate();
//     // pull out references to input/output/shyftAnalysis code
//    edm::ParameterSet const& shyftParameters = \
//       builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("shyftAnalysis");
//    edm::ParameterSet const& inputs = \
//       builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("inputs");
//    edm::ParameterSet const& outputs = \
//       builder.processDesc()->getProcessPSet()->getParameter<edm::ParameterSet>("outputs");
//    
// 

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   int numFailed = 0;
   int mode = shyftParameters.getParameter<int>("mode");
   //loop through each event
   cout << "Beginning event loop." << endl;
   for( ev.toBegin(); !ev.atEnd(); ++ev) {
//      This appears to be not event-specific so I pulled it out
//      std::strbitset ret = wPlusJetsSelector.getBitTemplate();
      bool passed = wPlusJetsSelector (ev, ret);
      // Since we only care about selected events, why make this more
      // complicated than it has to be?
      vector<reco::ShallowClonePtrCandidate> const & electrons 
         = wPlusJetsSelector.selectedElectrons();
      vector<reco::ShallowClonePtrCandidate> const & muons     
         = wPlusJetsSelector.selectedMuons();
      vector<reco::ShallowClonePtrCandidate> const & jets      
         = wPlusJetsSelector.cleanedJets();

      // cout << "E " << electrons.size()
      //      << " M " << muons.size()
      //      << " J " << jets.size() << endl;

      if (!passed) 
      {
         ++numFailed;
         continue;
      }
      

      string lepton = "";
      if (1 == muons.size() and ! electrons.size())
      {
         lepton = kMuon;
      } else if (1 == electrons.size() and ! muons.size())
      {
         lepton = kElectron;
      } else {
         // shouldn't be here
         assert (0);
      }


      // get the sample name for this event
      string sampleName;
      if ( ! calcSampleName (ev, sampleName, shyftParameters) )
      {
         // We don't want this one.
         continue;
      }


      //////////////////////////////////////
      // Tagged Jets and Flavor Separator //
      //////////////////////////////////////
      int numBottom = 0, numCharm = 0, numLight = 0;
      int numTags = 0;
      double sumVertexMass = 0.;
      // Loop over the jets and find out which are tagged
      int jetSize = (int) jets.size();
      for (int jetIndex = 0; jetIndex < jetSize; ++jetIndex)
      {
         const pat::Jet *jetPtr = 
            dynamic_cast<const pat::Jet *>(jets[jetIndex].masterClonePtr().get());
         // Is this jet tagged and does it have a good secondary vertex
         if( jetPtr->bDiscriminator("simpleSecondaryVertexBJetTags") < 2.05 )
         {
            // This jet isn't tagged
            continue;
         }
         reco::SecondaryVertexTagInfo const * svTagInfos 
            = jetPtr->tagInfoSecondaryVertex("secondaryVertex");
         if ( svTagInfos->nVertices() <= 0 ) 
         {
            // Given that we are using simple secondary vertex
            // tagging, I don't think this should ever happen.
            // Maybe we should put a counter here just to check.
            continue;
         } // if we have no secondary verticies
         
         // count it
         ++numTags;

         // Is this MC?
         if (mode >= 0)
         {
            // What is the flavor of this jet
            int jetFlavor = std::abs( jetPtr->partonFlavour() );
            if (5 == jetFlavor)
            {
               ++numBottom;
            } // if bottom 
            else if (4 == jetFlavor)
            {
               ++numCharm;
            } // if charm
            else
            {
               ++numLight;
            } // if light flavor
         }

         ///////////////////////////
         // Calculate SecVtx Mass //
         ///////////////////////////
         ROOT::Math::LorentzVector< ROOT::Math::PxPyPzM4D<double> > sumVec;
         reco::CompositeCandidate vertexCand;
         reco::Vertex::trackRef_iterator 
            kEndTracks = svTagInfos->secondaryVertex(0).tracks_end();
         for (reco::Vertex::trackRef_iterator trackIter = 
                 svTagInfos->secondaryVertex(0).tracks_begin(); 
              trackIter != kEndTracks; 
              ++trackIter ) 
         {
            const double kPionMass = 0.13957018;
            ROOT::Math::LorentzVector< ROOT::Math::PxPyPzM4D<double> > vec;
            vec.SetPx( (*trackIter)->px() );
            vec.SetPy( (*trackIter)->py() );
            vec.SetPz( (*trackIter)->pz() );
            vec.SetM (kPionMass);
            sumVec += vec;
         } // for trackIter
         sumVertexMass += sumVec.M();
         if (2 == numTags)
         {
            // We've got enough.  Stop.
            break;
         } // if we have enough tags
      } // for jet

      ////////////////////////
      // General Accounting //
      ////////////////////////
      int numJets = std::min( (int) jets.size(), 5 );
      histograms[ sampleName + "_jettag" + lepton ]->Fill (numJets, numTags);

      // If we don't have any tags, don't bother going on
      if ( ! numTags)
      {
         continue;
      }

      ///////////////////////////////////////
      // Calculate average SecVtx mass and //
      // fill appropriate histograms.      //
      ///////////////////////////////////////
      sumVertexMass /= numTags;
      string whichtag = "";
      if (1 == numTags)
      {
         // single tag
         if      (numBottom)              whichtag = "_b";
         else if (numCharm)               whichtag = "_c";
         else if (numLight)               whichtag = "_q";
         else                             whichtag = "_X"; // shouldn't happen
      } else {
         // double tags
         if      (2 == numBottom)         whichtag = "_bb";
         else if (2 == numCharm)          whichtag = "_cc";
         else if (2 == numLight)          whichtag = "_qq";
         else if (numBottom && numCharm)  whichtag = "_bc";
         else if (numBottom && numLight)  whichtag = "_bq";
         else if (numCharm  && numLight)  whichtag = "_cq";
         else                             whichtag = "_XX"; // shouldn't happen
      } // if two tags
      string massName = sampleName 
         + Form("_secvtxMass_%dj_%dt", numJets, numTags);
      histograms[massName + lepton]->Fill (sumVertexMass);
      if (mode >= 0)
      {      
         histograms[massName + whichtag + lepton]->Fill (sumVertexMass);
      }
   } // for eventCont
      
   ////////////////////////
   // ////////////////// //
   // // Clean Up Job // //
   // ////////////////// //
   ////////////////////////

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   cout << "numFailed: " << numFailed << endl;

   // All done!  Bye bye.
   return 0;
}


//////////////  //////////////////////////////////  //////////////
//////////////  // //////////////////////////// //  //////////////
//////////////  // // Supporting Subroutines // //  //////////////
//////////////  // //////////////////////////// //  //////////////
//////////////  //////////////////////////////////  //////////////


void outputNameTagFunc (string &tag)
{
   // If you do not want to give you output filename any "tag" based
   // on the command line options, simply do nothing here.  This
   // function is designed to be called by fwlite::EventContainer constructor.

   // if ( boolValue ("someCondition") )
   // { 
   //    tag += "_someCond";
   // }
}


void bookHistograms (fwlite::ChainEvent &eventCont,
                     const string &lepton,
                     edm::ParameterSet const& shyftParameters,
                     fwlite::TFileService& fs,
                     std::map<TString, TH1*>& histograms)
{
   int mode = shyftParameters.getParameter<int>("mode");
   std::string sampleName = shyftParameters.getParameter<std::string>("sampleName");
   /////////////////////////////////////////////
   // First, come up with all possible base   //
   // names (E.g., Wbb, Wb2, etc.).           //
   /////////////////////////////////////////////
   CommandLineParser::SVec baseNameVec;
   CommandLineParser::SVec beginningVec, endingVec;
   switch ( mode )
   {
      case kVqqMode:
         // We want Wbb, Wb2, .., Zbb, ..  In this case, we completely
         // ignore the sampleName that was passed in.
         // Starts with
         beginningVec.push_back ("X");
         beginningVec.push_back ("W");
         beginningVec.push_back ("Z");
         // Ends with
         endingVec.push_back( "bb" );
         endingVec.push_back( "b2" );
         endingVec.push_back( "cc" );
         endingVec.push_back( "c2" );
         for (CommandLineParser::SVecConstIter outerIter = beginningVec.begin();
              beginningVec.end() != outerIter;
              ++outerIter)
         {
            for (CommandLineParser::SVecConstIter innerIter = endingVec.begin();
                 endingVec.end() != innerIter;
                 ++innerIter)
            {
               baseNameVec.push_back( *outerIter + *innerIter);
            } // for innerIter
         } // for outerIter
         break;
      case kLFMode:
         // just like the default case, except that we do have some
         // heavy flavor pieces here, too.
         baseNameVec.push_back(sampleName + "b3");
         baseNameVec.push_back(sampleName + "c3");
         // no break because to add just the name as well
      default:
         // We just want to use the sample name as it was given to us.
         baseNameVec.push_back(sampleName);
   } // for switch

   ////////////////////////////////////////
   // Now the different tagging endings. //
   ////////////////////////////////////////
   CommandLineParser::SVec singleTagEndingVec, doubleTagEndingVec;
   singleTagEndingVec.push_back ("_b");
   singleTagEndingVec.push_back ("_c");
   singleTagEndingVec.push_back ("_q");
   doubleTagEndingVec.push_back ("_bb");
   doubleTagEndingVec.push_back ("_cc");
   doubleTagEndingVec.push_back ("_qq");
   doubleTagEndingVec.push_back ("_bc");
   doubleTagEndingVec.push_back ("_bq");
   doubleTagEndingVec.push_back ("_cq");

   /////////////////////////////////////////
   // Finally, let's put it all together. //
   /////////////////////////////////////////
   for (CommandLineParser::SVecConstIter baseIter = baseNameVec.begin();
        baseNameVec.end() != baseIter;
        ++baseIter)
   {
      //////////////////////////////////////////////////////
      // For each flavor, one jet/tag counting histogram. //
      //////////////////////////////////////////////////////
      TString histName = *baseIter + "_jettag" + lepton.c_str();
      histograms[histName] = fs.make<TH2F>( histName, histName, 
                               6, -0.5, 5.5,
                               3, -0.5, 2.5);
      for (int jet = 1; jet <= 5; ++jet)
      {
         for (int tag = 1; tag <= 2; ++tag)
         {
            ////////////////////////////////////////////
            // For each jet/tag, a single secvtx mass //
            ////////////////////////////////////////////
            if (tag > jet) continue;
            histName = *baseIter + Form ("_secvtxMass_%dj_%dt%s", 
                                         jet, tag, lepton.c_str());
            histograms[histName] = fs.make<TH1F>( histName, histName, 40, 0, 10);
            // if this is data, then we don't know the real flavor, so
            // don't bother
            if (mode < 0)
            {
               continue;
            }
            CommandLineParser::SVec *vecPtr = &singleTagEndingVec;
            if (2 == tag)
            {
               vecPtr = &doubleTagEndingVec;
            } // double tag
            for (CommandLineParser::SVecConstIter tagIter = vecPtr->begin();
                 vecPtr->end() != tagIter;
                 ++tagIter)
            {
               ////////////////////////////////////////////////////
               // And different secvtx mass for each tag ending. //
               ////////////////////////////////////////////////////
               histName = *baseIter + Form ("_secvtxMass_%dj_%dt", jet, tag)
                  + *tagIter +  lepton.c_str();
               histograms[histName] = fs.make<TH1F>( histName, histName, 40, 0, 10);
            } // for tagIter
         } // for tag
      } // for jet
   } // for baseIter
}
					

bool calcSampleName (fwlite::ChainEvent &eventCont, string &sampleName, edm::ParameterSet const & shyftParameters)
{
   // calculate sample name
   sampleName = shyftParameters.getParameter<std::string>("sampleName");
   int mode   = shyftParameters.getParameter<int>("mode");

   /////////////////
   // Normal Mode //
   //// /////////////
   if (kNormalMode == mode || kDataMode == mode)
   {
      // all we want is the sample name, so in this case we're done.
      return true;
   }
   // Get the heavy flavor category
   fwlite::Handle< unsigned int > heavyFlavorCategory;
   heavyFlavorCategory.getByLabel (eventCont, "flavorHistoryFilter");
   assert ( heavyFlavorCategory.isValid() );
   int HFcat = (*heavyFlavorCategory);

   ///////////////////////
   // Light Flavor Mode //
   ///////////////////////
   if (kLFMode == mode)
   {
      // Wqq
      if (5 == HFcat)
      {
         sampleName += "b3";
      } else if (6 == HFcat)
      {
         sampleName += "c3";
      } else if (11 != HFcat)
      {
         // skip this event
         return false;
      } // else if ! 11
      return true;
   }

   /////////////
   // Wc Mode //
   /////////////
   if (kWcMode == mode)
   {
      // Wc
      if (4 != HFcat)
      {
         // skip this event
         return false;
      } // if not Wc
      return true;
   } // else if Wc

   //////////////
   // Vqq Mode //
   //////////////
   // MadGraph (at least as CMS has implemented it) has this _lovely_
   // feature that if the W or Z is far enough off-shell, it erases
   // the W or Z from the event record.  This means that in some
   // number of cases, we won't be able to tell whether this is a W or
   // Z event by looking for a W or Z in the GenParticle collection.
   // (We'll eventually have to be more clever).
   sampleName = "X";
   fwlite::Handle< vector< reco::GenParticle > > genParticleCollection;
   genParticleCollection.getByLabel (eventCont, "prunedGenParticles");
   assert ( genParticleCollection.isValid() );
   // We don't know if it is a W, a Z, or neither
   // Iterate over genParticles
   const vector< reco::GenParticle>::const_iterator 
      kGenPartEnd = genParticleCollection->end();
   for (vector< reco::GenParticle>::const_iterator gpIter =
           genParticleCollection->begin(); 
        gpIter != kGenPartEnd; ++gpIter ) 
   {
      if (gpIter->status() == 3 && std::abs(gpIter->pdgId()) == 23)
      {
         sampleName = "Z";
         break;
      }
      if (gpIter->status() == 3 && std::abs(gpIter->pdgId()) == 24)
      {
         sampleName = "W";
         break;
      }
   } // for  gpIter
   switch (HFcat)
   {
      // from:
      // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideFlavorHistory
      //  1. W+bb with >= 2 jets from the ME (dr > 0.5)
      //  2. W+b or W+bb with 1 jet from the ME
      //  3. W+cc from the ME (dr > 0.5)
      //  4. W+c or W+cc with 1 jet from the ME
      //  5. W+bb with 1 jet from the parton shower (dr == 0.0)
      //  6. W+cc with 1 jet from the parton shower (dr == 0.0)
      //  7. W+bb with >= 2 partons but 1 jet from the ME (dr == 0.0)
      //  8. W+cc with >= 2 partons but 1 jet from the ME (dr == 0.0)
      //  9. W+bb with >= 2 partons but 2 jets from the PS (dr > 0.5)
      // 10. W+cc with >= 2 partons but 2 jets from the PS (dr > 0.5)
      // 11. Veto of all the previous (W+ light jets)
      case 1:
         sampleName += "bb";
         break;
      case 2:
         // Sometimes this is referred to as 'b' (e.g., 'Wb'), but I
         // am using the suffix '2' to keep this case clear for when
         // we have charm (see below).
         sampleName += "b2";
         break; 
      case 3:
         sampleName += "cc";
         break;
      case 4:
         // We want to keep this case clear from real W + single charm
         // produced (as opposed to two charm quarks produced and one
         // goes down the beampipe), so we use 'c2' instead of 'c'.
         sampleName += "c2";
         break;
      default:
         // we don't want the rest of the cases.  Return an empty
         // string so we know.
         return false;
   } // switch HFcat
   return true;
}

int main (int argc, char* argv[]) 
{

   try
   {
      return mainRoutine (argc, argv);
   }
   catch (cms::Exception& except)
   {
      cout << except.what() << endl;
   }
}

