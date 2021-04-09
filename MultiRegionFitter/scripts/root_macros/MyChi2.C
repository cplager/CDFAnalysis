#include "TH1.h"
#include "TMath.h"
#include <iostream>

using namespace std;

const int Ntot = 5;

//double Chi2(const int Nm, const TObjArray *mc_scaled, const TObjArray *mc_original, 
//	    const TH1 *parent, const TH1 *test) {

  double Chi2(const int Nm, const TH1*parent, const TH1 *test) {

  if (parent->GetNbinsX() != test->GetNbinsX()){
    cout << "Chi2 ERROR: Different number of bins between parent and test distributions" << endl;
    return -9E20;
  }

  double logLd = 0.0;
  double logLf  = 0.0;
  for (int i = 1; i <= parent->GetNbinsX(); ++i) {
    
    double di = test->GetBinContent(i);
    double fi = parent->GetBinContent(i);
    if (fi !=0 ) logLd += di * TMath::Log(fi) - fi;
    if (di !=0 ) logLf += di * TMath::Log(di) - di;

    //  cout << "i = " << i << endl;
 //    for (int j=0; j< Nm; j++){
//       //   cout << "j = " << j << endl;
//       double aji = ((TH1*)mc_original->At(j))->GetBinContent(i);
//       double bji = ((TH1*)mc_scaled->At(j))->GetBinContent(i);
//       if (bji != 0) logLd += aji * TMath::Log(bji) - bji;
//       if (aji != 0) logLf += bji * TMath::Log(aji) - aji;
//     }
  }

  //Get the Chi2
  return -2*(logLd - logLf);

}
