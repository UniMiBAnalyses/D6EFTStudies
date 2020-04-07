/*
c++ -o read_results_2D `root-config --glibs --cflags` ../utils/CfgParser.cc ../utils/dcutils.cc -lm read_results_2D.cpp

read the results obtained by combine running on the various workspaces created
when running on the outcome datacard_creator_2 with the same input cfg file
as the one used for this prgram

*/

#include <vector>
#include <string>
#include <map>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TGraph2D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"


#include "../utils/CfgParser.h"
#include "../utils/dcutils.h"

using namespace std ;

int main (int argc, char ** argv) 
{

  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }


  // read input informantion
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;
  sort (wilson_coeff_names.begin (), wilson_coeff_names.end ()) ;

  string outfiles_prefix           = gConfigParser->readStringOpt ("output::outfiles_prefix") ;
  string destination_folder_prefix = gConfigParser->readStringOpt ("output::destination_folder_prefix") ;

  vector<string> variables = gConfigParser->readStringListOpt ("general::variables") ;
  float luminosity         = gConfigParser->readFloatOpt ("general::luminosity") ;

  // alternative location of result files with respect to the cfg file
  if (argc > 2)
    {
      vector <string> percorso = split (destination_folder_prefix, '/') ;
      destination_folder_prefix = argv[2] + string ("/") + percorso.back () ;
    }

  // prepare the graphical options and the canvas for printing
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

  setTDRStyle () ;

  const Int_t __NRGBs = 2 ;
  const Int_t __NCont = 255 ;
  Double_t __stops[__NRGBs] = { 0.00, 1.00 } ;
  Double_t __red[__NRGBs]   = { 1.00, 0.00 } ;
  Double_t __green[__NRGBs] = { 1.00, 0.00 } ;
  Double_t __blue[__NRGBs]  = { 1.00, 1.00 } ;
  
  TColor::CreateGradientColorTable (__NRGBs, __stops, __red, __green, __blue, __NCont) ;
  gStyle->SetNumberContours (__NCont) ;
  gStyle->SetOptStat (0) ;

  TCanvas c1 ("c1", "", 300, 300) ;

  TLatex * tex1 = new TLatex (0.94, 0.92, "13 TeV") ;
  tex1->SetNDC () ;
  tex1->SetTextAlign (31) ;
  tex1->SetTextFont (42) ;
  tex1->SetTextSize (0.04) ;
  tex1->SetLineWidth (2) ;

  TLatex * tex2 = new TLatex (0.46, 0.92, ("L = " + to_string (int (luminosity)) + " fb^{-1}").c_str ()) ;
  tex2->SetNDC () ;
  tex2->SetTextFont (52) ;
  tex2->SetTextSize (0.035) ;
  tex2->SetLineWidth (2) ;

  // prepare variables useful outside of the loops
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

  limits_op_v all_limits ; // WAS IST DAS? un vettore di pair di vettori o cose simili

  // possible additional selections, for now not used, left as an option
  string globalCut = "1" ;

  TTree * limitTree ; // this will contain each tree during the loop

  //---- 2D likelihood thresholds
  // http://pdg.lbl.gov/2018/reviews/rpp2018-rev-statistics.pdf
  double contours[2] = {2.30, 5.99} ; // 1sigma, 95% CL
//  float contours[2] = {2.30, 6.18} ; // 1sigma, 2sigma

  // loop over first coefficient
  for (int iCoeff1 = 0 ; iCoeff1 < wilson_coeff_names.size () ; ++iCoeff1)
    {
      // loop over second coefficient
      for (int iCoeff2 = iCoeff1 + 1 ; iCoeff2 < wilson_coeff_names.size () ; ++iCoeff2)
        {

          cout << "--> coeffs " << wilson_coeff_names.at (iCoeff1) 
               << " vs. " << wilson_coeff_names.at (iCoeff2) << endl ;
          cout << "---- ---- ---- ---- ---- ---- ---- ---- ---- " << endl ;

          string destination_folder = destination_folder_prefix 
                                      + "_" + wilson_coeff_names.at (iCoeff1) 
                                      + "_" + wilson_coeff_names.at (iCoeff2) ;

          vector<pair <string, TH2F *> > contour_plots ;
          vector<pair <string, TGraph *> > contour_inner ;
          vector<pair <string, TGraph *> > contour_outer ;

          // loop over variables
          for (int iVar = 0 ; iVar < variables.size () ; ++iVar)
            {
              cout << "--> var " << variables.at (iVar) << endl ;

              // get the results from the root file
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              string filename = destination_folder 
                                + "/" + outfiles_prefix 
                                + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                                + "_" + variables.at (iVar) + "_fitresult.root" ;
              TFile f_in (filename.c_str ()) ;
              f_in.GetObject ("limit", limitTree) ;

              // extract info from TTree and dump it in a TGraph
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              // hidden drawing to get the variables ready to build the TGraph later on
              //  --> the ordering here corresponds (i.e. imposes) the one in the TGraph2D fill
              TString whatToDraw = Form ("%s:%s:2*deltaNLL", 
                                         ("k_" + wilson_coeff_names.at (iCoeff1)).c_str (), 
                                         ("k_" + wilson_coeff_names.at (iCoeff2)).c_str ()) ;
              TString cutToDraw = Form ("(deltaNLL<10) && (%s)", globalCut.c_str ()) ;
              int n = limitTree->Draw (whatToDraw.Data (), cutToDraw.Data (), "colz") ;
              TGraph2D * graphScan = new TGraph2D (n, limitTree->GetV1 (), limitTree->GetV2 (), limitTree->GetV3 ()) ;
              graphScan->GetHistogram ()->GetXaxis ()->SetTitle (wilson_coeff_names.at (iCoeff1).c_str ()) ;
              graphScan->GetHistogram ()->GetYaxis ()->SetTitle (wilson_coeff_names.at (iCoeff2).c_str ()) ;
              graphScan->GetHistogram ()->GetZaxis ()->SetTitle ("- 2#Delta logL") ;
              graphScan->SetTitle ("") ;
              graphScan->SetMarkerStyle (21) ;
              graphScan->SetMarkerColor (kRed) ;
              graphScan->SetLineColor (kRed) ;
              
//              graphScan->Draw ("colz") ;

              // prepare an histogram to draw level curves for the confidence regions
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              string h_name = "h_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                               + "_" + variables.at (iVar) ;
              TH2F * h_contour = (TH2F *) (graphScan->GetHistogram ())->Clone (h_name.c_str ()) ;
              contour_plots.push_back (pair<string, TH2F *> (variables.at (iVar), h_contour)) ;
              h_contour->SetContour (2, contours) ;
              h_contour->SetLineWidth (2) ;
              h_contour->SetLineStyle (1) ;
              h_contour->GetZaxis ()->SetRangeUser (0, 9.99) ;  
              h_contour->Draw ("CONT LIST") ; // the option LIST should save the contours in specials
              c1.Update () ;
              gPad->Update () ;
  
              // get the contours
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              // cout << "conts = " << h_contour->GetContour () << endl ;       // DEBUG
              // cout << "cont0 = " << h_contour->GetContourLevel (0) << endl ; // DEBUG
              // cout << "cont1 = " << h_contour->GetContourLevel (1) << endl ; // DEBUG
              TObjArray * conts = (TObjArray *) gROOT->GetListOfSpecials ()->FindObject ("contours") ;
              cout << " conts = " << conts << endl ;
              cout << " conts->GetSize() = " << conts->GetSize () << endl ;
              TGraph * gr_1sigma = (TGraph *) ( ((TList *) conts->At (0))->First ()) ;
              TGraph * gr_2sigma = (TGraph *) ( ((TList *) conts->At (1))->First ()) ;

              contour_inner.push_back (pair<string, TGraph *> (variables.at (iVar), (TGraph *) gr_1sigma->Clone ())) ;
              contour_outer.push_back (pair<string, TGraph *> (variables.at (iVar), (TGraph *) gr_2sigma->Clone ())) ;
              gr_1sigma->SetLineColor (kRed) ;
              gr_1sigma->Draw ("L SAME") ;
              c1.Update () ;

              // save the plots as pdfs
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              string outfile = string ("plot")
                               + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                               + "_" + variables.at (iVar) + "_LLRscan.pdf" ;
              c1.SaveAs (outfile.c_str ()) ;

            } // loop over variables

          TH1F * dummy_h = c1.DrawFrame (-2., -2., 2., 2.) ; // FIXME these have to end up in the cfg file
          dummy_h->GetXaxis ()->SetTitle (wilson_coeff_names.at (iCoeff1).c_str ()) ;
          dummy_h->GetYaxis ()->SetTitle (wilson_coeff_names.at (iCoeff2).c_str ()) ;

          TLegend legend (0.70, 0.80, 0.90, 0.90) ;

          for (int i = 0 ; i < contour_inner.size () ; ++i)
            {

              contour_inner.at (i).second->SetLineColor (kRed + 2*i) ;
              contour_inner.at (i).second->Draw ("L SAME") ;
              legend.AddEntry (contour_inner.at (i).second,
                               contour_inner.at (i).first.c_str (),
                               "l") ;
            }

          legend.SetBorderSize (0) ;
          legend.SetFillStyle (0) ;
          legend.Draw () ;
          string outfile = string ("plot")
                           + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                           + "_compareScan.pdf" ;
          c1.SaveAs (outfile.c_str ()) ;


        } // loop over second Wilson coefficient
    } // second loop over first Wilson coefficient

// FINO A QUI


  return 0 ; 


}