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
      vector<pair<int, TLorentzVector>> v_f_quarks_pdg ;
      vector<pair<int,int>> v_f_quarks_mothers; 
      vector<pair<int, TLorentzVector>> v_f_leptons ;
      vector<pair<int, TLorentzVector>> v_f_neutrinos ;
      vector<pair<int, TLorentzVector>> v_f_middle_pdg 
;
      int nbQuarks = 0;

      // loop over particles in the event
      for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart){

          int mother_1_idx = reader.hepeup.MOTHUP.at (iPart).first;
          int mother_2_idx = reader.hepeup.MOTHUP.at (iPart).second;

          if (reader.hepeup.ISTUP.at (iPart) > 0){
             mother_1_idx -= 1;
             mother_2_idx -= 1;
          }

          // std::cout << reader.hepeup.IDUP.at (iPart) << " " << reader.hepeup.ISTUP.at (iPart) << " " << mother_1_idx << " " << mother_2_idx << " " << reader.hepeup.IDUP.at (mother_1_idx) << " " << reader.hepeup.IDUP.at (mother_2_idx) << std::endl;
       }

      // loop over particles in the event
      for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) 
        {
          // middle particles
          if (abs(reader.hepeup.ISTUP.at (iPart)) == 2){
             // std::cout << "Middle particle: " <<  reader.hepeup.IDUP.at (iPart) << std::endl;
             TLorentzVector dummy = buildLP (reader, iPart) ;
             pair <int, TLorentzVector> p ;
             p.first = reader.hepeup.IDUP.at (iPart) ;
             p.second = dummy ;  
             v_f_middle_pdg.push_back (p) ;

          }
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
                  p.first = reader.hepeup.IDUP.at (iPart) ;
                  p.second = dummy ;
                  v_f_quarks_pdg.push_back (p) ;

                  pair <int, int> mothers_pdg;
                  // need the -1 on the index because if mother_idx == 0 then it means it does not have a mother
                  mothers_pdg.first = reader.hepeup.IDUP.at (reader.hepeup.MOTHUP.at (iPart).first - 1);
                  mothers_pdg.second = reader.hepeup.IDUP.at (reader.hepeup.MOTHUP.at (iPart).second -1);
                  v_f_quarks_mothers.push_back(mothers_pdg);

                  // increase number of b-jets if this is a bjet
                  if (abs (reader.hepeup.IDUP.at (iPart)) == 5) nbQuarks+= 1; 
      
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

         // std::cout << "Checking pdg ids of final state quarks and mothers" << std::endl;
         // for (int i = 0; i < 4; ++i) std::cout << "PDG: " << v_f_quarks_pdg.at(i).first << " Mother first: " << v_f_quarks_mothers.at(i).first << " Mother second: " << v_f_quarks_mothers.at(i).second << std::endl ;
         

         for (int vjet_idx = 0; vjet_idx < 3; ++vjet_idx){
            for (int vjet_idx2 = vjet_idx+1; vjet_idx2 < 4; ++vjet_idx2){
  
               int pdg_1 = v_f_quarks_pdg.at (vjet_idx).first ;
               int pdg_2 = v_f_quarks_pdg.at (vjet_idx2).first ;

               float charge_1 = 0;
               float charge_2 = 0;

               if (pdg_1 % 2 == 0) charge_1 = 2./3 * ((pdg_1 > 0) ? 1 : ((pdg_1 < 0) ? -1 : 0));
               else charge_1 = -1./3 * ((pdg_1 > 0) ? 1 : ((pdg_1 < 0) ? -1 : 0));

               if (pdg_2 % 2 == 0) charge_2 = 2./3 * ((pdg_2 > 0) ? 1 : ((pdg_2 < 0) ? -1 : 0));
               else charge_2 = -1./3 * ((pdg_2 > 0) ? 1 : ((pdg_2 < 0) ? -1 : 0));

               TLorentzVector v_jj_tmp = v_f_quarks_pdg.at (vjet_idx).second + v_f_quarks_pdg.at (vjet_idx2).second ;

               // save min invariant mass               
               if (v_jj_tmp.M () < min_mjj) min_mjj = v_jj_tmp.M ();

               // save min drjj
               float drjj = sqrt( (v_f_jets.at (vjet_idx).Eta() - v_f_jets.at (vjet_idx2).Eta())*(v_f_jets.at (vjet_idx).Eta() - v_f_jets.at (vjet_idx2).Eta()) + (v_f_jets.at (vjet_idx).Phi() - v_f_jets.at (vjet_idx2).Phi())*(v_f_jets.at (vjet_idx).Phi() - v_f_jets.at (vjet_idx2).Phi())     );
               if (drjj < min_drjj) min_drjj = drjj; 
           
               // std::cout << "Should continue? " << (charge_1 + charge_2 != 0 && abs(charge_1 + charge_2) != 1) << " " << charge_1 << " " << charge_2 << " " << charge_1 + charge_2 << std::endl; 
               // if (charge_1 + charge_2 != 0 && abs(charge_1 + charge_2) != 1){
               //    continue;
               // } 


               // if ((charge_1 + charge_2 == 0) && (pdg_1 + pdg_2) == 0){
               //    float z_or_w = fabs(v_jj_tmp.M () - 91.1876);
               //    if (z_or_w < closest){
               //       closest = z_or_w;
               //       v_idx_1 = vjet_idx;
               //       v_idx_2 = vjet_idx2;
               //    }
               // }
               // else if (abs(charge_1 + charge_2) == 1){
               //    float z_or_w = fabs(v_jj_tmp.M () - 80.377);
               //    if (z_or_w < closest){
               //       closest = z_or_w;
               //       v_idx_1 = vjet_idx;
               //       v_idx_2 = vjet_idx2;
               //    } 
               // }
               
               float z_or_w = (fabs(v_jj_tmp.M () - 80.377) < fabs(v_jj_tmp.M () - 91.1876))*fabs(v_jj_tmp.M () - 80.377) + (fabs(v_jj_tmp.M () - 80.377) > fabs(v_jj_tmp.M () - 91.1876))*fabs(v_jj_tmp.M () - 91.1876);
               if (z_or_w < closest){
                   closest = z_or_w;
                   v_idx_1 = vjet_idx; 
                   v_idx_2 = vjet_idx2;
               }
            }
         }
        
         for (int lep_idx = 0; lep_idx < v_f_leptons.size(); ++lep_idx){
            for(int jet_idx = 0; jet_idx < v_f_jets.size(); ++jet_idx){
        
               float drjl = sqrt(pow(v_f_leptons.at(lep_idx).second.Eta() - v_f_jets.at (jet_idx).Eta(), 2) + pow(v_f_leptons.at(lep_idx).second.Phi() - v_f_jets.at (jet_idx).Phi() , 2));
               if (drjl < min_drjl) min_drjl = drjl;
            }
         
         }
         
         TLorentzVector v_jj_tmp = v_f_quarks_pdg.at (v_idx_1).second + v_f_quarks_pdg.at (v_idx_2).second ; 
         mV = v_jj_tmp.M();
   
         // std::cout << "mV mass standard: " << mV << std::endl;
         // std::cout << "mV mass: " << mV << " mother indices for first al second V quarks: " << v_f_quarks_mothers.at(v_idx_1).first << " " << v_f_quarks_mothers.at(v_idx_1).second << " " << v_f_quarks_mothers.at(v_idx_2).first << " " << v_f_quarks_mothers.at(v_idx_2).second << std::endl;
 

         // new way to identify V jets, if theyr mother is the same
         float new_mV = 0;
         float min_mV = 1e9;
         int new_mv_index_1 = 0;
         int new_mv_index_2 = 0;

         vector<pair<int, int>> V_bosons;
         vector<pair<int, int>> VBS;
         for (int vjet_idx = 0; vjet_idx < 3; ++vjet_idx){
            for (int vjet_idx2 = vjet_idx+1; vjet_idx2 < 4; ++vjet_idx2){

               //TLorentzVector v_jj_tmp = v_f_quarks_pdg.at (vjet_idx).second + v_f_quarks_pdg.at (vjet_idx2).second ;
               if (v_f_quarks_mothers.at(vjet_idx).first == v_f_quarks_mothers.at(vjet_idx2).first && v_f_quarks_mothers.at(vjet_idx2).first  == 24 ){
                  pair <int, int> p;
                  p.first = vjet_idx;
                  p.second = vjet_idx2;
                  V_bosons.push_back(p);
               }
               else if (v_f_quarks_mothers.at(vjet_idx).first == v_f_quarks_mothers.at(vjet_idx2).first && v_f_quarks_mothers.at(vjet_idx2).first  == -24 ){
                  pair <int, int> p;
                  p.first = vjet_idx;
                  p.second = vjet_idx2;
                  V_bosons.push_back(p);
               }
               else if (v_f_quarks_mothers.at(vjet_idx).first == v_f_quarks_mothers.at(vjet_idx2).first && v_f_quarks_mothers.at(vjet_idx2).first  == 23 ){
                  pair <int, int> p;
                  p.first = vjet_idx;
                  p.second = vjet_idx2;
                  V_bosons.push_back(p);
               }
            }
         }

         vector<float> V_masses;
         float mV_new = -1;
         for (auto entry: V_bosons){
            TLorentzVector v_jj_new = v_f_quarks_pdg.at (entry.first).second + v_f_quarks_pdg.at (entry.second).second ;  
            V_masses.push_back( v_jj_new. M() );
         }

         vector<int> VBS_jets;
         float mVBS_new = -1;
         if (V_bosons.size() == 1){
            pair<int, int> p;
            for(int i = 0; i < 4; ++i){
               if ( i != V_bosons.at(0).first && i != V_bosons.at(0).second){
                  VBS_jets.push_back(i);
               }
            }
            TLorentzVector vbs_jj_new = v_f_quarks_pdg.at (VBS_jets.at(0)).second + v_f_quarks_pdg.at (VBS_jets.at(1)).second ;
            mVBS_new = vbs_jj_new.M ();
            mV_new = V_masses.at(0);
            
         }
         // std::cout << "mV mass new: " << min_mV << std::endl;

         // now compute VBS mass traditionally

         std::vector<int> v_index = {};
         for (int idx=0; idx < 4; idx++){
            if (idx != v_idx_1 && idx != v_idx_2){
               v_index.push_back(idx);
            }
         }
      
         TLorentzVector vbs_jj_tmp = v_f_jets.at (v_index.at(0)) + v_f_jets.at (v_index.at(1)) ; 
         mVBS = vbs_jj_tmp.M ();
       
         // std::cout << "Old VBS: " << mVBS << " Old V: " << mV << " New VBS: " << mVBS_new << " New V: " << mV_new << std::endl;
 
         Ntuple.setvalue ("mVBS_new", mVBS_new) ;
         Ntuple.setvalue ("mV_new", mV_new) ;
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

      Ntuple.setvalue ("nbQuarks", nbQuarks) ;

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
      
      // Filling all weights:

      // std::vector<std::string> wnames = {"cbHIm_cjj11",  "cbHIm_cjj18",  "cbHIm_cjj31",  "cbHIm_cjj38",  "cbHIm_cll1",  "cbHIm_ctBRe",  "cbHIm_ctGIm",  "cbHIm_ctj1",  "cbHIm_ctj8",  "cbHIm_ctWIm",  "cbHIm_ctWRe",  "cbHIm",  "cbHImm1",  "cHB_cbHIm",  "cHB_cHbox",  "cHB_cHbq",  "cHB_cHd",  "cHB_cHDD",  "cHB_cHe",  "cHB_cHj1",  "cHB_cHj3",  "cHB_cHl1",  "cHB_cHl3",  "cHB_cHQ1",  "cHB_cHQ3",  "cHB_cHtbRe",  "cHB_cHt",  "cHB_cHu",  "cHB_cHWB",  "cHB_cHW",  "cHB_cjj11",  "cHB_cjj18",  "cHB_cjj31",  "cHB_cjj38",  "cHB_cll1",  "cHB_cQj11",  "cHB_cQj18",  "cHB_cQj31",  "cHB_cQj38",  "cHB_cQQ1",  "cHB_cQQ8",  "cHB_cQt1",  "cHB_cQt8",  "cHB_ctBRe",  "cHB_ctGIm",  "cHB_ctj1",  "cHB_ctj8",  "cHB_ctWIm",  "cHB_ctWRe",  "cHB_cW",  "cHB",  "cHBm1",  "cHbox_cbHIm",  "cHbox_cHbq",  "cHbox_cHd",  "cHbox_cHe",  "cHbox_cHj1",  "cHbox_cHj3",  "cHbox_cHl1",  "cHbox_cHl3",  "cHbox_cHtbRe",  "cHbox_cHt",  "cHbox_cHu",  "cHbox_cjj11",  "cHbox_cjj18",  "cHbox_cjj31",  "cHbox_cjj38",  "cHbox_cll1",  "cHbox_cQj11",  "cHbox_cQj18",  "cHbox_cQj31",  "cHbox_cQj38",  "cHbox_cQQ1",  "cHbox_cQQ8",  "cHbox_cQt1",  "cHbox_cQt8",  "cHbox_ctBRe",  "cHbox_ctGIm",  "cHbox_ctj1",  "cHbox_ctj8",  "cHbox_ctWIm",  "cHbox_ctWRe",  "cHbox_cW",  "cHbox",  "cHboxm1",  "cHbq_cbHIm",  "cHbq_cHd",  "cHbq_cHe",  "cHbq_cHj1",  "cHbq_cHj3",  "cHbq_cHl1",  "cHbq_cHl3",  "cHbq_cHtbRe",  "cHbq_cHt",  "cHbq_cHu",  "cHbq_cjj11",  "cHbq_cjj18",  "cHbq_cjj31",  "cHbq_cjj38",  "cHbq_cll1",  "cHbq_cQj11",  "cHbq_cQj18",  "cHbq_cQj31",  "cHbq_cQj38",  "cHbq_cQQ1",  "cHbq_cQQ8",  "cHbq_cQt1",  "cHbq_cQt8",  "cHbq_ctBRe",  "cHbq_ctGIm",  "cHbq_ctj1",  "cHbq_ctj8",  "cHbq_ctWIm",  "cHbq_ctWRe",  "cHbq_cW",  "cHbq",  "cHbqm1",  "cHd_cbHIm",  "cHd_cHe",  "cHd_cHj1",  "cHd_cHj3",  "cHd_cHl1",  "cHd_cHl3",  "cHd_cHtbRe",  "cHd_cHt",  "cHd_cHu",  "cHd_cjj11",  "cHd_cjj18",  "cHd_cjj31",  "cHd_cjj38",  "cHd_cll1",  "cHd_cQj11",  "cHd_cQj18",  "cHd_cQj31",  "cHd_cQj38",  "cHd_cQQ1",  "cHd_cQQ8",  "cHd_cQt1",  "cHd_cQt8",  "cHd_ctBRe",  "cHd_ctGIm",  "cHd_ctj1",  "cHd_ctj8",  "cHd_ctWIm",  "cHd_ctWRe",  "cHd_cW",  "cHd",  "cHDD_cbHIm",  "cHDD_cHbox",  "cHDD_cHbq",  "cHDD_cHd",  "cHDD_cHe",  "cHDD_cHj1",  "cHDD_cHj3",  "cHDD_cHl1",  "cHDD_cHl3",  "cHDD_cHQ1",  "cHDD_cHQ3",  "cHDD_cHtbRe",  "cHDD_cHt",  "cHDD_cHu",  "cHDD_cHWB",  "cHDD_cHW",  "cHDD_cjj11",  "cHDD_cjj18",  "cHDD_cjj31",  "cHDD_cjj38",  "cHDD_cll1",  "cHDD_cQj11",  "cHDD_cQj18",  "cHDD_cQj31",  "cHDD_cQj38",  "cHDD_cQQ1",  "cHDD_cQQ8",  "cHDD_cQt1",  "cHDD_cQt8",  "cHDD_ctBRe",  "cHDD_ctGIm",  "cHDD_ctj1",  "cHDD_ctj8",  "cHDD_ctWIm",  "cHDD_ctWRe",  "cHDD_cW",  "cHDD",  "cHDDm1",  "cHdm1",  "cHe_cbHIm",  "cHe_cHj1",  "cHe_cHj3",  "cHe_cHl1",  "cHe_cHl3",  "cHe_cHtbRe",  "cHe_cHt",  "cHe_cHu",  "cHe_cjj11",  "cHe_cjj18",  "cHe_cjj31",  "cHe_cjj38",  "cHe_cll1",  "cHe_cQj11",  "cHe_cQj18",  "cHe_cQj31",  "cHe_cQj38",  "cHe_cQQ1",  "cHe_cQQ8",  "cHe_cQt1",  "cHe_cQt8",  "cHe_ctBRe",  "cHe_ctGIm",  "cHe_ctj1",  "cHe_ctj8",  "cHe_ctWIm",  "cHe_ctWRe",  "cHe_cW",  "cHe",  "cHem1",  "cHj1_cbHIm",  "cHj1_cHj3",  "cHj1_cHl1",  "cHj1_cHl3",  "cHj1_cHtbRe",  "cHj1_cHt",  "cHj1_cHu",  "cHj1_cjj11",  "cHj1_cjj18",  "cHj1_cjj31",  "cHj1_cjj38",  "cHj1_cll1",  "cHj1_cQj11",  "cHj1_cQj18",  "cHj1_cQj31",  "cHj1_cQj38",  "cHj1_cQQ1",  "cHj1_cQQ8",  "cHj1_cQt1",  "cHj1_cQt8",  "cHj1_ctBRe",  "cHj1_ctGIm",  "cHj1_ctj1",  "cHj1_ctj8",  "cHj1_ctWIm",  "cHj1_ctWRe",  "cHj1_cW",  "cHj1",  "cHj1m1",  "cHj3_cbHIm",  "cHj3_cHl1",  "cHj3_cHl3",  "cHj3_cHtbRe",  "cHj3_cHt",  "cHj3_cHu",  "cHj3_cjj11",  "cHj3_cjj18",  "cHj3_cjj31",  "cHj3_cjj38",  "cHj3_cll1",  "cHj3_cQj11",  "cHj3_cQj18",  "cHj3_cQj31",  "cHj3_cQj38",  "cHj3_cQQ1",  "cHj3_cQQ8",  "cHj3_cQt1",  "cHj3_cQt8",  "cHj3_ctBRe",  "cHj3_ctGIm",  "cHj3_ctj1",  "cHj3_ctj8",  "cHj3_ctWIm",  "cHj3_ctWRe",  "cHj3_cW",  "cHj3",  "cHj3m1",  "cHl1_cbHIm",  "cHl1_cHl3",  "cHl1_cHtbRe",  "cHl1_cHt",  "cHl1_cHu",  "cHl1_cjj11",  "cHl1_cjj18",  "cHl1_cjj31",  "cHl1_cjj38",  "cHl1_cll1",  "cHl1_cQj11",  "cHl1_cQj18",  "cHl1_cQj31",  "cHl1_cQj38",  "cHl1_cQQ1",  "cHl1_cQQ8",  "cHl1_cQt1",  "cHl1_cQt8",  "cHl1_ctBRe",  "cHl1_ctGIm",  "cHl1_ctj1",  "cHl1_ctj8",  "cHl1_ctWIm",  "cHl1_ctWRe",  "cHl1_cW",  "cHl1",  "cHl1m1",  "cHl3_cbHIm",  "cHl3_cHtbRe",  "cHl3_cHt",  "cHl3_cHu",  "cHl3_cjj11",  "cHl3_cjj18",  "cHl3_cjj31",  "cHl3_cjj38",  "cHl3_cll1",  "cHl3_cQj11",  "cHl3_cQj18",  "cHl3_cQj31",  "cHl3_cQj38",  "cHl3_cQQ1",  "cHl3_cQQ8",  "cHl3_cQt1",  "cHl3_cQt8",  "cHl3_ctBRe",  "cHl3_ctGIm",  "cHl3_ctj1",  "cHl3_ctj8",  "cHl3_ctWIm",  "cHl3_ctWRe",  "cHl3_cW",  "cHl3",  "cHl3m1",  "cHQ1_cbHIm",  "cHQ1_cHbox",  "cHQ1_cHbq",  "cHQ1_cHd",  "cHQ1_cHe",  "cHQ1_cHj1",  "cHQ1_cHj3",  "cHQ1_cHl1",  "cHQ1_cHl3",  "cHQ1_cHQ3",  "cHQ1_cHtbRe",  "cHQ1_cHt",  "cHQ1_cHu",  "cHQ1_cHWB",  "cHQ1_cHW",  "cHQ1_cjj11",  "cHQ1_cjj18",  "cHQ1_cjj31",  "cHQ1_cjj38",  "cHQ1_cll1",  "cHQ1_cQj11",  "cHQ1_cQj18",  "cHQ1_cQj31",  "cHQ1_cQj38",  "cHQ1_cQQ1",  "cHQ1_cQQ8",  "cHQ1_cQt1",  "cHQ1_cQt8",  "cHQ1_ctBRe",  "cHQ1_ctGIm",  "cHQ1_ctj1",  "cHQ1_ctj8",  "cHQ1_ctWIm",  "cHQ1_ctWRe",  "cHQ1_cW",  "cHQ1",  "cHQ1m1",  "cHQ3_cbHIm",  "cHQ3_cHbox",  "cHQ3_cHbq",  "cHQ3_cHd",  "cHQ3_cHe",  "cHQ3_cHj1",  "cHQ3_cHj3",  "cHQ3_cHl1",  "cHQ3_cHl3",  "cHQ3_cHtbRe",  "cHQ3_cHt",  "cHQ3_cHu",  "cHQ3_cHWB",  "cHQ3_cHW",  "cHQ3_cjj11",  "cHQ3_cjj18",  "cHQ3_cjj31",  "cHQ3_cjj38",  "cHQ3_cll1",  "cHQ3_cQj11",  "cHQ3_cQj18",  "cHQ3_cQj31",  "cHQ3_cQj38",  "cHQ3_cQQ1",  "cHQ3_cQQ8",  "cHQ3_cQt1",  "cHQ3_cQt8",  "cHQ3_ctBRe",  "cHQ3_ctGIm",  "cHQ3_ctj1",  "cHQ3_ctj8",  "cHQ3_ctWIm",  "cHQ3_ctWRe",  "cHQ3_cW",  "cHQ3",  "cHQ3m1",  "cHtbRe_cbHIm",  "cHtbRe_cHu",  "cHtbRe_cjj11",  "cHtbRe_cjj18",  "cHtbRe_cjj31",  "cHtbRe_cjj38",  "cHtbRe_cll1",  "cHtbRe_cQj11",  "cHtbRe_cQj18",  "cHtbRe_cQj31",  "cHtbRe_cQj38",  "cHtbRe_cQQ1",  "cHtbRe_cQQ8",  "cHtbRe_cQt1",  "cHtbRe_cQt8",  "cHtbRe_ctBRe",  "cHtbRe_ctGIm",  "cHtbRe_ctj1",  "cHtbRe_ctj8",  "cHtbRe_ctWIm",  "cHtbRe_ctWRe",  "cHtbRe_cW",  "cHtbRe",  "cHtbRem1",  "cHt_cbHIm",  "cHt_cHtbRe",  "cHt_cHu",  "cHt_cjj11",  "cHt_cjj18",  "cHt_cjj31",  "cHt_cjj38",  "cHt_cll1",  "cHt_cQj11",  "cHt_cQj18",  "cHt_cQj31",  "cHt_cQj38",  "cHt_cQQ1",  "cHt_cQQ8",  "cHt_cQt1",  "cHt_cQt8",  "cHt_ctBRe",  "cHt_ctGIm",  "cHt_ctj1",  "cHt_ctj8",  "cHt_ctWIm",  "cHt_ctWRe",  "cHt_cW",  "cHt",  "cHtm1",  "cHu_cbHIm",  "cHu_cjj11",  "cHu_cjj18",  "cHu_cjj31",  "cHu_cjj38",  "cHu_cll1",  "cHu_cQj11",  "cHu_cQj18",  "cHu_cQj31",  "cHu_cQj38",  "cHu_cQQ1",  "cHu_cQQ8",  "cHu_cQt1",  "cHu_cQt8",  "cHu_ctBRe",  "cHu_ctGIm",  "cHu_ctj1",  "cHu_ctj8",  "cHu_ctWIm",  "cHu_ctWRe",  "cHu_cW",  "cHu",  "cHum1",  "cHWB_cbHIm",  "cHWB_cHbox",  "cHWB_cHbq",  "cHWB_cHd",  "cHWB_cHe",  "cHWB_cHj1",  "cHWB_cHj3",  "cHWB_cHl1",  "cHWB_cHl3",  "cHWB_cHtbRe",  "cHWB_cHt",  "cHWB_cHu",  "cHWB_cjj11",  "cHWB_cjj18",  "cHWB_cjj31",  "cHWB_cjj38",  "cHWB_cll1",  "cHWB_cQj11",  "cHWB_cQj18",  "cHWB_cQj31",  "cHWB_cQj38",  "cHWB_cQQ1",  "cHWB_cQQ8",  "cHWB_cQt1",  "cHWB_cQt8",  "cHWB_ctBRe",  "cHWB_ctGIm",  "cHWB_ctj1",  "cHWB_ctj8",  "cHWB_ctWIm",  "cHWB_ctWRe",  "cHWB_cW",  "cHWB",  "cHWBm1",  "cHW_cbHIm",  "cHW_cHbox",  "cHW_cHbq",  "cHW_cHd",  "cHW_cHe",  "cHW_cHj1",  "cHW_cHj3",  "cHW_cHl1",  "cHW_cHl3",  "cHW_cHtbRe",  "cHW_cHt",  "cHW_cHu",  "cHW_cHWB",  "cHW_cjj11",  "cHW_cjj18",  "cHW_cjj31",  "cHW_cjj38",  "cHW_cll1",  "cHW_cQj11",  "cHW_cQj18",  "cHW_cQj31",  "cHW_cQj38",  "cHW_cQQ1",  "cHW_cQQ8",  "cHW_cQt1",  "cHW_cQt8",  "cHW_ctBRe",  "cHW_ctGIm",  "cHW_ctj1",  "cHW_ctj8",  "cHW_ctWIm",  "cHW_ctWRe",  "cHW_cW",  "cHW",  "cHWm1",  "cjj11_cjj18",  "cjj11_cjj31",  "cjj11_cjj38",  "cjj11_cll1",  "cjj11_ctBRe",  "cjj11_ctGIm",  "cjj11_ctj1",  "cjj11_ctj8",  "cjj11_ctWIm",  "cjj11_ctWRe",  "cjj11",  "cjj11m1",  "cjj18_cjj31",  "cjj18_cjj38",  "cjj18_cll1",  "cjj18_ctBRe",  "cjj18_ctGIm",  "cjj18_ctj1",  "cjj18_ctj8",  "cjj18_ctWIm",  "cjj18_ctWRe",  "cjj18",  "cjj18m1",  "cjj31_cjj38",  "cjj31_cll1",  "cjj31_ctBRe",  "cjj31_ctGIm",  "cjj31_ctj1",  "cjj31_ctj8",  "cjj31_ctWIm",  "cjj31_ctWRe",  "cjj31",  "cjj31m1",  "cjj38_cll1",  "cjj38_ctBRe",  "cjj38_ctGIm",  "cjj38_ctj1",  "cjj38_ctj8",  "cjj38_ctWIm",  "cjj38_ctWRe",  "cjj38",  "cjj38m1",  "cll1_ctBRe",  "cll1_ctGIm",  "cll1_ctj1",  "cll1_ctj8",  "cll1_ctWIm",  "cll1_ctWRe",  "cll1",  "cll1m1",  "cQj11_cbHIm",  "cQj11_cjj11",  "cQj11_cjj18",  "cQj11_cjj31",  "cQj11_cjj38",  "cQj11_cll1",  "cQj11_cQj18",  "cQj11_cQj31",  "cQj11_cQj38",  "cQj11_cQt1",  "cQj11_cQt8",  "cQj11_ctBRe",  "cQj11_ctGIm",  "cQj11_ctj1",  "cQj11_ctj8",  "cQj11_ctWIm",  "cQj11_ctWRe",  "cQj11_cW",  "cQj11",  "cQj11m1",  "cQj18_cbHIm",  "cQj18_cjj11",  "cQj18_cjj18",  "cQj18_cjj31",  "cQj18_cjj38",  "cQj18_cll1",  "cQj18_cQj31",  "cQj18_cQj38",  "cQj18_cQt1",  "cQj18_cQt8",  "cQj18_ctBRe",  "cQj18_ctGIm",  "cQj18_ctj1",  "cQj18_ctj8",  "cQj18_ctWIm",  "cQj18_ctWRe",  "cQj18_cW",  "cQj18",  "cQj18m1",  "cQj31_cbHIm",  "cQj31_cjj11",  "cQj31_cjj18",  "cQj31_cjj31",  "cQj31_cjj38",  "cQj31_cll1",  "cQj31_cQj38",  "cQj31_cQt1",  "cQj31_cQt8",  "cQj31_ctBRe",  "cQj31_ctGIm",  "cQj31_ctj1",  "cQj31_ctj8",  "cQj31_ctWIm",  "cQj31_ctWRe",  "cQj31_cW",  "cQj31",  "cQj31m1",  "cQj38_cbHIm",  "cQj38_cjj11",  "cQj38_cjj18",  "cQj38_cjj31",  "cQj38_cjj38",  "cQj38_cll1",  "cQj38_cQt1",  "cQj38_cQt8",  "cQj38_ctBRe",  "cQj38_ctGIm",  "cQj38_ctj1",  "cQj38_ctj8",  "cQj38_ctWIm",  "cQj38_ctWRe",  "cQj38_cW",  "cQj38",  "cQj38m1",  "cQQ1_cbHIm",  "cQQ1_cjj11",  "cQQ1_cjj18",  "cQQ1_cjj31",  "cQQ1_cjj38",  "cQQ1_cll1",  "cQQ1_cQj11",  "cQQ1_cQj18",  "cQQ1_cQj31",  "cQQ1_cQj38",  "cQQ1_cQQ8",  "cQQ1_cQt1",  "cQQ1_cQt8",  "cQQ1_ctBRe",  "cQQ1_ctGIm",  "cQQ1_ctj1",  "cQQ1_ctj8",  "cQQ1_ctWIm",  "cQQ1_ctWRe",  "cQQ1_cW",  "cQQ1",  "cQQ1m1",  "cQQ8_cbHIm",  "cQQ8_cjj11",  "cQQ8_cjj18",  "cQQ8_cjj31",  "cQQ8_cjj38",  "cQQ8_cll1",  "cQQ8_cQj11",  "cQQ8_cQj18",  "cQQ8_cQj31",  "cQQ8_cQj38",  "cQQ8_cQt1",  "cQQ8_cQt8",  "cQQ8_ctBRe",  "cQQ8_ctGIm",  "cQQ8_ctj1",  "cQQ8_ctj8",  "cQQ8_ctWIm",  "cQQ8_ctWRe",  "cQQ8_cW",  "cQQ8",  "cQQ8m1",  "cQt1_cbHIm",  "cQt1_cjj11",  "cQt1_cjj18",  "cQt1_cjj31",  "cQt1_cjj38",  "cQt1_cll1",  "cQt1_cQt8",  "cQt1_ctBRe",  "cQt1_ctGIm",  "cQt1_ctj1",  "cQt1_ctj8",  "cQt1_ctWIm",  "cQt1_ctWRe",  "cQt1_cW",  "cQt1",  "cQt1m1",  "cQt8_cbHIm",  "cQt8_cjj11",  "cQt8_cjj18",  "cQt8_cjj31",  "cQt8_cjj38",  "cQt8_cll1",  "cQt8_ctBRe",  "cQt8_ctGIm",  "cQt8_ctj1",  "cQt8_ctj8",  "cQt8_ctWIm",  "cQt8_ctWRe",  "cQt8_cW",  "cQt8",  "cQt8m1",  "ctBRe_ctGIm",  "ctBRe_ctj1",  "ctBRe_ctj8",  "ctBRe_ctWIm",  "ctBRe_ctWRe",  "ctBRe",  "ctBRem1",  "ctGIm_ctj1",  "ctGIm_ctj8",  "ctGIm_ctWIm",  "ctGIm_ctWRe",  "ctGIm",  "ctGImm1",  "ctj1_ctj8",  "ctj1",  "ctj1m1",  "ctj8",  "ctj8m1",  "ctWIm_ctj1",  "ctWIm_ctj8",  "ctWIm_ctWRe",  "ctWIm",  "ctWImm1",  "ctWRe_ctj1",  "ctWRe_ctj8",  "ctWRe",  "ctWRem1",  "cW_cbHIm",  "cW_cjj11",  "cW_cjj18",  "cW_cjj31",  "cW_cjj38",  "cW_cll1",  "cW_ctBRe",  "cW_ctGIm",  "cW_ctj1",  "cW_ctj8",  "cW_ctWIm",  "cW_ctWRe",  "cW",  "cWm1"};
   

      
      std::vector<std::string> wnames = {"SM" ,"cW_m1" ,"cW" ,"cHWB_m1" ,"cHWB" ,"cHbox_m1" ,"cHbox" ,"cHW_m1" ,"cHW" ,"cHl1_m1" ,"cHl1" ,"cHB_m1" ,"cHB" ,"cHQ1_m1" ,"cHQ1" ,"cHj1_m1" ,"cHj1" ,"cW_cHWB" ,"cW_cHbox" ,"cW_cHW" ,"cW_cHl1" ,"cW_cHB" ,"cW_cHQ1" ,"cW_cHj1" ,"cHWB_cHbox" ,"cHWB_cHW" ,"cHWB_cHl1" ,"cHWB_cHB" ,"cHWB_cHQ1" ,"cHWB_cHj1" ,"cHbox_cHW" ,"cHbox_cHl1" ,"cHbox_cHB" ,"cHbox_cHQ1" ,"cHbox_cHj1" ,"cHW_cHl1" ,"cHW_cHB" ,"cHW_cHQ1" ,"cHW_cHj1" ,"cHl1_cHB" ,"cHl1_cHQ1" ,"cHl1_cHj1" ,"cHB_cHQ1" ,"cHB_cHj1" ,"cHQ1_cHj1"};
      
      // std::vector<std::string> wnames = {};
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
