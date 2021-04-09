#include <stdio.h> // for sprintf

void hcentDists()
{
   for (int loop = 51; loop <= 100; ++loop)
   {
      char filename[100], epsname[100], gifname[100];
      sprintf (filename, "fc_%d.root", loop);
      sprintf (epsname, "images/hcent_%03d.eps", loop);
      sprintf (gifname, "images/hcent_%03d.gif", loop);
      TFile *file = new TFile (filename);
      if (! file->IsOpen())
      {
         continue;
      }
      TH1F *hist = (TH1F*)file->Get("hcent");
      hist->Draw();
      c1->Print(epsname);
      //c1->Print(gifname);
      file->Close();
      //delete file;
      //delete hist;
   } // for loop
}
