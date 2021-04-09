// -*- C++ -*-

#if !defined(TreeFileInterface_H)
#define TreeFileInterface_H

#include <iostream>
#include <typeinfo> // for RTTI
#include <string>
#include <set>
#include <map>
#include <vector>
#include <cstdlib>

#include "TTree.h"
#include "TString.h"
#include "TDirectory.h"

#include "RootFileCollection/RootFileCollection.h"
#include "RootFileCollection/TopNtupleDecoder.h"

class TreeFileInterface : public RootFileCollection
{
   public:

      //////////////////////
      // Public Constants //
      //////////////////////
      // name to TCA ptr
      typedef std::map< std::string, TClonesArray* > TClonesPtrMap;
      typedef TClonesPtrMap::iterator                TClonesPtrMapIter;
      // name to void ptr
      typedef std::map< std::string, void* >         NameVoidPtrMap;
      typedef NameVoidPtrMap::iterator               NameVoidPtrMapIter;
      // name to bool
      typedef std::map< std::string, bool >          NameBoolMap;
      typedef NameBoolMap::iterator                  NameBoolMapIter;
      // name to string (for RTTI use)
      typedef std::map< std::string, std::string >   NameStringMap;
      typedef NameStringMap::iterator                NameStringMapIter;
      // string vector
      typedef std::vector< std::string >             SVec;
      typedef SVec::iterator                         SVecIter;
      // TObject ptr vector
      typedef std::vector< TObject* >                TObjPtrVec;
      typedef TObjPtrVec::iterator                   TObjPtrVecIter;
      // std::string set
      typedef std::set< std::string >                SSet;
      typedef SSet::iterator                         SSetIter;

      /////////////
      // friends //
      /////////////
      // tells particle data how to print itself out
      friend std::ostream& operator<< (std::ostream& o_stream, 
                                       const TreeFileInterface &rhs);

      //////////////////////////
      //            _         //
      // |\/|      |_         //
      // |  |EMBER | UNCTIONS //
      //                      //
      //////////////////////////

      /////////////////////////////////
      // Constructors and Destructor //
      /////////////////////////////////
      // Standard constructor
      TreeFileInterface (TString treename);
      // Default constructor
      // Note: You MUST use 'setTreeName' before using instance
      TreeFileInterface ();
      // use if we want to "decode" collection names (Top Ntuple specific)
      TreeFileInterface(TString treename, bool decode);
      // destuctor
      virtual ~TreeFileInterface();

       ////////////////
      // One Liners //
      ////////////////

      // Turn top ntuple decoder on/off.  This MUST be called before
      // you get the first event.
      void setTopNtupleDecoder (bool status = "true")
      { m_decodeTopTree = status; }


      // take only 1 out of every N events from the input tree
      void set1inEveryN (int num) { m_1inEveryN = num; }

      // set the tree name that the TFI will look for
      void setTreeName (const TString &treename)
      { m_treename = treename; }

      // tell object to always get entire tree
      void setGetEntireTree (bool entire = true) { m_getEntireTree = entire; }

      // tell object how many events we want (default is to run on all
      // events available)
      void setEventsWanted (int wanted) { m_eventsWanted = wanted; }

      // returns true on events where a new file was opened.
      bool isNewFile() { return m_newFile; }

      // number of events seen
      int numberOfEventsSeen() { return m_eventsSeen; }

      // current file name
      TString filename() { return m_currentFilePtr->GetName(); }

      // return the tree pointer (avoid using  if possible)
      TTree *treePtr() { return m_treePtr; }

      // return the current file (avoid using if possible)
      TFile *filePtr() { return m_currentFilePtr; }

      // reset the list of active branche
      void resetActiveBranchList() { m_activeBranchesVec.clear(); }
      
      // reset the list of inactive branches
      void resetInactiveBranchList() { m_inactiveBranchesVec.clear(); }
      

      //////////////////////////////
      // Regular Member Functions //
      //////////////////////////////
      // this routine is called 'while (instance.getNextEvent())'.
      bool getNextEvent();

      // This routine skips N events (N > 0).  It returns true if
      // there are still events to be read.
      // WARNING: If you can this function, it returns false, AND you
      // try to access the event, you WILL segfault.
      bool skipNevents (int numEvents);

      // Add a TCA to be hooked up.  In sane cases, the branch name
      // and class name are the same.  If they are different, then
      // pass in the class name as well.
      void addTcaBranch (const std::string &branchName, 
                         std::string className = "");

      // NOTE: Root hooks up simple objects (e.g. ints and floats)
      // differently than classes.  If you want access to a simple
      // object branch, use 'addBranch()' and 'value()' or 'ptr()'.
      // If you want access to a class, use 'addClassBranch()' and
      // 'classPtr()'.

      // add a template branch to be hooked up.  You only need to put
      // in a postfix (e.g. "/I" for an int) IF you are going to copy
      // this branch to an output tree.
      template <class T>
      void addBranch (const std::string &branchName,
                      const std::string &postfix = "");

      // add a template branch to be hooked up.
      template <class T>
      void addClassBranch (const std::string &branchName);

      // Remove a TCA from being hooked up
      void removeTcaBranch (const std::string &branchName);

      // Remove a int from being hooked up
      template <class T>
      void removeBranch (const std::string &branchName);

      // Remove a int from being hooked up
      template <class T>
      void removeClassBranch (const std::string &branchName);
      
      // gets the branch status for a given branch on the input tree.
      // Note that the branch in question does not need to have
      // already been activated.
      bool getBranchStatus (const std::string &branchName) const;

      // returns a TCA pointer for a given name
      TClonesArray* tcaPtr (const std::string &branchName);

      // Equivalent to tcaPtr(branchName)->At(which), except that it
      // does bounds checking (i.e. will crash with an error message
      // if 'which' is too large).
      TObject* At (const std::string &branchName, int which);

      // Equivalent to tcaPtr(branchName)->GetEntries()
      Int_t GetEntries (const std::string &branchName);
      
      // returns template ptr for a given name
      template <class T>
      T *ptr (const std::string &branchName);

      // returns template ptr for a given name
      template <class T>
      T *classPtr (const std::string &branchName);

      // returns template value for a given name
      template <class T>
      T value (const std::string &branchName);      

      // returns true is branch ok (and active)
      bool isBranchOK (const std::string &branchName);

      // override nextFile to tell people not to use it
      TFile *nextFile();
      
      // Tell the TreeFileInterface to make a clone.  This hooks up
      // and fills all branches.  See getCloneTree() for details.
      // Calling with maxSize > 0 sets the maximum size of the cloned
      // tree before splitting.  If you call the '()' empty version,
      // you MUST call setOutputFile first().
      void makeCloneTree (TDirectory *outFile, Long64_t maxSize = -1);
      void makeCloneTree ();

      // set up the output file and maxsize for any clone or other
      // output trees.  You can only set them ONCE (either with this
      // function or 'makeCloneTree(...)'.
      void setOutputFile (TDirectory *outFile, Long64_t maxSize = -1);

      // Gets you a pointer to the "clone Tree" for this collection of
      // files where the branch address get correctly updated when you
      // switch files.  Note: for this to work, you need to have
      // called makeCloneTree() first.  To fill the clone, call
      // FillCloneTree(), which will ensure that for events you want
      // to keep, the full event information is transferred to the
      // clone.  Before the clone has been initialized, this returns a
      // null pointer.
      TTree* getCloneTree();

      // Fills the clone.  Call for events you've decided to keep.
      bool fillCloneTree();

      // Writes the clone tree (and any other TObjects given to the
      // class) to the file at the end of the job.  It closes the file
      // holding the clone tree.
      bool writeFinalCloneTree();

      // add a TObject pointer to be written to the same file as the
      // clone tree.  Must be called after 'makeCloneTree()'.
      void addTObjectPtrToWrite (TObject *tobjPtr);

      //Get the TopNtupleDecoder
      TopNtupleDecoder *getTopDecoder();

      // Add a branch to the list of active branches (all branches NOT
      // added will NOT be read in).  Set 'noStar' to true if you want
      // to ensure that ONLY "branchName" is taken and not
      // "branchName*" (don't use this if you are not REALLY sure you
      // want to).
      // NOTE: You can use either 'addActiveBranch' OR 'addInactiveBranch',
      // but NOT BOTH.
      void addActiveBranch (std::string branchName, 
                            bool noStar = false);

      // Add a branch to the list of inactive branches (all branches NOT
      // added WILL be read in).  Set 'noStar' same as 'addActiveBranch'.
      // NOTE: You can use either 'addActiveBranch' OR 'addInactiveBranch',
      // but NOT BOTH.
      void addInactiveBranch (std::string branchName, 
                              bool noStar = false);

      // Add branches to the clone tree.  There are three different
      // member functions depending on the object being added.
      // 'makeCloneTree()' must be called before any of these member
      // functions.

      // Add simple object branches
      void addCloneTreeBranch (const std::string &branchName, 
                               void *simplePtr,
                               const std::string &branchDescription);

      // Add class instance branches
      void addClassCloneTreeBranch (const std::string &branchName, 
                                    void *classAddPtr,
                                    const std::string &branchDescription);

      // Add TCA branches
      void addTCACloneTreeBranch (const std::string &branchName, 
                                  void *tcaAddPtr);

      // // adds an already existing branch (added below) to the clone tree
      // void addExistingCloneTreeBranch (const std::string &branchName,
      //                                  std::string addAs = "");

      ////////////////////////////////////////////
      // ////////////////////////////////////// //
      // // Adding Output Trees and Branches // //
      // ////////////////////////////////////// //
      ////////////////////////////////////////////

      // add a tree to be written to the output file.  If you pass in
      // tree name and description, the tree will be automatically
      // 'new'ed.  If you pass in tree name and a tree pointer, the
      // pointer value will be stored. The output directory must
      // already be setup (either by makeCloneTree(...)  or
      // setOutputFile().
      TTree* addTTree (const std::string &treename, 
                       const std::string &description = "");
      TTree* addTTree (const std::string &treename, TTree *treePtr);

      // get the address of a TTree
      TTree* getTTreeAddress (const std::string &treename);
      

      // add an object to be hooked up to a tree.  'postfix' argument
      // is the '/I' that are added to the end of the branch name to
      // add simple classes in root.
      template <class T>
      T* addBranchObject (const std::string &branchName,
                            const std::string &postfix,
                            T *ptr = 0);
      
      // add a class object to be hooked up to a tree
      template <class T>
      T* addClassBranchObject (const std::string &branchName,
                                 T *ptr = 0);

      // add a TClonesArray to be hooked up to a tree.  You either
      // need to pass in a pointer, or the classname and default size.
      TClonesArray* addTcaBranchObject (const std::string &branchName,
                                        TClonesArray *ptr);
      TClonesArray* addTcaBranchObject (const std::string &branchName,
                                        const std::string &className,
                                        int size = 1);

      // returns the address of the TClonesArray, Class, or simple
      // object.  This does NOT call getEntry on the input tree.  This
      // is for getting back the address of a branch used for an
      // output tree.
      // TCA
      TClonesArray *tcaBranchObjectAddress (const std::string &branchName);
      // simple
      template <class T>
      T* objectBranchObjectAddress (const std::string &branchName);
      // Classes
      template <class T>
      T* classBranchObjectAddress (const std::string &branchName);


      // Adds an already defined branch to an already defined tree.
      // Will attach branch as 'saveAsName' unless left blank (will
      // default to branchName).  Returns true if successfully found
      // both tree and branch. In the case where a user has mistakenly
      // named two different types of branches with the same name, the
      // search order will be TCA branches, class branches, and then
      // basic branches.
      bool addBranchToTree (const std::string &treeName,
                            const std::string &branchName,
                            std::string saveAsName = "");

  protected:
      ////////////////////////////////
      // Protected Member Functions //
      ////////////////////////////////
      // internal workings of 'getNextEvent()'.  Certain counters and
      // statuses should not be updated if this is called internally
      // (i.e. NOT from getNextEvent()).
      bool _internalGetNextEvent (bool calledInternally = false);

      // aborts program
      void _abort (const std::string &filename, int linenum);

      // get the tree from the new file.  This routine does NOT get a
      // new file, but uses what is in m_currentFilePtr already.
      bool _getTreeFromFile();

      // Hook up all of the branches on a new file
      void _hookUpBranches();

      // initialize all values
      void _initialize();

      // Check for consistency between 'addBranch', m_activeBranches, and
      // m_inactiveBranches
      void _checkConsistency();

      // hook up and refresh any extra CloneTree branches
      void _hookupExtraCTBranches();
      void _refreshExtraCTBranches();

      // hook up a new file.  Returns true if successful; false if
      // there are no more files to hook up
      bool _hookupNextFile();

      // returns true if a branch is input branch
      bool _isInputBranchSet (const std::string& branchname) const;
      // adds branchname to set
      void _addToInputBranchSet (const std::string& branchname);
      // removes branchname from set
      void _removeFromInputBranchSet (const std::string& branchname);

      ///////////////////////////
      // Protected Member Data //
      ///////////////////////////

      // Trees and files
      TString m_treename;
      TFile  *m_currentFilePtr;
      TTree  *m_treePtr;

      // Cloning
      bool        m_makeClone;
      Long64_t    m_maxTreeSize;
      TTree      *m_cloneTreePtr;
      TDirectory *m_outputDirectory;
      bool        m_wroteCloneTree;

      // counts and flags
      int  m_eventsSeen;
      int  m_previousEventsSeen;
      int  m_totalEventsPreviousFiles;
      int  m_eventsWanted;
      int  m_currentEntry;
      int  m_entriesThisFile;
      int  m_entriesSeen;
      int  m_entriesSeenThisFile;
      int  m_loadTreeFlag;
      int  m_1inEveryN;
      bool m_allDone;
      bool m_newFile;
      bool m_getEntireTree;
      bool m_decodeTopTree;
      bool m_firstFile;

      // Maps and vectors
      TClonesPtrMap  m_tcaPtrMap;
      NameVoidPtrMap m_voidPtrMap;
      NameVoidPtrMap m_classPtrMap;
      NameVoidPtrMap m_treePtrMap;
      NameBoolMap    m_retrievedBranchMap;
      NameStringMap  m_rttiMap;
      SVec           m_activeBranchesVec;
      SVec           m_inactiveBranchesVec;
      SSet           m_inputBranchesSet;

      // branches to add to the cloned tree
      NameVoidPtrMap m_CTbranchAddressMap; // for simple objects
      NameVoidPtrMap m_CTbranchPtrAddressMap; // for class instances and TCAs
      NameStringMap  m_CTbranchDescriptionMap;

      // Other
      NameStringMap     m_simplePostfixMap;
      TopNtupleDecoder *m_decoderPtr; // Top Ntuple Specific

      // vector of TObjects to write along with the clone tree
      TObjPtrVec m_objPtrVec;
};

////////////////////////
// ostream Definition //
////////////////////////
extern ostream& operator<< (ostream& o_stream, const TreeFileInterface &right);

// Template function definitions need to be inline.  So include them
// here.  This is slightly ugly, but works.
#include "RootFileCollection/TreeFileInterface.icc"

#endif // TreeFileInterface_H
