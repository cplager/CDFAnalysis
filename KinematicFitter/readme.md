# Kinematic Fitter

*   [Introduction](#intro)
*   [An Annotated Example](#example)
*   [Massive Jets](#massivejets)
*   [Mass Constraints](#constraint)
    *   [Changing Constraints](#changeconstraint)
*   [Finetuning the Fitter](#finetuning)
    *   [Iterating](#iterating)
    *   [Covariance Matrix Modes](#covariance)
    *   [Fitter Basis](#basis)
*   [Building the <tt>KinematicFitter</tt> library](#build)
*   [To Do list](#todo)

<a name="intro"></a>

## Introduction

This library, <tt>KinematicFitter</tt> is an implementation of [Paul Avery](http://www.phys.ufl.edu/~avery/)'s [kinematic fitting algorithms](http://www.phys.ufl.edu/~avery/fitting.html). This implementation has been a collaboration between Marcel Stanitzki, Mark Neubauer, Antonio Boveia, Elliot Lipeles, and myself. You can check out the current version from cvs:

<pre>unix> cvs -d :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2 co KinematicFitter 
</pre>

You can also take a look at the files [on the web](KinematicFitter/). The [header](KinematicFitter/KinematicFitter/KinematicFitter.hh) and [source files](KinematicFitter/src/KinematicFitter.cc) are the most informative.

<tt>KinematicFitter</tt> is an iterative χ² that _wiggles_ input <tt>TLorentzVector</tt>s within their errors to meet given constraints. This package does not depend on the <tt>cdfsoft</tt> environment, and should run (more or less) anywhere where <tt>Root</tt> is installed.

<a name="example"></a>

## An Annotated Example

For this example, we are going to be trying to reconstruct an all hadronic top pair decay (<tt>tt ⇒ WbWb</tt>, where both <tt>W</tt>s decay to jets).

First, start with an instance of the <tt>KinematicFitter</tt>

<pre>   KinematicFitter fitter;
</pre>

We then want to <tt>create</tt> our particles that represent the decay. We will tell the fitter about our <tt>input</tt> particles when we are inside the tree loop.

<pre>   int W1c   = fitter.createVirtualParticle();
   int B1c   = fitter.createVirtualParticle();
   int Top1c = fitter.createSuperVirtualParticle (W1c, B1c);
   int W2c   = fitter.createVirtualParticle();
   int B2c   = fitter.createVirtualParticle();
   int Top2c = fitter.createSuperVirtualParticle (W2c, B2c);
</pre>

A _virtual_ particle is a particle that will depend entirely on _input particles_ (below). A _virtual super particle_ is one that depends on other _virtual particles_. This notation seems a little strange at first, but is powerful in that it allows us to create all of our constraints outside the tree loop.

These functions return the index of the virtual particle. Using these indicies, we want to create our mass constraints. We will create multiple constraints first and turn different constraints off depending on what we want to do.

<pre>   // Put Mass constraints on the virtual particles
   // The Ws
   int constr_W1 = fitter.addSingleMassConstraint   (W1c,   80.425);
   int constr_W2 = fitter.addSingleMassConstraint   (W2c,   80.425);
   // Now, the tops - Note we will not have all three active at the
   // same time.
   //    - Equal mass constraint
   int constr_top  = fitter.addEqualMassConstraint  (Top1c, Top2c);
   //    - Two single mass constriants
   int constr_top1 = fitter.addSingleMassConstraint (Top1c, 175.);
   int constr_top2 = fitter.addSingleMassConstraint (Top2c, 175.);
</pre>

In this example, we either want to

*   Constrain the first W only
*   Constrain both Ws and
    *   Do not constrain the top masses,
    *   Constrain both top masses to be the same, **or**
    *   Constrain both top masses to be exactly 175 GeV

<pre>   // First W only?
   if (gFirstWonly)
   {
      fitter.setConstraintInactive (constr_W2);
      fitter.setConstraintInactive (constr_top);
      fitter.setConstraintInactive (constr_top1);
      fitter.setConstraintInactive (constr_top2);
   } else 
   {
      // What should we do about the tops?
      if (1 == gMassConstraintMode)
      {
         cout << "equal mass" << endl;
         fitter.setConstraintInactive (constr_top1);
         fitter.setConstraintInactive (constr_top2);
      } else if (2 == gMassConstraintMode)
      {
         // Use two single mass constraints
         cout << "single mass" << endl;
         fitter.setConstraintInactive (constr_top);
      } else {
         // Use no mass constraint
         cout << "no mass" << endl;
         fitter.setConstraintInactive (constr_top);
         fitter.setConstraintInactive (constr_top1);
         fitter.setConstraintInactive (constr_top2);
      }
   } // if ! gFirstOnly
</pre>

The reason we set up many different constraints is that we want to be able to try different constraints inside the event loop. To make sure we start each event with the fitter in the same state, we save the current constraints (_i.e.,_ which constraints are active and what masses the single mass constraint uses).

<pre>   // save the current constraints as the defaults.
   fitter.saveCurrentConstraintsAsDefault();
</pre>

We have now setup our fitter and are ready to iterate over events in a tree (or wherever else you may have them).

<pre>   for (int loop = 0; loop < nentries; ++loop)
   {
      // reset the constraints in case we played with them last iteration
      fitter.resetConstraints();

      // clear the input particles from last event
      fitter.clearAllInputParticles();

      if (gVerbose)
      {
         cout << endl << endl << "entry " << loop + 1 << endl
              << fitter << endl;
      }
      // load the TLorentzVectors from the tree
      treePtr->GetEntry (loop);
      TLorentzVector vec0  ( *((TLorentzVector*) reconTcaPtr->At(0)) );
      TLorentzVector vec1  ( *((TLorentzVector*) reconTcaPtr->At(1)) );
      TLorentzVector vec2  ( *((TLorentzVector*) reconTcaPtr->At(2)) );
      TLorentzVector vec3  ( *((TLorentzVector*) reconTcaPtr->At(3)) );
      TLorentzVector vec4  ( *((TLorentzVector*) reconTcaPtr->At(4)) );
      TLorentzVector vec5  ( *((TLorentzVector*) reconTcaPtr->At(5)) );
</pre>

Now, we want to tell the fitter about all of the interactive particles. It is at this stage that we tell the fitter about the resolution of each particle as well. We can pass in a <tt>TLorentzVector</tt> as well as the uncertainties on the energy, θ (not η), and φ. You can

1.  put in all these pieces at once:

    <pre> int part1 = fitter.addInputParticle (vec1, dE_1, dTheta_1, dPhi_1);
     int part2 = fitter.addInputParticle (vec2, dE_2, dTheta_2, dPhi_2);
    </pre>

2.  pass in a 4 by 4 TMatrixD error matrix (in the **E**, **P<sub>x</sub>**, **P<sub>y</sub>**, **P<sub>z</sub>** basis), or

    <pre> int part3 = fitter.addInputParticle (vec3, myTmatrixD3);
     int part4 = fitter.addInputParticle (vec4, myTmatrixD4);
    </pre>

3.  pass in particles first and update the errors later:

    <pre> int part5 = fitter.addInputParticle (vec5);
     int part6 = fitter.addInputParticle (vec6);
     fitter.updateErrorForInputParticle  (part5, dE_5, dTheta_5, dPhi_5);
     fitter.updateErrorForInputParticle  (part6, dE_6, dTheta_6, dPhi_6);
    </pre>

Except in MC studies, you will not know which input particles are associated with which <tt>virtual</tt> particles. You will only need to tell the fitter once per event about the input particles and simply change the parings later.

**Note:** In this example, we will want to have different versions of jets (_i.e.,_ different magintudes of the <tt>TLorentzVector</tt> as well as different resolutions) depending on whether we think a jet is a B jet or a W daughter. In this case, we will simply load in two different copies of each jet using <tt>addInputParticle()</tt> and make sure we hook the right one up.

Once we have decided which jets are associated with which particles, we hook them up:

<pre>      // update the Ws
      fitter.updateVirtualParticle (W1c, part1, part2);
      fitter.updateVirtualParticle (W2c, part3, part4);
      // update the Bs
      fitter.updateVirtualParticle (B1c, part5);
      fitter.updateVirtualParticle (B2c, part6);
</pre>

Now we are ready to calculate the χ²:

<pre>      double chi2 = fitter.minimizeChi2();
</pre>

The last thing we usually want to do is get back the <tt>TLorentzVector</tt>s. We can get them back all at once

<pre>      KinematicFitter::FourVectorVec origVec, modVec;
      fitter.fillAllVirtualMomentumVector (origVec,
                                           KinematicFitter::kOriginal);
      fitter.fillAllVirtualMomentumVector (modVec,
                                           KinematicFitter::kModified);
      // The values before the chi^2 minimization
      TLorentzVector w1beforevec   = origVec [W1c];
      TLorentzVector w2beforevec   = origVec [W2c];
      TLorentzVector top1beforevec = origVec [Top1c];
      TLorentzVector top2beforevec = origVec [Top2c];
      // After the minimization
      TLorentzVector w1aftervec    = modVec  [W1c];
      TLorentzVector w2aftervec    = modVec  [W2c];
      TLorentzVector top1aftervec  = modVec  [Top1c];
      TLorentzVector top2aftervec  = modVec  [Top2c];
</pre>

Or, one at a time

<pre>      TLorentzVector top2after =
        fitter.virtualParticleMom (Top2c,
                                   KinematicFitter::kModified);
</pre>

You also have access to the input particles, using <tt>fillAllInputMomentumVector()</tt> and <tt>particleMom()</tt>, again using <tt>KinematicFitter::kOriginal</tt> and <tt>KinematicFitter::kModified</tt>.

<a name="massivejets"></a>

## Massive Jets

CDF, by default, uses massive jets. What is unusual about these objects is that the mass of a jet is **not** conserved when a jet is scaled. Because this is not the regular behavior for most 4 vectors, the fitter has to be told explicitly that it should treat the particle as a massive jet. **Important:** The following **only** works if you are using the fitter in the (m, **P**) basis. This is the default basis, so this will work **unless** you change it to run in the (E, **P** basis. See the [Fitter Basis section](#basis) for more details.

If you are only dealing with jets, you can tell the fitter to use only massive jets.

<pre>   // Massive particles by default
   KinematicFitter::setDefaultParticleType (KinematicFitter::kMassiveJet);
</pre>

or

<pre>   // Normal particles by default
   KinematicFitter::setDefaultParticleType (KinematicFitter::kNormalParticle);
</pre>

You can also tell the fitter what type of particle it is when using <tt>addInputParticle()</tt>, by including the type (<tt>KinematicFitter::kMassiveJet</tt> or <tt>KinematicFitter::kNormalParticle</tt>) after the <tt>TLorentzVector</tt>. The [annotated example above](#example) now looks like:

<pre> int part1 = fitter.addInputParticle (vec1, KinematicFitter::kMassiveJet, dE_1, dTheta_1, dPhi_1);
 int part2 = fitter.addInputParticle (vec2, KinematicFitter::kMassiveJet, dE_2, dTheta_2, dPhi_2);
 int part3 = fitter.addInputParticle (vec3, KinematicFitter::kMassiveJet, myTmatrixD3);
 int part4 = fitter.addInputParticle (vec4, KinematicFitter::kMassiveJet, myTmatrixD4);
 int part5 = fitter.addInputParticle (vec5, KinematicFitter::kMassiveJet);
 int part6 = fitter.addInputParticle (vec6, KinematicFitter::kMassiveJet);
</pre>

<a name="constraint"></a>

## Mass Constraints

The current constraints come in two forms: single mass constraints and double mass constraints.

**Single mass constraints** - you ask for a single virtual particle to be contrained to a specific mass.

**Equal mass constraints** - you ask that two virtual particles are constrained to have the same but unspecified mass. Useful in bump hunting.

I have implemented intrinsic widths as Gaussians.

<a name="changeconstraint"></a>

## Changing Constraints

In an attempt to make this package more useful, you can change the constraints easily. From the header file, we have the complete list of appropriate member functions:

<pre>// Add a mass constraint of a virtual particle.  Returns
// constraint index.
int addSingleMassConstraint (int virtIndex, 
                             double mass, double width = -1.);

// Add a mass constraint to two virtual particles (to each
// other).  Returns constraint index.
int addEqualMassConstraint (int virtIndex1, int virtIndex2);

// update a single mass constraint mass (inputs are constraint
// index and new mass)
void updateSingleMassConstraint (int constraintIndex, 
                                 double mass, double width = -1.);

// return the current mass constraint of a single mass constraint
double currentSingleMassConstraint (int constraintIndex);

// sets a constraint as active
void setConstraintActive (int constraintIndex);

// sets a constraint as inactive
void setConstraintInactive (int constraintIndex);

// returns true if the given constraint is active
bool isConstraintActive (int constraintIndex);

// Save the current state of constraints as the default
void saveCurrentConstraintsAsDefault();

// reset all constraints to their defaults
void resetConstraints();
</pre>

<a name="finetuning"></a>

## Finetuning the Fitter

<a name="iterating"></a>

### Iterating

The <tt>KinematicFitter</tt> fitter is an iterative fitter. You can control how it iterates by setting three variables with member functions:

*   <tt>setNumIterations()</tt> - the maximum number of iterations. Default is 10.
*   <tt>setChi2Delta()</tt> - the Δχ² that tells the fitter when to break out of iteration loop (before reaching mamimum). Default is 0.01.
*   <tt>setMaxChi2()</tt> - the upper limit of χ² (above this value, it is assumed that this is not the right combination and we do not want to waste cycles iterating. Default is 1e20.

<a name="covariance"></a>

### Covariance Matrix Modes

There are (currently) two covaraince modes, set by <tt>setCovarianceMode()</tt>

*   <tt>KinematicFitter::kNormalCovarMode</tt> - Makes a new covariance matrix each iteration. The covariance matrix has a 4 by 4 entry for every input particle. This is the "safe" mode.
*   <tt>KinematicFitter::kFastCovarMode</tt> - Makes a new covariance matrix for each iteration. This covariance matrix only has 4 by 4 entries for input particles that are used in the χ² calculation. This is the default setting.
*   <tt>KinematicFitter::kKeepCovarMode</tt> - Makes a new covariance matrix only once per event.
*   <tt>KinematicFitter::kRecalcCovarMode</tt> - Recalculates the covariance matrix every iteration. This avoids problems probability distributions having peaks at low values. Should probably become the default.

<a name="basis"></a>

### Fitter Basis

The fitter can use either an interal representaion of (E, **P**) or (m, **P**). The first, (E, **P**)w, is more natural as we pass in <tt>TLorentzVector</tt>s. Paul Avery recommends the second (m, **P**) as more stable. In addition, the covariance matrix in the (E, **P**) basis assumes all input particles are massless. Even if we correct the matrix (we should), it would not deal well with CDFs massive jets. In any case, you can switch modes right after declaring an instance of <tt>KinematicFitter</tt> **before** adding any particles. The fitter uses the (m, **P**) basis by default.

<pre>   KinematicFitter fitter;
   // Use (M, P) basis
   fitter.changeBasis (KinematicFitter::kM_P_basis);
</pre>

or

<pre>   KinematicFitter fitter;
   // Use (E, P) basis
   fitter.changeBasis (KinematicFitter::kE_P_basis);
</pre>

<a name="build"></a>

## Building the <tt>KinematicFitter</tt> library

### Using the <tt>CDFSOFT</tt> environment

If you have decided to use the <tt>CDFSOFT</tt> environment, you should:

1.  Change to the right directory

    <pre>unix> cd releaseDir
    	  </pre>

2.  Check out the <tt>KinematicFitter</tt> package

    <pre>unix> cvs -d :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2 co KinematicFitter
    	  </pre>

3.  Add a softlink to the include directory

    <pre>unix> cd include; ln -s ../KinematicFitter/KinematicFitter
    	  </pre>

4.  Make the library

    <pre>unix> gmake KinematicFitter.nobin USESHLIBS=1
    	  </pre>

### Using a generic Root environment

If you want to use a more generic environment, you need to make sure that you have <tt>$ROOTSYS</tt> set. After that, you need to setup some directory structure. I am going to assume that your top level directory is called <tt>parentDir</tt>.

1.  Setup the necessary directories

    <pre>unix> mkdir parentDir/shlib; mkdir parentDir/include
    	  </pre>

2.  Check the package out of CVS

    <pre>unix> cd parentDir; cvs -d :pserver:anonymous@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2 co KinematicFitter
    	  </pre>

3.  Tell the include directory about <tt>KinematicFitter</tt>

    <pre>unix> cd parentDir/include; ln -s ../KinematicFitter/KinematicFitter
    	  </pre>

4.  Setup the correct makefile in <tt>KinematicFitter</tt>

    <pre>unix> cd parentDir/KinematicFitter; rm GNUMakefile; mv Makefile.no_cdfsoft Makefile
    	  </pre>

5.  Make the shared library

    <pre>unix> cd parentDir/KinematicFitter; make
    	  </pre>

6.  To use the example:

    <pre>unix> cd parentDir; mkdir exampleKF
    unix> cp KinematicFitter/examples/{Makefile.no_cdfsoft,exampleKF.cc} exampleKF/
    unix> cd exampleKF; mv Makefile.no_cdfsoft Makefile
    unix> make	  
    unix> ln -s ../shlib/libKinematicFitter.so .
    unix> ./exampleKF.exe

    	  </pre>

<a name="todo"></a>

## To Do List

### The Short List

On the short list, we need to implement the correct covariance matrix - it should depend on **dE**, **dθ**, and **dφ** where as it currently only uses **dE** <font color="red">⇐ **Done.**</font>

### The Not-So Short List

On the longer list, we may consider the following:

*   Change of basis -  
    The internal basis is currently **E**, **P<sub>x</sub>**, **P<sub>y</sub>**, **P<sub>z</sub>**. Paul Avery recommends replacing energy with mass (_e.g.,_ using **M**, **P<sub>x</sub>**, **P<sub>y</sub>**, **P<sub>z</sub>**) as this leads to more stable results (and therefore quicker iterating). <font color="red">⇐ **Done**</font>
*   Missing Transverse Energy constraints -  
    Mark Neubauer's implementation has this already. We need to see how to apply error conditions properly.
*   Back to back constraints -  
    Could be interesting.
*   Intrinsic Width of Particles <font color="red">⇐ **Done**</font> for Gaussian widths. Not yet implemented for Breit-Wigner widths.

* * *

<address>Last modified: Tue Jul 25 12:15:15 CDT 2006 by Charles Plager</address>
