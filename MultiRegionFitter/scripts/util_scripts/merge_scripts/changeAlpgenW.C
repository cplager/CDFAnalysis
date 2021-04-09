{

  // Merge the ptopw0 sections first
  TChain* ptop = new TChain("nt", "nt");
  ptop ->Add("files/ptopw0*root");
  ptop ->Merge("temp.root");

  // Now open the merged file to change the AlpgenW
  TFile* oldf = new TFile("temp.root", "READ");
  TFile* newf = new TFile("ptopw0.root", "RECREATE");

  TTree* tree;
  oldf ->GetObject("nt", tree);
  if (tree != NULL) {
    TTree* newtree = tree ->CloneTree(0);
    newtree ->SetName("nt");
  } else {
    cout << "Tree not found!" << endl;
  }

  float alpw = 0;
  tree ->SetBranchAddress("evt.AlpgenW", &alpw);
  
  // loop over all entries in the tree, change the AlpgenW, and Fill the new tree
  for (int i = 0; i < tree ->GetEntriesFast(); i++) {
    tree->GetEntry(i);
    alpw = 0.3656;

    newtree ->Fill();
  } // loop over tree entries

  newtree ->Print();
  newtree ->AutoSave();

}
