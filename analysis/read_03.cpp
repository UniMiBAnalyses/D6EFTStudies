// c++ -o read_03 `root-config --glibs --cflags` CfgParser.cc utils.cc LHEF.cc -lm read_03.cpp
/*
read LHE files and produce ntuples
apply basic VBS selections
http://govoni.web.cern.ch/govoni/tesi/docs/Alessandro_Tarabini_Tesi.pdf
*/


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
#include "utils.h"

using namespace std ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) 
{

  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  int maxEventsPerSample = -1 ;
  if (argc >= 3)
    {
      maxEventsPerSample = atoi (argv[2]) ;
    }
  cout << "reading " << maxEventsPerSample << " events per sample" << endl ;

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

  // if the variable is true, 
  // some VBS cuts are applied (and the presence of two jets in the event is required)
  bool applyCuts = gConfigParser->readBoolOpt ("general::applycuts") ;

  //PG get the list of variables to be plotted,
  //PG assuming some code exists to calculate them
  vector<string> variables = gConfigParser->readStringListOpt ("general::variables") ;

  //PG get the samples to be analised, 
  //PG including collection of LHE files and the relative XS
  vector<string> collections = gConfigParser->readStringListOpt ("general::samples") ;

  map<string, pair<float, vector<string> > > samples ;
  // loop over samples
  for (int i = 0 ; i < collections.size () ; ++i)
    {
      float XS = gConfigParser->readFloatOpt (collections.at (i) + "::XS") ;
      vector<string> inputfiles = gConfigParser->readStringListOpt (collections.at (i) + "::files") ;
      samples[collections.at (i)] = pair<float, vector<string> > (XS, inputfiles) ;
    } // loop over samples

  // loop over samples
  map<string, pair<float, vector<string> > >::iterator it ;
  vector<ntuple> Ntuples ;
  for (it = samples.begin () ; it != samples.end () ; ++it)
    {
      cout << "sample: " << it->first << endl ;
      Ntuples.push_back (ntuple (variables, it->first, it->second.first)) ;
      //                                       ^^^             ^^^
      //                                   sample-name     cross-section

      // loop over files
      int events = 0 ;
      for (int ifile = 0 ; ifile < it->second.second.size () ; ++ifile)
        {
          cout << "    reading: " << it->second.second.at (ifile) << endl ;
          std::ifstream ifs (it->second.second.at (ifile).c_str ()) ;
          LHEF::Reader reader (ifs) ;

          events += fillNtuple (reader, Ntuples.back (), maxEventsPerSample, applyCuts) ;

          if (maxEventsPerSample > 0 && events >= maxEventsPerSample) break ;
          cout << "    read: " << events << " events" << endl ;

        } // loop over files
    } // loop over samples

  //PG save ntuples
  string outfilename = gConfigParser->readStringOpt ("general::outputFile") ;
  TFile outFile (outfilename.c_str (), "recreate") ;
  for (int i = 0 ; i < Ntuples.size () ; ++i) Ntuples.at (i).save (outFile) ;
  outFile.Close () ;
  cout << "file " << outfilename << " created\n" ;

  return 0 ;
}
