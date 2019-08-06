// c++ -o checkEntries `root-config --glibs --cflags` -lm checkEntries.cpp
/*
Check the number of entries in ntuples, given the root file containing them

root tutorial here:
https://root.cern/doc/master/loopdir_8C.html
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
//#include "TIter.h"
#include "TKey.h"
#include "TNtuple.h"
#include "TROOT.h"

using namespace std ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) 
{

  if (argc < 2)
    {
      cerr << "Forgot to put the root file --> exit " << endl ;
      return 1 ;
    }

  TFile *f1 = TFile::Open (argv[1]) ;
  TIter keyList (f1->GetListOfKeys ()) ;
  TKey * key ;
  while ((key = (TKey*) keyList ())) 
    {
      TClass * cl = gROOT->GetClass (key->GetClassName ()) ;
      if (!cl->InheritsFrom ("TNtuple")) continue ;
      TNtuple * nt = (TNtuple*) key->ReadObj () ;
      cout << nt->GetName () << ":\t" << nt->GetEntries () << endl ;
    }

  return 0 ;
}


