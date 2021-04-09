{

  TObjArray *mc = new TObjArray(Nfit);       // MC histograms are put in this array

  for (int i=0; i < Nfit; i++)
    mc->Add(hmc[i]);

  TFractionFitter* fit = new TFractionFitter(hdata, mc); // initialise

  for (int i=0; i < Nfit; i++){  // apply constraints
    // parameters in the constrain functions seem to start with 1
    if (!constr[i])   fit->Constrain(i+1, 0.0, 1.0 );   
    else{
      double constrVal = hmc[i]->Integral() / hdata->Integral();
      double err = error[i]*constrVal;
      cout << "constrValue = " << constrVal << " +/- " << err << endl;
 //      double constrVal = constr_val[i];
//       double err = error[i];
//       double constrVal = constr_val[i] * hmc[i]->Integral() / hdata->Integral();
//       double err = error[i] * constrVal;
      fit->Constrain(i+1, constrVal - err, constrVal + err);
    }
  }//end: apply constraints


  Int_t status = fit->Fit();      
  cout << "fit status: " << status << endl;

  if (status != 0)    gApplication->Terminate();
   
   TH1F* result = (TH1F*) fit->GetPlot();
   hdata->SetMinimum(0.0);
   hdata->Draw("Ep");
   result->Draw("same");

   double fit_val[Nfit];
   double fit_err[Nfit];

   for (int i=0; i< Nfit; i++){
     fit->GetResult(i, fit_val[i], fit_err[i] );
     cout << hmc[i]->GetName() << ":     " << fit_val[i] << "  +/-  " << fit_err[i] << endl; 
   }
     
   cout << "Chi2/NDF = " << fit->GetChisquare() << " / " << fit->GetNDF() << " = " <<   fit->GetChisquare() / fit->GetNDF() << endl; 

 //   // draw fractions normalized to the fitted values

   float NormInteg = hdata->Integral();
   
   cout << "NormInteg = " << NormInteg << endl;
   for (int i=0; i<Nfit; i++){
     double Integ = hmc[i]->Integral(); 
     hmc[i]->Scale( NormInteg*fit_val[i] / Integ );
     hmc[i]->Draw("same");
     cout << hmc[i]->GetName() << " scale factor: " << NormInteg*fit_val[i] /  Integ   << " +/- " << NormInteg*fit_err[i] /  Integ  << endl;
     cout << "hmc[" << i << "] = " << hmc[i]->Integral() << endl; 
//      if (i !=0)
//        hmc[i]->Add(hmc[i-1]);
//      cout << "hmc[" << i << "] = " << hmc[i]->Integral() << endl; 
   }
	 
   for (int i=Nfit-1; i>=0; i--)
     hmc[i]->Draw("same");
   hdata->Draw("Epsame");

   float legSize = 1- Nfit*0.06;
   TLegend *leg = new TLegend(0.7,legSize,0.995,0.995); 
    for (int i=0; i<Nfit; i++)
       leg->AddEntry(hmc[i],hmc[i]->GetName(),"l");

  leg->SetBorderSize(1);
  leg->SetFillColor(10);
  leg->Draw();





}






