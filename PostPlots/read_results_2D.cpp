/*
c++ -o read_results_2D `root-config --glibs --cflags` ../utils/CfgParser.cc ../utils/dcutils.cc ../utils/sputils.cc -lm read_results_2D.cpp

read the results obtained by combine running on the various workspaces created
when running on the outcome datacard_creator_2 with the same input cfg file
as the one used for this prgram

*/

#include <vector>
#include <string>
#include <map>
#include <fstream>

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
#include "TMarker.h"


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

  // read the 4 collections, since joint sort works with the 4 only
  vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;
  vector<float> wilson_coeffs_plot  = gConfigParser->readFloatListOpt  ("eft::wilson_coeffs_plot") ;
  vector<float> wilson_coeffs       = gConfigParser->readFloatListOpt  ("eft::wilson_coeffs_gen") ;

  vector<string> wilson_coeff_ranges ;
  if (gConfigParser->hasOpt ("eft::wilson_coeff_ranges"))
    {
      wilson_coeff_ranges = gConfigParser->readStringListOpt ("eft::wilson_coeff_ranges") ;
      if (wilson_coeff_ranges.size () != wilson_coeff_names.size ())
        {
          wilson_coeff_ranges.clear () ;
          cout << "ranges list size does not match wilson coefficients list one, ignoring ranges\n" ;
        }
      else
        {
          for (int i = 0 ; i < wilson_coeff_ranges.size () ; ++i)
            replaceChar (wilson_coeff_ranges.at (i), ':', ',') ;
        }  
    }
  if (wilson_coeff_ranges.size () == 0)
      wilson_coeff_ranges = vector<string> (wilson_coeff_names.size (), "-2,2") ;

  jointSort (wilson_coeff_names, wilson_coeffs_plot, wilson_coeffs, wilson_coeff_ranges) ;

  map<string, pair <float, float> > plot_limits = getLimits (wilson_coeff_ranges, wilson_coeff_names) ;

// FINO A QUI


  // vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;
  // sort (wilson_coeff_names.begin (), wilson_coeff_names.end ()) ;

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

  vector <string> percorso = split (destination_folder_prefix, '/') ;
  string baseFileName = percorso.back () ;

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
  // double contours[1] = {2.30} ; // 1sigma, 95% CL
  double contours[2] = {2.30, 5.99} ; // 1sigma, 95% CL
  // float contours[2] = {2.30, 6.18} ; // 1sigma, 2sigma

  vector<vector <string> > bestVar ;
  vector<TGraph *> bestCont ;

  // loop over first coefficient
  for (int iCoeff1 = 0 ; iCoeff1 < wilson_coeff_names.size () ; ++iCoeff1)
    {
      // loop over second coefficient
      for (int iCoeff2 = iCoeff1 + 1 ; iCoeff2 < wilson_coeff_names.size () ; ++iCoeff2)
        {

          cout << "\n--> coeffs " << wilson_coeff_names.at (iCoeff1) 
               << " vs. " << wilson_coeff_names.at (iCoeff2) << endl ;
          cout << "---- ---- ---- ---- ---- ---- ---- ---- ---- " << endl ;

          string destination_folder = destination_folder_prefix 
                                      + "_" + wilson_coeff_names.at (iCoeff1) 
                                      + "_" + wilson_coeff_names.at (iCoeff2) ;

          vector<pair <string, cont> > contours_1sigma ;

          // loop over variables
          for (int iVar = 0 ; iVar < variables.size () ; ++iVar)
            {
//              cout << "--> var " << variables.at (iVar) << endl ;
              cout << "\t " << variables.at (iVar) << endl ;

              // gStyle->SetPalette (kDeepSea) ;
              TColor::CreateGradientColorTable (__NRGBs, __stops, __red, __green, __blue, __NCont) ;

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

              TGraph2D * g_scan = treeToGraph2D (
                  limitTree, 
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff1).c_str ()),
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff2).c_str ()), 
                  globalCut.c_str ()
                ) ;
              g_scan->Draw ("COLZ") ;

              if (g_scan->GetN () < 10)
                {
                  cout << "WARNING too few points found in the TGraph, skipping variable "
                       << variables.at (iVar)
                       << " for coeff " 
                       << wilson_coeff_names.at (iCoeff2) << " x " + variables.at (iVar)
                       << endl ;
                  continue ;
                }

              g_scan->GetHistogram ()->GetXaxis ()->SetTitle (TString (wilson_coeff_names.at (iCoeff1).c_str ())) ;
              g_scan->GetHistogram ()->GetYaxis ()->SetTitle (TString (wilson_coeff_names.at (iCoeff2).c_str ())) ;
              g_scan->GetHistogram ()->GetZaxis ()->SetTitle("- 2#Delta logL");

              string h_name = "h_" + localrootname + "_cont" ;
              TH2F * h_contour = (TH2F *) (g_scan->GetHistogram ())->Clone (h_name.c_str ()) ;

              for (int i=0; i<h_contour->GetSize () ; ++i) 
                {
                  //     std::cout << " [ " << i << " ] = " << graphScan->GetHistogram()->GetBinContent(i+1) << std::endl;
                  if (h_contour->GetBinContent (i+1) == 0) 
                    { 
                      h_contour->SetBinContent (i+1, 100) ;
                    }
                }

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

              if (contour->GetEntries () == 0)
                cout << "\t\tWARNING: no contour found\n" ;

              contours_1sigma.push_back (pair<string, cont> (variables.at (iVar), cont (contour))) ;

              // stats information from the likelihood scan
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   
              
              pair<float, float> llrMin = getMinCoordinates (
                  limitTree, 
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff1).c_str ()),
                  TString ("k_") + TString (wilson_coeff_names.at (iCoeff2).c_str ())) ;

              float area = getAreaWithinCL (h_contour, contours[0]) ;
              contours_1sigma.back ().second.area = area ;
              contours_1sigma.back ().second.xmin = llrMin.first ;
              contours_1sigma.back ().second.ymin = llrMin.second ;

              string outfile = destination_folder + string ("/plot")
                               + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                               + "_" + variables.at (iVar) + "_LLRscan" ;
              string outfile_pdf = outfile + ".pdf" ;
              string outfile_root = outfile + ".root" ;
              c1.SaveAs (outfile_pdf.c_str ()) ;
              c1.SaveAs (outfile_root.c_str ()) ;

            } // loop over variables
 
          sort (contours_1sigma.begin (), contours_1sigma.end (), sortByArea) ;

          TH1F * dummy_h = c1.DrawFrame 
            (
              plot_limits[wilson_coeff_names.at (iCoeff1)].first,
              plot_limits[wilson_coeff_names.at (iCoeff2)].first,
              plot_limits[wilson_coeff_names.at (iCoeff1)].second,
              plot_limits[wilson_coeff_names.at (iCoeff2)].second
            ) ;

          dummy_h->GetXaxis ()->SetTitle (wilson_coeff_names.at (iCoeff1).c_str ()) ;
          dummy_h->GetYaxis ()->SetTitle (wilson_coeff_names.at (iCoeff2).c_str ()) ;

          TLegend legend (0.70, 0.50, 0.95, 0.95) ;
          gStyle->SetPalette (kDarkBodyRadiator) ;
          // kRust
          // kBlueGreenYellow
          // kCopper
          // kDeepSea

          TGraph * dummy = new TGraph () ;
          for (int i = contours_1sigma.size () - 1 ; i >= 0 ; --i)
            {
              TIter next (contours_1sigma.at (i).second.graphs) ;
              bool first = true ;
              bestCont.push_back (dummy) ;
              while (TGraph * gr = (TGraph *) next ())
                {
                  vector<float> boundaries = findBoxAround (gr) ;
                  int icol = i * 250 / contours_1sigma.size () ; 
                  gr->SetLineColor (gStyle->GetColorPalette (icol)) ;
                  gr->SetLineWidth (2) ;
                  gr->Draw ("L same") ;
                  if (first)
                    {
                      legend.AddEntry (gr, contours_1sigma.at (i).first.c_str (), "l") ;
                      if (i == 0) bestCont.back () = gr ;
                      first = false ;
                    }
                  // for some reasons it draws is white
                  // drawMarker (contours_1sigma.at (i).second.xmin, contours_1sigma.at (i).second.ymin, 33, icol) ;
                }

            }

          legend.SetBorderSize (0) ;
          legend.SetFillStyle (0) ;
          legend.Draw () ;
          string outfile = string ("plot")
                           + "_" + baseFileName
                           + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2)
                           + "_compareScan.pdf" ;
          c1.SaveAs (outfile.c_str ()) ;

          vector <string> bestcomb (3) ;
          bestcomb.at (0) = wilson_coeff_names.at (iCoeff1) ;
          bestcomb.at (1) = wilson_coeff_names.at (iCoeff2) ;
          bestcomb.at (2) = contours_1sigma.at (0).first ;
          bestVar.push_back (bestcomb) ;

        } // loop over second Wilson coefficient
    } // second loop over first Wilson coefficient

  ofstream myfile ;
  string filename = destination_folder_prefix ;
  if (filename.back () == '/') filename = filename.substr (0, filename.size () - 1) ;
  filename += "_bestvars.txt" ;
  myfile.open (filename.c_str ()) ;
  myfile << "# cW1, cW2, var\n" ;
  for (int i = 0 ; i < bestVar.size () ; ++i)
    {
      myfile << bestVar.at(i).at (0)
             << ", " << bestVar.at(i).at (1)
             << ", " << bestVar.at(i).at (2)
             << "\n" ;
    }
  myfile.close () ;
  cout << "summary file " << filename << " created\n" ;

  replace (filename, ".txt", ".root") ;
  TFile outrootfile (filename.c_str (), "recreate") ;
  for (int i = 0 ; i < bestVar.size () ; ++i)
    {
      string grname = string ("cont")
                      + "_" + bestVar.at(i).at (0) + "_" +bestVar.at(i).at (1)
                      + "_" + bestVar.at(i).at (2) ;
      bestCont.at (i)->Write (grname.c_str ()) ;
    }
  outrootfile.Close () ;
  cout << "best profiles saved in " << filename << "\n" ;

  return 0 ; 


}