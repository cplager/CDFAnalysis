//#define kanamuon_cxx
//#include "kanamuon.h"

#ifndef KinFit_h
#define KinFit_h

//#include <TH2.h>
//#include <TStyle.h>
//#include <TCanvas.h>
//#include <iostream>

#include "Resolution.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintEp.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleCart.h"
#include "PhysicsTools/KinFitter/interface/TKinFitter.h"

//#include "ElectroWeakAnalysis/VPlusJets/interface/AngularVars.h"

bool doKinematicFit(bool                  isMuon,          
		    Int_t                 fflage,
		    const TLorentzVector     mup, 
		    const TLorentzVector     nvp, 
		    const TLorentzVector     ajp, 
		    const TLorentzVector     bjp, 
		    TLorentzVector     & fit_mup, 
		    TLorentzVector     & fit_nvp,
		    TLorentzVector     & fit_ajp, 
		    TLorentzVector     & fit_bjp, 
		    Float_t            & fit_chi2,
		    Int_t              & fit_NDF, 
		    Int_t              & fit_status)

{
    
  bool OK                     = false;
  Resolution* resolution      = new Resolution();
  
  TMatrixD m1(3,3);
  TMatrixD m2(3,3);
  TMatrixD m3(3,3);
  TMatrixD m4(3,3);
  m1.Zero();
  m2.Zero();
  m3.Zero();
  m4.Zero();
  
  double etRes, etaRes, phiRes;
  // lepton resolution
/*   const std::string& leptonName = ""; */
/*   if (isMuon) leptonName = "muon"; */
/*   else   leptonName = "electron"; */

  const TLorentzVector lepton   = mup;

  //  if(leptonName == "electron") {
  if(!isMuon) {
    OK = resolution->electronResolution(lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  } else {
    OK = resolution->muonResolution(    lepton.Et(), lepton.Eta(), etRes, etaRes, phiRes);
    if(!OK) return OK;
  }

  m1(0,0) = resolution->square(etRes);
  m1(1,1) = resolution->square(etaRes);
  m1(2,2) = resolution->square(phiRes);

  // MET resolution
  OK = resolution->PFMETResolution(     nvp.Et(),            etRes, etaRes, phiRes);
  if(!OK) return OK;
      
  m2(0,0) = resolution->square(etRes);
  m2(1,1) = 0.01; //25. ;//999.9; // resolution->square(etaRes)
  m2(2,2) = resolution->square(phiRes);
  
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( ajp.Et(), ajp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  
  m3(0,0) = resolution->square(etRes);
  m3(1,1) = resolution->square(etaRes);
  m3(2,2) = resolution->square(phiRes);
  
  // Leading Jet resolution
  OK = resolution->udscPFJetResolution( bjp.Et(), bjp.Eta(), etRes, etaRes, phiRes);
  if(!OK) return OK;
  m4(0,0) = resolution->square(etRes);
  m4(1,1) = resolution->square(etaRes);
  m4(2,2) = resolution->square(phiRes);
  
  TLorentzVector tmp_mup = mup;
  TLorentzVector tmp_nvp = nvp;
  TLorentzVector tmp_ajp = ajp;
  TLorentzVector tmp_bjp = bjp;
  
  // Fit Particle
  TFitParticleEtEtaPhi* particle1 = new TFitParticleEtEtaPhi( "Lepton",   "Lepton",   &tmp_mup,    &m1 );
  TFitParticleEtEtaPhi* particle2 = new TFitParticleEtEtaPhi( "Neutrino", "Neutrino", &tmp_nvp,    &m2 );
  TFitParticleEtEtaPhi* particle3 = new TFitParticleEtEtaPhi( "Jeta",     "Jeta",     &tmp_ajp,    &m3 );
  TFitParticleEtEtaPhi* particle4 = new TFitParticleEtEtaPhi( "Jetb",     "Jetb",     &tmp_bjp,    &m4 );
  
  // Constraint
  TFitConstraintM *mCons1 = new TFitConstraintM( "WMassConstrainta", "WMass-Constrainta", 0, 0 , 80.4);
  mCons1->addParticles1( particle1, particle2 );

  TFitConstraintM *mCons2 = new TFitConstraintM( "WMassConstraintb", "WMass-Constraintb", 0, 0 , 80.4);
  mCons2->addParticles1( particle3, particle4 );
  
/*   TFitConstraintEp *pxCons = new TFitConstraintEp( "PxConstraint", "Px-Constraint", 0, TFitConstraintEp::pX , (mup+nvp+ajp+bjp).Px() ); */
/*   pxCons->addParticles( particle1, particle2, particle3, particle4 ); */

/*   TFitConstraintEp *pyCons = new TFitConstraintEp( "PyConstraint", "Py-Constraint", 0, TFitConstraintEp::pY , (mup+nvp+ajp+bjp).Py() ); */
/*   pyCons->addParticles( particle1, particle2, particle3, particle4 ); */

  //Definition of the fitter
  TKinFitter* fitter = new TKinFitter("fitter", "fitter");
  if        (fflage == 1 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addMeasParticle( particle3 );
    fitter->addMeasParticle( particle4 );
    //    fitter->addConstraint( pxCons );
    //    fitter->addConstraint( pyCons );
    fitter->addConstraint( mCons1 );
    fitter->addConstraint( mCons2 );
  }else   if(fflage == 2 ){
    fitter->addMeasParticle( particle1 );
    fitter->addMeasParticle( particle2 );
    fitter->addConstraint( mCons1 );
  }else {return false;}

  //Set convergence criteria
  fitter->setMaxNbIter( 500 );
  fitter->setMaxDeltaS( 1e-2 );
  fitter->setMaxF( 1e-2 );
  fitter->setVerbosity(1);
  fitter->fit();

  //Return the kinematic fit results
  fit_status   = fitter->getStatus();
  fit_chi2     = fitter->getS();
  fit_NDF      = fitter->getNDF();
  fit_mup      = *(particle1->getCurr4Vec()); 
  fit_nvp      = *(particle2->getCurr4Vec()); 
  fit_ajp      = *(particle3->getCurr4Vec()); 
  fit_bjp      = *(particle4->getCurr4Vec()); 

  if(fitter->getStatus() == 0) { OK = true; } else { OK = false; }
 
  delete resolution;
  delete particle1;
  delete particle2;
  delete particle3;
  delete particle4;
  delete mCons1;
  delete mCons2;
/*   delete pxCons; */
/*   delete pyCons; */
  delete fitter;

  return OK;
}
//}
#endif
