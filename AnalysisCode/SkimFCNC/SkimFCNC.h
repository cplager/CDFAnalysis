//////////////////////////////////////////////////////////
//   This class has been automatically generated 
//     (Fri Apr 22 14:54:22 2005 by ROOT version3.05/07)
//   from TTree TopTree/TopTree
//   found on file: /cdf/scratch/zaw/fcnc/topnt_nt5_5.3.3test_tcZeeMC-Inclusive.root
//////////////////////////////////////////////////////////


#ifndef SkimFCNC_h
#define SkimFCNC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
// CLP start
#include <string>
#include "CLPClasses/CLPClasses.h" // all CLP stuff
#include "JetUser/JetEnergyCorrections.hh"
typedef std::vector< int > IVec;
// CLP stop

// CLP Update below constants to their maximums
const Int_t kMaxevt = 1;
const Int_t kMaxsummary = 1;
const Int_t kMaxprivertex = 1;
const Int_t kMaxzvtxs = 100;
const Int_t kMaxhepg = 1000;
const Int_t kMaxmuon = 100;
const Int_t kMaxsltmu = 100;
const Int_t kMaxcosmic = 1;
const Int_t kMaxelectron = 100;
const Int_t kMaxtau = 10;
const Int_t kMaxjet = 200;
const Int_t kMaxmet = 1;
const Int_t kMaxtopfit = 1;
const Int_t kMaxtrigInfo = 1;
const Int_t kMaxtrigName = 1;
const Int_t kMaxl2Cal = 100;
const Int_t kMaxoffltrack = 1000;
const Int_t kMaxsecvtxtrack = 1000;
const Int_t kMaxtjassoc = 1000;
const Int_t kMaxtvassoc = 100;
const Int_t kMaxjetprobtrack = 100;
const Int_t kMaxjetprobTJassoc = 200;
const Int_t kMaxobsp = 1000;
// CLP stop

//  const Int_t kMaxevt = 1;
//  const Int_t kMaxsummary = 1;
//  const Int_t kMaxprivertex = 1;
//  const Int_t kMaxzvtxs = 3;
//  const Int_t kMaxhepg = 790;
//  const Int_t kMaxmuon = 17;
//  const Int_t kMaxsltmu = 52;
//  const Int_t kMaxcosmic = 1;
//  const Int_t kMaxelectron = 21;
//  const Int_t kMaxtau = 7;
//  const Int_t kMaxjet = 153;
//  const Int_t kMaxmet = 1;
//  const Int_t kMaxtopfit = 1;
//  const Int_t kMaxtrigInfo = 1;
//  const Int_t kMaxtrigName = 1;
//  const Int_t kMaxl2Cal = 30;
//  const Int_t kMaxoffltrack = 168;
//  const Int_t kMaxsecvtxtrack = 91;
//  const Int_t kMaxtjassoc = 140;
//  const Int_t kMaxtvassoc = 43;
//  const Int_t kMaxjetprobtrack = 77;
//  const Int_t kMaxjetprobTJassoc = 150;
//  const Int_t kMaxobsp = 294;

class SkimFCNC {
   public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
//Declaration of leaves types
   Int_t           evt_;
   UInt_t          evt_fUniqueID[kMaxevt];   //[evt_]
   UInt_t          evt_fBits[kMaxevt];   //[evt_]
   Int_t           evt_runNumber[kMaxevt];   //[evt_]
   Int_t           evt_eventNumber[kMaxevt];   //[evt_]
   Int_t           evt_runSection[kMaxevt];   //[evt_]
   Int_t           evt_CosmicOOTLow[kMaxevt];   //[evt_]
   Int_t           evt_bunchNum159[kMaxevt];   //[evt_]
   Int_t           evt_bunchNum36[kMaxevt];   //[evt_]
   Double_t        evt_gliveInstLumi[kMaxevt];   //[evt_]
   Double_t        evt_gliveTotalLumi[kMaxevt];   //[evt_]
   Double_t        evt_instLumi[kMaxevt];   //[evt_]
   Double_t        evt_totalLumi[kMaxevt];   //[evt_]
   Double_t        evt_scalerInstLumi[kMaxevt];   //[evt_]
   Double_t        evt_scalerTotalLumi[kMaxevt];   //[evt_]
   Double_t        evt_bunchInstLumi[kMaxevt];   //[evt_]
   Double_t        evt_gliveBunchInstLumi[kMaxevt];   //[evt_]
   Double_t        evt_bunchTotalInstLumi[kMaxevt];   //[evt_]
   Int_t           evt_creationTime[kMaxevt];   //[evt_]
   Int_t           summary_;
   UInt_t          summary_fUniqueID[kMaxsummary];   //[summary_]
   UInt_t          summary_fBits[kMaxsummary];   //[summary_]
   Int_t           summary_fTopEventClass[kMaxsummary];   //[summary_]
   Int_t           summary_fnTightEle[kMaxsummary];   //[summary_]
   Int_t           summary_fnLooseEle[kMaxsummary];   //[summary_]
   Int_t           summary_fnTightMuo[kMaxsummary];   //[summary_]
   Int_t           summary_fnLooseMuo[kMaxsummary];   //[summary_]
   Int_t           summary_fnTightTau[kMaxsummary];   //[summary_]
   Int_t           summary_fnLooseTau[kMaxsummary];   //[summary_]
   Int_t           summary_fnTightLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnLooseLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnTrackLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnUniqueTrackLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnUniqueLooseLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnUniqueDilLepton[kMaxsummary];   //[summary_]
   Int_t           summary_fnTightJet[kMaxsummary];   //[summary_]
   Int_t           summary_fnLooseJet[kMaxsummary];   //[summary_]
   Float_t         summary_frawMet[kMaxsummary];   //[summary_]
   Float_t         summary_frawMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fvtxMet[kMaxsummary];   //[summary_]
   Float_t         summary_fvtxMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fmuoMet[kMaxsummary];   //[summary_]
   Float_t         summary_fmuoMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_ftlMet[kMaxsummary];   //[summary_]
   Float_t         summary_ftlMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fdilMet[kMaxsummary];   //[summary_]
   Float_t         summary_fdilMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_ftlJetMet[kMaxsummary];   //[summary_]
   Float_t         summary_ftlJetMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fdilJetMet[kMaxsummary];   //[summary_]
   Float_t         summary_fdilJetMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fljJetMet[kMaxsummary];   //[summary_]
   Float_t         summary_fljJetMetPhi[kMaxsummary];   //[summary_]
   Float_t         summary_fhadSumEt[kMaxsummary];   //[summary_]
   Float_t         summary_fhadSumEt3[kMaxsummary];   //[summary_]
   Float_t         summary_fhadCentrality[kMaxsummary];   //[summary_]
   Float_t         summary_fhadAplanarity[kMaxsummary];   //[summary_]
   Float_t         summary_fhadSphericity[kMaxsummary];   //[summary_]
   Float_t         summary_fExUnclustered[kMaxsummary];   //[summary_]
   Float_t         summary_fEyUnclustered[kMaxsummary];   //[summary_]
   Float_t         summary_fprimZV[kMaxsummary];   //[summary_]
   Float_t         summary_fjetZV[kMaxsummary];   //[summary_]
   Float_t         summary_fprimZVerr[kMaxsummary];   //[summary_]
   Float_t         summary_fjetZVerr[kMaxsummary];   //[summary_]
   Float_t         summary_fdileptonHT[kMaxsummary];   //[summary_]
   Float_t         summary_flepjetsHT[kMaxsummary];   //[summary_]
   Int_t           privertex_;
   UInt_t          privertex_fUniqueID[kMaxprivertex];   //[privertex_]
   UInt_t          privertex_fBits[kMaxprivertex];   //[privertex_]
   Float_t         privertex_sbm_x0[kMaxprivertex];   //[privertex_]
   Float_t         privertex_sbm_y0[kMaxprivertex];   //[privertex_]
   Float_t         privertex_sbm_slx[kMaxprivertex];   //[privertex_]
   Float_t         privertex_sbm_sly[kMaxprivertex];   //[privertex_]
   Float_t         privertex_cbm_x0[kMaxprivertex];   //[privertex_]
   Float_t         privertex_cbm_y0[kMaxprivertex];   //[privertex_]
   Float_t         privertex_cbm_slx[kMaxprivertex];   //[privertex_]
   Float_t         privertex_cbm_sly[kMaxprivertex];   //[privertex_]
   Float_t         privertex_x[kMaxprivertex];   //[privertex_]
   Float_t         privertex_y[kMaxprivertex];   //[privertex_]
   Float_t         privertex_z[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covxx[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covyy[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covzz[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covxy[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covxz[kMaxprivertex];   //[privertex_]
   Float_t         privertex_covyz[kMaxprivertex];   //[privertex_]
   Float_t         privertex_mcx[kMaxprivertex];   //[privertex_]
   Float_t         privertex_mcy[kMaxprivertex];   //[privertex_]
   Float_t         privertex_mcz[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxVx[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxVy[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxVz[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC11[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC22[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC33[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC12[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC13[kMaxprivertex];   //[privertex_]
   Float_t         privertex_SecVtxC23[kMaxprivertex];   //[privertex_]
   Int_t           privertex_nPrimeVtxTracks[kMaxprivertex];   //[privertex_]
   Float_t         privertex_primeVtxCharge[kMaxprivertex];   //[privertex_]
   Float_t         privertex_primeVtxChiSq[kMaxprivertex];   //[privertex_]
   Float_t         privertex_primeVtxNdof[kMaxprivertex];   //[privertex_]
   Int_t           zvtxs_;
   UInt_t          zvtxs_fUniqueID[kMaxzvtxs];   //[zvtxs_]
   UInt_t          zvtxs_fBits[kMaxzvtxs];   //[zvtxs_]
   Float_t         zvtxs_z_pos[kMaxzvtxs];   //[zvtxs_]
   Float_t         zvtxs_z_err[kMaxzvtxs];   //[zvtxs_]
   Int_t           zvtxs_n_trk[kMaxzvtxs];   //[zvtxs_]
   Float_t         zvtxs_sum_pt[kMaxzvtxs];   //[zvtxs_]
   Int_t           zvtxs_quality[kMaxzvtxs];   //[zvtxs_]
   Int_t           numZVtxs;
   Int_t           hepg_;
   UInt_t          hepg_fUniqueID[kMaxhepg];   //[hepg_]
   UInt_t          hepg_fBits[kMaxhepg];   //[hepg_]
   Int_t           hepg_Ind[kMaxhepg];   //[hepg_]
   Int_t           hepg_ID[kMaxhepg];   //[hepg_]
   Int_t           hepg_IDparent[kMaxhepg];   //[hepg_]
   Int_t           hepg_Stdhep[kMaxhepg];   //[hepg_]
   Int_t           hepg_Mo1[kMaxhepg];   //[hepg_]
   Int_t           hepg_Mo2[kMaxhepg];   //[hepg_]
   Int_t           hepg_Da1[kMaxhepg];   //[hepg_]
   Int_t           hepg_Da2[kMaxhepg];   //[hepg_]
   Float_t         hepg_Charge[kMaxhepg];   //[hepg_]
   Float_t         hepg_Px[kMaxhepg];   //[hepg_]
   Float_t         hepg_Py[kMaxhepg];   //[hepg_]
   Float_t         hepg_Pz[kMaxhepg];   //[hepg_]
   Float_t         hepg_E[kMaxhepg];   //[hepg_]
   Float_t         hepg_Mass[kMaxhepg];   //[hepg_]
   Int_t           numHepgObjs;
   Int_t           muon_;
   UInt_t          muon_fUniqueID[kMaxmuon];   //[muon_]
   UInt_t          muon_fBits[kMaxmuon];   //[muon_]
   Int_t           muon_ViewType[kMaxmuon];   //[muon_]
   Int_t           muon_muontype[kMaxmuon];   //[muon_]
   Int_t           muon_Fiducial[kMaxmuon];   //[muon_]
   Int_t           muon_trkLink[kMaxmuon];   //[muon_]
   Int_t           muon_TrkFiducial[kMaxmuon];   //[muon_]
   Float_t         muon_CmuFidD[kMaxmuon];   //[muon_]
   Float_t         muon_CmuFidX[kMaxmuon];   //[muon_]
   Float_t         muon_CmuFidZ[kMaxmuon];   //[muon_]
   Float_t         muon_CmpFidD[kMaxmuon];   //[muon_]
   Float_t         muon_CmpFidX[kMaxmuon];   //[muon_]
   Float_t         muon_CmpFidZ[kMaxmuon];   //[muon_]
   Float_t         muon_CmxFidD[kMaxmuon];   //[muon_]
   Float_t         muon_CmxFidX[kMaxmuon];   //[muon_]
   Float_t         muon_CmxFidZ[kMaxmuon];   //[muon_]
   UChar_t         muon_inBluebeam[kMaxmuon];   //[muon_]
   UChar_t         muon_inMiniskirt[kMaxmuon];   //[muon_]
   UChar_t         muon_inKeystone[kMaxmuon];   //[muon_]
   Float_t         muon_En[kMaxmuon];   //[muon_]
   Float_t         muon_P[kMaxmuon];   //[muon_]
   Float_t         muon_Px[kMaxmuon];   //[muon_]
   Float_t         muon_Py[kMaxmuon];   //[muon_]
   Float_t         muon_Pz[kMaxmuon];   //[muon_]
   Float_t         muon_Pt[kMaxmuon];   //[muon_]
   Float_t         muon_Eta[kMaxmuon];   //[muon_]
   Float_t         muon_Phi0[kMaxmuon];   //[muon_]
   Float_t         muon_Z0[kMaxmuon];   //[muon_]
   Float_t         muon_Charge[kMaxmuon];   //[muon_]
   Float_t         muon_D0[kMaxmuon];   //[muon_]
   Float_t         muon_PtCorr[kMaxmuon];   //[muon_]
   Float_t         muon_PxCorr[kMaxmuon];   //[muon_]
   Float_t         muon_PyCorr[kMaxmuon];   //[muon_]
   Float_t         muon_PzCorr[kMaxmuon];   //[muon_]
   Float_t         muon_PCorr[kMaxmuon];   //[muon_]
   Float_t         muon_BCP[kMaxmuon];   //[muon_]
   Float_t         muon_BCPx[kMaxmuon];   //[muon_]
   Float_t         muon_BCPy[kMaxmuon];   //[muon_]
   Float_t         muon_BCPz[kMaxmuon];   //[muon_]
   Float_t         muon_BCPt[kMaxmuon];   //[muon_]
   Float_t         muon_BCEta[kMaxmuon];   //[muon_]
   Float_t         muon_BCPhi0[kMaxmuon];   //[muon_]
   Float_t         muon_BCZ0[kMaxmuon];   //[muon_]
   Float_t         muon_BCCharge[kMaxmuon];   //[muon_]
   Float_t         muon_RawP[kMaxmuon];   //[muon_]
   Float_t         muon_RawPx[kMaxmuon];   //[muon_]
   Float_t         muon_RawPy[kMaxmuon];   //[muon_]
   Float_t         muon_RawPz[kMaxmuon];   //[muon_]
   Float_t         muon_RawPt[kMaxmuon];   //[muon_]
   Float_t         muon_RawEta[kMaxmuon];   //[muon_]
   Float_t         muon_RawPhi0[kMaxmuon];   //[muon_]
   Float_t         muon_RawZ0[kMaxmuon];   //[muon_]
   Float_t         muon_RawCharge[kMaxmuon];   //[muon_]
   Int_t           muon_TrkSiHits[kMaxmuon];   //[muon_]
   Int_t           muon_TrkAxHits[kMaxmuon];   //[muon_]
   Int_t           muon_TrkStHits[kMaxmuon];   //[muon_]
   Int_t           muon_TrkAxSeg[kMaxmuon];   //[muon_]
   Int_t           muon_TrkStSeg[kMaxmuon];   //[muon_]
   Float_t         muon_TrkRedChi2CT[kMaxmuon];   //[muon_]
   Int_t           muon_TrkID[kMaxmuon];   //[muon_]
   Int_t           muon_TrkIDOrig[kMaxmuon];   //[muon_]
   Float_t         muon_HadEnergy[kMaxmuon];   //[muon_]
   Float_t         muon_EmEnergy[kMaxmuon];   //[muon_]
   Float_t         muon_EventZVertex[kMaxmuon];   //[muon_]
   Float_t         muon_DeltaZVertex[kMaxmuon];   //[muon_]
   Float_t         muon_Isol[kMaxmuon];   //[muon_]
   Float_t         muon_EmIsol4[kMaxmuon];   //[muon_]
   Float_t         muon_HadIsol4[kMaxmuon];   //[muon_]
   Float_t         muon_TotalIsol4[kMaxmuon];   //[muon_]
   Float_t         muon_EmIsol7[kMaxmuon];   //[muon_]
   Float_t         muon_HadIsol7[kMaxmuon];   //[muon_]
   Float_t         muon_TotalIsol7[kMaxmuon];   //[muon_]
   Float_t         muon_TrkIsol[kMaxmuon];   //[muon_]
   Float_t         muon_CmuDx[kMaxmuon];   //[muon_]
   Float_t         muon_CmuDz[kMaxmuon];   //[muon_]
   Float_t         muon_CmuDphi[kMaxmuon];   //[muon_]
   Float_t         muon_CmpDx[kMaxmuon];   //[muon_]
   Float_t         muon_CmpDphi[kMaxmuon];   //[muon_]
   Float_t         muon_CmxDx[kMaxmuon];   //[muon_]
   Float_t         muon_CmxDz[kMaxmuon];   //[muon_]
   Float_t         muon_CmxDphi[kMaxmuon];   //[muon_]
   Float_t         muon_BmuDx[kMaxmuon];   //[muon_]
   Float_t         muon_Dx[kMaxmuon][4];   //[muon_]
   Int_t           muon_Wedge[kMaxmuon][4];   //[muon_]
   Int_t           muon_Side[kMaxmuon][4];   //[muon_]
   Int_t           muon_NumAssHits[kMaxmuon][4];   //[muon_]
   Float_t         muon_CheckEn[kMaxmuon];   //[muon_]
   Float_t         muon_CheckPx[kMaxmuon];   //[muon_]
   Float_t         muon_CheckPy[kMaxmuon];   //[muon_]
   Float_t         muon_CheckPz[kMaxmuon];   //[muon_]
   Float_t         muon_CheckZ0[kMaxmuon];   //[muon_]
   Float_t         muon_CheckZ0Err[kMaxmuon];   //[muon_]
   Float_t         muon_CheckD0[kMaxmuon];   //[muon_]
   Int_t           muon_CheckTrkId[kMaxmuon];   //[muon_]
   Int_t           muon_CutCode[kMaxmuon];   //[muon_]
   Float_t         muon_BruceLLK[kMaxmuon];   //[muon_]
   UChar_t         muon_goodForCMXTrig[kMaxmuon];   //[muon_]
   Float_t         muon_Dz[kMaxmuon][4];   //[muon_]
   Float_t         muon_Dphi[kMaxmuon][4];   //[muon_]
   Float_t         muon_Dtheta[kMaxmuon][4];   //[muon_]
   Float_t         muon_ChiSqX[kMaxmuon][4];   //[muon_]
   Float_t         muon_ChiSqZ[kMaxmuon][4];   //[muon_]
   Float_t         muon_ChiSqXPos[kMaxmuon][4];   //[muon_]
   Float_t         muon_ChiSqZPos[kMaxmuon][4];   //[muon_]
   Int_t           muon_NumCntrHits[kMaxmuon][4];   //[muon_]
   Int_t           muon_HitPatt[kMaxmuon][4];   //[muon_]
   Int_t           muon_NumChamberHits[kMaxmuon][4];   //[muon_]
   Float_t         muon_PosX[kMaxmuon][4];   //[muon_]
   Float_t         muon_PosY[kMaxmuon][4];   //[muon_]
   Float_t         muon_PosZ[kMaxmuon][4];   //[muon_]
   Float_t         muon_DirX[kMaxmuon][4];   //[muon_]
   Float_t         muon_DirY[kMaxmuon][4];   //[muon_]
   Float_t         muon_DirZ[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkPosX[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkPosY[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkPosZ[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkDirX[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkDirY[kMaxmuon][4];   //[muon_]
   Float_t         muon_TrkDirZ[kMaxmuon][4];   //[muon_]
   Int_t           numMuObjs;
   Char_t          muoViewDescr1[40];
   Char_t          muoViewDescr2[40];
   Char_t          muoViewDescr3[40];
   Char_t          muoViewDescr4[40];
   Char_t          muoViewDescr5[40];
   Int_t           sltmu_;
   UInt_t          sltmu_fUniqueID[kMaxsltmu];   //[sltmu_]
   UInt_t          sltmu_fBits[kMaxsltmu];   //[sltmu_]
   Short_t         sltmu_jetid[kMaxsltmu];   //[sltmu_]
   Short_t         sltmu_trkid[kMaxsltmu];   //[sltmu_]
   Short_t         sltmu_muid[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_jetCollType[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_dRclstJet[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_ptRel[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt06[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet06[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt06C1[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet06C1[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt06C2[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet06C2[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt04[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet04[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt04C1[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet04C1[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt04C2[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet04C2[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt02[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet02[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt02C1[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet02C1[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_sumPt02C2[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nTrkJet02C2[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_emE[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_hadE[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_neightborEmE[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_neightborHadE[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_coneR4Et[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_fiducial[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_cmuFidSig[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_cmpFidSig[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_cmxFidSig[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_fiducialSig[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_isTaggable[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_isTagged[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_nUsedVar[kMaxsltmu];   //[sltmu_]
   Int_t           sltmu_usedVar[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_likelihood[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuDx[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuDz[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuDphi[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuHits[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuChi2X[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmuChi2Z[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmpDx[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmpDphi[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmpChi2X[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmxDx[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmxDz[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmxDphi[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmxChi2X[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_pullCmxChi2Z[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_phiCot[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_etaCot[kMaxsltmu];   //[sltmu_]
   Float_t         sltmu_radLengths[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_absLengths[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_eLoss[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extPosX[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extPosY[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extPosZ[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extDirX[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extDirY[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_extDirZ[kMaxsltmu][4];   //[sltmu_]
   Float_t         sltmu_dist[kMaxsltmu][4];   //[sltmu_]
   Int_t           numSltMuObjs;
   Int_t           cosmic_;
   UInt_t          cosmic_fUniqueID[kMaxcosmic];   //[cosmic_]
   UInt_t          cosmic_fBits[kMaxcosmic];   //[cosmic_]
   Int_t           cosmic_cosmicBit[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasCosmicRay[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasFewTracks[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLargeD0[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLargeZ0[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasAcolTrack[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasOOTEnergy[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasOOTScintillators[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasNoVertex[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLargeDz[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLargeDPhiMuStub[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_isMuonNothing[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_isMuonStub[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_isMuonTrack[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_isMuonMuon[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasOOTTof[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasNotOutgoingPair[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLowEmFrac[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasLowQFrac[kMaxcosmic];   //[cosmic_]
   UChar_t         cosmic_hasOOTCaloE[kMaxcosmic];   //[cosmic_]
   Int_t           cosmic_nLegs[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_deltaTdc[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_deltaTof[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_dPhiMuStub[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_acollinearity[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_d0Rho[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_d0Beta[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_dPhi[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_sumEta[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_seedZ[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_seedZErr[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_nTrkZVtx[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_meanZVtx[kMaxcosmic];   //[cosmic_]
   Float_t         cosmic_meanZVtxErr[kMaxcosmic];   //[cosmic_]
   Int_t           cosmic_muoLnk[kMaxcosmic][2];   //[cosmic_]
   Int_t           cosmic_trkLnk[kMaxcosmic][2];   //[cosmic_]
   Int_t           cosmic_trackId[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_pt[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_crv[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_cot[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_phi[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_eta[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_z0[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_d0[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_d0C[kMaxcosmic][2];   //[cosmic_]
   Int_t           cosmic_charge[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_algo[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_deltaT0[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_errorDeltaT0[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_deltaBetaInv[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_errorDeltaBetaInv[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_fitT0[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_fitdT0[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_fitCrdT0[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_fitChi2[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_fitdChi2[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_fitDir[kMaxcosmic][2][2];   //[cosmic_]
   Int_t           cosmic_fitNCOTH[kMaxcosmic][2][2];   //[cosmic_]
   Int_t           cosmic_fitNCOTHExp[kMaxcosmic][2][2];   //[cosmic_]
   Float_t         cosmic_emE[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_hadE[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_hadTDC[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_ngbrEmE[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_ngbrHadE[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_ngbrHadTDC[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_stubPhi[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_tof[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_tofMethod[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_csp[kMaxcosmic][2];   //[cosmic_]
   Float_t         cosmic_csx[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validLeg[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validTrack[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validStub[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validEnergy[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validHadTdc[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validTof[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validCsp[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_validCsx[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_trackLessMuon[kMaxcosmic][2];   //[cosmic_]
   UChar_t         cosmic_stubLessMuon[kMaxcosmic][2];   //[cosmic_]
   Int_t           electron_;
   UInt_t          electron_fUniqueID[kMaxelectron];   //[electron_]
   UInt_t          electron_fBits[kMaxelectron];   //[electron_]
   Int_t           electron_CollType[kMaxelectron];   //[electron_]
   Int_t           electron_Region[kMaxelectron];   //[electron_]
   Int_t           electron_Conversion[kMaxelectron];   //[electron_]
   Int_t           electron_trkLink[kMaxelectron];   //[electron_]
   Int_t           electron_phxTrkLink[kMaxelectron];   //[electron_]
   Float_t         electron_En[kMaxelectron];   //[electron_]
   Float_t         electron_RawEn[kMaxelectron];   //[electron_]
   Float_t         electron_CluEn[kMaxelectron];   //[electron_]
   Float_t         electron_Px[kMaxelectron];   //[electron_]
   Float_t         electron_RawPx[kMaxelectron];   //[electron_]
   Float_t         electron_CluPx[kMaxelectron];   //[electron_]
   Float_t         electron_Py[kMaxelectron];   //[electron_]
   Float_t         electron_RawPy[kMaxelectron];   //[electron_]
   Float_t         electron_CluPy[kMaxelectron];   //[electron_]
   Float_t         electron_Pz[kMaxelectron];   //[electron_]
   Float_t         electron_RawPz[kMaxelectron];   //[electron_]
   Float_t         electron_CluPz[kMaxelectron];   //[electron_]
   Float_t         electron_Et[kMaxelectron];   //[electron_]
   Float_t         electron_RawEt[kMaxelectron];   //[electron_]
   Float_t         electron_CluEt[kMaxelectron];   //[electron_]
   Float_t         electron_Phi[kMaxelectron];   //[electron_]
   Float_t         electron_RawPhi[kMaxelectron];   //[electron_]
   Float_t         electron_CluPhi[kMaxelectron];   //[electron_]
   Float_t         electron_Eta[kMaxelectron];   //[electron_]
   Float_t         electron_RawEta[kMaxelectron];   //[electron_]
   Float_t         electron_CluEta[kMaxelectron];   //[electron_]
   Float_t         electron_Clu_totEt_jetZ[kMaxelectron];   //[electron_]
   Float_t         electron_Clu_phi_jetZ[kMaxelectron];   //[electron_]
   Int_t           electron_TrkSiHits[kMaxelectron];   //[electron_]
   Int_t           electron_TrkAxHits[kMaxelectron];   //[electron_]
   Int_t           electron_TrkStHits[kMaxelectron];   //[electron_]
   Int_t           electron_TrkAxSeg[kMaxelectron];   //[electron_]
   Int_t           electron_TrkStSeg[kMaxelectron];   //[electron_]
   Float_t         electron_TrkP[kMaxelectron];   //[electron_]
   Float_t         electron_BCP[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkP[kMaxelectron];   //[electron_]
   Float_t         electron_TrkPx[kMaxelectron];   //[electron_]
   Float_t         electron_BCPx[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkPx[kMaxelectron];   //[electron_]
   Float_t         electron_TrkPy[kMaxelectron];   //[electron_]
   Float_t         electron_BCPy[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkPy[kMaxelectron];   //[electron_]
   Float_t         electron_TrkPz[kMaxelectron];   //[electron_]
   Float_t         electron_BCPz[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkPz[kMaxelectron];   //[electron_]
   Float_t         electron_TrkPt[kMaxelectron];   //[electron_]
   Float_t         electron_BCPt[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkPt[kMaxelectron];   //[electron_]
   Float_t         electron_TrkEta[kMaxelectron];   //[electron_]
   Float_t         electron_BCEta[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkEta[kMaxelectron];   //[electron_]
   Float_t         electron_TrkPhi0[kMaxelectron];   //[electron_]
   Float_t         electron_BCPhi0[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkPhi0[kMaxelectron];   //[electron_]
   Float_t         electron_TrkZ0[kMaxelectron];   //[electron_]
   Float_t         electron_BCZ0[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkZ0[kMaxelectron];   //[electron_]
   Float_t         electron_Charge[kMaxelectron];   //[electron_]
   Float_t         electron_BCCharge[kMaxelectron];   //[electron_]
   Float_t         electron_RawTrkCharge[kMaxelectron];   //[electron_]
   Float_t         electron_TrkD0[kMaxelectron];   //[electron_]
   Int_t           electron_TrkID[kMaxelectron];   //[electron_]
   Int_t           electron_TrkIDOrig[kMaxelectron];   //[electron_]
   Int_t           electron_Fiducial[kMaxelectron];   //[electron_]
   Int_t           electron_TrkFiducial[kMaxelectron];   //[electron_]
   Float_t         electron_EtaDet[kMaxelectron];   //[electron_]
   Float_t         electron_EtaEvent[kMaxelectron];   //[electron_]
   Float_t         electron_HadEt[kMaxelectron];   //[electron_]
   Float_t         electron_TotalEt[kMaxelectron];   //[electron_]
   Float_t         electron_Hadem[kMaxelectron];   //[electron_]
   Float_t         electron_EmEt[kMaxelectron];   //[electron_]
   Float_t         electron_HadEn[kMaxelectron];   //[electron_]
   Float_t         electron_TotalEn[kMaxelectron];   //[electron_]
   Float_t         electron_TotalIsol4[kMaxelectron];   //[electron_]
   Float_t         electron_TotalIsol7[kMaxelectron];   //[electron_]
   Float_t         electron_EmIsol4[kMaxelectron];   //[electron_]
   Float_t         electron_EmIsol7[kMaxelectron];   //[electron_]
   Float_t         electron_HadIsol4[kMaxelectron];   //[electron_]
   Float_t         electron_HadIsol7[kMaxelectron];   //[electron_]
   Float_t         electron_PprEn[kMaxelectron];   //[electron_]
   Int_t           electron_Pem3x3FitTow[kMaxelectron];   //[electron_]
   Float_t         electron_Pem3x3DetEta[kMaxelectron];   //[electron_]
   Float_t         electron_Pem3x3Phi[kMaxelectron];   //[electron_]
   Float_t         electron_Pem3x3FitEn[kMaxelectron];   //[electron_]
   Float_t         electron_Pem3x3Chisq[kMaxelectron];   //[electron_]
   Int_t           electron_NumPes2d[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2dEta[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2dPhi[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2dX[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2dY[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2d5by9U[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2d5by9V[kMaxelectron];   //[electron_]
   Float_t         electron_Pes2dEn[kMaxelectron];   //[electron_]
   Float_t         electron_ClosestPes2d5by9U[kMaxelectron];   //[electron_]
   Float_t         electron_ClosestPes2d5by9V[kMaxelectron];   //[electron_]
   Float_t         electron_ClosestPes2dEn[kMaxelectron];   //[electron_]
   Float_t         electron_LshrTrk[kMaxelectron];   //[electron_]
   Float_t         electron_Lshr3[kMaxelectron];   //[electron_]
   Float_t         electron_Lshr2[kMaxelectron];   //[electron_]
   Float_t         electron_Isol[kMaxelectron];   //[electron_]
   Float_t         electron_LeakE[kMaxelectron];   //[electron_]
   Float_t         electron_EP[kMaxelectron];   //[electron_]
   Float_t         electron_EventZVertex[kMaxelectron];   //[electron_]
   Float_t         electron_DeltaZVertex[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCESx[kMaxelectron];   //[electron_]
   Float_t         electron_BestCESx[kMaxelectron];   //[electron_]
   Float_t         electron_DeltaX[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCESz[kMaxelectron];   //[electron_]
   Float_t         electron_BestCESz[kMaxelectron];   //[electron_]
   Float_t         electron_DeltaZ[kMaxelectron];   //[electron_]
   Float_t         electron_StripChi2[kMaxelectron];   //[electron_]
   Float_t         electron_WireChi2[kMaxelectron];   //[electron_]
   Float_t         electron_TrkIsol[kMaxelectron];   //[electron_]
   Int_t           electron_Towers[kMaxelectron];   //[electron_]
   Int_t           electron_Tracks[kMaxelectron];   //[electron_]
   Int_t           electron_SeedEta[kMaxelectron];   //[electron_]
   Int_t           electron_SeedPhi[kMaxelectron];   //[electron_]
   Float_t         electron_SeedEn[kMaxelectron];   //[electron_]
   Float_t         electron_SeedEt[kMaxelectron];   //[electron_]
   Float_t         electron_PhoZV[kMaxelectron];   //[electron_]
   Float_t         electron_PhoRawEn[kMaxelectron];   //[electron_]
   Float_t         electron_PhoRawEt[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEt[kMaxelectron];   //[electron_]
   Float_t         electron_PhoPhi[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEta[kMaxelectron];   //[electron_]
   Float_t         electron_PhoCESx[kMaxelectron];   //[electron_]
   Float_t         electron_PhoCESz[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEneStrip[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEneWire[kMaxelectron];   //[electron_]
   Float_t         electron_PhoStripChi2[kMaxelectron];   //[electron_]
   Float_t         electron_PhoWireChi2[kMaxelectron];   //[electron_]
   Float_t         electron_PhoCES2x[kMaxelectron];   //[electron_]
   Float_t         electron_PhoCES2z[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEneStrip2[kMaxelectron];   //[electron_]
   Float_t         electron_PhoEneWire2[kMaxelectron];   //[electron_]
   Float_t         electron_PhoCesSlide[kMaxelectron];   //[electron_]
   Int_t           electron_PhoN3D[kMaxelectron];   //[electron_]
   Float_t         electron_PhoPt[kMaxelectron];   //[electron_]
   Float_t         electron_PhoIsol[kMaxelectron];   //[electron_]
   Float_t         electron_PhoTrkIsol[kMaxelectron];   //[electron_]
   Float_t         electron_PhoLshr[kMaxelectron];   //[electron_]
   Float_t         electron_PhoHadem[kMaxelectron];   //[electron_]
   Float_t         electron_RawEnergyWire[kMaxelectron];   //[electron_]
   Float_t         electron_RawEnergyStrip[kMaxelectron];   //[electron_]
   Float_t         electron_FitEnergyWire[kMaxelectron];   //[electron_]
   Float_t         electron_FitEnergyStrip[kMaxelectron];   //[electron_]
   Int_t           electron_CESside[kMaxelectron];   //[electron_]
   Int_t           electron_CESmodule[kMaxelectron];   //[electron_]
   Int_t           electron_TrkCESside[kMaxelectron];   //[electron_]
   Int_t           electron_TrkCESmodule[kMaxelectron];   //[electron_]
   Int_t           electron_CPR[kMaxelectron];   //[electron_]
   Int_t           electron_CPRside[kMaxelectron];   //[electron_]
   Int_t           electron_CPRmodule[kMaxelectron];   //[electron_]
   Int_t           electron_TrkCPRside[kMaxelectron];   //[electron_]
   Int_t           electron_TrkCPRmodule[kMaxelectron];   //[electron_]
   Float_t         electron_CPRx[kMaxelectron];   //[electron_]
   Float_t         electron_DeltaXCPR[kMaxelectron];   //[electron_]
   Float_t         electron_RawEnergyCPR[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCPRx[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCPRz[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCPRphi[kMaxelectron];   //[electron_]
   Float_t         electron_TrkCPReta[kMaxelectron];   //[electron_]
   Float_t         electron_WidthWire[kMaxelectron];   //[electron_]
   Float_t         electron_WidthStrip[kMaxelectron];   //[electron_]
   Float_t         electron_CheckEn[kMaxelectron];   //[electron_]
   Float_t         electron_CheckPx[kMaxelectron];   //[electron_]
   Float_t         electron_CheckPy[kMaxelectron];   //[electron_]
   Float_t         electron_CheckPz[kMaxelectron];   //[electron_]
   Float_t         electron_CheckZ0[kMaxelectron];   //[electron_]
   Float_t         electron_CheckZ0Err[kMaxelectron];   //[electron_]
   Int_t           electron_CheckConv[kMaxelectron];   //[electron_]
   Int_t           electron_CheckTrkId[kMaxelectron];   //[electron_]
   Int_t           electron_CutCode[kMaxelectron];   //[electron_]
   Int_t           electron_PhxTrkSiHits[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkP[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkPx[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkPy[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkPz[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkPt[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkEta[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkPhi0[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkZ0[kMaxelectron];   //[electron_]
   Float_t         electron_PhxCharge[kMaxelectron];   //[electron_]
   Float_t         electron_PhxTrkD0[kMaxelectron];   //[electron_]
   UChar_t         electron_PhxMatch[kMaxelectron];   //[electron_]
   Int_t           electron_PhxCotMatchID[kMaxelectron];   //[electron_]
   Float_t         electron_PhxDrTrkPes[kMaxelectron];   //[electron_]
   Float_t         electron_BruceLLK[kMaxelectron];   //[electron_]
   Int_t           numEmObjs;
   Char_t          eleViewDescr1[40];
   Char_t          eleViewDescr2[40];
   Char_t          eleViewDescr3[40];
   Char_t          eleViewDescr4[40];
   Char_t          eleViewDescr5[40];
   Int_t           tau_;
   UInt_t          tau_fUniqueID[kMaxtau];   //[tau_]
   UInt_t          tau_fBits[kMaxtau];   //[tau_]
   Int_t           tau_ntowers[kMaxtau];   //[tau_]
   Int_t           tau_nclumptowers[kMaxtau];   //[tau_]
   Int_t           tau_ntrks[kMaxtau];   //[tau_]
   Int_t           tau_ntrks10deg[kMaxtau];   //[tau_]
   Int_t           tau_ntrks30deg[kMaxtau];   //[tau_]
   Int_t           tau_ntrks10to30deg[kMaxtau];   //[tau_]
   Int_t           tau_charge10deg[kMaxtau];   //[tau_]
   Int_t           tau_nwrongvtxtrk10deg[kMaxtau];   //[tau_]
   Int_t           tau_npi0s[kMaxtau];   //[tau_]
   Int_t           tau_nmustubs[kMaxtau];   //[tau_]
   Int_t           tau_nmuhits[kMaxtau];   //[tau_]
   Float_t         tau_px[kMaxtau];   //[tau_]
   Float_t         tau_py[kMaxtau];   //[tau_]
   Float_t         tau_pz[kMaxtau];   //[tau_]
   Float_t         tau_e[kMaxtau];   //[tau_]
   Float_t         tau_pxtrk[kMaxtau];   //[tau_]
   Float_t         tau_pytrk[kMaxtau];   //[tau_]
   Float_t         tau_pztrk[kMaxtau];   //[tau_]
   Float_t         tau_etrk[kMaxtau];   //[tau_]
   Float_t         tau_pxvis[kMaxtau];   //[tau_]
   Float_t         tau_pyvis[kMaxtau];   //[tau_]
   Float_t         tau_pzvis[kMaxtau];   //[tau_]
   Float_t         tau_evis[kMaxtau];   //[tau_]
   Float_t         tau_emfrac[kMaxtau];   //[tau_]
   Float_t         tau_clusterdeteta[kMaxtau];   //[tau_]
   Float_t         tau_seedtrkdeteta[kMaxtau];   //[tau_]
   Float_t         tau_clustereta[kMaxtau];   //[tau_]
   Float_t         tau_clusterphi[kMaxtau];   //[tau_]
   Float_t         tau_etaeta[kMaxtau];   //[tau_]
   Float_t         tau_phiphi[kMaxtau];   //[tau_]
   Float_t         tau_delr[kMaxtau];   //[tau_]
   Float_t         tau_emenergy[kMaxtau];   //[tau_]
   Float_t         tau_hadenergy[kMaxtau];   //[tau_]
   Float_t         tau_emet[kMaxtau];   //[tau_]
   Float_t         tau_hadet[kMaxtau];   //[tau_]
   Float_t         tau_caloetiso[kMaxtau];   //[tau_]
   Float_t         tau_calohadetiso[kMaxtau];   //[tau_]
   Float_t         tau_caloemetiso[kMaxtau];   //[tau_]
   Float_t         tau_refvtxz[kMaxtau];   //[tau_]
   Float_t         tau_trackspt[kMaxtau];   //[tau_]
   Float_t         tau_trkscalarptsum[kMaxtau];   //[tau_]
   Float_t         tau_tracksmass[kMaxtau];   //[tau_]
   Float_t         tau_trackseta[kMaxtau];   //[tau_]
   Float_t         tau_tracksphi[kMaxtau];   //[tau_]
   Float_t         tau_trackiso[kMaxtau];   //[tau_]
   Float_t         tau_seedtrkvz[kMaxtau];   //[tau_]
   Float_t         tau_seedtrkpt[kMaxtau];   //[tau_]
   Float_t         tau_angleseedtocluster[kMaxtau];   //[tau_]
   Float_t         tau_phiseedtocluster[kMaxtau];   //[tau_]
   Float_t         tau_etaseedtocluster[kMaxtau];   //[tau_]
   Float_t         tau_pi0trackmass[kMaxtau];   //[tau_]
   Float_t         tau_pi0trackpt[kMaxtau];   //[tau_]
   Int_t           tau_trkindex[kMaxtau];   //[tau_]
   Float_t         tau_et[kMaxtau];   //[tau_]
   Float_t         tau_ep[kMaxtau];   //[tau_]
   Float_t         tau_ep_vis[kMaxtau];   //[tau_]
   Float_t         tau_zces[kMaxtau];   //[tau_]
   Float_t         tau_d0[kMaxtau];   //[tau_]
   Int_t           tau_tracklinks[kMaxtau][5];   //[tau_]
   Int_t           tau_tracknaxialhits[kMaxtau][5];   //[tau_]
   Int_t           tau_tracknstereohits[kMaxtau][5];   //[tau_]
   Int_t           tau_cutword[kMaxtau];   //[tau_]
   Float_t         tau_conesize[kMaxtau];   //[tau_]
   Float_t         tau_ehadoverp[kMaxtau];   //[tau_]
   Float_t         tau_calomass[kMaxtau];   //[tau_]
   Int_t           tau_ntracksincone[kMaxtau];   //[tau_]
   Int_t           tau_chargeincone[kMaxtau];   //[tau_]
   Int_t           tau_nisotracks[kMaxtau];   //[tau_]
   Int_t           tau_nisopi0s[kMaxtau];   //[tau_]
   Int_t           tau_taugrouplikes[kMaxtau];   //[tau_]
   Float_t         tau_pi0eta[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0phi[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0e[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0px[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0py[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0pz[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0pt[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0detectorz[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip1rawenergy[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip1fitpos[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip1width[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip1chi2[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip2rawenergy[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip2fitpos[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip2width[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0strip2chi2[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire1rawenergy[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire1fitpos[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire1width[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire1chi2[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire2rawenergy[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire2fitpos[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire2width[kMaxtau][5];   //[tau_]
   Float_t         tau_pi0wire2chi2[kMaxtau][5];   //[tau_]
   Int_t           numTauObjs;
   Int_t           jet_;
   UInt_t          jet_fUniqueID[kMaxjet];   //[jet_]
   UInt_t          jet_fBits[kMaxjet];   //[jet_]
   Int_t           jet_CollType[kMaxjet];   //[jet_]
   Float_t         jet_Px[kMaxjet];   //[jet_]
   Float_t         jet_Py[kMaxjet];   //[jet_]
   Float_t         jet_Pz[kMaxjet];   //[jet_]
   Float_t         jet_En[kMaxjet];   //[jet_]
   Float_t         jet_EtaDetector[kMaxjet];   //[jet_]
   Float_t         jet_GuardEnergy[kMaxjet];   //[jet_]
   Float_t         jet_EmFraction[kMaxjet];   //[jet_]
   Float_t         jet_CentroidEta[kMaxjet];   //[jet_]
   Float_t         jet_CentroidPhi[kMaxjet];   //[jet_]
   Float_t         jet_EtaMoment[kMaxjet];   //[jet_]
   Float_t         jet_PhiMoment[kMaxjet];   //[jet_]
   Float_t         jet_EtaPhiMoment[kMaxjet];   //[jet_]
   Float_t         jet_CentroidEt[kMaxjet];   //[jet_]
   Float_t         jet_TotP[kMaxjet];   //[jet_]
   Float_t         jet_TotPt[kMaxjet];   //[jet_]
   Float_t         jet_Pt[kMaxjet];   //[jet_]
   Float_t         jet_PtSquared[kMaxjet];   //[jet_]
   Float_t         jet_TotEt[kMaxjet];   //[jet_]
   Float_t         jet_Et[kMaxjet];   //[jet_]
   Float_t         jet_Eta[kMaxjet];   //[jet_]
   Float_t         jet_Theta[kMaxjet];   //[jet_]
   Float_t         jet_Phi[kMaxjet];   //[jet_]
   Float_t         jet_MassSquared[kMaxjet];   //[jet_]
   Float_t         jet_Mass[kMaxjet];   //[jet_]
   Float_t         jet_Rapidity[kMaxjet];   //[jet_]
   Float_t         jet_secvTag[kMaxjet];   //[jet_]
   Float_t         jet_secvPass[kMaxjet];   //[jet_]
   Float_t         jet_secvCharge[kMaxjet];   //[jet_]
   Float_t         jet_secvL3d[kMaxjet];   //[jet_]
   Float_t         jet_secvDl3d[kMaxjet];   //[jet_]
   Float_t         jet_secvL2d[kMaxjet];   //[jet_]
   Float_t         jet_secvDl2d[kMaxjet];   //[jet_]
   Float_t         jet_secvLzd[kMaxjet];   //[jet_]
   Float_t         jet_secvDlzd[kMaxjet];   //[jet_]
   Float_t         jet_secvChisq[kMaxjet];   //[jet_]
   Float_t         jet_secvChisqr[kMaxjet];   //[jet_]
   Float_t         jet_secvChisqz[kMaxjet];   //[jet_]
   Float_t         jet_secvNtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvNdtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvNvtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvMass[kMaxjet];   //[jet_]
   Float_t         jet_secvPt[kMaxjet];   //[jet_]
   Float_t         jet_secvPxVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvPyVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvPzVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvEVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvX[kMaxjet];   //[jet_]
   Float_t         jet_secvY[kMaxjet];   //[jet_]
   Float_t         jet_secvZ[kMaxjet];   //[jet_]
   Float_t         jet_secvXXerr[kMaxjet];   //[jet_]
   Float_t         jet_secvYYerr[kMaxjet];   //[jet_]
   Float_t         jet_secvZZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvXYerr[kMaxjet];   //[jet_]
   Float_t         jet_secvXZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvYZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvNPass1Tracks[kMaxjet];   //[jet_]
   Float_t         jet_secvNPass2Tracks[kMaxjet];   //[jet_]
   Float_t         jet_secvDz[kMaxjet];   //[jet_]
   Float_t         jet_secvPtSec[kMaxjet];   //[jet_]
   Float_t         jet_secvPtRelSec[kMaxjet];   //[jet_]
   Float_t         jet_secvPlRelSec[kMaxjet];   //[jet_]
   Float_t         jet_secvESeed[kMaxjet];   //[jet_]
   Float_t         jet_secvPxSeed[kMaxjet];   //[jet_]
   Float_t         jet_secvPySeed[kMaxjet];   //[jet_]
   Float_t         jet_secvPzSeed[kMaxjet];   //[jet_]
   Float_t         jet_secvNLeptons[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseTag[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePass[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseCharge[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseL3d[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseDl3d[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseL2d[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseDl2d[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseLzd[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseDlzd[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseChisq[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseChisqr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseChisqz[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNdtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNvtrk[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseMass[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePt[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePxVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePyVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePzVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseEVtx[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseX[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseY[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseZ[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseXXerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseYYerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseZZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseXYerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseXZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseYZerr[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNPass1Tracks[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNPass2Tracks[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseDz[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePtSec[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePtRelSec[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePlRelSec[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseESeed[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePxSeed[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePySeed[kMaxjet];   //[jet_]
   Float_t         jet_secvLoosePzSeed[kMaxjet];   //[jet_]
   Float_t         jet_secvLooseNLeptons[kMaxjet];   //[jet_]
   Float_t         jet_drB[kMaxjet];   //[jet_]
   Float_t         jet_drC[kMaxjet];   //[jet_]
   Int_t           jet_NTracks[kMaxjet];   //[jet_]
   Int_t           jet_jpbNTracks[kMaxjet];   //[jet_]
   Float_t         jet_jpbJetEt[kMaxjet];   //[jet_]
   Float_t         jet_jpbJetEta[kMaxjet];   //[jet_]
   Float_t         jet_jpbJetPhi[kMaxjet];   //[jet_]
   Int_t           jet_jpbHFTag[kMaxjet];   //[jet_]
   Int_t           jet_jpbHFHepId[kMaxjet];   //[jet_]
   Float_t         jet_jpbHFdR[kMaxjet];   //[jet_]
   Float_t         jet_jpbRPunsign[kMaxjet];   //[jet_]
   Float_t         jet_jpbRPpos[kMaxjet];   //[jet_]
   Float_t         jet_jpbRPneg[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRPTrk[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRPTrkPos[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRPTrkNeg[kMaxjet];   //[jet_]
   Float_t         jet_jpbRZunsign[kMaxjet];   //[jet_]
   Float_t         jet_jpbRZpos[kMaxjet];   //[jet_]
   Float_t         jet_jpbRZneg[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRZTrk[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRZTrkPos[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGdRZTrkNeg[kMaxjet];   //[jet_]
   Float_t         jet_jpb3Dunsign[kMaxjet];   //[jet_]
   Float_t         jet_jpb3Dpos[kMaxjet];   //[jet_]
   Float_t         jet_jpb3Dneg[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGd3DTrk[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGd3DTrkPos[kMaxjet];   //[jet_]
   Int_t           jet_jpbNGd3DTrkNeg[kMaxjet];   //[jet_]
   Int_t           numJetObjs;
   Char_t          jetCollDescr1[40];
   Char_t          jetCollDescr2[40];
   Char_t          jetCollDescr3[40];
   Char_t          jetCollDescr4[40];
   Char_t          jetCollDescr5[40];
   Char_t          jetCollDescr6[40];
   Char_t          jetCollDescr7[40];
   Char_t          jetCollDescr8[40];
   Char_t          jetCollDescr9[40];
   Char_t          jetCollDescr10[40];
   Char_t          jetCollDescr11[40];
   Char_t          jetCollDescr12[40];
   Char_t          jetCollDescr13[40];
   Char_t          jetCollDescr14[40];
   Char_t          jetCollDescr15[40];
   Int_t           met_;
   UInt_t          met_fUniqueID[kMaxmet];   //[met_]
   UInt_t          met_fBits[kMaxmet];   //[met_]
   Float_t         met_Met[kMaxmet];   //[met_]
   Float_t         met_Phi[kMaxmet];   //[met_]
   Float_t         met_ZV[kMaxmet];   //[met_]
   Float_t         met_ESum[kMaxmet];   //[met_]
   Float_t         met_EtSum[kMaxmet];   //[met_]
   Float_t         met_ExSum[kMaxmet];   //[met_]
   Float_t         met_EySum[kMaxmet];   //[met_]
   Int_t           numMetObjs;
   Int_t           topfit_;
   UInt_t          topfit_fUniqueID[kMaxtopfit];   //[topfit_]
   UInt_t          topfit_fBits[kMaxtopfit];   //[topfit_]
   Float_t         topfit_Chi2[kMaxtopfit];   //[topfit_]
   Float_t         topfit_Mass[kMaxtopfit];   //[topfit_]
   Float_t         topfit_ErrPa[kMaxtopfit];   //[topfit_]
   Float_t         topfit_ErrPl[kMaxtopfit];   //[topfit_]
   Float_t         topfit_ErrMi[kMaxtopfit];   //[topfit_]
   Int_t           topfit_config[kMaxtopfit];   //[topfit_]
   Int_t           topfit_tagok[kMaxtopfit];   //[topfit_]
   Float_t         topfit_jettheta[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetphi[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetptraw[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetptgeneric[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetptgg[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetsigma[kMaxtopfit][4];   //[topfit_]
   Float_t         topfit_jetptout[kMaxtopfit][4];   //[topfit_]
   Int_t           numTopFitObjs;
   Int_t           numTopFitBtags;
   Int_t           trigInfo_;
   UInt_t          trigInfo_fUniqueID[kMaxtrigInfo];   //[trigInfo_]
   UInt_t          trigInfo_fBits[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1W1NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1W2NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1W1PS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1W2PS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W1[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W2[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W3[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W4[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W1NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W2NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W3NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L2W4NoPS[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W1[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W2[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W3[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W4[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W5[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L3W6[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1sumetW1[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1sumetW2[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1cal[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1mu[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1trk[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_L1bsc[kMaxtrigInfo];   //[trigInfo_]
   Int_t           trigInfo_summaryWord[kMaxtrigInfo];   //[trigInfo_]
   Char_t          trigBitDescr0[40];
   Char_t          trigBitDescr1[40];
   Char_t          trigBitDescr2[40];
   Char_t          trigBitDescr3[40];
   Char_t          trigBitDescr4[40];
   Char_t          trigBitDescr5[40];
   Char_t          trigBitDescr6[40];
   Char_t          trigBitDescr7[40];
   Char_t          trigBitDescr8[40];
   Char_t          trigBitDescr9[40];
   Int_t           trigName_;
   UInt_t          trigName_fUniqueID[kMaxtrigName];   //[trigName_]
   UInt_t          trigName_fBits[kMaxtrigName];   //[trigName_]
   Int_t           trigName_RunNumber[kMaxtrigName];   //[trigName_]
   Int_t           trigName_NumL1Triggers[kMaxtrigName];   //[trigName_]
   Int_t           trigName_NumL2Triggers[kMaxtrigName];   //[trigName_]
   Int_t           trigName_NumL3Triggers[kMaxtrigName];   //[trigName_]
   Int_t           trigName_L1Bit[kMaxtrigName][64];   //[trigName_]
   Int_t           trigName_L2Bit[kMaxtrigName][128];   //[trigName_]
   Int_t           trigName_L3Bit[kMaxtrigName][256];   //[trigName_]
   Char_t          trigName_L1Name[kMaxtrigName][64][64];   //[trigName_]
   Char_t          trigName_L2Name[kMaxtrigName][128][64];   //[trigName_]
   Char_t          trigName_L3Name[kMaxtrigName][256][64];   //[trigName_]
   Int_t           l2Cal_;
   UInt_t          l2Cal_fUniqueID[kMaxl2Cal];   //[l2Cal_]
   UInt_t          l2Cal_fBits[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_etEmL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_etHadL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_ntowL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_phiL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_etaL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           l2Cal_typL2[kMaxl2Cal];   //[l2Cal_]
   Int_t           numL2CalObjs;
   Int_t           offltrack_;
   UInt_t          offltrack_fUniqueID[kMaxoffltrack];   //[offltrack_]
   UInt_t          offltrack_fBits[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_Alg[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_CollType[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_PassesDefTrkCut[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_ObspId[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_Pt[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_Id[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_IdOrig[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_NumCTHitsAx[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_NumCTHitsSt[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_NumAxSeg[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_NumStSeg[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_NumCTHits[kMaxoffltrack][8];   //[offltrack_]
   Float_t         offltrack_Lambda[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_Curv[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_Z0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_D0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_D0C[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_Phi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_Eta[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_Charge[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_chi2[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_chi2CT[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCP[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCPx[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCPy[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCPz[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCPt[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCEta[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCPhi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_BCZ0[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_BCCharge[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIHits[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIFit[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIGoodHitX[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIGoodHitZ[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIGoodHitZ90[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SIID[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SIPhi[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SID0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SIPt[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SIZ[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SIZsig[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SID0sig[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SICot[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_SIChis[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SITotHitX[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SITotHitZ[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SITotHitZ90[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCotCot[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCotCrv[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCotZ0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCotD0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCotPhi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCrvCrv[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCrvZ0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCrvD0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrCrvPhi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrZ0Z0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrZ0D0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrZ0Phi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrD0D0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrD0Phi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_ErrPhi0Phi0[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_CalEmTower[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_CalHadTower[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_CalEm3x3Iso[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_CalHad3x3Iso[kMaxoffltrack];   //[offltrack_]
   Int_t           offltrack_SiExpectedLayers[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_trkIsol[kMaxoffltrack];   //[offltrack_]
   Float_t         offltrack_corrD0[kMaxoffltrack];   //[offltrack_]
   Int_t           numOfflTracks;
   Char_t          otrkCollDescr1[40];
   Char_t          otrkCollDescr2[40];
   Char_t          otrkCollDescr3[40];
   Char_t          otrkCollDescr4[40];
   Char_t          otrkCollDescr5[40];
   Int_t           secvtxtrack_;
   UInt_t          secvtxtrack_fUniqueID[kMaxsecvtxtrack];   //[secvtxtrack_]
   UInt_t          secvtxtrack_fBits[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_alg[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitx[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitz[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitSz[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitx_ISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitSz_ISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodHitx_L00[kMaxsecvtxtrack];   //[secvtxtrack_]
   UInt_t          secvtxtrack_tId[kMaxsecvtxtrack];   //[secvtxtrack_]
   UInt_t          secvtxtrack_tIdDerived[kMaxsecvtxtrack];   //[secvtxtrack_]
   UInt_t          secvtxtrack_tIdOrig[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_trkLink[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sPt[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tEta[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tCur[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tPhi0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tD0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tZ0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tCot[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tCurE[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tPhi0E[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tD0E[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tZ0E[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tCotE[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sD0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sD0s[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sZ0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sZ0s[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sB0[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sB0s[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_sChi2X[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitX[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitSZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitX_ISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitSZ_ISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sNHitX_L00[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_sType[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_nCotHits[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tKey1[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tKey2[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsGoodTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsPass1Track[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsPass2Track[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsGoodZTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsUsedTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsVeeTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsGoodLooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsPass1LooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsPass2LooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsGoodZLooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsUsedLooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsVeeLooseTrack[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNSharedHits[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tMaxClusterLength[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tMaxClusterLengthPhi[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tMaxClusterLengthZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tMaxClusterLengthSas[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNIntersections[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNGoodLayers[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNErrors[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tHitPattern[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tErrorLayers[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tIntegratedLayers[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tHitPatternZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tErrorLayersZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tIntegratedLayersZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNMissedLayers[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNMissedLayersZ[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNMissedLayersISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNMissedLayersZISL[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tNMissedLayersL00[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tWhyBadISLsz[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tWhyBadISLx[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tWhyBadsz[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tWhyBadz[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tWhyBadx[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsLepton[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsSvt[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsSuggested[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsRequired[kMaxsecvtxtrack];   //[secvtxtrack_]
   UChar_t         secvtxtrack_tIsSkipped[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           secvtxtrack_tGenpIndex[kMaxsecvtxtrack];   //[secvtxtrack_]
   Float_t         secvtxtrack_tCov[kMaxsecvtxtrack][15];   //[secvtxtrack_]
   Int_t           secvtxtrack_is_HFdaughter[kMaxsecvtxtrack];   //[secvtxtrack_]
   Int_t           numSecvtxTracks;
   Int_t           tjassoc_;
   UInt_t          tjassoc_fUniqueID[kMaxtjassoc];   //[tjassoc_]
   UInt_t          tjassoc_fBits[kMaxtjassoc];   //[tjassoc_]
   Short_t         tjassoc_jetid[kMaxtjassoc];   //[tjassoc_]
   Short_t         tjassoc_trkid[kMaxtjassoc];   //[tjassoc_]
   Short_t         tjassoc_acode[kMaxtjassoc];   //[tjassoc_]
   Int_t           numTJAssoc;
   Int_t           tvassoc_;
   UInt_t          tvassoc_fUniqueID[kMaxtvassoc];   //[tvassoc_]
   UInt_t          tvassoc_fBits[kMaxtvassoc];   //[tvassoc_]
   Short_t         tvassoc_vtxid[kMaxtvassoc];   //[tvassoc_]
   Short_t         tvassoc_trkid[kMaxtvassoc];   //[tvassoc_]
   Int_t           numTVAssoc;
   Int_t           jetprobtrack_;
   UInt_t          jetprobtrack_fUniqueID[kMaxjetprobtrack];   //[jetprobtrack_]
   UInt_t          jetprobtrack_fBits[kMaxjetprobtrack];   //[jetprobtrack_]
   UInt_t          jetprobtrack_tId[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tPt[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tTheta[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tEta[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tPhi0[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNSvxLayer[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNSvxHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNSvxPhiHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNSvxStHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNSvxZHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tTotNSvxRPhiHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tTotNGdSvxRPhiHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tD0[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tD0Err[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tSignD0[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tZ0[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tZ0Err[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tSignZ0[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tProbRPhi[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tProbRZ[kMaxjetprobtrack];   //[jetprobtrack_]
   Float_t         jetprobtrack_tProb3D[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNCotHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNCotAxHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tNCotStHit[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tTrackQtyRPhi[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tTrackQtyRZ[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tLongLiveFlagRPhi[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tLongLiveFlagRZ[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tIsGoodRPhi[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           jetprobtrack_tIsGoodRZ[kMaxjetprobtrack];   //[jetprobtrack_]
   Int_t           numJetProbTracks;
   Int_t           jetprobTJassoc_;
   UInt_t          jetprobTJassoc_fUniqueID[kMaxjetprobTJassoc];   //[jetprobTJassoc_]
   UInt_t          jetprobTJassoc_fBits[kMaxjetprobTJassoc];   //[jetprobTJassoc_]
   Short_t         jetprobTJassoc_jetid[kMaxjetprobTJassoc];   //[jetprobTJassoc_]
   Short_t         jetprobTJassoc_trkid[kMaxjetprobTJassoc];   //[jetprobTJassoc_]
   Int_t           numJetProbTJAssoc;
   Int_t           obsp_;
   UInt_t          obsp_fUniqueID[kMaxobsp];   //[obsp_]
   UInt_t          obsp_fBits[kMaxobsp];   //[obsp_]
   Int_t           obsp_index[kMaxobsp];   //[obsp_]
   Int_t           obsp_cdfid[kMaxobsp];   //[obsp_]
   Int_t           obsp_hepglink[kMaxobsp];   //[obsp_]
   Int_t           obsp_dkmode[kMaxobsp];   //[obsp_]
   Double_t        obsp_px[kMaxobsp];   //[obsp_]
   Double_t        obsp_py[kMaxobsp];   //[obsp_]
   Double_t        obsp_pz[kMaxobsp];   //[obsp_]
   Double_t        obsp_m[kMaxobsp];   //[obsp_]
   Double_t        obsp_qcharge[kMaxobsp];   //[obsp_]
   Double_t        obsp_path[kMaxobsp];   //[obsp_]
   Int_t           obsp_motherid[kMaxobsp];   //[obsp_]
   Int_t           obsp_nda[kMaxobsp];   //[obsp_]
   Double_t        obsp_Vx[kMaxobsp];   //[obsp_]
   Double_t        obsp_Vy[kMaxobsp];   //[obsp_]
   Double_t        obsp_Vz[kMaxobsp];   //[obsp_]
   Double_t        obsp_Vt[kMaxobsp];   //[obsp_]
   Int_t           obsp_is_bdaughter[kMaxobsp];   //[obsp_]
   Int_t           obsp_is_cdaughter[kMaxobsp];   //[obsp_]
   Int_t           obsp_beauty[kMaxobsp];   //[obsp_]
   Int_t           obsp_charm[kMaxobsp];   //[obsp_]
   Int_t           obsp_jmo1[kMaxobsp];   //[obsp_]
   Int_t           obsp_jda1[kMaxobsp];   //[obsp_]
   Int_t           obsp_jda2[kMaxobsp];   //[obsp_]
   Int_t           numObspObjs;

//List of branches
   TBranch        *b_evt_;   //!
   TBranch        *b_evt_fUniqueID;   //!
   TBranch        *b_evt_fBits;   //!
   TBranch        *b_evt_runNumber;   //!
   TBranch        *b_evt_eventNumber;   //!
   TBranch        *b_evt_runSection;   //!
   TBranch        *b_evt_CosmicOOTLow;   //!
   TBranch        *b_evt_bunchNum159;   //!
   TBranch        *b_evt_bunchNum36;   //!
   TBranch        *b_evt_gliveInstLumi;   //!
   TBranch        *b_evt_gliveTotalLumi;   //!
   TBranch        *b_evt_instLumi;   //!
   TBranch        *b_evt_totalLumi;   //!
   TBranch        *b_evt_scalerInstLumi;   //!
   TBranch        *b_evt_scalerTotalLumi;   //!
   TBranch        *b_evt_bunchInstLumi;   //!
   TBranch        *b_evt_gliveBunchInstLumi;   //!
   TBranch        *b_evt_bunchTotalInstLumi;   //!
   TBranch        *b_evt_creationTime;   //!
   TBranch        *b_summary_;   //!
   TBranch        *b_summary_fUniqueID;   //!
   TBranch        *b_summary_fBits;   //!
   TBranch        *b_summary_fTopEventClass;   //!
   TBranch        *b_summary_fnTightEle;   //!
   TBranch        *b_summary_fnLooseEle;   //!
   TBranch        *b_summary_fnTightMuo;   //!
   TBranch        *b_summary_fnLooseMuo;   //!
   TBranch        *b_summary_fnTightTau;   //!
   TBranch        *b_summary_fnLooseTau;   //!
   TBranch        *b_summary_fnTightLepton;   //!
   TBranch        *b_summary_fnLooseLepton;   //!
   TBranch        *b_summary_fnTrackLepton;   //!
   TBranch        *b_summary_fnUniqueTrackLepton;   //!
   TBranch        *b_summary_fnUniqueLooseLepton;   //!
   TBranch        *b_summary_fnUniqueDilLepton;   //!
   TBranch        *b_summary_fnTightJet;   //!
   TBranch        *b_summary_fnLooseJet;   //!
   TBranch        *b_summary_frawMet;   //!
   TBranch        *b_summary_frawMetPhi;   //!
   TBranch        *b_summary_fvtxMet;   //!
   TBranch        *b_summary_fvtxMetPhi;   //!
   TBranch        *b_summary_fmuoMet;   //!
   TBranch        *b_summary_fmuoMetPhi;   //!
   TBranch        *b_summary_ftlMet;   //!
   TBranch        *b_summary_ftlMetPhi;   //!
   TBranch        *b_summary_fdilMet;   //!
   TBranch        *b_summary_fdilMetPhi;   //!
   TBranch        *b_summary_ftlJetMet;   //!
   TBranch        *b_summary_ftlJetMetPhi;   //!
   TBranch        *b_summary_fdilJetMet;   //!
   TBranch        *b_summary_fdilJetMetPhi;   //!
   TBranch        *b_summary_fljJetMet;   //!
   TBranch        *b_summary_fljJetMetPhi;   //!
   TBranch        *b_summary_fhadSumEt;   //!
   TBranch        *b_summary_fhadSumEt3;   //!
   TBranch        *b_summary_fhadCentrality;   //!
   TBranch        *b_summary_fhadAplanarity;   //!
   TBranch        *b_summary_fhadSphericity;   //!
   TBranch        *b_summary_fExUnclustered;   //!
   TBranch        *b_summary_fEyUnclustered;   //!
   TBranch        *b_summary_fprimZV;   //!
   TBranch        *b_summary_fjetZV;   //!
   TBranch        *b_summary_fprimZVerr;   //!
   TBranch        *b_summary_fjetZVerr;   //!
   TBranch        *b_summary_fdileptonHT;   //!
   TBranch        *b_summary_flepjetsHT;   //!
   TBranch        *b_privertex_;   //!
   TBranch        *b_privertex_fUniqueID;   //!
   TBranch        *b_privertex_fBits;   //!
   TBranch        *b_privertex_sbm_x0;   //!
   TBranch        *b_privertex_sbm_y0;   //!
   TBranch        *b_privertex_sbm_slx;   //!
   TBranch        *b_privertex_sbm_sly;   //!
   TBranch        *b_privertex_cbm_x0;   //!
   TBranch        *b_privertex_cbm_y0;   //!
   TBranch        *b_privertex_cbm_slx;   //!
   TBranch        *b_privertex_cbm_sly;   //!
   TBranch        *b_privertex_x;   //!
   TBranch        *b_privertex_y;   //!
   TBranch        *b_privertex_z;   //!
   TBranch        *b_privertex_covxx;   //!
   TBranch        *b_privertex_covyy;   //!
   TBranch        *b_privertex_covzz;   //!
   TBranch        *b_privertex_covxy;   //!
   TBranch        *b_privertex_covxz;   //!
   TBranch        *b_privertex_covyz;   //!
   TBranch        *b_privertex_mcx;   //!
   TBranch        *b_privertex_mcy;   //!
   TBranch        *b_privertex_mcz;   //!
   TBranch        *b_privertex_SecVtxVx;   //!
   TBranch        *b_privertex_SecVtxVy;   //!
   TBranch        *b_privertex_SecVtxVz;   //!
   TBranch        *b_privertex_SecVtxC11;   //!
   TBranch        *b_privertex_SecVtxC22;   //!
   TBranch        *b_privertex_SecVtxC33;   //!
   TBranch        *b_privertex_SecVtxC12;   //!
   TBranch        *b_privertex_SecVtxC13;   //!
   TBranch        *b_privertex_SecVtxC23;   //!
   TBranch        *b_privertex_nPrimeVtxTracks;   //!
   TBranch        *b_privertex_primeVtxCharge;   //!
   TBranch        *b_privertex_primeVtxChiSq;   //!
   TBranch        *b_privertex_primeVtxNdof;   //!
   TBranch        *b_zvtxs_;   //!
   TBranch        *b_zvtxs_fUniqueID;   //!
   TBranch        *b_zvtxs_fBits;   //!
   TBranch        *b_zvtxs_z_pos;   //!
   TBranch        *b_zvtxs_z_err;   //!
   TBranch        *b_zvtxs_n_trk;   //!
   TBranch        *b_zvtxs_sum_pt;   //!
   TBranch        *b_zvtxs_quality;   //!
   TBranch        *b_numZVtxs;   //!
   TBranch        *b_hepg_;   //!
   TBranch        *b_hepg_fUniqueID;   //!
   TBranch        *b_hepg_fBits;   //!
   TBranch        *b_hepg_Ind;   //!
   TBranch        *b_hepg_ID;   //!
   TBranch        *b_hepg_IDparent;   //!
   TBranch        *b_hepg_Stdhep;   //!
   TBranch        *b_hepg_Mo1;   //!
   TBranch        *b_hepg_Mo2;   //!
   TBranch        *b_hepg_Da1;   //!
   TBranch        *b_hepg_Da2;   //!
   TBranch        *b_hepg_Charge;   //!
   TBranch        *b_hepg_Px;   //!
   TBranch        *b_hepg_Py;   //!
   TBranch        *b_hepg_Pz;   //!
   TBranch        *b_hepg_E;   //!
   TBranch        *b_hepg_Mass;   //!
   TBranch        *b_numHepgObjs;   //!
   TBranch        *b_muon_;   //!
   TBranch        *b_muon_fUniqueID;   //!
   TBranch        *b_muon_fBits;   //!
   TBranch        *b_muon_ViewType;   //!
   TBranch        *b_muon_muontype;   //!
   TBranch        *b_muon_Fiducial;   //!
   TBranch        *b_muon_trkLink;   //!
   TBranch        *b_muon_TrkFiducial;   //!
   TBranch        *b_muon_CmuFidD;   //!
   TBranch        *b_muon_CmuFidX;   //!
   TBranch        *b_muon_CmuFidZ;   //!
   TBranch        *b_muon_CmpFidD;   //!
   TBranch        *b_muon_CmpFidX;   //!
   TBranch        *b_muon_CmpFidZ;   //!
   TBranch        *b_muon_CmxFidD;   //!
   TBranch        *b_muon_CmxFidX;   //!
   TBranch        *b_muon_CmxFidZ;   //!
   TBranch        *b_muon_inBluebeam;   //!
   TBranch        *b_muon_inMiniskirt;   //!
   TBranch        *b_muon_inKeystone;   //!
   TBranch        *b_muon_En;   //!
   TBranch        *b_muon_P;   //!
   TBranch        *b_muon_Px;   //!
   TBranch        *b_muon_Py;   //!
   TBranch        *b_muon_Pz;   //!
   TBranch        *b_muon_Pt;   //!
   TBranch        *b_muon_Eta;   //!
   TBranch        *b_muon_Phi0;   //!
   TBranch        *b_muon_Z0;   //!
   TBranch        *b_muon_Charge;   //!
   TBranch        *b_muon_D0;   //!
   TBranch        *b_muon_PtCorr;   //!
   TBranch        *b_muon_PxCorr;   //!
   TBranch        *b_muon_PyCorr;   //!
   TBranch        *b_muon_PzCorr;   //!
   TBranch        *b_muon_PCorr;   //!
   TBranch        *b_muon_BCP;   //!
   TBranch        *b_muon_BCPx;   //!
   TBranch        *b_muon_BCPy;   //!
   TBranch        *b_muon_BCPz;   //!
   TBranch        *b_muon_BCPt;   //!
   TBranch        *b_muon_BCEta;   //!
   TBranch        *b_muon_BCPhi0;   //!
   TBranch        *b_muon_BCZ0;   //!
   TBranch        *b_muon_BCCharge;   //!
   TBranch        *b_muon_RawP;   //!
   TBranch        *b_muon_RawPx;   //!
   TBranch        *b_muon_RawPy;   //!
   TBranch        *b_muon_RawPz;   //!
   TBranch        *b_muon_RawPt;   //!
   TBranch        *b_muon_RawEta;   //!
   TBranch        *b_muon_RawPhi0;   //!
   TBranch        *b_muon_RawZ0;   //!
   TBranch        *b_muon_RawCharge;   //!
   TBranch        *b_muon_TrkSiHits;   //!
   TBranch        *b_muon_TrkAxHits;   //!
   TBranch        *b_muon_TrkStHits;   //!
   TBranch        *b_muon_TrkAxSeg;   //!
   TBranch        *b_muon_TrkStSeg;   //!
   TBranch        *b_muon_TrkRedChi2CT;   //!
   TBranch        *b_muon_TrkID;   //!
   TBranch        *b_muon_TrkIDOrig;   //!
   TBranch        *b_muon_HadEnergy;   //!
   TBranch        *b_muon_EmEnergy;   //!
   TBranch        *b_muon_EventZVertex;   //!
   TBranch        *b_muon_DeltaZVertex;   //!
   TBranch        *b_muon_Isol;   //!
   TBranch        *b_muon_EmIsol4;   //!
   TBranch        *b_muon_HadIsol4;   //!
   TBranch        *b_muon_TotalIsol4;   //!
   TBranch        *b_muon_EmIsol7;   //!
   TBranch        *b_muon_HadIsol7;   //!
   TBranch        *b_muon_TotalIsol7;   //!
   TBranch        *b_muon_TrkIsol;   //!
   TBranch        *b_muon_CmuDx;   //!
   TBranch        *b_muon_CmuDz;   //!
   TBranch        *b_muon_CmuDphi;   //!
   TBranch        *b_muon_CmpDx;   //!
   TBranch        *b_muon_CmpDphi;   //!
   TBranch        *b_muon_CmxDx;   //!
   TBranch        *b_muon_CmxDz;   //!
   TBranch        *b_muon_CmxDphi;   //!
   TBranch        *b_muon_BmuDx;   //!
   TBranch        *b_muon_Dx;   //!
   TBranch        *b_muon_Wedge;   //!
   TBranch        *b_muon_Side;   //!
   TBranch        *b_muon_NumAssHits;   //!
   TBranch        *b_muon_CheckEn;   //!
   TBranch        *b_muon_CheckPx;   //!
   TBranch        *b_muon_CheckPy;   //!
   TBranch        *b_muon_CheckPz;   //!
   TBranch        *b_muon_CheckZ0;   //!
   TBranch        *b_muon_CheckZ0Err;   //!
   TBranch        *b_muon_CheckD0;   //!
   TBranch        *b_muon_CheckTrkId;   //!
   TBranch        *b_muon_CutCode;   //!
   TBranch        *b_muon_BruceLLK;   //!
   TBranch        *b_muon_goodForCMXTrig;   //!
   TBranch        *b_muon_Dz;   //!
   TBranch        *b_muon_Dphi;   //!
   TBranch        *b_muon_Dtheta;   //!
   TBranch        *b_muon_ChiSqX;   //!
   TBranch        *b_muon_ChiSqZ;   //!
   TBranch        *b_muon_ChiSqXPos;   //!
   TBranch        *b_muon_ChiSqZPos;   //!
   TBranch        *b_muon_NumCntrHits;   //!
   TBranch        *b_muon_HitPatt;   //!
   TBranch        *b_muon_NumChamberHits;   //!
   TBranch        *b_muon_PosX;   //!
   TBranch        *b_muon_PosY;   //!
   TBranch        *b_muon_PosZ;   //!
   TBranch        *b_muon_DirX;   //!
   TBranch        *b_muon_DirY;   //!
   TBranch        *b_muon_DirZ;   //!
   TBranch        *b_muon_TrkPosX;   //!
   TBranch        *b_muon_TrkPosY;   //!
   TBranch        *b_muon_TrkPosZ;   //!
   TBranch        *b_muon_TrkDirX;   //!
   TBranch        *b_muon_TrkDirY;   //!
   TBranch        *b_muon_TrkDirZ;   //!
   TBranch        *b_numMuObjs;   //!
   TBranch        *b_muoViewDescr1;   //!
   TBranch        *b_muoViewDescr2;   //!
   TBranch        *b_muoViewDescr3;   //!
   TBranch        *b_muoViewDescr4;   //!
   TBranch        *b_muoViewDescr5;   //!
   TBranch        *b_sltmu_;   //!
   TBranch        *b_sltmu_fUniqueID;   //!
   TBranch        *b_sltmu_fBits;   //!
   TBranch        *b_sltmu_jetid;   //!
   TBranch        *b_sltmu_trkid;   //!
   TBranch        *b_sltmu_muid;   //!
   TBranch        *b_sltmu_jetCollType;   //!
   TBranch        *b_sltmu_dRclstJet;   //!
   TBranch        *b_sltmu_ptRel;   //!
   TBranch        *b_sltmu_sumPt06;   //!
   TBranch        *b_sltmu_nTrkJet06;   //!
   TBranch        *b_sltmu_sumPt06C1;   //!
   TBranch        *b_sltmu_nTrkJet06C1;   //!
   TBranch        *b_sltmu_sumPt06C2;   //!
   TBranch        *b_sltmu_nTrkJet06C2;   //!
   TBranch        *b_sltmu_sumPt04;   //!
   TBranch        *b_sltmu_nTrkJet04;   //!
   TBranch        *b_sltmu_sumPt04C1;   //!
   TBranch        *b_sltmu_nTrkJet04C1;   //!
   TBranch        *b_sltmu_sumPt04C2;   //!
   TBranch        *b_sltmu_nTrkJet04C2;   //!
   TBranch        *b_sltmu_sumPt02;   //!
   TBranch        *b_sltmu_nTrkJet02;   //!
   TBranch        *b_sltmu_sumPt02C1;   //!
   TBranch        *b_sltmu_nTrkJet02C1;   //!
   TBranch        *b_sltmu_sumPt02C2;   //!
   TBranch        *b_sltmu_nTrkJet02C2;   //!
   TBranch        *b_sltmu_emE;   //!
   TBranch        *b_sltmu_hadE;   //!
   TBranch        *b_sltmu_neightborEmE;   //!
   TBranch        *b_sltmu_neightborHadE;   //!
   TBranch        *b_sltmu_coneR4Et;   //!
   TBranch        *b_sltmu_fiducial;   //!
   TBranch        *b_sltmu_cmuFidSig;   //!
   TBranch        *b_sltmu_cmpFidSig;   //!
   TBranch        *b_sltmu_cmxFidSig;   //!
   TBranch        *b_sltmu_fiducialSig;   //!
   TBranch        *b_sltmu_isTaggable;   //!
   TBranch        *b_sltmu_isTagged;   //!
   TBranch        *b_sltmu_nUsedVar;   //!
   TBranch        *b_sltmu_usedVar;   //!
   TBranch        *b_sltmu_likelihood;   //!
   TBranch        *b_sltmu_pullCmuDx;   //!
   TBranch        *b_sltmu_pullCmuDz;   //!
   TBranch        *b_sltmu_pullCmuDphi;   //!
   TBranch        *b_sltmu_pullCmuHits;   //!
   TBranch        *b_sltmu_pullCmuChi2X;   //!
   TBranch        *b_sltmu_pullCmuChi2Z;   //!
   TBranch        *b_sltmu_pullCmpDx;   //!
   TBranch        *b_sltmu_pullCmpDphi;   //!
   TBranch        *b_sltmu_pullCmpChi2X;   //!
   TBranch        *b_sltmu_pullCmxDx;   //!
   TBranch        *b_sltmu_pullCmxDz;   //!
   TBranch        *b_sltmu_pullCmxDphi;   //!
   TBranch        *b_sltmu_pullCmxChi2X;   //!
   TBranch        *b_sltmu_pullCmxChi2Z;   //!
   TBranch        *b_sltmu_phiCot;   //!
   TBranch        *b_sltmu_etaCot;   //!
   TBranch        *b_sltmu_radLengths;   //!
   TBranch        *b_sltmu_absLengths;   //!
   TBranch        *b_sltmu_eLoss;   //!
   TBranch        *b_sltmu_extPosX;   //!
   TBranch        *b_sltmu_extPosY;   //!
   TBranch        *b_sltmu_extPosZ;   //!
   TBranch        *b_sltmu_extDirX;   //!
   TBranch        *b_sltmu_extDirY;   //!
   TBranch        *b_sltmu_extDirZ;   //!
   TBranch        *b_sltmu_dist;   //!
   TBranch        *b_numSltMuObjs;   //!
   TBranch        *b_cosmic_;   //!
   TBranch        *b_cosmic_fUniqueID;   //!
   TBranch        *b_cosmic_fBits;   //!
   TBranch        *b_cosmic_cosmicBit;   //!
   TBranch        *b_cosmic_hasCosmicRay;   //!
   TBranch        *b_cosmic_hasFewTracks;   //!
   TBranch        *b_cosmic_hasLargeD0;   //!
   TBranch        *b_cosmic_hasLargeZ0;   //!
   TBranch        *b_cosmic_hasAcolTrack;   //!
   TBranch        *b_cosmic_hasOOTEnergy;   //!
   TBranch        *b_cosmic_hasOOTScintillators;   //!
   TBranch        *b_cosmic_hasNoVertex;   //!
   TBranch        *b_cosmic_hasLargeDz;   //!
   TBranch        *b_cosmic_hasLargeDPhiMuStub;   //!
   TBranch        *b_cosmic_isMuonNothing;   //!
   TBranch        *b_cosmic_isMuonStub;   //!
   TBranch        *b_cosmic_isMuonTrack;   //!
   TBranch        *b_cosmic_isMuonMuon;   //!
   TBranch        *b_cosmic_hasOOTTof;   //!
   TBranch        *b_cosmic_hasNotOutgoingPair;   //!
   TBranch        *b_cosmic_hasLowEmFrac;   //!
   TBranch        *b_cosmic_hasLowQFrac;   //!
   TBranch        *b_cosmic_hasOOTCaloE;   //!
   TBranch        *b_cosmic_nLegs;   //!
   TBranch        *b_cosmic_deltaTdc;   //!
   TBranch        *b_cosmic_deltaTof;   //!
   TBranch        *b_cosmic_dPhiMuStub;   //!
   TBranch        *b_cosmic_acollinearity;   //!
   TBranch        *b_cosmic_d0Rho;   //!
   TBranch        *b_cosmic_d0Beta;   //!
   TBranch        *b_cosmic_dPhi;   //!
   TBranch        *b_cosmic_sumEta;   //!
   TBranch        *b_cosmic_seedZ;   //!
   TBranch        *b_cosmic_seedZErr;   //!
   TBranch        *b_cosmic_nTrkZVtx;   //!
   TBranch        *b_cosmic_meanZVtx;   //!
   TBranch        *b_cosmic_meanZVtxErr;   //!
   TBranch        *b_cosmic_muoLnk;   //!
   TBranch        *b_cosmic_trkLnk;   //!
   TBranch        *b_cosmic_trackId;   //!
   TBranch        *b_cosmic_pt;   //!
   TBranch        *b_cosmic_crv;   //!
   TBranch        *b_cosmic_cot;   //!
   TBranch        *b_cosmic_phi;   //!
   TBranch        *b_cosmic_eta;   //!
   TBranch        *b_cosmic_z0;   //!
   TBranch        *b_cosmic_d0;   //!
   TBranch        *b_cosmic_d0C;   //!
   TBranch        *b_cosmic_charge;   //!
   TBranch        *b_cosmic_algo;   //!
   TBranch        *b_cosmic_deltaT0;   //!
   TBranch        *b_cosmic_errorDeltaT0;   //!
   TBranch        *b_cosmic_deltaBetaInv;   //!
   TBranch        *b_cosmic_errorDeltaBetaInv;   //!
   TBranch        *b_cosmic_fitT0;   //!
   TBranch        *b_cosmic_fitdT0;   //!
   TBranch        *b_cosmic_fitCrdT0;   //!
   TBranch        *b_cosmic_fitChi2;   //!
   TBranch        *b_cosmic_fitdChi2;   //!
   TBranch        *b_cosmic_fitDir;   //!
   TBranch        *b_cosmic_fitNCOTH;   //!
   TBranch        *b_cosmic_fitNCOTHExp;   //!
   TBranch        *b_cosmic_emE;   //!
   TBranch        *b_cosmic_hadE;   //!
   TBranch        *b_cosmic_hadTDC;   //!
   TBranch        *b_cosmic_ngbrEmE;   //!
   TBranch        *b_cosmic_ngbrHadE;   //!
   TBranch        *b_cosmic_ngbrHadTDC;   //!
   TBranch        *b_cosmic_stubPhi;   //!
   TBranch        *b_cosmic_tof;   //!
   TBranch        *b_cosmic_tofMethod;   //!
   TBranch        *b_cosmic_csp;   //!
   TBranch        *b_cosmic_csx;   //!
   TBranch        *b_cosmic_validLeg;   //!
   TBranch        *b_cosmic_validTrack;   //!
   TBranch        *b_cosmic_validStub;   //!
   TBranch        *b_cosmic_validEnergy;   //!
   TBranch        *b_cosmic_validHadTdc;   //!
   TBranch        *b_cosmic_validTof;   //!
   TBranch        *b_cosmic_validCsp;   //!
   TBranch        *b_cosmic_validCsx;   //!
   TBranch        *b_cosmic_trackLessMuon;   //!
   TBranch        *b_cosmic_stubLessMuon;   //!
   TBranch        *b_electron_;   //!
   TBranch        *b_electron_fUniqueID;   //!
   TBranch        *b_electron_fBits;   //!
   TBranch        *b_electron_CollType;   //!
   TBranch        *b_electron_Region;   //!
   TBranch        *b_electron_Conversion;   //!
   TBranch        *b_electron_trkLink;   //!
   TBranch        *b_electron_phxTrkLink;   //!
   TBranch        *b_electron_En;   //!
   TBranch        *b_electron_RawEn;   //!
   TBranch        *b_electron_CluEn;   //!
   TBranch        *b_electron_Px;   //!
   TBranch        *b_electron_RawPx;   //!
   TBranch        *b_electron_CluPx;   //!
   TBranch        *b_electron_Py;   //!
   TBranch        *b_electron_RawPy;   //!
   TBranch        *b_electron_CluPy;   //!
   TBranch        *b_electron_Pz;   //!
   TBranch        *b_electron_RawPz;   //!
   TBranch        *b_electron_CluPz;   //!
   TBranch        *b_electron_Et;   //!
   TBranch        *b_electron_RawEt;   //!
   TBranch        *b_electron_CluEt;   //!
   TBranch        *b_electron_Phi;   //!
   TBranch        *b_electron_RawPhi;   //!
   TBranch        *b_electron_CluPhi;   //!
   TBranch        *b_electron_Eta;   //!
   TBranch        *b_electron_RawEta;   //!
   TBranch        *b_electron_CluEta;   //!
   TBranch        *b_electron_Clu_totEt_jetZ;   //!
   TBranch        *b_electron_Clu_phi_jetZ;   //!
   TBranch        *b_electron_TrkSiHits;   //!
   TBranch        *b_electron_TrkAxHits;   //!
   TBranch        *b_electron_TrkStHits;   //!
   TBranch        *b_electron_TrkAxSeg;   //!
   TBranch        *b_electron_TrkStSeg;   //!
   TBranch        *b_electron_TrkP;   //!
   TBranch        *b_electron_BCP;   //!
   TBranch        *b_electron_RawTrkP;   //!
   TBranch        *b_electron_TrkPx;   //!
   TBranch        *b_electron_BCPx;   //!
   TBranch        *b_electron_RawTrkPx;   //!
   TBranch        *b_electron_TrkPy;   //!
   TBranch        *b_electron_BCPy;   //!
   TBranch        *b_electron_RawTrkPy;   //!
   TBranch        *b_electron_TrkPz;   //!
   TBranch        *b_electron_BCPz;   //!
   TBranch        *b_electron_RawTrkPz;   //!
   TBranch        *b_electron_TrkPt;   //!
   TBranch        *b_electron_BCPt;   //!
   TBranch        *b_electron_RawTrkPt;   //!
   TBranch        *b_electron_TrkEta;   //!
   TBranch        *b_electron_BCEta;   //!
   TBranch        *b_electron_RawTrkEta;   //!
   TBranch        *b_electron_TrkPhi0;   //!
   TBranch        *b_electron_BCPhi0;   //!
   TBranch        *b_electron_RawTrkPhi0;   //!
   TBranch        *b_electron_TrkZ0;   //!
   TBranch        *b_electron_BCZ0;   //!
   TBranch        *b_electron_RawTrkZ0;   //!
   TBranch        *b_electron_Charge;   //!
   TBranch        *b_electron_BCCharge;   //!
   TBranch        *b_electron_RawTrkCharge;   //!
   TBranch        *b_electron_TrkD0;   //!
   TBranch        *b_electron_TrkID;   //!
   TBranch        *b_electron_TrkIDOrig;   //!
   TBranch        *b_electron_Fiducial;   //!
   TBranch        *b_electron_TrkFiducial;   //!
   TBranch        *b_electron_EtaDet;   //!
   TBranch        *b_electron_EtaEvent;   //!
   TBranch        *b_electron_HadEt;   //!
   TBranch        *b_electron_TotalEt;   //!
   TBranch        *b_electron_Hadem;   //!
   TBranch        *b_electron_EmEt;   //!
   TBranch        *b_electron_HadEn;   //!
   TBranch        *b_electron_TotalEn;   //!
   TBranch        *b_electron_TotalIsol4;   //!
   TBranch        *b_electron_TotalIsol7;   //!
   TBranch        *b_electron_EmIsol4;   //!
   TBranch        *b_electron_EmIsol7;   //!
   TBranch        *b_electron_HadIsol4;   //!
   TBranch        *b_electron_HadIsol7;   //!
   TBranch        *b_electron_PprEn;   //!
   TBranch        *b_electron_Pem3x3FitTow;   //!
   TBranch        *b_electron_Pem3x3DetEta;   //!
   TBranch        *b_electron_Pem3x3Phi;   //!
   TBranch        *b_electron_Pem3x3FitEn;   //!
   TBranch        *b_electron_Pem3x3Chisq;   //!
   TBranch        *b_electron_NumPes2d;   //!
   TBranch        *b_electron_Pes2dEta;   //!
   TBranch        *b_electron_Pes2dPhi;   //!
   TBranch        *b_electron_Pes2dX;   //!
   TBranch        *b_electron_Pes2dY;   //!
   TBranch        *b_electron_Pes2d5by9U;   //!
   TBranch        *b_electron_Pes2d5by9V;   //!
   TBranch        *b_electron_Pes2dEn;   //!
   TBranch        *b_electron_ClosestPes2d5by9U;   //!
   TBranch        *b_electron_ClosestPes2d5by9V;   //!
   TBranch        *b_electron_ClosestPes2dEn;   //!
   TBranch        *b_electron_LshrTrk;   //!
   TBranch        *b_electron_Lshr3;   //!
   TBranch        *b_electron_Lshr2;   //!
   TBranch        *b_electron_Isol;   //!
   TBranch        *b_electron_LeakE;   //!
   TBranch        *b_electron_EP;   //!
   TBranch        *b_electron_EventZVertex;   //!
   TBranch        *b_electron_DeltaZVertex;   //!
   TBranch        *b_electron_TrkCESx;   //!
   TBranch        *b_electron_BestCESx;   //!
   TBranch        *b_electron_DeltaX;   //!
   TBranch        *b_electron_TrkCESz;   //!
   TBranch        *b_electron_BestCESz;   //!
   TBranch        *b_electron_DeltaZ;   //!
   TBranch        *b_electron_StripChi2;   //!
   TBranch        *b_electron_WireChi2;   //!
   TBranch        *b_electron_TrkIsol;   //!
   TBranch        *b_electron_Towers;   //!
   TBranch        *b_electron_Tracks;   //!
   TBranch        *b_electron_SeedEta;   //!
   TBranch        *b_electron_SeedPhi;   //!
   TBranch        *b_electron_SeedEn;   //!
   TBranch        *b_electron_SeedEt;   //!
   TBranch        *b_electron_PhoZV;   //!
   TBranch        *b_electron_PhoRawEn;   //!
   TBranch        *b_electron_PhoRawEt;   //!
   TBranch        *b_electron_PhoEt;   //!
   TBranch        *b_electron_PhoPhi;   //!
   TBranch        *b_electron_PhoEta;   //!
   TBranch        *b_electron_PhoCESx;   //!
   TBranch        *b_electron_PhoCESz;   //!
   TBranch        *b_electron_PhoEneStrip;   //!
   TBranch        *b_electron_PhoEneWire;   //!
   TBranch        *b_electron_PhoStripChi2;   //!
   TBranch        *b_electron_PhoWireChi2;   //!
   TBranch        *b_electron_PhoCES2x;   //!
   TBranch        *b_electron_PhoCES2z;   //!
   TBranch        *b_electron_PhoEneStrip2;   //!
   TBranch        *b_electron_PhoEneWire2;   //!
   TBranch        *b_electron_PhoCesSlide;   //!
   TBranch        *b_electron_PhoN3D;   //!
   TBranch        *b_electron_PhoPt;   //!
   TBranch        *b_electron_PhoIsol;   //!
   TBranch        *b_electron_PhoTrkIsol;   //!
   TBranch        *b_electron_PhoLshr;   //!
   TBranch        *b_electron_PhoHadem;   //!
   TBranch        *b_electron_RawEnergyWire;   //!
   TBranch        *b_electron_RawEnergyStrip;   //!
   TBranch        *b_electron_FitEnergyWire;   //!
   TBranch        *b_electron_FitEnergyStrip;   //!
   TBranch        *b_electron_CESside;   //!
   TBranch        *b_electron_CESmodule;   //!
   TBranch        *b_electron_TrkCESside;   //!
   TBranch        *b_electron_TrkCESmodule;   //!
   TBranch        *b_electron_CPR;   //!
   TBranch        *b_electron_CPRside;   //!
   TBranch        *b_electron_CPRmodule;   //!
   TBranch        *b_electron_TrkCPRside;   //!
   TBranch        *b_electron_TrkCPRmodule;   //!
   TBranch        *b_electron_CPRx;   //!
   TBranch        *b_electron_DeltaXCPR;   //!
   TBranch        *b_electron_RawEnergyCPR;   //!
   TBranch        *b_electron_TrkCPRx;   //!
   TBranch        *b_electron_TrkCPRz;   //!
   TBranch        *b_electron_TrkCPRphi;   //!
   TBranch        *b_electron_TrkCPReta;   //!
   TBranch        *b_electron_WidthWire;   //!
   TBranch        *b_electron_WidthStrip;   //!
   TBranch        *b_electron_CheckEn;   //!
   TBranch        *b_electron_CheckPx;   //!
   TBranch        *b_electron_CheckPy;   //!
   TBranch        *b_electron_CheckPz;   //!
   TBranch        *b_electron_CheckZ0;   //!
   TBranch        *b_electron_CheckZ0Err;   //!
   TBranch        *b_electron_CheckConv;   //!
   TBranch        *b_electron_CheckTrkId;   //!
   TBranch        *b_electron_CutCode;   //!
   TBranch        *b_electron_PhxTrkSiHits;   //!
   TBranch        *b_electron_PhxTrkP;   //!
   TBranch        *b_electron_PhxTrkPx;   //!
   TBranch        *b_electron_PhxTrkPy;   //!
   TBranch        *b_electron_PhxTrkPz;   //!
   TBranch        *b_electron_PhxTrkPt;   //!
   TBranch        *b_electron_PhxTrkEta;   //!
   TBranch        *b_electron_PhxTrkPhi0;   //!
   TBranch        *b_electron_PhxTrkZ0;   //!
   TBranch        *b_electron_PhxCharge;   //!
   TBranch        *b_electron_PhxTrkD0;   //!
   TBranch        *b_electron_PhxMatch;   //!
   TBranch        *b_electron_PhxCotMatchID;   //!
   TBranch        *b_electron_PhxDrTrkPes;   //!
   TBranch        *b_electron_BruceLLK;   //!
   TBranch        *b_numEmObjs;   //!
   TBranch        *b_eleViewDescr1;   //!
   TBranch        *b_eleViewDescr2;   //!
   TBranch        *b_eleViewDescr3;   //!
   TBranch        *b_eleViewDescr4;   //!
   TBranch        *b_eleViewDescr5;   //!
   TBranch        *b_tau_;   //!
   TBranch        *b_tau_fUniqueID;   //!
   TBranch        *b_tau_fBits;   //!
   TBranch        *b_tau_ntowers;   //!
   TBranch        *b_tau_nclumptowers;   //!
   TBranch        *b_tau_ntrks;   //!
   TBranch        *b_tau_ntrks10deg;   //!
   TBranch        *b_tau_ntrks30deg;   //!
   TBranch        *b_tau_ntrks10to30deg;   //!
   TBranch        *b_tau_charge10deg;   //!
   TBranch        *b_tau_nwrongvtxtrk10deg;   //!
   TBranch        *b_tau_npi0s;   //!
   TBranch        *b_tau_nmustubs;   //!
   TBranch        *b_tau_nmuhits;   //!
   TBranch        *b_tau_px;   //!
   TBranch        *b_tau_py;   //!
   TBranch        *b_tau_pz;   //!
   TBranch        *b_tau_e;   //!
   TBranch        *b_tau_pxtrk;   //!
   TBranch        *b_tau_pytrk;   //!
   TBranch        *b_tau_pztrk;   //!
   TBranch        *b_tau_etrk;   //!
   TBranch        *b_tau_pxvis;   //!
   TBranch        *b_tau_pyvis;   //!
   TBranch        *b_tau_pzvis;   //!
   TBranch        *b_tau_evis;   //!
   TBranch        *b_tau_emfrac;   //!
   TBranch        *b_tau_clusterdeteta;   //!
   TBranch        *b_tau_seedtrkdeteta;   //!
   TBranch        *b_tau_clustereta;   //!
   TBranch        *b_tau_clusterphi;   //!
   TBranch        *b_tau_etaeta;   //!
   TBranch        *b_tau_phiphi;   //!
   TBranch        *b_tau_delr;   //!
   TBranch        *b_tau_emenergy;   //!
   TBranch        *b_tau_hadenergy;   //!
   TBranch        *b_tau_emet;   //!
   TBranch        *b_tau_hadet;   //!
   TBranch        *b_tau_caloetiso;   //!
   TBranch        *b_tau_calohadetiso;   //!
   TBranch        *b_tau_caloemetiso;   //!
   TBranch        *b_tau_refvtxz;   //!
   TBranch        *b_tau_trackspt;   //!
   TBranch        *b_tau_trkscalarptsum;   //!
   TBranch        *b_tau_tracksmass;   //!
   TBranch        *b_tau_trackseta;   //!
   TBranch        *b_tau_tracksphi;   //!
   TBranch        *b_tau_trackiso;   //!
   TBranch        *b_tau_seedtrkvz;   //!
   TBranch        *b_tau_seedtrkpt;   //!
   TBranch        *b_tau_angleseedtocluster;   //!
   TBranch        *b_tau_phiseedtocluster;   //!
   TBranch        *b_tau_etaseedtocluster;   //!
   TBranch        *b_tau_pi0trackmass;   //!
   TBranch        *b_tau_pi0trackpt;   //!
   TBranch        *b_tau_trkindex;   //!
   TBranch        *b_tau_et;   //!
   TBranch        *b_tau_ep;   //!
   TBranch        *b_tau_ep_vis;   //!
   TBranch        *b_tau_zces;   //!
   TBranch        *b_tau_d0;   //!
   TBranch        *b_tau_tracklinks;   //!
   TBranch        *b_tau_tracknaxialhits;   //!
   TBranch        *b_tau_tracknstereohits;   //!
   TBranch        *b_tau_cutword;   //!
   TBranch        *b_tau_conesize;   //!
   TBranch        *b_tau_ehadoverp;   //!
   TBranch        *b_tau_calomass;   //!
   TBranch        *b_tau_ntracksincone;   //!
   TBranch        *b_tau_chargeincone;   //!
   TBranch        *b_tau_nisotracks;   //!
   TBranch        *b_tau_nisopi0s;   //!
   TBranch        *b_tau_taugrouplikes;   //!
   TBranch        *b_tau_pi0eta;   //!
   TBranch        *b_tau_pi0phi;   //!
   TBranch        *b_tau_pi0e;   //!
   TBranch        *b_tau_pi0px;   //!
   TBranch        *b_tau_pi0py;   //!
   TBranch        *b_tau_pi0pz;   //!
   TBranch        *b_tau_pi0pt;   //!
   TBranch        *b_tau_pi0detectorz;   //!
   TBranch        *b_tau_pi0strip1rawenergy;   //!
   TBranch        *b_tau_pi0strip1fitpos;   //!
   TBranch        *b_tau_pi0strip1width;   //!
   TBranch        *b_tau_pi0strip1chi2;   //!
   TBranch        *b_tau_pi0strip2rawenergy;   //!
   TBranch        *b_tau_pi0strip2fitpos;   //!
   TBranch        *b_tau_pi0strip2width;   //!
   TBranch        *b_tau_pi0strip2chi2;   //!
   TBranch        *b_tau_pi0wire1rawenergy;   //!
   TBranch        *b_tau_pi0wire1fitpos;   //!
   TBranch        *b_tau_pi0wire1width;   //!
   TBranch        *b_tau_pi0wire1chi2;   //!
   TBranch        *b_tau_pi0wire2rawenergy;   //!
   TBranch        *b_tau_pi0wire2fitpos;   //!
   TBranch        *b_tau_pi0wire2width;   //!
   TBranch        *b_tau_pi0wire2chi2;   //!
   TBranch        *b_numTauObjs;   //!
   TBranch        *b_jet_;   //!
   TBranch        *b_jet_fUniqueID;   //!
   TBranch        *b_jet_fBits;   //!
   TBranch        *b_jet_CollType;   //!
   TBranch        *b_jet_Px;   //!
   TBranch        *b_jet_Py;   //!
   TBranch        *b_jet_Pz;   //!
   TBranch        *b_jet_En;   //!
   TBranch        *b_jet_EtaDetector;   //!
   TBranch        *b_jet_GuardEnergy;   //!
   TBranch        *b_jet_EmFraction;   //!
   TBranch        *b_jet_CentroidEta;   //!
   TBranch        *b_jet_CentroidPhi;   //!
   TBranch        *b_jet_EtaMoment;   //!
   TBranch        *b_jet_PhiMoment;   //!
   TBranch        *b_jet_EtaPhiMoment;   //!
   TBranch        *b_jet_CentroidEt;   //!
   TBranch        *b_jet_TotP;   //!
   TBranch        *b_jet_TotPt;   //!
   TBranch        *b_jet_Pt;   //!
   TBranch        *b_jet_PtSquared;   //!
   TBranch        *b_jet_TotEt;   //!
   TBranch        *b_jet_Et;   //!
   TBranch        *b_jet_Eta;   //!
   TBranch        *b_jet_Theta;   //!
   TBranch        *b_jet_Phi;   //!
   TBranch        *b_jet_MassSquared;   //!
   TBranch        *b_jet_Mass;   //!
   TBranch        *b_jet_Rapidity;   //!
   TBranch        *b_jet_secvTag;   //!
   TBranch        *b_jet_secvPass;   //!
   TBranch        *b_jet_secvCharge;   //!
   TBranch        *b_jet_secvL3d;   //!
   TBranch        *b_jet_secvDl3d;   //!
   TBranch        *b_jet_secvL2d;   //!
   TBranch        *b_jet_secvDl2d;   //!
   TBranch        *b_jet_secvLzd;   //!
   TBranch        *b_jet_secvDlzd;   //!
   TBranch        *b_jet_secvChisq;   //!
   TBranch        *b_jet_secvChisqr;   //!
   TBranch        *b_jet_secvChisqz;   //!
   TBranch        *b_jet_secvNtrk;   //!
   TBranch        *b_jet_secvNdtrk;   //!
   TBranch        *b_jet_secvNvtrk;   //!
   TBranch        *b_jet_secvMass;   //!
   TBranch        *b_jet_secvPt;   //!
   TBranch        *b_jet_secvPxVtx;   //!
   TBranch        *b_jet_secvPyVtx;   //!
   TBranch        *b_jet_secvPzVtx;   //!
   TBranch        *b_jet_secvEVtx;   //!
   TBranch        *b_jet_secvX;   //!
   TBranch        *b_jet_secvY;   //!
   TBranch        *b_jet_secvZ;   //!
   TBranch        *b_jet_secvXXerr;   //!
   TBranch        *b_jet_secvYYerr;   //!
   TBranch        *b_jet_secvZZerr;   //!
   TBranch        *b_jet_secvXYerr;   //!
   TBranch        *b_jet_secvXZerr;   //!
   TBranch        *b_jet_secvYZerr;   //!
   TBranch        *b_jet_secvNPass1Tracks;   //!
   TBranch        *b_jet_secvNPass2Tracks;   //!
   TBranch        *b_jet_secvDz;   //!
   TBranch        *b_jet_secvPtSec;   //!
   TBranch        *b_jet_secvPtRelSec;   //!
   TBranch        *b_jet_secvPlRelSec;   //!
   TBranch        *b_jet_secvESeed;   //!
   TBranch        *b_jet_secvPxSeed;   //!
   TBranch        *b_jet_secvPySeed;   //!
   TBranch        *b_jet_secvPzSeed;   //!
   TBranch        *b_jet_secvNLeptons;   //!
   TBranch        *b_jet_secvLooseTag;   //!
   TBranch        *b_jet_secvLoosePass;   //!
   TBranch        *b_jet_secvLooseCharge;   //!
   TBranch        *b_jet_secvLooseL3d;   //!
   TBranch        *b_jet_secvLooseDl3d;   //!
   TBranch        *b_jet_secvLooseL2d;   //!
   TBranch        *b_jet_secvLooseDl2d;   //!
   TBranch        *b_jet_secvLooseLzd;   //!
   TBranch        *b_jet_secvLooseDlzd;   //!
   TBranch        *b_jet_secvLooseChisq;   //!
   TBranch        *b_jet_secvLooseChisqr;   //!
   TBranch        *b_jet_secvLooseChisqz;   //!
   TBranch        *b_jet_secvLooseNtrk;   //!
   TBranch        *b_jet_secvLooseNdtrk;   //!
   TBranch        *b_jet_secvLooseNvtrk;   //!
   TBranch        *b_jet_secvLooseMass;   //!
   TBranch        *b_jet_secvLoosePt;   //!
   TBranch        *b_jet_secvLoosePxVtx;   //!
   TBranch        *b_jet_secvLoosePyVtx;   //!
   TBranch        *b_jet_secvLoosePzVtx;   //!
   TBranch        *b_jet_secvLooseEVtx;   //!
   TBranch        *b_jet_secvLooseX;   //!
   TBranch        *b_jet_secvLooseY;   //!
   TBranch        *b_jet_secvLooseZ;   //!
   TBranch        *b_jet_secvLooseXXerr;   //!
   TBranch        *b_jet_secvLooseYYerr;   //!
   TBranch        *b_jet_secvLooseZZerr;   //!
   TBranch        *b_jet_secvLooseXYerr;   //!
   TBranch        *b_jet_secvLooseXZerr;   //!
   TBranch        *b_jet_secvLooseYZerr;   //!
   TBranch        *b_jet_secvLooseNPass1Tracks;   //!
   TBranch        *b_jet_secvLooseNPass2Tracks;   //!
   TBranch        *b_jet_secvLooseDz;   //!
   TBranch        *b_jet_secvLoosePtSec;   //!
   TBranch        *b_jet_secvLoosePtRelSec;   //!
   TBranch        *b_jet_secvLoosePlRelSec;   //!
   TBranch        *b_jet_secvLooseESeed;   //!
   TBranch        *b_jet_secvLoosePxSeed;   //!
   TBranch        *b_jet_secvLoosePySeed;   //!
   TBranch        *b_jet_secvLoosePzSeed;   //!
   TBranch        *b_jet_secvLooseNLeptons;   //!
   TBranch        *b_jet_drB;   //!
   TBranch        *b_jet_drC;   //!
   TBranch        *b_jet_NTracks;   //!
   TBranch        *b_jet_jpbNTracks;   //!
   TBranch        *b_jet_jpbJetEt;   //!
   TBranch        *b_jet_jpbJetEta;   //!
   TBranch        *b_jet_jpbJetPhi;   //!
   TBranch        *b_jet_jpbHFTag;   //!
   TBranch        *b_jet_jpbHFHepId;   //!
   TBranch        *b_jet_jpbHFdR;   //!
   TBranch        *b_jet_jpbRPunsign;   //!
   TBranch        *b_jet_jpbRPpos;   //!
   TBranch        *b_jet_jpbRPneg;   //!
   TBranch        *b_jet_jpbNGdRPTrk;   //!
   TBranch        *b_jet_jpbNGdRPTrkPos;   //!
   TBranch        *b_jet_jpbNGdRPTrkNeg;   //!
   TBranch        *b_jet_jpbRZunsign;   //!
   TBranch        *b_jet_jpbRZpos;   //!
   TBranch        *b_jet_jpbRZneg;   //!
   TBranch        *b_jet_jpbNGdRZTrk;   //!
   TBranch        *b_jet_jpbNGdRZTrkPos;   //!
   TBranch        *b_jet_jpbNGdRZTrkNeg;   //!
   TBranch        *b_jet_jpb3Dunsign;   //!
   TBranch        *b_jet_jpb3Dpos;   //!
   TBranch        *b_jet_jpb3Dneg;   //!
   TBranch        *b_jet_jpbNGd3DTrk;   //!
   TBranch        *b_jet_jpbNGd3DTrkPos;   //!
   TBranch        *b_jet_jpbNGd3DTrkNeg;   //!
   TBranch        *b_numJetObjs;   //!
   TBranch        *b_jetCollDescr1;   //!
   TBranch        *b_jetCollDescr2;   //!
   TBranch        *b_jetCollDescr3;   //!
   TBranch        *b_jetCollDescr4;   //!
   TBranch        *b_jetCollDescr5;   //!
   TBranch        *b_jetCollDescr6;   //!
   TBranch        *b_jetCollDescr7;   //!
   TBranch        *b_jetCollDescr8;   //!
   TBranch        *b_jetCollDescr9;   //!
   TBranch        *b_jetCollDescr10;   //!
   TBranch        *b_jetCollDescr11;   //!
   TBranch        *b_jetCollDescr12;   //!
   TBranch        *b_jetCollDescr13;   //!
   TBranch        *b_jetCollDescr14;   //!
   TBranch        *b_jetCollDescr15;   //!
   TBranch        *b_met_;   //!
   TBranch        *b_met_fUniqueID;   //!
   TBranch        *b_met_fBits;   //!
   TBranch        *b_met_Met;   //!
   TBranch        *b_met_Phi;   //!
   TBranch        *b_met_ZV;   //!
   TBranch        *b_met_ESum;   //!
   TBranch        *b_met_EtSum;   //!
   TBranch        *b_met_ExSum;   //!
   TBranch        *b_met_EySum;   //!
   TBranch        *b_numMetObjs;   //!
   TBranch        *b_topfit_;   //!
   TBranch        *b_topfit_fUniqueID;   //!
   TBranch        *b_topfit_fBits;   //!
   TBranch        *b_topfit_Chi2;   //!
   TBranch        *b_topfit_Mass;   //!
   TBranch        *b_topfit_ErrPa;   //!
   TBranch        *b_topfit_ErrPl;   //!
   TBranch        *b_topfit_ErrMi;   //!
   TBranch        *b_topfit_config;   //!
   TBranch        *b_topfit_tagok;   //!
   TBranch        *b_topfit_jettheta;   //!
   TBranch        *b_topfit_jetphi;   //!
   TBranch        *b_topfit_jetptraw;   //!
   TBranch        *b_topfit_jetptgeneric;   //!
   TBranch        *b_topfit_jetptgg;   //!
   TBranch        *b_topfit_jetsigma;   //!
   TBranch        *b_topfit_jetptout;   //!
   TBranch        *b_numTopFitObjs;   //!
   TBranch        *b_numTopFitBtags;   //!
   TBranch        *b_trigInfo_;   //!
   TBranch        *b_trigInfo_fUniqueID;   //!
   TBranch        *b_trigInfo_fBits;   //!
   TBranch        *b_trigInfo_L1W1NoPS;   //!
   TBranch        *b_trigInfo_L1W2NoPS;   //!
   TBranch        *b_trigInfo_L1W1PS;   //!
   TBranch        *b_trigInfo_L1W2PS;   //!
   TBranch        *b_trigInfo_L2W1;   //!
   TBranch        *b_trigInfo_L2W2;   //!
   TBranch        *b_trigInfo_L2W3;   //!
   TBranch        *b_trigInfo_L2W4;   //!
   TBranch        *b_trigInfo_L2W1NoPS;   //!
   TBranch        *b_trigInfo_L2W2NoPS;   //!
   TBranch        *b_trigInfo_L2W3NoPS;   //!
   TBranch        *b_trigInfo_L2W4NoPS;   //!
   TBranch        *b_trigInfo_L3W1;   //!
   TBranch        *b_trigInfo_L3W2;   //!
   TBranch        *b_trigInfo_L3W3;   //!
   TBranch        *b_trigInfo_L3W4;   //!
   TBranch        *b_trigInfo_L3W5;   //!
   TBranch        *b_trigInfo_L3W6;   //!
   TBranch        *b_trigInfo_L1sumetW1;   //!
   TBranch        *b_trigInfo_L1sumetW2;   //!
   TBranch        *b_trigInfo_L1cal;   //!
   TBranch        *b_trigInfo_L1mu;   //!
   TBranch        *b_trigInfo_L1trk;   //!
   TBranch        *b_trigInfo_L1bsc;   //!
   TBranch        *b_trigInfo_summaryWord;   //!
   TBranch        *b_trigBitDescr0;   //!
   TBranch        *b_trigBitDescr1;   //!
   TBranch        *b_trigBitDescr2;   //!
   TBranch        *b_trigBitDescr3;   //!
   TBranch        *b_trigBitDescr4;   //!
   TBranch        *b_trigBitDescr5;   //!
   TBranch        *b_trigBitDescr6;   //!
   TBranch        *b_trigBitDescr7;   //!
   TBranch        *b_trigBitDescr8;   //!
   TBranch        *b_trigBitDescr9;   //!
   TBranch        *b_trigName_;   //!
   TBranch        *b_trigName_fUniqueID;   //!
   TBranch        *b_trigName_fBits;   //!
   TBranch        *b_trigName_RunNumber;   //!
   TBranch        *b_trigName_NumL1Triggers;   //!
   TBranch        *b_trigName_NumL2Triggers;   //!
   TBranch        *b_trigName_NumL3Triggers;   //!
   TBranch        *b_trigName_L1Bit;   //!
   TBranch        *b_trigName_L2Bit;   //!
   TBranch        *b_trigName_L3Bit;   //!
   TBranch        *b_trigName_L1Name;   //!
   TBranch        *b_trigName_L2Name;   //!
   TBranch        *b_trigName_L3Name;   //!
   TBranch        *b_l2Cal_;   //!
   TBranch        *b_l2Cal_fUniqueID;   //!
   TBranch        *b_l2Cal_fBits;   //!
   TBranch        *b_l2Cal_etEmL2;   //!
   TBranch        *b_l2Cal_etHadL2;   //!
   TBranch        *b_l2Cal_ntowL2;   //!
   TBranch        *b_l2Cal_phiL2;   //!
   TBranch        *b_l2Cal_etaL2;   //!
   TBranch        *b_l2Cal_typL2;   //!
   TBranch        *b_numL2CalObjs;   //!
   TBranch        *b_offltrack_;   //!
   TBranch        *b_offltrack_fUniqueID;   //!
   TBranch        *b_offltrack_fBits;   //!
   TBranch        *b_offltrack_Alg;   //!
   TBranch        *b_offltrack_CollType;   //!
   TBranch        *b_offltrack_PassesDefTrkCut;   //!
   TBranch        *b_offltrack_ObspId;   //!
   TBranch        *b_offltrack_Pt;   //!
   TBranch        *b_offltrack_Id;   //!
   TBranch        *b_offltrack_IdOrig;   //!
   TBranch        *b_offltrack_NumCTHitsAx;   //!
   TBranch        *b_offltrack_NumCTHitsSt;   //!
   TBranch        *b_offltrack_NumAxSeg;   //!
   TBranch        *b_offltrack_NumStSeg;   //!
   TBranch        *b_offltrack_NumCTHits;   //!
   TBranch        *b_offltrack_Lambda;   //!
   TBranch        *b_offltrack_Curv;   //!
   TBranch        *b_offltrack_Z0;   //!
   TBranch        *b_offltrack_D0;   //!
   TBranch        *b_offltrack_D0C;   //!
   TBranch        *b_offltrack_Phi0;   //!
   TBranch        *b_offltrack_Eta;   //!
   TBranch        *b_offltrack_Charge;   //!
   TBranch        *b_offltrack_chi2;   //!
   TBranch        *b_offltrack_chi2CT;   //!
   TBranch        *b_offltrack_BCP;   //!
   TBranch        *b_offltrack_BCPx;   //!
   TBranch        *b_offltrack_BCPy;   //!
   TBranch        *b_offltrack_BCPz;   //!
   TBranch        *b_offltrack_BCPt;   //!
   TBranch        *b_offltrack_BCEta;   //!
   TBranch        *b_offltrack_BCPhi0;   //!
   TBranch        *b_offltrack_BCZ0;   //!
   TBranch        *b_offltrack_BCCharge;   //!
   TBranch        *b_offltrack_SIHits;   //!
   TBranch        *b_offltrack_SIFit;   //!
   TBranch        *b_offltrack_SIGoodHitX;   //!
   TBranch        *b_offltrack_SIGoodHitZ;   //!
   TBranch        *b_offltrack_SIGoodHitZ90;   //!
   TBranch        *b_offltrack_SIID;   //!
   TBranch        *b_offltrack_SIPhi;   //!
   TBranch        *b_offltrack_SID0;   //!
   TBranch        *b_offltrack_SIPt;   //!
   TBranch        *b_offltrack_SIZ;   //!
   TBranch        *b_offltrack_SIZsig;   //!
   TBranch        *b_offltrack_SID0sig;   //!
   TBranch        *b_offltrack_SICot;   //!
   TBranch        *b_offltrack_SIChis;   //!
   TBranch        *b_offltrack_SITotHitX;   //!
   TBranch        *b_offltrack_SITotHitZ;   //!
   TBranch        *b_offltrack_SITotHitZ90;   //!
   TBranch        *b_offltrack_ErrCotCot;   //!
   TBranch        *b_offltrack_ErrCotCrv;   //!
   TBranch        *b_offltrack_ErrCotZ0;   //!
   TBranch        *b_offltrack_ErrCotD0;   //!
   TBranch        *b_offltrack_ErrCotPhi0;   //!
   TBranch        *b_offltrack_ErrCrvCrv;   //!
   TBranch        *b_offltrack_ErrCrvZ0;   //!
   TBranch        *b_offltrack_ErrCrvD0;   //!
   TBranch        *b_offltrack_ErrCrvPhi0;   //!
   TBranch        *b_offltrack_ErrZ0Z0;   //!
   TBranch        *b_offltrack_ErrZ0D0;   //!
   TBranch        *b_offltrack_ErrZ0Phi0;   //!
   TBranch        *b_offltrack_ErrD0D0;   //!
   TBranch        *b_offltrack_ErrD0Phi0;   //!
   TBranch        *b_offltrack_ErrPhi0Phi0;   //!
   TBranch        *b_offltrack_CalEmTower;   //!
   TBranch        *b_offltrack_CalHadTower;   //!
   TBranch        *b_offltrack_CalEm3x3Iso;   //!
   TBranch        *b_offltrack_CalHad3x3Iso;   //!
   TBranch        *b_offltrack_SiExpectedLayers;   //!
   TBranch        *b_offltrack_trkIsol;   //!
   TBranch        *b_offltrack_corrD0;   //!
   TBranch        *b_numOfflTracks;   //!
   TBranch        *b_otrkCollDescr1;   //!
   TBranch        *b_otrkCollDescr2;   //!
   TBranch        *b_otrkCollDescr3;   //!
   TBranch        *b_otrkCollDescr4;   //!
   TBranch        *b_otrkCollDescr5;   //!
   TBranch        *b_secvtxtrack_;   //!
   TBranch        *b_secvtxtrack_fUniqueID;   //!
   TBranch        *b_secvtxtrack_fBits;   //!
   TBranch        *b_secvtxtrack_alg;   //!
   TBranch        *b_secvtxtrack_tNGoodHitx;   //!
   TBranch        *b_secvtxtrack_tNGoodHitz;   //!
   TBranch        *b_secvtxtrack_tNGoodHitSz;   //!
   TBranch        *b_secvtxtrack_tNGoodHitx_ISL;   //!
   TBranch        *b_secvtxtrack_tNGoodHitSz_ISL;   //!
   TBranch        *b_secvtxtrack_tNGoodHitx_L00;   //!
   TBranch        *b_secvtxtrack_tId;   //!
   TBranch        *b_secvtxtrack_tIdDerived;   //!
   TBranch        *b_secvtxtrack_tIdOrig;   //!
   TBranch        *b_secvtxtrack_trkLink;   //!
   TBranch        *b_secvtxtrack_sPt;   //!
   TBranch        *b_secvtxtrack_tEta;   //!
   TBranch        *b_secvtxtrack_tCur;   //!
   TBranch        *b_secvtxtrack_tPhi0;   //!
   TBranch        *b_secvtxtrack_tD0;   //!
   TBranch        *b_secvtxtrack_tZ0;   //!
   TBranch        *b_secvtxtrack_tCot;   //!
   TBranch        *b_secvtxtrack_tCurE;   //!
   TBranch        *b_secvtxtrack_tPhi0E;   //!
   TBranch        *b_secvtxtrack_tD0E;   //!
   TBranch        *b_secvtxtrack_tZ0E;   //!
   TBranch        *b_secvtxtrack_tCotE;   //!
   TBranch        *b_secvtxtrack_sD0;   //!
   TBranch        *b_secvtxtrack_sD0s;   //!
   TBranch        *b_secvtxtrack_sZ0;   //!
   TBranch        *b_secvtxtrack_sZ0s;   //!
   TBranch        *b_secvtxtrack_sB0;   //!
   TBranch        *b_secvtxtrack_sB0s;   //!
   TBranch        *b_secvtxtrack_sChi2X;   //!
   TBranch        *b_secvtxtrack_sNHitX;   //!
   TBranch        *b_secvtxtrack_sNHitSZ;   //!
   TBranch        *b_secvtxtrack_sNHitZ;   //!
   TBranch        *b_secvtxtrack_sNHitX_ISL;   //!
   TBranch        *b_secvtxtrack_sNHitSZ_ISL;   //!
   TBranch        *b_secvtxtrack_sNHitX_L00;   //!
   TBranch        *b_secvtxtrack_sType;   //!
   TBranch        *b_secvtxtrack_nCotHits;   //!
   TBranch        *b_secvtxtrack_tKey1;   //!
   TBranch        *b_secvtxtrack_tKey2;   //!
   TBranch        *b_secvtxtrack_tIsGoodTrack;   //!
   TBranch        *b_secvtxtrack_tIsPass1Track;   //!
   TBranch        *b_secvtxtrack_tIsPass2Track;   //!
   TBranch        *b_secvtxtrack_tIsGoodZTrack;   //!
   TBranch        *b_secvtxtrack_tIsUsedTrack;   //!
   TBranch        *b_secvtxtrack_tIsVeeTrack;   //!
   TBranch        *b_secvtxtrack_tIsGoodLooseTrack;   //!
   TBranch        *b_secvtxtrack_tIsPass1LooseTrack;   //!
   TBranch        *b_secvtxtrack_tIsPass2LooseTrack;   //!
   TBranch        *b_secvtxtrack_tIsGoodZLooseTrack;   //!
   TBranch        *b_secvtxtrack_tIsUsedLooseTrack;   //!
   TBranch        *b_secvtxtrack_tIsVeeLooseTrack;   //!
   TBranch        *b_secvtxtrack_tNSharedHits;   //!
   TBranch        *b_secvtxtrack_tMaxClusterLength;   //!
   TBranch        *b_secvtxtrack_tMaxClusterLengthPhi;   //!
   TBranch        *b_secvtxtrack_tMaxClusterLengthZ;   //!
   TBranch        *b_secvtxtrack_tMaxClusterLengthSas;   //!
   TBranch        *b_secvtxtrack_tNIntersections;   //!
   TBranch        *b_secvtxtrack_tNGoodLayers;   //!
   TBranch        *b_secvtxtrack_tNErrors;   //!
   TBranch        *b_secvtxtrack_tHitPattern;   //!
   TBranch        *b_secvtxtrack_tErrorLayers;   //!
   TBranch        *b_secvtxtrack_tIntegratedLayers;   //!
   TBranch        *b_secvtxtrack_tHitPatternZ;   //!
   TBranch        *b_secvtxtrack_tErrorLayersZ;   //!
   TBranch        *b_secvtxtrack_tIntegratedLayersZ;   //!
   TBranch        *b_secvtxtrack_tNMissedLayers;   //!
   TBranch        *b_secvtxtrack_tNMissedLayersZ;   //!
   TBranch        *b_secvtxtrack_tNMissedLayersISL;   //!
   TBranch        *b_secvtxtrack_tNMissedLayersZISL;   //!
   TBranch        *b_secvtxtrack_tNMissedLayersL00;   //!
   TBranch        *b_secvtxtrack_tWhyBadISLsz;   //!
   TBranch        *b_secvtxtrack_tWhyBadISLx;   //!
   TBranch        *b_secvtxtrack_tWhyBadsz;   //!
   TBranch        *b_secvtxtrack_tWhyBadz;   //!
   TBranch        *b_secvtxtrack_tWhyBadx;   //!
   TBranch        *b_secvtxtrack_tIsLepton;   //!
   TBranch        *b_secvtxtrack_tIsSvt;   //!
   TBranch        *b_secvtxtrack_tIsSuggested;   //!
   TBranch        *b_secvtxtrack_tIsRequired;   //!
   TBranch        *b_secvtxtrack_tIsSkipped;   //!
   TBranch        *b_secvtxtrack_tGenpIndex;   //!
   TBranch        *b_secvtxtrack_tCov;   //!
   TBranch        *b_secvtxtrack_is_HFdaughter;   //!
   TBranch        *b_numSecvtxTracks;   //!
   TBranch        *b_tjassoc_;   //!
   TBranch        *b_tjassoc_fUniqueID;   //!
   TBranch        *b_tjassoc_fBits;   //!
   TBranch        *b_tjassoc_jetid;   //!
   TBranch        *b_tjassoc_trkid;   //!
   TBranch        *b_tjassoc_acode;   //!
   TBranch        *b_numTJAssoc;   //!
   TBranch        *b_tvassoc_;   //!
   TBranch        *b_tvassoc_fUniqueID;   //!
   TBranch        *b_tvassoc_fBits;   //!
   TBranch        *b_tvassoc_vtxid;   //!
   TBranch        *b_tvassoc_trkid;   //!
   TBranch        *b_numTVAssoc;   //!
   TBranch        *b_jetprobtrack_;   //!
   TBranch        *b_jetprobtrack_fUniqueID;   //!
   TBranch        *b_jetprobtrack_fBits;   //!
   TBranch        *b_jetprobtrack_tId;   //!
   TBranch        *b_jetprobtrack_tPt;   //!
   TBranch        *b_jetprobtrack_tTheta;   //!
   TBranch        *b_jetprobtrack_tEta;   //!
   TBranch        *b_jetprobtrack_tPhi0;   //!
   TBranch        *b_jetprobtrack_tNSvxLayer;   //!
   TBranch        *b_jetprobtrack_tNSvxHit;   //!
   TBranch        *b_jetprobtrack_tNSvxPhiHit;   //!
   TBranch        *b_jetprobtrack_tNSvxStHit;   //!
   TBranch        *b_jetprobtrack_tNSvxZHit;   //!
   TBranch        *b_jetprobtrack_tTotNSvxRPhiHit;   //!
   TBranch        *b_jetprobtrack_tTotNGdSvxRPhiHit;   //!
   TBranch        *b_jetprobtrack_tD0;   //!
   TBranch        *b_jetprobtrack_tD0Err;   //!
   TBranch        *b_jetprobtrack_tSignD0;   //!
   TBranch        *b_jetprobtrack_tZ0;   //!
   TBranch        *b_jetprobtrack_tZ0Err;   //!
   TBranch        *b_jetprobtrack_tSignZ0;   //!
   TBranch        *b_jetprobtrack_tProbRPhi;   //!
   TBranch        *b_jetprobtrack_tProbRZ;   //!
   TBranch        *b_jetprobtrack_tProb3D;   //!
   TBranch        *b_jetprobtrack_tNCotHit;   //!
   TBranch        *b_jetprobtrack_tNCotAxHit;   //!
   TBranch        *b_jetprobtrack_tNCotStHit;   //!
   TBranch        *b_jetprobtrack_tTrackQtyRPhi;   //!
   TBranch        *b_jetprobtrack_tTrackQtyRZ;   //!
   TBranch        *b_jetprobtrack_tLongLiveFlagRPhi;   //!
   TBranch        *b_jetprobtrack_tLongLiveFlagRZ;   //!
   TBranch        *b_jetprobtrack_tIsGoodRPhi;   //!
   TBranch        *b_jetprobtrack_tIsGoodRZ;   //!
   TBranch        *b_numJetProbTracks;   //!
   TBranch        *b_jetprobTJassoc_;   //!
   TBranch        *b_jetprobTJassoc_fUniqueID;   //!
   TBranch        *b_jetprobTJassoc_fBits;   //!
   TBranch        *b_jetprobTJassoc_jetid;   //!
   TBranch        *b_jetprobTJassoc_trkid;   //!
   TBranch        *b_numJetProbTJAssoc;   //!
   TBranch        *b_obsp_;   //!
   TBranch        *b_obsp_fUniqueID;   //!
   TBranch        *b_obsp_fBits;   //!
   TBranch        *b_obsp_index;   //!
   TBranch        *b_obsp_cdfid;   //!
   TBranch        *b_obsp_hepglink;   //!
   TBranch        *b_obsp_dkmode;   //!
   TBranch        *b_obsp_px;   //!
   TBranch        *b_obsp_py;   //!
   TBranch        *b_obsp_pz;   //!
   TBranch        *b_obsp_m;   //!
   TBranch        *b_obsp_qcharge;   //!
   TBranch        *b_obsp_path;   //!
   TBranch        *b_obsp_motherid;   //!
   TBranch        *b_obsp_nda;   //!
   TBranch        *b_obsp_Vx;   //!
   TBranch        *b_obsp_Vy;   //!
   TBranch        *b_obsp_Vz;   //!
   TBranch        *b_obsp_Vt;   //!
   TBranch        *b_obsp_is_bdaughter;   //!
   TBranch        *b_obsp_is_cdaughter;   //!
   TBranch        *b_obsp_beauty;   //!
   TBranch        *b_obsp_charm;   //!
   TBranch        *b_obsp_jmo1;   //!
   TBranch        *b_obsp_jda1;   //!
   TBranch        *b_obsp_jda2;   //!
   TBranch        *b_numObspObjs;   //!

   SkimFCNC(TTree *tree=0);
   ~SkimFCNC();
   Int_t  Cut(Int_t entry);
   Int_t  GetEntry(Int_t entry);
   Int_t  LoadTree(Int_t entry);
   void   Init(TTree *tree);
   // CLP start      
   //void   Loop();
   void   Loop(int numwanted = 0, bool debug = false);
   // CLP end
   Bool_t Notify();
   void   Show(Int_t entry = -1);
   // CLP start
#include "FromSkimFCNC.h"
   // CLP end
};

#endif

#ifdef SkimFCNC_cxx
SkimFCNC::SkimFCNC(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/cdf/scratch/zaw/fcnc/topnt_nt5_5.3.3test_tcZeeMC-Inclusive.root");
      if (!f) {
         f = new TFile("/cdf/scratch/zaw/fcnc/topnt_nt5_5.3.3test_tcZeeMC-Inclusive.root");
      }
      tree = (TTree*)gDirectory->Get("TopTree");

   }
   Init(tree);
}

SkimFCNC::~SkimFCNC()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SkimFCNC::GetEntry(Int_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Int_t SkimFCNC::LoadTree(Int_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Int_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SkimFCNC::Init(TTree *tree)
{
//   Set branch addresses
   if (tree == 0) return;
   fChain    = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt",&evt_);
   fChain->SetBranchAddress("evt.fUniqueID",evt_fUniqueID);
   fChain->SetBranchAddress("evt.fBits",evt_fBits);
   fChain->SetBranchAddress("evt.runNumber",evt_runNumber);
   fChain->SetBranchAddress("evt.eventNumber",evt_eventNumber);
   fChain->SetBranchAddress("evt.runSection",evt_runSection);
   fChain->SetBranchAddress("evt.CosmicOOTLow",evt_CosmicOOTLow);
   fChain->SetBranchAddress("evt.bunchNum159",evt_bunchNum159);
   fChain->SetBranchAddress("evt.bunchNum36",evt_bunchNum36);
   fChain->SetBranchAddress("evt.gliveInstLumi",evt_gliveInstLumi);
   fChain->SetBranchAddress("evt.gliveTotalLumi",evt_gliveTotalLumi);
   fChain->SetBranchAddress("evt.instLumi",evt_instLumi);
   fChain->SetBranchAddress("evt.totalLumi",evt_totalLumi);
   fChain->SetBranchAddress("evt.scalerInstLumi",evt_scalerInstLumi);
   fChain->SetBranchAddress("evt.scalerTotalLumi",evt_scalerTotalLumi);
   fChain->SetBranchAddress("evt.bunchInstLumi",evt_bunchInstLumi);
   fChain->SetBranchAddress("evt.gliveBunchInstLumi",evt_gliveBunchInstLumi);
   fChain->SetBranchAddress("evt.bunchTotalInstLumi",evt_bunchTotalInstLumi);
   fChain->SetBranchAddress("evt.creationTime",evt_creationTime);
   fChain->SetBranchAddress("summary",&summary_);
   fChain->SetBranchAddress("summary.fUniqueID",summary_fUniqueID);
   fChain->SetBranchAddress("summary.fBits",summary_fBits);
   fChain->SetBranchAddress("summary.fTopEventClass",summary_fTopEventClass);
   fChain->SetBranchAddress("summary.fnTightEle",summary_fnTightEle);
   fChain->SetBranchAddress("summary.fnLooseEle",summary_fnLooseEle);
   fChain->SetBranchAddress("summary.fnTightMuo",summary_fnTightMuo);
   fChain->SetBranchAddress("summary.fnLooseMuo",summary_fnLooseMuo);
   fChain->SetBranchAddress("summary.fnTightTau",summary_fnTightTau);
   fChain->SetBranchAddress("summary.fnLooseTau",summary_fnLooseTau);
   fChain->SetBranchAddress("summary.fnTightLepton",summary_fnTightLepton);
   fChain->SetBranchAddress("summary.fnLooseLepton",summary_fnLooseLepton);
   fChain->SetBranchAddress("summary.fnTrackLepton",summary_fnTrackLepton);
   fChain->SetBranchAddress("summary.fnUniqueTrackLepton",summary_fnUniqueTrackLepton);
   fChain->SetBranchAddress("summary.fnUniqueLooseLepton",summary_fnUniqueLooseLepton);
   fChain->SetBranchAddress("summary.fnUniqueDilLepton",summary_fnUniqueDilLepton);
   fChain->SetBranchAddress("summary.fnTightJet",summary_fnTightJet);
   fChain->SetBranchAddress("summary.fnLooseJet",summary_fnLooseJet);
   fChain->SetBranchAddress("summary.frawMet",summary_frawMet);
   fChain->SetBranchAddress("summary.frawMetPhi",summary_frawMetPhi);
   fChain->SetBranchAddress("summary.fvtxMet",summary_fvtxMet);
   fChain->SetBranchAddress("summary.fvtxMetPhi",summary_fvtxMetPhi);
   fChain->SetBranchAddress("summary.fmuoMet",summary_fmuoMet);
   fChain->SetBranchAddress("summary.fmuoMetPhi",summary_fmuoMetPhi);
   fChain->SetBranchAddress("summary.ftlMet",summary_ftlMet);
   fChain->SetBranchAddress("summary.ftlMetPhi",summary_ftlMetPhi);
   fChain->SetBranchAddress("summary.fdilMet",summary_fdilMet);
   fChain->SetBranchAddress("summary.fdilMetPhi",summary_fdilMetPhi);
   fChain->SetBranchAddress("summary.ftlJetMet",summary_ftlJetMet);
   fChain->SetBranchAddress("summary.ftlJetMetPhi",summary_ftlJetMetPhi);
   fChain->SetBranchAddress("summary.fdilJetMet",summary_fdilJetMet);
   fChain->SetBranchAddress("summary.fdilJetMetPhi",summary_fdilJetMetPhi);
   fChain->SetBranchAddress("summary.fljJetMet",summary_fljJetMet);
   fChain->SetBranchAddress("summary.fljJetMetPhi",summary_fljJetMetPhi);
   fChain->SetBranchAddress("summary.fhadSumEt",summary_fhadSumEt);
   fChain->SetBranchAddress("summary.fhadSumEt3",summary_fhadSumEt3);
   fChain->SetBranchAddress("summary.fhadCentrality",summary_fhadCentrality);
   fChain->SetBranchAddress("summary.fhadAplanarity",summary_fhadAplanarity);
   fChain->SetBranchAddress("summary.fhadSphericity",summary_fhadSphericity);
   fChain->SetBranchAddress("summary.fExUnclustered",summary_fExUnclustered);
   fChain->SetBranchAddress("summary.fEyUnclustered",summary_fEyUnclustered);
   fChain->SetBranchAddress("summary.fprimZV",summary_fprimZV);
   fChain->SetBranchAddress("summary.fjetZV",summary_fjetZV);
   fChain->SetBranchAddress("summary.fprimZVerr",summary_fprimZVerr);
   fChain->SetBranchAddress("summary.fjetZVerr",summary_fjetZVerr);
   fChain->SetBranchAddress("summary.fdileptonHT",summary_fdileptonHT);
   fChain->SetBranchAddress("summary.flepjetsHT",summary_flepjetsHT);
   fChain->SetBranchAddress("privertex",&privertex_);
   fChain->SetBranchAddress("privertex.fUniqueID",privertex_fUniqueID);
   fChain->SetBranchAddress("privertex.fBits",privertex_fBits);
   fChain->SetBranchAddress("privertex.sbm_x0",privertex_sbm_x0);
   fChain->SetBranchAddress("privertex.sbm_y0",privertex_sbm_y0);
   fChain->SetBranchAddress("privertex.sbm_slx",privertex_sbm_slx);
   fChain->SetBranchAddress("privertex.sbm_sly",privertex_sbm_sly);
   fChain->SetBranchAddress("privertex.cbm_x0",privertex_cbm_x0);
   fChain->SetBranchAddress("privertex.cbm_y0",privertex_cbm_y0);
   fChain->SetBranchAddress("privertex.cbm_slx",privertex_cbm_slx);
   fChain->SetBranchAddress("privertex.cbm_sly",privertex_cbm_sly);
   fChain->SetBranchAddress("privertex.x",privertex_x);
   fChain->SetBranchAddress("privertex.y",privertex_y);
   fChain->SetBranchAddress("privertex.z",privertex_z);
   fChain->SetBranchAddress("privertex.covxx",privertex_covxx);
   fChain->SetBranchAddress("privertex.covyy",privertex_covyy);
   fChain->SetBranchAddress("privertex.covzz",privertex_covzz);
   fChain->SetBranchAddress("privertex.covxy",privertex_covxy);
   fChain->SetBranchAddress("privertex.covxz",privertex_covxz);
   fChain->SetBranchAddress("privertex.covyz",privertex_covyz);
   fChain->SetBranchAddress("privertex.mcx",privertex_mcx);
   fChain->SetBranchAddress("privertex.mcy",privertex_mcy);
   fChain->SetBranchAddress("privertex.mcz",privertex_mcz);
   fChain->SetBranchAddress("privertex.SecVtxVx",privertex_SecVtxVx);
   fChain->SetBranchAddress("privertex.SecVtxVy",privertex_SecVtxVy);
   fChain->SetBranchAddress("privertex.SecVtxVz",privertex_SecVtxVz);
   fChain->SetBranchAddress("privertex.SecVtxC11",privertex_SecVtxC11);
   fChain->SetBranchAddress("privertex.SecVtxC22",privertex_SecVtxC22);
   fChain->SetBranchAddress("privertex.SecVtxC33",privertex_SecVtxC33);
   fChain->SetBranchAddress("privertex.SecVtxC12",privertex_SecVtxC12);
   fChain->SetBranchAddress("privertex.SecVtxC13",privertex_SecVtxC13);
   fChain->SetBranchAddress("privertex.SecVtxC23",privertex_SecVtxC23);
   fChain->SetBranchAddress("privertex.nPrimeVtxTracks",privertex_nPrimeVtxTracks);
   fChain->SetBranchAddress("privertex.primeVtxCharge",privertex_primeVtxCharge);
   fChain->SetBranchAddress("privertex.primeVtxChiSq",privertex_primeVtxChiSq);
   fChain->SetBranchAddress("privertex.primeVtxNdof",privertex_primeVtxNdof);
   fChain->SetBranchAddress("zvtxs",&zvtxs_);
   fChain->SetBranchAddress("zvtxs.fUniqueID",zvtxs_fUniqueID);
   fChain->SetBranchAddress("zvtxs.fBits",zvtxs_fBits);
   fChain->SetBranchAddress("zvtxs.z_pos",zvtxs_z_pos);
   fChain->SetBranchAddress("zvtxs.z_err",zvtxs_z_err);
   fChain->SetBranchAddress("zvtxs.n_trk",zvtxs_n_trk);
   fChain->SetBranchAddress("zvtxs.sum_pt",zvtxs_sum_pt);
   fChain->SetBranchAddress("zvtxs.quality",zvtxs_quality);
   fChain->SetBranchAddress("numZVtxs",&numZVtxs);
   fChain->SetBranchAddress("hepg",&hepg_);
   fChain->SetBranchAddress("hepg.fUniqueID",hepg_fUniqueID);
   fChain->SetBranchAddress("hepg.fBits",hepg_fBits);
   fChain->SetBranchAddress("hepg.Ind",hepg_Ind);
   fChain->SetBranchAddress("hepg.ID",hepg_ID);
   fChain->SetBranchAddress("hepg.IDparent",hepg_IDparent);
   fChain->SetBranchAddress("hepg.Stdhep",hepg_Stdhep);
   fChain->SetBranchAddress("hepg.Mo1",hepg_Mo1);
   fChain->SetBranchAddress("hepg.Mo2",hepg_Mo2);
   fChain->SetBranchAddress("hepg.Da1",hepg_Da1);
   fChain->SetBranchAddress("hepg.Da2",hepg_Da2);
   fChain->SetBranchAddress("hepg.Charge",hepg_Charge);
   fChain->SetBranchAddress("hepg.Px",hepg_Px);
   fChain->SetBranchAddress("hepg.Py",hepg_Py);
   fChain->SetBranchAddress("hepg.Pz",hepg_Pz);
   fChain->SetBranchAddress("hepg.E",hepg_E);
   fChain->SetBranchAddress("hepg.Mass",hepg_Mass);
   fChain->SetBranchAddress("numHepgObjs",&numHepgObjs);
   fChain->SetBranchAddress("muon",&muon_);
   fChain->SetBranchAddress("muon.fUniqueID",muon_fUniqueID);
   fChain->SetBranchAddress("muon.fBits",muon_fBits);
   fChain->SetBranchAddress("muon.ViewType",muon_ViewType);
   fChain->SetBranchAddress("muon.muontype",muon_muontype);
   fChain->SetBranchAddress("muon.Fiducial",muon_Fiducial);
   fChain->SetBranchAddress("muon.trkLink",muon_trkLink);
   fChain->SetBranchAddress("muon.TrkFiducial",muon_TrkFiducial);
   fChain->SetBranchAddress("muon.CmuFidD",muon_CmuFidD);
   fChain->SetBranchAddress("muon.CmuFidX",muon_CmuFidX);
   fChain->SetBranchAddress("muon.CmuFidZ",muon_CmuFidZ);
   fChain->SetBranchAddress("muon.CmpFidD",muon_CmpFidD);
   fChain->SetBranchAddress("muon.CmpFidX",muon_CmpFidX);
   fChain->SetBranchAddress("muon.CmpFidZ",muon_CmpFidZ);
   fChain->SetBranchAddress("muon.CmxFidD",muon_CmxFidD);
   fChain->SetBranchAddress("muon.CmxFidX",muon_CmxFidX);
   fChain->SetBranchAddress("muon.CmxFidZ",muon_CmxFidZ);
   fChain->SetBranchAddress("muon.inBluebeam",muon_inBluebeam);
   fChain->SetBranchAddress("muon.inMiniskirt",muon_inMiniskirt);
   fChain->SetBranchAddress("muon.inKeystone",muon_inKeystone);
   fChain->SetBranchAddress("muon.En",muon_En);
   fChain->SetBranchAddress("muon.P",muon_P);
   fChain->SetBranchAddress("muon.Px",muon_Px);
   fChain->SetBranchAddress("muon.Py",muon_Py);
   fChain->SetBranchAddress("muon.Pz",muon_Pz);
   fChain->SetBranchAddress("muon.Pt",muon_Pt);
   fChain->SetBranchAddress("muon.Eta",muon_Eta);
   fChain->SetBranchAddress("muon.Phi0",muon_Phi0);
   fChain->SetBranchAddress("muon.Z0",muon_Z0);
   fChain->SetBranchAddress("muon.Charge",muon_Charge);
   fChain->SetBranchAddress("muon.D0",muon_D0);
   fChain->SetBranchAddress("muon.PtCorr",muon_PtCorr);
   fChain->SetBranchAddress("muon.PxCorr",muon_PxCorr);
   fChain->SetBranchAddress("muon.PyCorr",muon_PyCorr);
   fChain->SetBranchAddress("muon.PzCorr",muon_PzCorr);
   fChain->SetBranchAddress("muon.PCorr",muon_PCorr);
   fChain->SetBranchAddress("muon.BCP",muon_BCP);
   fChain->SetBranchAddress("muon.BCPx",muon_BCPx);
   fChain->SetBranchAddress("muon.BCPy",muon_BCPy);
   fChain->SetBranchAddress("muon.BCPz",muon_BCPz);
   fChain->SetBranchAddress("muon.BCPt",muon_BCPt);
   fChain->SetBranchAddress("muon.BCEta",muon_BCEta);
   fChain->SetBranchAddress("muon.BCPhi0",muon_BCPhi0);
   fChain->SetBranchAddress("muon.BCZ0",muon_BCZ0);
   fChain->SetBranchAddress("muon.BCCharge",muon_BCCharge);
   fChain->SetBranchAddress("muon.RawP",muon_RawP);
   fChain->SetBranchAddress("muon.RawPx",muon_RawPx);
   fChain->SetBranchAddress("muon.RawPy",muon_RawPy);
   fChain->SetBranchAddress("muon.RawPz",muon_RawPz);
   fChain->SetBranchAddress("muon.RawPt",muon_RawPt);
   fChain->SetBranchAddress("muon.RawEta",muon_RawEta);
   fChain->SetBranchAddress("muon.RawPhi0",muon_RawPhi0);
   fChain->SetBranchAddress("muon.RawZ0",muon_RawZ0);
   fChain->SetBranchAddress("muon.RawCharge",muon_RawCharge);
   fChain->SetBranchAddress("muon.TrkSiHits",muon_TrkSiHits);
   fChain->SetBranchAddress("muon.TrkAxHits",muon_TrkAxHits);
   fChain->SetBranchAddress("muon.TrkStHits",muon_TrkStHits);
   fChain->SetBranchAddress("muon.TrkAxSeg",muon_TrkAxSeg);
   fChain->SetBranchAddress("muon.TrkStSeg",muon_TrkStSeg);
   fChain->SetBranchAddress("muon.TrkRedChi2CT",muon_TrkRedChi2CT);
   fChain->SetBranchAddress("muon.TrkID",muon_TrkID);
   fChain->SetBranchAddress("muon.TrkIDOrig",muon_TrkIDOrig);
   fChain->SetBranchAddress("muon.HadEnergy",muon_HadEnergy);
   fChain->SetBranchAddress("muon.EmEnergy",muon_EmEnergy);
   fChain->SetBranchAddress("muon.EventZVertex",muon_EventZVertex);
   fChain->SetBranchAddress("muon.DeltaZVertex",muon_DeltaZVertex);
   fChain->SetBranchAddress("muon.Isol",muon_Isol);
   fChain->SetBranchAddress("muon.EmIsol4",muon_EmIsol4);
   fChain->SetBranchAddress("muon.HadIsol4",muon_HadIsol4);
   fChain->SetBranchAddress("muon.TotalIsol4",muon_TotalIsol4);
   fChain->SetBranchAddress("muon.EmIsol7",muon_EmIsol7);
   fChain->SetBranchAddress("muon.HadIsol7",muon_HadIsol7);
   fChain->SetBranchAddress("muon.TotalIsol7",muon_TotalIsol7);
   fChain->SetBranchAddress("muon.TrkIsol",muon_TrkIsol);
   fChain->SetBranchAddress("muon.CmuDx",muon_CmuDx);
   fChain->SetBranchAddress("muon.CmuDz",muon_CmuDz);
   fChain->SetBranchAddress("muon.CmuDphi",muon_CmuDphi);
   fChain->SetBranchAddress("muon.CmpDx",muon_CmpDx);
   fChain->SetBranchAddress("muon.CmpDphi",muon_CmpDphi);
   fChain->SetBranchAddress("muon.CmxDx",muon_CmxDx);
   fChain->SetBranchAddress("muon.CmxDz",muon_CmxDz);
   fChain->SetBranchAddress("muon.CmxDphi",muon_CmxDphi);
   fChain->SetBranchAddress("muon.BmuDx",muon_BmuDx);
   fChain->SetBranchAddress("muon.Dx[4]",muon_Dx);
   fChain->SetBranchAddress("muon.Wedge[4]",muon_Wedge);
   fChain->SetBranchAddress("muon.Side[4]",muon_Side);
   fChain->SetBranchAddress("muon.NumAssHits[4]",muon_NumAssHits);
   fChain->SetBranchAddress("muon.CheckEn",muon_CheckEn);
   fChain->SetBranchAddress("muon.CheckPx",muon_CheckPx);
   fChain->SetBranchAddress("muon.CheckPy",muon_CheckPy);
   fChain->SetBranchAddress("muon.CheckPz",muon_CheckPz);
   fChain->SetBranchAddress("muon.CheckZ0",muon_CheckZ0);
   fChain->SetBranchAddress("muon.CheckZ0Err",muon_CheckZ0Err);
   fChain->SetBranchAddress("muon.CheckD0",muon_CheckD0);
   fChain->SetBranchAddress("muon.CheckTrkId",muon_CheckTrkId);
   fChain->SetBranchAddress("muon.CutCode",muon_CutCode);
   fChain->SetBranchAddress("muon.BruceLLK",muon_BruceLLK);
   fChain->SetBranchAddress("muon.goodForCMXTrig",muon_goodForCMXTrig);
   fChain->SetBranchAddress("muon.Dz[4]",muon_Dz);
   fChain->SetBranchAddress("muon.Dphi[4]",muon_Dphi);
   fChain->SetBranchAddress("muon.Dtheta[4]",muon_Dtheta);
   fChain->SetBranchAddress("muon.ChiSqX[4]",muon_ChiSqX);
   fChain->SetBranchAddress("muon.ChiSqZ[4]",muon_ChiSqZ);
   fChain->SetBranchAddress("muon.ChiSqXPos[4]",muon_ChiSqXPos);
   fChain->SetBranchAddress("muon.ChiSqZPos[4]",muon_ChiSqZPos);
   fChain->SetBranchAddress("muon.NumCntrHits[4]",muon_NumCntrHits);
   fChain->SetBranchAddress("muon.HitPatt[4]",muon_HitPatt);
   fChain->SetBranchAddress("muon.NumChamberHits[4]",muon_NumChamberHits);
   fChain->SetBranchAddress("muon.PosX[4]",muon_PosX);
   fChain->SetBranchAddress("muon.PosY[4]",muon_PosY);
   fChain->SetBranchAddress("muon.PosZ[4]",muon_PosZ);
   fChain->SetBranchAddress("muon.DirX[4]",muon_DirX);
   fChain->SetBranchAddress("muon.DirY[4]",muon_DirY);
   fChain->SetBranchAddress("muon.DirZ[4]",muon_DirZ);
   fChain->SetBranchAddress("muon.TrkPosX[4]",muon_TrkPosX);
   fChain->SetBranchAddress("muon.TrkPosY[4]",muon_TrkPosY);
   fChain->SetBranchAddress("muon.TrkPosZ[4]",muon_TrkPosZ);
   fChain->SetBranchAddress("muon.TrkDirX[4]",muon_TrkDirX);
   fChain->SetBranchAddress("muon.TrkDirY[4]",muon_TrkDirY);
   fChain->SetBranchAddress("muon.TrkDirZ[4]",muon_TrkDirZ);
   fChain->SetBranchAddress("numMuObjs",&numMuObjs);
   fChain->SetBranchAddress("muoViewDescr1",muoViewDescr1);
   fChain->SetBranchAddress("muoViewDescr2",muoViewDescr2);
   fChain->SetBranchAddress("muoViewDescr3",muoViewDescr3);
   fChain->SetBranchAddress("muoViewDescr4",muoViewDescr4);
   fChain->SetBranchAddress("muoViewDescr5",muoViewDescr5);
   fChain->SetBranchAddress("sltmu",&sltmu_);
   fChain->SetBranchAddress("sltmu.fUniqueID",sltmu_fUniqueID);
   fChain->SetBranchAddress("sltmu.fBits",sltmu_fBits);
   fChain->SetBranchAddress("sltmu.jetid",sltmu_jetid);
   fChain->SetBranchAddress("sltmu.trkid",sltmu_trkid);
   fChain->SetBranchAddress("sltmu.muid",sltmu_muid);
   fChain->SetBranchAddress("sltmu.jetCollType",sltmu_jetCollType);
   fChain->SetBranchAddress("sltmu.dRclstJet",sltmu_dRclstJet);
   fChain->SetBranchAddress("sltmu.ptRel",sltmu_ptRel);
   fChain->SetBranchAddress("sltmu.sumPt06",sltmu_sumPt06);
   fChain->SetBranchAddress("sltmu.nTrkJet06",sltmu_nTrkJet06);
   fChain->SetBranchAddress("sltmu.sumPt06C1",sltmu_sumPt06C1);
   fChain->SetBranchAddress("sltmu.nTrkJet06C1",sltmu_nTrkJet06C1);
   fChain->SetBranchAddress("sltmu.sumPt06C2",sltmu_sumPt06C2);
   fChain->SetBranchAddress("sltmu.nTrkJet06C2",sltmu_nTrkJet06C2);
   fChain->SetBranchAddress("sltmu.sumPt04",sltmu_sumPt04);
   fChain->SetBranchAddress("sltmu.nTrkJet04",sltmu_nTrkJet04);
   fChain->SetBranchAddress("sltmu.sumPt04C1",sltmu_sumPt04C1);
   fChain->SetBranchAddress("sltmu.nTrkJet04C1",sltmu_nTrkJet04C1);
   fChain->SetBranchAddress("sltmu.sumPt04C2",sltmu_sumPt04C2);
   fChain->SetBranchAddress("sltmu.nTrkJet04C2",sltmu_nTrkJet04C2);
   fChain->SetBranchAddress("sltmu.sumPt02",sltmu_sumPt02);
   fChain->SetBranchAddress("sltmu.nTrkJet02",sltmu_nTrkJet02);
   fChain->SetBranchAddress("sltmu.sumPt02C1",sltmu_sumPt02C1);
   fChain->SetBranchAddress("sltmu.nTrkJet02C1",sltmu_nTrkJet02C1);
   fChain->SetBranchAddress("sltmu.sumPt02C2",sltmu_sumPt02C2);
   fChain->SetBranchAddress("sltmu.nTrkJet02C2",sltmu_nTrkJet02C2);
   fChain->SetBranchAddress("sltmu.emE",sltmu_emE);
   fChain->SetBranchAddress("sltmu.hadE",sltmu_hadE);
   fChain->SetBranchAddress("sltmu.neightborEmE",sltmu_neightborEmE);
   fChain->SetBranchAddress("sltmu.neightborHadE",sltmu_neightborHadE);
   fChain->SetBranchAddress("sltmu.coneR4Et",sltmu_coneR4Et);
   fChain->SetBranchAddress("sltmu.fiducial",sltmu_fiducial);
   fChain->SetBranchAddress("sltmu.cmuFidSig",sltmu_cmuFidSig);
   fChain->SetBranchAddress("sltmu.cmpFidSig",sltmu_cmpFidSig);
   fChain->SetBranchAddress("sltmu.cmxFidSig",sltmu_cmxFidSig);
   fChain->SetBranchAddress("sltmu.fiducialSig",sltmu_fiducialSig);
   fChain->SetBranchAddress("sltmu.isTaggable",sltmu_isTaggable);
   fChain->SetBranchAddress("sltmu.isTagged",sltmu_isTagged);
   fChain->SetBranchAddress("sltmu.nUsedVar",sltmu_nUsedVar);
   fChain->SetBranchAddress("sltmu.usedVar",sltmu_usedVar);
   fChain->SetBranchAddress("sltmu.likelihood",sltmu_likelihood);
   fChain->SetBranchAddress("sltmu.pullCmuDx",sltmu_pullCmuDx);
   fChain->SetBranchAddress("sltmu.pullCmuDz",sltmu_pullCmuDz);
   fChain->SetBranchAddress("sltmu.pullCmuDphi",sltmu_pullCmuDphi);
   fChain->SetBranchAddress("sltmu.pullCmuHits",sltmu_pullCmuHits);
   fChain->SetBranchAddress("sltmu.pullCmuChi2X",sltmu_pullCmuChi2X);
   fChain->SetBranchAddress("sltmu.pullCmuChi2Z",sltmu_pullCmuChi2Z);
   fChain->SetBranchAddress("sltmu.pullCmpDx",sltmu_pullCmpDx);
   fChain->SetBranchAddress("sltmu.pullCmpDphi",sltmu_pullCmpDphi);
   fChain->SetBranchAddress("sltmu.pullCmpChi2X",sltmu_pullCmpChi2X);
   fChain->SetBranchAddress("sltmu.pullCmxDx",sltmu_pullCmxDx);
   fChain->SetBranchAddress("sltmu.pullCmxDz",sltmu_pullCmxDz);
   fChain->SetBranchAddress("sltmu.pullCmxDphi",sltmu_pullCmxDphi);
   fChain->SetBranchAddress("sltmu.pullCmxChi2X",sltmu_pullCmxChi2X);
   fChain->SetBranchAddress("sltmu.pullCmxChi2Z",sltmu_pullCmxChi2Z);
   fChain->SetBranchAddress("sltmu.phiCot",sltmu_phiCot);
   fChain->SetBranchAddress("sltmu.etaCot",sltmu_etaCot);
   fChain->SetBranchAddress("sltmu.radLengths[4]",sltmu_radLengths);
   fChain->SetBranchAddress("sltmu.absLengths[4]",sltmu_absLengths);
   fChain->SetBranchAddress("sltmu.eLoss[4]",sltmu_eLoss);
   fChain->SetBranchAddress("sltmu.extPosX[4]",sltmu_extPosX);
   fChain->SetBranchAddress("sltmu.extPosY[4]",sltmu_extPosY);
   fChain->SetBranchAddress("sltmu.extPosZ[4]",sltmu_extPosZ);
   fChain->SetBranchAddress("sltmu.extDirX[4]",sltmu_extDirX);
   fChain->SetBranchAddress("sltmu.extDirY[4]",sltmu_extDirY);
   fChain->SetBranchAddress("sltmu.extDirZ[4]",sltmu_extDirZ);
   fChain->SetBranchAddress("sltmu.dist[4]",sltmu_dist);
   fChain->SetBranchAddress("numSltMuObjs",&numSltMuObjs);
   fChain->SetBranchAddress("cosmic",&cosmic_);
   fChain->SetBranchAddress("cosmic.fUniqueID",cosmic_fUniqueID);
   fChain->SetBranchAddress("cosmic.fBits",cosmic_fBits);
   fChain->SetBranchAddress("cosmic.cosmicBit",cosmic_cosmicBit);
   fChain->SetBranchAddress("cosmic.hasCosmicRay",cosmic_hasCosmicRay);
   fChain->SetBranchAddress("cosmic.hasFewTracks",cosmic_hasFewTracks);
   fChain->SetBranchAddress("cosmic.hasLargeD0",cosmic_hasLargeD0);
   fChain->SetBranchAddress("cosmic.hasLargeZ0",cosmic_hasLargeZ0);
   fChain->SetBranchAddress("cosmic.hasAcolTrack",cosmic_hasAcolTrack);
   fChain->SetBranchAddress("cosmic.hasOOTEnergy",cosmic_hasOOTEnergy);
   fChain->SetBranchAddress("cosmic.hasOOTScintillators",cosmic_hasOOTScintillators);
   fChain->SetBranchAddress("cosmic.hasNoVertex",cosmic_hasNoVertex);
   fChain->SetBranchAddress("cosmic.hasLargeDz",cosmic_hasLargeDz);
   fChain->SetBranchAddress("cosmic.hasLargeDPhiMuStub",cosmic_hasLargeDPhiMuStub);
   fChain->SetBranchAddress("cosmic.isMuonNothing",cosmic_isMuonNothing);
   fChain->SetBranchAddress("cosmic.isMuonStub",cosmic_isMuonStub);
   fChain->SetBranchAddress("cosmic.isMuonTrack",cosmic_isMuonTrack);
   fChain->SetBranchAddress("cosmic.isMuonMuon",cosmic_isMuonMuon);
   fChain->SetBranchAddress("cosmic.hasOOTTof",cosmic_hasOOTTof);
   fChain->SetBranchAddress("cosmic.hasNotOutgoingPair",cosmic_hasNotOutgoingPair);
   fChain->SetBranchAddress("cosmic.hasLowEmFrac",cosmic_hasLowEmFrac);
   fChain->SetBranchAddress("cosmic.hasLowQFrac",cosmic_hasLowQFrac);
   fChain->SetBranchAddress("cosmic.hasOOTCaloE",cosmic_hasOOTCaloE);
   fChain->SetBranchAddress("cosmic.nLegs",cosmic_nLegs);
   fChain->SetBranchAddress("cosmic.deltaTdc",cosmic_deltaTdc);
   fChain->SetBranchAddress("cosmic.deltaTof",cosmic_deltaTof);
   fChain->SetBranchAddress("cosmic.dPhiMuStub",cosmic_dPhiMuStub);
   fChain->SetBranchAddress("cosmic.acollinearity",cosmic_acollinearity);
   fChain->SetBranchAddress("cosmic.d0Rho",cosmic_d0Rho);
   fChain->SetBranchAddress("cosmic.d0Beta",cosmic_d0Beta);
   fChain->SetBranchAddress("cosmic.dPhi",cosmic_dPhi);
   fChain->SetBranchAddress("cosmic.sumEta",cosmic_sumEta);
   fChain->SetBranchAddress("cosmic.seedZ",cosmic_seedZ);
   fChain->SetBranchAddress("cosmic.seedZErr",cosmic_seedZErr);
   fChain->SetBranchAddress("cosmic.nTrkZVtx",cosmic_nTrkZVtx);
   fChain->SetBranchAddress("cosmic.meanZVtx",cosmic_meanZVtx);
   fChain->SetBranchAddress("cosmic.meanZVtxErr",cosmic_meanZVtxErr);
   fChain->SetBranchAddress("cosmic.muoLnk[2]",cosmic_muoLnk);
   fChain->SetBranchAddress("cosmic.trkLnk[2]",cosmic_trkLnk);
   fChain->SetBranchAddress("cosmic.trackId[2]",cosmic_trackId);
   fChain->SetBranchAddress("cosmic.pt[2]",cosmic_pt);
   fChain->SetBranchAddress("cosmic.crv[2]",cosmic_crv);
   fChain->SetBranchAddress("cosmic.cot[2]",cosmic_cot);
   fChain->SetBranchAddress("cosmic.phi[2]",cosmic_phi);
   fChain->SetBranchAddress("cosmic.eta[2]",cosmic_eta);
   fChain->SetBranchAddress("cosmic.z0[2]",cosmic_z0);
   fChain->SetBranchAddress("cosmic.d0[2]",cosmic_d0);
   fChain->SetBranchAddress("cosmic.d0C[2]",cosmic_d0C);
   fChain->SetBranchAddress("cosmic.charge[2]",cosmic_charge);
   fChain->SetBranchAddress("cosmic.algo[2]",cosmic_algo);
   fChain->SetBranchAddress("cosmic.deltaT0[2]",cosmic_deltaT0);
   fChain->SetBranchAddress("cosmic.errorDeltaT0[2]",cosmic_errorDeltaT0);
   fChain->SetBranchAddress("cosmic.deltaBetaInv[2]",cosmic_deltaBetaInv);
   fChain->SetBranchAddress("cosmic.errorDeltaBetaInv[2]",cosmic_errorDeltaBetaInv);
   fChain->SetBranchAddress("cosmic.fitT0[2][2]",cosmic_fitT0);
   fChain->SetBranchAddress("cosmic.fitdT0[2][2]",cosmic_fitdT0);
   fChain->SetBranchAddress("cosmic.fitCrdT0[2][2]",cosmic_fitCrdT0);
   fChain->SetBranchAddress("cosmic.fitChi2[2][2]",cosmic_fitChi2);
   fChain->SetBranchAddress("cosmic.fitdChi2[2][2]",cosmic_fitdChi2);
   fChain->SetBranchAddress("cosmic.fitDir[2][2]",cosmic_fitDir);
   fChain->SetBranchAddress("cosmic.fitNCOTH[2][2]",cosmic_fitNCOTH);
   fChain->SetBranchAddress("cosmic.fitNCOTHExp[2][2]",cosmic_fitNCOTHExp);
   fChain->SetBranchAddress("cosmic.emE[2]",cosmic_emE);
   fChain->SetBranchAddress("cosmic.hadE[2]",cosmic_hadE);
   fChain->SetBranchAddress("cosmic.hadTDC[2]",cosmic_hadTDC);
   fChain->SetBranchAddress("cosmic.ngbrEmE[2]",cosmic_ngbrEmE);
   fChain->SetBranchAddress("cosmic.ngbrHadE[2]",cosmic_ngbrHadE);
   fChain->SetBranchAddress("cosmic.ngbrHadTDC[2]",cosmic_ngbrHadTDC);
   fChain->SetBranchAddress("cosmic.stubPhi[2]",cosmic_stubPhi);
   fChain->SetBranchAddress("cosmic.tof[2]",cosmic_tof);
   fChain->SetBranchAddress("cosmic.tofMethod[2]",cosmic_tofMethod);
   fChain->SetBranchAddress("cosmic.csp[2]",cosmic_csp);
   fChain->SetBranchAddress("cosmic.csx[2]",cosmic_csx);
   fChain->SetBranchAddress("cosmic.validLeg[2]",cosmic_validLeg);
   fChain->SetBranchAddress("cosmic.validTrack[2]",cosmic_validTrack);
   fChain->SetBranchAddress("cosmic.validStub[2]",cosmic_validStub);
   fChain->SetBranchAddress("cosmic.validEnergy[2]",cosmic_validEnergy);
   fChain->SetBranchAddress("cosmic.validHadTdc[2]",cosmic_validHadTdc);
   fChain->SetBranchAddress("cosmic.validTof[2]",cosmic_validTof);
   fChain->SetBranchAddress("cosmic.validCsp[2]",cosmic_validCsp);
   fChain->SetBranchAddress("cosmic.validCsx[2]",cosmic_validCsx);
   fChain->SetBranchAddress("cosmic.trackLessMuon[2]",cosmic_trackLessMuon);
   fChain->SetBranchAddress("cosmic.stubLessMuon[2]",cosmic_stubLessMuon);
   fChain->SetBranchAddress("electron",&electron_);
   fChain->SetBranchAddress("electron.fUniqueID",electron_fUniqueID);
   fChain->SetBranchAddress("electron.fBits",electron_fBits);
   fChain->SetBranchAddress("electron.CollType",electron_CollType);
   fChain->SetBranchAddress("electron.Region",electron_Region);
   fChain->SetBranchAddress("electron.Conversion",electron_Conversion);
   fChain->SetBranchAddress("electron.trkLink",electron_trkLink);
   fChain->SetBranchAddress("electron.phxTrkLink",electron_phxTrkLink);
   fChain->SetBranchAddress("electron.En",electron_En);
   fChain->SetBranchAddress("electron.RawEn",electron_RawEn);
   fChain->SetBranchAddress("electron.CluEn",electron_CluEn);
   fChain->SetBranchAddress("electron.Px",electron_Px);
   fChain->SetBranchAddress("electron.RawPx",electron_RawPx);
   fChain->SetBranchAddress("electron.CluPx",electron_CluPx);
   fChain->SetBranchAddress("electron.Py",electron_Py);
   fChain->SetBranchAddress("electron.RawPy",electron_RawPy);
   fChain->SetBranchAddress("electron.CluPy",electron_CluPy);
   fChain->SetBranchAddress("electron.Pz",electron_Pz);
   fChain->SetBranchAddress("electron.RawPz",electron_RawPz);
   fChain->SetBranchAddress("electron.CluPz",electron_CluPz);
   fChain->SetBranchAddress("electron.Et",electron_Et);
   fChain->SetBranchAddress("electron.RawEt",electron_RawEt);
   fChain->SetBranchAddress("electron.CluEt",electron_CluEt);
   fChain->SetBranchAddress("electron.Phi",electron_Phi);
   fChain->SetBranchAddress("electron.RawPhi",electron_RawPhi);
   fChain->SetBranchAddress("electron.CluPhi",electron_CluPhi);
   fChain->SetBranchAddress("electron.Eta",electron_Eta);
   fChain->SetBranchAddress("electron.RawEta",electron_RawEta);
   fChain->SetBranchAddress("electron.CluEta",electron_CluEta);
   fChain->SetBranchAddress("electron.Clu_totEt_jetZ",electron_Clu_totEt_jetZ);
   fChain->SetBranchAddress("electron.Clu_phi_jetZ",electron_Clu_phi_jetZ);
   fChain->SetBranchAddress("electron.TrkSiHits",electron_TrkSiHits);
   fChain->SetBranchAddress("electron.TrkAxHits",electron_TrkAxHits);
   fChain->SetBranchAddress("electron.TrkStHits",electron_TrkStHits);
   fChain->SetBranchAddress("electron.TrkAxSeg",electron_TrkAxSeg);
   fChain->SetBranchAddress("electron.TrkStSeg",electron_TrkStSeg);
   fChain->SetBranchAddress("electron.TrkP",electron_TrkP);
   fChain->SetBranchAddress("electron.BCP",electron_BCP);
   fChain->SetBranchAddress("electron.RawTrkP",electron_RawTrkP);
   fChain->SetBranchAddress("electron.TrkPx",electron_TrkPx);
   fChain->SetBranchAddress("electron.BCPx",electron_BCPx);
   fChain->SetBranchAddress("electron.RawTrkPx",electron_RawTrkPx);
   fChain->SetBranchAddress("electron.TrkPy",electron_TrkPy);
   fChain->SetBranchAddress("electron.BCPy",electron_BCPy);
   fChain->SetBranchAddress("electron.RawTrkPy",electron_RawTrkPy);
   fChain->SetBranchAddress("electron.TrkPz",electron_TrkPz);
   fChain->SetBranchAddress("electron.BCPz",electron_BCPz);
   fChain->SetBranchAddress("electron.RawTrkPz",electron_RawTrkPz);
   fChain->SetBranchAddress("electron.TrkPt",electron_TrkPt);
   fChain->SetBranchAddress("electron.BCPt",electron_BCPt);
   fChain->SetBranchAddress("electron.RawTrkPt",electron_RawTrkPt);
   fChain->SetBranchAddress("electron.TrkEta",electron_TrkEta);
   fChain->SetBranchAddress("electron.BCEta",electron_BCEta);
   fChain->SetBranchAddress("electron.RawTrkEta",electron_RawTrkEta);
   fChain->SetBranchAddress("electron.TrkPhi0",electron_TrkPhi0);
   fChain->SetBranchAddress("electron.BCPhi0",electron_BCPhi0);
   fChain->SetBranchAddress("electron.RawTrkPhi0",electron_RawTrkPhi0);
   fChain->SetBranchAddress("electron.TrkZ0",electron_TrkZ0);
   fChain->SetBranchAddress("electron.BCZ0",electron_BCZ0);
   fChain->SetBranchAddress("electron.RawTrkZ0",electron_RawTrkZ0);
   fChain->SetBranchAddress("electron.Charge",electron_Charge);
   fChain->SetBranchAddress("electron.BCCharge",electron_BCCharge);
   fChain->SetBranchAddress("electron.RawTrkCharge",electron_RawTrkCharge);
   fChain->SetBranchAddress("electron.TrkD0",electron_TrkD0);
   fChain->SetBranchAddress("electron.TrkID",electron_TrkID);
   fChain->SetBranchAddress("electron.TrkIDOrig",electron_TrkIDOrig);
   fChain->SetBranchAddress("electron.Fiducial",electron_Fiducial);
   fChain->SetBranchAddress("electron.TrkFiducial",electron_TrkFiducial);
   fChain->SetBranchAddress("electron.EtaDet",electron_EtaDet);
   fChain->SetBranchAddress("electron.EtaEvent",electron_EtaEvent);
   fChain->SetBranchAddress("electron.HadEt",electron_HadEt);
   fChain->SetBranchAddress("electron.TotalEt",electron_TotalEt);
   fChain->SetBranchAddress("electron.Hadem",electron_Hadem);
   fChain->SetBranchAddress("electron.EmEt",electron_EmEt);
   fChain->SetBranchAddress("electron.HadEn",electron_HadEn);
   fChain->SetBranchAddress("electron.TotalEn",electron_TotalEn);
   fChain->SetBranchAddress("electron.TotalIsol4",electron_TotalIsol4);
   fChain->SetBranchAddress("electron.TotalIsol7",electron_TotalIsol7);
   fChain->SetBranchAddress("electron.EmIsol4",electron_EmIsol4);
   fChain->SetBranchAddress("electron.EmIsol7",electron_EmIsol7);
   fChain->SetBranchAddress("electron.HadIsol4",electron_HadIsol4);
   fChain->SetBranchAddress("electron.HadIsol7",electron_HadIsol7);
   fChain->SetBranchAddress("electron.PprEn",electron_PprEn);
   fChain->SetBranchAddress("electron.Pem3x3FitTow",electron_Pem3x3FitTow);
   fChain->SetBranchAddress("electron.Pem3x3DetEta",electron_Pem3x3DetEta);
   fChain->SetBranchAddress("electron.Pem3x3Phi",electron_Pem3x3Phi);
   fChain->SetBranchAddress("electron.Pem3x3FitEn",electron_Pem3x3FitEn);
   fChain->SetBranchAddress("electron.Pem3x3Chisq",electron_Pem3x3Chisq);
   fChain->SetBranchAddress("electron.NumPes2d",electron_NumPes2d);
   fChain->SetBranchAddress("electron.Pes2dEta",electron_Pes2dEta);
   fChain->SetBranchAddress("electron.Pes2dPhi",electron_Pes2dPhi);
   fChain->SetBranchAddress("electron.Pes2dX",electron_Pes2dX);
   fChain->SetBranchAddress("electron.Pes2dY",electron_Pes2dY);
   fChain->SetBranchAddress("electron.Pes2d5by9U",electron_Pes2d5by9U);
   fChain->SetBranchAddress("electron.Pes2d5by9V",electron_Pes2d5by9V);
   fChain->SetBranchAddress("electron.Pes2dEn",electron_Pes2dEn);
   fChain->SetBranchAddress("electron.ClosestPes2d5by9U",electron_ClosestPes2d5by9U);
   fChain->SetBranchAddress("electron.ClosestPes2d5by9V",electron_ClosestPes2d5by9V);
   fChain->SetBranchAddress("electron.ClosestPes2dEn",electron_ClosestPes2dEn);
   fChain->SetBranchAddress("electron.LshrTrk",electron_LshrTrk);
   fChain->SetBranchAddress("electron.Lshr3",electron_Lshr3);
   fChain->SetBranchAddress("electron.Lshr2",electron_Lshr2);
   fChain->SetBranchAddress("electron.Isol",electron_Isol);
   fChain->SetBranchAddress("electron.LeakE",electron_LeakE);
   fChain->SetBranchAddress("electron.EP",electron_EP);
   fChain->SetBranchAddress("electron.EventZVertex",electron_EventZVertex);
   fChain->SetBranchAddress("electron.DeltaZVertex",electron_DeltaZVertex);
   fChain->SetBranchAddress("electron.TrkCESx",electron_TrkCESx);
   fChain->SetBranchAddress("electron.BestCESx",electron_BestCESx);
   fChain->SetBranchAddress("electron.DeltaX",electron_DeltaX);
   fChain->SetBranchAddress("electron.TrkCESz",electron_TrkCESz);
   fChain->SetBranchAddress("electron.BestCESz",electron_BestCESz);
   fChain->SetBranchAddress("electron.DeltaZ",electron_DeltaZ);
   fChain->SetBranchAddress("electron.StripChi2",electron_StripChi2);
   fChain->SetBranchAddress("electron.WireChi2",electron_WireChi2);
   fChain->SetBranchAddress("electron.TrkIsol",electron_TrkIsol);
   fChain->SetBranchAddress("electron.Towers",electron_Towers);
   fChain->SetBranchAddress("electron.Tracks",electron_Tracks);
   fChain->SetBranchAddress("electron.SeedEta",electron_SeedEta);
   fChain->SetBranchAddress("electron.SeedPhi",electron_SeedPhi);
   fChain->SetBranchAddress("electron.SeedEn",electron_SeedEn);
   fChain->SetBranchAddress("electron.SeedEt",electron_SeedEt);
   fChain->SetBranchAddress("electron.PhoZV",electron_PhoZV);
   fChain->SetBranchAddress("electron.PhoRawEn",electron_PhoRawEn);
   fChain->SetBranchAddress("electron.PhoRawEt",electron_PhoRawEt);
   fChain->SetBranchAddress("electron.PhoEt",electron_PhoEt);
   fChain->SetBranchAddress("electron.PhoPhi",electron_PhoPhi);
   fChain->SetBranchAddress("electron.PhoEta",electron_PhoEta);
   fChain->SetBranchAddress("electron.PhoCESx",electron_PhoCESx);
   fChain->SetBranchAddress("electron.PhoCESz",electron_PhoCESz);
   fChain->SetBranchAddress("electron.PhoEneStrip",electron_PhoEneStrip);
   fChain->SetBranchAddress("electron.PhoEneWire",electron_PhoEneWire);
   fChain->SetBranchAddress("electron.PhoStripChi2",electron_PhoStripChi2);
   fChain->SetBranchAddress("electron.PhoWireChi2",electron_PhoWireChi2);
   fChain->SetBranchAddress("electron.PhoCES2x",electron_PhoCES2x);
   fChain->SetBranchAddress("electron.PhoCES2z",electron_PhoCES2z);
   fChain->SetBranchAddress("electron.PhoEneStrip2",electron_PhoEneStrip2);
   fChain->SetBranchAddress("electron.PhoEneWire2",electron_PhoEneWire2);
   fChain->SetBranchAddress("electron.PhoCesSlide",electron_PhoCesSlide);
   fChain->SetBranchAddress("electron.PhoN3D",electron_PhoN3D);
   fChain->SetBranchAddress("electron.PhoPt",electron_PhoPt);
   fChain->SetBranchAddress("electron.PhoIsol",electron_PhoIsol);
   fChain->SetBranchAddress("electron.PhoTrkIsol",electron_PhoTrkIsol);
   fChain->SetBranchAddress("electron.PhoLshr",electron_PhoLshr);
   fChain->SetBranchAddress("electron.PhoHadem",electron_PhoHadem);
   fChain->SetBranchAddress("electron.RawEnergyWire",electron_RawEnergyWire);
   fChain->SetBranchAddress("electron.RawEnergyStrip",electron_RawEnergyStrip);
   fChain->SetBranchAddress("electron.FitEnergyWire",electron_FitEnergyWire);
   fChain->SetBranchAddress("electron.FitEnergyStrip",electron_FitEnergyStrip);
   fChain->SetBranchAddress("electron.CESside",electron_CESside);
   fChain->SetBranchAddress("electron.CESmodule",electron_CESmodule);
   fChain->SetBranchAddress("electron.TrkCESside",electron_TrkCESside);
   fChain->SetBranchAddress("electron.TrkCESmodule",electron_TrkCESmodule);
   fChain->SetBranchAddress("electron.CPR",electron_CPR);
   fChain->SetBranchAddress("electron.CPRside",electron_CPRside);
   fChain->SetBranchAddress("electron.CPRmodule",electron_CPRmodule);
   fChain->SetBranchAddress("electron.TrkCPRside",electron_TrkCPRside);
   fChain->SetBranchAddress("electron.TrkCPRmodule",electron_TrkCPRmodule);
   fChain->SetBranchAddress("electron.CPRx",electron_CPRx);
   fChain->SetBranchAddress("electron.DeltaXCPR",electron_DeltaXCPR);
   fChain->SetBranchAddress("electron.RawEnergyCPR",electron_RawEnergyCPR);
   fChain->SetBranchAddress("electron.TrkCPRx",electron_TrkCPRx);
   fChain->SetBranchAddress("electron.TrkCPRz",electron_TrkCPRz);
   fChain->SetBranchAddress("electron.TrkCPRphi",electron_TrkCPRphi);
   fChain->SetBranchAddress("electron.TrkCPReta",electron_TrkCPReta);
   fChain->SetBranchAddress("electron.WidthWire",electron_WidthWire);
   fChain->SetBranchAddress("electron.WidthStrip",electron_WidthStrip);
   fChain->SetBranchAddress("electron.CheckEn",electron_CheckEn);
   fChain->SetBranchAddress("electron.CheckPx",electron_CheckPx);
   fChain->SetBranchAddress("electron.CheckPy",electron_CheckPy);
   fChain->SetBranchAddress("electron.CheckPz",electron_CheckPz);
   fChain->SetBranchAddress("electron.CheckZ0",electron_CheckZ0);
   fChain->SetBranchAddress("electron.CheckZ0Err",electron_CheckZ0Err);
   fChain->SetBranchAddress("electron.CheckConv",electron_CheckConv);
   fChain->SetBranchAddress("electron.CheckTrkId",electron_CheckTrkId);
   fChain->SetBranchAddress("electron.CutCode",electron_CutCode);
   fChain->SetBranchAddress("electron.PhxTrkSiHits",electron_PhxTrkSiHits);
   fChain->SetBranchAddress("electron.PhxTrkP",electron_PhxTrkP);
   fChain->SetBranchAddress("electron.PhxTrkPx",electron_PhxTrkPx);
   fChain->SetBranchAddress("electron.PhxTrkPy",electron_PhxTrkPy);
   fChain->SetBranchAddress("electron.PhxTrkPz",electron_PhxTrkPz);
   fChain->SetBranchAddress("electron.PhxTrkPt",electron_PhxTrkPt);
   fChain->SetBranchAddress("electron.PhxTrkEta",electron_PhxTrkEta);
   fChain->SetBranchAddress("electron.PhxTrkPhi0",electron_PhxTrkPhi0);
   fChain->SetBranchAddress("electron.PhxTrkZ0",electron_PhxTrkZ0);
   fChain->SetBranchAddress("electron.PhxCharge",electron_PhxCharge);
   fChain->SetBranchAddress("electron.PhxTrkD0",electron_PhxTrkD0);
   fChain->SetBranchAddress("electron.PhxMatch",electron_PhxMatch);
   fChain->SetBranchAddress("electron.PhxCotMatchID",electron_PhxCotMatchID);
   fChain->SetBranchAddress("electron.PhxDrTrkPes",electron_PhxDrTrkPes);
   fChain->SetBranchAddress("electron.BruceLLK",electron_BruceLLK);
   fChain->SetBranchAddress("numEmObjs",&numEmObjs);
   fChain->SetBranchAddress("eleViewDescr1",eleViewDescr1);
   fChain->SetBranchAddress("eleViewDescr2",eleViewDescr2);
   fChain->SetBranchAddress("eleViewDescr3",eleViewDescr3);
   fChain->SetBranchAddress("eleViewDescr4",eleViewDescr4);
   fChain->SetBranchAddress("eleViewDescr5",eleViewDescr5);
   fChain->SetBranchAddress("tau",&tau_);
   fChain->SetBranchAddress("tau.fUniqueID",tau_fUniqueID);
   fChain->SetBranchAddress("tau.fBits",tau_fBits);
   fChain->SetBranchAddress("tau.ntowers",tau_ntowers);
   fChain->SetBranchAddress("tau.nclumptowers",tau_nclumptowers);
   fChain->SetBranchAddress("tau.ntrks",tau_ntrks);
   fChain->SetBranchAddress("tau.ntrks10deg",tau_ntrks10deg);
   fChain->SetBranchAddress("tau.ntrks30deg",tau_ntrks30deg);
   fChain->SetBranchAddress("tau.ntrks10to30deg",tau_ntrks10to30deg);
   fChain->SetBranchAddress("tau.charge10deg",tau_charge10deg);
   fChain->SetBranchAddress("tau.nwrongvtxtrk10deg",tau_nwrongvtxtrk10deg);
   fChain->SetBranchAddress("tau.npi0s",tau_npi0s);
   fChain->SetBranchAddress("tau.nmustubs",tau_nmustubs);
   fChain->SetBranchAddress("tau.nmuhits",tau_nmuhits);
   fChain->SetBranchAddress("tau.px",tau_px);
   fChain->SetBranchAddress("tau.py",tau_py);
   fChain->SetBranchAddress("tau.pz",tau_pz);
   fChain->SetBranchAddress("tau.e",tau_e);
   fChain->SetBranchAddress("tau.pxtrk",tau_pxtrk);
   fChain->SetBranchAddress("tau.pytrk",tau_pytrk);
   fChain->SetBranchAddress("tau.pztrk",tau_pztrk);
   fChain->SetBranchAddress("tau.etrk",tau_etrk);
   fChain->SetBranchAddress("tau.pxvis",tau_pxvis);
   fChain->SetBranchAddress("tau.pyvis",tau_pyvis);
   fChain->SetBranchAddress("tau.pzvis",tau_pzvis);
   fChain->SetBranchAddress("tau.evis",tau_evis);
   fChain->SetBranchAddress("tau.emfrac",tau_emfrac);
   fChain->SetBranchAddress("tau.clusterdeteta",tau_clusterdeteta);
   fChain->SetBranchAddress("tau.seedtrkdeteta",tau_seedtrkdeteta);
   fChain->SetBranchAddress("tau.clustereta",tau_clustereta);
   fChain->SetBranchAddress("tau.clusterphi",tau_clusterphi);
   fChain->SetBranchAddress("tau.etaeta",tau_etaeta);
   fChain->SetBranchAddress("tau.phiphi",tau_phiphi);
   fChain->SetBranchAddress("tau.delr",tau_delr);
   fChain->SetBranchAddress("tau.emenergy",tau_emenergy);
   fChain->SetBranchAddress("tau.hadenergy",tau_hadenergy);
   fChain->SetBranchAddress("tau.emet",tau_emet);
   fChain->SetBranchAddress("tau.hadet",tau_hadet);
   fChain->SetBranchAddress("tau.caloetiso",tau_caloetiso);
   fChain->SetBranchAddress("tau.calohadetiso",tau_calohadetiso);
   fChain->SetBranchAddress("tau.caloemetiso",tau_caloemetiso);
   fChain->SetBranchAddress("tau.refvtxz",tau_refvtxz);
   fChain->SetBranchAddress("tau.trackspt",tau_trackspt);
   fChain->SetBranchAddress("tau.trkscalarptsum",tau_trkscalarptsum);
   fChain->SetBranchAddress("tau.tracksmass",tau_tracksmass);
   fChain->SetBranchAddress("tau.trackseta",tau_trackseta);
   fChain->SetBranchAddress("tau.tracksphi",tau_tracksphi);
   fChain->SetBranchAddress("tau.trackiso",tau_trackiso);
   fChain->SetBranchAddress("tau.seedtrkvz",tau_seedtrkvz);
   fChain->SetBranchAddress("tau.seedtrkpt",tau_seedtrkpt);
   fChain->SetBranchAddress("tau.angleseedtocluster",tau_angleseedtocluster);
   fChain->SetBranchAddress("tau.phiseedtocluster",tau_phiseedtocluster);
   fChain->SetBranchAddress("tau.etaseedtocluster",tau_etaseedtocluster);
   fChain->SetBranchAddress("tau.pi0trackmass",tau_pi0trackmass);
   fChain->SetBranchAddress("tau.pi0trackpt",tau_pi0trackpt);
   fChain->SetBranchAddress("tau.trkindex",tau_trkindex);
   fChain->SetBranchAddress("tau.et",tau_et);
   fChain->SetBranchAddress("tau.ep",tau_ep);
   fChain->SetBranchAddress("tau.ep_vis",tau_ep_vis);
   fChain->SetBranchAddress("tau.zces",tau_zces);
   fChain->SetBranchAddress("tau.d0",tau_d0);
   fChain->SetBranchAddress("tau.tracklinks[5]",tau_tracklinks);
   fChain->SetBranchAddress("tau.tracknaxialhits[5]",tau_tracknaxialhits);
   fChain->SetBranchAddress("tau.tracknstereohits[5]",tau_tracknstereohits);
   fChain->SetBranchAddress("tau.cutword",tau_cutword);
   fChain->SetBranchAddress("tau.conesize",tau_conesize);
   fChain->SetBranchAddress("tau.ehadoverp",tau_ehadoverp);
   fChain->SetBranchAddress("tau.calomass",tau_calomass);
   fChain->SetBranchAddress("tau.ntracksincone",tau_ntracksincone);
   fChain->SetBranchAddress("tau.chargeincone",tau_chargeincone);
   fChain->SetBranchAddress("tau.nisotracks",tau_nisotracks);
   fChain->SetBranchAddress("tau.nisopi0s",tau_nisopi0s);
   fChain->SetBranchAddress("tau.taugrouplikes",tau_taugrouplikes);
   fChain->SetBranchAddress("tau.pi0eta[5]",tau_pi0eta);
   fChain->SetBranchAddress("tau.pi0phi[5]",tau_pi0phi);
   fChain->SetBranchAddress("tau.pi0e[5]",tau_pi0e);
   fChain->SetBranchAddress("tau.pi0px[5]",tau_pi0px);
   fChain->SetBranchAddress("tau.pi0py[5]",tau_pi0py);
   fChain->SetBranchAddress("tau.pi0pz[5]",tau_pi0pz);
   fChain->SetBranchAddress("tau.pi0pt[5]",tau_pi0pt);
   fChain->SetBranchAddress("tau.pi0detectorz[5]",tau_pi0detectorz);
   fChain->SetBranchAddress("tau.pi0strip1rawenergy[5]",tau_pi0strip1rawenergy);
   fChain->SetBranchAddress("tau.pi0strip1fitpos[5]",tau_pi0strip1fitpos);
   fChain->SetBranchAddress("tau.pi0strip1width[5]",tau_pi0strip1width);
   fChain->SetBranchAddress("tau.pi0strip1chi2[5]",tau_pi0strip1chi2);
   fChain->SetBranchAddress("tau.pi0strip2rawenergy[5]",tau_pi0strip2rawenergy);
   fChain->SetBranchAddress("tau.pi0strip2fitpos[5]",tau_pi0strip2fitpos);
   fChain->SetBranchAddress("tau.pi0strip2width[5]",tau_pi0strip2width);
   fChain->SetBranchAddress("tau.pi0strip2chi2[5]",tau_pi0strip2chi2);
   fChain->SetBranchAddress("tau.pi0wire1rawenergy[5]",tau_pi0wire1rawenergy);
   fChain->SetBranchAddress("tau.pi0wire1fitpos[5]",tau_pi0wire1fitpos);
   fChain->SetBranchAddress("tau.pi0wire1width[5]",tau_pi0wire1width);
   fChain->SetBranchAddress("tau.pi0wire1chi2[5]",tau_pi0wire1chi2);
   fChain->SetBranchAddress("tau.pi0wire2rawenergy[5]",tau_pi0wire2rawenergy);
   fChain->SetBranchAddress("tau.pi0wire2fitpos[5]",tau_pi0wire2fitpos);
   fChain->SetBranchAddress("tau.pi0wire2width[5]",tau_pi0wire2width);
   fChain->SetBranchAddress("tau.pi0wire2chi2[5]",tau_pi0wire2chi2);
   fChain->SetBranchAddress("numTauObjs",&numTauObjs);
   fChain->SetBranchAddress("jet",&jet_);
   fChain->SetBranchAddress("jet.fUniqueID",jet_fUniqueID);
   fChain->SetBranchAddress("jet.fBits",jet_fBits);
   fChain->SetBranchAddress("jet.CollType",jet_CollType);
   fChain->SetBranchAddress("jet.Px",jet_Px);
   fChain->SetBranchAddress("jet.Py",jet_Py);
   fChain->SetBranchAddress("jet.Pz",jet_Pz);
   fChain->SetBranchAddress("jet.En",jet_En);
   fChain->SetBranchAddress("jet.EtaDetector",jet_EtaDetector);
   fChain->SetBranchAddress("jet.GuardEnergy",jet_GuardEnergy);
   fChain->SetBranchAddress("jet.EmFraction",jet_EmFraction);
   fChain->SetBranchAddress("jet.CentroidEta",jet_CentroidEta);
   fChain->SetBranchAddress("jet.CentroidPhi",jet_CentroidPhi);
   fChain->SetBranchAddress("jet.EtaMoment",jet_EtaMoment);
   fChain->SetBranchAddress("jet.PhiMoment",jet_PhiMoment);
   fChain->SetBranchAddress("jet.EtaPhiMoment",jet_EtaPhiMoment);
   fChain->SetBranchAddress("jet.CentroidEt",jet_CentroidEt);
   fChain->SetBranchAddress("jet.TotP",jet_TotP);
   fChain->SetBranchAddress("jet.TotPt",jet_TotPt);
   fChain->SetBranchAddress("jet.Pt",jet_Pt);
   fChain->SetBranchAddress("jet.PtSquared",jet_PtSquared);
   fChain->SetBranchAddress("jet.TotEt",jet_TotEt);
   fChain->SetBranchAddress("jet.Et",jet_Et);
   fChain->SetBranchAddress("jet.Eta",jet_Eta);
   fChain->SetBranchAddress("jet.Theta",jet_Theta);
   fChain->SetBranchAddress("jet.Phi",jet_Phi);
   fChain->SetBranchAddress("jet.MassSquared",jet_MassSquared);
   fChain->SetBranchAddress("jet.Mass",jet_Mass);
   fChain->SetBranchAddress("jet.Rapidity",jet_Rapidity);
   fChain->SetBranchAddress("jet.secvTag",jet_secvTag);
   fChain->SetBranchAddress("jet.secvPass",jet_secvPass);
   fChain->SetBranchAddress("jet.secvCharge",jet_secvCharge);
   fChain->SetBranchAddress("jet.secvL3d",jet_secvL3d);
   fChain->SetBranchAddress("jet.secvDl3d",jet_secvDl3d);
   fChain->SetBranchAddress("jet.secvL2d",jet_secvL2d);
   fChain->SetBranchAddress("jet.secvDl2d",jet_secvDl2d);
   fChain->SetBranchAddress("jet.secvLzd",jet_secvLzd);
   fChain->SetBranchAddress("jet.secvDlzd",jet_secvDlzd);
   fChain->SetBranchAddress("jet.secvChisq",jet_secvChisq);
   fChain->SetBranchAddress("jet.secvChisqr",jet_secvChisqr);
   fChain->SetBranchAddress("jet.secvChisqz",jet_secvChisqz);
   fChain->SetBranchAddress("jet.secvNtrk",jet_secvNtrk);
   fChain->SetBranchAddress("jet.secvNdtrk",jet_secvNdtrk);
   fChain->SetBranchAddress("jet.secvNvtrk",jet_secvNvtrk);
   fChain->SetBranchAddress("jet.secvMass",jet_secvMass);
   fChain->SetBranchAddress("jet.secvPt",jet_secvPt);
   fChain->SetBranchAddress("jet.secvPxVtx",jet_secvPxVtx);
   fChain->SetBranchAddress("jet.secvPyVtx",jet_secvPyVtx);
   fChain->SetBranchAddress("jet.secvPzVtx",jet_secvPzVtx);
   fChain->SetBranchAddress("jet.secvEVtx",jet_secvEVtx);
   fChain->SetBranchAddress("jet.secvX",jet_secvX);
   fChain->SetBranchAddress("jet.secvY",jet_secvY);
   fChain->SetBranchAddress("jet.secvZ",jet_secvZ);
   fChain->SetBranchAddress("jet.secvXXerr",jet_secvXXerr);
   fChain->SetBranchAddress("jet.secvYYerr",jet_secvYYerr);
   fChain->SetBranchAddress("jet.secvZZerr",jet_secvZZerr);
   fChain->SetBranchAddress("jet.secvXYerr",jet_secvXYerr);
   fChain->SetBranchAddress("jet.secvXZerr",jet_secvXZerr);
   fChain->SetBranchAddress("jet.secvYZerr",jet_secvYZerr);
   fChain->SetBranchAddress("jet.secvNPass1Tracks",jet_secvNPass1Tracks);
   fChain->SetBranchAddress("jet.secvNPass2Tracks",jet_secvNPass2Tracks);
   fChain->SetBranchAddress("jet.secvDz",jet_secvDz);
   fChain->SetBranchAddress("jet.secvPtSec",jet_secvPtSec);
   fChain->SetBranchAddress("jet.secvPtRelSec",jet_secvPtRelSec);
   fChain->SetBranchAddress("jet.secvPlRelSec",jet_secvPlRelSec);
   fChain->SetBranchAddress("jet.secvESeed",jet_secvESeed);
   fChain->SetBranchAddress("jet.secvPxSeed",jet_secvPxSeed);
   fChain->SetBranchAddress("jet.secvPySeed",jet_secvPySeed);
   fChain->SetBranchAddress("jet.secvPzSeed",jet_secvPzSeed);
   fChain->SetBranchAddress("jet.secvNLeptons",jet_secvNLeptons);
   fChain->SetBranchAddress("jet.secvLooseTag",jet_secvLooseTag);
   fChain->SetBranchAddress("jet.secvLoosePass",jet_secvLoosePass);
   fChain->SetBranchAddress("jet.secvLooseCharge",jet_secvLooseCharge);
   fChain->SetBranchAddress("jet.secvLooseL3d",jet_secvLooseL3d);
   fChain->SetBranchAddress("jet.secvLooseDl3d",jet_secvLooseDl3d);
   fChain->SetBranchAddress("jet.secvLooseL2d",jet_secvLooseL2d);
   fChain->SetBranchAddress("jet.secvLooseDl2d",jet_secvLooseDl2d);
   fChain->SetBranchAddress("jet.secvLooseLzd",jet_secvLooseLzd);
   fChain->SetBranchAddress("jet.secvLooseDlzd",jet_secvLooseDlzd);
   fChain->SetBranchAddress("jet.secvLooseChisq",jet_secvLooseChisq);
   fChain->SetBranchAddress("jet.secvLooseChisqr",jet_secvLooseChisqr);
   fChain->SetBranchAddress("jet.secvLooseChisqz",jet_secvLooseChisqz);
   fChain->SetBranchAddress("jet.secvLooseNtrk",jet_secvLooseNtrk);
   fChain->SetBranchAddress("jet.secvLooseNdtrk",jet_secvLooseNdtrk);
   fChain->SetBranchAddress("jet.secvLooseNvtrk",jet_secvLooseNvtrk);
   fChain->SetBranchAddress("jet.secvLooseMass",jet_secvLooseMass);
   fChain->SetBranchAddress("jet.secvLoosePt",jet_secvLoosePt);
   fChain->SetBranchAddress("jet.secvLoosePxVtx",jet_secvLoosePxVtx);
   fChain->SetBranchAddress("jet.secvLoosePyVtx",jet_secvLoosePyVtx);
   fChain->SetBranchAddress("jet.secvLoosePzVtx",jet_secvLoosePzVtx);
   fChain->SetBranchAddress("jet.secvLooseEVtx",jet_secvLooseEVtx);
   fChain->SetBranchAddress("jet.secvLooseX",jet_secvLooseX);
   fChain->SetBranchAddress("jet.secvLooseY",jet_secvLooseY);
   fChain->SetBranchAddress("jet.secvLooseZ",jet_secvLooseZ);
   fChain->SetBranchAddress("jet.secvLooseXXerr",jet_secvLooseXXerr);
   fChain->SetBranchAddress("jet.secvLooseYYerr",jet_secvLooseYYerr);
   fChain->SetBranchAddress("jet.secvLooseZZerr",jet_secvLooseZZerr);
   fChain->SetBranchAddress("jet.secvLooseXYerr",jet_secvLooseXYerr);
   fChain->SetBranchAddress("jet.secvLooseXZerr",jet_secvLooseXZerr);
   fChain->SetBranchAddress("jet.secvLooseYZerr",jet_secvLooseYZerr);
   fChain->SetBranchAddress("jet.secvLooseNPass1Tracks",jet_secvLooseNPass1Tracks);
   fChain->SetBranchAddress("jet.secvLooseNPass2Tracks",jet_secvLooseNPass2Tracks);
   fChain->SetBranchAddress("jet.secvLooseDz",jet_secvLooseDz);
   fChain->SetBranchAddress("jet.secvLoosePtSec",jet_secvLoosePtSec);
   fChain->SetBranchAddress("jet.secvLoosePtRelSec",jet_secvLoosePtRelSec);
   fChain->SetBranchAddress("jet.secvLoosePlRelSec",jet_secvLoosePlRelSec);
   fChain->SetBranchAddress("jet.secvLooseESeed",jet_secvLooseESeed);
   fChain->SetBranchAddress("jet.secvLoosePxSeed",jet_secvLoosePxSeed);
   fChain->SetBranchAddress("jet.secvLoosePySeed",jet_secvLoosePySeed);
   fChain->SetBranchAddress("jet.secvLoosePzSeed",jet_secvLoosePzSeed);
   fChain->SetBranchAddress("jet.secvLooseNLeptons",jet_secvLooseNLeptons);
   fChain->SetBranchAddress("jet.drB",jet_drB);
   fChain->SetBranchAddress("jet.drC",jet_drC);
   fChain->SetBranchAddress("jet.NTracks",jet_NTracks);
   fChain->SetBranchAddress("jet.jpbNTracks",jet_jpbNTracks);
   fChain->SetBranchAddress("jet.jpbJetEt",jet_jpbJetEt);
   fChain->SetBranchAddress("jet.jpbJetEta",jet_jpbJetEta);
   fChain->SetBranchAddress("jet.jpbJetPhi",jet_jpbJetPhi);
   fChain->SetBranchAddress("jet.jpbHFTag",jet_jpbHFTag);
   fChain->SetBranchAddress("jet.jpbHFHepId",jet_jpbHFHepId);
   fChain->SetBranchAddress("jet.jpbHFdR",jet_jpbHFdR);
   fChain->SetBranchAddress("jet.jpbRPunsign",jet_jpbRPunsign);
   fChain->SetBranchAddress("jet.jpbRPpos",jet_jpbRPpos);
   fChain->SetBranchAddress("jet.jpbRPneg",jet_jpbRPneg);
   fChain->SetBranchAddress("jet.jpbNGdRPTrk",jet_jpbNGdRPTrk);
   fChain->SetBranchAddress("jet.jpbNGdRPTrkPos",jet_jpbNGdRPTrkPos);
   fChain->SetBranchAddress("jet.jpbNGdRPTrkNeg",jet_jpbNGdRPTrkNeg);
   fChain->SetBranchAddress("jet.jpbRZunsign",jet_jpbRZunsign);
   fChain->SetBranchAddress("jet.jpbRZpos",jet_jpbRZpos);
   fChain->SetBranchAddress("jet.jpbRZneg",jet_jpbRZneg);
   fChain->SetBranchAddress("jet.jpbNGdRZTrk",jet_jpbNGdRZTrk);
   fChain->SetBranchAddress("jet.jpbNGdRZTrkPos",jet_jpbNGdRZTrkPos);
   fChain->SetBranchAddress("jet.jpbNGdRZTrkNeg",jet_jpbNGdRZTrkNeg);
   fChain->SetBranchAddress("jet.jpb3Dunsign",jet_jpb3Dunsign);
   fChain->SetBranchAddress("jet.jpb3Dpos",jet_jpb3Dpos);
   fChain->SetBranchAddress("jet.jpb3Dneg",jet_jpb3Dneg);
   fChain->SetBranchAddress("jet.jpbNGd3DTrk",jet_jpbNGd3DTrk);
   fChain->SetBranchAddress("jet.jpbNGd3DTrkPos",jet_jpbNGd3DTrkPos);
   fChain->SetBranchAddress("jet.jpbNGd3DTrkNeg",jet_jpbNGd3DTrkNeg);
   fChain->SetBranchAddress("numJetObjs",&numJetObjs);
   fChain->SetBranchAddress("jetCollDescr1",jetCollDescr1);
   fChain->SetBranchAddress("jetCollDescr2",jetCollDescr2);
   fChain->SetBranchAddress("jetCollDescr3",jetCollDescr3);
   fChain->SetBranchAddress("jetCollDescr4",jetCollDescr4);
   fChain->SetBranchAddress("jetCollDescr5",jetCollDescr5);
   fChain->SetBranchAddress("jetCollDescr6",jetCollDescr6);
   fChain->SetBranchAddress("jetCollDescr7",jetCollDescr7);
   fChain->SetBranchAddress("jetCollDescr8",jetCollDescr8);
   fChain->SetBranchAddress("jetCollDescr9",jetCollDescr9);
   fChain->SetBranchAddress("jetCollDescr10",jetCollDescr10);
   fChain->SetBranchAddress("jetCollDescr11",jetCollDescr11);
   fChain->SetBranchAddress("jetCollDescr12",jetCollDescr12);
   fChain->SetBranchAddress("jetCollDescr13",jetCollDescr13);
   fChain->SetBranchAddress("jetCollDescr14",jetCollDescr14);
   fChain->SetBranchAddress("jetCollDescr15",jetCollDescr15);
   fChain->SetBranchAddress("met",&met_);
   fChain->SetBranchAddress("met.fUniqueID",met_fUniqueID);
   fChain->SetBranchAddress("met.fBits",met_fBits);
   fChain->SetBranchAddress("met.Met",met_Met);
   fChain->SetBranchAddress("met.Phi",met_Phi);
   fChain->SetBranchAddress("met.ZV",met_ZV);
   fChain->SetBranchAddress("met.ESum",met_ESum);
   fChain->SetBranchAddress("met.EtSum",met_EtSum);
   fChain->SetBranchAddress("met.ExSum",met_ExSum);
   fChain->SetBranchAddress("met.EySum",met_EySum);
   fChain->SetBranchAddress("numMetObjs",&numMetObjs);
   fChain->SetBranchAddress("topfit",&topfit_);
   fChain->SetBranchAddress("topfit.fUniqueID",&topfit_fUniqueID);
   fChain->SetBranchAddress("topfit.fBits",&topfit_fBits);
   fChain->SetBranchAddress("topfit.Chi2",&topfit_Chi2);
   fChain->SetBranchAddress("topfit.Mass",&topfit_Mass);
   fChain->SetBranchAddress("topfit.ErrPa",&topfit_ErrPa);
   fChain->SetBranchAddress("topfit.ErrPl",&topfit_ErrPl);
   fChain->SetBranchAddress("topfit.ErrMi",&topfit_ErrMi);
   fChain->SetBranchAddress("topfit.config",&topfit_config);
   fChain->SetBranchAddress("topfit.tagok",&topfit_tagok);
   fChain->SetBranchAddress("topfit.jettheta[4]",&topfit_jettheta);
   fChain->SetBranchAddress("topfit.jetphi[4]",&topfit_jetphi);
   fChain->SetBranchAddress("topfit.jetptraw[4]",&topfit_jetptraw);
   fChain->SetBranchAddress("topfit.jetptgeneric[4]",&topfit_jetptgeneric);
   fChain->SetBranchAddress("topfit.jetptgg[4]",&topfit_jetptgg);
   fChain->SetBranchAddress("topfit.jetsigma[4]",&topfit_jetsigma);
   fChain->SetBranchAddress("topfit.jetptout[4]",&topfit_jetptout);
   fChain->SetBranchAddress("numTopFitObjs",&numTopFitObjs);
   fChain->SetBranchAddress("numTopFitBtags",&numTopFitBtags);
   fChain->SetBranchAddress("trigInfo",&trigInfo_);
   fChain->SetBranchAddress("trigInfo.fUniqueID",trigInfo_fUniqueID);
   fChain->SetBranchAddress("trigInfo.fBits",trigInfo_fBits);
   fChain->SetBranchAddress("trigInfo.L1W1NoPS",trigInfo_L1W1NoPS);
   fChain->SetBranchAddress("trigInfo.L1W2NoPS",trigInfo_L1W2NoPS);
   fChain->SetBranchAddress("trigInfo.L1W1PS",trigInfo_L1W1PS);
   fChain->SetBranchAddress("trigInfo.L1W2PS",trigInfo_L1W2PS);
   fChain->SetBranchAddress("trigInfo.L2W1",trigInfo_L2W1);
   fChain->SetBranchAddress("trigInfo.L2W2",trigInfo_L2W2);
   fChain->SetBranchAddress("trigInfo.L2W3",trigInfo_L2W3);
   fChain->SetBranchAddress("trigInfo.L2W4",trigInfo_L2W4);
   fChain->SetBranchAddress("trigInfo.L2W1NoPS",trigInfo_L2W1NoPS);
   fChain->SetBranchAddress("trigInfo.L2W2NoPS",trigInfo_L2W2NoPS);
   fChain->SetBranchAddress("trigInfo.L2W3NoPS",trigInfo_L2W3NoPS);
   fChain->SetBranchAddress("trigInfo.L2W4NoPS",trigInfo_L2W4NoPS);
   fChain->SetBranchAddress("trigInfo.L3W1",trigInfo_L3W1);
   fChain->SetBranchAddress("trigInfo.L3W2",trigInfo_L3W2);
   fChain->SetBranchAddress("trigInfo.L3W3",trigInfo_L3W3);
   fChain->SetBranchAddress("trigInfo.L3W4",trigInfo_L3W4);
   fChain->SetBranchAddress("trigInfo.L3W5",trigInfo_L3W5);
   fChain->SetBranchAddress("trigInfo.L3W6",trigInfo_L3W6);
   fChain->SetBranchAddress("trigInfo.L1sumetW1",trigInfo_L1sumetW1);
   fChain->SetBranchAddress("trigInfo.L1sumetW2",trigInfo_L1sumetW2);
   fChain->SetBranchAddress("trigInfo.L1cal",trigInfo_L1cal);
   fChain->SetBranchAddress("trigInfo.L1mu",trigInfo_L1mu);
   fChain->SetBranchAddress("trigInfo.L1trk",trigInfo_L1trk);
   fChain->SetBranchAddress("trigInfo.L1bsc",trigInfo_L1bsc);
   fChain->SetBranchAddress("trigInfo.summaryWord",trigInfo_summaryWord);
   fChain->SetBranchAddress("trigBitDescr0",trigBitDescr0);
   fChain->SetBranchAddress("trigBitDescr1",trigBitDescr1);
   fChain->SetBranchAddress("trigBitDescr2",trigBitDescr2);
   fChain->SetBranchAddress("trigBitDescr3",trigBitDescr3);
   fChain->SetBranchAddress("trigBitDescr4",trigBitDescr4);
   fChain->SetBranchAddress("trigBitDescr5",trigBitDescr5);
   fChain->SetBranchAddress("trigBitDescr6",trigBitDescr6);
   fChain->SetBranchAddress("trigBitDescr7",trigBitDescr7);
   fChain->SetBranchAddress("trigBitDescr8",trigBitDescr8);
   fChain->SetBranchAddress("trigBitDescr9",trigBitDescr9);
   fChain->SetBranchAddress("trigName",&trigName_);
   fChain->SetBranchAddress("trigName.fUniqueID",trigName_fUniqueID);
   fChain->SetBranchAddress("trigName.fBits",trigName_fBits);
   fChain->SetBranchAddress("trigName.RunNumber",trigName_RunNumber);
   fChain->SetBranchAddress("trigName.NumL1Triggers",trigName_NumL1Triggers);
   fChain->SetBranchAddress("trigName.NumL2Triggers",trigName_NumL2Triggers);
   fChain->SetBranchAddress("trigName.NumL3Triggers",trigName_NumL3Triggers);
   fChain->SetBranchAddress("trigName.L1Bit[64]",trigName_L1Bit);
   fChain->SetBranchAddress("trigName.L2Bit[128]",trigName_L2Bit);
   fChain->SetBranchAddress("trigName.L3Bit[256]",trigName_L3Bit);
   fChain->SetBranchAddress("trigName.L1Name[64][64]",trigName_L1Name);
   fChain->SetBranchAddress("trigName.L2Name[128][64]",trigName_L2Name);
   fChain->SetBranchAddress("trigName.L3Name[256][64]",trigName_L3Name);
   fChain->SetBranchAddress("l2Cal",&l2Cal_);
   fChain->SetBranchAddress("l2Cal.fUniqueID",l2Cal_fUniqueID);
   fChain->SetBranchAddress("l2Cal.fBits",l2Cal_fBits);
   fChain->SetBranchAddress("l2Cal.etEmL2",l2Cal_etEmL2);
   fChain->SetBranchAddress("l2Cal.etHadL2",l2Cal_etHadL2);
   fChain->SetBranchAddress("l2Cal.ntowL2",l2Cal_ntowL2);
   fChain->SetBranchAddress("l2Cal.phiL2",l2Cal_phiL2);
   fChain->SetBranchAddress("l2Cal.etaL2",l2Cal_etaL2);
   fChain->SetBranchAddress("l2Cal.typL2",l2Cal_typL2);
   fChain->SetBranchAddress("numL2CalObjs",&numL2CalObjs);
   fChain->SetBranchAddress("offltrack",&offltrack_);
   fChain->SetBranchAddress("offltrack.fUniqueID",offltrack_fUniqueID);
   fChain->SetBranchAddress("offltrack.fBits",offltrack_fBits);
   fChain->SetBranchAddress("offltrack.Alg",offltrack_Alg);
   fChain->SetBranchAddress("offltrack.CollType",offltrack_CollType);
   fChain->SetBranchAddress("offltrack.PassesDefTrkCut",offltrack_PassesDefTrkCut);
   fChain->SetBranchAddress("offltrack.ObspId",offltrack_ObspId);
   fChain->SetBranchAddress("offltrack.Pt",offltrack_Pt);
   fChain->SetBranchAddress("offltrack.Id",offltrack_Id);
   fChain->SetBranchAddress("offltrack.IdOrig",offltrack_IdOrig);
   fChain->SetBranchAddress("offltrack.NumCTHitsAx",offltrack_NumCTHitsAx);
   fChain->SetBranchAddress("offltrack.NumCTHitsSt",offltrack_NumCTHitsSt);
   fChain->SetBranchAddress("offltrack.NumAxSeg",offltrack_NumAxSeg);
   fChain->SetBranchAddress("offltrack.NumStSeg",offltrack_NumStSeg);
   fChain->SetBranchAddress("offltrack.NumCTHits[8]",offltrack_NumCTHits);
   fChain->SetBranchAddress("offltrack.Lambda",offltrack_Lambda);
   fChain->SetBranchAddress("offltrack.Curv",offltrack_Curv);
   fChain->SetBranchAddress("offltrack.Z0",offltrack_Z0);
   fChain->SetBranchAddress("offltrack.D0",offltrack_D0);
   fChain->SetBranchAddress("offltrack.D0C",offltrack_D0C);
   fChain->SetBranchAddress("offltrack.Phi0",offltrack_Phi0);
   fChain->SetBranchAddress("offltrack.Eta",offltrack_Eta);
   fChain->SetBranchAddress("offltrack.Charge",offltrack_Charge);
   fChain->SetBranchAddress("offltrack.chi2",offltrack_chi2);
   fChain->SetBranchAddress("offltrack.chi2CT",offltrack_chi2CT);
   fChain->SetBranchAddress("offltrack.BCP",offltrack_BCP);
   fChain->SetBranchAddress("offltrack.BCPx",offltrack_BCPx);
   fChain->SetBranchAddress("offltrack.BCPy",offltrack_BCPy);
   fChain->SetBranchAddress("offltrack.BCPz",offltrack_BCPz);
   fChain->SetBranchAddress("offltrack.BCPt",offltrack_BCPt);
   fChain->SetBranchAddress("offltrack.BCEta",offltrack_BCEta);
   fChain->SetBranchAddress("offltrack.BCPhi0",offltrack_BCPhi0);
   fChain->SetBranchAddress("offltrack.BCZ0",offltrack_BCZ0);
   fChain->SetBranchAddress("offltrack.BCCharge",offltrack_BCCharge);
   fChain->SetBranchAddress("offltrack.SIHits",offltrack_SIHits);
   fChain->SetBranchAddress("offltrack.SIFit",offltrack_SIFit);
   fChain->SetBranchAddress("offltrack.SIGoodHitX",offltrack_SIGoodHitX);
   fChain->SetBranchAddress("offltrack.SIGoodHitZ",offltrack_SIGoodHitZ);
   fChain->SetBranchAddress("offltrack.SIGoodHitZ90",offltrack_SIGoodHitZ90);
   fChain->SetBranchAddress("offltrack.SIID",offltrack_SIID);
   fChain->SetBranchAddress("offltrack.SIPhi",offltrack_SIPhi);
   fChain->SetBranchAddress("offltrack.SID0",offltrack_SID0);
   fChain->SetBranchAddress("offltrack.SIPt",offltrack_SIPt);
   fChain->SetBranchAddress("offltrack.SIZ",offltrack_SIZ);
   fChain->SetBranchAddress("offltrack.SIZsig",offltrack_SIZsig);
   fChain->SetBranchAddress("offltrack.SID0sig",offltrack_SID0sig);
   fChain->SetBranchAddress("offltrack.SICot",offltrack_SICot);
   fChain->SetBranchAddress("offltrack.SIChis",offltrack_SIChis);
   fChain->SetBranchAddress("offltrack.SITotHitX",offltrack_SITotHitX);
   fChain->SetBranchAddress("offltrack.SITotHitZ",offltrack_SITotHitZ);
   fChain->SetBranchAddress("offltrack.SITotHitZ90",offltrack_SITotHitZ90);
   fChain->SetBranchAddress("offltrack.ErrCotCot",offltrack_ErrCotCot);
   fChain->SetBranchAddress("offltrack.ErrCotCrv",offltrack_ErrCotCrv);
   fChain->SetBranchAddress("offltrack.ErrCotZ0",offltrack_ErrCotZ0);
   fChain->SetBranchAddress("offltrack.ErrCotD0",offltrack_ErrCotD0);
   fChain->SetBranchAddress("offltrack.ErrCotPhi0",offltrack_ErrCotPhi0);
   fChain->SetBranchAddress("offltrack.ErrCrvCrv",offltrack_ErrCrvCrv);
   fChain->SetBranchAddress("offltrack.ErrCrvZ0",offltrack_ErrCrvZ0);
   fChain->SetBranchAddress("offltrack.ErrCrvD0",offltrack_ErrCrvD0);
   fChain->SetBranchAddress("offltrack.ErrCrvPhi0",offltrack_ErrCrvPhi0);
   fChain->SetBranchAddress("offltrack.ErrZ0Z0",offltrack_ErrZ0Z0);
   fChain->SetBranchAddress("offltrack.ErrZ0D0",offltrack_ErrZ0D0);
   fChain->SetBranchAddress("offltrack.ErrZ0Phi0",offltrack_ErrZ0Phi0);
   fChain->SetBranchAddress("offltrack.ErrD0D0",offltrack_ErrD0D0);
   fChain->SetBranchAddress("offltrack.ErrD0Phi0",offltrack_ErrD0Phi0);
   fChain->SetBranchAddress("offltrack.ErrPhi0Phi0",offltrack_ErrPhi0Phi0);
   fChain->SetBranchAddress("offltrack.CalEmTower",offltrack_CalEmTower);
   fChain->SetBranchAddress("offltrack.CalHadTower",offltrack_CalHadTower);
   fChain->SetBranchAddress("offltrack.CalEm3x3Iso",offltrack_CalEm3x3Iso);
   fChain->SetBranchAddress("offltrack.CalHad3x3Iso",offltrack_CalHad3x3Iso);
   fChain->SetBranchAddress("offltrack.SiExpectedLayers",offltrack_SiExpectedLayers);
   fChain->SetBranchAddress("offltrack.trkIsol",offltrack_trkIsol);
   fChain->SetBranchAddress("offltrack.corrD0",offltrack_corrD0);
   fChain->SetBranchAddress("numOfflTracks",&numOfflTracks);
   fChain->SetBranchAddress("otrkCollDescr1",otrkCollDescr1);
   fChain->SetBranchAddress("otrkCollDescr2",otrkCollDescr2);
   fChain->SetBranchAddress("otrkCollDescr3",otrkCollDescr3);
   fChain->SetBranchAddress("otrkCollDescr4",otrkCollDescr4);
   fChain->SetBranchAddress("otrkCollDescr5",otrkCollDescr5);
   fChain->SetBranchAddress("secvtxtrack",&secvtxtrack_);
   fChain->SetBranchAddress("secvtxtrack.fUniqueID",secvtxtrack_fUniqueID);
   fChain->SetBranchAddress("secvtxtrack.fBits",secvtxtrack_fBits);
   fChain->SetBranchAddress("secvtxtrack.alg",secvtxtrack_alg);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitx",secvtxtrack_tNGoodHitx);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitz",secvtxtrack_tNGoodHitz);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitSz",secvtxtrack_tNGoodHitSz);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitx_ISL",secvtxtrack_tNGoodHitx_ISL);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitSz_ISL",secvtxtrack_tNGoodHitSz_ISL);
   fChain->SetBranchAddress("secvtxtrack.tNGoodHitx_L00",secvtxtrack_tNGoodHitx_L00);
   fChain->SetBranchAddress("secvtxtrack.tId",secvtxtrack_tId);
   fChain->SetBranchAddress("secvtxtrack.tIdDerived",secvtxtrack_tIdDerived);
   fChain->SetBranchAddress("secvtxtrack.tIdOrig",secvtxtrack_tIdOrig);
   fChain->SetBranchAddress("secvtxtrack.trkLink",secvtxtrack_trkLink);
   fChain->SetBranchAddress("secvtxtrack.sPt",secvtxtrack_sPt);
   fChain->SetBranchAddress("secvtxtrack.tEta",secvtxtrack_tEta);
   fChain->SetBranchAddress("secvtxtrack.tCur",secvtxtrack_tCur);
   fChain->SetBranchAddress("secvtxtrack.tPhi0",secvtxtrack_tPhi0);
   fChain->SetBranchAddress("secvtxtrack.tD0",secvtxtrack_tD0);
   fChain->SetBranchAddress("secvtxtrack.tZ0",secvtxtrack_tZ0);
   fChain->SetBranchAddress("secvtxtrack.tCot",secvtxtrack_tCot);
   fChain->SetBranchAddress("secvtxtrack.tCurE",secvtxtrack_tCurE);
   fChain->SetBranchAddress("secvtxtrack.tPhi0E",secvtxtrack_tPhi0E);
   fChain->SetBranchAddress("secvtxtrack.tD0E",secvtxtrack_tD0E);
   fChain->SetBranchAddress("secvtxtrack.tZ0E",secvtxtrack_tZ0E);
   fChain->SetBranchAddress("secvtxtrack.tCotE",secvtxtrack_tCotE);
   fChain->SetBranchAddress("secvtxtrack.sD0",secvtxtrack_sD0);
   fChain->SetBranchAddress("secvtxtrack.sD0s",secvtxtrack_sD0s);
   fChain->SetBranchAddress("secvtxtrack.sZ0",secvtxtrack_sZ0);
   fChain->SetBranchAddress("secvtxtrack.sZ0s",secvtxtrack_sZ0s);
   fChain->SetBranchAddress("secvtxtrack.sB0",secvtxtrack_sB0);
   fChain->SetBranchAddress("secvtxtrack.sB0s",secvtxtrack_sB0s);
   fChain->SetBranchAddress("secvtxtrack.sChi2X",secvtxtrack_sChi2X);
   fChain->SetBranchAddress("secvtxtrack.sNHitX",secvtxtrack_sNHitX);
   fChain->SetBranchAddress("secvtxtrack.sNHitSZ",secvtxtrack_sNHitSZ);
   fChain->SetBranchAddress("secvtxtrack.sNHitZ",secvtxtrack_sNHitZ);
   fChain->SetBranchAddress("secvtxtrack.sNHitX_ISL",secvtxtrack_sNHitX_ISL);
   fChain->SetBranchAddress("secvtxtrack.sNHitSZ_ISL",secvtxtrack_sNHitSZ_ISL);
   fChain->SetBranchAddress("secvtxtrack.sNHitX_L00",secvtxtrack_sNHitX_L00);
   fChain->SetBranchAddress("secvtxtrack.sType",secvtxtrack_sType);
   fChain->SetBranchAddress("secvtxtrack.nCotHits",secvtxtrack_nCotHits);
   fChain->SetBranchAddress("secvtxtrack.tKey1",secvtxtrack_tKey1);
   fChain->SetBranchAddress("secvtxtrack.tKey2",secvtxtrack_tKey2);
   fChain->SetBranchAddress("secvtxtrack.tIsGoodTrack",secvtxtrack_tIsGoodTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsPass1Track",secvtxtrack_tIsPass1Track);
   fChain->SetBranchAddress("secvtxtrack.tIsPass2Track",secvtxtrack_tIsPass2Track);
   fChain->SetBranchAddress("secvtxtrack.tIsGoodZTrack",secvtxtrack_tIsGoodZTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsUsedTrack",secvtxtrack_tIsUsedTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsVeeTrack",secvtxtrack_tIsVeeTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsGoodLooseTrack",secvtxtrack_tIsGoodLooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsPass1LooseTrack",secvtxtrack_tIsPass1LooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsPass2LooseTrack",secvtxtrack_tIsPass2LooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsGoodZLooseTrack",secvtxtrack_tIsGoodZLooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsUsedLooseTrack",secvtxtrack_tIsUsedLooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tIsVeeLooseTrack",secvtxtrack_tIsVeeLooseTrack);
   fChain->SetBranchAddress("secvtxtrack.tNSharedHits",secvtxtrack_tNSharedHits);
   fChain->SetBranchAddress("secvtxtrack.tMaxClusterLength",secvtxtrack_tMaxClusterLength);
   fChain->SetBranchAddress("secvtxtrack.tMaxClusterLengthPhi",secvtxtrack_tMaxClusterLengthPhi);
   fChain->SetBranchAddress("secvtxtrack.tMaxClusterLengthZ",secvtxtrack_tMaxClusterLengthZ);
   fChain->SetBranchAddress("secvtxtrack.tMaxClusterLengthSas",secvtxtrack_tMaxClusterLengthSas);
   fChain->SetBranchAddress("secvtxtrack.tNIntersections",secvtxtrack_tNIntersections);
   fChain->SetBranchAddress("secvtxtrack.tNGoodLayers",secvtxtrack_tNGoodLayers);
   fChain->SetBranchAddress("secvtxtrack.tNErrors",secvtxtrack_tNErrors);
   fChain->SetBranchAddress("secvtxtrack.tHitPattern",secvtxtrack_tHitPattern);
   fChain->SetBranchAddress("secvtxtrack.tErrorLayers",secvtxtrack_tErrorLayers);
   fChain->SetBranchAddress("secvtxtrack.tIntegratedLayers",secvtxtrack_tIntegratedLayers);
   fChain->SetBranchAddress("secvtxtrack.tHitPatternZ",secvtxtrack_tHitPatternZ);
   fChain->SetBranchAddress("secvtxtrack.tErrorLayersZ",secvtxtrack_tErrorLayersZ);
   fChain->SetBranchAddress("secvtxtrack.tIntegratedLayersZ",secvtxtrack_tIntegratedLayersZ);
   fChain->SetBranchAddress("secvtxtrack.tNMissedLayers",secvtxtrack_tNMissedLayers);
   fChain->SetBranchAddress("secvtxtrack.tNMissedLayersZ",secvtxtrack_tNMissedLayersZ);
   fChain->SetBranchAddress("secvtxtrack.tNMissedLayersISL",secvtxtrack_tNMissedLayersISL);
   fChain->SetBranchAddress("secvtxtrack.tNMissedLayersZISL",secvtxtrack_tNMissedLayersZISL);
   fChain->SetBranchAddress("secvtxtrack.tNMissedLayersL00",secvtxtrack_tNMissedLayersL00);
   fChain->SetBranchAddress("secvtxtrack.tWhyBadISLsz",secvtxtrack_tWhyBadISLsz);
   fChain->SetBranchAddress("secvtxtrack.tWhyBadISLx",secvtxtrack_tWhyBadISLx);
   fChain->SetBranchAddress("secvtxtrack.tWhyBadsz",secvtxtrack_tWhyBadsz);
   fChain->SetBranchAddress("secvtxtrack.tWhyBadz",secvtxtrack_tWhyBadz);
   fChain->SetBranchAddress("secvtxtrack.tWhyBadx",secvtxtrack_tWhyBadx);
   fChain->SetBranchAddress("secvtxtrack.tIsLepton",secvtxtrack_tIsLepton);
   fChain->SetBranchAddress("secvtxtrack.tIsSvt",secvtxtrack_tIsSvt);
   fChain->SetBranchAddress("secvtxtrack.tIsSuggested",secvtxtrack_tIsSuggested);
   fChain->SetBranchAddress("secvtxtrack.tIsRequired",secvtxtrack_tIsRequired);
   fChain->SetBranchAddress("secvtxtrack.tIsSkipped",secvtxtrack_tIsSkipped);
   fChain->SetBranchAddress("secvtxtrack.tGenpIndex",secvtxtrack_tGenpIndex);
   fChain->SetBranchAddress("secvtxtrack.tCov[15]",secvtxtrack_tCov);
   fChain->SetBranchAddress("secvtxtrack.is_HFdaughter",secvtxtrack_is_HFdaughter);
   fChain->SetBranchAddress("numSecvtxTracks",&numSecvtxTracks);
   fChain->SetBranchAddress("tjassoc",&tjassoc_);
   fChain->SetBranchAddress("tjassoc.fUniqueID",tjassoc_fUniqueID);
   fChain->SetBranchAddress("tjassoc.fBits",tjassoc_fBits);
   fChain->SetBranchAddress("tjassoc.jetid",tjassoc_jetid);
   fChain->SetBranchAddress("tjassoc.trkid",tjassoc_trkid);
   fChain->SetBranchAddress("tjassoc.acode",tjassoc_acode);
   fChain->SetBranchAddress("numTJAssoc",&numTJAssoc);
   fChain->SetBranchAddress("tvassoc",&tvassoc_);
   fChain->SetBranchAddress("tvassoc.fUniqueID",tvassoc_fUniqueID);
   fChain->SetBranchAddress("tvassoc.fBits",tvassoc_fBits);
   fChain->SetBranchAddress("tvassoc.vtxid",tvassoc_vtxid);
   fChain->SetBranchAddress("tvassoc.trkid",tvassoc_trkid);
   fChain->SetBranchAddress("numTVAssoc",&numTVAssoc);
   fChain->SetBranchAddress("jetprobtrack",&jetprobtrack_);
   fChain->SetBranchAddress("jetprobtrack.fUniqueID",jetprobtrack_fUniqueID);
   fChain->SetBranchAddress("jetprobtrack.fBits",jetprobtrack_fBits);
   fChain->SetBranchAddress("jetprobtrack.tId",jetprobtrack_tId);
   fChain->SetBranchAddress("jetprobtrack.tPt",jetprobtrack_tPt);
   fChain->SetBranchAddress("jetprobtrack.tTheta",jetprobtrack_tTheta);
   fChain->SetBranchAddress("jetprobtrack.tEta",jetprobtrack_tEta);
   fChain->SetBranchAddress("jetprobtrack.tPhi0",jetprobtrack_tPhi0);
   fChain->SetBranchAddress("jetprobtrack.tNSvxLayer",jetprobtrack_tNSvxLayer);
   fChain->SetBranchAddress("jetprobtrack.tNSvxHit",jetprobtrack_tNSvxHit);
   fChain->SetBranchAddress("jetprobtrack.tNSvxPhiHit",jetprobtrack_tNSvxPhiHit);
   fChain->SetBranchAddress("jetprobtrack.tNSvxStHit",jetprobtrack_tNSvxStHit);
   fChain->SetBranchAddress("jetprobtrack.tNSvxZHit",jetprobtrack_tNSvxZHit);
   fChain->SetBranchAddress("jetprobtrack.tTotNSvxRPhiHit",jetprobtrack_tTotNSvxRPhiHit);
   fChain->SetBranchAddress("jetprobtrack.tTotNGdSvxRPhiHit",jetprobtrack_tTotNGdSvxRPhiHit);
   fChain->SetBranchAddress("jetprobtrack.tD0",jetprobtrack_tD0);
   fChain->SetBranchAddress("jetprobtrack.tD0Err",jetprobtrack_tD0Err);
   fChain->SetBranchAddress("jetprobtrack.tSignD0",jetprobtrack_tSignD0);
   fChain->SetBranchAddress("jetprobtrack.tZ0",jetprobtrack_tZ0);
   fChain->SetBranchAddress("jetprobtrack.tZ0Err",jetprobtrack_tZ0Err);
   fChain->SetBranchAddress("jetprobtrack.tSignZ0",jetprobtrack_tSignZ0);
   fChain->SetBranchAddress("jetprobtrack.tProbRPhi",jetprobtrack_tProbRPhi);
   fChain->SetBranchAddress("jetprobtrack.tProbRZ",jetprobtrack_tProbRZ);
   fChain->SetBranchAddress("jetprobtrack.tProb3D",jetprobtrack_tProb3D);
   fChain->SetBranchAddress("jetprobtrack.tNCotHit",jetprobtrack_tNCotHit);
   fChain->SetBranchAddress("jetprobtrack.tNCotAxHit",jetprobtrack_tNCotAxHit);
   fChain->SetBranchAddress("jetprobtrack.tNCotStHit",jetprobtrack_tNCotStHit);
   fChain->SetBranchAddress("jetprobtrack.tTrackQtyRPhi",jetprobtrack_tTrackQtyRPhi);
   fChain->SetBranchAddress("jetprobtrack.tTrackQtyRZ",jetprobtrack_tTrackQtyRZ);
   fChain->SetBranchAddress("jetprobtrack.tLongLiveFlagRPhi",jetprobtrack_tLongLiveFlagRPhi);
   fChain->SetBranchAddress("jetprobtrack.tLongLiveFlagRZ",jetprobtrack_tLongLiveFlagRZ);
   fChain->SetBranchAddress("jetprobtrack.tIsGoodRPhi",jetprobtrack_tIsGoodRPhi);
   fChain->SetBranchAddress("jetprobtrack.tIsGoodRZ",jetprobtrack_tIsGoodRZ);
   fChain->SetBranchAddress("numJetProbTracks",&numJetProbTracks);
   fChain->SetBranchAddress("jetprobTJassoc",&jetprobTJassoc_);
   fChain->SetBranchAddress("jetprobTJassoc.fUniqueID",jetprobTJassoc_fUniqueID);
   fChain->SetBranchAddress("jetprobTJassoc.fBits",jetprobTJassoc_fBits);
   fChain->SetBranchAddress("jetprobTJassoc.jetid",jetprobTJassoc_jetid);
   fChain->SetBranchAddress("jetprobTJassoc.trkid",jetprobTJassoc_trkid);
   fChain->SetBranchAddress("numJetProbTJAssoc",&numJetProbTJAssoc);
   fChain->SetBranchAddress("obsp",&obsp_);
   fChain->SetBranchAddress("obsp.fUniqueID",obsp_fUniqueID);
   fChain->SetBranchAddress("obsp.fBits",obsp_fBits);
   fChain->SetBranchAddress("obsp.index",obsp_index);
   fChain->SetBranchAddress("obsp.cdfid",obsp_cdfid);
   fChain->SetBranchAddress("obsp.hepglink",obsp_hepglink);
   fChain->SetBranchAddress("obsp.dkmode",obsp_dkmode);
   fChain->SetBranchAddress("obsp.px",obsp_px);
   fChain->SetBranchAddress("obsp.py",obsp_py);
   fChain->SetBranchAddress("obsp.pz",obsp_pz);
   fChain->SetBranchAddress("obsp.m",obsp_m);
   fChain->SetBranchAddress("obsp.qcharge",obsp_qcharge);
   fChain->SetBranchAddress("obsp.path",obsp_path);
   fChain->SetBranchAddress("obsp.motherid",obsp_motherid);
   fChain->SetBranchAddress("obsp.nda",obsp_nda);
   fChain->SetBranchAddress("obsp.Vx",obsp_Vx);
   fChain->SetBranchAddress("obsp.Vy",obsp_Vy);
   fChain->SetBranchAddress("obsp.Vz",obsp_Vz);
   fChain->SetBranchAddress("obsp.Vt",obsp_Vt);
   fChain->SetBranchAddress("obsp.is_bdaughter",obsp_is_bdaughter);
   fChain->SetBranchAddress("obsp.is_cdaughter",obsp_is_cdaughter);
   fChain->SetBranchAddress("obsp.beauty",obsp_beauty);
   fChain->SetBranchAddress("obsp.charm",obsp_charm);
   fChain->SetBranchAddress("obsp.jmo1",obsp_jmo1);
   fChain->SetBranchAddress("obsp.jda1",obsp_jda1);
   fChain->SetBranchAddress("obsp.jda2",obsp_jda2);
   fChain->SetBranchAddress("numObspObjs",&numObspObjs);
   Notify();
}

Bool_t SkimFCNC::Notify()
{
   // Called when loading a new file.
   // Get branch pointers.
   b_evt_ = fChain->GetBranch("evt");
   b_evt_fUniqueID = fChain->GetBranch("evt.fUniqueID");
   b_evt_fBits = fChain->GetBranch("evt.fBits");
   b_evt_runNumber = fChain->GetBranch("evt.runNumber");
   b_evt_eventNumber = fChain->GetBranch("evt.eventNumber");
   b_evt_runSection = fChain->GetBranch("evt.runSection");
   b_evt_CosmicOOTLow = fChain->GetBranch("evt.CosmicOOTLow");
   b_evt_bunchNum159 = fChain->GetBranch("evt.bunchNum159");
   b_evt_bunchNum36 = fChain->GetBranch("evt.bunchNum36");
   b_evt_gliveInstLumi = fChain->GetBranch("evt.gliveInstLumi");
   b_evt_gliveTotalLumi = fChain->GetBranch("evt.gliveTotalLumi");
   b_evt_instLumi = fChain->GetBranch("evt.instLumi");
   b_evt_totalLumi = fChain->GetBranch("evt.totalLumi");
   b_evt_scalerInstLumi = fChain->GetBranch("evt.scalerInstLumi");
   b_evt_scalerTotalLumi = fChain->GetBranch("evt.scalerTotalLumi");
   b_evt_bunchInstLumi = fChain->GetBranch("evt.bunchInstLumi");
   b_evt_gliveBunchInstLumi = fChain->GetBranch("evt.gliveBunchInstLumi");
   b_evt_bunchTotalInstLumi = fChain->GetBranch("evt.bunchTotalInstLumi");
   b_evt_creationTime = fChain->GetBranch("evt.creationTime");
   b_summary_ = fChain->GetBranch("summary");
   b_summary_fUniqueID = fChain->GetBranch("summary.fUniqueID");
   b_summary_fBits = fChain->GetBranch("summary.fBits");
   b_summary_fTopEventClass = fChain->GetBranch("summary.fTopEventClass");
   b_summary_fnTightEle = fChain->GetBranch("summary.fnTightEle");
   b_summary_fnLooseEle = fChain->GetBranch("summary.fnLooseEle");
   b_summary_fnTightMuo = fChain->GetBranch("summary.fnTightMuo");
   b_summary_fnLooseMuo = fChain->GetBranch("summary.fnLooseMuo");
   b_summary_fnTightTau = fChain->GetBranch("summary.fnTightTau");
   b_summary_fnLooseTau = fChain->GetBranch("summary.fnLooseTau");
   b_summary_fnTightLepton = fChain->GetBranch("summary.fnTightLepton");
   b_summary_fnLooseLepton = fChain->GetBranch("summary.fnLooseLepton");
   b_summary_fnTrackLepton = fChain->GetBranch("summary.fnTrackLepton");
   b_summary_fnUniqueTrackLepton = fChain->GetBranch("summary.fnUniqueTrackLepton");
   b_summary_fnUniqueLooseLepton = fChain->GetBranch("summary.fnUniqueLooseLepton");
   b_summary_fnUniqueDilLepton = fChain->GetBranch("summary.fnUniqueDilLepton");
   b_summary_fnTightJet = fChain->GetBranch("summary.fnTightJet");
   b_summary_fnLooseJet = fChain->GetBranch("summary.fnLooseJet");
   b_summary_frawMet = fChain->GetBranch("summary.frawMet");
   b_summary_frawMetPhi = fChain->GetBranch("summary.frawMetPhi");
   b_summary_fvtxMet = fChain->GetBranch("summary.fvtxMet");
   b_summary_fvtxMetPhi = fChain->GetBranch("summary.fvtxMetPhi");
   b_summary_fmuoMet = fChain->GetBranch("summary.fmuoMet");
   b_summary_fmuoMetPhi = fChain->GetBranch("summary.fmuoMetPhi");
   b_summary_ftlMet = fChain->GetBranch("summary.ftlMet");
   b_summary_ftlMetPhi = fChain->GetBranch("summary.ftlMetPhi");
   b_summary_fdilMet = fChain->GetBranch("summary.fdilMet");
   b_summary_fdilMetPhi = fChain->GetBranch("summary.fdilMetPhi");
   b_summary_ftlJetMet = fChain->GetBranch("summary.ftlJetMet");
   b_summary_ftlJetMetPhi = fChain->GetBranch("summary.ftlJetMetPhi");
   b_summary_fdilJetMet = fChain->GetBranch("summary.fdilJetMet");
   b_summary_fdilJetMetPhi = fChain->GetBranch("summary.fdilJetMetPhi");
   b_summary_fljJetMet = fChain->GetBranch("summary.fljJetMet");
   b_summary_fljJetMetPhi = fChain->GetBranch("summary.fljJetMetPhi");
   b_summary_fhadSumEt = fChain->GetBranch("summary.fhadSumEt");
   b_summary_fhadSumEt3 = fChain->GetBranch("summary.fhadSumEt3");
   b_summary_fhadCentrality = fChain->GetBranch("summary.fhadCentrality");
   b_summary_fhadAplanarity = fChain->GetBranch("summary.fhadAplanarity");
   b_summary_fhadSphericity = fChain->GetBranch("summary.fhadSphericity");
   b_summary_fExUnclustered = fChain->GetBranch("summary.fExUnclustered");
   b_summary_fEyUnclustered = fChain->GetBranch("summary.fEyUnclustered");
   b_summary_fprimZV = fChain->GetBranch("summary.fprimZV");
   b_summary_fjetZV = fChain->GetBranch("summary.fjetZV");
   b_summary_fprimZVerr = fChain->GetBranch("summary.fprimZVerr");
   b_summary_fjetZVerr = fChain->GetBranch("summary.fjetZVerr");
   b_summary_fdileptonHT = fChain->GetBranch("summary.fdileptonHT");
   b_summary_flepjetsHT = fChain->GetBranch("summary.flepjetsHT");
   b_privertex_ = fChain->GetBranch("privertex");
   b_privertex_fUniqueID = fChain->GetBranch("privertex.fUniqueID");
   b_privertex_fBits = fChain->GetBranch("privertex.fBits");
   b_privertex_sbm_x0 = fChain->GetBranch("privertex.sbm_x0");
   b_privertex_sbm_y0 = fChain->GetBranch("privertex.sbm_y0");
   b_privertex_sbm_slx = fChain->GetBranch("privertex.sbm_slx");
   b_privertex_sbm_sly = fChain->GetBranch("privertex.sbm_sly");
   b_privertex_cbm_x0 = fChain->GetBranch("privertex.cbm_x0");
   b_privertex_cbm_y0 = fChain->GetBranch("privertex.cbm_y0");
   b_privertex_cbm_slx = fChain->GetBranch("privertex.cbm_slx");
   b_privertex_cbm_sly = fChain->GetBranch("privertex.cbm_sly");
   b_privertex_x = fChain->GetBranch("privertex.x");
   b_privertex_y = fChain->GetBranch("privertex.y");
   b_privertex_z = fChain->GetBranch("privertex.z");
   b_privertex_covxx = fChain->GetBranch("privertex.covxx");
   b_privertex_covyy = fChain->GetBranch("privertex.covyy");
   b_privertex_covzz = fChain->GetBranch("privertex.covzz");
   b_privertex_covxy = fChain->GetBranch("privertex.covxy");
   b_privertex_covxz = fChain->GetBranch("privertex.covxz");
   b_privertex_covyz = fChain->GetBranch("privertex.covyz");
   b_privertex_mcx = fChain->GetBranch("privertex.mcx");
   b_privertex_mcy = fChain->GetBranch("privertex.mcy");
   b_privertex_mcz = fChain->GetBranch("privertex.mcz");
   b_privertex_SecVtxVx = fChain->GetBranch("privertex.SecVtxVx");
   b_privertex_SecVtxVy = fChain->GetBranch("privertex.SecVtxVy");
   b_privertex_SecVtxVz = fChain->GetBranch("privertex.SecVtxVz");
   b_privertex_SecVtxC11 = fChain->GetBranch("privertex.SecVtxC11");
   b_privertex_SecVtxC22 = fChain->GetBranch("privertex.SecVtxC22");
   b_privertex_SecVtxC33 = fChain->GetBranch("privertex.SecVtxC33");
   b_privertex_SecVtxC12 = fChain->GetBranch("privertex.SecVtxC12");
   b_privertex_SecVtxC13 = fChain->GetBranch("privertex.SecVtxC13");
   b_privertex_SecVtxC23 = fChain->GetBranch("privertex.SecVtxC23");
   b_privertex_nPrimeVtxTracks = fChain->GetBranch("privertex.nPrimeVtxTracks");
   b_privertex_primeVtxCharge = fChain->GetBranch("privertex.primeVtxCharge");
   b_privertex_primeVtxChiSq = fChain->GetBranch("privertex.primeVtxChiSq");
   b_privertex_primeVtxNdof = fChain->GetBranch("privertex.primeVtxNdof");
   b_zvtxs_ = fChain->GetBranch("zvtxs");
   b_zvtxs_fUniqueID = fChain->GetBranch("zvtxs.fUniqueID");
   b_zvtxs_fBits = fChain->GetBranch("zvtxs.fBits");
   b_zvtxs_z_pos = fChain->GetBranch("zvtxs.z_pos");
   b_zvtxs_z_err = fChain->GetBranch("zvtxs.z_err");
   b_zvtxs_n_trk = fChain->GetBranch("zvtxs.n_trk");
   b_zvtxs_sum_pt = fChain->GetBranch("zvtxs.sum_pt");
   b_zvtxs_quality = fChain->GetBranch("zvtxs.quality");
   b_numZVtxs = fChain->GetBranch("numZVtxs");
   b_hepg_ = fChain->GetBranch("hepg");
   b_hepg_fUniqueID = fChain->GetBranch("hepg.fUniqueID");
   b_hepg_fBits = fChain->GetBranch("hepg.fBits");
   b_hepg_Ind = fChain->GetBranch("hepg.Ind");
   b_hepg_ID = fChain->GetBranch("hepg.ID");
   b_hepg_IDparent = fChain->GetBranch("hepg.IDparent");
   b_hepg_Stdhep = fChain->GetBranch("hepg.Stdhep");
   b_hepg_Mo1 = fChain->GetBranch("hepg.Mo1");
   b_hepg_Mo2 = fChain->GetBranch("hepg.Mo2");
   b_hepg_Da1 = fChain->GetBranch("hepg.Da1");
   b_hepg_Da2 = fChain->GetBranch("hepg.Da2");
   b_hepg_Charge = fChain->GetBranch("hepg.Charge");
   b_hepg_Px = fChain->GetBranch("hepg.Px");
   b_hepg_Py = fChain->GetBranch("hepg.Py");
   b_hepg_Pz = fChain->GetBranch("hepg.Pz");
   b_hepg_E = fChain->GetBranch("hepg.E");
   b_hepg_Mass = fChain->GetBranch("hepg.Mass");
   b_numHepgObjs = fChain->GetBranch("numHepgObjs");
   b_muon_ = fChain->GetBranch("muon");
   b_muon_fUniqueID = fChain->GetBranch("muon.fUniqueID");
   b_muon_fBits = fChain->GetBranch("muon.fBits");
   b_muon_ViewType = fChain->GetBranch("muon.ViewType");
   b_muon_muontype = fChain->GetBranch("muon.muontype");
   b_muon_Fiducial = fChain->GetBranch("muon.Fiducial");
   b_muon_trkLink = fChain->GetBranch("muon.trkLink");
   b_muon_TrkFiducial = fChain->GetBranch("muon.TrkFiducial");
   b_muon_CmuFidD = fChain->GetBranch("muon.CmuFidD");
   b_muon_CmuFidX = fChain->GetBranch("muon.CmuFidX");
   b_muon_CmuFidZ = fChain->GetBranch("muon.CmuFidZ");
   b_muon_CmpFidD = fChain->GetBranch("muon.CmpFidD");
   b_muon_CmpFidX = fChain->GetBranch("muon.CmpFidX");
   b_muon_CmpFidZ = fChain->GetBranch("muon.CmpFidZ");
   b_muon_CmxFidD = fChain->GetBranch("muon.CmxFidD");
   b_muon_CmxFidX = fChain->GetBranch("muon.CmxFidX");
   b_muon_CmxFidZ = fChain->GetBranch("muon.CmxFidZ");
   b_muon_inBluebeam = fChain->GetBranch("muon.inBluebeam");
   b_muon_inMiniskirt = fChain->GetBranch("muon.inMiniskirt");
   b_muon_inKeystone = fChain->GetBranch("muon.inKeystone");
   b_muon_En = fChain->GetBranch("muon.En");
   b_muon_P = fChain->GetBranch("muon.P");
   b_muon_Px = fChain->GetBranch("muon.Px");
   b_muon_Py = fChain->GetBranch("muon.Py");
   b_muon_Pz = fChain->GetBranch("muon.Pz");
   b_muon_Pt = fChain->GetBranch("muon.Pt");
   b_muon_Eta = fChain->GetBranch("muon.Eta");
   b_muon_Phi0 = fChain->GetBranch("muon.Phi0");
   b_muon_Z0 = fChain->GetBranch("muon.Z0");
   b_muon_Charge = fChain->GetBranch("muon.Charge");
   b_muon_D0 = fChain->GetBranch("muon.D0");
   b_muon_PtCorr = fChain->GetBranch("muon.PtCorr");
   b_muon_PxCorr = fChain->GetBranch("muon.PxCorr");
   b_muon_PyCorr = fChain->GetBranch("muon.PyCorr");
   b_muon_PzCorr = fChain->GetBranch("muon.PzCorr");
   b_muon_PCorr = fChain->GetBranch("muon.PCorr");
   b_muon_BCP = fChain->GetBranch("muon.BCP");
   b_muon_BCPx = fChain->GetBranch("muon.BCPx");
   b_muon_BCPy = fChain->GetBranch("muon.BCPy");
   b_muon_BCPz = fChain->GetBranch("muon.BCPz");
   b_muon_BCPt = fChain->GetBranch("muon.BCPt");
   b_muon_BCEta = fChain->GetBranch("muon.BCEta");
   b_muon_BCPhi0 = fChain->GetBranch("muon.BCPhi0");
   b_muon_BCZ0 = fChain->GetBranch("muon.BCZ0");
   b_muon_BCCharge = fChain->GetBranch("muon.BCCharge");
   b_muon_RawP = fChain->GetBranch("muon.RawP");
   b_muon_RawPx = fChain->GetBranch("muon.RawPx");
   b_muon_RawPy = fChain->GetBranch("muon.RawPy");
   b_muon_RawPz = fChain->GetBranch("muon.RawPz");
   b_muon_RawPt = fChain->GetBranch("muon.RawPt");
   b_muon_RawEta = fChain->GetBranch("muon.RawEta");
   b_muon_RawPhi0 = fChain->GetBranch("muon.RawPhi0");
   b_muon_RawZ0 = fChain->GetBranch("muon.RawZ0");
   b_muon_RawCharge = fChain->GetBranch("muon.RawCharge");
   b_muon_TrkSiHits = fChain->GetBranch("muon.TrkSiHits");
   b_muon_TrkAxHits = fChain->GetBranch("muon.TrkAxHits");
   b_muon_TrkStHits = fChain->GetBranch("muon.TrkStHits");
   b_muon_TrkAxSeg = fChain->GetBranch("muon.TrkAxSeg");
   b_muon_TrkStSeg = fChain->GetBranch("muon.TrkStSeg");
   b_muon_TrkRedChi2CT = fChain->GetBranch("muon.TrkRedChi2CT");
   b_muon_TrkID = fChain->GetBranch("muon.TrkID");
   b_muon_TrkIDOrig = fChain->GetBranch("muon.TrkIDOrig");
   b_muon_HadEnergy = fChain->GetBranch("muon.HadEnergy");
   b_muon_EmEnergy = fChain->GetBranch("muon.EmEnergy");
   b_muon_EventZVertex = fChain->GetBranch("muon.EventZVertex");
   b_muon_DeltaZVertex = fChain->GetBranch("muon.DeltaZVertex");
   b_muon_Isol = fChain->GetBranch("muon.Isol");
   b_muon_EmIsol4 = fChain->GetBranch("muon.EmIsol4");
   b_muon_HadIsol4 = fChain->GetBranch("muon.HadIsol4");
   b_muon_TotalIsol4 = fChain->GetBranch("muon.TotalIsol4");
   b_muon_EmIsol7 = fChain->GetBranch("muon.EmIsol7");
   b_muon_HadIsol7 = fChain->GetBranch("muon.HadIsol7");
   b_muon_TotalIsol7 = fChain->GetBranch("muon.TotalIsol7");
   b_muon_TrkIsol = fChain->GetBranch("muon.TrkIsol");
   b_muon_CmuDx = fChain->GetBranch("muon.CmuDx");
   b_muon_CmuDz = fChain->GetBranch("muon.CmuDz");
   b_muon_CmuDphi = fChain->GetBranch("muon.CmuDphi");
   b_muon_CmpDx = fChain->GetBranch("muon.CmpDx");
   b_muon_CmpDphi = fChain->GetBranch("muon.CmpDphi");
   b_muon_CmxDx = fChain->GetBranch("muon.CmxDx");
   b_muon_CmxDz = fChain->GetBranch("muon.CmxDz");
   b_muon_CmxDphi = fChain->GetBranch("muon.CmxDphi");
   b_muon_BmuDx = fChain->GetBranch("muon.BmuDx");
   b_muon_Dx = fChain->GetBranch("muon.Dx[4]");
   b_muon_Wedge = fChain->GetBranch("muon.Wedge[4]");
   b_muon_Side = fChain->GetBranch("muon.Side[4]");
   b_muon_NumAssHits = fChain->GetBranch("muon.NumAssHits[4]");
   b_muon_CheckEn = fChain->GetBranch("muon.CheckEn");
   b_muon_CheckPx = fChain->GetBranch("muon.CheckPx");
   b_muon_CheckPy = fChain->GetBranch("muon.CheckPy");
   b_muon_CheckPz = fChain->GetBranch("muon.CheckPz");
   b_muon_CheckZ0 = fChain->GetBranch("muon.CheckZ0");
   b_muon_CheckZ0Err = fChain->GetBranch("muon.CheckZ0Err");
   b_muon_CheckD0 = fChain->GetBranch("muon.CheckD0");
   b_muon_CheckTrkId = fChain->GetBranch("muon.CheckTrkId");
   b_muon_CutCode = fChain->GetBranch("muon.CutCode");
   b_muon_BruceLLK = fChain->GetBranch("muon.BruceLLK");
   b_muon_goodForCMXTrig = fChain->GetBranch("muon.goodForCMXTrig");
   b_muon_Dz = fChain->GetBranch("muon.Dz[4]");
   b_muon_Dphi = fChain->GetBranch("muon.Dphi[4]");
   b_muon_Dtheta = fChain->GetBranch("muon.Dtheta[4]");
   b_muon_ChiSqX = fChain->GetBranch("muon.ChiSqX[4]");
   b_muon_ChiSqZ = fChain->GetBranch("muon.ChiSqZ[4]");
   b_muon_ChiSqXPos = fChain->GetBranch("muon.ChiSqXPos[4]");
   b_muon_ChiSqZPos = fChain->GetBranch("muon.ChiSqZPos[4]");
   b_muon_NumCntrHits = fChain->GetBranch("muon.NumCntrHits[4]");
   b_muon_HitPatt = fChain->GetBranch("muon.HitPatt[4]");
   b_muon_NumChamberHits = fChain->GetBranch("muon.NumChamberHits[4]");
   b_muon_PosX = fChain->GetBranch("muon.PosX[4]");
   b_muon_PosY = fChain->GetBranch("muon.PosY[4]");
   b_muon_PosZ = fChain->GetBranch("muon.PosZ[4]");
   b_muon_DirX = fChain->GetBranch("muon.DirX[4]");
   b_muon_DirY = fChain->GetBranch("muon.DirY[4]");
   b_muon_DirZ = fChain->GetBranch("muon.DirZ[4]");
   b_muon_TrkPosX = fChain->GetBranch("muon.TrkPosX[4]");
   b_muon_TrkPosY = fChain->GetBranch("muon.TrkPosY[4]");
   b_muon_TrkPosZ = fChain->GetBranch("muon.TrkPosZ[4]");
   b_muon_TrkDirX = fChain->GetBranch("muon.TrkDirX[4]");
   b_muon_TrkDirY = fChain->GetBranch("muon.TrkDirY[4]");
   b_muon_TrkDirZ = fChain->GetBranch("muon.TrkDirZ[4]");
   b_numMuObjs = fChain->GetBranch("numMuObjs");
   b_muoViewDescr1 = fChain->GetBranch("muoViewDescr1");
   b_muoViewDescr2 = fChain->GetBranch("muoViewDescr2");
   b_muoViewDescr3 = fChain->GetBranch("muoViewDescr3");
   b_muoViewDescr4 = fChain->GetBranch("muoViewDescr4");
   b_muoViewDescr5 = fChain->GetBranch("muoViewDescr5");
   b_sltmu_ = fChain->GetBranch("sltmu");
   b_sltmu_fUniqueID = fChain->GetBranch("sltmu.fUniqueID");
   b_sltmu_fBits = fChain->GetBranch("sltmu.fBits");
   b_sltmu_jetid = fChain->GetBranch("sltmu.jetid");
   b_sltmu_trkid = fChain->GetBranch("sltmu.trkid");
   b_sltmu_muid = fChain->GetBranch("sltmu.muid");
   b_sltmu_jetCollType = fChain->GetBranch("sltmu.jetCollType");
   b_sltmu_dRclstJet = fChain->GetBranch("sltmu.dRclstJet");
   b_sltmu_ptRel = fChain->GetBranch("sltmu.ptRel");
   b_sltmu_sumPt06 = fChain->GetBranch("sltmu.sumPt06");
   b_sltmu_nTrkJet06 = fChain->GetBranch("sltmu.nTrkJet06");
   b_sltmu_sumPt06C1 = fChain->GetBranch("sltmu.sumPt06C1");
   b_sltmu_nTrkJet06C1 = fChain->GetBranch("sltmu.nTrkJet06C1");
   b_sltmu_sumPt06C2 = fChain->GetBranch("sltmu.sumPt06C2");
   b_sltmu_nTrkJet06C2 = fChain->GetBranch("sltmu.nTrkJet06C2");
   b_sltmu_sumPt04 = fChain->GetBranch("sltmu.sumPt04");
   b_sltmu_nTrkJet04 = fChain->GetBranch("sltmu.nTrkJet04");
   b_sltmu_sumPt04C1 = fChain->GetBranch("sltmu.sumPt04C1");
   b_sltmu_nTrkJet04C1 = fChain->GetBranch("sltmu.nTrkJet04C1");
   b_sltmu_sumPt04C2 = fChain->GetBranch("sltmu.sumPt04C2");
   b_sltmu_nTrkJet04C2 = fChain->GetBranch("sltmu.nTrkJet04C2");
   b_sltmu_sumPt02 = fChain->GetBranch("sltmu.sumPt02");
   b_sltmu_nTrkJet02 = fChain->GetBranch("sltmu.nTrkJet02");
   b_sltmu_sumPt02C1 = fChain->GetBranch("sltmu.sumPt02C1");
   b_sltmu_nTrkJet02C1 = fChain->GetBranch("sltmu.nTrkJet02C1");
   b_sltmu_sumPt02C2 = fChain->GetBranch("sltmu.sumPt02C2");
   b_sltmu_nTrkJet02C2 = fChain->GetBranch("sltmu.nTrkJet02C2");
   b_sltmu_emE = fChain->GetBranch("sltmu.emE");
   b_sltmu_hadE = fChain->GetBranch("sltmu.hadE");
   b_sltmu_neightborEmE = fChain->GetBranch("sltmu.neightborEmE");
   b_sltmu_neightborHadE = fChain->GetBranch("sltmu.neightborHadE");
   b_sltmu_coneR4Et = fChain->GetBranch("sltmu.coneR4Et");
   b_sltmu_fiducial = fChain->GetBranch("sltmu.fiducial");
   b_sltmu_cmuFidSig = fChain->GetBranch("sltmu.cmuFidSig");
   b_sltmu_cmpFidSig = fChain->GetBranch("sltmu.cmpFidSig");
   b_sltmu_cmxFidSig = fChain->GetBranch("sltmu.cmxFidSig");
   b_sltmu_fiducialSig = fChain->GetBranch("sltmu.fiducialSig");
   b_sltmu_isTaggable = fChain->GetBranch("sltmu.isTaggable");
   b_sltmu_isTagged = fChain->GetBranch("sltmu.isTagged");
   b_sltmu_nUsedVar = fChain->GetBranch("sltmu.nUsedVar");
   b_sltmu_usedVar = fChain->GetBranch("sltmu.usedVar");
   b_sltmu_likelihood = fChain->GetBranch("sltmu.likelihood");
   b_sltmu_pullCmuDx = fChain->GetBranch("sltmu.pullCmuDx");
   b_sltmu_pullCmuDz = fChain->GetBranch("sltmu.pullCmuDz");
   b_sltmu_pullCmuDphi = fChain->GetBranch("sltmu.pullCmuDphi");
   b_sltmu_pullCmuHits = fChain->GetBranch("sltmu.pullCmuHits");
   b_sltmu_pullCmuChi2X = fChain->GetBranch("sltmu.pullCmuChi2X");
   b_sltmu_pullCmuChi2Z = fChain->GetBranch("sltmu.pullCmuChi2Z");
   b_sltmu_pullCmpDx = fChain->GetBranch("sltmu.pullCmpDx");
   b_sltmu_pullCmpDphi = fChain->GetBranch("sltmu.pullCmpDphi");
   b_sltmu_pullCmpChi2X = fChain->GetBranch("sltmu.pullCmpChi2X");
   b_sltmu_pullCmxDx = fChain->GetBranch("sltmu.pullCmxDx");
   b_sltmu_pullCmxDz = fChain->GetBranch("sltmu.pullCmxDz");
   b_sltmu_pullCmxDphi = fChain->GetBranch("sltmu.pullCmxDphi");
   b_sltmu_pullCmxChi2X = fChain->GetBranch("sltmu.pullCmxChi2X");
   b_sltmu_pullCmxChi2Z = fChain->GetBranch("sltmu.pullCmxChi2Z");
   b_sltmu_phiCot = fChain->GetBranch("sltmu.phiCot");
   b_sltmu_etaCot = fChain->GetBranch("sltmu.etaCot");
   b_sltmu_radLengths = fChain->GetBranch("sltmu.radLengths[4]");
   b_sltmu_absLengths = fChain->GetBranch("sltmu.absLengths[4]");
   b_sltmu_eLoss = fChain->GetBranch("sltmu.eLoss[4]");
   b_sltmu_extPosX = fChain->GetBranch("sltmu.extPosX[4]");
   b_sltmu_extPosY = fChain->GetBranch("sltmu.extPosY[4]");
   b_sltmu_extPosZ = fChain->GetBranch("sltmu.extPosZ[4]");
   b_sltmu_extDirX = fChain->GetBranch("sltmu.extDirX[4]");
   b_sltmu_extDirY = fChain->GetBranch("sltmu.extDirY[4]");
   b_sltmu_extDirZ = fChain->GetBranch("sltmu.extDirZ[4]");
   b_sltmu_dist = fChain->GetBranch("sltmu.dist[4]");
   b_numSltMuObjs = fChain->GetBranch("numSltMuObjs");
   b_cosmic_ = fChain->GetBranch("cosmic");
   b_cosmic_fUniqueID = fChain->GetBranch("cosmic.fUniqueID");
   b_cosmic_fBits = fChain->GetBranch("cosmic.fBits");
   b_cosmic_cosmicBit = fChain->GetBranch("cosmic.cosmicBit");
   b_cosmic_hasCosmicRay = fChain->GetBranch("cosmic.hasCosmicRay");
   b_cosmic_hasFewTracks = fChain->GetBranch("cosmic.hasFewTracks");
   b_cosmic_hasLargeD0 = fChain->GetBranch("cosmic.hasLargeD0");
   b_cosmic_hasLargeZ0 = fChain->GetBranch("cosmic.hasLargeZ0");
   b_cosmic_hasAcolTrack = fChain->GetBranch("cosmic.hasAcolTrack");
   b_cosmic_hasOOTEnergy = fChain->GetBranch("cosmic.hasOOTEnergy");
   b_cosmic_hasOOTScintillators = fChain->GetBranch("cosmic.hasOOTScintillators");
   b_cosmic_hasNoVertex = fChain->GetBranch("cosmic.hasNoVertex");
   b_cosmic_hasLargeDz = fChain->GetBranch("cosmic.hasLargeDz");
   b_cosmic_hasLargeDPhiMuStub = fChain->GetBranch("cosmic.hasLargeDPhiMuStub");
   b_cosmic_isMuonNothing = fChain->GetBranch("cosmic.isMuonNothing");
   b_cosmic_isMuonStub = fChain->GetBranch("cosmic.isMuonStub");
   b_cosmic_isMuonTrack = fChain->GetBranch("cosmic.isMuonTrack");
   b_cosmic_isMuonMuon = fChain->GetBranch("cosmic.isMuonMuon");
   b_cosmic_hasOOTTof = fChain->GetBranch("cosmic.hasOOTTof");
   b_cosmic_hasNotOutgoingPair = fChain->GetBranch("cosmic.hasNotOutgoingPair");
   b_cosmic_hasLowEmFrac = fChain->GetBranch("cosmic.hasLowEmFrac");
   b_cosmic_hasLowQFrac = fChain->GetBranch("cosmic.hasLowQFrac");
   b_cosmic_hasOOTCaloE = fChain->GetBranch("cosmic.hasOOTCaloE");
   b_cosmic_nLegs = fChain->GetBranch("cosmic.nLegs");
   b_cosmic_deltaTdc = fChain->GetBranch("cosmic.deltaTdc");
   b_cosmic_deltaTof = fChain->GetBranch("cosmic.deltaTof");
   b_cosmic_dPhiMuStub = fChain->GetBranch("cosmic.dPhiMuStub");
   b_cosmic_acollinearity = fChain->GetBranch("cosmic.acollinearity");
   b_cosmic_d0Rho = fChain->GetBranch("cosmic.d0Rho");
   b_cosmic_d0Beta = fChain->GetBranch("cosmic.d0Beta");
   b_cosmic_dPhi = fChain->GetBranch("cosmic.dPhi");
   b_cosmic_sumEta = fChain->GetBranch("cosmic.sumEta");
   b_cosmic_seedZ = fChain->GetBranch("cosmic.seedZ");
   b_cosmic_seedZErr = fChain->GetBranch("cosmic.seedZErr");
   b_cosmic_nTrkZVtx = fChain->GetBranch("cosmic.nTrkZVtx");
   b_cosmic_meanZVtx = fChain->GetBranch("cosmic.meanZVtx");
   b_cosmic_meanZVtxErr = fChain->GetBranch("cosmic.meanZVtxErr");
   b_cosmic_muoLnk = fChain->GetBranch("cosmic.muoLnk[2]");
   b_cosmic_trkLnk = fChain->GetBranch("cosmic.trkLnk[2]");
   b_cosmic_trackId = fChain->GetBranch("cosmic.trackId[2]");
   b_cosmic_pt = fChain->GetBranch("cosmic.pt[2]");
   b_cosmic_crv = fChain->GetBranch("cosmic.crv[2]");
   b_cosmic_cot = fChain->GetBranch("cosmic.cot[2]");
   b_cosmic_phi = fChain->GetBranch("cosmic.phi[2]");
   b_cosmic_eta = fChain->GetBranch("cosmic.eta[2]");
   b_cosmic_z0 = fChain->GetBranch("cosmic.z0[2]");
   b_cosmic_d0 = fChain->GetBranch("cosmic.d0[2]");
   b_cosmic_d0C = fChain->GetBranch("cosmic.d0C[2]");
   b_cosmic_charge = fChain->GetBranch("cosmic.charge[2]");
   b_cosmic_algo = fChain->GetBranch("cosmic.algo[2]");
   b_cosmic_deltaT0 = fChain->GetBranch("cosmic.deltaT0[2]");
   b_cosmic_errorDeltaT0 = fChain->GetBranch("cosmic.errorDeltaT0[2]");
   b_cosmic_deltaBetaInv = fChain->GetBranch("cosmic.deltaBetaInv[2]");
   b_cosmic_errorDeltaBetaInv = fChain->GetBranch("cosmic.errorDeltaBetaInv[2]");
   b_cosmic_fitT0 = fChain->GetBranch("cosmic.fitT0[2][2]");
   b_cosmic_fitdT0 = fChain->GetBranch("cosmic.fitdT0[2][2]");
   b_cosmic_fitCrdT0 = fChain->GetBranch("cosmic.fitCrdT0[2][2]");
   b_cosmic_fitChi2 = fChain->GetBranch("cosmic.fitChi2[2][2]");
   b_cosmic_fitdChi2 = fChain->GetBranch("cosmic.fitdChi2[2][2]");
   b_cosmic_fitDir = fChain->GetBranch("cosmic.fitDir[2][2]");
   b_cosmic_fitNCOTH = fChain->GetBranch("cosmic.fitNCOTH[2][2]");
   b_cosmic_fitNCOTHExp = fChain->GetBranch("cosmic.fitNCOTHExp[2][2]");
   b_cosmic_emE = fChain->GetBranch("cosmic.emE[2]");
   b_cosmic_hadE = fChain->GetBranch("cosmic.hadE[2]");
   b_cosmic_hadTDC = fChain->GetBranch("cosmic.hadTDC[2]");
   b_cosmic_ngbrEmE = fChain->GetBranch("cosmic.ngbrEmE[2]");
   b_cosmic_ngbrHadE = fChain->GetBranch("cosmic.ngbrHadE[2]");
   b_cosmic_ngbrHadTDC = fChain->GetBranch("cosmic.ngbrHadTDC[2]");
   b_cosmic_stubPhi = fChain->GetBranch("cosmic.stubPhi[2]");
   b_cosmic_tof = fChain->GetBranch("cosmic.tof[2]");
   b_cosmic_tofMethod = fChain->GetBranch("cosmic.tofMethod[2]");
   b_cosmic_csp = fChain->GetBranch("cosmic.csp[2]");
   b_cosmic_csx = fChain->GetBranch("cosmic.csx[2]");
   b_cosmic_validLeg = fChain->GetBranch("cosmic.validLeg[2]");
   b_cosmic_validTrack = fChain->GetBranch("cosmic.validTrack[2]");
   b_cosmic_validStub = fChain->GetBranch("cosmic.validStub[2]");
   b_cosmic_validEnergy = fChain->GetBranch("cosmic.validEnergy[2]");
   b_cosmic_validHadTdc = fChain->GetBranch("cosmic.validHadTdc[2]");
   b_cosmic_validTof = fChain->GetBranch("cosmic.validTof[2]");
   b_cosmic_validCsp = fChain->GetBranch("cosmic.validCsp[2]");
   b_cosmic_validCsx = fChain->GetBranch("cosmic.validCsx[2]");
   b_cosmic_trackLessMuon = fChain->GetBranch("cosmic.trackLessMuon[2]");
   b_cosmic_stubLessMuon = fChain->GetBranch("cosmic.stubLessMuon[2]");
   b_electron_ = fChain->GetBranch("electron");
   b_electron_fUniqueID = fChain->GetBranch("electron.fUniqueID");
   b_electron_fBits = fChain->GetBranch("electron.fBits");
   b_electron_CollType = fChain->GetBranch("electron.CollType");
   b_electron_Region = fChain->GetBranch("electron.Region");
   b_electron_Conversion = fChain->GetBranch("electron.Conversion");
   b_electron_trkLink = fChain->GetBranch("electron.trkLink");
   b_electron_phxTrkLink = fChain->GetBranch("electron.phxTrkLink");
   b_electron_En = fChain->GetBranch("electron.En");
   b_electron_RawEn = fChain->GetBranch("electron.RawEn");
   b_electron_CluEn = fChain->GetBranch("electron.CluEn");
   b_electron_Px = fChain->GetBranch("electron.Px");
   b_electron_RawPx = fChain->GetBranch("electron.RawPx");
   b_electron_CluPx = fChain->GetBranch("electron.CluPx");
   b_electron_Py = fChain->GetBranch("electron.Py");
   b_electron_RawPy = fChain->GetBranch("electron.RawPy");
   b_electron_CluPy = fChain->GetBranch("electron.CluPy");
   b_electron_Pz = fChain->GetBranch("electron.Pz");
   b_electron_RawPz = fChain->GetBranch("electron.RawPz");
   b_electron_CluPz = fChain->GetBranch("electron.CluPz");
   b_electron_Et = fChain->GetBranch("electron.Et");
   b_electron_RawEt = fChain->GetBranch("electron.RawEt");
   b_electron_CluEt = fChain->GetBranch("electron.CluEt");
   b_electron_Phi = fChain->GetBranch("electron.Phi");
   b_electron_RawPhi = fChain->GetBranch("electron.RawPhi");
   b_electron_CluPhi = fChain->GetBranch("electron.CluPhi");
   b_electron_Eta = fChain->GetBranch("electron.Eta");
   b_electron_RawEta = fChain->GetBranch("electron.RawEta");
   b_electron_CluEta = fChain->GetBranch("electron.CluEta");
   b_electron_Clu_totEt_jetZ = fChain->GetBranch("electron.Clu_totEt_jetZ");
   b_electron_Clu_phi_jetZ = fChain->GetBranch("electron.Clu_phi_jetZ");
   b_electron_TrkSiHits = fChain->GetBranch("electron.TrkSiHits");
   b_electron_TrkAxHits = fChain->GetBranch("electron.TrkAxHits");
   b_electron_TrkStHits = fChain->GetBranch("electron.TrkStHits");
   b_electron_TrkAxSeg = fChain->GetBranch("electron.TrkAxSeg");
   b_electron_TrkStSeg = fChain->GetBranch("electron.TrkStSeg");
   b_electron_TrkP = fChain->GetBranch("electron.TrkP");
   b_electron_BCP = fChain->GetBranch("electron.BCP");
   b_electron_RawTrkP = fChain->GetBranch("electron.RawTrkP");
   b_electron_TrkPx = fChain->GetBranch("electron.TrkPx");
   b_electron_BCPx = fChain->GetBranch("electron.BCPx");
   b_electron_RawTrkPx = fChain->GetBranch("electron.RawTrkPx");
   b_electron_TrkPy = fChain->GetBranch("electron.TrkPy");
   b_electron_BCPy = fChain->GetBranch("electron.BCPy");
   b_electron_RawTrkPy = fChain->GetBranch("electron.RawTrkPy");
   b_electron_TrkPz = fChain->GetBranch("electron.TrkPz");
   b_electron_BCPz = fChain->GetBranch("electron.BCPz");
   b_electron_RawTrkPz = fChain->GetBranch("electron.RawTrkPz");
   b_electron_TrkPt = fChain->GetBranch("electron.TrkPt");
   b_electron_BCPt = fChain->GetBranch("electron.BCPt");
   b_electron_RawTrkPt = fChain->GetBranch("electron.RawTrkPt");
   b_electron_TrkEta = fChain->GetBranch("electron.TrkEta");
   b_electron_BCEta = fChain->GetBranch("electron.BCEta");
   b_electron_RawTrkEta = fChain->GetBranch("electron.RawTrkEta");
   b_electron_TrkPhi0 = fChain->GetBranch("electron.TrkPhi0");
   b_electron_BCPhi0 = fChain->GetBranch("electron.BCPhi0");
   b_electron_RawTrkPhi0 = fChain->GetBranch("electron.RawTrkPhi0");
   b_electron_TrkZ0 = fChain->GetBranch("electron.TrkZ0");
   b_electron_BCZ0 = fChain->GetBranch("electron.BCZ0");
   b_electron_RawTrkZ0 = fChain->GetBranch("electron.RawTrkZ0");
   b_electron_Charge = fChain->GetBranch("electron.Charge");
   b_electron_BCCharge = fChain->GetBranch("electron.BCCharge");
   b_electron_RawTrkCharge = fChain->GetBranch("electron.RawTrkCharge");
   b_electron_TrkD0 = fChain->GetBranch("electron.TrkD0");
   b_electron_TrkID = fChain->GetBranch("electron.TrkID");
   b_electron_TrkIDOrig = fChain->GetBranch("electron.TrkIDOrig");
   b_electron_Fiducial = fChain->GetBranch("electron.Fiducial");
   b_electron_TrkFiducial = fChain->GetBranch("electron.TrkFiducial");
   b_electron_EtaDet = fChain->GetBranch("electron.EtaDet");
   b_electron_EtaEvent = fChain->GetBranch("electron.EtaEvent");
   b_electron_HadEt = fChain->GetBranch("electron.HadEt");
   b_electron_TotalEt = fChain->GetBranch("electron.TotalEt");
   b_electron_Hadem = fChain->GetBranch("electron.Hadem");
   b_electron_EmEt = fChain->GetBranch("electron.EmEt");
   b_electron_HadEn = fChain->GetBranch("electron.HadEn");
   b_electron_TotalEn = fChain->GetBranch("electron.TotalEn");
   b_electron_TotalIsol4 = fChain->GetBranch("electron.TotalIsol4");
   b_electron_TotalIsol7 = fChain->GetBranch("electron.TotalIsol7");
   b_electron_EmIsol4 = fChain->GetBranch("electron.EmIsol4");
   b_electron_EmIsol7 = fChain->GetBranch("electron.EmIsol7");
   b_electron_HadIsol4 = fChain->GetBranch("electron.HadIsol4");
   b_electron_HadIsol7 = fChain->GetBranch("electron.HadIsol7");
   b_electron_PprEn = fChain->GetBranch("electron.PprEn");
   b_electron_Pem3x3FitTow = fChain->GetBranch("electron.Pem3x3FitTow");
   b_electron_Pem3x3DetEta = fChain->GetBranch("electron.Pem3x3DetEta");
   b_electron_Pem3x3Phi = fChain->GetBranch("electron.Pem3x3Phi");
   b_electron_Pem3x3FitEn = fChain->GetBranch("electron.Pem3x3FitEn");
   b_electron_Pem3x3Chisq = fChain->GetBranch("electron.Pem3x3Chisq");
   b_electron_NumPes2d = fChain->GetBranch("electron.NumPes2d");
   b_electron_Pes2dEta = fChain->GetBranch("electron.Pes2dEta");
   b_electron_Pes2dPhi = fChain->GetBranch("electron.Pes2dPhi");
   b_electron_Pes2dX = fChain->GetBranch("electron.Pes2dX");
   b_electron_Pes2dY = fChain->GetBranch("electron.Pes2dY");
   b_electron_Pes2d5by9U = fChain->GetBranch("electron.Pes2d5by9U");
   b_electron_Pes2d5by9V = fChain->GetBranch("electron.Pes2d5by9V");
   b_electron_Pes2dEn = fChain->GetBranch("electron.Pes2dEn");
   b_electron_ClosestPes2d5by9U = fChain->GetBranch("electron.ClosestPes2d5by9U");
   b_electron_ClosestPes2d5by9V = fChain->GetBranch("electron.ClosestPes2d5by9V");
   b_electron_ClosestPes2dEn = fChain->GetBranch("electron.ClosestPes2dEn");
   b_electron_LshrTrk = fChain->GetBranch("electron.LshrTrk");
   b_electron_Lshr3 = fChain->GetBranch("electron.Lshr3");
   b_electron_Lshr2 = fChain->GetBranch("electron.Lshr2");
   b_electron_Isol = fChain->GetBranch("electron.Isol");
   b_electron_LeakE = fChain->GetBranch("electron.LeakE");
   b_electron_EP = fChain->GetBranch("electron.EP");
   b_electron_EventZVertex = fChain->GetBranch("electron.EventZVertex");
   b_electron_DeltaZVertex = fChain->GetBranch("electron.DeltaZVertex");
   b_electron_TrkCESx = fChain->GetBranch("electron.TrkCESx");
   b_electron_BestCESx = fChain->GetBranch("electron.BestCESx");
   b_electron_DeltaX = fChain->GetBranch("electron.DeltaX");
   b_electron_TrkCESz = fChain->GetBranch("electron.TrkCESz");
   b_electron_BestCESz = fChain->GetBranch("electron.BestCESz");
   b_electron_DeltaZ = fChain->GetBranch("electron.DeltaZ");
   b_electron_StripChi2 = fChain->GetBranch("electron.StripChi2");
   b_electron_WireChi2 = fChain->GetBranch("electron.WireChi2");
   b_electron_TrkIsol = fChain->GetBranch("electron.TrkIsol");
   b_electron_Towers = fChain->GetBranch("electron.Towers");
   b_electron_Tracks = fChain->GetBranch("electron.Tracks");
   b_electron_SeedEta = fChain->GetBranch("electron.SeedEta");
   b_electron_SeedPhi = fChain->GetBranch("electron.SeedPhi");
   b_electron_SeedEn = fChain->GetBranch("electron.SeedEn");
   b_electron_SeedEt = fChain->GetBranch("electron.SeedEt");
   b_electron_PhoZV = fChain->GetBranch("electron.PhoZV");
   b_electron_PhoRawEn = fChain->GetBranch("electron.PhoRawEn");
   b_electron_PhoRawEt = fChain->GetBranch("electron.PhoRawEt");
   b_electron_PhoEt = fChain->GetBranch("electron.PhoEt");
   b_electron_PhoPhi = fChain->GetBranch("electron.PhoPhi");
   b_electron_PhoEta = fChain->GetBranch("electron.PhoEta");
   b_electron_PhoCESx = fChain->GetBranch("electron.PhoCESx");
   b_electron_PhoCESz = fChain->GetBranch("electron.PhoCESz");
   b_electron_PhoEneStrip = fChain->GetBranch("electron.PhoEneStrip");
   b_electron_PhoEneWire = fChain->GetBranch("electron.PhoEneWire");
   b_electron_PhoStripChi2 = fChain->GetBranch("electron.PhoStripChi2");
   b_electron_PhoWireChi2 = fChain->GetBranch("electron.PhoWireChi2");
   b_electron_PhoCES2x = fChain->GetBranch("electron.PhoCES2x");
   b_electron_PhoCES2z = fChain->GetBranch("electron.PhoCES2z");
   b_electron_PhoEneStrip2 = fChain->GetBranch("electron.PhoEneStrip2");
   b_electron_PhoEneWire2 = fChain->GetBranch("electron.PhoEneWire2");
   b_electron_PhoCesSlide = fChain->GetBranch("electron.PhoCesSlide");
   b_electron_PhoN3D = fChain->GetBranch("electron.PhoN3D");
   b_electron_PhoPt = fChain->GetBranch("electron.PhoPt");
   b_electron_PhoIsol = fChain->GetBranch("electron.PhoIsol");
   b_electron_PhoTrkIsol = fChain->GetBranch("electron.PhoTrkIsol");
   b_electron_PhoLshr = fChain->GetBranch("electron.PhoLshr");
   b_electron_PhoHadem = fChain->GetBranch("electron.PhoHadem");
   b_electron_RawEnergyWire = fChain->GetBranch("electron.RawEnergyWire");
   b_electron_RawEnergyStrip = fChain->GetBranch("electron.RawEnergyStrip");
   b_electron_FitEnergyWire = fChain->GetBranch("electron.FitEnergyWire");
   b_electron_FitEnergyStrip = fChain->GetBranch("electron.FitEnergyStrip");
   b_electron_CESside = fChain->GetBranch("electron.CESside");
   b_electron_CESmodule = fChain->GetBranch("electron.CESmodule");
   b_electron_TrkCESside = fChain->GetBranch("electron.TrkCESside");
   b_electron_TrkCESmodule = fChain->GetBranch("electron.TrkCESmodule");
   b_electron_CPR = fChain->GetBranch("electron.CPR");
   b_electron_CPRside = fChain->GetBranch("electron.CPRside");
   b_electron_CPRmodule = fChain->GetBranch("electron.CPRmodule");
   b_electron_TrkCPRside = fChain->GetBranch("electron.TrkCPRside");
   b_electron_TrkCPRmodule = fChain->GetBranch("electron.TrkCPRmodule");
   b_electron_CPRx = fChain->GetBranch("electron.CPRx");
   b_electron_DeltaXCPR = fChain->GetBranch("electron.DeltaXCPR");
   b_electron_RawEnergyCPR = fChain->GetBranch("electron.RawEnergyCPR");
   b_electron_TrkCPRx = fChain->GetBranch("electron.TrkCPRx");
   b_electron_TrkCPRz = fChain->GetBranch("electron.TrkCPRz");
   b_electron_TrkCPRphi = fChain->GetBranch("electron.TrkCPRphi");
   b_electron_TrkCPReta = fChain->GetBranch("electron.TrkCPReta");
   b_electron_WidthWire = fChain->GetBranch("electron.WidthWire");
   b_electron_WidthStrip = fChain->GetBranch("electron.WidthStrip");
   b_electron_CheckEn = fChain->GetBranch("electron.CheckEn");
   b_electron_CheckPx = fChain->GetBranch("electron.CheckPx");
   b_electron_CheckPy = fChain->GetBranch("electron.CheckPy");
   b_electron_CheckPz = fChain->GetBranch("electron.CheckPz");
   b_electron_CheckZ0 = fChain->GetBranch("electron.CheckZ0");
   b_electron_CheckZ0Err = fChain->GetBranch("electron.CheckZ0Err");
   b_electron_CheckConv = fChain->GetBranch("electron.CheckConv");
   b_electron_CheckTrkId = fChain->GetBranch("electron.CheckTrkId");
   b_electron_CutCode = fChain->GetBranch("electron.CutCode");
   b_electron_PhxTrkSiHits = fChain->GetBranch("electron.PhxTrkSiHits");
   b_electron_PhxTrkP = fChain->GetBranch("electron.PhxTrkP");
   b_electron_PhxTrkPx = fChain->GetBranch("electron.PhxTrkPx");
   b_electron_PhxTrkPy = fChain->GetBranch("electron.PhxTrkPy");
   b_electron_PhxTrkPz = fChain->GetBranch("electron.PhxTrkPz");
   b_electron_PhxTrkPt = fChain->GetBranch("electron.PhxTrkPt");
   b_electron_PhxTrkEta = fChain->GetBranch("electron.PhxTrkEta");
   b_electron_PhxTrkPhi0 = fChain->GetBranch("electron.PhxTrkPhi0");
   b_electron_PhxTrkZ0 = fChain->GetBranch("electron.PhxTrkZ0");
   b_electron_PhxCharge = fChain->GetBranch("electron.PhxCharge");
   b_electron_PhxTrkD0 = fChain->GetBranch("electron.PhxTrkD0");
   b_electron_PhxMatch = fChain->GetBranch("electron.PhxMatch");
   b_electron_PhxCotMatchID = fChain->GetBranch("electron.PhxCotMatchID");
   b_electron_PhxDrTrkPes = fChain->GetBranch("electron.PhxDrTrkPes");
   b_electron_BruceLLK = fChain->GetBranch("electron.BruceLLK");
   b_numEmObjs = fChain->GetBranch("numEmObjs");
   b_eleViewDescr1 = fChain->GetBranch("eleViewDescr1");
   b_eleViewDescr2 = fChain->GetBranch("eleViewDescr2");
   b_eleViewDescr3 = fChain->GetBranch("eleViewDescr3");
   b_eleViewDescr4 = fChain->GetBranch("eleViewDescr4");
   b_eleViewDescr5 = fChain->GetBranch("eleViewDescr5");
   b_tau_ = fChain->GetBranch("tau");
   b_tau_fUniqueID = fChain->GetBranch("tau.fUniqueID");
   b_tau_fBits = fChain->GetBranch("tau.fBits");
   b_tau_ntowers = fChain->GetBranch("tau.ntowers");
   b_tau_nclumptowers = fChain->GetBranch("tau.nclumptowers");
   b_tau_ntrks = fChain->GetBranch("tau.ntrks");
   b_tau_ntrks10deg = fChain->GetBranch("tau.ntrks10deg");
   b_tau_ntrks30deg = fChain->GetBranch("tau.ntrks30deg");
   b_tau_ntrks10to30deg = fChain->GetBranch("tau.ntrks10to30deg");
   b_tau_charge10deg = fChain->GetBranch("tau.charge10deg");
   b_tau_nwrongvtxtrk10deg = fChain->GetBranch("tau.nwrongvtxtrk10deg");
   b_tau_npi0s = fChain->GetBranch("tau.npi0s");
   b_tau_nmustubs = fChain->GetBranch("tau.nmustubs");
   b_tau_nmuhits = fChain->GetBranch("tau.nmuhits");
   b_tau_px = fChain->GetBranch("tau.px");
   b_tau_py = fChain->GetBranch("tau.py");
   b_tau_pz = fChain->GetBranch("tau.pz");
   b_tau_e = fChain->GetBranch("tau.e");
   b_tau_pxtrk = fChain->GetBranch("tau.pxtrk");
   b_tau_pytrk = fChain->GetBranch("tau.pytrk");
   b_tau_pztrk = fChain->GetBranch("tau.pztrk");
   b_tau_etrk = fChain->GetBranch("tau.etrk");
   b_tau_pxvis = fChain->GetBranch("tau.pxvis");
   b_tau_pyvis = fChain->GetBranch("tau.pyvis");
   b_tau_pzvis = fChain->GetBranch("tau.pzvis");
   b_tau_evis = fChain->GetBranch("tau.evis");
   b_tau_emfrac = fChain->GetBranch("tau.emfrac");
   b_tau_clusterdeteta = fChain->GetBranch("tau.clusterdeteta");
   b_tau_seedtrkdeteta = fChain->GetBranch("tau.seedtrkdeteta");
   b_tau_clustereta = fChain->GetBranch("tau.clustereta");
   b_tau_clusterphi = fChain->GetBranch("tau.clusterphi");
   b_tau_etaeta = fChain->GetBranch("tau.etaeta");
   b_tau_phiphi = fChain->GetBranch("tau.phiphi");
   b_tau_delr = fChain->GetBranch("tau.delr");
   b_tau_emenergy = fChain->GetBranch("tau.emenergy");
   b_tau_hadenergy = fChain->GetBranch("tau.hadenergy");
   b_tau_emet = fChain->GetBranch("tau.emet");
   b_tau_hadet = fChain->GetBranch("tau.hadet");
   b_tau_caloetiso = fChain->GetBranch("tau.caloetiso");
   b_tau_calohadetiso = fChain->GetBranch("tau.calohadetiso");
   b_tau_caloemetiso = fChain->GetBranch("tau.caloemetiso");
   b_tau_refvtxz = fChain->GetBranch("tau.refvtxz");
   b_tau_trackspt = fChain->GetBranch("tau.trackspt");
   b_tau_trkscalarptsum = fChain->GetBranch("tau.trkscalarptsum");
   b_tau_tracksmass = fChain->GetBranch("tau.tracksmass");
   b_tau_trackseta = fChain->GetBranch("tau.trackseta");
   b_tau_tracksphi = fChain->GetBranch("tau.tracksphi");
   b_tau_trackiso = fChain->GetBranch("tau.trackiso");
   b_tau_seedtrkvz = fChain->GetBranch("tau.seedtrkvz");
   b_tau_seedtrkpt = fChain->GetBranch("tau.seedtrkpt");
   b_tau_angleseedtocluster = fChain->GetBranch("tau.angleseedtocluster");
   b_tau_phiseedtocluster = fChain->GetBranch("tau.phiseedtocluster");
   b_tau_etaseedtocluster = fChain->GetBranch("tau.etaseedtocluster");
   b_tau_pi0trackmass = fChain->GetBranch("tau.pi0trackmass");
   b_tau_pi0trackpt = fChain->GetBranch("tau.pi0trackpt");
   b_tau_trkindex = fChain->GetBranch("tau.trkindex");
   b_tau_et = fChain->GetBranch("tau.et");
   b_tau_ep = fChain->GetBranch("tau.ep");
   b_tau_ep_vis = fChain->GetBranch("tau.ep_vis");
   b_tau_zces = fChain->GetBranch("tau.zces");
   b_tau_d0 = fChain->GetBranch("tau.d0");
   b_tau_tracklinks = fChain->GetBranch("tau.tracklinks[5]");
   b_tau_tracknaxialhits = fChain->GetBranch("tau.tracknaxialhits[5]");
   b_tau_tracknstereohits = fChain->GetBranch("tau.tracknstereohits[5]");
   b_tau_cutword = fChain->GetBranch("tau.cutword");
   b_tau_conesize = fChain->GetBranch("tau.conesize");
   b_tau_ehadoverp = fChain->GetBranch("tau.ehadoverp");
   b_tau_calomass = fChain->GetBranch("tau.calomass");
   b_tau_ntracksincone = fChain->GetBranch("tau.ntracksincone");
   b_tau_chargeincone = fChain->GetBranch("tau.chargeincone");
   b_tau_nisotracks = fChain->GetBranch("tau.nisotracks");
   b_tau_nisopi0s = fChain->GetBranch("tau.nisopi0s");
   b_tau_taugrouplikes = fChain->GetBranch("tau.taugrouplikes");
   b_tau_pi0eta = fChain->GetBranch("tau.pi0eta[5]");
   b_tau_pi0phi = fChain->GetBranch("tau.pi0phi[5]");
   b_tau_pi0e = fChain->GetBranch("tau.pi0e[5]");
   b_tau_pi0px = fChain->GetBranch("tau.pi0px[5]");
   b_tau_pi0py = fChain->GetBranch("tau.pi0py[5]");
   b_tau_pi0pz = fChain->GetBranch("tau.pi0pz[5]");
   b_tau_pi0pt = fChain->GetBranch("tau.pi0pt[5]");
   b_tau_pi0detectorz = fChain->GetBranch("tau.pi0detectorz[5]");
   b_tau_pi0strip1rawenergy = fChain->GetBranch("tau.pi0strip1rawenergy[5]");
   b_tau_pi0strip1fitpos = fChain->GetBranch("tau.pi0strip1fitpos[5]");
   b_tau_pi0strip1width = fChain->GetBranch("tau.pi0strip1width[5]");
   b_tau_pi0strip1chi2 = fChain->GetBranch("tau.pi0strip1chi2[5]");
   b_tau_pi0strip2rawenergy = fChain->GetBranch("tau.pi0strip2rawenergy[5]");
   b_tau_pi0strip2fitpos = fChain->GetBranch("tau.pi0strip2fitpos[5]");
   b_tau_pi0strip2width = fChain->GetBranch("tau.pi0strip2width[5]");
   b_tau_pi0strip2chi2 = fChain->GetBranch("tau.pi0strip2chi2[5]");
   b_tau_pi0wire1rawenergy = fChain->GetBranch("tau.pi0wire1rawenergy[5]");
   b_tau_pi0wire1fitpos = fChain->GetBranch("tau.pi0wire1fitpos[5]");
   b_tau_pi0wire1width = fChain->GetBranch("tau.pi0wire1width[5]");
   b_tau_pi0wire1chi2 = fChain->GetBranch("tau.pi0wire1chi2[5]");
   b_tau_pi0wire2rawenergy = fChain->GetBranch("tau.pi0wire2rawenergy[5]");
   b_tau_pi0wire2fitpos = fChain->GetBranch("tau.pi0wire2fitpos[5]");
   b_tau_pi0wire2width = fChain->GetBranch("tau.pi0wire2width[5]");
   b_tau_pi0wire2chi2 = fChain->GetBranch("tau.pi0wire2chi2[5]");
   b_numTauObjs = fChain->GetBranch("numTauObjs");
   b_jet_ = fChain->GetBranch("jet");
   b_jet_fUniqueID = fChain->GetBranch("jet.fUniqueID");
   b_jet_fBits = fChain->GetBranch("jet.fBits");
   b_jet_CollType = fChain->GetBranch("jet.CollType");
   b_jet_Px = fChain->GetBranch("jet.Px");
   b_jet_Py = fChain->GetBranch("jet.Py");
   b_jet_Pz = fChain->GetBranch("jet.Pz");
   b_jet_En = fChain->GetBranch("jet.En");
   b_jet_EtaDetector = fChain->GetBranch("jet.EtaDetector");
   b_jet_GuardEnergy = fChain->GetBranch("jet.GuardEnergy");
   b_jet_EmFraction = fChain->GetBranch("jet.EmFraction");
   b_jet_CentroidEta = fChain->GetBranch("jet.CentroidEta");
   b_jet_CentroidPhi = fChain->GetBranch("jet.CentroidPhi");
   b_jet_EtaMoment = fChain->GetBranch("jet.EtaMoment");
   b_jet_PhiMoment = fChain->GetBranch("jet.PhiMoment");
   b_jet_EtaPhiMoment = fChain->GetBranch("jet.EtaPhiMoment");
   b_jet_CentroidEt = fChain->GetBranch("jet.CentroidEt");
   b_jet_TotP = fChain->GetBranch("jet.TotP");
   b_jet_TotPt = fChain->GetBranch("jet.TotPt");
   b_jet_Pt = fChain->GetBranch("jet.Pt");
   b_jet_PtSquared = fChain->GetBranch("jet.PtSquared");
   b_jet_TotEt = fChain->GetBranch("jet.TotEt");
   b_jet_Et = fChain->GetBranch("jet.Et");
   b_jet_Eta = fChain->GetBranch("jet.Eta");
   b_jet_Theta = fChain->GetBranch("jet.Theta");
   b_jet_Phi = fChain->GetBranch("jet.Phi");
   b_jet_MassSquared = fChain->GetBranch("jet.MassSquared");
   b_jet_Mass = fChain->GetBranch("jet.Mass");
   b_jet_Rapidity = fChain->GetBranch("jet.Rapidity");
   b_jet_secvTag = fChain->GetBranch("jet.secvTag");
   b_jet_secvPass = fChain->GetBranch("jet.secvPass");
   b_jet_secvCharge = fChain->GetBranch("jet.secvCharge");
   b_jet_secvL3d = fChain->GetBranch("jet.secvL3d");
   b_jet_secvDl3d = fChain->GetBranch("jet.secvDl3d");
   b_jet_secvL2d = fChain->GetBranch("jet.secvL2d");
   b_jet_secvDl2d = fChain->GetBranch("jet.secvDl2d");
   b_jet_secvLzd = fChain->GetBranch("jet.secvLzd");
   b_jet_secvDlzd = fChain->GetBranch("jet.secvDlzd");
   b_jet_secvChisq = fChain->GetBranch("jet.secvChisq");
   b_jet_secvChisqr = fChain->GetBranch("jet.secvChisqr");
   b_jet_secvChisqz = fChain->GetBranch("jet.secvChisqz");
   b_jet_secvNtrk = fChain->GetBranch("jet.secvNtrk");
   b_jet_secvNdtrk = fChain->GetBranch("jet.secvNdtrk");
   b_jet_secvNvtrk = fChain->GetBranch("jet.secvNvtrk");
   b_jet_secvMass = fChain->GetBranch("jet.secvMass");
   b_jet_secvPt = fChain->GetBranch("jet.secvPt");
   b_jet_secvPxVtx = fChain->GetBranch("jet.secvPxVtx");
   b_jet_secvPyVtx = fChain->GetBranch("jet.secvPyVtx");
   b_jet_secvPzVtx = fChain->GetBranch("jet.secvPzVtx");
   b_jet_secvEVtx = fChain->GetBranch("jet.secvEVtx");
   b_jet_secvX = fChain->GetBranch("jet.secvX");
   b_jet_secvY = fChain->GetBranch("jet.secvY");
   b_jet_secvZ = fChain->GetBranch("jet.secvZ");
   b_jet_secvXXerr = fChain->GetBranch("jet.secvXXerr");
   b_jet_secvYYerr = fChain->GetBranch("jet.secvYYerr");
   b_jet_secvZZerr = fChain->GetBranch("jet.secvZZerr");
   b_jet_secvXYerr = fChain->GetBranch("jet.secvXYerr");
   b_jet_secvXZerr = fChain->GetBranch("jet.secvXZerr");
   b_jet_secvYZerr = fChain->GetBranch("jet.secvYZerr");
   b_jet_secvNPass1Tracks = fChain->GetBranch("jet.secvNPass1Tracks");
   b_jet_secvNPass2Tracks = fChain->GetBranch("jet.secvNPass2Tracks");
   b_jet_secvDz = fChain->GetBranch("jet.secvDz");
   b_jet_secvPtSec = fChain->GetBranch("jet.secvPtSec");
   b_jet_secvPtRelSec = fChain->GetBranch("jet.secvPtRelSec");
   b_jet_secvPlRelSec = fChain->GetBranch("jet.secvPlRelSec");
   b_jet_secvESeed = fChain->GetBranch("jet.secvESeed");
   b_jet_secvPxSeed = fChain->GetBranch("jet.secvPxSeed");
   b_jet_secvPySeed = fChain->GetBranch("jet.secvPySeed");
   b_jet_secvPzSeed = fChain->GetBranch("jet.secvPzSeed");
   b_jet_secvNLeptons = fChain->GetBranch("jet.secvNLeptons");
   b_jet_secvLooseTag = fChain->GetBranch("jet.secvLooseTag");
   b_jet_secvLoosePass = fChain->GetBranch("jet.secvLoosePass");
   b_jet_secvLooseCharge = fChain->GetBranch("jet.secvLooseCharge");
   b_jet_secvLooseL3d = fChain->GetBranch("jet.secvLooseL3d");
   b_jet_secvLooseDl3d = fChain->GetBranch("jet.secvLooseDl3d");
   b_jet_secvLooseL2d = fChain->GetBranch("jet.secvLooseL2d");
   b_jet_secvLooseDl2d = fChain->GetBranch("jet.secvLooseDl2d");
   b_jet_secvLooseLzd = fChain->GetBranch("jet.secvLooseLzd");
   b_jet_secvLooseDlzd = fChain->GetBranch("jet.secvLooseDlzd");
   b_jet_secvLooseChisq = fChain->GetBranch("jet.secvLooseChisq");
   b_jet_secvLooseChisqr = fChain->GetBranch("jet.secvLooseChisqr");
   b_jet_secvLooseChisqz = fChain->GetBranch("jet.secvLooseChisqz");
   b_jet_secvLooseNtrk = fChain->GetBranch("jet.secvLooseNtrk");
   b_jet_secvLooseNdtrk = fChain->GetBranch("jet.secvLooseNdtrk");
   b_jet_secvLooseNvtrk = fChain->GetBranch("jet.secvLooseNvtrk");
   b_jet_secvLooseMass = fChain->GetBranch("jet.secvLooseMass");
   b_jet_secvLoosePt = fChain->GetBranch("jet.secvLoosePt");
   b_jet_secvLoosePxVtx = fChain->GetBranch("jet.secvLoosePxVtx");
   b_jet_secvLoosePyVtx = fChain->GetBranch("jet.secvLoosePyVtx");
   b_jet_secvLoosePzVtx = fChain->GetBranch("jet.secvLoosePzVtx");
   b_jet_secvLooseEVtx = fChain->GetBranch("jet.secvLooseEVtx");
   b_jet_secvLooseX = fChain->GetBranch("jet.secvLooseX");
   b_jet_secvLooseY = fChain->GetBranch("jet.secvLooseY");
   b_jet_secvLooseZ = fChain->GetBranch("jet.secvLooseZ");
   b_jet_secvLooseXXerr = fChain->GetBranch("jet.secvLooseXXerr");
   b_jet_secvLooseYYerr = fChain->GetBranch("jet.secvLooseYYerr");
   b_jet_secvLooseZZerr = fChain->GetBranch("jet.secvLooseZZerr");
   b_jet_secvLooseXYerr = fChain->GetBranch("jet.secvLooseXYerr");
   b_jet_secvLooseXZerr = fChain->GetBranch("jet.secvLooseXZerr");
   b_jet_secvLooseYZerr = fChain->GetBranch("jet.secvLooseYZerr");
   b_jet_secvLooseNPass1Tracks = fChain->GetBranch("jet.secvLooseNPass1Tracks");
   b_jet_secvLooseNPass2Tracks = fChain->GetBranch("jet.secvLooseNPass2Tracks");
   b_jet_secvLooseDz = fChain->GetBranch("jet.secvLooseDz");
   b_jet_secvLoosePtSec = fChain->GetBranch("jet.secvLoosePtSec");
   b_jet_secvLoosePtRelSec = fChain->GetBranch("jet.secvLoosePtRelSec");
   b_jet_secvLoosePlRelSec = fChain->GetBranch("jet.secvLoosePlRelSec");
   b_jet_secvLooseESeed = fChain->GetBranch("jet.secvLooseESeed");
   b_jet_secvLoosePxSeed = fChain->GetBranch("jet.secvLoosePxSeed");
   b_jet_secvLoosePySeed = fChain->GetBranch("jet.secvLoosePySeed");
   b_jet_secvLoosePzSeed = fChain->GetBranch("jet.secvLoosePzSeed");
   b_jet_secvLooseNLeptons = fChain->GetBranch("jet.secvLooseNLeptons");
   b_jet_drB = fChain->GetBranch("jet.drB");
   b_jet_drC = fChain->GetBranch("jet.drC");
   b_jet_NTracks = fChain->GetBranch("jet.NTracks");
   b_jet_jpbNTracks = fChain->GetBranch("jet.jpbNTracks");
   b_jet_jpbJetEt = fChain->GetBranch("jet.jpbJetEt");
   b_jet_jpbJetEta = fChain->GetBranch("jet.jpbJetEta");
   b_jet_jpbJetPhi = fChain->GetBranch("jet.jpbJetPhi");
   b_jet_jpbHFTag = fChain->GetBranch("jet.jpbHFTag");
   b_jet_jpbHFHepId = fChain->GetBranch("jet.jpbHFHepId");
   b_jet_jpbHFdR = fChain->GetBranch("jet.jpbHFdR");
   b_jet_jpbRPunsign = fChain->GetBranch("jet.jpbRPunsign");
   b_jet_jpbRPpos = fChain->GetBranch("jet.jpbRPpos");
   b_jet_jpbRPneg = fChain->GetBranch("jet.jpbRPneg");
   b_jet_jpbNGdRPTrk = fChain->GetBranch("jet.jpbNGdRPTrk");
   b_jet_jpbNGdRPTrkPos = fChain->GetBranch("jet.jpbNGdRPTrkPos");
   b_jet_jpbNGdRPTrkNeg = fChain->GetBranch("jet.jpbNGdRPTrkNeg");
   b_jet_jpbRZunsign = fChain->GetBranch("jet.jpbRZunsign");
   b_jet_jpbRZpos = fChain->GetBranch("jet.jpbRZpos");
   b_jet_jpbRZneg = fChain->GetBranch("jet.jpbRZneg");
   b_jet_jpbNGdRZTrk = fChain->GetBranch("jet.jpbNGdRZTrk");
   b_jet_jpbNGdRZTrkPos = fChain->GetBranch("jet.jpbNGdRZTrkPos");
   b_jet_jpbNGdRZTrkNeg = fChain->GetBranch("jet.jpbNGdRZTrkNeg");
   b_jet_jpb3Dunsign = fChain->GetBranch("jet.jpb3Dunsign");
   b_jet_jpb3Dpos = fChain->GetBranch("jet.jpb3Dpos");
   b_jet_jpb3Dneg = fChain->GetBranch("jet.jpb3Dneg");
   b_jet_jpbNGd3DTrk = fChain->GetBranch("jet.jpbNGd3DTrk");
   b_jet_jpbNGd3DTrkPos = fChain->GetBranch("jet.jpbNGd3DTrkPos");
   b_jet_jpbNGd3DTrkNeg = fChain->GetBranch("jet.jpbNGd3DTrkNeg");
   b_numJetObjs = fChain->GetBranch("numJetObjs");
   b_jetCollDescr1 = fChain->GetBranch("jetCollDescr1");
   b_jetCollDescr2 = fChain->GetBranch("jetCollDescr2");
   b_jetCollDescr3 = fChain->GetBranch("jetCollDescr3");
   b_jetCollDescr4 = fChain->GetBranch("jetCollDescr4");
   b_jetCollDescr5 = fChain->GetBranch("jetCollDescr5");
   b_jetCollDescr6 = fChain->GetBranch("jetCollDescr6");
   b_jetCollDescr7 = fChain->GetBranch("jetCollDescr7");
   b_jetCollDescr8 = fChain->GetBranch("jetCollDescr8");
   b_jetCollDescr9 = fChain->GetBranch("jetCollDescr9");
   b_jetCollDescr10 = fChain->GetBranch("jetCollDescr10");
   b_jetCollDescr11 = fChain->GetBranch("jetCollDescr11");
   b_jetCollDescr12 = fChain->GetBranch("jetCollDescr12");
   b_jetCollDescr13 = fChain->GetBranch("jetCollDescr13");
   b_jetCollDescr14 = fChain->GetBranch("jetCollDescr14");
   b_jetCollDescr15 = fChain->GetBranch("jetCollDescr15");
   b_met_ = fChain->GetBranch("met");
   b_met_fUniqueID = fChain->GetBranch("met.fUniqueID");
   b_met_fBits = fChain->GetBranch("met.fBits");
   b_met_Met = fChain->GetBranch("met.Met");
   b_met_Phi = fChain->GetBranch("met.Phi");
   b_met_ZV = fChain->GetBranch("met.ZV");
   b_met_ESum = fChain->GetBranch("met.ESum");
   b_met_EtSum = fChain->GetBranch("met.EtSum");
   b_met_ExSum = fChain->GetBranch("met.ExSum");
   b_met_EySum = fChain->GetBranch("met.EySum");
   b_numMetObjs = fChain->GetBranch("numMetObjs");
   b_topfit_ = fChain->GetBranch("topfit");
   b_topfit_fUniqueID = fChain->GetBranch("topfit.fUniqueID");
   b_topfit_fBits = fChain->GetBranch("topfit.fBits");
   b_topfit_Chi2 = fChain->GetBranch("topfit.Chi2");
   b_topfit_Mass = fChain->GetBranch("topfit.Mass");
   b_topfit_ErrPa = fChain->GetBranch("topfit.ErrPa");
   b_topfit_ErrPl = fChain->GetBranch("topfit.ErrPl");
   b_topfit_ErrMi = fChain->GetBranch("topfit.ErrMi");
   b_topfit_config = fChain->GetBranch("topfit.config");
   b_topfit_tagok = fChain->GetBranch("topfit.tagok");
   b_topfit_jettheta = fChain->GetBranch("topfit.jettheta[4]");
   b_topfit_jetphi = fChain->GetBranch("topfit.jetphi[4]");
   b_topfit_jetptraw = fChain->GetBranch("topfit.jetptraw[4]");
   b_topfit_jetptgeneric = fChain->GetBranch("topfit.jetptgeneric[4]");
   b_topfit_jetptgg = fChain->GetBranch("topfit.jetptgg[4]");
   b_topfit_jetsigma = fChain->GetBranch("topfit.jetsigma[4]");
   b_topfit_jetptout = fChain->GetBranch("topfit.jetptout[4]");
   b_numTopFitObjs = fChain->GetBranch("numTopFitObjs");
   b_numTopFitBtags = fChain->GetBranch("numTopFitBtags");
   b_trigInfo_ = fChain->GetBranch("trigInfo");
   b_trigInfo_fUniqueID = fChain->GetBranch("trigInfo.fUniqueID");
   b_trigInfo_fBits = fChain->GetBranch("trigInfo.fBits");
   b_trigInfo_L1W1NoPS = fChain->GetBranch("trigInfo.L1W1NoPS");
   b_trigInfo_L1W2NoPS = fChain->GetBranch("trigInfo.L1W2NoPS");
   b_trigInfo_L1W1PS = fChain->GetBranch("trigInfo.L1W1PS");
   b_trigInfo_L1W2PS = fChain->GetBranch("trigInfo.L1W2PS");
   b_trigInfo_L2W1 = fChain->GetBranch("trigInfo.L2W1");
   b_trigInfo_L2W2 = fChain->GetBranch("trigInfo.L2W2");
   b_trigInfo_L2W3 = fChain->GetBranch("trigInfo.L2W3");
   b_trigInfo_L2W4 = fChain->GetBranch("trigInfo.L2W4");
   b_trigInfo_L2W1NoPS = fChain->GetBranch("trigInfo.L2W1NoPS");
   b_trigInfo_L2W2NoPS = fChain->GetBranch("trigInfo.L2W2NoPS");
   b_trigInfo_L2W3NoPS = fChain->GetBranch("trigInfo.L2W3NoPS");
   b_trigInfo_L2W4NoPS = fChain->GetBranch("trigInfo.L2W4NoPS");
   b_trigInfo_L3W1 = fChain->GetBranch("trigInfo.L3W1");
   b_trigInfo_L3W2 = fChain->GetBranch("trigInfo.L3W2");
   b_trigInfo_L3W3 = fChain->GetBranch("trigInfo.L3W3");
   b_trigInfo_L3W4 = fChain->GetBranch("trigInfo.L3W4");
   b_trigInfo_L3W5 = fChain->GetBranch("trigInfo.L3W5");
   b_trigInfo_L3W6 = fChain->GetBranch("trigInfo.L3W6");
   b_trigInfo_L1sumetW1 = fChain->GetBranch("trigInfo.L1sumetW1");
   b_trigInfo_L1sumetW2 = fChain->GetBranch("trigInfo.L1sumetW2");
   b_trigInfo_L1cal = fChain->GetBranch("trigInfo.L1cal");
   b_trigInfo_L1mu = fChain->GetBranch("trigInfo.L1mu");
   b_trigInfo_L1trk = fChain->GetBranch("trigInfo.L1trk");
   b_trigInfo_L1bsc = fChain->GetBranch("trigInfo.L1bsc");
   b_trigInfo_summaryWord = fChain->GetBranch("trigInfo.summaryWord");
   b_trigBitDescr0 = fChain->GetBranch("trigBitDescr0");
   b_trigBitDescr1 = fChain->GetBranch("trigBitDescr1");
   b_trigBitDescr2 = fChain->GetBranch("trigBitDescr2");
   b_trigBitDescr3 = fChain->GetBranch("trigBitDescr3");
   b_trigBitDescr4 = fChain->GetBranch("trigBitDescr4");
   b_trigBitDescr5 = fChain->GetBranch("trigBitDescr5");
   b_trigBitDescr6 = fChain->GetBranch("trigBitDescr6");
   b_trigBitDescr7 = fChain->GetBranch("trigBitDescr7");
   b_trigBitDescr8 = fChain->GetBranch("trigBitDescr8");
   b_trigBitDescr9 = fChain->GetBranch("trigBitDescr9");
   b_trigName_ = fChain->GetBranch("trigName");
   b_trigName_fUniqueID = fChain->GetBranch("trigName.fUniqueID");
   b_trigName_fBits = fChain->GetBranch("trigName.fBits");
   b_trigName_RunNumber = fChain->GetBranch("trigName.RunNumber");
   b_trigName_NumL1Triggers = fChain->GetBranch("trigName.NumL1Triggers");
   b_trigName_NumL2Triggers = fChain->GetBranch("trigName.NumL2Triggers");
   b_trigName_NumL3Triggers = fChain->GetBranch("trigName.NumL3Triggers");
   b_trigName_L1Bit = fChain->GetBranch("trigName.L1Bit[64]");
   b_trigName_L2Bit = fChain->GetBranch("trigName.L2Bit[128]");
   b_trigName_L3Bit = fChain->GetBranch("trigName.L3Bit[256]");
   b_trigName_L1Name = fChain->GetBranch("trigName.L1Name[64][64]");
   b_trigName_L2Name = fChain->GetBranch("trigName.L2Name[128][64]");
   b_trigName_L3Name = fChain->GetBranch("trigName.L3Name[256][64]");
   b_l2Cal_ = fChain->GetBranch("l2Cal");
   b_l2Cal_fUniqueID = fChain->GetBranch("l2Cal.fUniqueID");
   b_l2Cal_fBits = fChain->GetBranch("l2Cal.fBits");
   b_l2Cal_etEmL2 = fChain->GetBranch("l2Cal.etEmL2");
   b_l2Cal_etHadL2 = fChain->GetBranch("l2Cal.etHadL2");
   b_l2Cal_ntowL2 = fChain->GetBranch("l2Cal.ntowL2");
   b_l2Cal_phiL2 = fChain->GetBranch("l2Cal.phiL2");
   b_l2Cal_etaL2 = fChain->GetBranch("l2Cal.etaL2");
   b_l2Cal_typL2 = fChain->GetBranch("l2Cal.typL2");
   b_numL2CalObjs = fChain->GetBranch("numL2CalObjs");
   b_offltrack_ = fChain->GetBranch("offltrack");
   b_offltrack_fUniqueID = fChain->GetBranch("offltrack.fUniqueID");
   b_offltrack_fBits = fChain->GetBranch("offltrack.fBits");
   b_offltrack_Alg = fChain->GetBranch("offltrack.Alg");
   b_offltrack_CollType = fChain->GetBranch("offltrack.CollType");
   b_offltrack_PassesDefTrkCut = fChain->GetBranch("offltrack.PassesDefTrkCut");
   b_offltrack_ObspId = fChain->GetBranch("offltrack.ObspId");
   b_offltrack_Pt = fChain->GetBranch("offltrack.Pt");
   b_offltrack_Id = fChain->GetBranch("offltrack.Id");
   b_offltrack_IdOrig = fChain->GetBranch("offltrack.IdOrig");
   b_offltrack_NumCTHitsAx = fChain->GetBranch("offltrack.NumCTHitsAx");
   b_offltrack_NumCTHitsSt = fChain->GetBranch("offltrack.NumCTHitsSt");
   b_offltrack_NumAxSeg = fChain->GetBranch("offltrack.NumAxSeg");
   b_offltrack_NumStSeg = fChain->GetBranch("offltrack.NumStSeg");
   b_offltrack_NumCTHits = fChain->GetBranch("offltrack.NumCTHits[8]");
   b_offltrack_Lambda = fChain->GetBranch("offltrack.Lambda");
   b_offltrack_Curv = fChain->GetBranch("offltrack.Curv");
   b_offltrack_Z0 = fChain->GetBranch("offltrack.Z0");
   b_offltrack_D0 = fChain->GetBranch("offltrack.D0");
   b_offltrack_D0C = fChain->GetBranch("offltrack.D0C");
   b_offltrack_Phi0 = fChain->GetBranch("offltrack.Phi0");
   b_offltrack_Eta = fChain->GetBranch("offltrack.Eta");
   b_offltrack_Charge = fChain->GetBranch("offltrack.Charge");
   b_offltrack_chi2 = fChain->GetBranch("offltrack.chi2");
   b_offltrack_chi2CT = fChain->GetBranch("offltrack.chi2CT");
   b_offltrack_BCP = fChain->GetBranch("offltrack.BCP");
   b_offltrack_BCPx = fChain->GetBranch("offltrack.BCPx");
   b_offltrack_BCPy = fChain->GetBranch("offltrack.BCPy");
   b_offltrack_BCPz = fChain->GetBranch("offltrack.BCPz");
   b_offltrack_BCPt = fChain->GetBranch("offltrack.BCPt");
   b_offltrack_BCEta = fChain->GetBranch("offltrack.BCEta");
   b_offltrack_BCPhi0 = fChain->GetBranch("offltrack.BCPhi0");
   b_offltrack_BCZ0 = fChain->GetBranch("offltrack.BCZ0");
   b_offltrack_BCCharge = fChain->GetBranch("offltrack.BCCharge");
   b_offltrack_SIHits = fChain->GetBranch("offltrack.SIHits");
   b_offltrack_SIFit = fChain->GetBranch("offltrack.SIFit");
   b_offltrack_SIGoodHitX = fChain->GetBranch("offltrack.SIGoodHitX");
   b_offltrack_SIGoodHitZ = fChain->GetBranch("offltrack.SIGoodHitZ");
   b_offltrack_SIGoodHitZ90 = fChain->GetBranch("offltrack.SIGoodHitZ90");
   b_offltrack_SIID = fChain->GetBranch("offltrack.SIID");
   b_offltrack_SIPhi = fChain->GetBranch("offltrack.SIPhi");
   b_offltrack_SID0 = fChain->GetBranch("offltrack.SID0");
   b_offltrack_SIPt = fChain->GetBranch("offltrack.SIPt");
   b_offltrack_SIZ = fChain->GetBranch("offltrack.SIZ");
   b_offltrack_SIZsig = fChain->GetBranch("offltrack.SIZsig");
   b_offltrack_SID0sig = fChain->GetBranch("offltrack.SID0sig");
   b_offltrack_SICot = fChain->GetBranch("offltrack.SICot");
   b_offltrack_SIChis = fChain->GetBranch("offltrack.SIChis");
   b_offltrack_SITotHitX = fChain->GetBranch("offltrack.SITotHitX");
   b_offltrack_SITotHitZ = fChain->GetBranch("offltrack.SITotHitZ");
   b_offltrack_SITotHitZ90 = fChain->GetBranch("offltrack.SITotHitZ90");
   b_offltrack_ErrCotCot = fChain->GetBranch("offltrack.ErrCotCot");
   b_offltrack_ErrCotCrv = fChain->GetBranch("offltrack.ErrCotCrv");
   b_offltrack_ErrCotZ0 = fChain->GetBranch("offltrack.ErrCotZ0");
   b_offltrack_ErrCotD0 = fChain->GetBranch("offltrack.ErrCotD0");
   b_offltrack_ErrCotPhi0 = fChain->GetBranch("offltrack.ErrCotPhi0");
   b_offltrack_ErrCrvCrv = fChain->GetBranch("offltrack.ErrCrvCrv");
   b_offltrack_ErrCrvZ0 = fChain->GetBranch("offltrack.ErrCrvZ0");
   b_offltrack_ErrCrvD0 = fChain->GetBranch("offltrack.ErrCrvD0");
   b_offltrack_ErrCrvPhi0 = fChain->GetBranch("offltrack.ErrCrvPhi0");
   b_offltrack_ErrZ0Z0 = fChain->GetBranch("offltrack.ErrZ0Z0");
   b_offltrack_ErrZ0D0 = fChain->GetBranch("offltrack.ErrZ0D0");
   b_offltrack_ErrZ0Phi0 = fChain->GetBranch("offltrack.ErrZ0Phi0");
   b_offltrack_ErrD0D0 = fChain->GetBranch("offltrack.ErrD0D0");
   b_offltrack_ErrD0Phi0 = fChain->GetBranch("offltrack.ErrD0Phi0");
   b_offltrack_ErrPhi0Phi0 = fChain->GetBranch("offltrack.ErrPhi0Phi0");
   b_offltrack_CalEmTower = fChain->GetBranch("offltrack.CalEmTower");
   b_offltrack_CalHadTower = fChain->GetBranch("offltrack.CalHadTower");
   b_offltrack_CalEm3x3Iso = fChain->GetBranch("offltrack.CalEm3x3Iso");
   b_offltrack_CalHad3x3Iso = fChain->GetBranch("offltrack.CalHad3x3Iso");
   b_offltrack_SiExpectedLayers = fChain->GetBranch("offltrack.SiExpectedLayers");
   b_offltrack_trkIsol = fChain->GetBranch("offltrack.trkIsol");
   b_offltrack_corrD0 = fChain->GetBranch("offltrack.corrD0");
   b_numOfflTracks = fChain->GetBranch("numOfflTracks");
   b_otrkCollDescr1 = fChain->GetBranch("otrkCollDescr1");
   b_otrkCollDescr2 = fChain->GetBranch("otrkCollDescr2");
   b_otrkCollDescr3 = fChain->GetBranch("otrkCollDescr3");
   b_otrkCollDescr4 = fChain->GetBranch("otrkCollDescr4");
   b_otrkCollDescr5 = fChain->GetBranch("otrkCollDescr5");
   b_secvtxtrack_ = fChain->GetBranch("secvtxtrack");
   b_secvtxtrack_fUniqueID = fChain->GetBranch("secvtxtrack.fUniqueID");
   b_secvtxtrack_fBits = fChain->GetBranch("secvtxtrack.fBits");
   b_secvtxtrack_alg = fChain->GetBranch("secvtxtrack.alg");
   b_secvtxtrack_tNGoodHitx = fChain->GetBranch("secvtxtrack.tNGoodHitx");
   b_secvtxtrack_tNGoodHitz = fChain->GetBranch("secvtxtrack.tNGoodHitz");
   b_secvtxtrack_tNGoodHitSz = fChain->GetBranch("secvtxtrack.tNGoodHitSz");
   b_secvtxtrack_tNGoodHitx_ISL = fChain->GetBranch("secvtxtrack.tNGoodHitx_ISL");
   b_secvtxtrack_tNGoodHitSz_ISL = fChain->GetBranch("secvtxtrack.tNGoodHitSz_ISL");
   b_secvtxtrack_tNGoodHitx_L00 = fChain->GetBranch("secvtxtrack.tNGoodHitx_L00");
   b_secvtxtrack_tId = fChain->GetBranch("secvtxtrack.tId");
   b_secvtxtrack_tIdDerived = fChain->GetBranch("secvtxtrack.tIdDerived");
   b_secvtxtrack_tIdOrig = fChain->GetBranch("secvtxtrack.tIdOrig");
   b_secvtxtrack_trkLink = fChain->GetBranch("secvtxtrack.trkLink");
   b_secvtxtrack_sPt = fChain->GetBranch("secvtxtrack.sPt");
   b_secvtxtrack_tEta = fChain->GetBranch("secvtxtrack.tEta");
   b_secvtxtrack_tCur = fChain->GetBranch("secvtxtrack.tCur");
   b_secvtxtrack_tPhi0 = fChain->GetBranch("secvtxtrack.tPhi0");
   b_secvtxtrack_tD0 = fChain->GetBranch("secvtxtrack.tD0");
   b_secvtxtrack_tZ0 = fChain->GetBranch("secvtxtrack.tZ0");
   b_secvtxtrack_tCot = fChain->GetBranch("secvtxtrack.tCot");
   b_secvtxtrack_tCurE = fChain->GetBranch("secvtxtrack.tCurE");
   b_secvtxtrack_tPhi0E = fChain->GetBranch("secvtxtrack.tPhi0E");
   b_secvtxtrack_tD0E = fChain->GetBranch("secvtxtrack.tD0E");
   b_secvtxtrack_tZ0E = fChain->GetBranch("secvtxtrack.tZ0E");
   b_secvtxtrack_tCotE = fChain->GetBranch("secvtxtrack.tCotE");
   b_secvtxtrack_sD0 = fChain->GetBranch("secvtxtrack.sD0");
   b_secvtxtrack_sD0s = fChain->GetBranch("secvtxtrack.sD0s");
   b_secvtxtrack_sZ0 = fChain->GetBranch("secvtxtrack.sZ0");
   b_secvtxtrack_sZ0s = fChain->GetBranch("secvtxtrack.sZ0s");
   b_secvtxtrack_sB0 = fChain->GetBranch("secvtxtrack.sB0");
   b_secvtxtrack_sB0s = fChain->GetBranch("secvtxtrack.sB0s");
   b_secvtxtrack_sChi2X = fChain->GetBranch("secvtxtrack.sChi2X");
   b_secvtxtrack_sNHitX = fChain->GetBranch("secvtxtrack.sNHitX");
   b_secvtxtrack_sNHitSZ = fChain->GetBranch("secvtxtrack.sNHitSZ");
   b_secvtxtrack_sNHitZ = fChain->GetBranch("secvtxtrack.sNHitZ");
   b_secvtxtrack_sNHitX_ISL = fChain->GetBranch("secvtxtrack.sNHitX_ISL");
   b_secvtxtrack_sNHitSZ_ISL = fChain->GetBranch("secvtxtrack.sNHitSZ_ISL");
   b_secvtxtrack_sNHitX_L00 = fChain->GetBranch("secvtxtrack.sNHitX_L00");
   b_secvtxtrack_sType = fChain->GetBranch("secvtxtrack.sType");
   b_secvtxtrack_nCotHits = fChain->GetBranch("secvtxtrack.nCotHits");
   b_secvtxtrack_tKey1 = fChain->GetBranch("secvtxtrack.tKey1");
   b_secvtxtrack_tKey2 = fChain->GetBranch("secvtxtrack.tKey2");
   b_secvtxtrack_tIsGoodTrack = fChain->GetBranch("secvtxtrack.tIsGoodTrack");
   b_secvtxtrack_tIsPass1Track = fChain->GetBranch("secvtxtrack.tIsPass1Track");
   b_secvtxtrack_tIsPass2Track = fChain->GetBranch("secvtxtrack.tIsPass2Track");
   b_secvtxtrack_tIsGoodZTrack = fChain->GetBranch("secvtxtrack.tIsGoodZTrack");
   b_secvtxtrack_tIsUsedTrack = fChain->GetBranch("secvtxtrack.tIsUsedTrack");
   b_secvtxtrack_tIsVeeTrack = fChain->GetBranch("secvtxtrack.tIsVeeTrack");
   b_secvtxtrack_tIsGoodLooseTrack = fChain->GetBranch("secvtxtrack.tIsGoodLooseTrack");
   b_secvtxtrack_tIsPass1LooseTrack = fChain->GetBranch("secvtxtrack.tIsPass1LooseTrack");
   b_secvtxtrack_tIsPass2LooseTrack = fChain->GetBranch("secvtxtrack.tIsPass2LooseTrack");
   b_secvtxtrack_tIsGoodZLooseTrack = fChain->GetBranch("secvtxtrack.tIsGoodZLooseTrack");
   b_secvtxtrack_tIsUsedLooseTrack = fChain->GetBranch("secvtxtrack.tIsUsedLooseTrack");
   b_secvtxtrack_tIsVeeLooseTrack = fChain->GetBranch("secvtxtrack.tIsVeeLooseTrack");
   b_secvtxtrack_tNSharedHits = fChain->GetBranch("secvtxtrack.tNSharedHits");
   b_secvtxtrack_tMaxClusterLength = fChain->GetBranch("secvtxtrack.tMaxClusterLength");
   b_secvtxtrack_tMaxClusterLengthPhi = fChain->GetBranch("secvtxtrack.tMaxClusterLengthPhi");
   b_secvtxtrack_tMaxClusterLengthZ = fChain->GetBranch("secvtxtrack.tMaxClusterLengthZ");
   b_secvtxtrack_tMaxClusterLengthSas = fChain->GetBranch("secvtxtrack.tMaxClusterLengthSas");
   b_secvtxtrack_tNIntersections = fChain->GetBranch("secvtxtrack.tNIntersections");
   b_secvtxtrack_tNGoodLayers = fChain->GetBranch("secvtxtrack.tNGoodLayers");
   b_secvtxtrack_tNErrors = fChain->GetBranch("secvtxtrack.tNErrors");
   b_secvtxtrack_tHitPattern = fChain->GetBranch("secvtxtrack.tHitPattern");
   b_secvtxtrack_tErrorLayers = fChain->GetBranch("secvtxtrack.tErrorLayers");
   b_secvtxtrack_tIntegratedLayers = fChain->GetBranch("secvtxtrack.tIntegratedLayers");
   b_secvtxtrack_tHitPatternZ = fChain->GetBranch("secvtxtrack.tHitPatternZ");
   b_secvtxtrack_tErrorLayersZ = fChain->GetBranch("secvtxtrack.tErrorLayersZ");
   b_secvtxtrack_tIntegratedLayersZ = fChain->GetBranch("secvtxtrack.tIntegratedLayersZ");
   b_secvtxtrack_tNMissedLayers = fChain->GetBranch("secvtxtrack.tNMissedLayers");
   b_secvtxtrack_tNMissedLayersZ = fChain->GetBranch("secvtxtrack.tNMissedLayersZ");
   b_secvtxtrack_tNMissedLayersISL = fChain->GetBranch("secvtxtrack.tNMissedLayersISL");
   b_secvtxtrack_tNMissedLayersZISL = fChain->GetBranch("secvtxtrack.tNMissedLayersZISL");
   b_secvtxtrack_tNMissedLayersL00 = fChain->GetBranch("secvtxtrack.tNMissedLayersL00");
   b_secvtxtrack_tWhyBadISLsz = fChain->GetBranch("secvtxtrack.tWhyBadISLsz");
   b_secvtxtrack_tWhyBadISLx = fChain->GetBranch("secvtxtrack.tWhyBadISLx");
   b_secvtxtrack_tWhyBadsz = fChain->GetBranch("secvtxtrack.tWhyBadsz");
   b_secvtxtrack_tWhyBadz = fChain->GetBranch("secvtxtrack.tWhyBadz");
   b_secvtxtrack_tWhyBadx = fChain->GetBranch("secvtxtrack.tWhyBadx");
   b_secvtxtrack_tIsLepton = fChain->GetBranch("secvtxtrack.tIsLepton");
   b_secvtxtrack_tIsSvt = fChain->GetBranch("secvtxtrack.tIsSvt");
   b_secvtxtrack_tIsSuggested = fChain->GetBranch("secvtxtrack.tIsSuggested");
   b_secvtxtrack_tIsRequired = fChain->GetBranch("secvtxtrack.tIsRequired");
   b_secvtxtrack_tIsSkipped = fChain->GetBranch("secvtxtrack.tIsSkipped");
   b_secvtxtrack_tGenpIndex = fChain->GetBranch("secvtxtrack.tGenpIndex");
   b_secvtxtrack_tCov = fChain->GetBranch("secvtxtrack.tCov[15]");
   b_secvtxtrack_is_HFdaughter = fChain->GetBranch("secvtxtrack.is_HFdaughter");
   b_numSecvtxTracks = fChain->GetBranch("numSecvtxTracks");
   b_tjassoc_ = fChain->GetBranch("tjassoc");
   b_tjassoc_fUniqueID = fChain->GetBranch("tjassoc.fUniqueID");
   b_tjassoc_fBits = fChain->GetBranch("tjassoc.fBits");
   b_tjassoc_jetid = fChain->GetBranch("tjassoc.jetid");
   b_tjassoc_trkid = fChain->GetBranch("tjassoc.trkid");
   b_tjassoc_acode = fChain->GetBranch("tjassoc.acode");
   b_numTJAssoc = fChain->GetBranch("numTJAssoc");
   b_tvassoc_ = fChain->GetBranch("tvassoc");
   b_tvassoc_fUniqueID = fChain->GetBranch("tvassoc.fUniqueID");
   b_tvassoc_fBits = fChain->GetBranch("tvassoc.fBits");
   b_tvassoc_vtxid = fChain->GetBranch("tvassoc.vtxid");
   b_tvassoc_trkid = fChain->GetBranch("tvassoc.trkid");
   b_numTVAssoc = fChain->GetBranch("numTVAssoc");
   b_jetprobtrack_ = fChain->GetBranch("jetprobtrack");
   b_jetprobtrack_fUniqueID = fChain->GetBranch("jetprobtrack.fUniqueID");
   b_jetprobtrack_fBits = fChain->GetBranch("jetprobtrack.fBits");
   b_jetprobtrack_tId = fChain->GetBranch("jetprobtrack.tId");
   b_jetprobtrack_tPt = fChain->GetBranch("jetprobtrack.tPt");
   b_jetprobtrack_tTheta = fChain->GetBranch("jetprobtrack.tTheta");
   b_jetprobtrack_tEta = fChain->GetBranch("jetprobtrack.tEta");
   b_jetprobtrack_tPhi0 = fChain->GetBranch("jetprobtrack.tPhi0");
   b_jetprobtrack_tNSvxLayer = fChain->GetBranch("jetprobtrack.tNSvxLayer");
   b_jetprobtrack_tNSvxHit = fChain->GetBranch("jetprobtrack.tNSvxHit");
   b_jetprobtrack_tNSvxPhiHit = fChain->GetBranch("jetprobtrack.tNSvxPhiHit");
   b_jetprobtrack_tNSvxStHit = fChain->GetBranch("jetprobtrack.tNSvxStHit");
   b_jetprobtrack_tNSvxZHit = fChain->GetBranch("jetprobtrack.tNSvxZHit");
   b_jetprobtrack_tTotNSvxRPhiHit = fChain->GetBranch("jetprobtrack.tTotNSvxRPhiHit");
   b_jetprobtrack_tTotNGdSvxRPhiHit = fChain->GetBranch("jetprobtrack.tTotNGdSvxRPhiHit");
   b_jetprobtrack_tD0 = fChain->GetBranch("jetprobtrack.tD0");
   b_jetprobtrack_tD0Err = fChain->GetBranch("jetprobtrack.tD0Err");
   b_jetprobtrack_tSignD0 = fChain->GetBranch("jetprobtrack.tSignD0");
   b_jetprobtrack_tZ0 = fChain->GetBranch("jetprobtrack.tZ0");
   b_jetprobtrack_tZ0Err = fChain->GetBranch("jetprobtrack.tZ0Err");
   b_jetprobtrack_tSignZ0 = fChain->GetBranch("jetprobtrack.tSignZ0");
   b_jetprobtrack_tProbRPhi = fChain->GetBranch("jetprobtrack.tProbRPhi");
   b_jetprobtrack_tProbRZ = fChain->GetBranch("jetprobtrack.tProbRZ");
   b_jetprobtrack_tProb3D = fChain->GetBranch("jetprobtrack.tProb3D");
   b_jetprobtrack_tNCotHit = fChain->GetBranch("jetprobtrack.tNCotHit");
   b_jetprobtrack_tNCotAxHit = fChain->GetBranch("jetprobtrack.tNCotAxHit");
   b_jetprobtrack_tNCotStHit = fChain->GetBranch("jetprobtrack.tNCotStHit");
   b_jetprobtrack_tTrackQtyRPhi = fChain->GetBranch("jetprobtrack.tTrackQtyRPhi");
   b_jetprobtrack_tTrackQtyRZ = fChain->GetBranch("jetprobtrack.tTrackQtyRZ");
   b_jetprobtrack_tLongLiveFlagRPhi = fChain->GetBranch("jetprobtrack.tLongLiveFlagRPhi");
   b_jetprobtrack_tLongLiveFlagRZ = fChain->GetBranch("jetprobtrack.tLongLiveFlagRZ");
   b_jetprobtrack_tIsGoodRPhi = fChain->GetBranch("jetprobtrack.tIsGoodRPhi");
   b_jetprobtrack_tIsGoodRZ = fChain->GetBranch("jetprobtrack.tIsGoodRZ");
   b_numJetProbTracks = fChain->GetBranch("numJetProbTracks");
   b_jetprobTJassoc_ = fChain->GetBranch("jetprobTJassoc");
   b_jetprobTJassoc_fUniqueID = fChain->GetBranch("jetprobTJassoc.fUniqueID");
   b_jetprobTJassoc_fBits = fChain->GetBranch("jetprobTJassoc.fBits");
   b_jetprobTJassoc_jetid = fChain->GetBranch("jetprobTJassoc.jetid");
   b_jetprobTJassoc_trkid = fChain->GetBranch("jetprobTJassoc.trkid");
   b_numJetProbTJAssoc = fChain->GetBranch("numJetProbTJAssoc");
   b_obsp_ = fChain->GetBranch("obsp");
   b_obsp_fUniqueID = fChain->GetBranch("obsp.fUniqueID");
   b_obsp_fBits = fChain->GetBranch("obsp.fBits");
   b_obsp_index = fChain->GetBranch("obsp.index");
   b_obsp_cdfid = fChain->GetBranch("obsp.cdfid");
   b_obsp_hepglink = fChain->GetBranch("obsp.hepglink");
   b_obsp_dkmode = fChain->GetBranch("obsp.dkmode");
   b_obsp_px = fChain->GetBranch("obsp.px");
   b_obsp_py = fChain->GetBranch("obsp.py");
   b_obsp_pz = fChain->GetBranch("obsp.pz");
   b_obsp_m = fChain->GetBranch("obsp.m");
   b_obsp_qcharge = fChain->GetBranch("obsp.qcharge");
   b_obsp_path = fChain->GetBranch("obsp.path");
   b_obsp_motherid = fChain->GetBranch("obsp.motherid");
   b_obsp_nda = fChain->GetBranch("obsp.nda");
   b_obsp_Vx = fChain->GetBranch("obsp.Vx");
   b_obsp_Vy = fChain->GetBranch("obsp.Vy");
   b_obsp_Vz = fChain->GetBranch("obsp.Vz");
   b_obsp_Vt = fChain->GetBranch("obsp.Vt");
   b_obsp_is_bdaughter = fChain->GetBranch("obsp.is_bdaughter");
   b_obsp_is_cdaughter = fChain->GetBranch("obsp.is_cdaughter");
   b_obsp_beauty = fChain->GetBranch("obsp.beauty");
   b_obsp_charm = fChain->GetBranch("obsp.charm");
   b_obsp_jmo1 = fChain->GetBranch("obsp.jmo1");
   b_obsp_jda1 = fChain->GetBranch("obsp.jda1");
   b_obsp_jda2 = fChain->GetBranch("obsp.jda2");
   b_numObspObjs = fChain->GetBranch("numObspObjs");
   return kTRUE;
}

void SkimFCNC::Show(Int_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SkimFCNC::Cut(Int_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SkimFCNC_cxx

