{
Double_t x[5] = {.2,.7,.6,.25,.2};
Double_t y[5] = {.5,.1,.9,.7,.5};
TPolyLine *pline = new TPolyLine(5,x,y);
pline->SetFillColor(2);
pline->SetFillStyle(3013); // page 151 of 4_08 Users Guide
pline->SetLineColor(2);
pline->SetLineWidth(0);
pline->Draw("f");
pline->Draw();
}
