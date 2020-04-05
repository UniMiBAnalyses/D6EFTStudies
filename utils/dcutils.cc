#include "dcutils.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "TTreeReader.h"
#include "TFile.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TString.h"
#include "TMath.h"

using namespace std ;

void setTDRStyle () 
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  //  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.04, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector<string> 
split (const string& s, char delimiter)
{
   vector<string> tokens;
   string token ;
   istringstream tokenStream (s) ;
   while (getline (tokenStream, token, delimiter))
   {
      tokens.push_back (token) ;
   }
   return tokens ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// for debugging purposes
void printvector (vector<string> & v)
{
  for (int i = 0 ; i < v.size () ; ++i) cout << "\t" << v.at (i) ;
  cout << endl ;	
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool replace (std::string& str, const std::string& from, const std::string& to) 
{
    size_t start_pos = str.find (from) ;
    if (start_pos == std::string::npos) return false;
    str.replace (start_pos, from.length (), to) ;
    return true ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// produces vector of bin edges for uniform binning
vector<float> splitaxis (float min, float max, int nbins)
{
  vector<float> dummy ;
  float delta = (max - min) / nbins ;
  for (int i = 0 ; i <= nbins ; ++i)
    {
      dummy.push_back (min + delta * i) ;
    }
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// produce the vector of bin edges for the histograms,
// to that binning may be variable and tails do not have empty bins
vector<float> calcBinEdges (string varname, float min, float max, int nbins) 
{
  if (   varname == "deltaetajj" 
      || varname == "deltaphijj"
      || varname == "noshape")
    {
      return splitaxis (min, max, nbins) ;
    }  
  vector<float> dummy ;
  float delta = (max - min) / (3. * nbins) ;
  int changes = 0 ;
  float limit = min ;
  while (limit < max)
    {
      dummy.push_back (limit) ;
      if (changes == 2) continue ;
      if (limit >= 4./5. * (max-min)) 
        {
          delta = delta * 3. / 2. ;
          changes = 2 ;
        }  
      if (changes == 1) continue ;
      if (limit >= 3./4. * (max-min)) 
        {
          delta = delta * 2. ;
          changes = 1 ; 
        } 
      limit += delta ;
    }
  dummy.push_back (max) ;
  return dummy ;
}  


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// loop on the ntuple saved in a root file
// and fill the histograms to be used for the creation of the datacards.
// Each variable in the cfg file will have an histogram created.
map<string, TH1F *> 
readNtupleFile (string rootFileName, string ntupleName,
                string hTag, string hTitle,
                CfgParser * gConfigParser)
{
  // extract from the config parser the relevant information
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  // variables to be studied
  vector<string> variables = gConfigParser->readStringListOpt ("general::variables") ;
  sort (variables.begin (), variables.end ()) ;

  // create the output histograms
  map<string, TH1F *> output_histos ;
  for (int i = 0 ; i < variables.size () ; ++i)
    {
      vector<float> edges = calcBinEdges (
          variables.at (i),
          gConfigParser->readFloatOpt (variables.at (i) + "::min"),
          gConfigParser->readFloatOpt (variables.at (i) + "::max"),
          gConfigParser->readFloatOpt (variables.at (i) + "::bins")
       ) ;

      output_histos[variables.at (i)] = new TH1F 
        (
          (hTag + variables.at (i)).c_str (), hTitle.c_str (),
          edges.size () - 1,
          edges.data ()
        ) ;
    }

  // read cuts
  map <string, float> cuts ;
  vector<string> cutslist = gConfigParser->readListOfOpts ("cuts") ;
  for (int iCut = 0 ; iCut < cutslist.size () ; ++iCut)
    {
      cuts[cutslist[iCut]] = gConfigParser->readFloatOpt ("cuts::" + cutslist[iCut]) ;
    } 

  map <string, float> uppercuts ;
  vector<string> uppercutslist = gConfigParser->readListOfOpts ("uppercuts") ;
  for (int iCut = 0 ; iCut < uppercutslist.size () ; ++iCut)
    {
      uppercuts[uppercutslist[iCut]] = gConfigParser->readFloatOpt ("uppercuts::" + uppercutslist[iCut]) ;
    } 

  // open the input file and extract general info concerning the sample 
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  TFile rootFile (rootFileName.c_str ()) ;
  string histoName = ntupleName + "_nums" ;
  TH1F * global_numbers = (TH1F *) rootFile.Get (histoName.c_str ()) ;

  float cross_section        = global_numbers->GetBinContent (1) ;
  float sum_weights_total    = global_numbers->GetBinContent (2) ;
  float sum_weights_selected = global_numbers->GetBinContent (3) ;
  
  float luminosity = gConfigParser->readFloatOpt ("general::luminosity") ;
  //NB luminosity in fb, cross-section expected in pb in the config files
  float normalization = cross_section * 1000. * luminosity / (sum_weights_total) ;

  // open the TTree for reading
  TTreeReader reader (ntupleName.c_str (), &rootFile) ;

  // prepare the raeders for each variable
  bool savenoshape = false ;
  map<string, TTreeReaderValue<float> > treeReaderValues ;

  // all variables to be saved and to be cut on have to be read
  vector<string> allvars = variables ;
  allvars.insert (allvars.end (), cutslist.begin (), cutslist.end ()) ;
  allvars.insert (allvars.end (), uppercutslist.begin (), uppercutslist.end ()) ;
  sort (allvars.begin (), allvars.end ()) ;
  vector<string>::iterator it = unique (allvars.begin (), allvars.end ()) ;
  allvars.resize (distance (allvars.begin (), it)) ;
 
//  printvector (allvars) ;

  for (int iVar = 0 ; iVar < allvars.size () ; ++iVar)
    {	
      if (allvars[iVar] == "noshape") 
        {
          savenoshape = true ;
          continue ;
        }
      treeReaderValues.insert (
        pair<string, TTreeReaderValue<float> > (
          allvars[iVar], 
          TTreeReaderValue<float> (reader, allvars[iVar].c_str ())
        ) 
      ) ; 
//      treeReaderValues[ variables[iVar]] = TTreeReaderValue<float> (reader, variables[iVar].c_str ()) ;
    } 
  TTreeReaderValue<float> weight (reader, "w") ;

  // loop over the events
  while (reader.Next ())
    {
      // apply selections
      bool save = true ;
      for (map<string, float>::const_iterator iCut = cuts.begin () ;
           iCut != cuts.end () ;
           ++iCut)
        {
          if (*(treeReaderValues.at (iCut->first)) < iCut->second) 
            {
              save = false ;
              break ;
            } 
        } 
      for (map<string, float>::const_iterator iCut = uppercuts.begin () ;
           iCut != uppercuts.end () ;
           ++iCut)
        {
          if (*(treeReaderValues.at (iCut->first)) > iCut->second) 
            {
              save = false ;
              break ;
            } 
        } 
      if (save == false) continue ;

      // fill histograms
      for (map<string, TH1F*>::iterator iHisto = output_histos.begin () ;
           iHisto != output_histos.end () ;
           ++iHisto)
        {
          if (iHisto->first == "noshape") continue ; 
          iHisto->second->Fill ( *(treeReaderValues.at (iHisto->first)), *weight) ;
        }
      if (savenoshape) output_histos["noshape"]->Fill (1., *weight) ;

    } // loop over the events

  for (map<string, TH1F* >::const_iterator iHisto = output_histos.begin () ;
       iHisto != output_histos.end () ;
       ++iHisto)
    {
      iHisto->second->Scale (normalization) ;
      //overflow bin events moved in the last bin
      //last bin content=last bin content + content of overflow bin
      //FIXME underflow missing
      if (iHisto->first != "noshape")
        {
          iHisto->second->SetBinContent 
            (
          	  iHisto->second->GetNbinsX (), 
              iHisto->second->GetBinContent (iHisto->second->GetNbinsX ())
                + iHisto->second->GetBinContent (iHisto->second->GetNbinsX () + 1)
            ) ;
          iHisto->second->SetBinContent (iHisto->second->GetNbinsX () + 1, 0.) ;
        }
    } 
//  checkEmptyBins (output_histos) ;

  return output_histos ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void 
scaleAllHistos (std::map<std::string, TH1F *> & hMap, float value) 
{
  for (map<string, TH1F* >::const_iterator iHisto = hMap.begin () ;
       iHisto != hMap.end () ;
       ++iHisto)
    {
      iHisto->second->Scale (value) ;
    } 
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void 
checkEmptyBins (std::map<std::string, TH1F *> & hMap)
{
  cout << "Empty bins check:\n" ;
  for (map<string, TH1F* >::const_iterator iHisto = hMap.begin () ;
       iHisto != hMap.end () ;
       ++iHisto)
    {
      cout << " > histo: " << iHisto->second->GetName () << "\n" ; 	
      for (int iBin = 0 ; iBin < iHisto->second->GetNbinsX () + 2 ; ++iBin) 
        {
          if (fabs (iHisto->second->GetBinContent (iBin)) < 1)
            {
              cout << "   bin " << iBin << " = " << iHisto->second->GetBinContent (iBin) << "\n" ;
            }  
        }
    } 
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// build the list of the operators to be frozen
vector <string>
prepareFreeze (vector<string> activeCoeff, float frange)
{
  string range = to_string (-1 * frange) + "," + to_string (frange) ;

  string ranges = "" ;
  string list = "" ;
  string vals = "" ;
  string active = "" ;
  for (int iCoeff = 0 ; iCoeff < gAllCoeff.size () ; ++iCoeff)
    { 
      if (find (activeCoeff.begin (), activeCoeff.end (), gAllCoeff.at (iCoeff)) 
          == activeCoeff.end () ) 
        {
          list += "k_" + gAllCoeff.at (iCoeff) + "," ;
          vals += "k_" + gAllCoeff.at (iCoeff) + "=0," ;

        }
      else
        {
          active += "k_" + gAllCoeff.at (iCoeff) + "," ;
          ranges += "k_" + gAllCoeff.at (iCoeff) + "=" + range + ":" ;
        }  
    }
  // remove the last comma
  vector<string> result ;
  result.push_back (list.substr (0, list.size () - 1)) ;
  result.push_back (vals.substr (0, vals.size () - 1)) ;
  result.push_back (active.substr (0, active.size () - 1)) ;
  result.push_back (ranges.substr (0, ranges.size () - 1)) ;
  return result ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


string
merge (vector<string> list, const string & joint)
{
  string output ;
  for (int i = 0 ; i < list.size () - 1 ; ++i)
    output += list.at (i) + joint ;
  output += list.back () ;
  return output ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// NEW version, to replace the other one also for 1D
pair <string, string>  
createDataCard (TH1F * h_SM, map<string, TH1F *> h_eftInput, 
                string destinationfolder, string prefix, string varname,
                vector<string> active_coeffs, 
                CfgParser * gConfigParser)
{
  // create the root file containing the three histograms
  string rootfilename = destinationfolder + "/" + prefix + "_" + varname + ".root" ;
  // get the configuration of the combine running
  string comb_verbosity = gConfigParser->readStringOpt ("combine::verbosity") ;
  string comb_model     = gConfigParser->readStringOpt ("combine::model") ;

//  string wilson_coeff_list = gConfigParser->readStringOpt ("eft::wilson_coeff_names") ;
  vector<string> wilson_coeff_names = gConfigParser->readStringListOpt ("eft::wilson_coeff_names") ;

  TFile outf (rootfilename.c_str (), "recreate") ;
  h_SM->Write ("histo_sm") ;
  for (map<string, TH1F *>::iterator it = h_eftInput.begin () ; 
       it != h_eftInput.end () ; ++it)
    it->second->Write (("histo_" + it->first).c_str ()) ;
  outf.Close () ; // PG will this generate memory problems when using histos later on?

  // create the root file containing the three histograms
  string txtfilename = destinationfolder + "/" + prefix + "_" + varname + ".txt" ;
  ofstream output_datacard (txtfilename) ;

  string separator= "-------------------------------------------------------\n" ;
  output_datacard << separator ;
  output_datacard << "imax *" <<endl ;
  output_datacard << "jmax *" <<endl ;
  output_datacard << "kmax *" <<endl ;
  output_datacard << separator ;

  output_datacard << "shapes *\t* " + rootfilename + " histo_$PROCESS $PROCESS_$SYSTEMATIC\n" ;
  output_datacard << "shapes data_obs\t* " + rootfilename + " " + "histo_sm" << endl ;
  output_datacard << separator ;
  output_datacard << "bin\t\ttest\n" ;
  output_datacard << "observation\t" << h_SM->Integral () << endl ;
  output_datacard <<separator ;

  output_datacard << "bin\t\ttest";
  for (int i = 0 ; i < h_eftInput.size () ; ++i) output_datacard << "\ttest" ;
  output_datacard << "\n" ;

  output_datacard << "process\t"
                  << "\tsm" ;
  for (map<string, TH1F *>::iterator it = h_eftInput.begin () ; 
       it != h_eftInput.end () ; ++it)
    output_datacard << "\t" << it->first ;
  output_datacard << "\n" ;

  output_datacard << "process\t\t0" ;
  for (int i = 0 ; i < h_eftInput.size () ; ++i) output_datacard << "\t" << i+1 ;
  output_datacard << "\n" ;

  output_datacard << "rate\t\t" 
                  << h_SM->Integral () << "\t" ;
  for (map<string, TH1F *>::iterator it = h_eftInput.begin () ; 
       it != h_eftInput.end () ; ++it)
    output_datacard << it->second->Integral () << "\t" ;
  output_datacard << "\n" ;
  output_datacard <<separator ;

  output_datacard <<"lumi\t\tlnN\t1.02" ;
  for (map<string, TH1F *>::iterator it = h_eftInput.begin () ; 
       it != h_eftInput.end () ; ++it)
    output_datacard << "\t1.02" ;
  output_datacard << "\n" ;

//  output_datacard <<"bla\t\tlnN\t-\t-\t1.05\n";
  output_datacard.close () ;

  //PG apparently this may work with the EFT model only
  string wscreation_command = "text2workspace.py " ;
  wscreation_command += txtfilename ;
  wscreation_command += " -P HiggsAnalysis.AnalyticAnomalousCoupling." + comb_model ;
//  wscreation_command += " --PO=k_" + merge (wilson_coeff_names,",k_") + ",r" ;
  wscreation_command += " -o " ;
  replace (rootfilename, ".root", "_WS.root") ;
  wscreation_command += rootfilename ;
  wscreation_command += " > " + destinationfolder + "/WScreation_" + varname + ".log 2>&1" ; 

  vector<string> paramFreeze = prepareFreeze (active_coeffs) ;

  string fitting_command = "combine -M MultiDimFit " + rootfilename ;
  fitting_command += " --algo=grid --points 1200 -m 125" ;
  fitting_command += " -t -1 --expectSignal=1" ;  // FIXME check whehter expectSignal is needed
  fitting_command += " --redefineSignalPOIs " + paramFreeze.at (2) ;
  fitting_command += " --freezeParameters r," + paramFreeze.at (0) ;
  fitting_command += " --setParameters r=1," ; // + paramFreeze.at (1) ;
  fitting_command += " --setParameterRanges " + paramFreeze.at (3) ;
  fitting_command += " --verbose " + comb_verbosity ;
  fitting_command += " --robustFit=1" ;
  fitting_command += " --X-rtd FITTER_NEW_CROSSING_ALGO" ;
  fitting_command += " --X-rtd FITTER_NEVER_GIVE_UP" ;
  fitting_command += " > " + destinationfolder + "/fitting_" + varname + ".log 2>&1" ; 
  replace (rootfilename, "_WS.root", "_fitresult.root") ;
  fitting_command += " ; mv higgsCombineTest.MultiDimFit.mH125.root " + rootfilename  ;

  return pair <string, string> (wscreation_command, fitting_command) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


string findAfter (const vector<string> & command, const string & token)
{
  bool found = false ; 
  for (int i = 0 ; i < command.size () ; ++i)
    {
      if (found) return command.at (i) ;
      if (command.at (i) == token) found = true ;
    }
  return "NOTHING_FOUND" ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void
createCondorScripts (pair <std::string, string> fittingCommands,
                     string output_folder,
                     string cmssw_folder,
                     string execution_folder,
                     string varname)
{

  string folder = execution_folder + "/" + output_folder ;

  vector<string> step1words = split (fittingCommands.first, ' ') ;
  string output1 = findAfter (step1words, "-o") ;
  string output2 = findAfter (step1words, ">") ;
  vector<string> step2words = split (fittingCommands.second, ' ') ;
  string output3 = findAfter (step2words, ">") ;
  ofstream jobfile (output_folder + "/submit_" + varname + ".sh") ;
  jobfile << "#!/usr/bin/bash\n" ;
  jobfile << "cd " << cmssw_folder << "\n" ;
  jobfile << "eval `scram run -sh`\n" ;
  jobfile << "cd -\n" ;
  jobfile << "cp -r " << folder << " ./\n" ; 
  jobfile << fittingCommands.first  << "\n" ;
  jobfile << fittingCommands.second << "\n" ;
  jobfile << "cp " << output1            << " " << folder << "\n" ; 
  jobfile << "cp " << output2            << " " << folder << "\n" ; 
  jobfile << "cp " << output3            << " " << folder << "\n" ; 
  jobfile << "cp " << step2words.back () << " " << folder << "\n" ; 
  jobfile.close () ;

  ofstream submitfile (output_folder + "/submit_" + varname + ".sub") ;
  submitfile << "executable = " + output_folder + "/submit_" + varname + ".sh\n" ;
  submitfile << "output     = " + output_folder + "/submit_" + varname + ".out\n" ;
  submitfile << "error      = " + output_folder + "/submit_" + varname + ".err\n" ;
  submitfile << "log        = " + output_folder + "/submit_" + varname + ".log\n" ;
  submitfile << "queue 1\n" ;
  submitfile << "+JobFlavour = \"espresso\"\n" ;
  submitfile.close () ;

// espresso  20min 8nm
// microcentury  1h  1nh
// longlunch 2h  8nh
// workday 8h  1nd
// tomorrow  1d  2nd
// testmatch 3d  1nw
// nextweek  1w  2nw

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int 
plotHistos (TH1F * h_SM, map<string, TH1F *> h_eftInput,
            string destinationfolder, string prefix, string varname, 
            vector<float> h_rescales, 
            bool log) 
{
  setTDRStyle () ;

  vector<TH1F *> h_loc ;
  TH1F * h_tot = (TH1F *) h_SM->Clone (TString (h_SM->GetName ()) + "_TOT") ;
  for (map<string, TH1F *>::iterator it = h_eftInput.begin () ;
       it != h_eftInput.end () ; ++it)
    {
      h_loc.push_back ((TH1F *) it->second->Clone (TString (it->second->GetName ()) + "_loc")) ; 
      h_loc.back ()->Scale (h_rescales.at (h_loc.size () - 1)) ;      
      h_tot->Add (h_loc.back ()) ;
    }

  h_tot->SetLineColor (kOrange + 8) ;
  h_tot->SetFillColor (kOrange + 8) ;
  h_tot->SetTitle ("") ;
  h_tot->GetXaxis ()->SetTitle (varname.c_str ()) ;

  h_SM->SetLineColor (1) ;
  h_SM->SetFillColor (kYellow-9) ;

  TLegend legend (0.70, 0.80, 0.90, 0.90) ;
  legend.AddEntry (h_SM    , "SM"    , "f") ;
  legend.AddEntry (h_tot   , "total" , "f") ;
  legend.SetBorderSize (0) ;
  legend.SetFillStyle (0) ;

  TCanvas c1 ("c1", "", 600, 600) ;
  if (log) c1.SetLogy () ;
  h_tot->DrawCopy ("HIST") ;
  h_tot->SetFillColor (kWhite) ;
  h_tot->Draw ("HIST same") ;
  h_SM->Draw ("HIST same") ;
  c1.RedrawAxis () ;
  legend.Draw () ;

  // create the root file containing the three histograms
  string outfilename = destinationfolder + "/" + prefix + "_plots_" + varname ;
  if (log) outfilename += "_log" ;
  outfilename += ".gif" ;
  c1.Print (outfilename.c_str (), "gif") ;

  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// set the likelihood scan aestetics
void setLSaspect (TGraph * graphScan, string variable)
{

  graphScan->SetTitle ("") ;
  graphScan->SetMarkerStyle (21) ;
  graphScan->SetLineWidth (2);
  graphScan->SetMarkerColor (kRed) ;
  graphScan->SetLineColor (kRed) ;

  graphScan->GetXaxis()->SetTitle (variable.c_str ()) ;
  graphScan->GetYaxis()->SetTitle ("-2 #Delta LL") ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


pair<float, float>
getXrange (TGraph * graphScan)
{
  // FIXME implement more sophisticated version that performs  horizontal zoom
  // FIXME such that on the vertical axis one arrives at a certain max DLL value
  return pair<float, float> 
    (
      graphScan->GetXaxis ()->GetXmin (),
      graphScan->GetXaxis ()->GetXmax ()
    ) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float 
getLSminimum (TGraph * graphScan)
{
  int n = graphScan->GetN () ;
  double * y = graphScan->GetY () ;
  int locmin = TMath::LocMin (n,y) ;
  return y[locmin] ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector <float>
getLSintersections (TGraph * graphScan, float val)
{
  vector <float> xings ;
  int n = graphScan->GetN () ;
  double * x = graphScan->GetX () ;
  double * y = graphScan->GetY () ;
  bool found = false ;
  pair<float, float> around ;
  // loop over tgraph points
  for (int i = 1 ; i < n ; ++i)
    {
      if (y[i] == val) 
        {
          xings.push_back (x[i]) ;
          continue ;
        }  
      // notice a crossing 
      if ((y[i] - val) * (y[i-1] - val) < 0)
        {
          xings.push_back (x[i-1] +  fabs ((y[i-1] - val) * (x[i] - x[i-1]) / (y[i] - y[i-1])) ) ;
        }
    }  // loop over tgraph points

  if (xings.size () == 0)
    { 
      cout << "WARNING: returning graph x-axis range limits" << endl ;
      xings.push_back (graphScan->GetXaxis ()->GetXmin ()) ;
      xings.push_back (graphScan->GetXaxis ()->GetXmax ()) ;
    }
  else if (xings.size () == 1)
    {
      if (xings.back () < 0) 
        {
          cout << "WARNING: returning graph x-axis higher limit" << endl ;
          xings.push_back (graphScan->GetXaxis ()->GetXmax ()) ;
        }
      else 
        {
          cout << "WARNING: returning graph x-axis lower limit" << endl ;
          xings.push_back (xings.back ()) ;
          xings.at (0) = graphScan->GetXaxis ()->GetXmin () ;
        }
    }
  if (xings.size () > 2) 
    {
      cout << "WARNING: more than two intersections found, returning the first two" << endl ;
      xings.resize (2) ;

    }
  return xings ;
}  


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool
sortBySensitivity (const limits_var & a ,
                   const limits_var & b)

{
  return (  (0.5 * (a.second[0] - a.second[1]))
          < (0.5 * (b.second[0] - b.second[1])) ) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TH1F * setDummyHisto (float xmin, float ymin, float xmax, float ymax, 
                      vector<string> labels)
{
  TH1F * dh = new TH1F ("dh", "", xmax - xmin, xmin, xmax) ;
  dh->Fill (xmin + 0.1 * (xmax - xmin), ymin) ;
  dh->Fill (xmin + 0.9 * (xmax - xmin), ymax) ;
  dh->SetFillColor (0) ;
  dh->SetLineColor (10) ;
  for (int i = 1 ; i <= labels.size () ; ++i) 
    dh->GetXaxis ()->SetBinLabel (i, labels[i-1].c_str ()) ;
  return dh ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void 
drawSensitivities (string op, 
                   limits_var_v limits, 
                   string basefilename)
{

  // prepare the sensitivity bars,
  // separated for positive and negative intersections
  // of the likelihood profile with the horizonal lines
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  string hname = "h_" + op + "_OSUP" ; 
  TH1F h_OSUP (hname.c_str (), "", limits.size (), 0, limits.size ()) ;
  hname = "h_" + op + "_SSDO" ; 
  TH1F h_OSDO (hname.c_str (), "", limits.size (), 0, limits.size ()) ;
  hname = "h_" + op + "_TSUP" ; 
  TH1F h_TSUP (hname.c_str (), "", limits.size (), 0, limits.size ()) ;
  hname = "h_" + op + "_TSDO" ; 
  TH1F h_TSDO (hname.c_str (), "", limits.size (), 0, limits.size ()) ;

  float min = +10. ;
  float max = -10. ;
  vector<string> labels ;
  for (int i = 0 ; i < limits.size () ; ++i)
    {
      h_OSDO.Fill (i+0.5, limits.at (i).second.at (0)) ;
      h_OSUP.Fill (i+0.5, limits.at (i).second.at (1)) ;
      h_TSDO.Fill (i+0.5, limits.at (i).second.at (2)) ;
      h_TSUP.Fill (i+0.5, limits.at (i).second.at (3)) ;

      if (limits.at (i).second.at (2) < min) min = limits.at (i).second.at (2) ;
      if (limits.at (i).second.at (3) > max) max = limits.at (i).second.at (3) ;

      labels.push_back (limits.at (i).first) ;
    }

  // draw the sensitivity bars in a single canvas
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  TCanvas clims ("clims", "", 600, 600) ;
  TH1F * dh = setDummyHisto (0, min * 1.1, limits.size (), max * 1.1, labels) ;
  dh->GetYaxis ()->SetTitle ((op + " sensitivity").c_str ()) ;
  dh->Draw ("hist") ;

  h_OSDO.SetFillColor (kOrange+1) ;
  h_OSUP.SetFillColor (kOrange+1) ;
  h_TSDO.SetFillColor (kCyan-3) ;
  h_TSUP.SetFillColor (kCyan-3) ;

  h_TSDO.Draw ("hist same") ;
  h_TSUP.Draw ("hist same") ;
  h_OSDO.Draw ("hist same") ;
  h_OSUP.Draw ("hist same") ;

  TLegend legend (0.70, 0.80, 0.90, 0.90) ;
  legend.AddEntry (&h_OSDO, "one sigma bound", "f") ;
  legend.AddEntry (&h_TSDO, "two sigma bound", "f") ;
  legend.SetBorderSize (0) ;
  legend.SetFillStyle (0) ;
  legend.Draw () ;

  clims.RedrawAxis () ;
  clims.SaveAs ((basefilename + "_cfr.png").c_str ()) ;

  // save histos in a root file
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  TFile outrootfile ((basefilename + "_cfr.root").c_str (), "recreate") ;
  h_TSDO.Write () ;
  h_TSUP.Write () ;
  h_OSDO.Write () ;
  h_OSUP.Write () ;
  outrootfile.Write () ;

  // save numbers in a txt file
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  ofstream myfile;
  myfile.open ((basefilename + "_cfr.txt").c_str ()) ;
  myfile << "#var  1sigmaLow 1sigmaHigh 2sigmaLow 2sigmaHigh\n" ;
  for (int i = 0 ; i < limits.size () ; ++i)
    {
      myfile << limits.at (i).first
             << "\t" << limits.at (i).second.at (0)
             << "\t" << limits.at (i).second.at (1)
             << "\t" << limits.at (i).second.at (2)
             << "\t" << limits.at (i).second.at (3)
             << "\n" ;
    }
  myfile.close () ;
 
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void 
writeCSVlimits (limits_op_v all_limits, 
                string basefilename)
{
  // save numbers in a csv file
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  ofstream myfile;
  myfile.open ((basefilename + "_CSV.txt").c_str ()) ;
  myfile << "operator, variable, 1sigmaLow, 1sigmaHigh, 2sigmaLow, 2sigmaHigh\n" ;
  for (int iOp = 0 ; iOp < all_limits.size () ; ++iOp)
    {
      string op = all_limits.at (iOp).first ;
      for (int iVar = 0 ; iVar < all_limits.at(iOp).second.size () ; ++iVar)
        {
          myfile << op
                 << ", " << all_limits.at(iOp).second.at (iVar).first
                 << ", " << all_limits.at(iOp).second.at (iVar).second.at (0)
                 << ", " << all_limits.at(iOp).second.at (iVar).second.at (1)
                 << ", " << all_limits.at(iOp).second.at (iVar).second.at (2)
                 << ", " << all_limits.at(iOp).second.at (iVar).second.at (3)
                 << "\n" ;
        }
    }
  myfile.close () ;
 
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bridge::bridge (vector<string> &v1, vector<float> &v2, vector<float> &v3)
  {
    vector<float> dummy ;
    dummy.push_back (0.) ; 
    dummy.push_back (0.) ; 
    for (int i = 0 ; i < v1.size () ; ++i)
      {
        dummy.at (0) = v2.at (i) ;
        dummy.at (1) = v3.at (i) ;
        m_container.push_back ( pair<string, vector<float>> (v1.at (i), dummy) ) ;
      }  
  } 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void bridge::pour (vector<string> &v1, vector<float> &v2, vector<float> &v3)
  {
    assert (v1.size () == m_container.size ()) ;  
    assert (v2.size () == m_container.size ()) ;  
    assert (v3.size () == m_container.size ()) ;  
    for (int i = 0 ; i < m_container.size () ; ++i)
      {
        v1.at (i) = m_container.at (i).first ;
        v2.at (i) = m_container.at (i).second.at (0) ;
        v3.at (i) = m_container.at (i).second.at (1) ;
      }
    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool
sortByFirstElem (const std::pair<std::string, std::vector<float> > & a , 
                 const std::pair<std::string, std::vector<float> > & b) 
{
  return a.first < b.first ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void
jointSort (vector<string> &v1, vector<float> &v2, vector<float> &v3)
{
  assert (v1.size () == v2.size ()) ;
  assert (v2.size () == v3.size ()) ;
  bridge sorter (v1, v2, v3) ;
  sort (sorter.m_container.begin (), sorter.m_container.end (), sortByFirstElem) ;
  sorter.pour (v1, v2, v3) ;

  return ;
}

