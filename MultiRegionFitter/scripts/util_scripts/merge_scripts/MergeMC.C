{
  TString type = "_JESp0.5"; // to append to filenames. Begin with _ if not null
  /*
  TChain *anti = new TChain("nt","nt");
  anti->Add("files/anti*root");
  anti->Merge("anti.root");

  cout << "anti is done" << endl;

  TChain *jetele = new TChain("nt","nt");
  jetele->Add("files/jetele-p0_section001.root");
  jetele->Add("files/jetele-p0_section002.root");
  jetele->Add("files/jetele-p0_section003.root");
  jetele->Add("files/jetele-p0_section004.root");
  jetele->Add("files/jetele-p0_section005.root");
  jetele->Add("files/jetele-p0_section006.root");
  jetele->Add("files/jetele-p0_section007.root");
  jetele->Add("files/jetele-p0_section008.root");
  jetele->Add("files/jetele-p0_section009.root");
  jetele->Add("files/jetele-p0_section010.root");
  jetele->Add("files/jetele-p1-4.root");
  jetele->Add("files/jetele-p5-9.root");
  jetele->Add("files/jetele-p10.root");
  jetele->Add("files/jetele-p11.root");
  jetele->Add("files/jetele-p12.root");
  jetele->Add("files/jetele-p13.root");
  jetele->Add("files/jetele-p14.root");
  jetele->Add("files/jetele-p15.root");
  jetele->Add("files/jetele-p16.root");
  jetele->Add("files/jetele-p17.root");

  cout << "Added all the jet electron files" << endl;
  jetele->Merge("anti_jete.root");

  cout << "jetele is done" << endl;
  */
  TChain *top = new TChain("nt","nt");
  top->Add("files/ttop*root");
  top->Merge("top" + type + ".root");

  cout << "top is done" << endl;
  /*
  TChain *toph = new TChain("nt","nt");
  toph->Add("files/otop1s*root");
  toph->Merge("top_Herwig.root");

  cout << "top Herwig is done" << endl;
  */
  TChain *wlf = new TChain("nt","nt");
  wlf->Add("files/utop*root");
  wlf->Add("files/ptop2w*root");
  wlf->Add("files/ptop3w*root");
  wlf->Add("files/ptop7w*root");
  wlf->Add("files/ptop8w*root");
  wlf->Add("files/ptop9w*root");
  wlf->Add("files/ptop4w_section001.root");
  wlf->Add("files/ptop4w_section002.root");
  wlf->Add("files/ptop4w_section003.root");
  wlf->Add("files/ptop4w_section004.root");
  wlf->Add("files/ptopw0*root");
  wlf->Add("files/ptopw1*root");
  wlf->Add("files/ptopw5*root");
  wlf->Add("files/ptopw6*root");
  wlf->Merge("wlf" + type + ".root");

  cout << "wlf is done" << endl;
 
  TChain *wbb = new TChain("nt","nt");
  wbb->Add("files/btop*root");
  wbb->Add("files/dtop*root");
  wbb->Merge("wbb" + type + ".root"); 
  
  cout << "wbb is done" << endl;
  
  TChain *wcc = new TChain("nt","nt");
  wcc->Add("files/ctop0w*root");
  wcc->Add("files/ctop1w*root");
  wcc->Add("files/ctop2w*root");
  wcc->Add("files/ctop5w*root");
  wcc->Add("files/ctop6w*root");
  wcc->Add("files/ctop7w_section001.root");
  wcc->Add("files/ctop7w_section002.root");
  wcc->Add("files/ctop7w_section003.root");
  wcc->Add("files/ctop7w_section004.root");
  wcc->Add("files/etop*root");
  wcc->Merge("wcc" + type + ".root");

 cout << "wcc is done" << endl;

  TChain *wc = new TChain("nt","nt");
  wc->Add("files/stopw*root");
  wc->Merge("wc" + type + ".root");

 cout << "wc is done" << endl;


  TChain *stop = new TChain("nt","nt");
  stop->Add("files/stop00*root");
  stop->Add("files/stopm0*root");
  stop->Merge("stop" + type + ".root");
  cout << "stop is done" << endl;


  TChain *vv = new TChain("nt","nt");
  vv->Add("files/*hht1a*root");
  vv->Merge("vv" + type + ".root");

  cout << "ww/wz/zz are done" << endl;

  TChain *zhf = new TChain("nt","nt");
  zhf->Add("files/ztopb*root");
  zhf->Add("files/ztopc*root");
  zhf->Add("files/ztktc7*root");
  zhf->Merge("zhf" + type + ".root");

  cout << "zhf is done" << endl;

  TChain *zlf = new TChain("nt","nt");
  zlf->Add("files/ztopp*root");
  zlf->Add("files/ztopz*root");
  zlf->Add("files/ztopt*root");
  zlf->Add("files/ztop*p*root");
  zlf->Merge("zlf" + type + ".root");
  
  cout << "zlf is done" << endl;
  

  gApplication->Terminate();

}
