/*
c++ -o datacard_twoOp_creator_2 `root-config --glibs --cflags` CfgParser.cc dcutils.cc -lm datacard_creator_2.cpp


one should pass the program a config file like file.cfg
to run: ./datacard_creator_2 file.cfg
*/

// FIXME da generalizzare aggiungendo il loop sui coefficienti di Wilson
// FIXME e la ricerca dei termini incrociati
// FIXME per ora dovrebbe girare, se la variabile e' una sola

#include <iomanip>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

#include <TFile.h>
#include <TNtuple.h>
#include <TTreeReader.h>
#include <TH1.h>
#include <TH1F.h>

#include "CfgParser.h"
#include "dcutils.h"

using namespace std ;


int main (int argc, char ** argv)
{
  TH1::SetDefaultSumw2 () ;
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  // reading generic parameters of the generation
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;
  vector<float> wilson_coeffs_plot  = gConfigParser->readFloatListOpt ("eft::wilson_coeffs_plot") ;
  vector<float> wilson_coeffs       = gConfigParser->readFloatListOpt ("eft::wilson_coeffs_gen") ;
  jointSort (wilson_coeff_names, wilson_coeffs_plot, wilson_coeffs) ;

  // reading input and output files information
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  string input_files_folder   = gConfigParser->readStringOpt ("input::files_folder") ;
  string input_files_prefix   = gConfigParser->readStringOpt ("input::files_prefix") ;
  string input_ntuples_prefix = gConfigParser->readStringOpt ("input::ntuples_prefix") ;

  string outfiles_prefix           = gConfigParser->readStringOpt ("output::outfiles_prefix") ;
  string destination_folder_prefix = gConfigParser->readStringOpt ("output::destination_folder_prefix") ;

  string cmssw_folder = gConfigParser->readStringOpt ("combine::cmssw_folder") ;

  map<string, TH1F *> hmap_SM = readNtupleFile (
      input_files_folder + "/" + input_files_prefix + "_SM.root", 
      input_ntuples_prefix + "_SM", 
      "SM_", "sm", gConfigParser
    ) ;

  vector<string> input_files (2, "") ;
  vector<string> ntuple_names  (2, "") ;

  map <string, map<string, TH1F *> > h_LI ;
  map <string, map<string, TH1F *> > h_QU ;

  // first loop to open all 1D histograms
  for (int iCoeff = 0 ; iCoeff < wilson_coeff_names.size () ; ++iCoeff)
    {
      // first coefficient
      input_files.at (0) = input_files_folder + "/" + input_files_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_LI.root" ;
      input_files.at (1) = input_files_folder + "/" + input_files_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_QU.root" ;

      // first coefficient
      ntuple_names.at (0) = input_ntuples_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_LI" ;
      ntuple_names.at (1) = input_ntuples_prefix + "_" + wilson_coeff_names.at (iCoeff) + "_QU" ;

      // first coefficient
      h_LI[wilson_coeff_names.at (iCoeff)] = readNtupleFile (input_files[0], ntuple_names[0], wilson_coeff_names.at (iCoeff) + "_LI_", "linear",    gConfigParser) ;
      scaleAllHistos (h_LI[wilson_coeff_names.at (iCoeff)], 1./wilson_coeffs.at (iCoeff)) ;
      h_QU[wilson_coeff_names.at (iCoeff)] = readNtupleFile (input_files[1], ntuple_names[1], wilson_coeff_names.at (iCoeff) + "_QU_", "quadratic", gConfigParser) ;
      scaleAllHistos (h_QU[wilson_coeff_names.at (iCoeff)], 1./(wilson_coeffs.at (iCoeff) * wilson_coeffs.at (iCoeff))) ;
    } // first loop to open all histograms

  // second loop over Wilson coefficients, for 2D fits
  // and create datacards, root files, and condor scripts for 1D fits
  for (int iCoeff1 = 0 ; iCoeff1 < wilson_coeff_names.size () ; ++iCoeff1)
    {

      for (int iCoeff2 = iCoeff1 + 1 ; iCoeff2 < wilson_coeff_names.size () ; ++iCoeff2)
        {

          cout << "--> coeffs " << wilson_coeff_names.at (iCoeff1) 
               << " vs. " << wilson_coeff_names.at (iCoeff2) << endl ;
          cout << "---- ---- ---- ---- ---- ---- ---- ---- ---- " << endl ;
    
          // interference between the two
          string input_file = input_files_folder + "/" + input_files_prefix + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2) + "_IN.root" ;
          string ntuple_name = input_ntuples_prefix + "_" + wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2) + "_IN" ;

          // reading the physics from the input files
          // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    
          // interference between the two
          map<string, TH1F *> hmap_IN = readNtupleFile (input_file, ntuple_name, wilson_coeff_names.at (iCoeff1) + "_" + wilson_coeff_names.at (iCoeff2) + "_IN_", "interference", gConfigParser) ;
          scaleAllHistos (hmap_IN, 1./(wilson_coeffs.at (iCoeff1) * wilson_coeffs.at (iCoeff2))) ;
    
          // creating datacards and rootfile for each variable
          // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    
          vector<pair <string, string> > WScreation_commands ;
          // FIXME creare se non esiste, pulire se esiste
          string destination_folder = destination_folder_prefix + "_" + wilson_coeff_names.at (iCoeff1)+ "_" + wilson_coeff_names.at (iCoeff2) ;
          mkdir (destination_folder.c_str (), S_IRWXU) ;
          // https://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
          // expected error if exists: EEXIST          17      /* File exists */
          // permission options listed here https://pubs.opengroup.org/onlinepubs/007908775/xsh/sysstat.h.html
    
          //loop on variables  
          for (map<string, TH1F* >::const_iterator iHisto = hmap_SM.begin () ;
               iHisto != hmap_SM.end () ;
               ++iHisto)
            {
              // get the three histograms 
              TH1F * h_SM = iHisto->second ;
              TH1F * h_LI_C1 = h_LI[wilson_coeff_names.at (iCoeff1)].at (iHisto->first) ;
              TH1F * h_QU_C1 = h_QU[wilson_coeff_names.at (iCoeff1)].at (iHisto->first) ;
              TH1F * h_LI_C2 = h_LI[wilson_coeff_names.at (iCoeff2)].at (iHisto->first) ;
              TH1F * h_QU_C2 = h_QU[wilson_coeff_names.at (iCoeff2)].at (iHisto->first) ;
              TH1F * h_IN = hmap_IN.at (iHisto->first) ;

              map<string, TH1F *> h_eftInputs ;
              h_eftInputs["linear_" + wilson_coeff_names.at (iCoeff1)]      = h_LI_C1 ;
              h_eftInputs["quadratic_" + wilson_coeff_names.at (iCoeff1)]   = h_QU_C1 ;
              h_eftInputs["linear_" + wilson_coeff_names.at (iCoeff2)]      = h_LI_C2 ;
              h_eftInputs["quadratic_" + wilson_coeff_names.at (iCoeff2)]   = h_QU_C2 ;
              h_eftInputs["interference_" + wilson_coeff_names.at (iCoeff1) 
                          + "_" + wilson_coeff_names.at (iCoeff2)]          = h_IN ;

              vector<string> active_coeffs ; 
              active_coeffs.push_back (wilson_coeff_names.at (iCoeff1)) ;
              active_coeffs.push_back (wilson_coeff_names.at (iCoeff2)) ;

              WScreation_commands.push_back (
                  createDataCard (h_SM, h_eftInputs, 
                                  destination_folder, outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff1)+ "_" + wilson_coeff_names.at (iCoeff2), 
                                  iHisto->first, active_coeffs,
                                  gConfigParser)
                ) ;

              // plotting histograms for visualisation purposes
              // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

              vector<float> h_rescales ;
              h_rescales.push_back (wilson_coeffs_plot.at (iCoeff1) / wilson_coeffs.at (iCoeff1)) ; // Lin C1
              h_rescales.push_back (h_rescales.back () * h_rescales.back ()) ;                      // Qua C1
              h_rescales.push_back (wilson_coeffs_plot.at (iCoeff2) / wilson_coeffs.at (iCoeff2)) ; // Lin C2
              h_rescales.push_back (h_rescales.back () * h_rescales.back ()) ;                      // Qua C2
              h_rescales.push_back (h_rescales.at (0) + h_rescales.at (2)) ;                        // Interf

              plotHistos (h_SM, h_eftInputs, destination_folder, 
                          outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff1)+ "_" + wilson_coeff_names.at (iCoeff2),
                          iHisto->first, h_rescales) ;

              plotHistos (h_SM, h_eftInputs, destination_folder, 
                          outfiles_prefix + "_" + wilson_coeff_names.at (iCoeff1)+ "_" + wilson_coeff_names.at (iCoeff2),
                          iHisto->first, h_rescales, true) ;
        
              char pathname[300] ;
              createCondorScripts (WScreation_commands.back (),
                                   destination_folder,
                                   cmssw_folder,
                                   getcwd (pathname, 300),
                                   iHisto->first) ;
    
            } //loop on variables  
          // creating the scripts to be launched to use combine
          // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
          
          ofstream WScreation_script (destination_folder + "/launchWScreation.sh") ;
          WScreation_script << "#!/usr/bin/bash\n" ;
          WScreation_script << "\n" ;
          for (int i = 0 ; i < WScreation_commands.size () ; ++i)
               WScreation_script << WScreation_commands.at (i).first << "\n" ;
          WScreation_script.close () ;
        
          ofstream fitting_script (destination_folder + "/launchFitting.sh") ;
          fitting_script << "#!/usr/bin/bash\n" ;
          fitting_script << "\n" ;
          for (int i = 0 ; i < WScreation_commands.size () ; ++i)
               fitting_script << WScreation_commands.at (i).second << "\n" ;
          fitting_script.close () ;
        
          cout << "Datacards and plots created.\n" ;
          cout << "To convert datacards in workspaces, run (from the same folder where datacard_creator_2 was executed): \n";
          cout << "source " << destination_folder << "/launchWScreation.sh\n" ;
          cout << "To launch the fitting process, run (from the same folder where datacard_creator_2 was executed): \n";
          cout << "source " << destination_folder << "/launchFitting.sh\n" ;
    
        } // loop over second Wilson coefficient
    } // second loop over Wilson coefficients, for 2D fits

  return 0 ;
}
