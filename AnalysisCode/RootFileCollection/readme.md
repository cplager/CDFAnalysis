 The RootFileCollection Library

The RootFileCollection Library
==============================

*   [Introduction](#intro)
    *   [Browse all files](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/?v=RootFileCollection)
*   [Getting and Building the RootFileCollection Library](#build)
*   [The TreeFileInterface Class](#TFI)
    *   [TFINamespace and Choosing Input Source](#tfinamespace)
    *   [Selecting Branches](#branches)
    *   [The Event Loop](#eventloop)
*   [Skimming and Thinning Ntuples](#skim)
*   [Output Trees](#output)
*   [Disk Analysis Pool and its Doors](#dcap)
*   [The SAM Environment](#samenv)
    *   [Making a SAM Recovery Project](#samrecov)
*   [The RootFileCollection Class](#RFC)
*   [Command Line Options](#clo)
*   [Using the RootFileCollection Library in Root/Cint](#inroot)
*   [Examples](#examples)

[](#examples)

[](#examples)

Introduction
------------

This page describes the RootFileCollection class and its derived class the TreeFileInterface. These classes were written to give an alternative that works with SAM since TChain will not work. You can see the TreeFileInterface class use with [Top Ntuples here](tools.html). You will also find other utilities such as the [header to catch duplicate events](../UsefulScripts/useful.html#duplicate) as "RootFileCollection/RunEventSet.h".

Getting and Building the RootFileCollection Library
---------------------------------------------------

There is a CVS package called RootFileCollection. To get this package and build it, simply do the following (I am assuming that your code lives in a directory called releaseDir/)

unix> setup cdfsoft2 6.1.4
unix> cd releaseDir/
unix> cvs co RootFileCollection
unix> RootFileCollection/setup\_RootFileCollection.pl
unix> cd RootFileCollection/
unix> gmake 

If you want to compile this class without using the CDFSOFT environment, simply set the environment variable NO\_CDF\_SAM to 1. _I.e.,_ type unix> setenv NO\_CDF\_SAM 1 instead of  
unix> setup cdfsoft2 6.1.4  

See [Top Analysis Tools Page](tools.html) for examples of how to incorporate this class into stand-alone executables (and although I recommend against it, see the [Using the RootFileCollection Library in Root/Cint](#inroot) if you are interested).

The TreeFileInterface Class
---------------------------

This class is a derived class from [RootFileCollection](#RFC) class below. It is intended to replace TChains as SAM is not TChain\-friendly. This class takes care of looping over files and hooking up branches of an input tree for you. If you want to hook the trees up yourself, I recommend using [RootFileCollection](#RFC) below.

The TreTCAInterface class is made up of four files

*   [TreeFileInterface.h](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/RootFileCollection/TreeFileInterface.h?v=RootFileCollection) - This is a great source of documentation itself
*   [TreeFileInterface.cc](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/src/TreeFileInterface.cc?v=RootFileCollection) - non template source
*   [TreeFileInterface.icc](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/RootFileCollection/TreeFileInterface.icc?v=RootFileCollection) - template source
*   [TopTreeNamespace.h](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/RootFileCollection/TopTreeNamespace.h?v=RootFileCollection) - Top Ntuple branch names

Setting up the TreeFileInterface is done as below. Note that when using SAM, you will need to setup extra environment variables to tell it what files to use (See [The SAM Environment](#samenv) below). See [topAnaExample.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topAnaExample/topAnaExample.cc) to see a complete example.

**Note:** The example below uses TopNtuple, but the TreeFileInterface is meant to be a generic class that can be used with any tree.

TFINamespace and Choosing Input Source
--------------------------------------

TFINamespace is a way to easily setup all of what you need to read files, either a single file at a time, a list of files, or from a SAM project. Using this will automatically hook up several command line options for you and sets up an instance of the TreeFileInterface for you (ns\_tfi). Note that options are case-insensitive. If you do not want to use TFINamespace, you can use the [old instructions below.](#inputold)

Option

Meaning

usesam

If true (\=1), tells the TreeFileInterface to use SAM as an input source.

file

Name of single file for input

list

Name of list of files for input

num

Number of events to run over. 0 signifies all available events.

section, totalsection

Used with list to split up job into different sections.  
section=3 totalsection=8 is the 3rd section out of a total of 8.

availportsname

Loads [a text file](availPorts.txt) to change which doors are used for disk anaysis pool.

unsetDcacheIOtunnel

Needs to be set when using non-kerberized disk pool doors.

outputAvailPorts

Prints out doors that will be used.

To start, put these two lines in all files from where you want to look at data:

```
#include "RootFileCollection/TFINamespace.h"
using namespace TFINamespace;
```
In your main program, you want to have the following to initialize everything:

```
// forward declarations
void localInitialize();  // you write this subroutine and put it at
                         // the end of this file

int main(int argc, char\*\* argv)
{
   // Parse Command Line Arguments          
   setupDefaultOptions(); // hook up standard options
   localInitialize();
   parseArguments (argc, argv); // OptionUtils::

   string tag = ""; // any additional tags you want added on an output
                    // filename

   // setup the TFI to use the top tree and TopNtupleDecoder
   setupTFI ("TopTree", tag, true);

   // If you are not using the Top Tree, you would not use TopNtupleDecoder
   // so you'd do something like
   // setupTFI ("MyTree", tag);
```
### Selecting Branches

Next, we tell the TreeFileInterface to _register_ branches we want.
```
   ////////////////////////
   // Setup TCA Branches //
   ////////////////////////
   // Tell 'ns\_tfi' which branches we want.
   // 
   // The constants are defined in 
   // RootFileCollection/TopTreeNamespace.h
   // If you do not have a 'using namespace TopTreeNamespace;'
   // then you need to use 'TopTreeNamespace::kSummaryBranch'

   // TClonesArrays
   // If the branch and class name are the same, you simply do
   // ns\_tfi.addTcaBranch ("branchname");
   // If they are different, then it's
   // ns\_tfi.addTcaBranch ("branchname", "classname");
   ns\_tfi.addTcaBranch (kSummaryBranch);
   ns\_tfi.addTcaBranch (kEvtBranch    );
   ns\_tfi.addTcaBranch (kHepgBranch   );

   // Integers
   ns\_tfi.addBranch <int> (kNumJetObjs   );

   // Char \[40\]s
   ns\_tfi.addBranch <char\[40\]> (kMuoViewDescr1Branch);

   // Use 'addClassBranch < TClassName >'  to add a branch
   // that is a single instance of a class.
   // ns\_tfi.addClassBranch < MyClass > ("branchname");
```
### The Event Loop

Finally, we do the event loop.
```
   while (ns\_tfi.getNextEvent())
   {
      evt \*evtPtr = (evt\*) ns\_tfi.tcaPtr(kEvtBranch)->At(0);
      cout << "run " << evtPtr->runNumber 
           << " event " << evtPtr->eventNumber
           << endl;

      // Print out the number of jets two different ways to make sure
      // we agree (and a char\[40\] for the fun of it, too)
      cout << "  " 
           << ns\_tfi.tcaPtr (kJetBranch)->GetEntries() // one way
           << "  " 
           << ns\_tfi.GetEntries (kJetBranch) // equivalent way way
           << "  "
           << ns\_tfi.value <int> (kNumJetObjsBranch) // a third way
           << "  "
           << (char\*) ns\_tfi.ptr <char\[40\]> (kMuoViewDescr1Branch)
           << endl;
      cout << "  " << ns\_tfi.tcaPtr (kJetBranch)->GetEntries()
           << "  " << ns\_tfi.value <int> (kNumJetObjsBranch)
           << "  "
           << (char\*) ns\_tfi.ptr <char\[40\]> (kMuoViewDescr1Branch)
           << endl;

      // use 'classPtr < ClassName >' to get a pointer to a class
      // branch
      // MyClass \*classPtr = ns\_tfi.classPtr < ClassName > ("branchname");

} // while the next event
```
If you have code that runs when branches are not always present in a tree, you can check to see if a branch is OK inside the event loop.
```
      if (ns\_tfi.isBranchOK (kObspBranch))
      {
         // do something with Obsp branch
      }
```
**Note:** The _registered_ branches are not "filled" for each event **until** they are asked for by

*   tcaPtr(),
*   At(),
*   GetEntries(),
*   ptr(),
*   classPtr(), or
*   value()

This is done transparently as far as the user is concerned and done for speed purposes. This feature can be turned off (ns\_tfi.setGetEntireTree(true);) if you are both skimming and updating a tree. This means that you can ask for a branch (using add\*Branch() without worrying whether that branch exists in the files or not. See [topThin.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/topThin.cc) for an example of this.

Skimming and Thinning Trees
---------------------------

The TreeFileInterface makes it **very** easy to create a skim of a tree. You hookup the input files to an instance of TreeFileInterface as [above](#TFI). To prepare for skimming, first you open a file and then tell the TreeFileInterface object to make a clone tree. See [topThin.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/topThin.cc) for a complete example.
```
   TFile \*filePtr = new TFile (ns\_utputName.c\_str(), "RECREATE");
   if (! filePtr->IsOpen()) {
      cerr << "Did not successfully open " << ns\_outputName 
           << " for output.  Aborting." << endl;
      return 0;      
   }

   ns\_tfi.makeCloneTree (filePtr, ns\_maxTreeSize);
```

If you are going to be modifying the tree you are skimming, then you need to make sure that you read in the entire tree once per event.
```
   // By default, TreeFileInterface only does a 'GetEntry()' on the
   // branches as they are asked for.  When 'fillCloneTree()' is
   // called, it will do a 'GetEntry()' on the entire tree.  If you
   // are modifying the values of the branches, these changes will be
   // lost unless you tell the the TreeFileInterface to get the whole
   // event the first time.  Note that this is slower, but safer.
   ns\_tfi.setGetEntireTree (true);
```
If you want to drop branches, you can either specify all the branches you want to keep (addActiveBranch()) **or** all the branches you want to drop (addInactiveBranch()), but not both.

```
   // select only certain branches to be kept
   ns\_tfi.addActiveBranch (kEvtBranch);
   ns\_tfi.addActiveBranch (kSummaryBranch);
```

**OR**

```
   // select only certain branches to be thrown away
   ns\_tfi.addInactiveBranch(kNumTopFitBtagsBranch);    
   ns\_tfi.addInactiveBranch(kNumXFTHitsBranch);        
```
Note that any branches are _registered_ (_e.g.,_ using addTcaBranch()) must either be on the _active branch list_, or **not** on the _inactive branch list_. The TreeFileInterface class will make sure this is the case by adding or removing branches from the appropriate list.

The final option to make before looping over the events is the possibility of adding a new branch to the cloned tree. You can add simple objects (_e.g.,_ integers and floats), classes that derive from TObject, or TClonesArray of objects. For the simple objects, you will need to give the address of the object you want in the branch; for the latter two, you will need to give the address of **the pointer** of the object you want.

```
   Int\_t myIntValue;
   SomeClass \*classPtr = new SomeClass();
   TClonesArray \*tcaPtr = new TClonesArray ("SomeClass");
   
   ns\_tfi.addCloneTreeBranch ("myValue", &myIntValue, "myValue/I");
   ns\_tfi.addClassCloneTreeBranch ("myBranch", &classPtr, "SomeClass");
   ns\_tfi.addCloneTreeBranch ("myTcaBranch", &tcaPtr);
```
Note that we must finish all of the above before calling getNextEvent() for the first time. Doing otherwise will lead to very unpredictable results. Now, we loop over the events using getNextEvent(). If we want to _thin_ a branch, we can do something like the following:

```
      TClonesArray \*jetTcaPtr = ns\_tfi.tcaPtr (kJetBranch);
      Int\_t numJets = jetTcaPtr->GetEntries();
      vector< jet > jetVec;
      for (int jetLoop = 0; jetLoop < numJets; ++jetLoop)
      {
         jet \*jetPtr = (jet \*) jetTcaPtr->At(jetLoop);
	    
         // is this jet the right collection
         if (JETTYPE\_Jetclu\_04\_LJ == jetPtr->CollType)
         {
            // Store the jet corrections
            // ...

            // Put it on the new list
            jetVec.push\_back( \*jetPtr );
            
         } // if we want this jet
      } // for jetLoop
      
      // correct the number of jets
      int \*numJetsPtr = ns\_tfi.ptr <int> (kNumJetObjsBranch);
      \*numJetsPtr = numJets = (int) jetVec.size();

      // get rid of the old jets
      jetTcaPtr->Clear();
     
      // now add them back on
      for (int loop = 0; loop < numJets; ++loop)
      {
         new ( (\*jetTcaPtr) \[loop\] ) jet ( jetVec\[loop\] );
      }
```

The final choice inside the event loop is whether or not we want an individual event or not. Once we have decided to take an event, we use fillCloneTre()
```
      if (takeEvent)
      {
         ns\_tfi.fillCloneTree();
         ++numActual;
      }
```
That's almost everything. The last thing we need to do is make sure that the file gets written out at the end.

```
   if (! ns\_tfi.writeFinalCloneTree())
   {
      cerr << "File wasn't written correctly" << endl;
      // assert causes a segfault and will tell the CAF that this
      // section was not successful.
      assert(0);
   }
```
**Note:** writeFinalCloneTree() will get called automatically if you do not put it in your code. It is better that you call it so that you can tell your job to crash if it is not successful (so that the CAF knows this section did not succeed).

Output Trees
------------

When using TChains, most of us are used to opening the input file and _hooking up_ the input tree first and then opening the output file and the output tree last. Using the RootFileCollection or TreeFileInterface, this is no longer possible: you must open the output file first.

Extra attention needs to be paid if you are writing out trees that can get large. Root is smart enough to automatically open a new output file in this case, but not smart enough to be able to close it well unless you help it. Note that if you are using the CloneTree option of TreeFileInterface [above](#skim), this is done for you automatically.

When opening the output file, you **need** to use use a TFile pointer, and **not** a regular TFile object.

```
TFile \*filePtr = new TFile ("output.root", "RECREATE"), or  
TFile \*filePtr = TFile::Open ("output.root", "RECREATE"), but not  
TFile file("output.root", "RECREATE") &lArr **bad**
```

The problem with the last is that when Root changes the output file because of size, file no longer represents the current file. When the program ends, it tries to close this file, but finds that it is already closes and crashes.

Next, you open the input files and loop over all the events. When you are finished, you want to make sure that you write out your output trees. Because Root may have switched the output tree, you do not want to use filePtr->Write(). Instead, you need to tell Root to switch to the latest file and then write out (all) objects (trees and histograms).

```
   outputTreePtr->GetCurrentFile()->cd();
   outputTreePtr->Write();
   cout << "Writing complete." << endl;
```

**Note:** Do **not** delete filePtr at the end of your code. This is not a memory leak as this is the end of your program. If you do delete filePtr, you will crash the prgram.

Disk Analysis Pool and its Doors
--------------------------------

The disk analysis pool currently has 13 "doors" (ports). We want to make sure that these ports get used evenly (_e.g.,_ we do not want somebody running 1000 sections all hitting the same door).

To that end, the RootFileCollection class will pick doors if told to. Instead of hardwiring a port with a line like

dcap://fcdfrdc3.fnal.gov:22131/pnfs/top/charles.root **⇐ bad**

tell RootFileCollection that you are using dcap with fcdfrdc3:

dcap fcdfrdc3 /pnfs/top/charles.root

Currently, RootFileCollection only knows about dcap addresses that are on fcdfrdc3, and assumes that all doors between 22126 and 22130 work. Since these doors are non-kerberized, you must unset the DCACHE\_IO\_TUNNEL environment variables. You can either do this in before you run OR you can use the unsetDcacheIOtunnel=1 if you are using TFINamespace.

If you are not using TFINamespace, you can unset the variable inside your code. I strongly recommend setting up an option of the same name unsetDcacheIOtunnel:

```
#include if (gUnsetDcacheIOtunnel)
   {
      unsetenv ("DCACHE\_IO\_TUNNEL");
   }
```

**Important:** If you unset DCACHE\_TO\_TUNNEL, you will **NOT** be able to access SAM files.

If you need to change which doors are used, you can use a specially formatted [a text file](availPorts.txt) to define which doors are used. If you use TFINamespace, you can use the command line option availportsname=availPorts.txt. Otherwise, you can call RootFileCollection::addPortsFromFile ("availPorts.txt") from your code.

**Note:** If you would rather use TChains instead of the RootFileCollection, you can get a TChain from a list of files (with the dcap port assignment) using TChain\* RootFileCollection::chainFromFile ("listname.txt", "TopTree") where the first argument is the file containing the list of root files and the second is tree name.

SAM Environment
---------------

This description is **not** intented as a substitution for reading the [SAM documentation](/tiki/tiki-index.php?page=CdfSamUserDocumentation).

In order to run on data in SAM, you need to setup a SAM project. This needs to be an unique string (I recommend a method below). You need to tell the SAM project what dataset you would like to use. In the example below, I use the bmclkh dataset.

unix> setenv SAM\_PROJECT \`whoami\`.\`date +%s\`
unix> echo $SAM\_PROJECT
cplager.1150314119
unix> sam start project --project=$SAM\_PROJECT --defname=bmclkh --group=test

You can now ready to run topThin.exe or topAnaExample.exe If you finish before running on all of the files, stop the SAM project.

unix> sam stop project --project=$SAM\_PROJECT --force 

Please visit the [SAM documentation](/tiki/tiki-index.php?page=CdfSamUserDocumentation) for for details.

**Important:** When using SAM while [running on the CAF](tools.html#caf), you do **NOT** set up a SAM project. The CAF submission process does this automatically for you and doing this yourself can **crash** SAM.

### Making a SAM Recovery Project

As can happen for many reasons, CAF sections can fail (_e.g.,_ A job crashes, SAM does not deliver a file, etc.).

To generate a SAM recovery project, you

```
unix> sam generate strict recovery project --project=\[Original Project Name\] --recoDefname="recovery\_\[Original Project Name\]"

unix> sam list files --dim="DATASET\_DEF\_NAME = recovery\_\[Original Project Name\]"
```

The list is not actually needed, but it's nice to see what files you're going to run on again.

```
unix> setup cdfsoft2 development

unix> CafSubmit --tarFile=topAnaExample.tgz \\
--outLocation=$USER@fcdflnx7.fnal.gov:/cdf/scratch/$USER/output\\$.tgz \\
--dataset=recovery\_\[Original Project Name\] --dhaccess=SAM \\
--email=$USER@fnal.gov --procType=short --start=1 \\
--end=999 ./runCAFjob.bash \\$ 
```

The RootFileCollection Class
----------------------------

RootFileCollection is the base class of TreeFileInterface that allows us to access files that are local, on ROOTD, on DCAP, or in SAM. For most people, they should use the derived class [TreeFileInterface](#TFI) instead.

In the code, this access consists of two parts: choosing the location of the files and looping over the files. To implement the SAM access, I used Thomas Kuhr's (very easy to use) TSam class. The class is made up of two files:

*   [RootFileCollection.h](http://cdfkits.fnal.gov/SamCode/source//RootFileCollection/RootFileCollection/RootFileCollection.h?v=RootFileCollection)
*   [RootFileCollection.cc](http://cdfkits.fnal.gov/SamCode/source//RootFileCollection/src/RootFileCollection.cc?v=RootFileCollection)

Setting up the RootFileCollection is done as below. Note that when using SAM, you will need to setup extra environment variables to tell it what files to use (See [The SAM Environment](#samenv) above).

```
   bool useSam = false;
   RootFileCollection fileCollection;
   if ( useSam )
   {
      // tell the file colleciton to use Sam
      fileCollection.setCollectionType( RootFileCollection::kSam );
   } // if using sam 
   else
   {
      // tell the file collection tot use filelists
      fileCollection.setCollectionType( RootFileCollection::kFileList );
      // add a list of files
      fileCollection.addFilesFromList (gListName);
      // // or add one file at a time
      // fileCollection.addFile (gFileName);
      // Tell the file collection that this is a caf section
      if (gSection && gTotalSection)
      {
         fileCollection.setSegment (gSection, gTotalSection);
      }
   } // if not using sam
```

**Note:** I **highly** recommend that you create a RootFileCollection instance (_e.g.,_ RootFileCollection tfi;) instead of using new and creating a pointer (_e.g.,_ RootFileCollection \*tfiPtr = new RootFileCollection();). There is no advantage of a pointer and you the destructor of RootFileCollection must be called for SAM jobs to finish successfully. If you do use pointers, you **must** delete the pointer at the end of the job.

When using SAM, you will tell the program which files to run over by setting up your [SAM environment](#samenv).

When using filelists, you either give it a file that contains a list of root files **or** you tell it which file you want to run on. Note that you can add multiple lists or files using addFilesFromList or addFile multiple times.

When using filelists on the CAF (see [Running on the CAF](tools.html#caf)), you can tell the program to use only a part of the files. For example, if the program is currently running section 13 out of 40 sections, fileCollection.setSegment (13, 40) will select the proper subset of files.

To access the files, it is a simple while loop:

```
   while (TFile \*filePtr = fileCollection.nextFile())
   {
      TTree \*inputTreePtr = (TTree\*) filePtr->Get("TopTree");
      if (! inputTreePtr)
      {
         cerr << "Warning: '" << filePtr->GetName() 
              << "' did not contain a 'TopTree'." << endl;
         continue;
      }
      // hookup input tree
      setupInputTree(inputTreePtr);

      cout << "looking at " << filePtr->GetName() << endl;

      // Hook up the tree and put your code here
   } // while filePtr
```

Note that this is a different way of looping over many files than we are used to with TChain. This is necessary since SAM only returns one file at a time.

Command Line Options
--------------------

Included in the RootFileCollection is the OptionUtils namespace that allows an easy way to parse command line options when building stand-alone executables. Note that if you are using TFINamespace, many of these are setup for you automatically. Adding others is easy:

```
// The include file
#include "RootFileCollection/OptionUtils.h"

// Much easier if you use the namespace
using namespace OptionUtils;

// Global variables that you want to change
int gSomeVariable = 3;
bool gThin = false;

// forward declarations
// The routine to hook up the variables to the strings that represent
// them.
void localInitialize();
```

The localInitialize() function looks something like this:

```
void localInitialize()
{
   // Integers
   // Three strings for the same integer
   addOptionKey ("somevar",   gSomeVariable);
   addOptionKey ("thin",      gThin);
}
```
To hook this up, simply call localInitialize() and then parseArguments() with argc and argv:

```
int main(int argc, char\*\* argv)
{
   // Parse Command Line Arguments          
   ...
   localInitialize();
   parseArguments (argc, argv);
```

When an executable is run, the arguments after the executable name are parsed into the variables. For example;

unix> ./myExe.exe num=1e3 list=files.list thin=0

will set

gNumWanted

⇒ 1000,

gListname

⇒ files.list,

gThin

⇒ false (0; use 1 for true),

Finally, there are two other options that are always defined.

\-print

⇒ Prints out the current values of all options and continues running the program. This is a useful option to put in your CAF jobs so you show what options are used in your log file.

\-exit

⇒ Prints out the current values of all options and exits. Useful for debugging.

Using the RootFileCollection Library in Root/Cint
-------------------------------------------------

I **strongly** recommend using standalone compiled executables (linked against the Root libraries) **instead of** using Root and Cint (see [A Top Analysis Framework](tools.html) for examples and Makefiles).

For those of you who haven't heeded the above warning, here is how to use Root. You will need to setup your [environment](tools.html#environment) first. Once inside Root, you will need to load in three libraries by hand.

gSystem->Load("libsam\_cpp\_api.so");
gSystem->Load("libdiskcache\_samroot.so"); 
gSystem->Load("RootFileCollection.so");

Now simply compile your Root script and you should be good to go. **Note**, using uncompiled Root scripts is a recipe for disaster. If you do this, you are on your own.

Examples
--------

Below are three examples (all using Top Ntuples) of using the TreeFileInterface.

*   [topAnaExample.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topAnaExample/topAnaExample.cc)
*   [topMerge.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topMerge/topMerge.cc)
*   [topThin.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/topThin.cc)

### Choosing Input Source (old)

**Note:** This is now out of date, but I am keeping it for a while. Please use the advice [above.](#input)

The first step is to tell the TreeFileInterface instance where it should get its files from.

```
   bool useSam = false;
   TreeFileInterface tfi;
   if ( useSam )
   {
      // tell the file colleciton to use Sam
      tfi.setCollectionType( RootFileCollection::kSam );
   } // if using sam 
   else
   {
      // tell the file collection tot use filelists
      tfi.setCollectionType( RootFileCollection::kFileList );
      // add a list of files
      tfi.addFilesFromList (gListName);
      // Tell the file collection that this is a caf section
      if (gSection && gTotalSection)
      {
         tfi.setSegment (gSection, gTotalSection);
      }
   } // if not using sam

```

**Note:** I **highly** recommend that you create a TreeFileInterface instance (_e.g.,_ TreeFileInterface tfi;) instead of using new and creating a pointer (_e.g.,_ TreeFileInterface \*tfiPtr = new TreeFileInterface();) as there is no advantage in using a pointer in this case (and possible problems caused by it). In order for SAM jobs to finish successfully, the destructor of TreeFileInterface must be called; if you do use pointers, you **must** delete the pointer at the end of the job.

* * *

Last modified: Sat June 10 11:17:48 CDT 2008
