{
TCanvas *c1 = new TCanvas("c1", "Test",4,18,500,500);
//TBox *box1 = new TBox(0.5,0.5,0.6,0.6);
//box1->SetFillColor(19);
//box1->Draw();
TBox *box2 = new TBox(0.5,0.5,0.6,0.6);
box2->SetFillColor(kBlue);
box2->SetFillStyle(0);
box2->SetLineColor(kBlack);
box2->SetLineWidth(2);
box2->Draw();
}

