// c++ -o fit_03 `root-config --glibs --cflags` CfgParser.cc  -lm fit_03.cpp
/**
Perform a test fit on a SM-like toy experiment, 
differential in the variables indicated in a config file,
deciding automatically the binning of the variables
on the basis of the differences between SM and INT behaviour.

TTreeReader example: https://root.cern.ch/doc/v608/classTTreeReader.html

*/


#include <iostream>
#include <string>
#include <cmath>

#include "TH1.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TTreeReader.h"

#include "CfgParser.h"

using namespace std ;


float getVarSum (TTreeReader & nt, string varname)
{
  float sum = 0. ; 
  nt.Restart () ;
  TTreeReaderValue<Float_t> myVar (nt, varname.c_str ());
  TTreeReaderValue<Float_t> weight (nt, "w");

  while (nt.Next()) 
    {
      sum += *myVar * *weight ;
    } 

  return sum ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float getVarSumSq (TTreeReader & nt, string varname)
{
  float sumsq = 0. ; 
  nt.Restart () ;
  TTreeReaderValue<float> myVar (nt, varname.c_str ());
  TTreeReaderValue<float> weight (nt, "w");

  while (nt.Next()) 
    {
      sumsq += *myVar * *myVar * *weight * *weight ;
    } 

  return sumsq ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float getVarMean (TTreeReader & nt, string varname)
{
  return getVarSum (nt, varname) / nt.GetEntries (true) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float getVarSigma (TTreeReader & nt, string varname)
{
  float dummy = getVarMean (nt, varname) ;
  dummy *= dummy ;
  return sqrt (getVarSumSq (nt, varname) / nt.GetEntries (true) - dummy) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  //PG get the list of variables to be plotted,
  //PG assuming some code exists to calculate them
  //PG ideally using the same cfg file sintax used for the ntuple production
  vector<string> variables = gConfigParser->readStringListOpt ("general::variables") ;
  //PG integrated luminosity
  float lumi = gConfigParser->readFloatOpt ("general::lumi") ;

  string SMInputFileName = gConfigParser->readStringOpt ("samples::SMInputFileName") ;
  TFile SMInputFile (SMInputFileName.c_str (), "READ") ;
  string SMNtupleName = gConfigParser->readStringOpt ("samples::SMNtupleName") ;
  TTreeReader SMtr (SMNtupleName.c_str (), &SMInputFile) ;
  cout << "SM events number: " << SMtr.GetEntries (true) << endl ;
  string SMNumsHistoName = gConfigParser->readStringOpt ("samples::SMNumsHistoName") ;
  TH1F * SMNumsHisto = (TH1F *) SMInputFile.Get (SMNumsHistoName.c_str ()) ;
  float SMXS = SMNumsHisto->GetBinContent (1) ;   // sample cross-section
  float SMtotW = SMNumsHisto->GetBinContent (2) ; // weighted sum of events in the sample
  float SM_weight = lumi * SMXS / SMtotW ;
  cout << "SM cross-section: " << SMXS << " fb" << endl ;

  string BSMInputFileName = gConfigParser->readStringOpt ("samples::BSMInputFileName") ;
  TFile BSMInputFile (BSMInputFileName.c_str (), "READ") ;
  string BSMNtupleName = gConfigParser->readStringOpt ("samples::BSMNtupleName") ;
  TTreeReader BSMtr (BSMNtupleName.c_str (), &BSMInputFile) ;
  cout << "BSM events number: " << BSMtr.GetEntries (true) << endl ;
  string BSMNumsHistoName = gConfigParser->readStringOpt ("samples::BSMNumsHistoName") ;
  TH1F * BSMNumsHisto = (TH1F *) BSMInputFile.Get (BSMNumsHistoName.c_str ()) ;
  float BSMXS = BSMNumsHisto->GetBinContent (1) ;
  float BSMtotW = BSMNumsHisto->GetBinContent (2) ;
  float BSM_weight = lumi * BSMXS / BSMtotW ;
  cout << "BSM cross-section: " << BSMXS << endl ;
  
  string INTNtupleName = gConfigParser->readStringOpt ("samples::INTNtupleName") ;
  TTreeReader INTtr (INTNtupleName.c_str (), &BSMInputFile) ;
  cout << "INT events number: " << INTtr.GetEntries (true) << endl ;
  string INTNumsHistoName = gConfigParser->readStringOpt ("samples::INTNumsHistoName") ;
  TH1F * INTNumsHisto = (TH1F *) BSMInputFile.Get (INTNumsHistoName.c_str ()) ;
  float INTXS = INTNumsHisto->GetBinContent (1) ;
  float INTtotW = INTNumsHisto->GetBinContent (2) ;
  float INT_weight = lumi * INTXS / INTtotW ;
  cout << "INT cross-section: " << INTXS << " fb" << endl ;

  string var = "ptl1" ;

  cout << "SM weighted mean for ptl1: " << getVarMean (SMtr, var) << endl ;
  cout << "   weighted sigma for ptl1: " << getVarSigma (SMtr, var) << endl ;

  cout << "BSM weighted mean for ptl1: " << getVarMean (BSMtr, var) << endl ;
  cout << "    weighted sigma for ptl1: " << getVarSigma (BSMtr, var) << endl ;

  
/*
 - get input from a from config file:
	- var list
	- sm sample file and name
	- bsm sample file and names
	- lumi
	- expected sensitivity on the Wilson coefficient (in terms of its value)
	- selection applied

 - for each variable
   - define initial variables
      - tntuples for the input distributions
      - from tntuples, calculate the range of the histograms, the number of bins
      - binning of hisograms:
        - start from SM, since bsm sensitivity is not known yet.
          start from inferior extreme of x axis, 
          and define default bin size as a fraction of the histogram sigma.
          When the number of events goes below a certain threshold, enlarge the bin 
          until a minimum number of expected events given lumi is present,
          until a maximum threshold for the bin size, defined as a fixed number of the 
          default bin size.
        - is there a better way to find each bin size and the number of bins,
          besides start from reasonable hypotheses and a rather biased procedure?
          - toss random numbers in the x range, use them to define bins, calculate 
            the difference in shape between the two obtained histograms, miximise the 
            difference. 
            - Random numbers is too generic.
          - hypothesis: N (small) bins is the best configuration.
            Try N+1, where the N+1 bin has a variable width, 
            and the other ones are shrinked to fit the remaining interval. 
            Start from 1 and go up until no improvements in a metric that measures
            the distance of the two histograms are observed
          - hypothesis: N (large) bins is the best configuration.
            Try N-1, with a loop over removed bins. For each removed bin, 
            define a way to resize the others. It seems can diverge very quicly 
            in many different cases.
      - range of histograms:
        - define the range as the one which includes all the bins,
          for which the SM expected number of events or the total_bsm expected n of events
          for the case of the expected Wilson coefficient sensitivity is larger than one
   - fill histograms from the ntuple, of SM, INT, INT, with the maximum stats available,
     to define the initial model, after the selections applied
   - fill a SM histogram (toy experiment) with the number of events expected with the lumi, 
     after selection applied
   - define a INT model with SM + c_w * INT + c_w^2 * INT, where in INT and INT
     the right normalisation is inserted in order to have all renormalised to the right
     value of c_w
   - perform a Likelihood fit of the model on the toy experiment
     - define the likelihood
       - use RooFit or plain root? In case of plain root, define a function from a histogram
     - actually do the fit
   - show the likelihood scan (and obtain the uncertinty on the fit)
     (- test the goodness of the estimate of the likelihood profile with several toys generations)
   - from the result of the fit, determine the actual sensitivity on c_w  
     - re-perform the study from the binning definition, starting from the new number 
       for the sensitivity
*/




}