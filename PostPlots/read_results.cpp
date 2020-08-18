/*
c++ -o read_results `root-config --glibs --cflags` ../utils/CfgParser.cc ../utils/dcutils.cc -lm read_results.cpp

read the results obtained by combine running on the various workspaces created
when running on the outcome datacard_creator_2 with the same input cfg file
as the one used for this prgram

*/

#include <vector>
#include <string>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"

#include "../utils/CfgParser.h"
#include "../utils/dcutils.h"

using namespace std ;

int main (int argc, char ** argv) 
{

  setTDRStyle () ;

  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  // read input informantion
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;

  string outfiles_prefix           = gConfigParser->readStringOpt ("output::outfiles_prefix") ;
  string destination_folder_prefix = gConfigParser->readStringOpt ("output::destination_folder_prefix") ;
  string destination_root_folder = gConfigParser->readStringOpt ("output::destination_root_folder") ;

  vector<string> variables = gConfigParser->readStringListOpt ("general::variables") ;
  float luminosity         = gConfigParser->readFloatOpt ("general::luminosity") ;

  // alternative location of result files with respect to the cfg file
  if (argc > 2)
    {
      vector <string> percorso = split (destination_folder_prefix, '/') ;
      destination_folder_prefix = argv[2] + string ("/") + percorso.back () ;
    }

  // vector <string> percorso = split (destination_folder_prefix, '/') ;
  // string baseFileName = percorso.back () ;

  TCanvas c1 ("c1", "", 600, 600) ;

  TLatex * tex1 = new TLatex (0.94, 0.90, "13 TeV") ;
  tex1->SetNDC () ;
  tex1->SetTextAlign (31) ;
  tex1->SetTextFont (42) ;
  tex1->SetTextSize (0.04) ;
  tex1->SetLineWidth (2) ;

  TLatex * tex2 = new TLatex (0.52, 0.90, ("L = " + to_string (int (luminosity)) + " fb^{-1}").c_str ()) ;
  tex2->SetNDC () ;
  tex2->SetTextFont (52) ;
  tex2->SetTextSize (0.035) ;
  tex2->SetLineWidth (2) ;

  limits_op_v all_limits ;

  // loop over Wilson coefficients
  for (int iCoeff = 0 ; iCoeff < wilson_coeff_names.size () ; ++iCoeff)
    {

      cout << "--> coeff " << wilson_coeff_names.at (iCoeff) << endl ;
      cout << "---- ---- ---- ---- ---- ---- ---- ---- ---- " << endl ;

      string wilson_coeff_name = wilson_coeff_names.at (iCoeff) ;
      string destinationfolder = destination_root_folder + destination_folder_prefix + "_" + wilson_coeff_names.at (iCoeff) ;
      TString toDraw = Form ("2*deltaNLL:%s", ("k_" + wilson_coeff_name).c_str ()) ;

      limits_var_v limits ;
      // loop on variables
      for (int iVar = 0 ; iVar < variables.size () ; ++iVar)
        {
          cout << "--> var " << variables.at (iVar) << endl ;

          // build the results filename
          string filename = destinationfolder + "/" + outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_" + variables.at (iVar) + "_fitresult.root" ;
          TFile * result_file = new TFile (filename.c_str ()) ;
          TTree * limit = (TTree*) result_file->Get ("limit") ;  
          int n = limit->Draw (toDraw.Data () , "deltaNLL<50 && deltaNLL>-30", "l") ;
          TGraph * graphScan = new TGraph (n, limit->GetV2 (),limit->GetV1 ()) ;
          graphScan->SetName("graph");
	  graphScan->RemovePoint (0) ;
          setLSaspect (graphScan, wilson_coeff_name) ;
          graphScan->Draw ("AL") ;
//          graphScan->SetMarkerStyle (2) ;
//          graphScan->SetMarkerColor (4) ;
//          graphScan->Draw ("ALP") ;

              //  2deltaLogL = 1.00
          pair<float, float> xrange = getXrange (graphScan) ;
          
          //   TLine *line1 = new TLine((limit->GetV2())[0],1.0,(limit->GetV2())[n-1],1.0);
          TLine *line1 = new TLine (xrange.first, 1.0, xrange.second, 1.0) ;
          line1->SetLineWidth (2) ;
          line1->SetLineStyle (2) ;
          line1->SetLineColor (kRed) ;
          line1->Draw () ; 
            
          //  2deltaLogL = 3.84
          //   TLine *line2 = new TLine((limit->GetV2())[0],3.84,(limit->GetV2())[n-1],3.84);
          TLine *line2 = new TLine (xrange.first, 3.84, xrange.second, 3.84) ;
          line2->SetLineWidth (2) ;
          line2->SetLineStyle (2) ;
          line2->SetLineColor (kRed) ;
          line2->Draw () ;
      
          c1.RedrawAxis () ;

	  const char *variabil = gvarNames.at(variables.at(iVar)).c_str();
          TLatex * tex3 = new TLatex (0.20, 0.90,  variabil) ;
//          TLatex * tex3 = new TLatex (0.14, 0.92,  "m^{jj}") ; al massimo metto iVar
          tex3->SetNDC () ;
          tex3->SetTextFont (61) ;
          tex3->SetTextSize (0.04) ;
          tex3->SetLineWidth (2) ;
      
          tex1->Draw ("same") ;
          tex2->Draw ("same") ;
          tex3->Draw ("same") ;

          TLegend legend (0.70, 0.80, 0.90, 0.90) ;
          legend.AddEntry (graphScan, "likelihood scan", "l") ;
          legend.SetBorderSize (0) ;
          legend.SetFillStyle (0) ;
          legend.Draw () ;

          string outfilename = destinationfolder + "/" + outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_" + variables.at (iVar) + "_scan" ;
          string outfilename_png =  outfilename + ".png" ;
          string outfilename_root = outfilename + ".root" ;
          c1.SaveAs (outfilename_png.c_str ()) ;
          c1.SaveAs (outfilename_root.c_str ()) ;

          // ranges
          // ---- ---- ---- ---- ---- ----

          cout << getLSminimum (graphScan) << endl ;
          vector<float> OSB = getLSintersections (graphScan, 1.  ) ; // One Sigma Boundaries
          vector<float> TSB = getLSintersections (graphScan, 3.84) ; // Two Sigma Boundaries
          OSB.insert (OSB.end (), TSB.begin (), TSB.end ()) ; 	
          limits.push_back (limits_var (variables.at (iVar), OSB)) ;

        } // loop on variables

      sort (limits.begin (), limits.end (), sortBySensitivity) ;
      all_limits.push_back (limits_op (wilson_coeff_name, limits)) ;
      drawSensitivities (wilson_coeff_name, limits, 
                         destinationfolder + "/" + outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff), 
                         true) ;

    } // loop over Wilson coefficients

  cout << "this is destination folder for the CSV limits fle: " << destination_folder_prefix << endl ;
  writeCSVlimits (all_limits, destination_root_folder + "/" + outfiles_prefix, true) ;
  

  return 0 ; 


}
