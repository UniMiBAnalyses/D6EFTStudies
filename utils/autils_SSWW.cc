#include "autils.h"
#include "dcutils.h"

#include "Math/Vector4Dfwd.h"
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/Boost.h"
#include "Math/GenVector/VectorUtil.h"
using namespace std ;
using namespace ROOT::Math;


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
      std::cout << variables.at (i) << std::endl;
    }
  //std::cout << variables.size() << std::endl;
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
  // std::cout << ++m_varcounter << " " << m_values.size () << " " << eventWeight << std::endl;
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


vector<pair<int, TLorentzVector>>
findLeptonW (vector<pair<int, TLorentzVector>> v_f_leptons)
{
  vector<pair<int, TLorentzVector>> L ;
  double mZ = 91.1876 ;

  int f0 = v_f_leptons.at(0).first ;
  int f1 = v_f_leptons.at(1).first ;
  int f2 = v_f_leptons.at(2).first ;
  if (abs(f0) == abs(f1) && abs(f0) != abs(f2))
    {
      L.push_back(v_f_leptons.at(2)) ;
      L.push_back(v_f_leptons.at(0)) ;
      L.push_back(v_f_leptons.at(1)) ;
    }
  else if (abs(f0) == abs(f2) && abs(f0) != abs(f1))
    {
      L.push_back(v_f_leptons.at(1)) ;
      L.push_back(v_f_leptons.at(0)) ;
      L.push_back(v_f_leptons.at(2)) ;
    }
  else if (abs(f1) == abs(f2) && abs(f1) != abs(f0))
    {
      L.push_back(v_f_leptons.at(0)) ;
      L.push_back(v_f_leptons.at(1)) ;
      L.push_back(v_f_leptons.at(2)) ;
    }
  else if (abs(f0) == abs(f1) && abs(f0) == abs(f2))
    {
      pair <int, TLorentzVector> os ;
      vector<pair<int, TLorentzVector>> ss ;
      for (int i=0; i<3; i++) 
        {
          if (abs(v_f_leptons.at(i).first) != (f0+f1+f2))
            {
              os = v_f_leptons.at(i) ;
            }
          else
            {
              ss.push_back(v_f_leptons.at(i)) ;
            }
        }
      TLorentzVector v_0 = os.second + ss.at(0).second ;
      TLorentzVector v_1 = os.second + ss.at(1).second ;
      if (abs(v_0.M()-mZ) < abs(v_1.M()-mZ))
        {
          L.push_back(ss.at(1)) ;
          if (ss.at(0).second.Pt() > os.second.Pt())
            {
              L.push_back(ss.at(0)) ;
              L.push_back(os) ;
            }
          else
            {
              L.push_back(os) ;
              L.push_back(ss.at(0)) ;
            }
        }
      else 
        {
          L.push_back(ss.at(0)) ;
          if (ss.at(1).second.Pt() > os.second.Pt())
            {
              L.push_back(ss.at(1)) ;
              L.push_back(os) ;
            }
          else
            {
              L.push_back(os) ;
              L.push_back(ss.at(1)) ;
            }
        }
    }
  return L;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double
findPzneutrino (TLorentzVector lep, TLorentzVector nu)
{
  double Pz_nu ;
  double mW = 80.385 ;
  double Pl = sqrt(pow(lep.Px(),2) + pow(lep.Py(),2) + pow(lep.Pz(),2)) ;
  double a = sqrt(pow(nu.Px(),2) + pow(nu.Py(),2)) ;
  double b = lep.Pz() / Pl ;
  double c = (pow(mW,2) + 2.*(nu.Px()*lep.Px() + nu.Py()*lep.Py())) / (2.*Pl) ;
  double Delta = pow(c,2) + pow(a,2) * (pow(b,2) - 1.) ;
  if (Delta <= 0)
    {
      Pz_nu = -1. * b * c / (pow(b,2) - 1.) ;
    }
  else if (Delta > 0)
    {
      double Pz_nu_1 = (-1.*b*c + sqrt(Delta)) / (pow(b,2) - 1.) ;
      double Pz_nu_2 = (-1.*b*c - sqrt(Delta)) / (pow(b,2) - 1.) ;
      if (abs(Pz_nu_1 - lep.Pz()) < abs(Pz_nu_2 - lep.Pz()))
        {
          Pz_nu = Pz_nu_1 ;
        }
      else if (abs(Pz_nu_1 - lep.Pz()) > abs(Pz_nu_2 - lep.Pz()))
        {
          Pz_nu = Pz_nu_2 ;
        }
      if (Pz_nu > 300)
        {
          Pz_nu = min(abs(Pz_nu_1), abs(Pz_nu_2)) ;
        }
      // cout << "Pz nu = " << Pz_nu << "    1 = " << Pz_nu_1 << "   2 = " << Pz_nu_2 << "       true = " << nu.Pz() << endl ;
    }
  return Pz_nu ; 
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TLorentzVector
RecoNeutrino (TLorentzVector v_l_W, TLorentzVector v_f_nu)
{
  double pz_nu_W = findPzneutrino (v_l_W, v_f_nu) ;
  double E_nu = sqrt(pow(v_f_nu.Px(),2) + pow(v_f_nu.Py(),2) + pow(pz_nu_W,2)) ;
  TLorentzVector nu_reco = TLorentzVector(v_f_nu.Px(), v_f_nu.Py(), pz_nu_W, E_nu) ;
  return nu_reco ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector <double>
WZangles (TLorentzVector w, TLorentzVector z, TLorentzVector l_w, TLorentzVector l_z)
{
  vector <double> p ;
  PtEtaPhiMVector W {w.Pt(), w.Eta(), w.Phi(), w.M()} ;
  PtEtaPhiMVector Z {z.Pt(), z.Eta(), z.Phi(), z.M()} ;
  PtEtaPhiMVector L_W {l_w.Pt(), l_w.Eta(), l_w.Phi(), l_w.M()} ;
  PtEtaPhiMVector L_Z {l_z.Pt(), l_z.Eta(), l_z.Phi(), l_z.M()} ;
  PtEtaPhiMVector WZsyst = W + Z ;
  Boost WZboost {WZsyst.BoostToCM()} ;
  Boost Wboost  {W.BoostToCM()} ;
  Boost Zboost  {Z.BoostToCM()} ;

  PtEtaPhiMVector W_WZcm = WZboost(W) ;
  PtEtaPhiMVector Z_WZcm = WZboost(Z) ;
  PtEtaPhiMVector L_W_Wcm = Wboost(L_W) ;
  PtEtaPhiMVector L_Z_Zcm = Zboost(L_Z) ;
  auto W_plane = L_W_Wcm.Vect().Cross(W) ;
  auto Z_plane = L_Z_Zcm.Vect().Cross(Z) ;

  p.push_back(abs(VectorUtil::DeltaPhi(W_plane, Z_plane))) ;
  p.push_back(W_WZcm.Theta()) ;
  p.push_back(L_W_Wcm.Theta()) ;
  p.push_back(L_Z_Zcm.Theta()) ;

  return p;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool largerPt (const pair <int, TLorentzVector>& i, const pair <int, TLorentzVector>& j)
{
  return (i.second.Pt () < j.second.Pt ()) ;
}

bool largerPtJets ( TLorentzVector& i,  TLorentzVector& j)
{
  return (i.Pt () < j.Pt ()) ;
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

      float ptj1 = -1;
      float ptj2 = -1;
      float ptj3 = -1;
      float ptj4 = -1;

      float pxj1 = -1;
      float pyj1 = -1;
      float pzj1 = -1;
      float etaj1 = -1;
      float phij1 = -1;
      float Ej1 = -1;

      float pxj2 = -1;
      float pyj2 = -1;
      float pzj2 = -1;
      float etaj2 = -1;
      float phij2 = -1;
      float Ej2 = -1;

      float pxj3 = -1;
      float pyj3 = -1;
      float pzj3 = -1;
      float etaj3 = -1;
      float phij3 = -1;
      float Ej3 = -1;

      float pxj4 = -1;
      float pyj4 = -1;
      float pzj4 = -1;
      float etaj4 = -1;
      float phij4 = -1;
      float Ej4 = -1;


      //cout << ">>> FillNtuple:: this is the jets' size ::  " << v_f_jets.size() << endl;
      //cout << "                 these are from quarks ::  " << v_f_quarks.size() << endl;
      //cout << "                 these are from gluons ::  " << v_f_gluons.size() << endl;

      sort (v_f_jets.begin (), v_f_jets.end (), largerPtJets ) ; 
       
      if (v_f_jets.size () >= 1)
        {
          //cout << ">>> FillNtuple:: filling 1st jet variables ..." << endl;
          ptj1  = v_f_jets.at (0).Pt () ;
          etaj1 = v_f_jets.at (0).Eta () ;
          phij1 = v_f_jets.at (0).Phi () ;

          pxj1 = v_f_jets.at (0).Px ();
          pyj1 = v_f_jets.at (0).Py ();
          pzj1 = v_f_jets.at (0).Pz ();
          Ej1 = v_f_jets.at (0).E ();
          

          // cout << ">>>>>> ptj1 ::  " << ptj1  << endl;
          //cout << ">>>>>> etaj1 ::  " << etaj1  << endl;
          //cout << ">>>>>> phij1 ::  " << phij1  << endl;
        }

      float mjj = -1. ;
      float mV = -1;
      float mVBS = -1;

      float phi_vbs_1 = -1;
      float phi_vbs_2 = -1;
      float eta_vbs_1 = -1;
      float eta_vbs_2 = -1;
      float pt_vbs_1 = -1;
      float pt_vbs_2 = -1;

      if (v_f_jets.size () >= 2)
        { 
          //cout << ">>> FillNtuple:: filling 2nd quark variables..." << endl;
          ptj2  = v_f_jets.at (1).Pt () ;
          etaj2 = v_f_jets.at (1).Eta () ;
          phij2 = v_f_jets.at (1).Phi () ;

          pxj2 = v_f_jets.at (1).Px ();
          pyj2 = v_f_jets.at (1).Py ();
          pzj2 = v_f_jets.at (1).Pz ();
          Ej2 = v_f_jets.at (1).E ();

          //cout << ">>>>>> ptj2 ::  " << ptj2  << endl;
          //cout << ">>>>>> etaj2 ::  " << etaj2  << endl;
          //cout << ">>>>>> phij2 ::  " << phij2  << endl;

          // if (ptj1 < ptj2) 
          //   {
          //     swap (ptj1, ptj2) ;
          //     swap (etaj1, etaj2) ;
          //     swap (phij1, phij2) ;
          //   }

          TLorentzVector v_jj = v_f_jets.at (0) + v_f_jets.at (1) ;
          mjj = v_jj.M () ;
          //cout << ">>>>>> mjj ::  " << mjj  << endl;
        }

      if (v_f_jets.size () >= 3){

          ptj3  = v_f_jets.at (2).Pt () ;
          etaj3 = v_f_jets.at (2).Eta () ;
          phij3 = v_f_jets.at (2).Phi () ;

          pxj3 = v_f_jets.at (2).Px ();
          pyj3 = v_f_jets.at (2).Py ();
          pzj3 = v_f_jets.at (2).Pz ();
          Ej3 = v_f_jets.at (2).E ();
      }

      if (v_f_jets.size () >= 4){
       
         ptj4  = v_f_jets.at (3).Pt () ;
         etaj4 = v_f_jets.at (3).Eta () ;
         phij4 = v_f_jets.at (3).Phi () ;
      
         pxj4 = v_f_jets.at (3).Px ();
         pyj4 = v_f_jets.at (3).Py ();
         pzj4 = v_f_jets.at (3).Pz ();
         Ej4 = v_f_jets.at (3).E ();

         int v_idx_1 = 0;
         int v_idx_2 = 0;
         float closest = 1e9;
         float min_mjj = 1e9;
         float min_drjj = 1e9;
         float min_drjl = 1e9;

         for (int vjet_idx = 0; vjet_idx < 3; ++vjet_idx){
            for (int vjet_idx2 = vjet_idx+1; vjet_idx2 < 4; ++vjet_idx2){
               TLorentzVector v_jj_tmp = v_f_jets.at (vjet_idx) + v_f_jets.at (vjet_idx2) ;
               float z_or_w = (fabs(v_jj_tmp.M () - 80.377) < fabs(v_jj_tmp.M () - 91.1876))*fabs(v_jj_tmp.M () - 80.377) + (fabs(v_jj_tmp.M () - 80.377) > fabs(v_jj_tmp.M () - 91.1876))*fabs(v_jj_tmp.M () - 91.1876);
               if (z_or_w < closest){
                  closest = z_or_w;
                  v_idx_1 = vjet_idx;
                  v_idx_2 = vjet_idx2;
               } 
               // save min invariant mass               
               if (v_jj_tmp.M () < min_mjj) min_mjj = v_jj_tmp.M ();

               // save min drjj
               float drjj = sqrt( (v_f_jets.at (vjet_idx).Eta() - v_f_jets.at (vjet_idx2).Eta())*(v_f_jets.at (vjet_idx).Eta() - v_f_jets.at (vjet_idx2).Eta()) + (v_f_jets.at (vjet_idx).Phi() - v_f_jets.at (vjet_idx2).Phi())*(v_f_jets.at (vjet_idx).Phi() - v_f_jets.at (vjet_idx2).Phi())     );
               if (drjj < min_drjj) min_drjj = drjj;             
            }

         }
        
         for (int lep_idx = 0; lep_idx < v_f_leptons.size(); ++lep_idx){
            for(int jet_idx = 0; jet_idx < v_f_jets.size(); ++jet_idx){
        
               float drjl = sqrt(pow(v_f_leptons.at(lep_idx).second.Eta() - v_f_jets.at (jet_idx).Eta(), 2) + pow(v_f_leptons.at(lep_idx).second.Phi() - v_f_jets.at (jet_idx).Phi() , 2));
               if (drjl < min_drjl) min_drjl = drjl;
            }
         
         }
         
         TLorentzVector v_jj_tmp = v_f_jets.at (v_idx_1) + v_f_jets.at (v_idx_2) ; 
         mV = v_jj_tmp.M();
    
         std::vector<int> v_index = {};
         for (int idx=0; idx < 4; idx++){
            if (idx != v_idx_1 && idx != v_idx_2){
               v_index.push_back(idx);
            }
         }
     
         
         TLorentzVector vbs_jj_tmp = v_f_jets.at (v_index.at(0)) + v_f_jets.at (v_index.at(1)) ; 
         mVBS = vbs_jj_tmp.M ();
         
         Ntuple.setvalue ("mV", mV) ;
         Ntuple.setvalue ("mVBS", mVBS) ;
         Ntuple.setvalue ("minmjj", min_mjj) ;
         Ntuple.setvalue ("mindrjj", min_drjj) ;
         Ntuple.setvalue ("mindrjl", min_drjl) ;
         Ntuple.setvalue ("phi_vbs_1", v_f_jets.at (v_index.at(0)).Phi ()) ;
         Ntuple.setvalue ("phi_vbs_2", v_f_jets.at (v_index.at(1)).Phi ()) ;
         Ntuple.setvalue ("eta_vbs_1", v_f_jets.at (v_index.at(0)).Eta ()) ;
         Ntuple.setvalue ("eta_vbs_2", v_f_jets.at (v_index.at(1)).Eta ()) ;
         Ntuple.setvalue ("pt_vbs_1", v_f_jets.at (v_index.at(0)).Pt ()) ;
         Ntuple.setvalue ("pt_vbs_2", v_f_jets.at (v_index.at(1)).Pt ()) ;
      }

      Ntuple.setvalue ("pxj1", pxj1) ;
      Ntuple.setvalue ("pyj1", pyj1) ;
      Ntuple.setvalue ("pzj1", pzj1) ;
      Ntuple.setvalue ("Ej1", Ej1) ;
      Ntuple.setvalue ("etaj1", etaj1) ;
      Ntuple.setvalue ("phij1", phij1) ;
      Ntuple.setvalue ("pxj2", pxj2) ;
      Ntuple.setvalue ("pyj2", pyj2) ;
      Ntuple.setvalue ("pzj2", pzj2) ;
      Ntuple.setvalue ("Ej2", Ej2) ;
      Ntuple.setvalue ("etaj2", etaj2) ;
      Ntuple.setvalue ("phij2", phij2) ;
      Ntuple.setvalue ("pxj3", pxj3) ;
      Ntuple.setvalue ("pyj3", pyj3) ;
      Ntuple.setvalue ("pzj3", pzj3) ;
      Ntuple.setvalue ("Ej3", Ej3) ;
      Ntuple.setvalue ("etaj3", etaj3) ;
      Ntuple.setvalue ("phij3", phij3) ;
      Ntuple.setvalue ("pxj4", pxj4) ;
      Ntuple.setvalue ("pyj4", pyj4) ;
      Ntuple.setvalue ("pzj4", pzj4) ;
      Ntuple.setvalue ("Ej4", Ej4) ;
      Ntuple.setvalue ("etaj4", etaj4) ;
      Ntuple.setvalue ("phij4", phij4) ;

      Ntuple.setvalue ("ptj1", ptj1) ;
      Ntuple.setvalue ("ptj2", ptj2) ;
      Ntuple.setvalue ("ptj3", ptj3) ;
      Ntuple.setvalue ("ptj4", ptj4) ;
      Ntuple.setvalue ("mjj", mjj) ;


      TLorentzVector v_ll ;
      TLorentzVector ME ;



      float pxv1 = -1;
      float pyv1 = -1;
      float pzv1 = -1;
      float etav1 = -1;
      float phiv1 = -1;
      float Ev1 = -1;
      float ptv1 = -1;

      float pxv2 = -1;
      float pyv2 = -1;
      float pzv2 = -1;
      float etav2 = -1;
      float phiv2 = -1;
      float Ev2 = -1;
      float ptv2 = -1;



      sort (v_f_neutrinos.begin (), v_f_neutrinos.end (), largerPt ) ;

      if (v_f_neutrinos.size () >= 1)
        {
          ptv1  = v_f_neutrinos.at (0).second.Pt () ;
          etav1 = v_f_neutrinos.at (0).second.Eta () ;
          phiv1 = v_f_neutrinos.at (0).second.Phi () ;

          pxv1 = v_f_neutrinos.at (0).second.Px ();
          pyv1 = v_f_neutrinos.at (0).second.Py ();
          pzv1 = v_f_neutrinos.at (0).second.Pz ();
          Ev1 = v_f_neutrinos.at (0).second.E ();
        }

      if (v_f_neutrinos.size () >= 2)
        {
          ptv2  = v_f_neutrinos.at (1).second.Pt () ;
          etav2 = v_f_neutrinos.at (1).second.Eta () ;
          phiv2 = v_f_neutrinos.at (1).second.Phi () ;

          pxv2 = v_f_neutrinos.at (1).second.Px ();
          pyv2 = v_f_neutrinos.at (1).second.Py ();
          pzv2 = v_f_neutrinos.at (1).second.Pz ();
          Ev2 = v_f_neutrinos.at (1).second.E ();
        }

      Ntuple.setvalue ("pxv1", pxv1) ;
      Ntuple.setvalue ("pyv1", pyv1) ;
      Ntuple.setvalue ("pzv1", pzv1) ;
      Ntuple.setvalue ("Ev1", Ev1) ;
      Ntuple.setvalue ("etav1", etav1) ;
      Ntuple.setvalue ("phiv1", phiv1) ;
      Ntuple.setvalue ("pxv2", pxv2) ;
      Ntuple.setvalue ("pyv2", pyv2) ;
      Ntuple.setvalue ("pzv2", pzv2) ;
      Ntuple.setvalue ("Ev2", Ev2) ;
      Ntuple.setvalue ("etav2", etav2) ;
      Ntuple.setvalue ("phiv2", phiv2) ;

      Ntuple.setvalue ("ptv1", ptv1) ;
      Ntuple.setvalue ("ptv2", ptv2) ;

      for (int inu = 0 ; inu < v_f_neutrinos.size () ; inu++)
        {
          ME += v_f_neutrinos.at (inu) .second ;
        }

      for (int ilep = 0 ; ilep < v_f_leptons.size () ; ilep++)
        {
          v_ll += v_f_leptons.at (ilep) .second ;
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
      
      Ntuple.setvalue ("mll", v_ll.M ()) ;
      Ntuple.setvalue ("ptll", v_ll.Pt ()) ;
      Ntuple.setvalue ("met", ME.Pt ()) ;
      Ntuple.setvalue ("metphi", ME.Phi ()) ;
      Ntuple.setvalue ("meteta", ME.Eta ()) ;

// Ordering v_f_Leptons according to Pt.

      float pxl1 = -1;
      float pyl1 = -1;
      float pzl1 = -1;
      float etal1 = -1;
      float phil1 = -1;
      float El1 = -1;
      float ptl1 = -1;

      float pxl2 = -1;
      float pyl2 = -1;
      float pzl2 = -1;
      float etal2 = -1;
      float phil2 = -1;
      float El2 = -1;
      float ptl2 = -1;

      sort (v_f_leptons.begin (), v_f_leptons.end (), largerPt ) ;   

      if (v_f_leptons.size () >= 1)
        {
          ptl1  = v_f_leptons.at (0).second.Pt () ;
          etal1 = v_f_leptons.at (0).second.Eta () ;
          phil1 = v_f_leptons.at (0).second.Phi () ;

          pxl1 = v_f_leptons.at (0).second.Px ();
          pyl1 = v_f_leptons.at (0).second.Py ();
          pzl1 = v_f_leptons.at (0).second.Pz ();
          El1 = v_f_leptons.at (0).second.E ();
        }

      if (v_f_leptons.size () >= 2)
        { 
          ptl2  = v_f_leptons.at (1).second.Pt () ;
          etal2 = v_f_leptons.at (1).second.Eta () ;
          phil2 = v_f_leptons.at (1).second.Phi () ;

          pxl2 = v_f_leptons.at (1).second.Px ();
          pyl2 = v_f_leptons.at (1).second.Py ();
          pzl2 = v_f_leptons.at (1).second.Pz ();
          El2 = v_f_leptons.at (1).second.E ();
        }

      
      Ntuple.setvalue ("pxl1", pxl1) ;
      Ntuple.setvalue ("pyl1", pyl1) ;
      Ntuple.setvalue ("pzl1", pzl1) ;
      Ntuple.setvalue ("El1", El1) ;
      Ntuple.setvalue ("etal1", etal1) ;
      Ntuple.setvalue ("phil1", phil1) ;
      Ntuple.setvalue ("pxl2", pxl2) ;
      Ntuple.setvalue ("pyl2", pyl2) ;
      Ntuple.setvalue ("pzl2", pzl2) ;
      Ntuple.setvalue ("El2", El2) ;
      Ntuple.setvalue ("etal2", etal2) ;
      Ntuple.setvalue ("phil2", phil2) ;

      Ntuple.setvalue ("ptl1", ptl1) ;
      Ntuple.setvalue ("ptl2", ptl2) ;
      

      // save leptons and nu pt in order wrt W boson mother
      // Use algorithm closest mass to W 
  
      if (v_f_leptons.size () >= 2 && v_f_neutrinos.size() >= 2)
         {
            float pxl_W1 = -1;
            float pyl_W1 = -1;
            float pzl_W1 = -1;
            float etal_W1 = -1;
            float phil_W1 = -1;
            float El_W1 = -1;
            float ptl_W1 = -1;
            int   idl_W1 = -1;

            float pxv_W1 = -1;
            float pyv_W1 = -1;
            float pzv_W1 = -1;
            float etav_W1 = -1;
            float phiv_W1 = -1;
            float Ev_W1 = -1;
            float ptv_W1 = -1;
            int   idv_W1 = -1;

            float pxl_W2 = -1;
            float pyl_W2 = -1;
            float pzl_W2 = -1;
            float etal_W2 = -1;
            float phil_W2 = -1;
            float El_W2 = -1;
            float ptl_W2 = -1;
            int   idl_W2 = -1;
  
            float pxv_W2 = -1;
            float pyv_W2 = -1;
            float pzv_W2 = -1;
            float etav_W2 = -1;
            float phiv_W2 = -1;
            float Ev_W2 = -1;
            float ptv_W2 = -1;
            int   idv_W2 = -1;


            float mW1 = -1;
            float mW2 = -1;

            float closest_mW = 1e9;
            int lep_idx = 0;
            int nu_idx = 0;  
            int lep_idx_2 = 0;
            int nu_idx_2 = 0;
    
            for (int l_idx = 0; l_idx < v_f_leptons.size (); ++l_idx){
               for (int n_idx = 0; n_idx < v_f_neutrinos.size(); ++n_idx){

                  int l_idx_2 = 0;
                  int n_idx_2 = 0;

                  for (int i_ = 0; i_ < v_f_leptons.size (); ++i_){
                     if (i_ != l_idx) l_idx_2 = i_;
                  }

                  for (int j_ = 0; j_ < v_f_neutrinos.size (); ++j_){
                     if (j_ != n_idx) n_idx_2 = j_;
                  }

                  TLorentzVector v_lv1_tmp = v_f_leptons.at (l_idx).second + v_f_neutrinos.at (n_idx).second ;
                  TLorentzVector v_lv2_tmp = v_f_leptons.at (l_idx_2).second + v_f_neutrinos.at (n_idx_2).second ;

                  float distance_w = fabs(v_lv1_tmp.M () - 80.377) + fabs(v_lv2_tmp.M () - 80.377) ;
                  if (distance_w < closest_mW){
                     closest_mW = distance_w;
                     lep_idx = l_idx;
                     nu_idx = n_idx;
                     lep_idx_2 = l_idx_2;
                     nu_idx_2 = n_idx_2;
                  }

               }

            } 



            pxl_W1 = v_f_leptons.at (lep_idx).second.Px();
            pyl_W1 = v_f_leptons.at (lep_idx).second.Py();
            pzl_W1 = v_f_leptons.at (lep_idx).second.Pz();
            El_W1 = v_f_leptons.at (lep_idx).second.E();
            etal_W1 = v_f_leptons.at (lep_idx).second.Eta();
            phil_W1 = v_f_leptons.at (lep_idx).second.Phi();
            ptl_W1 = v_f_leptons.at (lep_idx).second.Pt();
            idl_W1 = v_f_leptons.at (lep_idx).first;

            pxl_W2 = v_f_leptons.at (lep_idx_2).second.Px();
            pyl_W2 = v_f_leptons.at (lep_idx_2).second.Py();
            pzl_W2 = v_f_leptons.at (lep_idx_2).second.Pz();
            El_W2 = v_f_leptons.at (lep_idx_2).second.E();
            etal_W2 = v_f_leptons.at (lep_idx_2).second.Eta();
            phil_W2 = v_f_leptons.at (lep_idx_2).second.Phi();
            ptl_W2 = v_f_leptons.at (lep_idx_2).second.Pt();
            idl_W2 = v_f_leptons.at (lep_idx_2).first;

            pxv_W1 = v_f_neutrinos.at (nu_idx).second.Px();
            pyv_W1 = v_f_neutrinos.at (nu_idx).second.Py();
            pzv_W1 = v_f_neutrinos.at (nu_idx).second.Pz();
            Ev_W1 = v_f_neutrinos.at (nu_idx).second.E();
            etav_W1 = v_f_neutrinos.at (nu_idx).second.Eta();
            phiv_W1 = v_f_neutrinos.at (nu_idx).second.Phi();
            ptv_W1 = v_f_neutrinos.at (nu_idx).second.Pt();
            idv_W1 = v_f_neutrinos.at (nu_idx).first;

            pxv_W2 = v_f_neutrinos.at (nu_idx_2).second.Px();
            pyv_W2 = v_f_neutrinos.at (nu_idx_2).second.Py();
            pzv_W2 = v_f_neutrinos.at (nu_idx_2).second.Pz();
            Ev_W2 = v_f_neutrinos.at (nu_idx_2).second.E();
            etav_W2 = v_f_neutrinos.at (nu_idx_2).second.Eta();
            phiv_W2 = v_f_neutrinos.at (nu_idx_2).second.Phi();
            ptv_W2 = v_f_neutrinos.at (nu_idx_2).second.Pt();
            idv_W2 = v_f_neutrinos.at (nu_idx_2).first;
            


            TLorentzVector v_lv_1 = v_f_leptons.at (lep_idx).second + v_f_neutrinos.at (nu_idx).second ;
            TLorentzVector v_lv_2 = v_f_leptons.at (lep_idx_2).second + v_f_neutrinos.at (nu_idx_2).second ;
    
            mW1 = v_lv_1.M ();
            mW2 = v_lv_2.M ();

            Ntuple.setvalue ("pxl_W1", pxl_W1) ;
            Ntuple.setvalue ("pyl_W1", pyl_W1) ;
            Ntuple.setvalue ("pzl_W1", pzl_W1) ;
            Ntuple.setvalue ("El_W1", El_W1) ;
            Ntuple.setvalue ("etal_W1", etal_W1) ;
            Ntuple.setvalue ("phil_W1", phil_W1) ;
            Ntuple.setvalue ("ptl_W1", ptl_W1) ;
            Ntuple.setvalue ("idl_W1", idl_W1) ;         

            Ntuple.setvalue ("pxl_W2", pxl_W2) ;
            Ntuple.setvalue ("pyl_W2", pyl_W2) ;
            Ntuple.setvalue ("pzl_W2", pzl_W2) ;
            Ntuple.setvalue ("El_W2", El_W2) ;
            Ntuple.setvalue ("etal_W2", etal_W2) ;
            Ntuple.setvalue ("phil_W2", phil_W2) ;
            Ntuple.setvalue ("ptl_W2", ptl_W2) ; 
            Ntuple.setvalue ("idl_W2", idl_W2) ;

            Ntuple.setvalue ("pxv_W1", pxv_W1) ;
            Ntuple.setvalue ("pyv_W1", pyv_W1) ;
            Ntuple.setvalue ("pzv_W1", pzv_W1) ;
            Ntuple.setvalue ("Ev_W1", Ev_W1) ;
            Ntuple.setvalue ("etav_W1", etav_W1) ;
            Ntuple.setvalue ("phiv_W1", phiv_W1) ;
            Ntuple.setvalue ("ptv_W1", ptv_W1) ;
            Ntuple.setvalue ("idv_W1", idv_W1) ; 

            Ntuple.setvalue ("pxv_W2", pxv_W2) ;
            Ntuple.setvalue ("pyv_W2", pyv_W2) ;
            Ntuple.setvalue ("pzv_W2", pzv_W2) ;
            Ntuple.setvalue ("Ev_W2", Ev_W2) ;
            Ntuple.setvalue ("etav_W2", etav_W2) ;
            Ntuple.setvalue ("phiv_W2", phiv_W2) ;
            Ntuple.setvalue ("ptv_W2", ptv_W2) ;
            Ntuple.setvalue ("idv_W2", idv_W2) ;      

            Ntuple.setvalue ("mW1", mW1) ;
            Ntuple.setvalue ("mW2", mW2) ;
      }
      // Filling all weights:

      
      std::vector<std::string> wnames = {};
      for(int wgt_idx = 0; wgt_idx < wnames.size(); ++wgt_idx){
   	// std::cout << wgt_idx << " " << wnames.at(wgt_idx) << " " << wnames.size() << std::endl; 
        Ntuple.setvalue(wnames.at(wgt_idx), reader.hepeup.namedweights[wgt_idx].weights[0]);
      }  

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
