// c++ -o findInterference `root-config --glibs --cflags` `lhapdf-config --cppflags  --ldflags` -lGenVector -lm findInterference.cpp


#include "LHEF.h"
#include <iomanip>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <cassert>
#include <fstream>

#include "TH1.h"
#include "TFile.h"
// #include "TLorentzVector.h"
// CINT does not understand some files included by LorentzVector
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

//PG #include "LHAPDF.h"
#include "LHAPDF/LHAPDF.h"


using namespace ROOT::Math ;
using namespace std ;

#include "findInterferenceTools.h"


struct histos
{
  TH1F *  m_h_MWW ;
  TH1F *  m_h_scale ;
  TString m_name ;
  double  m_XS ;
  
  histos (TString name, double XS) : m_name (name), m_XS (XS)
    {
      m_h_MWW = new TH1F (TString ("h_MWW_") + name, 
                          TString ("h_MWW_") + name, 800., 200., 1800.) ;
      m_h_MWW->Sumw2 () ;
      m_h_scale = new TH1F (TString ("h_scale_") + name, 
                          TString ("h_scale_") + name, 100, 0., 1000.) ;
      m_h_scale->Sumw2 () ;
    }
 
  void norm (double total = 0)
    {
      double factor = m_XS / m_h_MWW->GetEntries () ;
      if (total != 0) factor = m_XS / total ;
      m_h_MWW->Scale (factor) ;
      m_h_scale->Scale (factor) ;
    }
  
  ~histos ()
    {
      delete m_h_MWW ;
      delete m_h_scale ;
    }  
    
  void save (TFile & outfile) 
    {
      outfile.cd () ;
      m_h_MWW->Write () ;
      m_h_scale->Write () ;
    }
  

} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double 
fillHistos (LHEF::Reader & reader, histos & Histos, double XS, double referenceScale = 0, int max = -1)
{
  double totalCount = 0. ;
  int events = 0 ;
   
  //PG loop over input events
  while (reader.readEvent ()) 
    {
//      if ( reader.outsideBlock.length() ) std::cout << reader.outsideBlock;

      if (events % 10000 == 0) cout << "reading " << events << " event" << endl ;
          
      vector<lorentzVector> v_f_Ws ;
      vector<lorentzVector> v_f_quarks ;
      vector<lorentzVector> v_f_leptons ;
      vector<lorentzVector> v_f_neutrinos ;
      
      double x[2] = {0., 0.} ;
      int flavour[2] = {0, 0} ;

      // loop over particles in the event
      for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) 
        {
          lorentzVector dummy = buildLP (reader.hepeup, iPart) ;

           // incoming particles        
           if (reader.hepeup.ISTUP.at (iPart) == -1) 
             {
               x[iPart] = dummy.P () / 4000. ;
               flavour[iPart] = reader.hepeup.IDUP.at (iPart) ;
             } // incoming particles         

          // intermediate particles          
          if (reader.hepeup.ISTUP.at (iPart) == 2)
            {
              if (abs (reader.hepeup.IDUP.at (iPart)) == 24) 
                {
                  v_f_Ws.push_back (dummy) ;
                }              
            } // intermediate particles
            
          // outgoing particles          
          if (reader.hepeup.ISTUP.at (iPart) == 1)
            {
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

//      if (totalCount < 10) cout << "PARTICLES " <<  v_f_leptons.size () << "\t" << v_f_neutrinos.size () << "\t" << v_f_quarks.size () << "\n" ;

      double weight = 1. ;
      float scale = reader.hepeup.SCALUP ;
      if (referenceScale != 0 )
        weight = LHAPDF::xfx (x[0], referenceScale, flavour[0]) * LHAPDF::xfx (x[1], referenceScale, flavour[1]) /
                 (LHAPDF::xfx (x[0], scale, flavour[0]) * LHAPDF::xfx (x[1], scale, flavour[1])) ;
      totalCount += weight ;

      int warnNum = 0 ;
      if (v_f_quarks.size () < 4)
        {
          cout << "warning, not enough quarks" << endl ;
          ++warnNum ;
        }
      if (v_f_leptons.size () < 1)
        {
          cout << "warning, not enough leptons" << endl ;
          ++warnNum ;
        }
      if (v_f_neutrinos.size () < 1)
        {
          cout << "warning, not enough neutrinos" << endl ;
          ++warnNum ;
        }
      if (warnNum > 0) continue ;

      //PG apply all the production cuts from phantom and madgraph to the sample
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      if (v_f_leptons.at (0).Pt () < 20) continue ;
      if (v_f_leptons.at (0).E () < 20) continue ;
      if (fabs (v_f_leptons.at (0).Eta ()) > 3) continue ;
      if (fabs (v_f_neutrinos.at (0).Pt ()) < 20) continue ;
      int cont = 0 ;
      for (int i = 0 ; i < 4 ; ++i) 
        if (v_f_quarks.at (i).Pt () > 20 && 
            v_f_quarks.at (i).E () > 20 && 
            fabs (v_f_quarks.at (i).Eta ()) < 6.5) cont += 1 ;
      if (cont < 4) continue ;
      
      pair<int, int> detaIndices = findPairWithLargestDeta (v_f_quarks) ;
      if (v_f_quarks.at (detaIndices.second).Eta () - v_f_quarks.at (detaIndices.first).Eta () < 2) continue ;
      lorentzVector largestPair = v_f_quarks.at (detaIndices.second) + v_f_quarks.at (detaIndices.first) ;
      if (largestPair.M () < 100) continue ; //PG selection applied in phantom

      cont = 0 ;
      for (int iJ = 0 ; iJ < 4 ; ++iJ)
        for (int iJ2 = iJ + 1 ; iJ2 < 4 ; ++iJ2)
          {
//            if (v_f_quarks.at (iJ).DeltaR (v_f_quarks.at (iJ2)) < 0.4) 
//              {
//                cont = 1 ;
//                break ;
//              }
            lorentzVector thisPair = v_f_quarks.at (iJ) + v_f_quarks.at (iJ2) ;
            if (thisPair.M () < 30)  
              {
                cont = 1 ;
                break ;
              }
          }
      if (cont == 1) continue ;

      cont = 0 ;
      for (int iJ = 0 ; iJ < 4 ; ++iJ)
        {
          if (deltaR2<lorentzVector> (v_f_quarks.at (iJ), v_f_leptons.at (0)) < 0.16) cont = 1 ;
        }
      if (cont == 1) continue ;

      //PG the first two are the VBF jets, the following ones the W jets
//      sort (v_f_quarks.rbegin (), v_f_quarks.rend (), ptsort ()) ;  
      
//      pair<int, int> Wpair (2, 3) ;
      pair<int, int> Wpair = findPairWithWMass (v_f_quarks) ;

      if (Wpair.first > 3 || Wpair.second > 3)
        {
          cout << "warning, wrong quarks in W determination\n" ;
        }

      lorentzVector total = (v_f_leptons.at (0) + v_f_neutrinos.at (0)) + 
                             (v_f_quarks.at (Wpair.first) + v_f_quarks.at (Wpair.second)) ;

      //PG the scale:
      Histos.m_h_scale->Fill (scale) ;

      Histos.m_h_MWW->Fill (total.M (), weight) ;
      ++events ;
      if (max > 0 && max < events) 
        {
          cout << max << " events reached, exiting" << endl ;
          break ;
        }

    } //PG loop over input events

  Histos.norm (totalCount) ;

  return totalCount ;
  
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

  const int SUBSET = 0 ;
  const string NAME = "cteq6ll" ; //"cteq6l1"

  LHAPDF::initPDFSet (NAME, LHAPDF::LHPDF, SUBSET) ;
  const int NUMBER = LHAPDF::numberPDF () ;

  LHAPDF::initPDF (0) ;

  if (argc < 3) 
    {
      cout << "usage " << argv[0] << " mass datafile [scale [events]]" << endl ;
      cout << "scale = 1, 0.5, 2, default is 1" << endl ;
      exit (1) ;
    }

  double mass = atof (argv[1]) ;
  if (mass != 350 && mass != 500 && mass != 650 && mass != 800 && mass != 1000)
    {
      cout << "wrong mass: " << mass << endl ;
      exit (1) ;
    }

  inputInfo iInfo[6] ;

  ifstream inputstream ;
  inputstream.open (argv[2]);
  if (!inputstream.is_open ()) 
    {
      cerr << "could not open " << argv[2] << endl ;
      exit (1) ;
    }
//  while (!inputstream.eof ()) 
  for (int i = 0 ; i < 6 ; ++i)
    {
      inputstream >> iInfo[i].mass ;
      inputstream >> iInfo[i].mg_file ;
      inputstream >> iInfo[i].mg_xs ;
      inputstream >> iInfo[i].ph_file ;
      inputstream >> iInfo[i].ph_xs ;
      iInfo[i].print () ;
    }
  inputstream.close () ;
  
  double multiplyScale = 1. ;
  if (argc >= 4)
    {
      multiplyScale = atof (argv[3]) ;
    }

  int maxEventsPerSample = -1 ;
  if (argc == 5)
    {
      int dummy = atoi (argv[4]) ;
      if (dummy > 0) maxEventsPerSample = dummy ;
    }
    
  //PG choose the samples
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ----

  string filename_phbkg = iInfo[0].ph_file ;
  double XS_phbkg = iInfo[0].ph_xs ;

  string filename_mg = "" ;
  double XS_mg = -1 ;
  string filename_phbkgsig = "" ;
  double XS_phbkgsig = -1 ;

  for (int i = 0 ; i < 6 ; ++i)
    {
      if (mass == iInfo[i].mass)
        {
          filename_mg       = iInfo[i].mg_file ;
          XS_mg             = iInfo[i].mg_xs ;
          filename_phbkgsig = iInfo[i].ph_file ;
          XS_phbkgsig       = iInfo[i].ph_xs ;
          break ;
        }
    }

  //PG messages
  
  cout << "\nworking with mass   :\t" << mass << endl ;
  cout << "signal                :\t" << filename_mg << endl ;
  cout << "bkg                   :\t" << filename_phbkg << endl ;
  cout << "S + bkg               :\t" << filename_phbkgsig << "\n" ;
  cout << "multiply the scale by :\t" << multiplyScale << "\n\n" ;

  //PG ---- madgraph ---- signal only
  
  std::ifstream ifs_mg (filename_mg.c_str ()) ;
  LHEF::Reader reader_mg (ifs_mg) ;
  histos H_mg ("mg", XS_mg) ;
  double entries_mg = fillHistos (reader_mg, H_mg, XS_mg, multiplyScale * mass, maxEventsPerSample) ;

  cout << "madgraph events : " << entries_mg << endl ;
  
  //PG ---- phantom ---- background only

  std::ifstream ifs_phbkg (filename_phbkg.c_str ()) ;
  LHEF::Reader reader_phbkg (ifs_phbkg) ;
  histos H_phbkg ("phbkg", XS_phbkg) ;
  double entries_phbkg = fillHistos (reader_phbkg, H_phbkg, XS_phbkg, multiplyScale * mass, maxEventsPerSample) ;

  cout << "phantom bkg events : " << entries_phbkg << endl ;

  //PG ---- phantom ---- background and signal

  std::ifstream ifs_phbkgsig (filename_phbkgsig.c_str ()) ;
  LHEF::Reader reader_phbkgsig (ifs_phbkgsig) ;
  histos H_phbkgsig ("phbkgsig", XS_phbkgsig) ;
  double entries_phbkgsig = fillHistos (reader_phbkgsig, H_phbkgsig, XS_phbkgsig, multiplyScale * mass, maxEventsPerSample) ;

  cout << "phantom bkg+sig events : " << entries_phbkgsig << endl ;

  //PG saving the histograms

  TString name = "findInterference." ;
  name += mass ;
  name += "." ;
  name += multiplyScale ;
  name += ".root" ;
  TFile f (name, "recreate") ;
  H_phbkgsig.save (f) ;
  H_phbkg.save (f) ;
  H_mg.save (f) ;
  f.Close () ;

  cout << "madgraph signal (" << XS_mg << " pb):\n" << filename_mg << "\n" ;
  cout << "phantom signal + bkg (" << XS_phbkgsig << " pb):\n" << filename_phbkgsig << "\n" ;
  cout << "phantom bkg (" << XS_phbkg << " pb):\n" << filename_phbkg << "\n" ;
  cout << "\n produced " << name << endl ;


  return 0 ;
}
