{
  TString variable = "m_ptl1" ;

  TCanvas * c0 = new TCanvas () ;
  TFile * inputf_0p3 = new TFile ("histograms_RcW_0p3_GP_50K.root", "read") ;
//  TFile * inputf_0p3 = new TFile ("histos_test.root", "read") ;

//  TH1F * h_cW_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_0p3") ;
  TH1F * h_cW_bsm_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_bsm_0p3") ;
  TH1F * h_cW_int_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_int_0p3") ;
  TH1F * h_SM_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_SMlimit") ;

  TFile * file0 = new TFile ("ntuple_SMlimit.root", "read") ;
//  TFile * file0 = new TFile ("ntuple_test.root", "read") ;
  file0->ls () ;
  TNtuple * nt_SM = (TNtuple *) file0->Get ("SSeu_SMlimit") ;  
  TH1F * h_SM_nt = (TH1F *) h_SM_0p3->Clone ("h_SM_nt") ;
  h_SM_nt->Reset () ;
  TH1F * h_SM_nums = (TH1F *) file0->Get ("SSeu_SMlimit_nums") ;  
  nt_SM->Draw ("ptl1>>h_SM_nt", "w") ;
  float scale = h_SM_nums->GetBinContent (1) / h_SM_nums->GetBinContent (2) ;
  h_SM_nt->Scale (scale) ;

  TCanvas * c1 = new TCanvas () ;
  h_SM_0p3->Draw ("hist") ;
  h_SM_nt->SetMarkerStyle (8) ;
  h_SM_nt->Draw ("same") ;

  c0->cd () ;

  TFile * file1 = new TFile ("ntuple_RcW_0p3.root", "read") ;
  file1->ls () ;
  TNtuple * nt_0p3_bsm = (TNtuple *) file1->Get ("SSeu_RcW_bsm_0p3") ;  
  TH1F * h_0p3_bsm_nt = (TH1F *) h_SM_0p3->Clone ("h_0p3_bsm_nt") ;
  h_0p3_bsm_nt->Reset () ;
  TH1F * h_0p3_bsm_nums = (TH1F *) file1->Get ("SSeu_RcW_bsm_0p3_nums") ;  
  nt_0p3_bsm->Draw ("ptl1>>h_0p3_bsm_nt", "w") ;
  scale = h_0p3_bsm_nums->GetBinContent (1) / h_0p3_bsm_nums->GetBinContent (2) ;
  h_0p3_bsm_nt->Scale (scale) ;

  TCanvas * c2 = new TCanvas () ;
  h_cW_bsm_0p3->Draw ("hist") ;
  h_0p3_bsm_nt->SetMarkerStyle (8) ;
  h_0p3_bsm_nt->Draw ("same") ;

  c0->cd () ;

  TFile * file2 = new TFile ("ntuple_RcW_0p3.root", "read") ;
  file2->ls () ;
  TNtuple * nt_0p3_int = (TNtuple *) file2->Get ("SSeu_RcW_int_0p3") ;  
  TH1F * h_0p3_int_nt = (TH1F *) h_SM_0p3->Clone ("h_0p3_int_nt") ;
  h_0p3_int_nt->Reset () ;
  TH1F * h_0p3_int_nums = (TH1F *) file2->Get ("SSeu_RcW_int_0p3_nums") ;  
  nt_0p3_int->Draw ("ptl1>>h_0p3_int_nt", "w") ;
  scale = h_0p3_int_nums->GetBinContent (1) / h_0p3_int_nums->GetBinContent (2) ;
  h_0p3_int_nt->Scale (scale) ;

  TCanvas * c3 = new TCanvas () ;
  h_cW_int_0p3->Draw ("hist") ;
  h_0p3_int_nt->SetMarkerStyle (8) ;
  h_0p3_int_nt->Draw ("same") ;
}  

