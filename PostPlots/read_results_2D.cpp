/*
c++ -o read_results_2D `root-config --glibs --cflags` ../utils/CfgParser.cc ../utils/dcutils.cc ../utils/sputils.cc -lm read_results_2D.cpp

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
#include "../utils/sputils.h"

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
//  double contours[1] = {2.30} ; // 1sigma, 95% CL
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

//          vector<pair <string, TH2F *> > contour_plots ;
          vector<pair <string, TList *> > contours_inner ;

          // loop over variables
          for (int iVar = 0 ; iVar < variables.size () ; ++iVar)
            {
              cout << "--> var " << variables.at (iVar) << endl ;

              // get the results from the root file
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              string localrootname = wilson_coeff_names.at (iCoeff1) 
                                     + "_" + wilson_coeff_names.at (iCoeff2)
                                     + "_" + variables.at (iVar) ;

              string filename = destination_folder 
                                + "/" + outfiles_prefix 
                                + "_" + localrootname
                                + "_fitresult.root" ;
              TFile f_in (filename.c_str ()) ;
              f_in.GetObject ("limit", limitTree) ;

              // extract info from TTree and dump it in a TGraph
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

              // hidden drawing to get the variables ready to build the TGraph later on
              //  --> the ordering here corresponds (i.e. imposes) the one in the TGraph2D fill

              // TH2 * h_scan = treeToHist2D (
              //     limitTree, 
              //     TString ("k_") + TString (wilson_coeff_names.at (iCoeff1).c_str ()),
              //     TString ("k_") + TString (wilson_coeff_names.at (iCoeff2).c_str ()), 
              //     "h_" + localrootname, 
              //     globalCut.c_str (), 
              //     -2, 2, -2, 2, 100, 100
              //   ) ;
              /* TODO FIXME
               - cerca nel tree il numero di punti totali, il range in x e in y, il binning in x e in y
               - se alcune di qs info non ci sono, capisci come ricavarle
               - la macro deve anche dire se il numero di punti e' adeguato
              */

              //h_scan->Draw ("COLZ") ;

              TGraph2D * g_scan = treeToGraph2D (
                  limitTree, 
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff1).c_str ()),
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff2).c_str ()), 
                  globalCut.c_str ()
                ) ;
              g_scan->Draw ("COLZ") ;

              string h_name = "h_" + localrootname + "_cont" ;
              TH2F * h_contour = (TH2F *) (g_scan->GetHistogram ())->Clone (h_name.c_str ()) ;

              // list of TGraph describing the contour
              TList * contour = contourFromTH2 (h_contour, contours[0]) ; 
              g_scan->Draw ("COLZ") ;
              TIter next (contour) ;
              while (TGraph * gr = (TGraph *) next ())
                {
                  gr->SetLineColor (kRed) ;
                  gr->SetLineWidth (2) ;
                  gr->Draw ("L same") ;
                }

              contours_inner.push_back (pair<string, TList *> (variables.at (iVar), contour)) ;

              // string outfile = string ("plot")
              //                  + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
              //                  + "_" + variables.at (iVar) + "_LLRscan.pdf" ;
              // c1.SaveAs (outfile.c_str ()) ;

            } // loop over variables

          TH1F * dummy_h = c1.DrawFrame (-2., -2., 2., 2.) ; // FIXME these have to end up in the cfg file
          dummy_h->GetXaxis ()->SetTitle (wilson_coeff_names.at (iCoeff1).c_str ()) ;
          dummy_h->GetYaxis ()->SetTitle (wilson_coeff_names.at (iCoeff2).c_str ()) ;

          TLegend legend (0.70, 0.50, 0.95, 0.95) ;

          for (int i = 0 ; i < contours_inner.size () ; ++i)
            {

              TIter next (contours_inner.at (i).second) ;
              bool first = true ;
              while (TGraph * gr = (TGraph *) next ())
                {
                  gr->SetLineColor (20 + 2*i) ;
                  gr->SetLineWidth (2) ;
                  gr->Draw ("L same") ;
                  if (first)
                    {
                      legend.AddEntry (gr, contours_inner.at (i).first.c_str (), "l") ;
                      first = false ;
                    }
                }
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