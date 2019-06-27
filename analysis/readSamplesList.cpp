// c++ -o readSamplesList `root-config --glibs --cflags` CfgParser.cc -lGenVector -lm readSamplesList.cpp

#include "CfgParser.h"
#include <string>

using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

/*
  TString config ; 
  config.Form ("%s",argv[1]) ;
  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }
*/

  vector<string> collections = gConfigParser->readStringListOpt ("general::samples") ;

  for (int i = 0 ; i < collections.size () ; ++i)
    {
      float XS = gConfigParser->readFloatOpt (collections.at (i) + "::XS") ;
      cout << collections.at (i) << "\tXS = " << XS << endl ;
      vector<string> inputfiles = gConfigParser->readStringListOpt (collections.at (i) + "::files") ;
      for (int j = 0 ; j < inputfiles.size () ; ++j)
        cout << "\t" << inputfiles.at (j) << endl ;
    }

  delete gConfigParser ;

  return 0 ;
}