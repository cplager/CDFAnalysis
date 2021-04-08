// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>

#include "KinematicFitter/KinematicFitter.hh"
#include "KinematicFitter/dout.h"

using namespace std;

// static member data
KinematicFitter::TMatrixDPtrVec KinematicFitter::sm_matrixPtrVec;
KinematicFitter::ParticleType   KinematicFitter::sm_defaultParticleType =
   KinematicFitter::kNormalParticle;

//////////////////////////////////
// Constructors and Destructors //
//////////////////////////////////

KinematicFitter::KinematicFitter()
{
   // maximum number of iterations
   m_numIterations = 10;
   // delta chi^2 value after which we break out of iteration
   m_chi2delta     = 0.01;
   // value of chi^2 where we say to not bother iterating any more.
   m_maxChi2       = 1e20;
   // create a full covariance matrix, even it it is not needed, by default
   m_covarMode     = kFastCovarMode;
   // which basis are we using
   m_basis         = kM_P_basis;
   // null pointers
   m_V_a0          = 0;
}

KinematicFitter::~KinematicFitter()
{
   if (m_V_a0)
   {
      delete m_V_a0;
      m_V_a0 = 0;
   }
}

/////////////////////
// Input Particles //
/////////////////////

void
KinematicFitter::clearAllInputParticles()
{
   // First, the easy part
   m_inputParticles.clear();
   m_middleParticles.clear();
   m_outputParticles.clear();
   m_inputErrorMatricies.clear();
   // Now, the (slightly) tricker part (the virtual particles)
   for (VirtualParticleVecIter iter = m_virtualParticles.begin();
        m_virtualParticles.end() != iter;
        ++iter)
   {
      if (iter->usesInputParticles)
      {
         // clear the list of input particles
         iter->particleList.clear();
      }
   } // for iter
   // clear out the covariance matrix
   if (m_V_a0)
   {
      delete m_V_a0;
      m_V_a0 = 0;
   }
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec,
                                   ParticleType type,
                                   const TMatrixD &errMat)
{
   // make sure the error matrix is the right size
   int nRows = errMat.GetNrows();
   int nCols = errMat.GetNcols();
   assert (kNumParametersPerParticle == nRows &&
           kNumParametersPerParticle == nCols);
   int index = (int) m_inputParticles.size();
   InputParticle input;
   input.Px   = vec.Px();
   input.Py   = vec.Py();
   input.Pz   = vec.Pz();
   input.type = type;
   if (kM_P_basis == m_basis)
   {
      // M P Basis
      input.E = -1.;
      if (kNormalParticle == type)
      {
         input.mass = vec.M();
         input.percentage = -1.;
      } else if (kMassiveJet == type)
      {
         // Doesn't Root have a better way of doing this?
         double scalarMom = sqrt ( _sq ( vec.Px() ) +
                             _sq ( vec.Py() ) +
                             _sq ( vec.Pz() ) );
         input.percentage = vec.E() / scalarMom;
         input.mass = -1.;
      } else {
         assert (0);
      }
   } else {
      // E P Basis
      input.E = vec.E();
      input.mass = -1.;
      input.percentage = -1.;
   }
   m_inputParticles.push_back (input);
   m_inputErrorMatricies.push_back (errMat);
   return index;   
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec,
                                   const TMatrixD &errMat)
{
   return addInputParticle (vec, sm_defaultParticleType, errMat);
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec, 
                                   ParticleType type,
                                   double dE, double dTheta, double dPhi)
{
   TMatrixD errMat (kNumParametersPerParticle, kNumParametersPerParticle);
   _covarianceMatrixFromDeltaETP (errMat, vec, dE, dTheta, dPhi, m_basis);
   return addInputParticle (vec, type, errMat);
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec, 
                                   double dE, double dTheta, double dPhi)
{
   return addInputParticle (vec, sm_defaultParticleType, dE, dTheta, dPhi);
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec,
                                   ParticleType type)
{
   TMatrixD errMat (kNumParametersPerParticle, kNumParametersPerParticle);
   _zeroMatrix (errMat);
   return addInputParticle (vec, type, errMat);
}

int
KinematicFitter::addInputParticle (const TLorentzVector &vec)
{
   return addInputParticle (vec, sm_defaultParticleType);
}

void
KinematicFitter::updateErrorForInputParticle (int index, 
                                              double dE, double dPhi, 
                                              double dTheta)
{
   // is our index valid?
   _validateInputIndex (__FILE__, __LINE__, kOriginal, index);
   TMatrixD errorMat (kNumParametersPerParticle, kNumParametersPerParticle);
   _covarianceMatrixFromDeltaETP (errorMat, 
                                  particleMom (index, kOriginal),
                                  dE, dTheta, dPhi, m_basis);
   m_inputErrorMatricies[index] = errorMat;
}

///////////////////////
// Virtual Particles //
///////////////////////

int
KinematicFitter::createVirtualParticle (IVec &vec)
{
   IVec virtpart;
   int inSize = (int) m_inputParticles.size();
   for (IVecIter iter = vec.begin();
        vec.end() != iter;
        ++iter)
   {
      int value = *iter;
      if (value >= 0 && value < inSize)
      {
         virtpart.push_back (value);
      } // if value is o.k.
   } // for iter
   int retval = (int) m_virtualParticles.size();
   VirtualParticle particle;
   particle.particleList = virtpart;
   particle.usesInputParticles = true;
   m_virtualParticles.push_back (particle);
   return retval;
}

int
KinematicFitter::createVirtualParticle (int in1, int in2, int in3, 
                                        int in4, int in5, int in6)
{
   IVec ivec;
   ivec.push_back (in1);
   ivec.push_back (in2);
   ivec.push_back (in3);
   ivec.push_back (in4);
   ivec.push_back (in5);
   ivec.push_back (in6);
   return createVirtualParticle (ivec);
}

int
KinematicFitter::createSuperVirtualParticle (IVec &vec)
{
   IVec virtpart;
   int inSize = (int) m_virtualParticles.size();
   for (IVecIter iter = vec.begin();
        vec.end() != iter;
        ++iter)
   {
      int value = *iter;
      if (value >= 0 && value < inSize)
      {
         virtpart.push_back (value);
      } // if value is o.k.
   } // for iter
   if (! virtpart.size())
   {
      // nothing good to add
      cerr << "KinematicFitter::createSuperParticle() Warning: "
           << "No particles added."
           << endl;
      return -1; // failure
   }
   int retval = (int) m_virtualParticles.size();
   VirtualParticle particle;
   particle.particleList = virtpart;
   particle.usesInputParticles = false;
   m_virtualParticles.push_back (particle);
   return retval;
}

int
KinematicFitter::createSuperVirtualParticle (int in1, int in2, int in3, 
                                             int in4, int in5, int in6)
{
   IVec ivec;
   ivec.push_back (in1);
   ivec.push_back (in2);
   ivec.push_back (in3);
   ivec.push_back (in4);
   ivec.push_back (in5);
   ivec.push_back (in6);
   return createSuperVirtualParticle (ivec);
}

void
KinematicFitter::updateVirtualParticle (int virtualIndex, IVec &vec)
{
   // first validate index
   _validateVirtualPartIndex (__FILE__, __LINE__, kOriginal, virtualIndex, 
                              true);
   // now make sure this virtual particle is NOT a super particle
   VirtualParticle &virtualPart = m_virtualParticles [virtualIndex];
   if (! virtualPart.usesInputParticles)
   {
      cerr << "KinematicFitter::updateVirtualParticle Fatal Error:"
           << endl << "You can not update a Super virtual particle. "
           << "Aborting." << endl;
      assert(0);
   }
   IVec virtpart;
   int inSize = (int) m_inputParticles.size();
   for (IVecIter iter = vec.begin();
        vec.end() != iter;
        ++iter)
   {
      int value = *iter;
      if (value >= 0 && value < inSize)
      {
         virtpart.push_back (value);
      } // if value is o.k.
   } // for iter
   if (! virtpart.size())
   {
      // nothing good to add
      cerr << "KinematicFitter::updateVirtualParticle() Warning: "
           << "No particles added."
           << endl;
      return; // failure
   }
   virtualPart.particleList = virtpart;
}

void
KinematicFitter::updateVirtualParticle (int virtualPart,
                                        int in1, int in2, int in3, 
                                        int in4, int in5, int in6)
{
   IVec ivec;
   ivec.push_back (in1);
   ivec.push_back (in2);
   ivec.push_back (in3);
   ivec.push_back (in4);
   ivec.push_back (in5);
   ivec.push_back (in6);
   return updateVirtualParticle (virtualPart, ivec);
}

//////////////////////
// Mass Constraints //
//////////////////////

int
KinematicFitter::addSingleMassConstraint (int newIndex, double mass)
{
   _validateVirtualPartIndex (__FILE__, __LINE__, kOriginal, newIndex);
   int retval = (int) m_constraintVec.size();
   ConstraintStruct constraint;
   constraint.index1        = newIndex;
   constraint.type          = kSingleMassConstraint;
   constraint.def_massConst = constraint.massConst = mass;
   constraint.def_isActive  = constraint.isActive  = true;
   m_constraintVec.push_back (constraint);
   return retval;
}

int
KinematicFitter::addEqualMassConstraint (int newIndex1, int newIndex2)
{ 
   _validateVirtualPartIndex (__FILE__, __LINE__, kOriginal, newIndex1);
   _validateVirtualPartIndex (__FILE__, __LINE__, kOriginal, newIndex2);
   int retval = (int) m_constraintVec.size();
   ConstraintStruct constraint;
   constraint.index1        = newIndex1;
   constraint.index2        = newIndex2;
   constraint.type          = kEqualMassConstraint;
   constraint.def_isActive  = constraint.isActive  = true;
   m_constraintVec.push_back (constraint);
   return retval;
}

void
KinematicFitter::updateSingleMassConstraint (int constraintIndex,
                                                 double mass)
{
   _validateConstraintIndex (__FILE__, __LINE__, constraintIndex);
   // now make sure this is a single mass constraint
   ConstraintStruct &constraint = m_constraintVec[constraintIndex];
   if (kSingleMassConstraint != constraint.type)
   {
      cerr << "KinematicFitter updateSingleMassConstraint()Fatal Error:"
           << "This constraint is not a single mass constraint. Aborting." 
           << endl;
      abort();
   }
   // If we're still here, then we haven't done anything stupid
   constraint.massConst = mass;
}

double
KinematicFitter::currentSingleMassConstraint (int constraintIndex)
{
   _validateConstraintIndex (__FILE__, __LINE__, constraintIndex);
   // now make sure this is a single mass constraint
   ConstraintStruct &constraint = m_constraintVec[constraintIndex];
   if (kSingleMassConstraint != constraint.type)
   {
      cerr << "KinematicFitter singleSingleMassConstraint()Fatal Error:"
           << "This constraint is not a single mass constraint. Aborting." 
           << endl;
      abort();
   }
   // If we're still here, then we haven't done anything stupid
   return constraint.massConst;
}

void 
KinematicFitter::setConstraintActive (int constraintIndex)
{
   _validateConstraintIndex (__FILE__, __LINE__, constraintIndex);
   ConstraintStruct &constraint = m_constraintVec[constraintIndex];
   constraint.isActive = true;
}

void 
KinematicFitter::setConstraintInactive (int constraintIndex)
{
   _validateConstraintIndex (__FILE__, __LINE__, constraintIndex);
   ConstraintStruct &constraint = m_constraintVec[constraintIndex];
   constraint.isActive = false;
}

bool 
KinematicFitter::isConstraintActive (int constraintIndex)
{
   _validateConstraintIndex (__FILE__, __LINE__, constraintIndex);
   ConstraintStruct &constraint = m_constraintVec[constraintIndex];
   return constraint.isActive;
}

void 
KinematicFitter::saveCurrentConstraintsAsDefault()
{
   for (ConstraintVecIter iter = m_constraintVec.begin();
        m_constraintVec.end() != iter;
        ++iter)
   {
      iter->def_massConst = iter->massConst;
      iter->def_isActive  = iter->isActive;
   } // for iter
}


void 
KinematicFitter::resetConstraints()
{
   for (ConstraintVecIter iter = m_constraintVec.begin();
        m_constraintVec.end() != iter;
        ++iter)
   {
      iter->massConst = iter->def_massConst;
      iter->isActive  = iter->def_isActive;
   } // for iter
}

int 
KinematicFitter::numConstraints() 
{ 
   int retval = 0;
   for (ConstraintVecIter iter = m_constraintVec.begin();
        m_constraintVec.end() != iter;
        ++iter)
   {
      if (iter->isActive)
      {
         ++retval;
      } // is active
   } // for iter
   return retval;
}

int 
KinematicFitter::numTotalConstraints() 
{ 
   return (int) m_constraintVec.size(); 
}

///////////////////////
// Particle Momentum //
///////////////////////

TLorentzVector
KinematicFitter::particleMom (int index, InOutType type)
{
   _validateInputIndex (__FILE__, __LINE__, type, index);
   InputParticleVec *vPtr = &m_inputParticles;
   if (kModified == type)
   {
      vPtr = &m_outputParticles;
   } // if output
   else if (kIntermediate == type)
   {
      vPtr = &m_middleParticles;
   }
   TLorentzVector retval;
   InputParticle &input = (*vPtr)[index];
   retval.SetPx (input.Px);
   retval.SetPy (input.Py);
   retval.SetPz (input.Pz);
   if (kE_P_basis == m_basis) 
   {
      // In E_P basis
      retval.SetE (input.E);
   } else {
      // In M_P basis
      double energy = 0; // avoid compiler warnings
      if (kNormalParticle == input.type)
      {
         // regular particles
         energy = sqrt ( _sq (input.mass) +
                         _sq (input.Px)   + 
                         _sq (input.Py)   + 
                         _sq (input.Pz)    );
      } else if (kMassiveJet == input.type) 
      {
         // Massive Jets
         double scalarMom = sqrt ( _sq (input.Px) + 
                                   _sq (input.Py) + 
                                   _sq (input.Pz)  );
         energy = input.percentage * scalarMom;
      } else {
         assert (0);
      }
      retval.SetE (energy);
   }
   return retval;
}

double
KinematicFitter::E (int index, InOutType type)
{
   _validateInputIndex (__FILE__, __LINE__, type, index);
   InputParticleVec *vPtr = &m_inputParticles;
   if (kModified == type)
   {
      vPtr = &m_outputParticles;
   } // if output
   else if (kIntermediate == type)
   {
      vPtr = &m_middleParticles;
   }
   // In E_P basis
   if (kE_P_basis == m_basis) 
   {
      return (*vPtr)[index].E;
   }
   // In M_P basis
   InputParticle &input = (*vPtr)[index];
   double energy = 0; // avoid compiler warnings
   if (kNormalParticle == input.type)
   {
      // regular particles
      energy = sqrt ( _sq (input.mass) +
                      _sq (input.Px)   + 
                      _sq (input.Py)   + 
                      _sq (input.Pz)    );
   } else if (kMassiveJet == input.type) 
   {
      double scalarMom = sqrt ( _sq (input.Px) + 
                                _sq (input.Py) + 
                                _sq (input.Pz)  );
      energy = input.percentage * scalarMom;
   } else {
      assert (0);
   }
   return energy;
}

double
KinematicFitter::Px (int index, InOutType type)
{
   _validateInputIndex (__FILE__, __LINE__, type, index);
   InputParticleVec *vPtr = &m_inputParticles;
   if (kModified == type)
   {
      vPtr = &m_outputParticles;
   } // if output
   else if (kIntermediate == type)
   {
      vPtr = &m_middleParticles;
   }
   return (*vPtr)[index].Px;
}

double
KinematicFitter::Py (int index, InOutType type)
{
   _validateInputIndex (__FILE__, __LINE__, type, index);
   InputParticleVec *vPtr = &m_inputParticles;
   if (kModified == type)
   {
      vPtr = &m_outputParticles;
   } // if output
   else if (kIntermediate == type)
   {
      vPtr = &m_middleParticles;
   }
   return (*vPtr)[index].Py;
}

double
KinematicFitter::Pz (int index, InOutType type)
{
   _validateInputIndex (__FILE__, __LINE__, type, index);
   InputParticleVec *vPtr = &m_inputParticles;
   if (kModified == type)
   {
      vPtr = &m_outputParticles;
   } // if output
   else if (kIntermediate == type)
   {
      vPtr = &m_middleParticles;
   }
   return (*vPtr)[index].Pz;
}

TLorentzVector
KinematicFitter::virtualParticleMom (int index, InOutType type)
{
   _validateVirtualPartIndex (__FILE__, __LINE__, type, index, true);
   IVec vec;
   _allInputParticles (index, vec);
   TLorentzVector retval(0., 0., 0., 0.);
   for (unsigned int loop = 0; loop < vec.size(); ++loop)
   {
      retval += particleMom (vec[loop], type);
   }
   return retval;
}

double
KinematicFitter::virtualE (int index, InOutType type)
{
   return virtualParticleMom (index, type).E();
}

double
KinematicFitter::virtualPx (int index, InOutType type)
{
   return virtualParticleMom (index, type).Px();
}

double
KinematicFitter::virtualPy (int index, InOutType type)
{
   return virtualParticleMom (index, type).Py();
}

double
KinematicFitter::virtualPz (int index, InOutType type)
{
   return virtualParticleMom (index, type).Pz();
}

void
KinematicFitter::fillAllInputMomentumVector 
                                   (KinematicFitter::FourVectorVec &retval,
                                    InOutType type)
{
   retval.clear();
   for (int loop = 0; loop < numInputParticles(); ++loop)
   {
      retval.push_back( particleMom (loop, type) );
   }
}

void
KinematicFitter::fillAllVirtualMomentumVector 
                                   (KinematicFitter::FourVectorVec &retval,
                                    InOutType type)
{
   retval.clear();
   for (int loop = 0; loop < numVirtualParticles(); ++loop)
   {
      retval.push_back( virtualParticleMom (loop, type) );
   }
}

/////////////////////////////
// /////////////////////// //
// // The Big Enchilada // //
// /////////////////////// //
/////////////////////////////

double
KinematicFitter::minimizeChi2()
{
   int numConstr = numConstraints();
   // get the number of used parameters and setup the needed set and map.
   int numUsedPart = _numUsedInputParticles();
   int numParam = numUsedPart * kNumParametersPerParticle;
   double chi2 = -1.;
   double oldchi2 = -1.;
   // reset m_V_a0 if we are not in keep covariance mode
   if (kKeepCovarMode != m_covarMode)
   {
      if (m_V_a0)
      {
         delete m_V_a0;
         m_V_a0 = 0;
      }
   } // if keep covar mode
   for (int iteration = 0; iteration < m_numIterations; ++iteration)
   {
      // for the first iteration, copy the input particles into the
      // middle particles.  Otherwise, copy the output particles into the
      // middle particles.
      if (iteration)
      {
         // not first iteration
         m_middleParticles = m_outputParticles;
      } else {
         // first iteration
         m_middleParticles = m_inputParticles;
      }

      ///////////////////////
      // Covariance Matrix //
      ///////////////////////
      // We only need to create the covariance matrix once.
      if (! m_V_a0)
      {
         m_V_a0 = new TMatrixD (numParam, numParam);
         _zeroMatrix (*m_V_a0);
         int count = 0;
         // Loop over the input particles
         for (ISetIter iter = m_usedInputPartSet.begin();
              m_usedInputPartSet.end() != iter;
              ++iter)
         {
            int offset = (count++) * kNumParametersPerParticle;
            int inpIndex = *iter;
            _insertSmallMatrixIntoLarge (m_inputErrorMatricies[inpIndex],
                                         *m_V_a0, offset, offset);
         } // for iter
      } // if no covariance matrix

      ////////////////////////////////
      // ////////////////////////// //
      // // Constraint Matricies // //
      // ////////////////////////// //
      ////////////////////////////////

      // Before doing anything else, lets get rid of the old matricies if
      // there is anything to get rid of.
      TMatrixD *D = 0;
      TMatrixD *dvec = 0;
      D = new TMatrixD (numConstr, numParam);
      _zeroMatrix (*D);
      dvec = new TMatrixD (numConstr, 1); 
      // We want these cleaned up
      sm_matrixPtrVec.push_back (D);
      sm_matrixPtrVec.push_back (dvec);
      
      int constraintIndex = 0;
      for (ConstraintVecIter iter = m_constraintVec.begin();
           m_constraintVec.end() != iter;
           ++iter)
      {
         // Don't do inactive constraints
         if (! iter->isActive )
         {
            // not this time
            continue;
         }
         if (kSingleMassConstraint == iter->type)
         {
            _setupSingleConstraint (D, dvec, constraintIndex, *iter);
         } else if (kEqualMassConstraint == iter->type)
         {
            _setupEqualConstraint (D, dvec, constraintIndex, *iter);
         } else {
            assert(0);
         }
      } // for iter

      ///////////////////////
      // Chi^2 Calculation //
      ///////////////////////
   
      TMatrixD *DT = _transpose (*D);

      // V_D = D * m_V_a0 * DT
      TMatrixD *V_D = _mult (D, m_V_a0, DT);
      V_D->Invert();

      TMatrixD *delAlpha = _deltaAlpha(numUsedPart);
      // lambda =               V_D *    (    (  D * delAlpha ) + dvec )
      TMatrixD *lambda = _mult( V_D, _add( _mult( D, delAlpha ),  dvec ) );

      // calculate the chi^2 = lambdaT * ( ( D * delAlpha ) + dvec )
      TMatrixD *chi2M = _mult (_transpose (lambda),
                               _add( _mult ( D, delAlpha ), dvec) );
      oldchi2 = chi2;
      chi2 = (*chi2M) (0, 0);

      /////////////////////////////
      // Create Output Particles //
      /////////////////////////////
      // alpha = a0 - m_V_a0 * DT * lambda = a0 - shift
      TMatrixD *shift = _mult (m_V_a0, DT, lambda);

      // Copies all particles over (even those not modified)
      m_outputParticles = m_middleParticles;
      for (ISetIter iter = m_usedInputPartSet.begin();
           m_usedInputPartSet.end() != iter;
           ++iter)
      {
         int partIndex = *iter;
         int offset = _calcOffset (partIndex);
         InputParticle &outvec = m_outputParticles[partIndex];
         InputParticle &invec  = m_inputParticles[partIndex];
         if (kE_P_basis == m_basis)
         {
            outvec.E =  invec.E - (*shift) (kEnergy + offset, 0);
         }
         outvec.Px = invec.Px - (*shift) (kPx + offset, 0);
         outvec.Py = invec.Py - (*shift) (kPy + offset, 0);
         outvec.Pz = invec.Pz - (*shift) (kPz + offset, 0);
      } // for iter

      /////////////////////
      // Keep Iterating? //
      /////////////////////
      if (fabs (chi2 - oldchi2) < m_chi2delta ||
          chi2 > m_maxChi2)
      {
         // we're either close enough or way out in the boonies
         break;
      }

   } // for iteration
   ///////////////////////////////////
   // Delete used TMatrixD pointers //
   ///////////////////////////////////

   // get rid of the covariance matrix, _unless_ we are supposed to
   // keep it.
   if (kKeepCovarMode != m_covarMode)
   {
      delete m_V_a0;
      m_V_a0 = 0;
   }
   _cleanupMatrixPtrs();

   // return our chi2
   return chi2;
}

//////////////////////////////
// Private Member Functions //
//////////////////////////////

bool
KinematicFitter::_validateInputIndex (string filename,
                                      int linenum,
                                      KinematicFitter::InOutType type,
                                      int index)
{
   // assuming input type
   InputParticleVec *vecPtr = &m_inputParticles;
   if (kIntermediate == type)
   {
      // intermediate
      vecPtr = &m_middleParticles;
   } else if (kModified == type)
   {
      // output
      vecPtr = &m_outputParticles;
   }
   if (index < 0 || index >= (int) vecPtr->size())
   {
      cerr << "KinematicFitter Fatal Error: Invalid index of "
           << "input particle called from " << endl
           << filename << "(" << linenum << "). Aborting." << endl;
      abort();
   }
   return true;
}

bool
KinematicFitter::_validateVirtualPartIndex (string filename,
                                            int linenum,
                                            KinematicFitter::InOutType type,
                                            int index,
                                            bool checkInputParticles)
{
   if (checkInputParticles)
   {
      // assuming input type
      InputParticleVec *vecPtr = &m_inputParticles;
      if (kIntermediate == type)
      {
         // intermediate
         vecPtr = &m_middleParticles;
      } else if (kModified == type)
      {
         // output
         vecPtr = &m_outputParticles;
      }
      // make sure the input particles have some entries
      if (! vecPtr->size())
      {
         cerr << "KinematicFitter Fatal Error: Virtual particle list "
              << "is empty. Called from " << endl
              << filename << "(" << linenum << "). Aborting. (zero)" << endl;
         abort();
      }
   }
   // now make sure we have the right number
   if (index < 0 || index >= (int) m_virtualParticles.size())
   {
      cerr << "KinematicFitter Fatal Error: Invalid index of "
           << "virtual particle called from " << endl
           << filename << "(" << linenum << "). Aborting." << endl;
      abort();
   }
   return true;
}

bool
KinematicFitter::_validateConstraintIndex (std::string  filename,
                                               int linenum,
                                               int index)
{
   if (index < 0 || index >= (int) m_constraintVec.size())
   {
      cerr << "KinematicFitter Fatal Error: Invalid index of "
           <<" constraint index." << endl << " Called from " << endl
           << filename << "(" << linenum << "). Aborting." << endl;
      abort();
   }
   return true;
}

void
KinematicFitter::_setupSingleConstraint 
                      (TMatrixD* D, TMatrixD* dvec, 
                       int &constraintIndex,
                       const KinematicFitter::ConstraintStruct &constraint)
{
   // which virtual (new) particle is this?
   int virtpart = constraint.index1;         
   // inpPart - an STL vector of the input particle indicies that make
   // up the 'virtpart' virtual particle
   IVec inpPart;
   _allInputParticles (virtpart, inpPart);
      
   // get me the current state of the four vector of this virtual particle
   TLorentzVector virtpartvec = 
      virtualParticleMom (virtpart, kIntermediate);

   // The D matrix
   if (kE_P_basis == m_basis)
   {
      // E P Basis

      for (IVecIter inpIter = inpPart.begin();
           inpPart.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         (*D) (constraintIndex, offset + kEnergy) =  2 * virtpartvec.E(); 
         (*D) (constraintIndex, offset + kPx)     = -2 * virtpartvec.Px();
         (*D) (constraintIndex, offset + kPy)     = -2 * virtpartvec.Py();
         (*D) (constraintIndex, offset + kPz)     = -2 * virtpartvec.Pz();
      } // for inpIter
   } else {
      // M P Basis

      for (IVecIter inpIter = inpPart.begin();
           inpPart.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         double E_virt   = virtpartvec.E();
         double Px_virt  = virtpartvec.Px();
         double Py_virt  = virtpartvec.Py();
         double Pz_virt  = virtpartvec.Pz();
         double Ei  = E (*inpIter, kIntermediate);
         double Pxi = Px(*inpIter, kIntermediate);
         double Pyi = Py(*inpIter, kIntermediate);
         double Pzi = Pz(*inpIter, kIntermediate);
         (*D) (constraintIndex, offset + kMass) =  0.;
         (*D) (constraintIndex, offset + kPx)   =
            2 * (E_virt - Ei) * Pxi / Ei - 2 * (Px_virt - Pxi);
         (*D) (constraintIndex, offset + kPy)   =
            2 * (E_virt - Ei) * Pyi / Ei - 2 * (Py_virt - Pyi);
         (*D) (constraintIndex, offset + kPz)   =
            2 * (E_virt - Ei) * Pzi / Ei - 2 * (Pz_virt - Pzi);
      } // for inpIter
   }

   // little d vector
   (*dvec) (constraintIndex, 0) 
      = virtpartvec.M2() - _sq( constraint.massConst );

   // Don't forget to increase the constraintIndexs index
   ++constraintIndex;
}

void
KinematicFitter::_setupEqualConstraint 
                      (TMatrixD* D, TMatrixD* dvec, 
                       int &constraintIndex,
                       const KinematicFitter::ConstraintStruct &constraint)
{
   // which new particle is this?
   int virtpart1 = constraint.index1;
   int virtpart2 = constraint.index2;
   // inpPart1,2 - STL vectors of the input particle indicies that make
   // up the 'virtpart1,2' virtual particles
   IVec inpPart1;
   IVec inpPart2;
   _allInputParticles (virtpart1, inpPart1);
   _allInputParticles (virtpart2, inpPart2);
         
   // Make the new particle four vectors
   TLorentzVector virtpart1vec 
      = virtualParticleMom (virtpart1, kIntermediate);
   TLorentzVector virtpart2vec 
      = virtualParticleMom (virtpart2, kIntermediate);

   // The D matrix
   if (kE_P_basis == m_basis)
   {
      // E P Basis

      // Positive particle
      for (IVecIter inpIter = inpPart1.begin();
           inpPart1.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         (*D) (constraintIndex, offset + kEnergy) =  2 * virtpart1vec.E(); 
         (*D) (constraintIndex, offset + kPx)     = -2 * virtpart1vec.Px();
         (*D) (constraintIndex, offset + kPy)     = -2 * virtpart1vec.Py();
         (*D) (constraintIndex, offset + kPz)     = -2 * virtpart1vec.Pz();
      } // for inpIter
      
      // Now set the D matrix - The negative particle (swap the sign
      // of '2' below '2 <=> -2')
      for (IVecIter inpIter = inpPart2.begin();
           inpPart2.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         (*D) (constraintIndex, offset + kEnergy) = -2 * virtpart2vec.E(); 
         (*D) (constraintIndex, offset + kPx)     =  2 * virtpart2vec.Px();
         (*D) (constraintIndex, offset + kPy)     =  2 * virtpart2vec.Py();
         (*D) (constraintIndex, offset + kPz)     =  2 * virtpart2vec.Pz();
      } // for inpIter
   } else {
      // M P Basis

      // Positive particle
      for (IVecIter inpIter = inpPart1.begin();
           inpPart1.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         double E_virt   = virtpart1vec.E();
         double Px_virt  = virtpart1vec.Px();
         double Py_virt  = virtpart1vec.Py();
         double Pz_virt  = virtpart1vec.Pz();
         double Ei  = E (*inpIter, kIntermediate);
         double Pxi = Px(*inpIter, kIntermediate);
         double Pyi = Py(*inpIter, kIntermediate);
         double Pzi = Pz(*inpIter, kIntermediate);
         (*D) (constraintIndex, offset + kMass) =  0.;
         (*D) (constraintIndex, offset + kPx)   =
            2 * (E_virt - Ei) * Pxi / Ei - 2 * (Px_virt - Pxi);
         (*D) (constraintIndex, offset + kPy)   =
            2 * (E_virt - Ei) * Pyi / Ei - 2 * (Py_virt - Pyi);
         (*D) (constraintIndex, offset + kPz)   =
            2 * (E_virt - Ei) * Pzi / Ei - 2 * (Pz_virt - Pzi);
      } // for inpIter

      // Now set the D matrix - The negative particle (swap the sign
      // of '2' below '2 <=> -2')
      for (IVecIter inpIter = inpPart2.begin();
           inpPart2.end() != inpIter;
           ++inpIter)
      {
         int offset = _calcOffset (*inpIter);
         double E_virt   = virtpart2vec.E();
         double Px_virt  = virtpart2vec.Px();
         double Py_virt  = virtpart2vec.Py();
         double Pz_virt  = virtpart2vec.Pz();
         double Ei  = E (*inpIter, kIntermediate);
         double Pxi = Px(*inpIter, kIntermediate);
         double Pyi = Py(*inpIter, kIntermediate);
         double Pzi = Pz(*inpIter, kIntermediate);
         (*D) (constraintIndex, offset + kMass) =  0.;
         (*D) (constraintIndex, offset + kPx)   =
            -2 * (E_virt - Ei) * Pxi / Ei + 2 * (Px_virt - Pxi);
         (*D) (constraintIndex, offset + kPy)   =
            -2 * (E_virt - Ei) * Pyi / Ei + 2 * (Py_virt - Pyi);
         (*D) (constraintIndex, offset + kPz)   =
            -2 * (E_virt - Ei) * Pzi / Ei + 2 * (Pz_virt - Pzi);
      } // for inpIter

   }

   // little d vector
   (*dvec) (constraintIndex, 0) = virtpart1vec.M2() - virtpart2vec.M2();

   // Don't forget to increase the constraintIndexs index
   ++constraintIndex;
}

TMatrixD*
KinematicFitter::_deltaAlpha (int numUsedParticles, bool useDeleteList)
{
   assert (m_inputParticles.size() == m_middleParticles.size());
   TMatrixD *deltaPtr 
      = new TMatrixD (numUsedParticles * kNumParametersPerParticle, 1);
   for (ISetIter iter = m_usedInputPartSet.begin();
        m_usedInputPartSet.end() != iter;
        ++iter)
   {
      int partIndex = *iter;
      TLorentzVector orig = particleMom (partIndex, kOriginal);
      TLorentzVector mid  = particleMom (partIndex, kIntermediate);
      int offset = _calcOffset (partIndex);
      if (kE_P_basis == m_basis)
      {
         // E P Basis
         (*deltaPtr) (offset + kEnergy, 0) = orig.E()  - mid.E();
      } else {
         // M P Basis
         (*deltaPtr) (offset + kMass, 0)   = 0.;
      }
      (*deltaPtr) (offset + kPx, 0)     = orig.Px() - mid.Px();
      (*deltaPtr) (offset + kPy, 0)     = orig.Py() - mid.Py();
      (*deltaPtr) (offset + kPz, 0)     = orig.Pz() - mid.Pz();
   } // for iter
   if (useDeleteList)
   {
      sm_matrixPtrVec.push_back (deltaPtr);
   }
   return deltaPtr;
}

void
KinematicFitter::_allInputParticles (int index, IVec &retval, bool clear)
{
   _validateVirtualPartIndex (__FILE__, __LINE__, kOriginal, index, true);
   if (clear)
   {
      retval.clear();
   }
   // Is this a regular virtual particle or a super virtual particle
   if (m_virtualParticles[index].usesInputParticles)
   {
      // Regular virtual particles
      for (IVecIter iter = m_virtualParticles[index].particleList.begin();
           m_virtualParticles[index].particleList.end() != iter;
           ++iter)
      {
         retval.push_back( *iter );
      } // for iter
      return;
   }
   // If we're still here, we must be dealing with a super particle.

   // NOTE: This function does not check that you haven't done
   // something stupid. If your super particle A calls super particle
   // B which in turn calls super particle A, then you are in
   // *TROUBLE*.
   IVec &myVec = m_virtualParticles[index].particleList;
   for (IVecIter outerIter = myVec.begin();
        myVec.end() != outerIter;
        ++outerIter)
   {
      IVec tempList;
      _allInputParticles (*outerIter, tempList);
      for (IVecIter innerIter = tempList.begin();
           tempList.end() != innerIter;
           ++innerIter)
      {
         retval.push_back( *innerIter );
      } // innerIter      
   } // for outerIter
   // all done.
   return;
}

int
KinematicFitter::_calcOffset (int partIndex)
{
   if (kNormalCovarMode == m_covarMode)
   {
      return partIndex * kNumParametersPerParticle;
   }
   IIMapIter iter = m_usedInputPartMap.find (partIndex);
   if (m_usedInputPartMap.end() == iter)
   {
      cout << "partIndex " << partIndex << endl;
      for (iter = m_usedInputPartMap.begin();
           m_usedInputPartMap.end() != iter;
           ++iter)
      {
         cout << iter->first << " " << iter->second << endl;
      }
      dout << "Fatal Error" << endl;
      exit(1);
   }
   return iter->second * kNumParametersPerParticle;
}

int
KinematicFitter::_numUsedInputParticles()
{
   if (kFastCovarMode != m_covarMode)
   {
      // normal covariance mode
      int retval = numInputParticles();
      for (int loop = 0; loop < retval; ++loop)
      {
         m_usedInputPartSet.insert (loop);
      }
      return retval;
   }
   // kFastCovarMode
   IVec vecWithDuplicates;
   for (ConstraintVecIter iter = m_constraintVec.begin();
        m_constraintVec.end() != iter;
        ++iter)
   {
      // Don't do inactive constraints
      if (! iter->isActive )
      {
         // We don't care about inactive constraints
         continue;
      }
      if (kSingleMassConstraint == iter->type)
      {
         // get the list of all particles used in this constraint
         // WITHOUT clearing 'vecWithDupliates'.
         _allInputParticles (iter->index1, vecWithDuplicates, false);
      } else if (kEqualMassConstraint == iter->type)
      {
         // get the list of all particles used in this constraint
         // WITHOUT clearing 'vecWithDupliates'.
         _allInputParticles (iter->index1, vecWithDuplicates, false);
         _allInputParticles (iter->index2, vecWithDuplicates, false);
      } else {
         assert(0);
      }
   } // for iter

   // We now have a vector that may have many duplicates.  Let's make
   // an ordered set with no duplicates (i.e., std::set<>).
   m_usedInputPartSet.clear();
   for (IVecIter iter = vecWithDuplicates.begin();
        vecWithDuplicates.end() != iter;
        ++iter)
   {
      m_usedInputPartSet.insert( *iter );
   } // for iter

   // Since we're not going in order anymore, we need to create a map
   // to tell us how to jump around.
   m_usedInputPartMap.clear();
   int count = 0;
   for (ISetIter iter = m_usedInputPartSet.begin();
        m_usedInputPartSet.end() != iter;
        ++iter)
   {
      m_usedInputPartMap[*iter] = count++;
   }
   return count;
}

/////////////////////////////
// Static Member Functions //
/////////////////////////////

void
KinematicFitter::_zeroMatrix (TMatrixD &matrix)
{
   int nRows = matrix.GetNrows();
   int nCols = matrix.GetNcols();
   for (int row = 0; row < nRows; ++row)
   {
      for (int col = 0; col < nCols; ++col)
      {
         matrix (row, col) = 0.;
      } // for col
   } // for row
}

void
KinematicFitter::_covarianceMatrixFromDeltaETP (TMatrixD &matrix,
                                                const TLorentzVector &vec,
                                                double dE,
                                                double dTheta,
                                                double dPhi,
                                                Basis basis)
{
   assert (4 == matrix.GetNrows() &&
           4 == matrix.GetNcols() );
   // define all of the sines and cosines we'll need
   double cosphi    =  vec.Px() / vec.Pt();
   double sinphi    =  vec.Py() / vec.Pt();
   double costta    =  vec.CosTheta();
   double sintta    =  sqrt (1 - _sq(costta));
   double cos2phi   = _sq(cosphi);
   double sin2phi   = _sq(sinphi);
   double cos2tta   = _sq(costta);
   double sin2tta   = _sq(sintta);
   double dE2       = _sq (dE);
   double E         =  vec.E();
   double dTheta2E2 = _sq (dTheta * E);
   double dPhi2E2   = _sq (dPhi   * E);
   //////////////////////////////
   // The dEnergy pieces first //
   //////////////////////////////
   // First, the on-diagonal pieces
   matrix (kEnergy, kEnergy) = dE2;
   matrix (kPx,     kPx)     = dE2 * sin2tta * cos2phi;
   matrix (kPy,     kPy)     = dE2 * sin2tta * sin2phi;
   matrix (kPz,     kPz)     = dE2 * cos2tta;
   // Next, the off-diagonal energy pieces
   matrix (kPx,     kEnergy) = dE2 * sintta  * cosphi;
   matrix (kPy,     kEnergy) = dE2 * sintta  * sinphi;
   matrix (kPz,     kEnergy) = dE2 * costta;
   // And finally, the off-diagonal Px, Py, Pz
   matrix (kPy,     kPx)     = dE2 * sin2tta * sinphi * cosphi;
   matrix (kPz,     kPx)     = dE2 * sintta  * costta * cosphi;
   matrix (kPz,     kPy)     = dE2 * sintta  * costta * sinphi;
   
   if (kM_P_basis == basis)
   {
      // Zero out the mass/energy uncertaities
      // Remember kMass = kEnergy
      matrix (kMass, kMass) = 0.;
      matrix (kPx,   kMass) = 0.;
      matrix (kPy,   kMass) = 0.;
      matrix (kPz,   kMass) = 0.;
   }

   ////////////////////////////
   // The dTheta pieces next //
   ////////////////////////////
   // First, the on-diagonal pieces
   matrix (kPx, kPx) +=      dTheta2E2 * cos2tta * cos2phi;
   matrix (kPy, kPy) +=      dTheta2E2 * cos2tta * sin2phi;
   matrix (kPz, kPz) +=      dTheta2E2 * sin2tta;
   // And finally, the off-diagonal Px, Py, Pz
   matrix (kPy, kPx) +=      dTheta2E2 * cos2tta * sinphi * cosphi;
   matrix (kPz, kPx) += -1 * dTheta2E2 * sintta  * costta * cosphi;
   matrix (kPz, kPy) += -1 * dTheta2E2 * sintta  * costta * sinphi;

   /////////////////////
   // The dPhi Pieces //
   /////////////////////
   matrix (kPx, kPx) +=      dPhi2E2 * sin2tta * sin2phi;
   matrix (kPy, kPy) +=      dPhi2E2 * sin2tta * cos2tta;
   matrix (kPy, kPx) += -1 * dPhi2E2 * sin2tta * sinphi * cosphi;

   ///////////////////////////////
   // Make the matrix symmetric //
   ///////////////////////////////
   matrix (kEnergy, kPx) = matrix (kPx, kEnergy);
   matrix (kEnergy, kPy) = matrix (kPy, kEnergy);
   matrix (kEnergy, kPz) = matrix (kPz, kEnergy);
   matrix (kPx,     kPy) = matrix (kPy, kPx);
   matrix (kPx,     kPz) = matrix (kPz, kPx);
   matrix (kPy,     kPz) = matrix (kPz, kPy);   
}

void
KinematicFitter::_covarianceMatrixALaMarcel (TMatrixD &matrix, 
                                             const TLorentzVector &vec,
                                             double percent)
{
   assert (4 == matrix.GetNrows() &&
           4 == matrix.GetNcols() );
   // zero everything first
   _zeroMatrix (matrix);
   // The on-diagonal pieces
   matrix (kEnergy, kEnergy) = _sq (percent * vec.E());
   matrix (kPx,     kPx)     = _sq (percent * vec.Px());
   matrix (kPy,     kPy)     = _sq (percent * vec.Py());
   matrix (kPz,     kPz)     = _sq (percent * vec.Pz());
}

void 
KinematicFitter::_insertSmallMatrixIntoLarge (const TMatrixD &small,
                                              TMatrixD &large,
                                              int rowOff, int colOff)
{
   int smallRow = small.GetNrows();
   int smallCol = small.GetNcols();
   // Make sure that there is room
   assert ( (smallRow + rowOff <= large.GetNrows() ) &&
            (smallCol + colOff <= large.GetNcols() ) );
   for (int row = 0; row < smallRow; ++row)
   {
      for (int col = 0; col < smallCol; ++col)
      {
         large (row + rowOff, col + colOff) = small (row, col);
      } // for col
   } // for row
}

TMatrixD*
KinematicFitter::_mult (const TMatrixD &alpha, 
                        const TMatrixD &beta,
                        bool useDeleteList)
{
   int alphaRow = alpha.GetNrows();
   int alphaCol = alpha.GetNcols();
   int betaRow  = beta.GetNrows();
   int betaCol  = beta.GetNcols();
   assert (alphaCol == betaRow);
   TMatrixD *retvalPtr = new TMatrixD (alphaRow, betaCol);
   retvalPtr->Mult (alpha, beta);
   if (useDeleteList)
   {
      sm_matrixPtrVec.push_back (retvalPtr);
   }
   return retvalPtr;
}

TMatrixD*
KinematicFitter::_mult (TMatrixD *alphaPtr, 
                        TMatrixD *betaPtr,
                        bool useDeleteList)
{
   return _mult (*alphaPtr, *betaPtr, useDeleteList);
}

TMatrixD*
KinematicFitter::_mult (const TMatrixD &alpha, 
                        const TMatrixD &beta,
                        const TMatrixD &gamma,
                        bool useDeleteList)
{
   //int alphaRow = alpha.GetNrows();
   int alphaCol = alpha.GetNcols();
   int betaRow  = beta.GetNrows();
   int betaCol  = beta.GetNcols();
   int gammaRow = gamma.GetNrows();
   //int gammaCol = gamma.GetNcols();
   assert (alphaCol == betaRow);
   assert (betaCol  == gammaRow);
   // I'll delete temp myself, so tell _mult() not to keep track of
   // it.
   TMatrixD *temp = _mult (alpha, beta, false);
   TMatrixD *retvalPtr = _mult (*temp, gamma, useDeleteList);
   delete temp;
   return retvalPtr;
}

TMatrixD*
KinematicFitter::_mult (TMatrixD *alphaPtr, 
                        TMatrixD *betaPtr,
                        TMatrixD *gammaPtr,
                        bool useDeleteList)
{
   return _mult (*alphaPtr, *betaPtr, *gammaPtr, useDeleteList);
}

TMatrixD*
KinematicFitter::_add (const TMatrixD &alpha, 
                       const TMatrixD &beta,
                       bool useDeleteList)
{
   int alphaRow = alpha.GetNrows();
   int alphaCol = alpha.GetNcols();
   int betaRow  = beta.GetNrows();
   int betaCol  = beta.GetNcols();
   assert (alphaCol == betaCol && alphaRow == betaRow);
   TMatrixD *retvalPtr = new TMatrixD (alphaRow, alphaCol);
   *retvalPtr = alpha;
   *retvalPtr += beta;
   if (useDeleteList)
   {
      sm_matrixPtrVec.push_back (retvalPtr);
   }
   return retvalPtr;
}

TMatrixD*
KinematicFitter::_add (TMatrixD *alphaPtr, 
                       TMatrixD *betaPtr,
                       bool useDeleteList)
{
   return _add (*alphaPtr, *betaPtr, useDeleteList);
}

TMatrixD*
KinematicFitter::_subtract (const TMatrixD &alpha, 
                            const TMatrixD &beta,
                            bool useDeleteList)
{
   int alphaRow = alpha.GetNrows();
   int alphaCol = alpha.GetNcols();
   int betaRow  = beta.GetNrows();
   int betaCol  = beta.GetNcols();
   assert (alphaCol == betaCol && alphaRow == betaRow);
   TMatrixD *retvalPtr = new TMatrixD (alphaRow, alphaCol);
   *retvalPtr = alpha;
   *retvalPtr -= beta;
   if (useDeleteList)
   {
      sm_matrixPtrVec.push_back (retvalPtr);
   }
   return retvalPtr;
}

TMatrixD*
KinematicFitter::_subtract (TMatrixD *alphaPtr, 
                            TMatrixD *betaPtr,
                            bool useDeleteList)
{
   return _subtract (*alphaPtr, *betaPtr, useDeleteList);
}


TMatrixD*
KinematicFitter::_transpose (const TMatrixD &alpha,
                             bool useDeleteList)
{
   int alphaRow = alpha.GetNrows();
   int alphaCol = alpha.GetNcols();
   TMatrixD *retvalPtr = new TMatrixD (alphaCol, alphaRow);
   retvalPtr->Transpose (alpha);
   if (useDeleteList)
   {
      sm_matrixPtrVec.push_back (retvalPtr);
   }
   return retvalPtr;
}
  
TMatrixD*
KinematicFitter::_transpose (TMatrixD *alphaPtr,
                             bool useDeleteList)
{
   return _transpose (*alphaPtr, useDeleteList);
}

void
KinematicFitter::_cleanupMatrixPtrs()
{
   unsigned int size = sm_matrixPtrVec.size();
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      delete sm_matrixPtrVec[loop];
   } // for loop;
   // We've delete everything.  Clear the vector so we don't re-delete
   // anything.
   sm_matrixPtrVec.clear();
}

// friends
ostream& operator<< (ostream& o_stream, KinematicFitter &rhs)
{
   KinematicFitter::InOutType type = KinematicFitter::kModified;
   if (! rhs.m_outputParticles.size())
   {
      type = KinematicFitter::kOriginal;
   }
   o_stream << "Input Particles:" << endl;
   for (int loop = 0; loop < rhs.numInputParticles(); ++loop)
   {
      o_stream << loop  << ") " << rhs.particleMom (loop, type) << endl;
   }
   vector< string > virtualNamesVec;
   o_stream << "Virtual Particles:" << endl;
   for (int loop = 0; loop < rhs.numVirtualParticles(); ++loop)
   {
      o_stream << loop  << ") " << rhs.virtualParticleMom (loop, type) 
               << " :";
      KinematicFitter::IVec retval;
      rhs._allInputParticles (loop, retval);
      string name = " [";
      for (int inner = 0; inner < (int) retval.size(); ++inner)
      {
         if (inner)
         {
            name += ", ";
         }
         name += Form ("%d", retval[inner]);
      }
      name += "]";
      o_stream << name << endl;     
      virtualNamesVec.push_back (name);
   }
   o_stream << "Constraints:" << endl;
   int count = 0;
   for (KinematicFitter::ConstraintVecIter iter 
           = rhs.m_constraintVec.begin();
        rhs.m_constraintVec.end() != iter;
        ++iter)
   {
      o_stream << count++ << ") ";
      if (! iter->isActive )
      {
         o_stream << "**INACTIVE**  ";
      }
      if (KinematicFitter::kSingleMassConstraint == iter->type)
      {
         o_stream << "part " << iter->index1 
                  << virtualNamesVec[iter->index1]
                  << " mass constraint = "
                  << iter->massConst << ".";
      } else if (KinematicFitter::kEqualMassConstraint == iter->type)
      {
         o_stream << "part " << iter->index1 
                  << virtualNamesVec[iter->index1]
                  << " mass constraint = "
                  << "part " << iter->index2 
                  << virtualNamesVec[iter->index2]
                  << " mass.";
      }
      o_stream << endl;
   } // for iter
   
   return o_stream;
} 

ostream& operator<< (ostream& o_stream, 
                     const TMatrixD &rhs)
{
   int numRows = rhs.GetNrows();
   int numCols = rhs.GetNcols();
   for (int row = 0; row < numRows; ++row)
   {
      for (int col = 0; col < numCols; ++col)
      {
         if (col)
         {
            o_stream << "  ";           
         }
         o_stream << setw (9) << rhs(row, col);
      } // for col
      o_stream << endl;
   } // for row
   return o_stream;
}

ostream& operator<< (ostream& o_stream, 
                     TMatrixD *rhs)
{
   o_stream << *rhs;
   return o_stream;
}

ostream& operator<< (ostream& o_stream, 
                     const TLorentzVector &rhs)
{
   o_stream << "("  << Form ("%6.2f", rhs.E()  )
            << ": " << Form ("%7.2f", rhs.Px() )
            << ", " << Form ("%7.2f", rhs.Py() )
            << ", " << Form ("%7.2f", rhs.Pz() )
            << ")";
   return o_stream;
}

ostream& operator<< (ostream& o_stream, 
                     const KinematicFitter::ISet &rhs)
{
   for (KinematicFitter::ISetIter iter = rhs.begin();
        rhs.end() != iter;
        ++iter)
   {
      o_stream << *iter << " ";
   } // for iter   
   return o_stream;
}

ostream& operator<< (ostream& o_stream, 
                     KinematicFitter::IVec &rhs)
{
   for (KinematicFitter::IVecIter iter = rhs.begin();
        rhs.end() != iter;
        ++iter)
   {
      o_stream << *iter << " ";
   } // for iter   
   return o_stream;
}

ostream& operator<< (ostream& o_stream, 
                     KinematicFitter::InputParticle &rhs)
{
   o_stream << "("  << Form ("%6.2f", rhs.E   )
            << ", " << Form ("%7.2f", rhs.mass)
            << ": " << Form ("%7.2f", rhs.Px  )
            << ", " << Form ("%7.2f", rhs.Py  )
            << ", " << Form ("%7.2f", rhs.Pz  )
            << ")";
   return o_stream;
}
