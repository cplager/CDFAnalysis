{

  // Merge Data by Removing Duplicate Events
  // we use RunEventMet rather than RunEventSet 
  // to account for (int) -> (float) type re-definition

  gSystem->Load("/data/njhu05/a/mmathis/topCode/shlib/libRootFileCollection.so");
  RunEventMet mySet; // keeping track of duplicate events

  TChain *data = new TChain("nt","nt");
  data->Add("data-dupl.root");
  TFile fskim("data.root","recreate");

  float run, evt, met;
  TBranch *br_run, *br_evt, *br_met;
  
   data->SetBranchAddress("evt.Run",&run);
   data->SetBranchAddress("evt.Event",&evt);
   data->SetBranchAddress("evt.Met",&met);
   
   br_run = data->GetBranch("evt.Run");
   br_evt = data->GetBranch("evt.Event");
   br_met = data->GetBranch("evt.Met");
   
   Int_t nentries = Int_t(data->GetEntriesFast());

   cout << "total number of entries = " << nentries << endl;
 
   fskim->cd();
   TTree * tskim = 0;
   tskim = (TTree*)data->GetTree()->CloneTree(0);
   cout << "starting the duplicate removal" << endl;


   Int_t nbytes = 0, nb = 0;
   Int_t Nent = 0;

   for (Int_t jentry=0; jentry<nentries;jentry++) {

     nb = data->GetEntry(jentry);   nbytes += nb;
     //cout << "jentry = " << jentry << endl;
    
     // do rounding to avoid small met value fluctuations
     double newMet = met*100;
     int roundMet=  (int) newMet;
     if (newMet  - roundMet > 0.5 ) roundMet++;

     // filter some ntuple entries for the sample
     if (mySet->alreadySeen((int)run,(int)evt, (int)roundMet)){
       cout << " found a duplicate event: run# =  " << (int)run << "; event# = " << (int)evt << "; met = " << met << endl;
       continue;
     }

     // add to the list here
      Nent++;
      tskim->Fill();

   }//jentry

   cout << "found " << Nent << " non-duplicate entries " << endl;
   fskim->cd();
   tskim->Write();
   cout << "filled " << tskim->GetEntriesFast() << " events " << endl;
   fskim->Close();


}
