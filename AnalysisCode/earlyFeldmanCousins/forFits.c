const int kCenter = 0.018;
const int kWidth = 0.005;

Double_t splitGauss(Double_t *x, Double_t *par)
{
   double value = x[0];   
   double norm    = par[0];
   double arg     = value - par[1];
   double sigma1  = par[2]; // negative sigma1
   double frac    = par[4]; // negative fraction
   double sigma2  = sigma1 * par[6]; // negative sigma2
   double zConst  = par[8];
   double zWidth  = par[9];
   double zCenter = par[10];
   if (arg > 0)
   {
      sigma1 = par[2 + 1]; // positive sigma1
      frac   = par[4 + 1]; // positive fraction
      sigma2 = sigma1 * par[6 + 1]; // positive sigma2
   }
   double zValue = value - zCenter;
   Double_t fitval = 
      norm * frac * TMath::Exp( -0.5 * arg * arg / (sigma1 * sigma1)) 
      + norm * (1 - frac) * TMath::Exp( -0.5 * arg * arg / (sigma2 * sigma2))
      + zConst * TMath::Exp( -0.5 * zValue * zValue / (zWidth * zWidth));
   return fitval;
}

void fixParameter(TF1 *func, int parameter)
{
   double value = func->GetParameter(parameter);
   func->FixParameter(parameter, value);
}

void floatParameter(TF1 *func, int param, double lower, double higher)
{
   double value = func->GetParameter(param);
   func->SetParLimits(param, lower, higher);
   func->SetParameter(param, value);
}


void setNames (TF1 *func)
{
      func->SetParNames("Constant",         //  0
                        "Max_bValue",       //  1
                        "Negative_Sigma1",  //  2
                        "Positive_Sigma1",  //  3
                        "Negative_Frac",    //  4
                        "Positive_Frac",    //  5
                        "Negative_SigFrac", //  6
                        "Positive_SigFrac", //  7
                        "Zero_Const",       //  8
                        "Zero_Width",       //  9
                        "Zero_Center"       // 10
         );
}

void fixNonBump (TF1 *func, int loop = 3)
{
      // fix everything but the Zero_Const
      func->FixParameter(0, 0.); // Constant",        
      func->FixParameter(1, loop/100.); // Max_bValue",      
      func->FixParameter(2, 0.1); // Negative_Sigma1", 
      func->FixParameter(3, 0.1); // Positive_Sigma1", 
      func->FixParameter(4, 0.5); // Negative_Frac",   
      func->FixParameter(5, 0.5); // Positive_Frac",   
      func->FixParameter(6, 3.); // Negative_SigFrac",
      func->FixParameter(7, 3.); // Positive_SigFrac",
      // tell parameter 8 it must be positive
      func->SetParLimits(8, 0, 1e6);
      func->SetParLimits(9, 0, 0.01);
      func->SetParLimits(10, 0, 0.02);
      func->SetParameter(8, 8e3);
      func->SetParameter(9, kWidth);
      func->SetParameter(10, kCenter);
}

void fixBump (TF1 *func)
{
   //gMinuit->Release(0);
   //gMinuit->Release(1);
   //gMinuit->Release(2);
   //gMinuit->Release(3);
   //gMinuit->Release(4);
   //gMinuit->Release(5);
   //gMinuit->Release(6);
   //gMinuit->Release(7);
   //gMinuit->FixParameter(8);
   //gMinuit->FixParameter(9);
   //gMinuit->FixParameter(10);

   floatParameter(func, 0, 0., 1e6);
   floatParameter(func, 1, -0.1, 1.1);
   floatParameter(func, 2, 0., 3.);
   floatParameter(func, 3, 0., 3.);
   floatParameter(func, 4, 0, 1);
   floatParameter(func, 5, 0, 1);
   floatParameter(func, 6, 0.2, 5);
   floatParameter(func, 7, 0.2, 5);
   fixParameter(func, 8);
   fixParameter(func, 9);
   fixParameter(func, 10);
}

void floatBump (TF1* func) 
{
   floatParameter(func,  8, 0, 1e6);
   floatParameter(func,  9, 0, 0.03);
   floatParameter(func, 10, 0, 0.02);
}


void forFits() 
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptFit(0011);
   gStyle->SetStatH(0.4);          // height of box
   gStyle->SetStatW(0.1);         // width of box 
   cout << "TFile file (\"all_short.root\") " << endl;
   cout << "TH1F *h05 = (TH1F*) file.Get(\"Rgen005_short\")" << endl;   
   cout << "TF1 *func = new TF1(\"splitGauss\",splitGauss,0,1,11);" << endl;
   cout << "setNames(func)" << endl
        << "fixNonBump(func)" << endl;
   cout << "h05->Fit(\"splitGauss\",\"W\");" << endl;
   cout << "fixBump(func)" << endl;
   cout << "h05->Fit(\"splitGauss\",\"W\");" << endl;
   cout << "floatBump(func)" << endl;
   cout << "h05->Fit(\"splitGauss\",\"W\");" << endl;
   cout << "h05->Fit(\"splitGauss\",\"M\");" << endl;
}

/*
TFile file ("all.root")
TH1F *h05 = (TH1F*) file.Get("Rgen005")

      TF1 *func = new TF1("splitGauss",splitGauss,0,1,10);
      // Sets initial values and parameter names
      // fix everything but the Zero_Const
      func->FixParameter(0, 0.); // Constant",        
      func->FixParameter(1, loop/100.); // Max_bValue",      
      func->FixParameter(2, 0.1); // Negative_Sigma1", 
      func->FixParameter(3, 0.1); // Positive_Sigma1", 
      func->FixParameter(4, 0.5); // Negative_Frac",   
      func->FixParameter(5, 0.5); // Positive_Frac",   
      func->FixParameter(6, 3.); // Negative_SigFrac",
      func->FixParameter(7, 3.); // Positive_SigFrac",
      func->FixParameter(9, 0.002); // Zero_Width"       
      // tell parameter 8 it must be positive
      func->SetParLimits(8, 0, 1e6);
      hist->Fit("splitGauss","W");

      func->SetParameters(3000, 0.05, 0.1, 0.1, 0.5, 0.5, 3, 3, 
                          30000., 0.02);


*/
