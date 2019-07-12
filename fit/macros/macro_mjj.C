{
  TString variable = "m_mjj" ;
  int color_SM  = kGreen - 8 ;
  int color_INT = kOrange - 4 ;
  int color_BSM = kAzure - 9 ;
  bool logscale = true ;

  TFile * inputf_0p1 = new TFile ("histograms_RcW_0p1.root", "read") ;
  inputf_0p1->ls () ;

  TH1F * h_cW_0p1 = (TH1F*) inputf_0p1->Get (variable + "_SSeu_RcW_0p1") ;
  TH1F * h_cW_bsm_0p1 = (TH1F*) inputf_0p1->Get (variable + "_SSeu_RcW_bsm_0p1") ;
  TH1F * h_cW_int_0p1 = (TH1F*) inputf_0p1->Get (variable + "_SSeu_RcW_int_0p1") ;
  TH1F * h_SM_0p1 = (TH1F*) inputf_0p1->Get (variable + "_SSeu_SMlimit") ;

  TLegend * legend = new TLegend (0.8,0.7,0.9,0.9) ;
  legend->AddEntry (h_cW_0p1, "full", "pl") ;
  legend->AddEntry (h_SM_0p1, "SM", "f") ;
  legend->AddEntry (h_cW_int_0p1, "int", "f") ;
  legend->AddEntry (h_cW_bsm_0p1, "BSM", "f") ;

  h_SM_0p1->SetFillColor (color_SM) ;
  h_cW_int_0p1->SetFillColor (color_INT) ;
  h_cW_bsm_0p1->SetFillColor (color_BSM) ;

  THStack * st_cW_0p1 = new THStack ("st_cW_0p1", "rebuild of bsm") ;
  st_cW_0p1->Add (h_SM_0p1) ;
  st_cW_0p1->Add (h_cW_int_0p1) ;
  st_cW_0p1->Add (h_cW_bsm_0p1) ;

  TCanvas * c_0p1 = new TCanvas () ;
  h_cW_0p1->GetXaxis ()->SetTitle (variable) ;
  h_cW_0p1->SetLineWidth (4) ;
  h_cW_0p1->SetLineColor (kBlack) ;
  h_cW_0p1->SetStats (0) ;
  if (logscale) c_0p1->SetLogy () ;
  h_cW_0p1->Draw ("") ;
  st_cW_0p1->Draw ("hist same") ;
  h_cW_0p1->Draw ("same") ;
  legend->Draw () ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


 TFile * inputf_0p3 = new TFile ("histograms_RcW_0p3.root", "read") ;
  inputf_0p3->ls () ;

  TH1F * h_cW_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_0p3") ;
  TH1F * h_cW_bsm_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_bsm_0p3") ;
  TH1F * h_cW_int_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcW_int_0p3") ;
  TH1F * h_SM_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_SMlimit") ;

  h_SM_0p3->SetFillColor (color_SM) ;
  h_cW_int_0p3->SetFillColor (color_INT) ;
  h_cW_bsm_0p3->SetFillColor (color_BSM) ;

  THStack * st_cW_0p3 = new THStack ("st_cW_0p3", "rebuild of bsm") ;
  st_cW_0p3->Add (h_SM_0p3) ;
  st_cW_0p3->Add (h_cW_int_0p3) ;
  st_cW_0p3->Add (h_cW_bsm_0p3) ;

  TCanvas * c_0p3 = new TCanvas () ;
  if (logscale) c_0p3->SetLogy () ;
  h_cW_0p3->GetXaxis ()->SetTitle (variable) ;
  h_cW_0p3->SetLineWidth (4) ;
  h_cW_0p3->SetLineColor (kBlack) ;
  h_cW_0p3->SetStats (0) ;
  h_cW_0p3->Draw ("") ;
  st_cW_0p3->Draw ("hist same") ;
  h_cW_0p3->Draw ("same") ;
  legend->Draw () ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TFile * inputf_1 = new TFile ("histograms_RcW_1.root", "read") ;
  inputf_1->ls () ;

  TH1F * h_cW_1 = (TH1F*) inputf_1->Get (variable + "_SSeu_RcW_1") ;
  TH1F * h_cW_bsm_1 = (TH1F*) inputf_1->Get (variable + "_SSeu_RcW_bsm_1") ;
  TH1F * h_cW_int_1 = (TH1F*) inputf_1->Get (variable + "_SSeu_RcW_int_1") ;
  TH1F * h_SM_1 = (TH1F*) inputf_1->Get (variable + "_SSeu_SMlimit") ;

  h_SM_1->SetFillColor (color_SM) ;
  h_cW_int_1->SetFillColor (color_INT) ;
  h_cW_bsm_1->SetFillColor (color_BSM) ;

  THStack * st_cW_1 = new THStack ("st_cW_1", "rebuild of bsm") ;
  st_cW_1->Add (h_SM_1) ;
  st_cW_1->Add (h_cW_int_1) ;
  st_cW_1->Add (h_cW_bsm_1) ;

  TCanvas * c_1 = new TCanvas () ;
  if (logscale) c_1->SetLogy () ;
  h_cW_1->GetXaxis ()->SetTitle (variable) ;
  h_cW_1->SetLineWidth (4) ;
  h_cW_1->SetLineColor (kBlack) ;
  h_cW_1->SetStats (0) ;
  h_cW_1->Draw ("") ;
  st_cW_1->Draw ("hist same") ;
  h_cW_1->Draw ("same") ;
  legend->Draw () ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TFile * inputf_0p05 = new TFile ("histograms_RcW_0p05.root", "read") ;
  inputf_0p05->ls () ;

  TH1F * h_cW_0p05 = (TH1F*) inputf_0p05->Get (variable + "_SSeu_RcW_0p05") ;
  TH1F * h_cW_bsm_0p05 = (TH1F*) inputf_0p05->Get (variable + "_SSeu_RcW_bsm_0p05") ;
  TH1F * h_cW_int_0p05 = (TH1F*) inputf_0p05->Get (variable + "_SSeu_RcW_int_0p05") ;
  TH1F * h_SM_0p05 = (TH1F*) inputf_0p05->Get (variable + "_SSeu_SMlimit") ;

  h_SM_0p05->SetFillColor (color_SM) ;
  h_cW_int_0p05->SetFillColor (color_INT) ;
  h_cW_bsm_0p05->SetFillColor (color_BSM) ;

  THStack * st_cW_0p05 = new THStack ("st_cW_0p05", "rebuild of bsm") ;
  st_cW_0p05->Add (h_SM_0p05) ;
  st_cW_0p05->Add (h_cW_int_0p05) ;
  st_cW_0p05->Add (h_cW_bsm_0p05) ;

  TCanvas * c_0p05 = new TCanvas () ;
  if (logscale) c_0p05->SetLogy () ;
  h_cW_0p05->GetXaxis ()->SetTitle (variable) ;
  h_cW_0p05->SetLineWidth (4) ;
  h_cW_0p05->SetLineColor (kBlack) ;
  h_cW_0p05->SetStats (0) ;
  h_cW_0p05->Draw ("") ;
  st_cW_0p05->Draw ("hist same") ;
  h_cW_0p05->Draw ("same") ;
  legend->Draw () ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TCanvas * c_int = new TCanvas () ;
//  c_int->DrawFrame (200, 0.0000001, 1800, 0.003) ;

  h_cW_int_1->GetXaxis ()->SetTitle (variable) ;
  h_cW_int_1->SetLineWidth (4) ;
  h_cW_int_1->SetLineColor (kBlack) ;
  h_cW_int_1->SetStats (0) ;
  h_cW_int_1->Draw ("hist") ;
 
  TH1F * h_cW_int_0p3_scale = (TH1F *) h_cW_int_0p3->Clone ("h_cW_int_0p3_scale") ;
  h_cW_int_0p3_scale->Scale (3.33) ;
  h_cW_int_0p3_scale->SetLineWidth (4) ;
  h_cW_int_0p3_scale->SetLineColor (kBlack) ;
  h_cW_int_0p3_scale->SetFillColor (0) ;
  h_cW_int_0p3_scale->SetLineStyle (3) ;
  h_cW_int_0p3_scale->Draw ("same") ;

  TH1F * h_cW_int_0p1_scale = (TH1F *) h_cW_int_0p1->Clone ("h_cW_int_0p1_scale") ;
  h_cW_int_0p1_scale->Scale (10.) ;
  h_cW_int_0p1_scale->SetLineWidth (4) ;
  h_cW_int_0p1_scale->SetLineColor (kBlack) ;
  h_cW_int_0p1_scale->SetFillColor (0) ;
  h_cW_int_0p1_scale->SetLineStyle (2) ;
  h_cW_int_0p1_scale->Draw ("same") ;

  TH1F * h_cW_int_0p05_scale = (TH1F *) h_cW_int_0p05->Clone ("h_cW_int_0p05_scale") ;
  h_cW_int_0p05_scale->Scale (20.) ;
  h_cW_int_0p05_scale->SetLineWidth (4) ;
  h_cW_int_0p05_scale->SetLineColor (kBlack) ;
  h_cW_int_0p05_scale->SetFillColor (0) ;
  h_cW_int_0p05_scale->SetLineStyle (4) ;
  h_cW_int_0p05_scale->Draw ("same") ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TCanvas * c_bsm = new TCanvas () ;
//  c_bsm->DrawFrame (200, 0.0000001, 1800, 0.003) ;

  h_cW_bsm_1->GetXaxis ()->SetTitle (variable) ;
  h_cW_bsm_1->SetLineWidth (4) ;
  h_cW_bsm_1->SetLineColor (kBlack) ;
  h_cW_bsm_1->SetStats (0) ;
  h_cW_bsm_1->Draw ("hist") ;
 
  TH1F * h_cW_bsm_0p3_scale = (TH1F *) h_cW_bsm_0p3->Clone ("h_cW_bsm_0p3_scale") ;
  h_cW_bsm_0p3_scale->Scale (11.11) ;
  h_cW_bsm_0p3_scale->SetLineWidth (4) ;
  h_cW_bsm_0p3_scale->SetLineColor (kBlack) ;
  h_cW_bsm_0p3_scale->SetFillColor (0) ;
  h_cW_bsm_0p3_scale->SetLineStyle (3) ;
  h_cW_bsm_0p3_scale->Draw ("same") ;

  TH1F * h_cW_bsm_0p1_scale = (TH1F *) h_cW_bsm_0p1->Clone ("h_cW_bsm_0p1_scale") ;
  h_cW_bsm_0p1_scale->Scale (100.) ;
  h_cW_bsm_0p1_scale->SetLineWidth (4) ;
  h_cW_bsm_0p1_scale->SetLineColor (kBlack) ;
  h_cW_bsm_0p1_scale->SetFillColor (0) ;
  h_cW_bsm_0p1_scale->SetLineStyle (2) ;
  h_cW_bsm_0p1_scale->Draw ("same") ;

  TH1F * h_cW_bsm_0p05_scale = (TH1F *) h_cW_bsm_0p05->Clone ("h_cW_bsm_0p05_scale") ;
  h_cW_bsm_0p05_scale->Scale (400.) ;
  h_cW_bsm_0p05_scale->SetLineWidth (4) ;
  h_cW_bsm_0p05_scale->SetLineColor (kBlack) ;
  h_cW_bsm_0p05_scale->SetFillColor (0) ;
  h_cW_bsm_0p05_scale->SetLineStyle (4) ;
  h_cW_bsm_0p05_scale->Draw ("same") ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TCanvas * c_compare = new TCanvas () ;
  h_cW_int_1->DrawNormalized ("hist") ;
  h_cW_bsm_1->DrawNormalized ("same") ;




}