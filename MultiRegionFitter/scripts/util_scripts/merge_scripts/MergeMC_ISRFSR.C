{
  TChain *top = new TChain("nt","nt");
  top->Add("files/otop03*root"); // up
  top->Merge("top_IFSRup.root");
  top->Add("files/otop04*root"); // down
  top->Merge("top_IFSRdown.root");

  //top->Add("files/itktrk*root"); // up
  //top->Merge("top_ISRup.root");
  //top->Add("files/itktrl*root"); // down
  //top->Merge("top_ISRdown.root");
  //top->Add("files/ftktrj*root"); // up
  //top->Merge("top_FSRup.root");
  //top->Add("files/ftktrl*root"); // down
  //top->Merge("top_FSRdown.root");

  cout << "top is done" << endl;

  gApplication->Terminate();

}
