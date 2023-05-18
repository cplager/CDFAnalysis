 A Top Analysis Framework Using Top Ntuples

A Top Analysis Framework Using Top Ntuples
==========================================

*   [Introduction](#intro)
    *   [Browse all Top Analysis files](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/)
    *   [Browse all RootFileCollection files](http://cdfkits.fnal.gov/SamCode/source/RootFileCollection/?v=RootFileCollection)
*   [Quick Start Guide](#quick)
*   [Your Environment](#environment)
*   [Updating Packages to New Releases](#updatepack)
    *   [Updating HighLevelObject Shared Library](#hlo)
*   [The TreeFileInterface and RootFileInterface Classes](#rfc)
    *   [Complete RootFileInterface Library Documentation](rfc.html)
*   [Example Analysis Code](#example) - topAnaExample - use if you want to run an analysis job on top ntuples (either your skimmed top ntuples or the default top ntuples).
*   [Root File Merging](#merge) - topMerge - use for merging Top Ntuples.
*   [_Thin and Skim_ming](#thin) - topThin - use if you want to make your own smaller Top Ntuples.
*   [Running on the CAF](#caf)
    *   [CAF submission script](#cafsub)
    *   [Determining the Number of Sections](#numsec)
    *   [Reducing the Needed Libraries](#neededlib)
*   [Useful Scripts, etc](../UsefulScripts/useful.html)
*   [Common Problems](#problems)
    *   [MET and topFit Branches](#problems_met)
    *   [Finding the Shared Libraries](#problems_shlib)

Introduction
------------

This page is to describe how to setup a top analysis framework. Using this framework, you can _Thin and Skim™_ your own top ntuples using topThin (filling in the jet corrections at skim time) as well as run with top ntuples on many computers with root, even those without CDF soft. This page has documentation on how to skim, or even a skeleton analysis program topAnaExample that you can fill in however you like. Both of these programs let you access local ntuples, via ROOTD, DCAP, and ntuples stored in SAM. Note that you can more examples at [The Top Analysis Tools Wiki Page](http://www-cdf.fnal.gov/tiki/tiki-index.php?page=TopAnaTools).

Quick Start Guide
-----------------

If you want to get up and running first and figure out what you're doing next, here are the necessary steps. I am going to assume that your top level directory is called parentDir/.

**Note:** To avoid typing mistakes, you should simply highlight and copy the six lines below and paste them into a terminal window.

source ~cdfsoft/cdf2.cshrc
setup cdfsoft2 6.1.4
cvs co -d parentDir TopMods/[setupTopAnalysis.pl](http://cdfkits.fnal.gov/CdfCode/source/TopMods/setupTopAnalysis.pl)
cd parentDir
./setupTopAnalysis.pl 
gmake

If you want to build this on a machine **without** CDFSOFT, you can set the CVSROOT environment variable to :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2 and then do the cvs co command above (you may need to set the CVS\_RSH environment variable to ssh). See [setupTopAnalysis.pl](http://cdfkits.fnal.gov/CdfCode/source/TopMods/setupTopAnalysis.pl) for more details.

The setupTopAnalysis.pl script will check out the correct (_i.e.,_ tagged) versions of BTagObjects, JetUser, RootFileCollection, and the new JointPhysics libraries.

This gmake will compile these (and other) shared libraries as well as the stand alone executables. Lots of stuff scrolls by. If you want to tar up everything you have just checked out, simply type gmake tar in the parent directory and topanalysis.tgz will be made for you. To run on computers without CDFSOFT (and therefore without SAM), simply set the environment variable NO\_CDF\_SAM to 1 before compiling.

unix> setenv LD\_LIBRARY\_PATH "$LD\_LIBRARY\_PATH":"$PWD/shlib"
unix> cd topAnaExample
unix> ./topAnaExample.exe file=/cdf/scratch/cplager/

(prints out run and event numbers)

Read below to find out more options, such as how to run on files in [SAM](rfc.html#samenv).

Your Environment
----------------

This code can use SAM, ROOTD, and DCAP to access Top Ntuples. In order to use SAM, you must setup the cdfsoft2 environment. You will also need to tell it where to find the shared objects. Please see the [Running on the CAF](#caf) section if you want to run on the CAF.

unix> setup cdfsoft2 6.1.4
unix> cd parentDir
unix> setenv LD\_LIBRARY\_PATH "$LD\_LIBRARY\_PATH":"$PWD/shlib"

Note that you need to do this before you can make or run this code. **Please** make sure you setup diskcache\_i **after** setting up cdfsoft2 6.1.4 as cdfsoft2 6.1.4 will set up an older (non-usable) version of diskcache\_i. To use SAM, you have to setup the proper [SAM environment](rfc.html#samenv).

In general, this code has been designed to not need the cdfsoft2 environment. Unfortunately, it is needed to get SAM access to root files. If you do not want SAM (or if you want to use this code in a **5.X.X** version of CDFSOFT), you can forgo the use of cdfsoft2 environment by simply setting the environment variable NO\_CDF\_SAM to 1 unix> setenv NO\_CDF\_SAM 1

before compiling RootFileCollection or any of the other examples that use it.

You need to tell the environment where to find the shared libraries it needs. You can

*   unix> cd parentDir/topThin; ln -s ../shlib/lib\*.so **.** - you only need to do this once per executable directory (don't forget the **.** at the end of the command), OR
*   unix> cd parentDir; setenv LD\_LIBRARY\_PATH "$LD\_LIBRARY\_PATH":"$PWD/shlib"

Updating Packages to New Releases
---------------------------------

**Important:** After updating packages to new releases, it is important to rebuild everything from scratch: (all libraries and executables). If you have not added any additional packages yourself, typing gmake clean from the parentDir/ is sufficient. If you have added packages that you have not added to the master makefile, clean those libraries and executables by hand.

When you run [setupTopAnalysis.pl](http://cdfkits.fnal.gov/CdfCode/source/TopMods/setupTopAnalysis.pl), this script checks out tagged versions of the following packages (libraries) for you ([click here to see the current tags](http://cdfkits.fnal.gov/CdfCode/source/TopMods/setupTopAnalysis.pl#034)):

*   BTagObjects
*   JetUser
*   JointPhysics
*   RootFileCollection
*   TopAnaTools (part of the TopMods package)

Since the script used CVS to check out these packages, the CVS information is still intact and you can use CVS to get the latest version of any of these.

Updating takes two parts. First, telling CVS to update the package. Second, making any small changes necessary to run in this framework. The first part is the same for all packages. For example, if we want to update the JetUser package to the tag jetCorr06c

unix> cd parentDir/JetUser
unix> cvs update -r jetCorr06c

(**Note:** if you are really gung-ho or a developer, you may want to check out a package from the head instead of a tagged release. To do this, change the cvs command to cvs update -dAP. We don't recommend this for general use, however.)

The second step is to possibly make a few small changes to make this compatible with this framework. The table below explains all necessary changes for each of the packages. These changes should be made from the top level directory of the package (_e.g.,_ from parentDir/JetUser for the JetUser package).

Package

Changes

BTagObjects

./setup\_BTagObjectStandalone.pl

JetUser

rm -f GNUmakefile; mv Makefile.standalone Makefile

JointPhysics

rm -f GNUmakefile; mv Makefile.standalone Makefile

RootFileCollection

No Changes Needed

TopAnaTools

No Changes Needed

Note that this document deliberately does not tell you which versions to use. You need to follow the recommendations of the top and joint physics groups to decide that.

### Updating HighLevelObject Shared Library

If you want to run on Top Ntuples other than the 1infb version, you will need to rebuild the HighLevelObject shared library. Included in the tarball are all necessary files for running on 5.3.3 trees as well. In any case, you need to make sure you have the correct versions of:

*   HighLevelObjects/src/HighLevelObjects.cc
*   HighLevelObjects/include/HighLevelObjects.hh
*   HighLevelObjects/include/CollectionAndViewTypeFinder.hh - Useful but not strictly necessary
*   HighLevelObjects/dict/HighLevelObjects\_linkdef.h

Once you have all of the files, simply make the library  

unix> cd HighLevelObjects/
unix> gmake

Note that there are different versions of HighLevelObjects stored. If you want to run, for example, on 5.3.3 ntuples, you can copy the \_533 version of the above three files and remake the library.

The TreeFileInterface and RootFileInterface Classes
---------------------------------------------------

Because it is not possible to use TChains with SAM, we have introduced the [TreeFileInterface and RootFileInterface Classes](rfc.html) as a substitute. The following examples all use the [TreeFileInterface.](rfc.html#TFI)

Example Analysis Code - topAnaExample
-------------------------------------

[topAnaExample.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topAnaExample/topAnaExample.cc) is an example piece of code that hooks up some TClonesArrays and prints out run and event number. It assumes you have already built the HighLevelObject shared library.

First, you build the executable:

unix> cd topAnaExample/
unix> gmake

And then you run it on a list of files

unix> ./topAnaExample.exe list=files.list numwanted=0

or a single file

unix> ./topAnaExample.exe file=../rootfiles/myfile.root numwanted=0

or using SAM after setting up the [SAM environment](rfc.html#samenv)

unix> ./topAnaExample.exe sam=1 numwanted=0

topAnaExample uses TreeFileInterface so the branches you want will be automatically hooked up and filled for you (take a look at [topAnaExample.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topAnaExample/topAnaExample.cc) to see how this is all coded up).

To ask for a branch to be hooked up, you need to have the following line before your event loop.

tfi.addTcaBranch(kHepgBranch);

If you want to remove a branch

tfi.removeTcaBranch(kHepgBranch);

To access the hepg TClonesArrayPtr inside the event loop, you use tfi.tcaPtr(kHepgBranch). If you wanted to loop over the hepg objects, you would:

int numHepgObjects = tfi.tcaPtr(kHepgBranch)->GetEntries();
for (int loop = 0; loop < numHepgObjects; ++loop)
{
   hepg \*hepgPtr = (hepg\*) tfi.tcaPtr(kHepgBranch)->At(0);
   // do something with the hepgPtr
   cout << "HEPG ID " << hepgPtr->ID << endl;
} // for loop

Note that there is a _"feature"_ in Top Ntuples such that the classes missingEt and topFit are hooked up to branches named met and topfit respectively (note the capitalization). If you want to use the missingEt/met branch for example, you need to use tell addTcaBranch() the class name of the TClonesArray as well:

tfi.addTcaBranch(kMetBranch, kMetClassname);

to hook up the branch. But when you need to use the class name when getting access.  
missingEt \*metPtr = (missingEt\*) tfi.tcaPtr(kMetBranch);

Root File Merging - topMerge
----------------------------

topMerge.exe will be made already if you typed gmake from the top level directory (if not, you'll need to first make HighLevelObjects and then topMerge). This differs from topThin.exe in that this program clones the entire tree without you specifying which branches you want. You can see a copy of [topMerge.cc here](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topMerge/topMerge.cc).

This program is made to be run in two modes. First, as a root file merging program:

cplager@fcdflnx8> ./topMerge.exe list=inputfiles.list output=mydir/myrootfile

where inputfiles.list is the list of files to merge, and mydir/myrootfile is the prefix of the merged files.

This program can also be used to skim.

cplager@fcdflnx8> ./topMerge.exe list=inputfiles.list output=mydir/myrootfile dil=1

where we have the same options as above, except dil=1 tells the program to do a dilepton skim (lj=1 is for a lepton+jets skim and allhad=1 is for an all-hadronic skim). Note that if none of these three options is given, topMerge.exe will take all events. **Of course, Kevin needs to implement the skimming part before this works.**

**Note:** Due to the fact that only one event per run contains the trigName objects, any event that has this event will be skimmed, regardless of which options are set.

_Thin and Skim_ming - topThin
-----------------------------

For this, I am assuming that you have already compiled the libJetUser.so library and put it in the shlib directory (cd parentDir/JetUser; gmake). You can read Ulrich Husemann's page describing [how to build libJetUser.so without CDFSoft](http://www-cdf.fnal.gov/~husemann/internal/standalone.html). Note that you can get the latest JetCorr library and compile it yourself using Makefile.standalone.

The topThin skimming directory mainly consists of [topThin.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/topThin.cc). This code is very similar to the topMerge code, except that you can tell it to not skim all branches. You can either choose only the branches you want:

// select only certain branches
tfi.addActiveBranch (kEvtBranch);
tfi.addActiveBranch (kSummaryBranch);
tfi.addActiveBranch (kJetBranch);
tfi.addActiveBranch (kNumJetObjsBranch);
tfi.addActiveBranch (kMuoViewDescr1Branch);

(where tfi is the instance of the TreeFileInterface. You can also choose which branches you **do not** want:

// select to NOT take only certain branches
tfi.addInactiveBranch (kEvtBranch);
tfi.addInactiveBranch (kSummaryBranch);
tfi.addInactiveBranch (kJetBranch);
tfi.addInactiveBranch (kNumJetObjsBranch);
tfi.addInactiveBranch (kMuoViewDescr1Branch);

In the [topThin.cc](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/topThin.cc) code provided, the code currently only _thins_ the jet collection so it only takes the JETTYPE\_Jetclu\_04\_LJ collection and does Level 5 jet corrections.

When thinning a collection, make sure to update the counter (if it is there). See the example below to see how to thin the jet collection.

         TClonesArray \*jetTcaPtr = tfi.tcaPtr (kJetBranch);
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
         int \*numJetsPtr = tfi.ptr <int> (kNumJetObjsBranch);
         \*numJetsPtr = numJets = (int) jetVec.size();

         // get rid of the old jets
         jetTcaPtr->Clear();
        
         // now add them back on
         for (int loop = 0; loop < numJets; ++loop)
         {
            new ( (\*jetTcaPtr) \[loop\] ) jet ( jetVec\[loop\] );
         }

To compile this code, type gmake in the topThin directory (you can also type gmake in the parentDir to make everything). To run the skim, you need to give it a list of root files to skim from **or** to use SAM, an output file to skim to, and the number of events (0 means skim everything). You don't put the .root on the output file as the number of events skimmed will be appended.

unix> ./topThin.exe numwanted=0 list=test.list output=../Tuples/testTop2

to run on a list of files test.list, or

unix> ./topThin.exe numwanted=0 file=test.root output=../Tuples/testTop2

to run on a single file test.root, or

unix> ./topThin.exe numwanted=0 sam=1 output=../Tuples/testTop2

after setting up the [SAM environment](rfc.html#samenv).

Running on the CAF
------------------

**Important:** When using SAM on the CAF, you do **NOT** set up a SAM project. The CAF submission process does this automatically for you and doing this yourself can **crash** SAM..

This is not meant to supersede the [CAF web page](http://cdfcaf.fnal.gov/). Please visit this page as well. To use these executables on the CAF, I recommend using the [setupCafJob.pl](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/scripts/setupCafJob.pl) script.

unix> cd topAnaExample
unix>  ../scripts/setupCafJob.pl 

(text scrolls by)

This script sets up a directory called cafjob/ that will facilitate sending the job to the CAF.

### CAF submission script

After executing ../scripts/setupCafjob.pl and editing your cafjob/runCAFjob.bash script to customize it for your job, you can submit jobs to the caf using scripts/cafsubmit.csh. Assuming your in the TopAnaExample directory, the command would look like this:

unix> ../script/cafsubmit.csh cafjob XYZ

where XYZ is either the name of a file list file in the filelists directory (_e.g.,_ ptop0t.rootlist) or the name of a SAM dataset definition (bhelkd).

There are other optional parameters that let you fine-tune how you want your CAF submission to proceed. Call cafsubmit.csh with no arguments for some help on using them. As an example, you could submit a CAF job to the short queue of the fermigrid CAF where each segment processes a maximum of 10 files from the bhmuki dataset in SAM as follows:

unix>  ../script/cafsubmit.csh cafjob bhmuki -m 10 -c fermigrid -q short

Setting the queue to "test" will cause only 2 segments to run and each segment will process only 100 events. If you have a SAM job where some sections crash, you can re-run the job and tell the script the name of the SAM project in which some segments crashed and the script will run a SAM recovery job for you.

Below is an example assuming some crashed segments from the job above:

unix> ../script/cafsubmit.csh cafjob bhmuki -m 10 -c fermigrid -q short -r "original project name"

Where "original project name" can be found by grepping for "Sam2DiskCacheInterface::createAll-> Project" in any log file from the job with crashed segments.

### Determining the Number of Sections

To determine the number of sections, you need to first know how many files you can safely run in one section without going over the CAF time limit. Next, you need to know how many files are in the dataset you are processing. The number of sections you need is simply the ratio (with _1_ added to make sure we have enough):

![](numsec.png)

If you are running on a file list, simply count the number of files. The unix command wc is very useful for this. In the example below, we see that test1.list has 10 lines.

unix> wc --lines test1.list
     10 test1.list

If you are running on a SAM dataset, you need to ask SAM. The example below shows how to get the number of files in the xyz dataset.

unix> sam list files --returnCountOnly --dim="\_\_set\_\_ xyz"

### Reducing the Needed Libraries

**Note:** This step is **completely** optional. The only advantage is having a smaller tarball to send to the CAF.

The default [Makefile](http://cdfkits.fnal.gov/CdfCode/source/TopMods/AnalysisCode/topThin/Makefile) for both topThin.exe and topAnaExample.exe assume that you want to use RootFileCollection and HighLevelObjects as well as JetUser and BTagObjects. **IF** you do not want to use all of these libraries, you can tell the Makefile not to link against them.

For example, let use not use the BTagObjects library. First, we edit the Makefile changing the following line from  
FCNCLIBS=-L$(LIBDIR) -lHighLevelObjects -lJetUser -lBTagObjects -lRootFileCollection  
to  
FCNCLIBS=-L$(LIBDIR) -lHighLevelObjects -lJetUser -lRootFileCollection  

Remake the executable with the new Makefile (gmake clean && gmake). Next, we can delete the library from the cafjob directory (rm libBTagObjects.so).

The last thing we want to do is make sure (before we submit the job to the CAF) that everything works. So we run the executable with no arguments. Make sure that you do **not** have LD\_LIBRARY\_PATH set to include the shlib/ directory; if you do, the test will succeed regardless.

unix> ./topAnaExample.exe 
You must specify both an input list root file.
Use 'list=bla.list' arguments.

means that everything is good to go. If, on the other hand, we get a shell error,

unix> ./topAnaExample.exe 
./topAnaExample.exe: error while loading shared libraries: libBTagObjects.so: cannot open shared object file: No such file or directory

then we did not succeed.

Useful Scripts, etc.
--------------------

has moved to [its own page.](../UsefulScripts/useful.html)

Common Problems
---------------

### MET and topFit Branches

For almost all classes in Top Ntuple, the class name and the branch name are the same. This is **not** true for the missing ET and topFit branches. Because of this, you need to access them differently. To hoop up the branches, you will need to pass in two options

   tfi.addTcaBranch( kMetBranch, kMetClassName ); 

instead of  
   tfi.addTcaBranch( kMetBranch ); ⇐ **bad**

To use the branch, you will do something like

      missingEt \*metPtr = (missingEt\*) tfi.At (kMetBranch, 0);
      cout << "met " << metPtr->Met << " phi " << metPtr->Phi
           << endl;

### Finding the Shared Libraries

You will need to make sure that this executable can find the shared libraries. You have two choices:

*   unix> cd topThin; ln -s ../shlib/lib\*.so **.** - you only need to do this once per executable directory (don't forget the **.** at the end of the command), OR
*   unix> setenv LD\_LIBRARY\_PATH "$LD\_LIBRARY\_PATH":"/_parentDir_/shlib" - where _parentDir/_ is the top level directory. You need to do this every time you log in.

* * *

Last modified: Tue May 29 13:27:44 CDT 2007 by Charles Plager
