// c++ -o fit_02 `root-config --glibs --cflags` CfgParser.cc  -lm fit_02.cpp
/**
Perform a test fit on a SM-like toy experiment, 
differential in the variables indicated in a config file,
deciding automatically the binning of the variables
on the basis of the differences between SM and BSM behaviour.
*/


#include <iostream>
#include <string>
#include <cmath>

#include "TH1.h"
#include "TNtuple.h"
#include "TFile.h"
#include "ROOT/RDataFrame.hxx"

#include "CfgParser.h"

using namespace std ;


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
  string vars = "" ;
  for (int i = 0 ; i < variables.size () ; ++i)
    {
      vars += variables.at (i) ;
      vars += "," ;
    }
  vars = vars.substr (0, vars.size () - 1) ;

  //PG integrated luminosity
  float lumi = gConfigParser->readFloatOpt ("general::lumi") ;

  //PG standard model sample
  string SMInputFileName = gConfigParser->readStringOpt ("samples::SMInputFileName") ;
  string SMNtupleName = gConfigParser->readStringOpt ("samples::SMNtupleName") ;
  ROOT::RDataFrame SMdf (SMNtupleName, SMInputFileName, {vars});
  cout << "SM events number: " << *SMdf.Count () << endl ;

  string SMNumsHistoName = gConfigParser->readStringOpt ("samples::SMNumsHistoName") ;
  TFile SMInputFile (SMInputFileName.c_str (), "READ") ;
  TH1F * SMNumsHisto = (TH1F *) SMInputFile.Get (SMNumsHistoName.c_str ()) ;
  float SMXS = SMNumsHisto->GetBinContent (1) ;   // sample cross-section
  float SMtotW = SMNumsHisto->GetBinContent (2) ; // weighted sum of events in the sample
  float SMweight = lumi * SMXS / SMtotW ;
  cout << "SM cross-section: " << SMXS << " fb" << endl ;

  //PG squared BSM term  
  string BSMInputFileName = gConfigParser->readStringOpt ("samples::BSMInputFileName") ;
  string BSMNtupleName = gConfigParser->readStringOpt ("samples::BSMNtupleName") ;
  ROOT::RDataFrame BSMdf (BSMNtupleName, BSMInputFileName, {vars});
  cout << "BSM events number: " << *BSMdf.Count () << endl ;

  string BSMNumsHistoName = gConfigParser->readStringOpt ("samples::BSMNumsHistoName") ;
  TFile BSMInputFile (BSMInputFileName.c_str (), "READ") ;
  TH1F * BSMNumsHisto = (TH1F *) BSMInputFile.Get (BSMNumsHistoName.c_str ()) ;
  float BSMXS = BSMNumsHisto->GetBinContent (1) ;
  float BSMtotW = BSMNumsHisto->GetBinContent (2) ;
  float BSMweight = lumi * BSMXS / BSMtotW ;
  cout << "BSM cross-section: " << BSMXS << " fb" << endl ;
  
  //PG interference term
  string INTNtupleName = gConfigParser->readStringOpt ("samples::INTNtupleName") ;
  ROOT::RDataFrame INTdf (INTNtupleName, BSMInputFileName, {vars});
  cout << "INT events number: " << *INTdf.Count () << endl ;

  string INTNumsHistoName = gConfigParser->readStringOpt ("samples::INTNumsHistoName") ;
  TH1F * INTNumsHisto = (TH1F *) BSMInputFile.Get (INTNumsHistoName.c_str ()) ;
  float INTXS = INTNumsHisto->GetBinContent (1) ;
  float INTtotW = INTNumsHisto->GetBinContent (2) ;
  float INTweight = lumi * INTXS / INTtotW ;
  cout << "INT cross-section: " << INTXS << " fb" << endl ;


  //PG FIXME this will become a loop later
  string var = variables.at (0) ; 

  
  //PG get the range of the distributions
  float hMIN = *SMdf.Min (var) ;
  if (hMIN > min (*BSMdf.Min (var), *INTdf.Min (var))) 
    hMIN = min (*BSMdf.Min (var), *INTdf.Min (var)) ;

  float hMAX = *SMdf.Max (var) ;
  if (hMAX < max (*BSMdf.Min (var), *INTdf.Min (var))) 
    hMAX = max (*BSMdf.Min (var), *INTdf.Min (var)) ;

  
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
   - fill histograms from the ntuple, of SM, INT, BSM, with the maximum stats available,
     to define the initial model, after the selections applied
   - fill a SM histogram (toy experiment) with the number of events expected with the lumi, 
     after selection applied
   - define a BSM model with SM + c_w * INT + c_w^2 * BSM, where in INT and BSM
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