{

 
  TChain *data = new TChain("nt","nt");
  data->Add("files/data-p5-7*root");
  data->Add("files/data-p8*root");
  data->Add("files/data-p9*root");
  data->Add("files/data-p10*root");
  data->Add("files/data-p11*root");
  data->Add("files/data-p12*root");
  data->Add("files/data-p13*root");
  data->Add("files/data-p14*root");
  data->Add("files/data-p15*root");
  data->Add("files/data-p16*root");
  data->Add("files/data-p17*root");
  data->Add("files/data-p1-4*root");
  data->Add("files/data-p0*root");
  data->Merge("data-dupl.root");
  

  //  data->Merge("data-p11.root");

  
  cout << "data is done" << endl;

 

gApplication->Terminate();

}
