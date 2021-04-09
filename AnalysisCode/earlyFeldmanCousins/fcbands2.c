// -*- C++ -*-
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TPolyLine.h"
#include "StringUtils.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h> // for sprintf
#include <vector>
#include <assert.h>
#include "TLine.h"
#include "CLPTypeDef.h"
#include "TBox.h"
#include "TText.h"
#include "TGaxis.h"

using namespace std;


// constants
const double kB0min = 0.0;
const double kB0max = 1.0;
const double kBmeasMin = 0;
const double kBmeasMax = 2.0;
const int kNumSteps = 60;
const double kStepSize = (kB0max - kB0min) / kNumSteps;

Double_t bBestArray[4] = {-2.13666e-02, 1.04467e+00, 
                          -4.46550e-02, 2.92435e-02};
DVec bBestvec(bBestArray, bBestArray + 4);

const int k68color = 5;
const int k90color = 3;
const int k95color = 2;

//const int k68fill = 3004;
//const int k90fill = 3005;
//const int k95fill = 3006;

const int k68fill = 1001;
const int k90fill = 1001;
const int k95fill = 1001;


void loadpoints(const char filename[], DVec &xVec, DVec &yVec);
Double_t bBestFunc (Double_t *x, Double_t *par);
Double_t polynomial (Double_t x, const DVec &params);

Double_t bBestFunc (Double_t *x, Double_t *par)
{
   double retval = polynomial (*x, bBestvec);
   return retval;
}


void loadpoints(const char filename[], DVec &xVec, DVec &yVec)
{
   
   ifstream source(filename, ios::in);
   if (! source)
   {
      cerr << "file " << filename << " could not be opened" << endl;
      assert(0);
   }
   cerr << "Loading file " << filename << endl;
   string line;
   while (getline (source, line))
   {
      SVec sList;
      DVec values;
      split (sList, line);
      int size = sList.size();
      if (size < 2)
      {
         continue;
      }
      double y = atof( sList[0].c_str() ) / 100;
      double x = atof( sList[1].c_str() );
      xVec.push_back (x);
      yVec.push_back (y);
   } // while getline
}

void fcbands2()
{
   TCanvas *c1 = new TCanvas;
   TH2F *h2 = new TH2F ("h2", "Feldman Cousins Bands for R", 
                        2, kBmeasMin, kBmeasMax, 2, kB0min, kB0max);
   h2->GetXaxis()->SetTitle("R Measured");
   h2->GetYaxis()->SetTitle("R Generated");
   h2->Draw();
   DVec x95, y95, x68, y68, x90, y90;
   loadpoints ("fc95.out", x95, y95);
   unsigned int size = x95.size();
   double *x95_array = new double[size + 1];
   double *y95_array = new double[size + 1];
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      x95_array[loop] = x95[loop];
      y95_array[loop] = y95[loop];
   }
   x95_array[size] = x95_array[0];
   y95_array[size] = y95_array[0];
   TPolyLine *fc95Line = new TPolyLine(size + 1, x95_array, y95_array);
   fc95Line->SetFillColor(k95color);
   fc95Line->SetFillStyle(k95fill); // page 151 of 4_08 Users Guide
   fc95Line->SetLineColor(k95color);
   fc95Line->SetLineWidth(0);
   fc95Line->Draw("f");
   fc95Line->Draw();
   
   loadpoints ("fc90.out", x90, y90);
   size = x90.size();
   double *x90_array = new double[size + 1];
   double *y90_array = new double[size + 1];
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      x90_array[loop] = x90[loop];
      y90_array[loop] = y90[loop];
   }
   x90_array[size] = x90_array[0];
   y90_array[size] = y90_array[0];
   TPolyLine *fc90Line = new TPolyLine(size + 1, x90_array, y90_array);
   fc90Line->SetFillColor(k90color);
   fc90Line->SetFillStyle(k90fill); // page 151 of 4_08 Users Guide
   fc90Line->SetLineColor(k90color);
   fc90Line->SetLineWidth(0);
   fc90Line->Draw("f");
   fc90Line->Draw();
   
   loadpoints ("fc68.out", x68, y68);
   size = x68.size();
   double *x68_array = new double[size + 1];
   double *y68_array = new double[size + 1];
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      x68_array[loop] = x68[loop];
      y68_array[loop] = y68[loop];
   }
   x68_array[size] = x68_array[0];
   y68_array[size] = y68_array[0];
   TPolyLine *fc68Line = new TPolyLine(size + 1, x68_array, y68_array);
   fc68Line->SetFillColor(k68color);
   fc68Line->SetFillStyle(k68fill); // page 151 of 4_08 Users Guide
   fc68Line->SetLineColor(k68color);
   fc68Line->SetLineWidth(0);
   fc68Line->Draw("f");
   fc68Line->Draw();
   
   TF1 *bBest = new TF1 ("bBest", bBestFunc, 0, 1.02, 0);
   bBest->SetLineColor(kBlue);
   bBest->SetLineWidth(1);
   bBest->SetLineStyle(2);
   bBest->Draw("same");
   h2->Draw("same");

   double yup = 0.03;
   

   // border
   TBox *border = new TBox(1.25, 0.03 + yup, 1.92, 0.6 + yup);
   border->SetFillColor(kBlue);
   border->SetFillStyle(0);
   border->SetLineColor(kBlack);
   border->SetLineWidth(1);
   border->Draw();

   // 95%
   TBox *box95 = new TBox(1.3, 0.46 + yup, 1.4, 0.56 + yup);
   box95->SetFillColor(k95color);
   box95->SetFillStyle(k95fill);
   box95->Draw();
   
   TText cap95 (1.55, 0.50 + yup, "95% C.L.");
   cap95.SetTextSize(0.035);
   cap95.Draw();

   // 90%
   TBox *box90 = new TBox(1.3, 0.31 + yup, 1.4, 0.41 + yup);
   box90->SetFillColor(k90color);
   box90->SetFillStyle(k90fill);
   box90->Draw();
   
   TText cap90 (1.55, 0.35 + yup, "90% C.L.");
   cap90.SetTextSize(0.035);
   cap90.Draw();

   // 68%
   TBox *box68 = new TBox(1.3, 0.16 + yup, 1.4, 0.26 + yup);
   box68->SetFillColor(k68color);
   box68->SetFillStyle(k68fill);
   box68->Draw();
   
   TText cap68 (1.55, 0.21 + yup, "68.28% C.L.");
   cap68.SetTextSize(0.035);
   cap68.Draw();



   TLine *line = new TLine (1.3, 0.11 + yup, 1.4, 0.11 + yup);

   line->SetLineColor(kBlue);
   line->SetLineWidth(1);
   line->SetLineStyle(2);
   line->Draw("same");
   TText capLine1 (1.55, 0.11 + yup, "Most Likely");
   TText capLine2 (1.55, 0.06 + yup, "Value");
   capLine1.SetTextSize (0.03);
   capLine2.SetTextSize (0.03);
   capLine1.Draw();
   capLine2.Draw();

   TGaxis *gaxis = new TGaxis(0, 1, 2, 1, 0, 2, 505, "U-");
   gaxis->Draw();

   
   c1->Print("FC.eps");
   if (1)
   {
      TLine line1(1.124, 0, 1.124, 1);
      line1.SetLineWidth(2);
      line1.Draw();
      TLine line2(1.124, 0.61, 0, 0.61);
      line2.SetLineWidth(2);
      line2.Draw();
      c1->Print("FC2.eps");
   } else {
      TLine line1(0.50, 0, 0.50, 1);
      line1.SetLineWidth(2);
      line1.Draw();
      TLine line2(0.50, 0.495, 0, 0.495);
      line2.SetLineWidth(2);
      line2.Draw();
      TLine line3(0.50, 0.355, 0, 0.355);
      line3.SetLineWidth(2);
      line3.Draw();
      TLine line4(0.50, 0.67, 0, 0.67);
      line4.SetLineWidth(2);
      line4.Draw();
      c1->Print("FC3.eps");
   }
}

Double_t polynomial (Double_t x, const DVec &params)
{
   Double_t xToTheN = 1.;
   Double_t total = 0.;
   unsigned int size = params.size();
   for (unsigned int loop = 0; loop < size; ++loop)
   {
      total += xToTheN * params[loop];
      xToTheN *= x;
   }
   return total;
}
