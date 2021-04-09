{
  TChain *wlf = new TChain("nt","nt");
  // Q^2 = 0.5
  
  wlf->Add("files/otkt0e*.root");
  wlf->Add("files/otkt0g*root");
  wlf->Add("files/otkt0i*root");
  wlf->Add("files/otkt0k*root");
  wlf->Add("files/otkt0m*root");
  wlf->Add("files/otkt0o*root");
  wlf->Add("files/otkt0q*root");
  wlf->Add("files/otkt0s*root");
  wlf->Add("files/otkt0u*root");
  wlf->Add("files/otkt0w*root");
  wlf->Add("files/wtopq4*root");
  wlf->Add("files/wtopq3*root");
  wlf->Add("files/wtopq2*root");
  wlf->Add("files/wtopq1*root");
  wlf->Add("files/wtopq0*root");

  wlf->Merge("wlf_Q0.5.root");
  /*
  // Q^2 = 2.0
  wlf->Add("files/otop0f*root");
  wlf->Add("files/otkt0h*root");
  wlf->Add("files/otkt0j*root");
  wlf->Add("files/otkt0l*root");
  wlf->Add("files/otkt0n*root");
  wlf->Add("files/otkt0p*root");
  wlf->Add("files/otkt0r*root");
  wlf->Add("files/otkt0t*root");
  wlf->Add("files/otkt0v*root");
  wlf->Add("files/otkt0x*root");
  wlf->Add("files/wtopq9*root");
  wlf->Add("files/wtopq8*root");
  wlf->Add("files/wtopq7*root");
  wlf->Add("files/wtopq6*root");
  wlf->Add("files/wtopq5*root");
  
  wlf->Merge("wlf_Q2.0.root");
  */
  cout << "wlf is done" << endl;

  TChain *wbb = new TChain("nt","nt");
  
  // Q^2 = 0.5
  wbb->Add("files/qtopw0*root");
  wbb->Add("files/qtopw1*root");
  wbb->Add("files/qtopw2*root");
  wbb->Add("files/qtopw6*root");
  wbb->Add("files/qtopw7*root");
  wbb->Add("files/qtopw8*root");
  wbb->Add("files/qtopwc*root");
  wbb->Add("files/qtopwd*root");
  wbb->Add("files/qtopwe*root");
  
  wbb->Merge("wbb_Q0.5.root");
  /*
  
  // Q^2 = 2.0
  wbb->Add("files/qtopw3*root");
  wbb->Add("files/qtopw4*root");
  wbb->Add("files/qtopw5*root");
  wbb->Add("files/qtopw9*root");
  wbb->Add("files/qtopwa*root");
  wbb->Add("files/qtopwb*root");
  wbb->Add("files/qtopwf*root");
  wbb->Add("files/qtopwg*root");
  wbb->Add("files/qtopwh*root");
  
  
  wbb->Merge("wbb_Q2.0.root"); 
  */
  cout << "wbb is done" << endl;

  TChain *wcc = new TChain("nt","nt");
  
  // Q^2 = 0.5
  wcc->Add("files/qtopwi*root");
  wcc->Add("files/qtopwj*root");
  wcc->Add("files/qtopwk*root");
  wcc->Add("files/qtopwo*root");
  wcc->Add("files/qtopwp*root");
  wcc->Add("files/qtopwq*root");
  wcc->Add("files/qtopwu*root");
  wcc->Add("files/qtopwv*root");
  wcc->Add("files/qtopww*root");
  
  wcc->Merge("wcc_Q0.5.root");
  /*

  // Q^2 = 2.0
  wcc->Add("files/qtopwl*root");
  wcc->Add("files/qtopwm*root");
  wcc->Add("files/qtopwn*root");
  wcc->Add("files/qtopwr*root");
  wcc->Add("files/qtopws*root");
  wcc->Add("files/qtopwt*root");
  wcc->Add("files/qtopwx*root");
  wcc->Add("files/qtopwy*root");
  wcc->Add("files/qtopwz*root");
  
  wcc->Merge("wcc_Q2.0.root");
  */
  cout << "wcc is done" << endl;

  TChain *wc = new TChain("nt","nt");
  
  //Q^2 = 0.5
  wc->Add("files/ctopq0*root");
  wc->Add("files/ctopq1*root");
  wc->Add("files/ctopq2*root");
  wc->Add("files/ctopq3*root");
  wc->Add("files/ctopq8*root");
  wc->Add("files/ctopq9*root");
  wc->Add("files/ctopqa*root");
  wc->Add("files/ctopqb*root");
  wc->Add("files/ctopqg*root");
  wc->Add("files/ctopqh*root");
  wc->Add("files/ctopqi*root");
  wc->Add("files/ctopqj*root");
  
  wc->Merge("wc_Q0.5.root");
  /*

  // Q^2 = 2.0
  wc->Add("files/ctopq4*root");
  wc->Add("files/ctopq5*root");
  wc->Add("files/ctopq6*root");
  wc->Add("files/ctopq7*root");
  wc->Add("files/ctopqc*root");
  wc->Add("files/ctopqd*root");
  wc->Add("files/ctopqe*root");
  wc->Add("files/ctopqf*root");
  wc->Add("files/ctopqk*root");
  wc->Add("files/ctopql*root");
  wc->Add("files/ctopqm*root");
  wc->Add("files/ctopqn*root");
  
  wc->Merge("wc_Q2.0.root");
  */
  cout << "wc is done" << endl;


gApplication->Terminate();

}
