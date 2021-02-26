
#include "autils.h"
#include "dcutils.h"

using namespace std ;


histos::histos (TString name, double XS, double lumi) : 
  m_name (name), 
  m_XS (XS),
  m_lumi (lumi)
  {
    makeHisto ("m_mjj", 48, 200., 5000.) ;
    makeHisto ("m_mll", 15, 0., 1500.) ;
    makeHisto ("m_ptl1", 15, 0., 1500.) ;
    makeHisto ("m_ptl2", 15, 0., 1500.) ;
    makeHisto ("m_met", 20, 0., 2000.) ;
  }

// simplify histogram creation
TH1F * histos::makeHisto (const TString & varname, int nBins, float min, float max)
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

// to be called at each event, before any selections, 
// for a proper normalisation of the histograms
double histos::increaseNorm (double eventweight) 
  {
     m_norm += eventweight ;
     return m_norm ;
  }
 

// fill the histograms through the map
void histos::fill (string varname, double value, double weight)
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
void histos::norm ()
  {
    double factor = m_lumi * m_XS / m_norm ;
    for (map<string, TH1F *>::iterator it = m_histograms.begin () ; 
         it != m_histograms.end () ; ++it)
       it->second->Scale (factor) ;
    return ; 
  }

histos::~histos ()
  {
    // this being empty is not very nice, to be replaced with the unique_ptr or auto_ptr thing 
    // for the histogram pointers, but I need to find a smart way to do it
    // w/o need for implementing it for each histogram I add      
  }  
  
void histos::save (TFile & outfile) 
  {
    outfile.cd () ;
    for (map<string, TH1F *>::iterator it = m_histograms.begin () ; 
         it != m_histograms.end () ; ++it)
      it->second->Write () ;
    return ;
  }
  

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// Fill the histograms for a single sample
// histograms will not get normalised, since the same sample
// could be split in several LHE files and this function
// may get called several times, one for each LHE file.
// Therefore, the normalisation will have to be called afterwards
double 
fillHistos (LHEF::Reader & reader, histos & Histos, int max)
{
  int events = 0 ;
   
  //PG loop over input events
  while (reader.readEvent ()) 
    {
//      if ( reader.outsideBlock.length() ) std::cout << reader.outsideBlock;

      if (events++ % 10000 == 0) cout << "        reading event in file: " << events << endl ;
          
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
      // if (v_f_leptons.size () < 2)
      //   {
      //     cout << "warning, not enough leptons" << endl ;
      //     ++warnNum ;
      //   }
      // if (v_f_neutrinos.size () < 2)
      //   {
      //     cout << "warning, not enough neutrinos" << endl ;
      //     ++warnNum ;
      //   }
      if (warnNum > 0) continue ;

      //PG apply basic selections
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      float weight = reader.hepeup.XWGTUP ;
      Histos.increaseNorm (weight) ;

      if (v_f_quarks.at (0).Pt () < 30) continue ;
      if (v_f_quarks.at (1).Pt () < 30) continue ;

      TLorentzVector ME = v_f_neutrinos.at (0) + v_f_neutrinos.at (1) ;
      if (ME.Pt () < 40) continue ;

      TLorentzVector v_jj = v_f_quarks.at (0) + v_f_quarks.at (1) ;
      if (fabs (v_f_quarks.at (0).Eta () - v_f_quarks.at (1).Eta ()) < 2.5 ) continue ;
      if (v_jj.M () < 500) continue ;

      TLorentzVector v_ll = v_f_leptons.at (0) + v_f_leptons.at (1) ;
      if (v_ll.M () < 20) continue ;
      if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (0).Eta ())) > 0.75) continue ;
      if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (1).Eta ())) > 0.75) continue ;

      //PG fill histograms
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
//      Histos.m_mjj->Fill (v_jj.M (), reader.hepeup.XWGTUP) ;
//      Histos.m_mll->Fill (v_ll.M (), reader.hepeup.XWGTUP) ;
      Histos.fill ("m_mjj", v_jj.M (), weight) ;
      Histos.fill ("m_mll", v_ll.M (), weight) ;

      float ptl1 = v_f_leptons.at (0).Pt () ;
      float ptl2 = v_f_leptons.at (1).Pt () ;

      if (ptl1 < ptl2) swap (ptl1, ptl2) ;

      Histos.fill ("m_ptl1", ptl1, weight) ;
      Histos.fill ("m_ptl2", ptl2, weight) ;

      Histos.fill ("m_met", ME.Pt (), weight) ;

      if (max > 0 && max < events) 
        {
          cout << max << " events reached, exiting" << endl ;
          break ;
        }

    } //PG loop over input events

  return events ;
  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


ntuple::ntuple (std::vector<std::string> variables, TString name, double XS, double lumi) :
  m_values (variables.size () + 1., 0.),
  m_varcounter (0),
  m_nums (new TH1F (name + "_nums", "global numbers", 3, 0, 3)), 
  m_total (0.),
  m_selected (0.)
{
  sort (variables.begin (), variables.end ()) ;
  string listForNtuple ;
  //PG prepare TNtuple definition string 
  //PG and remove spaces from strings and prepare TNtuple definition string
  //PG FIXME how do I preserve the sorting of the cfg file?
  //PG I don't: just use alphabetical ordering
  for (int i = 0 ; i < variables.size () ; ++i)
    {
      // remove spaces 
      variables.at (i).erase (
          remove (variables.at (i).begin (), variables.at (i).end (), ' '), 
          variables.at (i).end ()
        ) ;
      listForNtuple += variables.at (i) + ":" ;
      varpos[variables.at (i)] = i ;
    }
  // the last element is the event weight  
  listForNtuple += "w" ;
  varpos["w"] = variables.size () ;
  m_it = varpos.end () ;
  m_ntuple = new TNtuple (name, name, listForNtuple.c_str ()) ;
  m_nums->SetBinContent (1, XS) ;
}
  

/**
 This function retains the value only if it's requested
 in the initial list of variables.
 In this way, in the fill function the setvalue is always called,
 as anyways the test should be done in one place or another.
*/
void ntuple::setvalue (std::string varname, double value)
{
//FIXME
  m_it = varpos.find (varname) ;
  if (m_it != varpos.end ())
    {
      m_values.at (m_it->second) = value ;
      ++m_varcounter ; //PG this is a sloppy check, as it's a global counting only
    }
  return ;
}


// transfer values in the ntuple
void ntuple::fill (double eventWeight)
{
  if (++m_varcounter != m_values.size ()) 
    {
      cout << "WARNING not all variables filled" << endl ;
    }
  m_values.back () = eventWeight ;
  m_ntuple->Fill (&m_values[0]) ;
  m_varcounter = 0 ;
  m_selected += eventWeight ;
//  reset (m_values) ;
  return ;
}


// to be called at each event, before any selections, 
// for a proper normalisation of the histograms
double ntuple::increaseNorm (double eventweight) 
  {
     m_total += eventweight ;
     return m_total ;
  }
 

// save ntuples in the outfile
void ntuple::save (TFile & outfile)
  {
    m_nums->SetBinContent (2, m_total) ;
    m_nums->SetBinContent (3, m_selected) ;
    outfile.cd () ;
    m_ntuple->Write () ;
    m_nums->Write () ;
    return ;
  }


//dtor
ntuple::~ntuple () 
{
  //PG for the time being no destructions
  //PG since objects are allocated dinamically
  //PG and when filling containers one should properly
  //PG pass around the ownership of pointers
} 


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


float deltaPhi (float phi1, float phi2)
{
  float delta = abs (phi1 - phi2) ;
  if (delta > M_PI) delta = 2. * M_PI - delta ;
  return delta ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool largerPt (const pair <int, TLorentzVector>& i, const pair <int, TLorentzVector>& j)
{
  return (i.second.Pt () < j.second.Pt ()) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double fillNtuple (LHEF::Reader & reader, ntuple & Ntuple, int max, bool applyCuts)
{
  int events = 0 ;
   
  //PG loop over input events
  while (reader.readEvent ()) 
    {
//      if ( reader.outsideBlock.length() ) std::cout << reader.outsideBlock;

      if (events++ % 10000 == 0) cout << "        reading event in file: " << events << endl ;
          
      vector<TLorentzVector> v_f_Ws ;
      vector<TLorentzVector> v_f_jets ;
      vector<TLorentzVector> v_f_gluons ;
      vector<TLorentzVector> v_f_quarks ;
      vector<pair<int, TLorentzVector>> v_f_leptons ;
      vector<pair<int, TLorentzVector>> v_f_neutrinos ;

      // loop over particles in the event
      for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) 
        {
          // outgoing particles          
          if (reader.hepeup.ISTUP.at (iPart) == 1)
            {
              TLorentzVector dummy = buildLP (reader, iPart) ;
              pair <int, TLorentzVector> p ;
              // quarks-->jets
              if (abs (reader.hepeup.IDUP.at (iPart)) < 7) 
                {
                  v_f_quarks.push_back (dummy) ;
                  v_f_jets.push_back (dummy) ;        
                } // quarks-->jets
              // gluons-->jets
              else if (abs (reader.hepeup.IDUP.at (iPart)) == 21 ) 
                {
                  v_f_gluons.push_back (dummy) ; 
                  v_f_jets.push_back (dummy) ;       
                } // gluons-->jets

              // leptons
              else if (abs (reader.hepeup.IDUP.at (iPart)) == 11 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 13 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 15)
                {
                  p.first = reader.hepeup.IDUP.at (iPart) ;
                  p.second = dummy ;
                  v_f_leptons.push_back (p) ;
                } // leptons
              // neutrinos
              else if (abs (reader.hepeup.IDUP.at (iPart)) == 12 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 14 ||
                       abs (reader.hepeup.IDUP.at (iPart)) == 16)
                {
                  p.first = reader.hepeup.IDUP.at (iPart) ;
                  p.second = dummy ;
                  v_f_neutrinos.push_back (p) ;
                } // neutrinos
            } // outgoing particles
        } // loop over particles in the event

    
      //generic controls
      int warnNum = 0 ;
      //PG commenting these lines, to allow for the code to run also in the inclusve case
      // if (v_f_jets.size () < 2)
      //   {
      //     cout << "warning, not enough jets" << endl ;
      //     ++warnNum ;
      //   }
      // if (v_f_leptons.size () < 2)
      //   {
      //     cout << "warning, not enough leptons" << endl ;
      //     ++warnNum ;
      //   }
      // if (v_f_neutrinos.size () < 2)
      //   {
      //     cout << "warning, not enough neutrinos" << endl ;
      //     ++warnNum ;
      //   }
      if (warnNum > 0) continue ;

      //PG apply basic selections
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      float eventWeight = reader.hepeup.XWGTUP ;
      Ntuple.increaseNorm (eventWeight) ;

      float ptj1  = -1. ;
      float ptj2  = -1. ;
      float etaj1 = -1. ;
      float etaj2 = -1. ;
      float phij1 = -1. ;
      float phij2 = -1. ;

      //cout << ">>> FillNtuple:: this is the jets' size ::  " << v_f_jets.size() << endl;
      //cout << "                 these are from quarks ::  " << v_f_quarks.size() << endl;
      //cout << "                 these are from gluons ::  " << v_f_gluons.size() << endl;
       
      if (v_f_jets.size () >= 1)
        {
          //cout << ">>> FillNtuple:: filling 1st jet variables ..." << endl;
          ptj1  = v_f_jets.at (0).Pt () ;
          etaj1 = v_f_jets.at (0).Eta () ;
          phij1 = v_f_jets.at (0).Phi () ;
          // cout << ">>>>>> ptj1 ::  " << ptj1  << endl;
          //cout << ">>>>>> etaj1 ::  " << etaj1  << endl;
          //cout << ">>>>>> phij1 ::  " << phij1  << endl;
        }

      float mjj = -1. ;
      if (v_f_jets.size () >= 2)
        { 
          //cout << ">>> FillNtuple:: filling 2nd quark variables..." << endl;
          ptj2  = v_f_jets.at (1).Pt () ;
          etaj2 = v_f_jets.at (1).Eta () ;
          phij2 = v_f_jets.at (1).Phi () ;
          //cout << ">>>>>> ptj2 ::  " << ptj2  << endl;
          //cout << ">>>>>> etaj2 ::  " << etaj2  << endl;
          //cout << ">>>>>> phij2 ::  " << phij2  << endl;
          if (ptj1 < ptj2) 
            {
              swap (ptj1, ptj2) ;
              swap (etaj1, etaj2) ;
              swap (phij1, phij2) ;
            }
          TLorentzVector v_jj = v_f_jets.at (0) + v_f_jets.at (1) ;
          mjj = v_jj.M () ;
          //cout << ">>>>>> mjj ::  " << mjj  << endl;
        }

      TLorentzVector v_ll ;
      TLorentzVector v_ee ; 
      TLorentzVector ME ;

      for (int inu = 0 ; inu < v_f_neutrinos.size () ; inu++)
        {
          ME += v_f_neutrinos.at (inu) .second ;
        }

      for (int ilep = 0 ; ilep < v_f_leptons.size () ; ilep++)
        {
          v_ll += v_f_leptons.at (ilep) .second ;
        }
      // Lorentz vector of electrons only for WZ
      for (int ilep = 0 ; ilep < v_f_leptons.size () ; ilep++)
        {
          if (abs(v_f_leptons.at (ilep) .first) == 11)
           {
             v_ee += v_f_leptons.at (ilep) .second ;
           }  
        }

      if (applyCuts)
        {
          if (v_f_jets.size () < 2)
            {
              cerr << "cannot apply VBS selections without two jets, exiting\n" ;
              exit (1) ;
            }
          if (ptj1 < 30) continue ;
          if (ptj2 < 30) continue ;

          if (ME.Pt () < 40) continue ;

          if (fabs (v_f_jets.at (0).Eta () - v_f_jets.at (1).Eta ()) < 2.5 ) continue ;
          if (mjj < 500) continue ;

          if (v_ll.M () < 20) continue ;
          if (fabs (zetaStar (v_f_jets.at (0).Eta (), v_f_jets.at (1).Eta (), v_f_leptons.at (0).second.Eta ())) > 0.75) continue ;
          if (fabs (zetaStar (v_f_jets.at (0).Eta (), v_f_jets.at (1).Eta (), v_f_leptons.at (1).second.Eta ())) > 0.75) continue ;
        }

      //PG fill variables
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      Ntuple.setvalue ("mjj", mjj) ;
      Ntuple.setvalue ("mll", v_ll.M ()) ;
      Ntuple.setvalue ("mee", v_ee.M ()) ;

      Ntuple.setvalue ("ptj1", ptj1) ;
      Ntuple.setvalue ("ptj2", ptj2) ;
      Ntuple.setvalue ("etaj1", etaj1) ;
      Ntuple.setvalue ("etaj2", etaj2) ;
      Ntuple.setvalue ("phij1", phij1) ;
      Ntuple.setvalue ("phij2", phij2) ;
      Ntuple.setvalue ("deltaetajj", fabs (etaj1 - etaj2)) ;
      Ntuple.setvalue ("deltaphijj", deltaPhi (phij1, phij2)) ;

// Ordering v_f_Leptons according to Pt.

      sort (v_f_leptons.begin (), v_f_leptons.end (), largerPt ) ;     

      float ptl = 0 ;
      float etal = 0 ;
      int idl = 0 ;
      for (int l = 0; l < v_f_leptons.size (); ++l)
        {
          ptl = v_f_leptons.at (l).second.Pt () ;
          etal = v_f_leptons.at (l).second.Eta () ;
          idl = v_f_leptons.at (l).first ;
          Ntuple.setvalue (Form("ptl%d", l+1), ptl) ;
          Ntuple.setvalue (Form("etal%d", l+1), etal) ;
          Ntuple.setvalue (Form("idl%d", l+1), idl) ;
        }

      Ntuple.setvalue ("met", ME.Pt ()) ;
      Ntuple.setvalue ("ptll", v_ll.Pt ()) ;
      Ntuple.setvalue ("ptee", v_ee.Pt ()) ;

      Ntuple.fill (eventWeight) ;

      if (max > 0 && max < events) 
        {
          cout << max << " events reached, exiting" << endl ;
          break ;
        }
    } //PG loop over input events

  return events ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void
changeLHEfolder (vector<string> & inputfiles, string newfolder)
{
  for (int i = 0 ; i < inputfiles.size () ; ++i)
    {
      vector<string> fullpath =  split (inputfiles.at (i), '/') ;
      inputfiles.at (i) = newfolder 
                         + "/" + fullpath.at (fullpath.size () - 3)
                         + "/" + fullpath.at (fullpath.size () - 2) 
                         + "/" + fullpath.back () ; 
    }  
  return ;
}




