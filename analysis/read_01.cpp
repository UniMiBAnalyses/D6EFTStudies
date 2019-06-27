// c++ -o read_01 `root-config --glibs --cflags` CfgParser.cc -lm read_01.cpp
/*
read LHE files produced with madgraph and produce some plots, 
with no selections applied.
*/



#include "LHEF.h"
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
#include "TLorentzVector.h"

#include "CfgParser.h"

using namespace std ;


// all histograms of a single sample
struct histos
{
  TString m_name ;  // name of the sample
  double  m_XS ;    // cross-section of the sample (in fb)
  double  m_lumi ;  // integrated luminosity (in /fb)
  map<string, TH1F *> m_histograms ;
  
  histos (TString name, double XS, double lumi = 1.) : 
    m_name (name), 
    m_XS (XS),
    m_lumi (lumi)
    {
      makeHisto ("m_mjj", 48, 200., 5000.) ;
      makeHisto ("m_mll", 15, 0., 1500.) ;
      makeHisto ("m_ptl1", 15, 0., 1500.) ;
      makeHisto ("m_ptl2", 15, 0., 1500.) ;
    }
 
  // simplify histogram creation
  TH1F * makeHisto (const TString & varname, int nBins, float min, float max)
    {
      TH1F * histo = new TH1F (varname + TString ("_") + m_name, 
                               varname + TString ("_")  + m_name, 
                               nBins, min, max) ;
      histo->Sumw2 () ;
      if (m_histograms.find (varname.Data ()) != m_histograms.end ())
        {
          cout << "WARNING histogram " << varname << " defined twice" << endl ;
          return histo ;
        }
      m_histograms[varname.Data ()] = histo ;
      return histo ;
    } 

  // fill the histograms through the map
  void fill (string varname, double value, double weight = 1.)
    {
      if (m_histograms.find (varname) == m_histograms.end ())
        {
          cout << "WARNING histogram " << varname << " does not exist" << endl ;
          return ;
        }
      m_histograms[varname]->Fill (value, weight) ;
      return ; 
    }
  
  // normalise histograms to the integrated cross-section
  void norm (double inputIntegral = 0)
    {
      double factor = m_lumi * m_XS / fabs (m_histograms.begin ()->second->Integral ()) ;
      if (inputIntegral != 0) factor = m_lumi * m_XS / inputIntegral ;
      for (map<string, TH1F *>::iterator it = m_histograms.begin () ; 
           it != m_histograms.end () ; ++it)
         it->second->Scale (factor) ;
    }
  
  ~histos ()
    {
      // this being empty is not very nice, to be replaced with the unique_ptr or auto_ptr thing 
      // for the histogram pointers, but I need to find a smart way to do it
      // w/o need for implementing it for each histogram I add      
    }  
    
  void save (TFile & outfile) 
    {
      outfile.cd () ;
      for (map<string, TH1F *>::iterator it = m_histograms.begin () ; 
           it != m_histograms.end () ; ++it)
        it->second->Write () ;
    }
  
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TLorentzVector buildLP (LHEF::Reader & reader, int iPart)
{
  TLorentzVector tlv
    (
      reader.hepeup.PUP.at (iPart).at (0), //PG px
      reader.hepeup.PUP.at (iPart).at (1), //PG py
      reader.hepeup.PUP.at (iPart).at (2), //PG pz
      reader.hepeup.PUP.at (iPart).at (3)  //PG E
    ) ;
  return tlv ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// Fill the histograms for a single sample
// histograms will not get normalised, since the same sample
// could be split in several LHE files and this function
// may get called several times, one for each LHE file.
// Therefore, the normalisation will have to be called afterwards
double 
fillHistos (LHEF::Reader & reader, histos & Histos, int max = -1)
{
  int events = 0 ;
   
  //PG loop over input events
  while (reader.readEvent ()) 
    {
//      if ( reader.outsideBlock.length() ) std::cout << reader.outsideBlock;

      if (events % 10000 == 0) cout << "reading event in file: " << events << endl ;
          
      vector<TLorentzVector> v_f_Ws ;
      vector<TLorentzVector> v_f_quarks ;
      vector<TLorentzVector> v_f_leptons ;
      vector<TLorentzVector> v_f_neutrinos ;

      // loop over particles in the event
      for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) 
        {
          // outgoing particles          
          if (reader.hepeup.ISTUP.at (iPart) == 1)
            {
              TLorentzVector dummy = buildLP (reader, iPart) ;
              // quarks
              if (abs (reader.hepeup.IDUP.at (iPart)) < 7) 
                {
                  v_f_quarks.push_back (dummy) ;        
                } // quarks
              else if (abs (reader.hepeup.IDUP.at (iPart)) == 11 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 13 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 15)
                {
                  v_f_leptons.push_back (dummy) ;
                }
              else if (abs (reader.hepeup.IDUP.at (iPart)) == 12 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 14 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 16)
                {
                  v_f_neutrinos.push_back (dummy) ;        
                }
            } // outgoing particles
        } // loop over particles in the event

      int warnNum = 0 ;
      if (v_f_quarks.size () < 2)
        {
          cout << "warning, not enough quarks" << endl ;
          ++warnNum ;
        }
      if (v_f_leptons.size () < 2)
        {
          cout << "warning, not enough leptons" << endl ;
          ++warnNum ;
        }
      if (v_f_neutrinos.size () < 2)
        {
          cout << "warning, not enough neutrinos" << endl ;
          ++warnNum ;
        }
      if (warnNum > 0) continue ;

      //PG fill histograms
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      TLorentzVector v_jj = v_f_quarks.at (0) + v_f_quarks.at (1) ;
      TLorentzVector v_ll = v_f_leptons.at (0) + v_f_leptons.at (1) ;

//      Histos.m_mjj->Fill (v_jj.M (), reader.hepeup.XWGTUP) ;
//      Histos.m_mll->Fill (v_ll.M (), reader.hepeup.XWGTUP) ;
      Histos.fill ("m_mjj", v_jj.M ()) ;
      Histos.fill ("m_mll", v_ll.M ()) ;

      float ptl1 = v_f_leptons.at (0).Pt () ;
      float ptl2 = v_f_leptons.at (1).Pt () ;

      if (ptl1 < ptl2) swap (ptl1, ptl2) ;

      Histos.fill ("m_ptl1", ptl1) ;
      Histos.fill ("m_ptl2", ptl2) ;

      ++events ;
      if (max > 0 && max < events) 
        {
          cout << max << " events reached, exiting" << endl ;
          break ;
        }

    } //PG loop over input events

  return events ;
  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


struct inputInfo
{
  double mass ;
  string mg_file ;
  double mg_xs ;
  string ph_file ;
  double ph_xs ;
  void print ()
    {
      cout << "----------------------------\n" ;
      cout << "mass    : " << mass << "\n" ;
      cout << "mg_file : " << mg_file << "\n" ;
      cout << "mg_xs   : " << mg_xs << "\n" ;
      cout << "ph_file : " << ph_file << "\n" ;
      cout << "ph_xs   : " << ph_xs << "\n" ;
      cout << "----------------------------\n" ;
    }
} ;


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
      cout << "reading " << maxEventsPerSample << " events per sample" << endl ;
    }

  CfgParser * gConfigParser = new CfgParser (argv[1]) ;

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


  //PG prepare the histogram structs to be filled,
  //PG fill the histograms looping on LHE events

  // loop over samples
  map<string, pair<float, vector<string> > >::iterator it ;
  vector<histos> Histos ;
  for (it = samples.begin () ; it != samples.end () ; ++it)
    {
      cout << "sample: " << it->first << endl ;
      Histos.push_back (histos (it->first, it->second.first)) ;
      // loop over files
      int events = 0 ;
      for (int ifile = 0 ; ifile < it->second.second.size () ; ++ifile)
        {
          cout << "    reading: " << it->second.second.at (ifile) << endl ;
          std::ifstream ifs (it->second.second.at (ifile).c_str ()) ;
          LHEF::Reader reader (ifs) ;
          events += fillHistos (reader, Histos.back (), maxEventsPerSample) ;
          if (maxEventsPerSample > 0 && events >= maxEventsPerSample) break ;
          cout << "    read: " << events << " events" << endl ;

        } // loop over files
      Histos.back ().norm () ;
    } // loop over samples

  //PG save histograms
  string outfilename = gConfigParser->readStringOpt ("general::outputFile") ;
  TFile outFile (outfilename.c_str (), "recreate") ;
  for (int i = 0 ; i < Histos.size () ; ++i) Histos.at (i).save (outFile) ;
  outFile.Close () ;

  return 0 ;
}
