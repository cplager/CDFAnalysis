/*
 *  makeNtuplePlots.C
 *  
 *
 *  Created by Mark Mathis on 5/31/2009
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <iostream>
#include <TH1F.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TF1.h>
#include <TLine.h>


void makeNtuplePlots(char* filename = "top_new.root") {
	TFile file(filename);
	TTree* tree;

	file.GetObject("nt", tree); 
	if (tree == NULL) {

		file.GetObject("nt", tree); 
		if (tree == NULL){
			std::cout << "ERROR! Cannot find the nt tree." << std::endl;
			file.Close();
			exit;
		}
	}
	
	Double_t leppt, lepeta, ht, b1pt, b2pt, b1eta, b2eta, neutpt;
	Double_t b1hadpt, b2hadpt, b1hadeta, b2hadeta;
	Double_t recoleppt, recolepeta, met, mTW;
	Double_t jet1et, jet2et, jet3et, jet4et, jet1eta, jet2eta, jet3eta, jet4eta;
	Double_t jet5et, jet6et, jet7et, jet8et, jet5eta, jet6eta, jet7eta, jet8eta;
	Int_t njets;


	TH1F* leptonPt = new TH1F("leptonPt", "Lepton pT", 350, 10, 360);
	leptonPt->SetTitle("Lepton pT");
	leptonPt->GetXaxis()->SetTitle("pT [GeV]");
	leptonPt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* leptonEta = new TH1F("leptonEta", "Lepton Eta", 60, -3, 3);
	leptonEta->SetTitle("Lepton Eta");
	leptonEta->GetXaxis()->SetTitle("Eta");
	leptonEta->GetYaxis()->SetTitle("# events");

	TH1F* evtHt = new TH1F("evtHt", "Event H_T", 700, 10, 710);
	evtHt->SetTitle("Event H_T");
	evtHt->GetXaxis()->SetTitle("H_T [GeV]");
	evtHt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* neutPt = new TH1F("neutPt", "Neutrino pT", 400, 10, 410);
	neutPt->SetTitle("Neutrino pT");
	neutPt->GetXaxis()->SetTitle("pT [GeV]");
	neutPt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* b1Pt = new TH1F("b1Pt", "1st b pT", 400, 10, 410);
	b1Pt->SetTitle("1st b pT");
	b1Pt->GetXaxis()->SetTitle("pT [GeV]");
	b1Pt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* b2Pt = new TH1F("b2Pt", "2nd b pT", 400, 10, 410);
	b2Pt->SetTitle("2nd b pT");
	b2Pt->GetXaxis()->SetTitle("pT [GeV]");
	b2Pt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* b1Eta = new TH1F("b1Eta", "1st b Eta", 60, -3, 3);
	b1Eta->SetTitle("1st b Eta");
	b1Eta->GetXaxis()->SetTitle("Eta");
	b1Eta->GetYaxis()->SetTitle("# events");

	TH1F* b2Eta = new TH1F("b2Eta", "2nd Eta", 60, -3, 3);
	b2Eta->SetTitle("2nd b Eta");
	b2Eta->GetXaxis()->SetTitle("Eta");
	b2Eta->GetYaxis()->SetTitle("# events");



	TH1F* b1hadPt = new TH1F("b1hadPt", "1st hadronic (W decay) b pT", 400, 10, 410);
	b1hadPt->SetTitle("1st had b pT");
	b1hadPt->GetXaxis()->SetTitle("pT [GeV]");
	b1hadPt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* b2hadPt = new TH1F("b2hadPt", "2nd hadronic (W decay) b pT", 400, 10, 410);
	b2hadPt->SetTitle("2nd had b pT");
	b2hadPt->GetXaxis()->SetTitle("pT [GeV]");
	b2hadPt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* b1hadEta = new TH1F("b1hadEta", "1st hadronic (W decay) b Eta", 60, -3, 3);
	b1hadEta->SetTitle("1st had b Eta");
	b1hadEta->GetXaxis()->SetTitle("Eta");
	b1hadEta->GetYaxis()->SetTitle("# events");

	TH1F* b2hadEta = new TH1F("b2hadEta", "2nd hadronic (W decay) Eta", 60, -3, 3);
	b2hadEta->SetTitle("2nd had b Eta");
	b2hadEta->GetXaxis()->SetTitle("Eta");
	b2hadEta->GetYaxis()->SetTitle("# events");



	// Reconstructed-level plots
	TH1F* recoleptonPt = new TH1F("recoleptonPt", "Reco Lepton pT", 400, 10, 410);
	recoleptonPt->SetTitle("Reco Lepton pT");
	recoleptonPt->GetXaxis()->SetTitle("pT [GeV]");
	recoleptonPt->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recoleptonEta = new TH1F("recoleptonEta", "Reco Lepton Eta", 60, -3, 3);
	recoleptonEta->SetTitle("Reco Lepton Eta");
	recoleptonEta->GetXaxis()->SetTitle("Eta");
	recoleptonEta->GetYaxis()->SetTitle("# events");

	TH1F* evtMet = new TH1F("evtMet", "Event MET", 300, 10, 310);
	evtMet->SetTitle("Event MET");
	evtMet->GetXaxis()->SetTitle("MET [GeV]");
	evtMet->GetYaxis()->SetTitle("# events / GeV");


	TH1F* tmassW = new TH1F("tmassW", "W Transverse Mass", 500, 10, 510);
	tmassW->SetTitle("W Transverse Mass");
	tmassW->GetXaxis()->SetTitle("Mass [GeV]");
	tmassW->GetYaxis()->SetTitle("# events / GeV");


	TH1F* recojet1Et = new TH1F("recojet1Et", "Reco Jet1 E_T", 400, 10, 410);
	recojet1Et->SetTitle("Reco Jet1 ET");
	recojet1Et->GetXaxis()->SetTitle("pT [GeV]");
	recojet1Et->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recojet1Eta = new TH1F("recojet1Eta", "Reco Jet1 Eta", 60, -3, 3);
	recojet1Eta->SetTitle("Reco Jet1 Eta");
	recojet1Eta->GetXaxis()->SetTitle("Eta");
	recojet1Eta->GetYaxis()->SetTitle("# events");

	TH1F* recojet2Et = new TH1F("recojet2Et", "Reco Jet2 E_T", 400, 10, 410);
	recojet2Et->SetTitle("Reco Jet2 ET");
	recojet2Et->GetXaxis()->SetTitle("pT [GeV]");
	recojet2Et->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recojet2Eta = new TH1F("recojet2Eta", "Reco Jet2 Eta", 60, -3, 3);
	recojet2Eta->SetTitle("Reco Jet2 Eta");
	recojet2Eta->GetXaxis()->SetTitle("Eta");
	recojet2Eta->GetYaxis()->SetTitle("# events");

	TH1F* recojet3Et = new TH1F("recojet3Et", "Reco Jet3 E_T", 400, 10, 410);
	recojet3Et->SetTitle("Reco Jet3 ET");
	recojet3Et->GetXaxis()->SetTitle("pT [GeV]");
	recojet3Et->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recojet3Eta = new TH1F("recojet3Eta", "Reco Jet3 Eta", 60, -3, 3);
	recojet3Eta->SetTitle("Reco Jet3 Eta");
	recojet3Eta->GetXaxis()->SetTitle("Eta");
	recojet3Eta->GetYaxis()->SetTitle("# events");

	TH1F* recojet4Et = new TH1F("recojet4Et", "Reco Jet4 E_T", 400, 10, 410);
	recojet4Et->SetTitle("Reco Jet4 ET");
	recojet4Et->GetXaxis()->SetTitle("pT [GeV]");
	recojet4Et->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recojet4Eta = new TH1F("recojet4Eta", "Reco Jet4 Eta", 60, -3, 3);
	recojet4Eta->SetTitle("Reco Jet4 Eta");
	recojet4Eta->GetXaxis()->SetTitle("Eta");
	recojet4Eta->GetYaxis()->SetTitle("# events");

	TH1F* recojet5Et = new TH1F("recojet5Et", "Reco Jet5+ E_T", 400, 10, 410);
	recojet5Et->SetTitle("Reco Jet5+ ET");
	recojet5Et->GetXaxis()->SetTitle("pT [GeV]");
	recojet5Et->GetYaxis()->SetTitle("# events / GeV");

	TH1F* recojet5Eta = new TH1F("recojet5Eta", "Reco Jet5+ Eta", 60, -3, 3);
	recojet5Eta->SetTitle("Reco Jet5+ Eta");
	recojet5Eta->GetXaxis()->SetTitle("Eta");
	recojet5Eta->GetYaxis()->SetTitle("# events");






	//tree->Show(1);
/*
Structure of the tree:
0 evt.AlpgenW     = 0.00141968
 evt.Run         = 167541
 evt.Event       = 2017
 evt.isGoodSiRun = 1
 evt.Trigger     = 0
 evt.passedQCDveto = 1
 evt.nJets       = 3
 evt.nLJets      = 1
 evt.nJetsPosTag = 1
 evt.nJetsNegTag = 0
10 evt.Met         = 19.3993
 evt.MetPhi      = 5.93798
 evt.nZvtx       = 1
 evt.nZvtx2      = 1
 lep.isEle       = 1
 lep.isMuo       = 0
 lep.Type        = -99
 lep.Eff         = 0.9533
 lep.Err         = 0.0079
 lep.Pt          = 25.5624
20 lep.Eta         = 0.36331
 lep.Phi         = 2.83035
 lep.deltaPhiMet = 0.0323727
 lep.mTW         = 46.2525
 lep.TrkFiducial = -99
 lep.nSiHits     = 10
 lep.nAxHits     = 43
 lep.nStHits     = 47
 lep.nAxSeg      = 4
 lep.nStSeg      = 4
30 lep.TrkChi2     = -99
 lep.Charge      = 1
 lep.BCCharge    = 1
 lep.D0          = -0.00284837
 lep.Z0          = -23.5029
 lep.Isol        = 0.0462783
 jet1.EtRaw      = 52.644
 jet1.Et         = 63.6811
 jet1.Eta        = 0.61037
 jet1.EtaDet     = 0.508568
40 jet1.Phi        = 1.16243
 jet1.deltaPhiMet = 1.50763
 jet1.EmFrac     = 0.710466
 jet1.isTagged   = 0
 jet1.isLTagged  = 1
 jet1.isUTagged  = 0
 jet1.isTaggable = 1
 jet1.bTagProb   = 0
 jet1.bTagErr    = 0.1
 jet1.bLTagProb  = 0.95
50 jet1.bLTagErr   = 0.1
 jet1.bUTagProb  = 0
 jet1.bUTagErr   = 0.1
 jet1.matchBottom = 0
 jet1.matchCharm = 1
 jet2.EtRaw      = 36.5206
 jet2.Et         = 45.2759
 jet2.Eta        = -0.274839
 jet2.EtaDet     = -0.367717
 jet2.Phi        = 4.18365
60 jet2.deltaPhiMet = 1.75434
 jet2.EmFrac     = 0.563447
 jet2.isTagged   = 1
 jet2.isLTagged  = 1
 jet2.isUTagged  = 1
 jet2.isTaggable = 1
 jet2.bTagProb   = 0.95
 jet2.bTagErr    = 0.05
 jet2.bLTagProb  = 0.95
 jet2.bLTagErr   = 0.05
70 jet2.bUTagProb  = 0.88
 jet2.bUTagErr   = 0.05
 jet2.matchBottom = 1
 jet2.matchCharm = 0
 jet3.EtRaw      = 22.6939
 jet3.Et         = 35.2146
 jet3.Eta        = -1.06508
 jet3.EtaDet     = -1.12813
 jet3.Phi        = 4.52059
 jet3.deltaPhiMet = 1.41739
80 jet3.EmFrac     = 0.740363
 jet3.isTagged   = 0
 jet3.isLTagged  = 0
 jet3.isUTagged  = 0
 jet3.isTaggable = 1
 jet3.bTagProb   = 0
 jet3.bTagErr    = 0
 jet3.bLTagProb  = 0
 jet3.bLTagErr   = 0
 jet3.bUTagProb  = 0
90 jet3.bUTagErr   = 0
 jet3.matchBottom = 1
 jet3.matchCharm = 1
 jet4.EtRaw      = -999
 jet4.Et         = -999
 jet4.Eta        = -999
 jet4.EtaDet     = -999
 jet4.Phi        = -999
 jet4.deltaPhiMet = -999
 jet4.EmFrac     = -999
100 jet4.isTagged   = -999
 jet4.isLTagged  = -999
 jet4.isUTagged  = -999
 jet4.isTaggable = -999
 jet4.bTagProb   = -999
 jet4.bTagErr    = -999
 jet4.bLTagProb  = -999
 jet4.bLTagErr   = -999
 jet4.bUTagProb  = -999
 jet4.bUTagErr   = -999
110 jet4.matchBottom = -999
 jet4.matchCharm = -999
 jet5.EtRaw      = -999
 jet5.Et         = -999
 jet5.Eta        = -999
 jet5.EtaDet     = -999
 jet5.Phi        = -999
 jet5.deltaPhiMet = -999
 jet5.EmFrac     = -999
 jet5.isTagged   = -999
120 jet5.isLTagged  = -999
 jet5.isUTagged  = -999
 jet5.isTaggable = -999
 jet5.bTagProb   = -999
 jet5.bTagErr    = -999
 jet5.bLTagProb  = -999
 jet5.bLTagErr   = -999
 jet5.bUTagProb  = -999
 jet5.bUTagErr   = -999
 jet5.matchBottom = -999
130 jet5.matchCharm = -999
 jet6.EtRaw      = -999
 jet6.Et         = -999
 jet6.Eta        = -999
 jet6.EtaDet     = -999
 jet6.Phi        = -999
 jet6.deltaPhiMet = -999
 jet6.EmFrac     = -999
 jet6.isTagged   = -999
 jet6.isLTagged  = -999
140 jet6.isUTagged  = -999
 jet6.isTaggable = -999
 jet6.bTagProb   = -999
 jet6.bTagErr    = -999
 jet6.bLTagProb  = -999
 jet6.bLTagErr   = -999
 jet6.bUTagProb  = -999
 jet6.bUTagErr   = -999
 jet6.matchBottom = -999
 jet6.matchCharm = -999
150 jet7.EtRaw      = -999
 jet7.Et         = -999
 jet7.Eta        = -999
 jet7.EtaDet     = -999
 jet7.Phi        = -999
 jet7.deltaPhiMet = -999
 jet7.EmFrac     = -999
 jet7.isTagged   = -999
 jet7.isLTagged  = -999
 jet7.isUTagged  = -999
160 jet7.isTaggable = -999
 jet7.bTagProb   = -999
 jet7.bTagErr    = -999
 jet7.bLTagProb  = -999
 jet7.bLTagErr   = -999
 jet7.bUTagProb  = -999
 jet7.bUTagErr   = -999
 jet7.matchBottom = -999
 jet7.matchCharm = -999
 jet8.EtRaw      = -999
170 jet8.Et         = -999
 jet8.Eta        = -999
 jet8.EtaDet     = -999
 jet8.Phi        = -999
 jet8.deltaPhiMet = -999
 jet8.EmFrac     = -999
 jet8.isTagged   = -999
 jet8.isLTagged  = -999
 jet8.isUTagged  = -999
 jet8.isTaggable = -999
180 jet8.bTagProb   = -999
 jet8.bTagErr    = -999
 jet8.bLTagProb  = -999
 jet8.bLTagErr   = -999
 jet8.bUTagProb  = -999
 jet8.bUTagErr   = -999
 jet8.matchBottom = -999
 jet8.matchCharm = -999
 bjet1.EtRaw     = 36.5206
 bjet1.Et        = 45.2759
190 bjet1.Eta       = -0.274839
 bjet1.EtaDet    = -0.367717
 bjet1.Phi       = 4.18365
 bjet1.deltaPhiMet = 1.38566
 bjet1.bTagProb  = 0.95
 bjet1.bTagErr   = 0.05
 bjet1.Mvtx      = 3.54378
 bjet1.PtVtx     = 18.6263
 bjet1.L2d       = 0.138714
 bjet1.flavor    = 5
 bjet1.KaNN      = 0.931117
 bjet1.KaNNcorr  = 0.931117
 bjet1.KNN.NtrackSig3 = 5
 bjet1.KNN.signD0sig1 = 8.3086
 bjet1.KNN.signD02 = -0.00567775
 bjet1.KNN.signD03 = 0.00477791
 bjet1.KNN.Ntrk  = 8
 bjet1.KNN.ctau  = 0.0263913
 bjet2.EtRaw     = -999
 bjet2.Et        = -999
 bjet2.Eta       = -999
 bjet2.EtaDet    = -999
 bjet2.Phi       = -999
 bjet2.deltaPhiMet = -999
 bjet2.bTagProb  = -999
 bjet2.bTagErr   = -999
 bjet2.Mvtx      = -999
 bjet2.PtVtx     = -999
 bjet2.L2d       = -999
 bjet2.flavor    = -999
 bjet2.KaNN      = -999
 bjet2.KaNNcorr  = -999
 bjet2.KNN.NtrackSig3 = -999
 bjet2.KNN.signD0sig1 = -999
 bjet2.KNN.signD02 = -999
 bjet2.KNN.signD03 = -999
 bjet2.KNN.Ntrk  = -999
 bjet2.KNN.ctau  = -999
 bjetL1.EtRaw    = 52.644
 bjetL1.Et       = 63.6811
 bjetL1.Eta      = 0.61037
 bjetL1.EtaDet   = 0.508568
 bjetL1.Phi      = 1.16243
 bjetL1.deltaPhiMet = 1.63555
 bjetL1.bTagProb = 0
 bjetL1.bTagErr  = 0.1
 bjetL1.Mvtx     = 2.03037
 bjetL1.PtVtx    = 18.1567
 bjetL1.L2d      = 0.633287
 bjetL1.flavor   = 4
 bjetL1.KaNN     = 0.114065
 bjetL1.KaNNcorr = 0.114065
 bjetL1.KNN.NtrackSig3 = 2
 bjetL1.KNN.signD0sig1 = -0.632533
 bjetL1.KNN.signD02 = 0.0736583
 bjetL1.KNN.signD03 = 0.00256617
 bjetL1.KNN.Ntrk = 10
 bjetL1.KNN.ctau = 0.0708173
 bjetL2.EtRaw    = 36.5206
 bjetL2.Et       = 45.2759
 bjetL2.Eta      = -0.274839
 bjetL2.EtaDet   = -0.367717
 bjetL2.Phi      = 4.18365
 bjetL2.deltaPhiMet = 1.38566
 bjetL2.bTagProb = 0.95
 bjetL2.bTagErr  = 0.05
 bjetL2.Mvtx     = 4.1818
 bjetL2.PtVtx    = 25.8168
 bjetL2.L2d      = 0.14065
 bjetL2.flavor   = 5
 bjetL2.KaNN     = 0.911994
 bjetL2.KaNNcorr = 0.911994
 bjetL2.KNN.NtrackSig3 = 5
 bjetL2.KNN.signD0sig1 = 8.3086
 bjetL2.KNN.signD02 = -0.00567775
 bjetL2.KNN.signD03 = 0.00477791
 bjetL2.KNN.Ntrk = 8
 bjetL2.KNN.ctau = 0.0227825
 bjetU1.EtRaw    = 36.5206
 bjetU1.Et       = 45.2759
 bjetU1.Eta      = -0.274839
 bjetU1.EtaDet   = -0.367717
 bjetU1.Phi      = 4.18365
 bjetU1.deltaPhiMet = 1.38566
 bjetU1.bTagProb = 0.95
 bjetU1.bTagErr  = 0.05
 bjetU1.Mvtx     = 2.66048
 bjetU1.PtVtx    = 14.5605
 bjetU1.L2d      = 0.138748
 bjetU1.flavor   = 5
 bjetU1.KaNN     = 0.875975
 bjetU1.KaNNcorr = 0.875975
 bjetU1.KNN.NtrackSig3 = 5
 bjetU1.KNN.signD0sig1 = 8.3086
 bjetU1.KNN.signD02 = -0.00567775
 bjetU1.KNN.signD03 = 0.00477791
 bjetU1.KNN.Ntrk = 8
 bjetU1.KNN.ctau = 0.025352
 bjetU2.EtRaw    = -999
 bjetU2.Et       = -999
 bjetU2.Eta      = -999
 bjetU2.EtaDet   = -999
 bjetU2.Phi      = -999
 bjetU2.deltaPhiMet = -999
 bjetU2.bTagProb = -999
 bjetU2.bTagErr  = -999
 bjetU2.Mvtx     = -999
 bjetU2.PtVtx    = -999
 bjetU2.L2d      = -999
 bjetU2.flavor   = -999
 bjetU2.KaNN     = -999
 bjetU2.KaNNcorr = -999
 bjetU2.KNN.NtrackSig3 = -999
 bjetU2.KNN.signD0sig1 = -999
 bjetU2.KNN.signD02 = -999
 bjetU2.KNN.signD03 = -999
 bjetU2.KNN.Ntrk = -999
 bjetU2.KNN.ctau = -999
 evt.nLposTags   = 2
 evt.nTposTags   = 1
 evt.nUposTags   = 1
 evt.TmatchB     = 1
 evt.TmatchC     = 0
 evt.LmatchB     = 1
 evt.LmatchC     = 0
 evt.UmatchB     = 1
 evt.UmatchC     = 0
 evt.bProb0T     = 0.05
 evt.bProb0L     = 0.0025
 evt.bProb0U     = 0.12
 evt.bProb1T     = 0.95
 evt.bProb1L     = 0.095
 evt.bProb1U     = 0.88
 evt.bProb2T     = 0
 evt.bProb2L     = 0.9025
 evt.bProb2U     = 0
 evt.bProb12T    = 0.95
 evt.bProb12L    = 0.9975
 evt.bProb12U    = 0.88
 evt.bProb0Tpl   = 0.1
 evt.bProb0Lpl   = 0.015
 evt.bProb0Upl   = 0.17
 evt.bProb1Tpl   = 0.9
 evt.bProb1Lpl   = 0.22
 evt.bProb1Upl   = 0.83
 evt.bProb2Tpl   = 0
 evt.bProb2Lpl   = 0.765
 evt.bProb2Upl   = 0
 evt.bProb12Tpl  = 0.9
 evt.bProb12Lpl  = 0.985
 evt.bProb12Upl  = 0.83
 evt.bProb0Tmn   = 0
 evt.bProb0Lmn   = -0
 evt.bProb0Umn   = 0.07
 evt.bProb1Tmn   = 1
 evt.bProb1Lmn   = -0.05
 evt.bProb1Umn   = 0.93
 evt.bProb2Tmn   = 0
 evt.bProb2Lmn   = 1.05
 evt.bProb2Umn   = 0
 evt.bProb12Tmn  = 1
 evt.bProb12Lmn  = 1
 evt.bProb12Umn  = 0.93
 evt.jetSumEt    = 144.172
 evt.Ht          = 189.133
 evt.dR12        = 3.14823
 evt.M12         = 117.857
 ANN.Aplanarity  = 0.0419711
 ANN.EtaMax      = 0.61037
 ANN.Ht          = 187.277
360 ANN.MinDijetMass = 39.0437
 ANN.MinDijetSep = 0.859071
 ANN.SumEtJet    = 34.824
 ANN.SumPzSumEt  = -0.111377
 ANN.3j          = 0.331211
 ANN.4j          = 0.0286571
 gen.isLJ        = 1
 gen.b.Pt        = 57.0592
 gen.b.Eta       = -1.07788
 gen.b.Phi       = -1.76959
370 gen.bbar.Pt     = 50.517
 gen.bbar.Eta    = -0.263085
 gen.bbar.Phi    = -2.07693
 gen.blep.Pt     = 57.0592
 gen.blep.Eta    = -1.07788
 gen.blep.Phi    = -1.76959
 gen.bhad.Pt     = 50.517
 gen.bhad.Eta    = -0.263085
 gen.bhad.Phi    = -2.07693
 gen.q.Pt        = 16.4576
380 gen.q.Eta       = -1.21419
 gen.q.Phi       = 0.662484
 gen.qbar.Pt     = 82.022
 gen.qbar.Eta    = 0.627746
 gen.qbar.Phi    = 1.17934
 gen.neu.Pt      = 35.7115
 gen.neu.Eta     = 0.58855
 gen.neu.Phi     = -0.00992364
 gen.lep.Pt      = 45.1613
 gen.lep.Eta     = 0.391046
390 gen.lep.Phi     = 2.8156
 jet.blep.EtRaw  = 22.6939
 jet.blep.Et     = 35.2146
 jet.blep.Eta    = -1.06508
 jet.blep.EtaDet = -1.12813
 jet.blep.Phi    = 4.52059
 jet.blep.deltaPhiMet = 1.41739
 jet.blep.deltaR = 0.0145906
 jet.bhad.EtRaw  = 36.5206
 jet.bhad.Et     = 45.2759
 jet.bhad.Eta    = -0.274839
 jet.bhad.EtaDet = -0.367717
 jet.bhad.Phi    = 4.18365
 jet.bhad.deltaPhiMet = 1.75434
 jet.bhad.deltaR = 0.0254787
 jet.q.EtRaw     = -999
 jet.q.Et        = -999
 jet.q.Eta       = -999
 jet.q.EtaDet    = -999
 jet.q.Phi       = -999
 jet.q.deltaPhiMet = -999
 jet.q.deltaR    = -999
 jet.qbar..EtRaw = 52.644
 jet.qbar..Et    = 63.6811
 jet.qbar..Eta   = 0.61037
 jet.qbar..EtaDet = 0.508568
 jet.qbar..Phi   = 1.16243
 jet.qbar..deltaPhiMet = 1.50763
 jet.qbar..deltaR = 0.0242505
 
 */
 
	// Loop over all events
	for (Int_t i = 0; i < tree ->GetEntries(); i++) {
		tree->GetEntry(i);
		TObjArray* leaves = tree ->GetListOfLeaves();
		
		TLeaf* lepptLeaf = (TLeaf*) leaves->UncheckedAt(388);
		TLeaf* lepetaLeaf = (TLeaf*) leaves->UncheckedAt(389);
		TLeaf* htLeaf = (TLeaf*) leaves->UncheckedAt(354);
		TLeaf* b1ptLeaf = (TLeaf*) leaves->UncheckedAt(367);
		TLeaf* b1etaLeaf = (TLeaf*) leaves->UncheckedAt(368);
		TLeaf* b2ptLeaf = (TLeaf*) leaves->UncheckedAt(370);
		TLeaf* b2etaLeaf = (TLeaf*) leaves->UncheckedAt(371);
		TLeaf* neutptLeaf = (TLeaf*) leaves->UncheckedAt(385);
		TLeaf* b1hadptLeaf = (TLeaf*) leaves->UncheckedAt(373);
		TLeaf* b1hadetaLeaf = (TLeaf*) leaves->UncheckedAt(374);
		TLeaf* b2hadptLeaf = (TLeaf*) leaves->UncheckedAt(376);
		TLeaf* b2hadetaLeaf = (TLeaf*) leaves->UncheckedAt(377);


		TLeaf* recolepptLeaf = (TLeaf*) leaves->UncheckedAt(19);
		TLeaf* recolepetaLeaf = (TLeaf*) leaves->UncheckedAt(20);
		TLeaf* metLeaf = (TLeaf*) leaves->UncheckedAt(10);
		TLeaf* mTWLeaf = (TLeaf*) leaves->UncheckedAt(23);
		TLeaf* njetsLeaf = (TLeaf*) leaves->UncheckedAt(6);

		TLeaf* jet1etLeaf = (TLeaf*) leaves->UncheckedAt(37);
		TLeaf* jet1etaLeaf = (TLeaf*) leaves->UncheckedAt(38);
		TLeaf* jet2etLeaf = (TLeaf*) leaves->UncheckedAt(56);
		TLeaf* jet2etaLeaf = (TLeaf*) leaves->UncheckedAt(57);
		TLeaf* jet3etLeaf = (TLeaf*) leaves->UncheckedAt(75);
		TLeaf* jet3etaLeaf = (TLeaf*) leaves->UncheckedAt(76);
		TLeaf* jet4etLeaf = (TLeaf*) leaves->UncheckedAt(94);
		TLeaf* jet4etaLeaf = (TLeaf*) leaves->UncheckedAt(95);
		TLeaf* jet5etLeaf = (TLeaf*) leaves->UncheckedAt(113);
		TLeaf* jet5etaLeaf = (TLeaf*) leaves->UncheckedAt(114);
		TLeaf* jet6etLeaf = (TLeaf*) leaves->UncheckedAt(132);
		TLeaf* jet6etaLeaf = (TLeaf*) leaves->UncheckedAt(133);
		TLeaf* jet7etLeaf = (TLeaf*) leaves->UncheckedAt(151);
		TLeaf* jet7etaLeaf = (TLeaf*) leaves->UncheckedAt(152);
		TLeaf* jet8etLeaf = (TLeaf*) leaves->UncheckedAt(170);
		TLeaf* jet8etaLeaf = (TLeaf*) leaves->UncheckedAt(171);


		njets = njetsLeaf->GetValue(0);

		leppt = lepptLeaf->GetValue(0);
		lepeta = lepetaLeaf->GetValue(0);
		ht = htLeaf->GetValue(0);
		b1pt = b1ptLeaf->GetValue(0);
		b1eta = b1etaLeaf->GetValue(0);
		b2pt = b2ptLeaf->GetValue(0);
		b2eta = b2etaLeaf->GetValue(0);
		neutpt = neutptLeaf->GetValue(0);
		b1hadpt = b1hadptLeaf->GetValue(0);
		b1hadeta = b1hadetaLeaf->GetValue(0);
		b2hadpt = b2hadptLeaf->GetValue(0);
		b2hadeta = b2hadetaLeaf->GetValue(0);


		recoleppt = recolepptLeaf->GetValue(0);
		recolepeta = recolepetaLeaf->GetValue(0);
		met = metLeaf->GetValue(0);
		mTW = mTWLeaf->GetValue(0);


		jet1et = jet1etLeaf->GetValue(0);
		jet1eta = jet1etaLeaf->GetValue(0);
		jet2et = jet2etLeaf->GetValue(0);
		jet2eta = jet2etaLeaf->GetValue(0);
		jet3et = jet3etLeaf->GetValue(0);
		jet3eta = jet3etaLeaf->GetValue(0);
		jet4et = jet4etLeaf->GetValue(0);
		jet4eta = jet4etaLeaf->GetValue(0);
		jet5et = jet5etLeaf->GetValue(0);
		jet5eta = jet5etaLeaf->GetValue(0);
		jet6et = jet6etLeaf->GetValue(0);
		jet6eta = jet6etaLeaf->GetValue(0);
		jet7et = jet7etLeaf->GetValue(0);
		jet7eta = jet7etaLeaf->GetValue(0);
		jet8et = jet8etLeaf->GetValue(0);
		jet8eta = jet8etaLeaf->GetValue(0);



		leptonPt->Fill(leppt);
		leptonEta->Fill(lepeta);
		evtHt->Fill(ht);
		b1Pt->Fill(b1pt);
		b1Eta->Fill(b1eta);
		b2Pt->Fill(b2pt);
		b2Eta->Fill(b2eta);
		neutPt->Fill(neutpt);
		b1hadPt->Fill(b1hadpt);
		b1hadEta->Fill(b1hadeta);
		b2hadPt->Fill(b2hadpt);
		b2hadEta->Fill(b2hadeta);



		recoleptonPt->Fill(recoleppt);
		recoleptonEta->Fill(recolepeta);
		evtMet->Fill(met);
		tmassW->Fill(mTW);

		recojet1Et->Fill(jet1et);
		recojet1Eta->Fill(jet1eta);

		if (jet2et > 0) {
		  recojet2Et->Fill(jet2et);
		  recojet2Eta->Fill(jet2eta);
		}

		if (jet3et > 0) {
		  recojet3Et->Fill(jet3et);
		  recojet3Eta->Fill(jet3eta);
		}
		
		if (jet4et > 0) {
		  recojet4Et->Fill(jet4et);
		  recojet4Eta->Fill(jet4eta);
		}
		
		if (jet5et > 0) {
		  recojet5Et->Fill(jet5et);
		  recojet5Eta->Fill(jet5eta);
		}


		if (jet6et > 0) {
		  recojet5Et->Fill(jet6et);
		  recojet5Eta->Fill(jet6eta);
		}

		if (jet7et > 0) {
		  recojet5Et->Fill(jet7et);
		  recojet5Eta->Fill(jet7eta);
		}

		if (jet8et > 0) {
		  recojet5Et->Fill(jet8et);
		  recojet5Eta->Fill(jet8eta);
		}




	}	// for loop over events
	
	leptonPt ->SetDirectory(0);
	leptonEta->SetDirectory(0);
	evtHt->SetDirectory(0);
	neutPt->SetDirectory(0);
	b1Pt->SetDirectory(0);
	b2Pt->SetDirectory(0);
	b1Eta->SetDirectory(0);
	b2Eta->SetDirectory(0);
	b1hadPt->SetDirectory(0);
	b2hadPt->SetDirectory(0);
	b1hadEta->SetDirectory(0);
	b2hadEta->SetDirectory(0);


	recoleptonPt ->SetDirectory(0);
	recoleptonEta->SetDirectory(0);
	evtMet->SetDirectory(0);
	tmassW->SetDirectory(0);

	recojet1Et ->SetDirectory(0);
	recojet1Eta->SetDirectory(0);
	recojet2Et ->SetDirectory(0);
	recojet2Eta->SetDirectory(0);
	recojet3Et ->SetDirectory(0);
	recojet3Eta->SetDirectory(0);
	recojet4Et ->SetDirectory(0);
	recojet4Eta->SetDirectory(0);
	recojet5Et ->SetDirectory(0);
	recojet5Eta->SetDirectory(0);


	TLine line;
	line.SetLineWidth(3);
	// draw the plots here
	TCanvas* c1 = new TCanvas("c1","pT of the lepton",900,600);
	c1->SetLogy(1);
	leptonPt->Draw();
	line.DrawLine(15,0,15,20000);
	c1->SaveAs("top_lepton_pt.eps");
	c1->SaveAs("top_lepton_pt.gif");
	
	TCanvas* c2 = new TCanvas("c2","Eta of the lepton",900,600);
	c2->SetLogy(1);
	leptonEta->Draw();
	line.DrawLine(15,0,15,10000);
	c2->SaveAs("top_lepton_eta.eps");
	c2->SaveAs("top_lepton_eta.gif");

	TCanvas* c3 = new TCanvas("c3","H_T of the Event",900,600);
	//	c3->SetLogy(1);
	evtHt->Draw();
	line.DrawLine(15,0,15,10000);
	c3->SaveAs("top_event_ht.eps");
	c3->SaveAs("top_event_ht.gif");
	
	TCanvas* c4 = new TCanvas("c4","pT of the 1st b-quark",900,600);
	c4->SetLogy(1);
	b1Pt->Draw();
	line.DrawLine(15,0,15,10000);
	c4->SaveAs("top_b1_pt.eps");
	c4->SaveAs("top_b1_pt.gif");

	TCanvas* c4a = new TCanvas("c4a","pT of the 2nd b-quark",900,600);
	c4a->SetLogy(1);
	b2Pt->Draw();
	line.DrawLine(15,0,15,10000);
	c4a->SaveAs("top_b2_pt.eps");
	c4a->SaveAs("top_b2_pt.gif");

	TCanvas* c4b = new TCanvas("c4b","pT of the 1st hadronic b-quark",900,600);
	c4b->SetLogy(1);
	b1hadPt->Draw();
	line.DrawLine(15,0,15,10000);
	c4b->SaveAs("top_b1had_pt.eps");
	c4b->SaveAs("top_b1had_pt.gif");

	TCanvas* c4c = new TCanvas("c4c","pT of the 2nd hadronic b-quark",900,600);
	c4c->SetLogy(1);
	b2hadPt->Draw();
	line.DrawLine(15,0,15,10000);
	c4c->SaveAs("top_b2had_pt.eps");
	c4c->SaveAs("top_b2had_pt.gif");


	TCanvas* c4d = new TCanvas("c4d","pT of the neutrino",900,600);
	c4d->SetLogy(1);
	neutPt->Draw();
	line.DrawLine(15,0,15,10000);
	c4d->SaveAs("top_neut_pt.eps");
	c4d->SaveAs("top_neut_pt.gif");

	TCanvas* c5 = new TCanvas("c5","Eta of the 1st b-quark",900,600);
	c5->SetLogy(1);
	b1Eta->Draw();
	line.DrawLine(15,0,15,10000);
	c5->SaveAs("top_b1_eta.eps");
	c5->SaveAs("top_b1_eta.gif");

	TCanvas* c5a = new TCanvas("c5a","Eta of the 2nd b-quark",900,600);
	c5a->SetLogy(1);
	b2Eta->Draw();
	line.DrawLine(15,0,15,10000);
	c5a->SaveAs("top_b2_eta.eps");
	c5a->SaveAs("top_b2_eta.gif");

	TCanvas* c5b = new TCanvas("c5b","Eta of the 1st hadronic b-quark",900,600);
	c5b->SetLogy(1);
	b1hadEta->Draw();
	line.DrawLine(15,0,15,10000);
	c5b->SaveAs("top_b1had_eta.eps");
	c5b->SaveAs("top_b1had_eta.gif");

	TCanvas* c5c = new TCanvas("c5c","Eta of the 2nd hadronic b-quark",900,600);
	c5c->SetLogy(1);
	b2hadEta->Draw();
	line.DrawLine(15,0,15,10000);
	c5c->SaveAs("top_b2had_eta.eps");
	c5c->SaveAs("top_b2had_eta.gif");



	// Make the plots of the reconstructed-level quantities
	TCanvas* c6 = new TCanvas("c6","Reco pT of the lepton",900,600);
	c6->SetLogy(1);
	recoleptonPt->Draw();
	c6->SaveAs("top_lepton_pt_reco.eps");
	c6->SaveAs("top_lepton_pt_reco.gif");
	
	TCanvas* c6a = new TCanvas("c6a","Reco Eta of the lepton",900,600);
	c6a->SetLogy(1);
	recoleptonEta->Draw();
	c6a->SaveAs("top_lepton_eta_reco.eps");
	c6a->SaveAs("top_lepton_eta_reco.gif");

	TCanvas* c7 = new TCanvas("c7","MET of the Event",900,600);
	//	c7->SetLogy(1);
	evtMet->Draw();
	line.DrawLine(15,0,15,8250);
	c7->SaveAs("top_event_met.eps");
	c7->SaveAs("top_event_met.gif");

	TCanvas* c8 = new TCanvas("c8","Reco E_T of the 1st jet",900,600);
	c8->SetLogy(1);
	recojet1Et->Draw();
	c8->SaveAs("top_jet1_et_reco.eps");
	c8->SaveAs("top_jet1_et_reco.gif");

	TCanvas* c8a = new TCanvas("c8a","Reco E_T of the 2nd jet",900,600);
	c8a->SetLogy(1);
	recojet2Et->Draw();
	c8a->SaveAs("top_jet2_et_reco.eps");
	c8a->SaveAs("top_jet2_et_reco.gif");

	TCanvas* c8b = new TCanvas("c8b","Reco E_T of the 3rd jet",900,600);
	c8b->SetLogy(1);
	recojet3Et->Draw();
	c8b->SaveAs("top_jet3_et_reco.eps");
	c8b->SaveAs("top_jet3_et_reco.gif");

	TCanvas* c8c = new TCanvas("c8c","Reco E_T of the 4th jet",900,600);
	c8c->SetLogy(1);
	recojet4Et->Draw();
	c8c->SaveAs("top_jet4_et_reco.eps");
	c8c->SaveAs("top_jet4_et_reco.gif");

	TCanvas* c8d = new TCanvas("c8d","Reco E_T of the 5th+ jets",900,600);
	c8d->SetLogy(1);
	recojet5Et->Draw();
	c8d->SaveAs("top_jet5_et_reco.eps");
	c8d->SaveAs("top_jet5_et_reco.gif");

	TCanvas* c9 = new TCanvas("c9","Reco Eta of the 1st jet",900,600);
	c9->SetLogy(1);
	recojet1Eta->Draw();
	c9->SaveAs("top_jet1_eta_reco.eps");
	c9->SaveAs("top_jet1_eta_reco.gif");

	TCanvas* c9a = new TCanvas("c9a","Reco Eta of the 2nd jet",900,600);
	c9a->SetLogy(1);
	recojet2Eta->Draw();
	c9a->SaveAs("top_jet2_eta_reco.eps");
	c9a->SaveAs("top_jet2_eta_reco.gif");

	TCanvas* c9b = new TCanvas("c9b","Reco Eta of the 3rd jet",900,600);
	c9b->SetLogy(1);
	recojet3Eta->Draw();
	c9b->SaveAs("top_jet3_eta_reco.eps");
	c9b->SaveAs("top_jet3_eta_reco.gif");

	TCanvas* c9c = new TCanvas("c9c","Reco Eta of the 4th jet",900,600);
	c9c->SetLogy(1);
	recojet4Eta->Draw();
	c9c->SaveAs("top_jet4_eta_reco.eps");
	c9c->SaveAs("top_jet4_eta_reco.gif");

	TCanvas* c9d = new TCanvas("c9d","Reco Eta of the 5th+ jets",900,600);
	c9d->SetLogy(1);
	recojet5Eta->Draw();
	c9d->SaveAs("top_jet5_eta_reco.eps");
	c9d->SaveAs("top_jet5_eta_reco.gif");

	TCanvas* c10 = new TCanvas("c10","Reco transverse mass of the W",900,600);
	c10->SetLogy(1);
	tmassW->Draw();
	c10->SaveAs("top_mTW_reco.eps");
	c10->SaveAs("top_mTW_reco.gif");



}	// function
			
			

