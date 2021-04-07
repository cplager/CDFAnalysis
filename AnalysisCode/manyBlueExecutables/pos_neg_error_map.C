{

gROOT->Reset();
//gSystem->Load("libRootFileCollection");
gROOT->SetStyle("Plain");
gROOT->ForceStyle();
gStyle->SetOptFit(11);


bool tr_shoot=false;


string base="/data/ncdf214/b/group/Higgs/SingleTop/SYS_error_map/COMBINE_2_14_08_Sys_0_map_sigma/";


float Vmax;
float Vmin;
float beta;

int icurrent=-1;
int intuple=0;


for ( int ifile_loop = 1; ifile_loop <= 20; ifile_loop++ ) {
//for ( int ifile_loop = 1; ifile_loop <= 3; ifile_loop++ ) {
  
  Vmax=(ifile_loop+1)*0.1;
  Vmin=(ifile_loop-1)*0.1;
  beta=ifile_loop*0.1;
  icurrent=-1;
  intuple=0;
  
  
  TChain *chain = new TChain("Fits","Fits");
  
  string file_str=Form("%s/singleTop_combine_%d_Tree.root",base.c_str(),ifile_loop);
  
  int N=chain->AddFile(file_str.c_str());
  


  if (tr_shoot) cout<<"N : "<< N<< "name : " << file_str<<endl;

  
  //  if(N==0){
  //  cout<<file_str<<" missing.."<<endl;
  //  continue;
  //}

  Int_t nevent = int(chain->GetEntries());
  if (tr_shoot) cout<<"nevent :" <<nevent<<endl;

  if(nevent!=20000){
    cout << "SKIPPING " << file_str<<endl;
    continue;
  }

    

  
  //Define the branch structure
  //TBranch* VALUE;
  TLeaf* me_value;
  TLeaf* lf_value;
  TLeaf* nn_value;


  TLeaf* me_errorUp;
  TLeaf* lf_errorUp;
  TLeaf* nn_errorUp;

  TLeaf* me_errorDn;
  TLeaf* lf_errorDn;
  TLeaf* nn_errorDn;



  TH1F *h_error_Uplf = new TH1F("h_errorUp_lf","h_errorUp_lf",100,0,.5);
  TH1F *h_error_Dnlf = new TH1F("h_errorDn_lf","h_errorDn_lf",100,-0.5,0.);

  TH1F *h_error_Upme = new TH1F("h_errorUp_me","h_errorUp_me",100,0,.5);
  TH1F *h_error_Dnme = new TH1F("h_errorDn_me","h_errorDn_me",100,-0.5,0);

  TH1F *h_error_Upnn = new TH1F("h_errorUp_nn","h_errorUp_nn",100,0,.5);
  TH1F *h_error_Dnnn = new TH1F("h_errorDn_nn","h_errorDn_nn",100,-0.5,0.);



  
  //Loop over entries
  for ( int iloop = 0; iloop < nevent; iloop++ ) {
    
    
    if(tr_shoot){
      if(iloop%5000 == 0) printf("Event:%d / %d\n",iloop,nevent);
    }

    chain->LoadTree(iloop);

    Int_t itree = chain->GetTreeNumber();

    //    cout<<endl<< "**** itree "<< itree << " icurrent :"<<icurrent<<endl<<endl;


   //Advance to next ntuple
    if (itree!=icurrent) {

       if(tr_shoot) cout<<"Setting icurrent=itree"<<endl;
       icurrent= itree;
       
       if(tr_shoot) cout<<endl<< "**** itree "<< itree << " icurrent :"<<icurrent<<endl<<endl;

       //VALUE         = chain->GetBranch("value");
       //me_value      = chain->GetLeaf("value.ME");
  
       lf_value = chain->FindLeaf("value.LF");
       me_value = chain->FindLeaf("value.ME");
       nn_value = chain->FindLeaf("value.NN");
       
       lf_errorDn = chain->FindLeaf("error.LF_DN");
       me_errorDn = chain->FindLeaf("error.ME_DN");
       nn_errorDn = chain->FindLeaf("error.NN_DN");
       
       
       lf_errorUp = chain->FindLeaf("error.LF_UP");
       me_errorUp = chain->FindLeaf("error.ME_UP");
       nn_errorUp = chain->FindLeaf("error.NN_UP");
       
       if(tr_shoot) printf("Ntuple number = %4d, Name=%s\n",intuple,chain->GetFile()->GetName());
       intuple++;
    }
    
    chain->GetEntry(iloop);
    
    if (tr_shoot){
    cout << "Fits.value.LF "<< lf_value->GetValue()<<endl;
    cout << "Fits.value.ME "<< me_value->GetValue()<<endl;
    cout << "Fits.value.NN "<< nn_value->GetValue()<<endl;
    cout << endl;

    cout << "Fits.errorUp.LF "<< lf_errorUp->GetValue()<<endl;
    cout << "Fits.errorUp.ME "<< me_errorUp->GetValue()<<endl;
    cout << "Fits.errorUp.NN "<< nn_errorUp->GetValue()<<endl;
    cout << endl;

    cout << "Fits.errorDn.LF "<< lf_errorDn->GetValue()<<endl;
    cout << "Fits.errorDn.ME "<< me_errorDn->GetValue()<<endl;
    cout << "Fits.errorDn.NN "<< nn_errorDn->GetValue()<<endl;
    cout << endl;

    }
    
    
    //    cout<<"max : "<<Vmax<<" min : "<<Vmin<<" value : "<< lf_value->GetValue() <<endl;

    if((Vmax>lf_value->GetValue())&&(Vmin<lf_value->GetValue())){
      h_error_Uplf->Fill(lf_errorUp->GetValue());
      h_error_Dnlf->Fill(lf_errorDn->GetValue());
    }


    if((Vmax>me_value->GetValue())&&(Vmin<me_value->GetValue())){
      h_error_Upme->Fill(me_errorUp->GetValue());
      h_error_Dnme->Fill(me_errorDn->GetValue());
    }

    if((Vmax>nn_value->GetValue())&&(Vmin<nn_value->GetValue())){
      h_error_Upnn->Fill(nn_errorUp->GetValue());
      h_error_Dnnn->Fill(nn_errorDn->GetValue());
    }
    


  }//End loop over events


  cout<< beta<< "   "
      <<   h_error_Dnlf->GetMean() << "   "<< h_error_Uplf->GetMean()<< "   "
      <<   h_error_Dnme->GetMean() << "   "<< h_error_Upme->GetMean()<< "   "
      <<   h_error_Dnnn->GetMean() << "   "<< h_error_Upnn->GetMean()<< "   "
      << endl;


  //  cout<<"Mean errorUp LF : "<< h_error_Uplf->GetMean()<<endl;
  //cout<<"Mean errorDn LF : "<< h_error_Dnlf->GetMean()<<endl;

  delete h_error_Uplf;
  delete h_error_Dnlf;
  delete h_error_Upme;
  delete h_error_Dnme;
  delete h_error_Upnn;
  delete h_error_Dnnn;
  delete chain;
  
}

}
