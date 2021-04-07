// Double bifircated Gaussian at x with parameters passed in to params
Double_t splitGauss(Double_t *x, Double_t *par);
Double_t halfGauss(Double_t *x, Double_t *par);

void acceptanceErrors()
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.4);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   bool useRootFile = false;
   TH1F accptHist ("accept", "Acceptance with 8% error", 100, 0.7, 1.4);
   TH1F recipHist ("recip", "Reciprical of Acceptance with 8% error", 
                   100, 0.7, 1.4);
   const int kTrials = 1000000;
   if (useRootFile)
   {
      TFile a("random.root");
      TTree *tree = (TTree *) gROOT->FindObject("random");
      if (! tree)
      {
         cerr << "no tree, dude." << endl;
         return;
      }
      tree->Draw ("accept >> accept");
      tree->Draw ("acceptRecip >> recip");
   }
   else 
   {
      // just fill the damn things yourself
      for (int loop = 0; loop < kTrials; ++loop)
      {
         double accept = gRandom->Gaus (1.0, 0.08);
         if (accept <= 0)
         {
            continue;
         }
         accptHist.Fill( accept );
         recipHist.Fill( 1. / accept );
      } // for loop
   } // else if rootfile
   // Split Gaussian Function
   TF1 *splitGaussFunc = new TF1 ("splitGauss", splitGauss, 0.7, 1.4, 8);
   splitGaussFunc->SetParameters(kTrials/300, 1., 0.1, 0.1, 1., 1., 1., 1.);
   splitGaussFunc->SetParNames("Constant",         // 0
                               "Max_bValue",       // 1
                               "Negative_Sigma1",  // 2
                               "Positive_Sigma1",  // 3
                               "Negative_Frac",    // 4
                               "Positive_Frac",    // 5
                               "Negative_SigFrac", // 6
                               "Positive_SigFrac"  // 7
                               );
   splitGaussFunc->FixParameter (1, 1.);
   splitGaussFunc->FixParameter (4, 1.);
   splitGaussFunc->FixParameter (5, 1.);
   splitGaussFunc->FixParameter (6, 1.);
   splitGaussFunc->FixParameter (7, 1.);
   // Half Gaussian Function
   TF1 *halfGaussFunc = new TF1 ("halfGauss", halfGauss, 0.7, 1.4, 5);
   halfGaussFunc->SetParameters (1., 0.1, kTrials / 300, 0.1, kTrials / 300);
   halfGaussFunc->SetParNames ("center",         // 0
                               "Negative_Sigma", // 1
                               "Negative_Norm",  // 2
                               "Positive_Sigma", // 3
                               "Positive_Norm"  // 4
                               );
   halfGaussFunc->FixParameter (0, 1.0);
   
   // set line colors
   accptHist.SetLineColor(kRed);
   recipHist.SetLineColor(kRed);
   accptHist.Draw();
   c1->Print("accept_only.eps");
   
   accptHist.Fit("gaus","W");
   accptHist.Draw();
   c1->Print("accept.eps");
   recipHist.Fit("splitGauss","W");
   recipHist.Draw();
   c1->Print("recip.eps");
   recipHist.Fit("halfGauss", "W");
   recipHist.Draw();
   c1->Print("recip_half.eps");
   recipHist.Fit ("gaus", "W");
   recipHist.Draw();
   c1->Print("recip_gaus.eps");
}

Double_t halfGauss(Double_t *x, Double_t *par)
{
   double value = x[0];
   double arg    = value - par[0];
   double sigma, norm;
   if (arg > 0)
   {
      double sigma = par[3]; // positive sigma
      double norm  = par[4];
   }
   else
   {
      double sigma = par[1]; // negative sigma
      double norm  = par[2];
   }
   Double_t fitval =
      norm * TMath::Exp ( -0.5 * arg * arg / (sigma * sigma) );
   return fitval;
}

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];
   double norm   = par[0];
   double arg    = value - par[1];
   double sigma1 = par[2]; // negative sigma1
   double frac   = par[4]; // negative fraction
   double sigma2 = sigma1 * par[6]; // negative sigma2
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }
   Double_t fitval =
      norm * frac * TMath::Exp( -0.5 *arg * arg / (sigma1 * sigma1)) +
      norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2));
   return fitval;
}

