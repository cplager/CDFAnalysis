// -*- C++ -*-
#include "TSystem.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TPolyLine.h"
#include "TGraphErrors.h"
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

using namespace std;


Double_t bBestArray[4] = {-2.13666e-02, 1.04467e+00, 
                          -4.46550e-02, 2.92435e-02};
DVec bBestvec(bBestArray, bBestArray + 4);



double b0min = 0.0;
double b0max = 1.0;
double bMeasMin = 0;
double bMeasMax = 2.5;
string filename68 = "fc68.txt";
string filename95 = "fc95.txt";
double measured = 1.11;
double limit  = 0.705;
string prefix = "FC";

void loadpoints(const char filename[], DVec &xVec, DVec &yVec);
Double_t bBestFunc (Double_t *x, Double_t *par);
Double_t polynomial (Double_t x, const DVec &params);
void loadControlFile(const char filename[]);

Double_t bBestFunc (Double_t *x, Double_t *par)
{
   double retval = polynomial (*x, bBestvec);
   return retval;
}

void loadControlFile(const char filename[])
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
      split (sList, line);
      unsigned int size = sList.size();
      if (size < 2)
      {
         continue;
      }
      if (string::npos != sList[0].find("b0min"))
      {
         b0min = atof( sList[1].c_str() );
         continue;
      } // if b0min
      if (string::npos != sList[0].find("b0max"))
      {
         b0max = atof( sList[1].c_str() );
         continue;
      } // if b0max
      if (string::npos != sList[0].find("bMeasMin"))
      {
         bMeasMin = atof( sList[1].c_str() );
         continue;
      } // if bMeasMin
      if (string::npos != sList[0].find("bMeasMax"))
      {
         bMeasMax = atof( sList[1].c_str() );
         continue;
      } // if bMeasMax
      if (string::npos != sList[0].find("68"))
      {
         filename68 = sList[1];
         continue;
      } // if 68
      if (string::npos != sList[0].find("95"))
      {
         filename95 = sList[1];
         continue;
      } // if 95
      if (string::npos != sList[0].find("prefix"))
      {
         prefix = sList[1];
         continue;
      } // if prefix
      if (string::npos != sList[0].find("measured"))
      {
         measured = atof( sList[1].c_str() );
         continue;
      } // if measured
      if (string::npos != sList[0].find("limit"))
      {
         limit = atof( sList[1].c_str() );
         continue;
      } // if limit
      if (string::npos != sList[0].find("best_measured"))
      {
         bBestvec.clear();
         for (unsigned int loop = 1; loop < size; ++loop)
         {
            bBestvec.push_back( atof( sList[loop].c_str() ) );
         } // for loop
         continue;
      } // if bMeasMax
      
   } // while getline
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

void fcbands(const char filename[])
{
   loadControlFile (filename);
   TCanvas *c1 = new TCanvas;
   TH2F *h2 = new TH2F ("h2", "Feldman Cousins Bands for R", 
                        2, bMeasMin, bMeasMax, 2, b0min, b0max);
   h2->Draw();
   DVec x95, y95, x68, y68;
   loadpoints (filename95.c_str(), x95, y95);
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
   fc95Line->SetFillColor(2);
   fc95Line->SetFillStyle(1001); // page 151 of 4_08 Users Guide
   fc95Line->SetLineColor(2);
   fc95Line->SetLineWidth(0);
   fc95Line->Draw("f");
   fc95Line->Draw();
   
   loadpoints (filename68.c_str(), x68, y68);
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
   fc68Line->SetFillColor(3);
   fc68Line->SetFillStyle(1001); // page 151 of 4_08 Users Guide
   fc68Line->SetLineColor(2);
   fc68Line->SetLineWidth(0);
   fc68Line->Draw("f");
   fc68Line->Draw();
   
   TF1 *bBest = new TF1 ("bBest", bBestFunc, 0, 1.02, 0);
   bBest->SetLineColor(kBlue);
   bBest->SetLineWidth(1);
   bBest->Draw("same");
   TLegend *legend = new TLegend (0.5, 0.2, 0.8, 0.5);
   legend->AddEntry(fc68Line, "68% Confidence", "F");
   legend->AddEntry(fc95Line, "95% Confidence", "F");
   legend->AddEntry(bBest, "Most Likely Value");
   legend->SetFillStyle(4000);
   legend->Draw();
   h2->Draw("same");
   char name[100];
   sprintf (name, "%s.eps", prefix.c_str());
   c1->Print(name);
   if (1)
   {
      TLine line1(measured, 0, measured, 1);
      line1.SetLineWidth(2);
      line1.Draw();
      TLine line2(measured, limit, 0, limit);
      line2.SetLineWidth(2);
      line2.Draw();
      sprintf (name, "%s2.eps", prefix.c_str());
      c1->Print(name);
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
      sprintf (name, "%s3.eps", prefix.c_str());
      c1->Print(name);
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
