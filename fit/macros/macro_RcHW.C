{
  // TString variable = "m_mjj" ;
  // TString variable = "m_mll" ;
  // TString variable = "m_ptl1" ;
  TString variable = "m_ptl2" ;

  int color_SM  = kGreen - 8 ;
  int color_INT = kOrange - 4 ;
  int color_BSM = kAzure - 9 ;
  bool logscale = true ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


  TFile * inputf_0p3 = new TFile ("histograms_RcHW_0p3.root", "read") ;
  inputf_0p3->ls () ;

  TH1F * h_cHW_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_RcHW_0p3") ;
  TH1F * h_SM_0p3 = (TH1F*) inputf_0p3->Get (variable + "_SSeu_SMlimit") ;

  h_SM_0p3->SetFillColor (color_SM) ;

  TCanvas * c_0p3 = new TCanvas () ;
  if (logscale) c_0p3->SetLogy () ;
  h_cHW_0p3->GetXaxis ()->SetTitle (variable) ;
  h_cHW_0p3->SetLineWidth (4) ;
  h_cHW_0p3->SetLineColor (kBlack) ;
  h_cHW_0p3->SetStats (0) ;
  h_cHW_0p3->Draw ("") ;
  h_SM_0p3->Draw ("hist same") ;
  h_cHW_0p3->Draw ("same") ;

}