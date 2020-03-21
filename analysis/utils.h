#ifndef utils_h
#define utils_h

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

#include "TH1.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TLorentzVector.h"

#include "LHEF.h"
#include "CfgParser.h"

using namespace std ;

/**
 all histograms of a single sample
*/
struct histos
{
  TString m_name ;  // name of the sample
  double  m_XS ;    // cross-section of the sample (in fb)
  double  m_lumi ;  // integrated luminosity (in /fb)
  
  // the collection of histograms to be filled
  std::map<std::string, TH1F *> m_histograms ;

  // integral of the events collected, 
  // needed for the histograms normalisation
  double m_norm ;

  histos (TString name, double XS, double lumi = 1.) ;
  // simplify histogram creation
  TH1F * makeHisto (const TString & varname, int nBins, float min, float max) ;
  // to be called at each event, before any selections, 
  // for a proper normalisation of the histograms
  double increaseNorm (double eventweight = 1) ;
  // fill the histograms through the std::map
  void fill (std::string varname, double value, double weight = 1.) ;
  // normalise histograms to the integrated cross-section
  void norm () ;
  // dtor
  ~histos () ;
  // save histos in the outfile
  void save (TFile & outfile) ;
  
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
 Fill the histograms for a single sample.
 Histograms will not get normalised, since the same sample
 could be split in several LHE files and this function
 may get called several times, one for each LHE file.
 Therefore, the normalisation will have to be called afterwards
*/
double fillHistos (LHEF::Reader & reader, histos & Histos, int max = -1) ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
 fill a map according to the list of variables received as input
*/
struct ntuple
{

  TString m_name ;                    // name of the sample
  double  m_XS ;                      // cross-section of the sample (in fb)
  double  m_lumi ;                    // integrated luminosity (in /fb)
  TNtuple * m_ntuple ;                // where the events are stored
  std::vector<float> m_values ;       // values of single variables in each event
  std::map<std::string, int> varpos ; // position of variable in the vector of values
  int m_varcounter ;                  // to check that all variables have been filled at each event
  TH1F * m_nums ;                     // dummy histogram to save numbers [bin]:
                                      // m_num[1] is the XS
                                      // m_num[2] is the total events integral (sum of weights)
                                      // m_num[3] is the selected events integral (sum of weights)


  std::map<std::string, int>::iterator m_it ; // service iterator

  // integral of the events collected, 
  // needed for the histograms normalisation
  double m_total ;
  // integral of the events selected, 
  // needed for the histograms normalisation
  double m_selected ;

  //ctor
  ntuple (std::vector<std::string> variables, TString name, double XS, double lumi = 1.) ;
  // fill the value of a single variable
  void setvalue (std::string varname, double value) ;
  // transfer values in the ntuple
  void fill (double weight = 1.) ;
  // to be called at each event, before any selections, 
  // for a proper normalisation of the histograms
  double increaseNorm (double eventweight = 1) ;
  // save ntuples in the outfile
  void save (TFile & outfile) ;
    //dtor
  ~ntuple () ;

} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float deltaPhi (float phi, float phi2) ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
 Fill the class ntuple for a single sample.
*/
double fillNtuple (LHEF::Reader & reader, ntuple & Ntuple, int max = -1, bool applyCuts = false) ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


inline float zetaStar (float etaq1, float etaq2, float eta)
{
  return (eta - 0.5 * (etaq1 + etaq2)) / fabs (etaq1 - etaq2) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TLorentzVector buildLP (LHEF::Reader & reader, int iPart) ;
#endif
