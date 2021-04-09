#include <iostream>

#include "TBranch.h"
#include "TLeaf.h"
#include "TLeafC.h"

#include "RootFileCollection/TopNtupleDecoder.h"

using namespace std;

int TopNtupleDecoder::sm_verbose = -1;

TopNtupleDecoder::TopNtupleDecoder(TTree *t):
   m_treePtr(t),
   m_electronCollTypes(),  m_muonViewTypes(),   m_jetCollTypes(), 
   m_offltrackCollTypes(), m_trigSummaryBits(), m_isMC(false),
   m_offlVers(-1)
{

   //Read the data from the first entry
   readData(0);

}

void TopNtupleDecoder::readData(int entry) 
{

   //Unpack the name to number correspondences in TopNtuple.  Assume
   //that the information stored in the first event is valid for all
   //events.

   //Variable to store the block name
   char blockName[100];

   //Flag for when to stop
   bool keepLooping = true;

   //Counter for the collection and view types
   int num = 0;

   //Start with the electrons
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl
           << "Checking Electron CollTypes:" << endl;

   m_electronCollTypes.clear();

   keepLooping = true;
   num = 1;
   while (keepLooping) 
   {

      //Assume this will be the last time through
      keepLooping = false;

      sprintf (blockName,"eleViewDescr%d",num);
      TBranch *branchPtr = m_treePtr->GetBranch(blockName);
      if (branchPtr) 
      {
         branchPtr->GetEntry(entry);
         TLeafC *leafPtr = (TLeafC*) branchPtr->GetLeaf(blockName);
         TString name (leafPtr->GetValueString());
         name.ToUpper();

         if (name != "NULL") 
         {
            m_electronCollTypes[name] = num;
	
            if (sm_verbose) 
               cout << "-->Found " << name << " as CollType = " << num << endl; 
	  
            //Well, we found another description, so keep looping
            keepLooping = true;
	  
            num++;
         } // if name != NULL
      } // if branchPtr
   } // while keepLooping
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;

   //Next Muons
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl
           << "Checking Muon ViewTypes:" << endl;

   m_muonViewTypes.clear();

   keepLooping = true;
   num = 1;
   while (keepLooping) 
   {

      //Assume this will be the last time through
      keepLooping = false;

      sprintf (blockName,"muoViewDescr%d",num);

      TBranch *branchPtr = m_treePtr->GetBranch(blockName);
      if (branchPtr) 
      {

         branchPtr->GetEntry(entry);
         TLeafC *leafPtr = (TLeafC*) branchPtr->GetLeaf(blockName);
         TString name (leafPtr->GetValueString());
         name.ToUpper();

         if (name != "NULL") 
         {
            m_muonViewTypes[name] = num;
	
            if (sm_verbose) 
               cout << "-->Found " << name << " as CollType = " << num << endl; 
	
            //Well, we found another description, so keep looping
            keepLooping = true;
	
            num++;
         } // if name != NULL
      } // if branchPtr
   } // while keepLooping
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;

   //Next Jets
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl
           << "Checking Jet CollTypes:" << endl;

   m_jetCollTypes.clear();

   keepLooping = true;
   num = 1;
   while (keepLooping) 
   {

      //Assume this will be the last time through
      keepLooping = false;

      sprintf (blockName,"jetCollDescr%d",num);

      TBranch *branchPtr = m_treePtr->GetBranch(blockName);
      if (branchPtr) 
      {

         branchPtr->GetEntry(entry);
         TLeafC *leafPtr = (TLeafC*) branchPtr->GetLeaf(blockName);
         TString name (leafPtr->GetValueString());
         name.ToUpper();
      
         if (name != "NULL") 
         {
            m_jetCollTypes[name] = num;
	
            if (sm_verbose) 
               cout << "-->Found " << name << " as CollType = " << num << endl; 
	
            //Well, we found another description, so keep looping
            keepLooping = true;
	
            num++;
         } // if name != NULL
      } // if branchPtr
   } // while keepLooping
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;

   //Next Offline Tracks
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl
	    << "Checking Offline Tracks:" << endl;

   m_offltrackCollTypes.clear();

   keepLooping = true;
   num = 1;
   while (keepLooping) {

      //Assume this will be the last time through
      keepLooping = false;

      sprintf (blockName,"otrkCollDescr%d",num);

      TBranch *branchPtr = m_treePtr->GetBranch(blockName);
      if (branchPtr) 
      {

         branchPtr->GetEntry(entry);
         TLeafC *leafPtr = (TLeafC*) branchPtr->GetLeaf(blockName);
         TString name (leafPtr->GetValueString());
         name.ToUpper();

         if (name != "NULL") {
            m_offltrackCollTypes[name] = num;
	
            if (sm_verbose) 
               cout << "-->Found " << name << " as CollType = " << num << endl; 
	
            //Well, we found another description, so keep looping
            keepLooping = true;
	
            num++;
         } // if name != NULL
      } // if branchPtr
   } // while keepLooping
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;
  
   //Finally Triggers
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl
           << "Checking Trigger Summary Word Bits:" << endl;

   m_trigSummaryBits.clear();

   keepLooping = true;
   num = 0;
   while (keepLooping) 
   {

      //Assume this will be the last time through
      keepLooping = false;

      sprintf (blockName,"trigBitDescr%d",num);

      TBranch *branchPtr = m_treePtr->GetBranch(blockName);
      if (branchPtr) 
      {

         branchPtr->GetEntry(entry);
         TLeafC *leafPtr = (TLeafC*) branchPtr->GetLeaf(blockName);
         TString name (leafPtr->GetValueString());
         name.ToUpper();

         if (name != "NULL") {
            m_trigSummaryBits[name] = num;
	
            if (sm_verbose) 
               cout << "-->Found " << name << " for bit = " << num << endl; 
	
            //Well, we found another description, so keep looping
            keepLooping = true;
	
            num++;
         } // if name != NULL
      } // if branchPtr
   } // while keepLooping
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;

   //Check whether this is MC or data and get the offline production version
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl;
   TBranch *evtB = m_treePtr->GetBranch("evt.isMonteCarlo");
   if (evtB) 
   {
      evtB->GetEntry(entry);
      TLeaf *leafPtr = evtB->GetLeaf("evt.isMonteCarlo");
      m_isMC = (bool)leafPtr->GetValue();
      if (m_isMC) 
      {
         if (sm_verbose) 
                     cout << "Ntuple contains Monte Carlo events" << endl;
      }
      else 
      {
         if (sm_verbose) 
            cout << "Ntuple contains data events" << endl;
      }
   } else {
      cout << "Warning: Can't find evt branch.  Can't tell whether this is Monte Carlo or data." << endl;
   }
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;

   if (sm_verbose) 
      cout << "----------------------------------------------" << endl;
   evtB = m_treePtr->GetBranch("evt.productionVersion");
   if (evtB) 
   {
      evtB->GetEntry(entry);
      TLeaf *leafPtr = evtB->GetLeaf("evt.productionVersion");
      m_offlVers = (int)leafPtr->GetValue();
      if (sm_verbose) 
         cout << "Ntuple contains data processed with production version " 
              << m_offlVers << endl;
   } else {
      cout << "Warning: Can't find evt branch.  Can't determine version of production used on this data" << endl;
   }
   if (sm_verbose) 
      cout << "----------------------------------------------" << endl << endl;
  
}

int TopNtupleDecoder::getEleCollType(TString name) const 
{

   name.ToUpper();

   //Check whether we have this collection in this TopNtuple
   map<TString,int>::const_iterator i = m_electronCollTypes.find(name);
   if (i != m_electronCollTypes.end()) 
   {

      return i->second;

   }

   //Didn't find a match
   return -1;

}

int TopNtupleDecoder::getMuoViewType(TString name) const 
{

   name.ToUpper();

   //Check whether we have this collection in this TopNtuple
   map<TString,int>::const_iterator i = m_muonViewTypes.find(name);
   if (i != m_muonViewTypes.end()) 
   {

      return i->second;

   }

   //Didn't find a match
   return -1;

}

int TopNtupleDecoder::getJetCollType(TString name) const 
{

   name.ToUpper();

   //Check whether we have this collection in this TopNtuple
   map<TString,int>::const_iterator i = m_jetCollTypes.find(name);
   if (i != m_jetCollTypes.end()) 
   {

      return i->second;

   }

   //Didn't find a match
   return -1;

}

int TopNtupleDecoder::getOfflTrkCollType(TString name) const 
{

   name.ToUpper();

   //Check whether we have this collection in this TopNtuple
   map<TString,int>::const_iterator i = m_offltrackCollTypes.find(name);
   if (i != m_offltrackCollTypes.end()) 
   {

      return i->second;

   }

   //Didn't find a match
   return -1;

}

int TopNtupleDecoder::getTrigSummaryBit(TString name) const 
{

   name.ToUpper();

   //Check whether we have this collection in this TopNtuple
   map< TString,int >::const_iterator i = m_trigSummaryBits.find(name);
   if (i != m_trigSummaryBits.end()) 
   {

      return i->second;

   }

   //Didn't find a match
   return -1;

}

bool TopNtupleDecoder::isMonteCarlo() const 
{
  
   return m_isMC;

}

int TopNtupleDecoder::getProductionOfflineVersion() const 
{

   return m_offlVers;

}
