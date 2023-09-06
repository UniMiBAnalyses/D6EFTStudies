//modified by Cristiano Tarricone to be inclusive considering the triboson channels:
//>fully leptonic: WZGamma, ZZGamma
//>semi-leptonic : VZGamma, VZZ (V=W,Z->jj)
#include "autils.h"
#include "dcutils.h"
#include "TMath.h"

using namespace std ;

const float massZ = 91.1876;
//const float CMenergy = 13000;//sqrt(s)=13TeV

histos::histos (TString name, double XS, double lumi) : 
  m_name (name), 
  m_XS (XS),
  m_lumi (lumi)
  {
    makeHisto ("m_mjj", 48, 200., 5000.) ;
    makeHisto ("m_mll", 15, 0., 1500.) ;
    makeHisto ("m_m4l", 15, 0., 1500.) ;
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
double cosOmega(TLorentzVector a, TLorentzVector b){
  return a.CosTheta()*b.CosTheta()+TMath::Sin(a.Theta())*TMath::Sin(b.Theta())*TMath::Cos(a.Phi()-b.Phi());
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
double Legendre(int l, double x){
  double P=1;
  switch(l)
    {
    case 0:
      P=1;
      break;
      
    case 1:
      P=x;
      break;
  
    case 2:
      P=0.5*(3*x*x-1);
      break;

    case 3:
      P=0.5*x*(5*x*x-3);      
      break;
  
    case 4:
      P=(1./8.)*(35*x*x*x*x-30*x*x+3);
      break;
      
    case 5:
      P=(1./8.)*x*(63*x*x*x*x-70*x*x+15);
      break;
    
    case 6:
      P=(1./16.)*(231*pow(x,6)-315*pow(x,4)+105*x*x-5);
      break;
    
    default:
      P=1;
      break;
    }
  return P;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
double WeightsFWM(char wi, TLorentzVector a, TLorentzVector b){
  //double W=1;
  double den, num;
  double ya=a.Rapidity();
  double yb=b.Rapidity();
  double ymean=(ya+yb)/2.;

  switch(wi)
    {
    case 's':
      num = a.Mag()*b.Mag();
      den = (a+b)*(a+b);
      break;

    case 'p':
      num = a.Mag()*b.Mag();
      den = (a+b).Mag2();
      break;
  
    case 't':
      num = a.Pt()*b.Pt();
      den = (a.Pt()+b.Pt())*(a.Pt()+b.Pt());
      break;

    case 'z':
      num = a.Pz()*b.Pz();
      den = ( (a+b).Pz() )*( (a+b).Pz() );
      break;
  
    case 'y':
      ya=1./fabs(ya-ymean);
      yb=1./fabs(yb-ymean);
      num =ya*yb;
      den =(ya+yb)*(ya+yb);
      break;
    
    default:
      num=1.;
      den=1.;
      break;
    }
  return num/den;
}

/*
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
double FWM(int l, char wi, TLorentzVector* objects){
  double H=0; 
  int N =objects->size();
  for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
      H+=WeightsFWM(wi,objects[i],objects[j])*Legendre(l, cosOmega(objects[i],objects[j]));
  //return H;
  return H/2.;
  
}
*/

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
double FWM(int l, char wi, TLorentzVector a, TLorentzVector b){
  return WeightsFWM(wi,a,b)*Legendre( l, cosOmega(a,b) );
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
double SumFWM(int l, char wi, vector <TLorentzVector> objects){//, int N){
  double H=0; 
  int N =objects.size();
  for(int i=0; i<N-1; i++)
    for(int j=i+1; j<N; j++)
      H+=FWM(l,wi,objects[i],objects[j]);

  return H;
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

       cout<<"loop particles"<< endl;
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
      //      if (ME.Pt () < 40) continue ; no MET selection in VBS ZZ baseline
      if (events++ % 10000 == 0)cout <<"no ME selection"<<endl;

      TLorentzVector v_jj = v_f_quarks.at (0) + v_f_quarks.at (1) ;
      if (fabs (v_f_quarks.at (0).Eta () - v_f_quarks.at (1).Eta ()) < 2.5 ) continue ;
      if (v_jj.M () < 100) continue ;
      if (events++ % 10000 == 0)cout<<"quark selection"<<endl;

      TLorentzVector v_ll = v_f_leptons.at (0) + v_f_leptons.at (1) ;
      if (v_ll.M () < 20) continue ;
      // if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (0).Eta ())) > 0.75) continue ;
      //if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (1).Eta ())) > 0.75) continue ;

      if (events++ % 10000 == 0)cout <<"di-lepton selection"<<endl;
      TLorentzVector v_4l;
      if ( v_f_leptons.size() < 4 ) continue;
      cout<<"At least 4 leptons"<< endl;
      v_4l = v_ll + v_f_leptons.at (2) + v_f_leptons.at (3) ;
      if (v_4l.M () < 100) continue ; // cut on ZZmass
      cout<<"mass m4l = "<< v_4l.M() <<endl;
      //      if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (2).Eta ())) > 0.75) continue ; //WHAT  IS Z*
      //      if (fabs (zetaStar (v_f_quarks.at (0).Eta (), v_f_quarks.at (1).Eta (), v_f_leptons.at (3).Eta ())) > 0.75) continue ;


      //PG fill histograms
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
//      Histos.m_mjj->Fill (v_jj.M (), reader.hepeup.XWGTUP) ;
//      Histos.m_mll->Fill (v_ll.M (), reader.hepeup.XWGTUP) ;
      Histos.fill ("m_mjj", v_jj.M (), weight) ;
      Histos.fill ("m_mll", v_ll.M (), weight) ;
      Histos.fill ("m_m4l", v_4l.M (), weight) ;

      float ptl1 = v_f_leptons.at (0).Pt () ;
      float ptl2 = v_f_leptons.at (1).Pt () ;

      //NEED TO ADD pTZ1, pTZ2
      // NEED to SORT pT of all 4 LEPTONS

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
{/*
  if (++m_varcounter != m_values.size ()) 
    {
      cout << "WARNING not all variables filled" << endl ;
    }
 */
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
  return (i.second.Pt () > j.second.Pt ()) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double fillNtuple (LHEF::Reader & reader, ntuple & Ntuple, int max, bool applyCuts)
{
  int events = 0 ;

  int nsel[10] = { };  

  //PG loop over input events
  while (reader.readEvent ()) 
    {
//      if ( reader.outsideBlock.length() ) std::cout << reader.outsideBlock;

      if (events++ % 10000 == 0) cout << "        reading event in file: " << events << endl ;
          
      vector<TLorentzVector> v_f_Ws ;
      vector<TLorentzVector> v_f_jets ;
      vector<TLorentzVector> v_f_gluons ;
      vector<TLorentzVector> v_f_quarks ;
      vector<TLorentzVector> v_f_photons ;

      vector<pair<int, TLorentzVector>> v_f_leptons ;
      vector<pair<int, TLorentzVector>> v_f_neutrinos ;

      //string chan ="2e2mu";//CT: extention to channels with photons 
      string chan;//ZZ ="2e2mu";

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

	      //photons //CT: Added photons for VZG channels
	      else if (abs (reader.hepeup.IDUP.at (iPart)) == 22 ) 
                {
                  v_f_photons.push_back (dummy) ;       
		  } // photons

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
      //PG commenting these lines, to allow for the code to run also in the inclusive case
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
      /*
      if (v_f_photons.size () < 1)
        {
           cout << "warning, no photons!!!" << endl ;
           ++warnNum ;
        }
      else if (v_f_photons.size () > 1)
        {
           cout << "warning, too many photons!!!" << endl ;
           ++warnNum ;
        }
      if (warnNum > 0) continue ;
      */
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
      //float ptl1  = -1. ;
      //float ptl2  = -1. ;
      //float etal1 = -1. ;
      //float etal2 = -1. ;
      float ptZ   = -1. ;
      float ptee  = -1. ;
      float ptmumu  = -1. ;
      float m4l     = -1. ;

      float mll     = -1.;
      float mee     = -1.;
      float mmumu   = -1.;
      float ptll    = -1.;
      float ptemuplus    = -1.;
      float ptemuminus   = -1.;

      //CT: new variables for ZZV
      float mjj = -1. ;
      float m4ljj =-1.;
      float ptl1RelZZ  =-1.;
      float ptl2RelZZ  =-1.;
      float ptj1RelZZ  =-1.;
      float ptj2RelZZ  =-1.;
      float pteeRelZZ  =-1.;
      float ptl1RelZ1  =-1.;
      float ptl2RelZ1  =-1.;
      float ptj1RelZ1  =-1.;
      float ptj2RelZ1  =-1.;
      float ptl1RelZ2  =-1.;
      float ptl2RelZ2  =-1.;
      float ptj1RelZ2  =-1.;
      float ptj2RelZ2  =-1.;
    
      float ptZ1RelZ2  =-1.;
      float ptVRelZ1   =-1.;
      float ptVRelZ2   =-1.;
      float ptV        =-1.;

      float ptVRelZZ   =-1.;
      float ptGamma    =-1.;
      float etaGamma   =-1.;
      float phiGamma   =-1.;
      float ptGRelZ1   =-1.;
      float ptGRelZZ   =-1.;
      float ptjjRelZG  =-1.;


      //CT: FWM variables for ZZV
      float H0sjj =-1.;
      float H0tjj =-1.;
      float H0zjj =-1.;
      float H1sjj =-1.;
      float H1tjj =-1.;
      float H1zjj =-1.;
      float H2sjj =-1.;
      float H2tjj =-1.;
      float H2zjj =-1.;
      //      float H3sjj =-1.;
      //      float H3tjj =-1.;
      //      float H3zjj =-1.;
      float H0sll =-1.;
      float H0tll =-1.;
      float H0zll =-1.;
      float H1sll =-1.;
      float H1tll =-1.;
      float H1zll =-1.;
      float H2sll =-1.;
      float H2tll =-1.;
      float H2zll =-1.;
      float H6tjj =-1.;
      float H6tll =-1.;
      float TotH0s =-1.;
      float TotH0t =-1.;
      float TotH0z =-1.;
      float TotH1s =-1.;
      float TotH1t =-1.;
      float TotH1z =-1.;
      float TotH2s =-1.;
      float TotH2t =-1.;
      float TotH2z =-1.;
      //      float H3sjj =-1.;
      //      float H3tjj =-1.;
      //      float H3zjj =-1.;
      float TotH6t =-1.;

      float H0sVB =-1.;
      float H0tVB =-1.;
      float H0zVB =-1.;
      float H1sVB =-1.;
      float H1tVB =-1.;
      float H1zVB =-1.;
      float H2sVB =-1.;
      float H2tVB =-1.;
      float H2zVB =-1.;
      float H6tVB =-1.;


      //CT: new variables for channels with photons
      float ptG = -1.;
      float met = -1.;
      float mWlept = -1.;
      float mtWlept=-1.;
      float ptW = -1.;
      float ptl1RelWZ  =-1.;
      float ptl2RelWZ  =-1.;
      //float ptllRelWZ  =-1.;
      float ptl1RelW  =-1.;
      float ptl2RelW  =-1.;
    
      float ptWRelZ  =-1.;
      float ptWRelG   =-1.;

      float ptl1RelG  =-1.;
      float ptl2RelG  =-1.;
      float ptj1RelG  =-1.;
      float ptj2RelG  =-1.;
      float ptGRelV  =-1.;
      float ptGRelVZ  =-1.;


      /*
      cout << ">>> FillNtuple:: this is the jets' size ::  " << v_f_jets.size() << endl;
      cout << "                 these are from quarks ::  " << v_f_quarks.size() << endl;
      cout << "                 these are from gluons ::  " << v_f_gluons.size() << endl;
      cout << "                 these are from photons ::  " << v_f_gluons.size() << endl;
      */
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

      TLorentzVector ME ;

      for (int inu = 0 ; inu < v_f_neutrinos.size () ; inu++)
        {
          ME += v_f_neutrinos.at (inu) .second ;
        }


      //CT: cut flow extended to all the cases
      if(applyCuts){
	if (v_f_photons.size() >= 1){
	  if (v_f_leptons.size() >= 2){//channels WZG, ZZG, VZG
	    if (v_f_leptons.size() >= 3){//channels WZG, ZZG
	      if (v_f_leptons.size() >= 4){//channel ZZG
		//cout<<"----------------___CHANNEL_ZZGamma-->e+e-mu+mu- Gamma____----------------"<<endl;
		chan="ZZG";
		++nsel[0];
	      }else{
		if(v_f_neutrinos.size() >= 1){//channel WZG
		  //cout<<"----------------___CHANNEL_ZZGamma-->e+e-mu+vm Gamma____----------------"<<endl;
		  chan="WZG";
		  ++nsel[0];
		}
		else{
		  cerr << "3 charged leptons and one photon but no neutrinos: cannot apply any final state selections, exiting\n" ;
		  exit (1) ;
		}
	      }
	    }else{	      
	      if(v_f_jets.size() >= 2){//channel VZG
		//cout<<"----------------___CHANNEL_VZGamma-->jj l+l- Gamma____----------------"<<endl;
		chan="VZG";
		++nsel[0];
	      }else{
		cerr << "1 photon but less than 3 charged leptons and less than 2 jets: cannot apply any final state selections, exiting\n" ;
		exit (1) ;
	      }	  
	    }
	  }else{
	    cerr << "less than 2 charged leptons: cannot apply any final state selections, exiting\n" ;
	    exit (1) ;
	  }
	}else{
	  if (v_f_leptons.size() >= 4 && v_f_jets.size() >= 2){//channel VZZ
	    //cout<<"----------------___CHANNEL_ZZV-->e+e-mu+mu-jj____----------------"<<endl;
	    chan="VZZ";
	    ++nsel[0];
	  }else{
	    cerr << "cannot apply any final state selections, exiting\n" ;
	    exit (1) ;
	  }
	}
      




       
      /*
      if (applyCuts)//CT: modified to be inclusive
        {
	  //nJets
          if (v_f_jets.size () < 2)
            {
              cerr << "cannot apply semi-leptonic final state selections without two jets, exiting\n" ;
              exit (1) ;
	    }
	      //cout << "cannot apply semi-leptonic final state selections without two jets\n" <<endl;
	      //nLeptons
	  if (v_f_leptons.size () < 4)
	    {
	      cout << "cannot apply any semi-leptonic final state selections without two jets\n" <<endl;
	    }

	  ++nsel[0];
      */
	//Jets selection
	vector<TLorentzVector> v_goodJets ;
	for (int j = 0 ; j < v_f_jets.size () ; j++)
	  {
	    TLorentzVector v_j = v_f_jets.at (j) ;
	    //if (fabs (v_j.Eta()) < 4.7 && v_j.Pt() > 30 ) v_goodJets.push_back(v_j);
	    if (fabs (v_j.Eta()) < 2.5 && v_j.Pt() > 30 ) v_goodJets.push_back(v_j);
	  }

	//Leading(sub-) jets selection
	// if(v_f_jets.size()>=2){
	//  if (ptj1 < 30 || fabs(etaj1) > 2.5) continue ;
	//  if (ptj2 < 30 || fabs(etaj2) > 2.5) continue ;
	// }
	//          if (ME.Pt () < 40) continue ; NO METselection in ZZ
	// ++nsel[1];

	/*
	//VBS ZZ enriched selection LOOSE WP
	if (fabs (v_f_jets.at (0).Eta () - v_f_jets.at (1).Eta ()) < 2.4 ) continue ; 
	if (mjj < 400) continue ;
	*/  	  


	// ++nsel[2];
	//Lepton selection
	vector<TLorentzVector> v_goodMuons ;	  
	vector<TLorentzVector> v_goodElectrons ;	 
	vector<int> v_leptonPlus;  // indices +ly charged leps
	vector<int> v_leptonMinus; // indices -ly charged leps

	bool pTl1min = false; 
	int  pTl2min = 0;
  
	int sumtype_e = 0;
	int sumtype_m = 0;
	int sumtype_ve= 0;
	int sumtype_vm= 0;

	mll  = ( v_f_leptons.at(0).second + v_f_leptons.at(1).second ).M();
	ptll = ( v_f_leptons.at(0).second + v_f_leptons.at(1).second ).Pt();
	//ptl1 = v_f_leptons.at(0).second.Pt();
	//ptl2 = v_f_leptons.at(1).second.Pt();
	//etal1 = v_f_leptons.at(0).second.Eta();
	//etal2 = v_f_leptons.at(1).second.Eta();

	for (int l = 0 ; l < v_f_leptons.size () ; l++)
	  {
	    int type           = v_f_leptons.at (l) .first ;
	    TLorentzVector v_l = v_f_leptons.at (l) .second ;
	      	      
	    if (fabs (v_l.Eta()) < 2.5 && v_l.Pt() > 5 ) //good leptons 
	      {
		if ( abs(type) == 11 || abs(type) == 13 ) 
		  {
		    if (v_l.Pt() > 20) pTl1min = true; //ptmin 20 at least one lep in event
		    if (v_l.Pt() > 10) pTl2min += 1;    //ptmin 10 at least two leps in event

		    //correct lepton pT if neighbouring photon - no FSR @LHE level
		    /*      for (int y = 0 ; y < v_f_photons.size () ; y++)
			    {
			    TLorentzVector v_gamma = v_f_photons.at (y) ;
			    float dummy = v_l.Pt(); 

			    if( v_gamma.DeltaR(v_l)<0.1) v_l += v_gamma;
			    cout << events << "type = " << type <<"pT = "<< dummy << endl;
			    cout << events << " photon pT = " << v_gamma.Pt() << "corrected lepton pT =" << v_l.Pt() << endl;
			    } */

		    if (type>0) v_leptonPlus.push_back(l);  
		    else v_leptonMinus.push_back(l); 

		  }

		if ( abs(type) == 11 ) //electrons 
		  {
		    v_goodElectrons.push_back(v_l);
		    sumtype_e+=type; 
		    //cout<<"electron loop " << sumtype_e<<endl;
		  }

		if ( abs(type) == 13 ) //muons
		  {
		    v_goodMuons.push_back(v_l);	
		    sumtype_m+=type;
		    //cout<<"muon loop " << sumtype_m<<endl;
		  }
	      }
	  }
	  	  
	/*
	//Leading(sub-) leptons selection
	//	  if (!pTl1min || pTl2min<2 ) continue;
	if (!pTl1min || pTl2min<2 ) continue;
	*/

	for (int vl = 0 ; vl < v_f_neutrinos.size () ; vl++)
	  {
	    int vtype           = v_f_neutrinos.at (vl) .first ;
	    if ( abs(vtype) == 12 ) sumtype_ve+=vtype; //ve 
		
	    if ( abs(vtype) == 14 ) sumtype_vm+=vtype; //vm
	  }   

	//Check pairwise OS electrons and muons 
	if ( v_f_neutrinos.size()==0 && (sumtype_e != 0 || sumtype_m != 0) ) continue; //ZZX
	if ( v_f_neutrinos.size()!=0)
	  {
	    if ( abs( sumtype_e+sumtype_ve ) != 0 && abs( sumtype_m+sumtype_vm ) != 0  )  continue; //XZX	
	    if ( ( abs( sumtype_e+sumtype_ve ) == 0 && abs( sumtype_m+sumtype_vm ) != 1 ) || ( abs( sumtype_m+sumtype_vm ) == 0 && abs( sumtype_e+sumtype_ve ) != 1 ))  continue; //WZX	
	  }
	++nsel[3];
	  
	//Photons selection
	vector<TLorentzVector> v_goodPhotons; 
	for(int i=0; i < v_f_photons.size(); i++){ //photon selection
	  if( v_f_photons.at(i).Pt() < 20 || v_f_photons.at(i).Eta() > 2.5 ) continue;
	  //CT: eventually it has to be added the cut given by the photon isolation cone R0 that was anyway included at the runcard level
	  v_goodPhotons.push_back(v_f_photons.at(i));
	}
	if (v_f_photons.size() >= 1 && v_goodPhotons.size() < 1) continue;
	++nsel[4];

	bool goodSeparation=true;
	//Jet-lepton and jet-photon separation
	for (int j = 0 ; j < v_goodJets.size () ; j++)
	  {
	    TLorentzVector v_j = v_goodJets.at(j) ;
	    //cout <<"jet" << j << "pT ="<< v_j.Pt()<< endl;

	    for (int el = 0 ; el < v_goodElectrons.size () ; el++)
	      {
		if (v_j.DeltaR(v_goodElectrons.at(el))<0.4) goodSeparation=false;
		    //break;
		  
		//cout << "e" << el << "pT ="<< v_goodElectrons.at(el).Pt()<< endl;
		//cout << "dR(j" <<j <<",e"<<el<<")="<< v_j.DeltaR(v_goodElectrons.at(el)) <<endl;
	      }

	    for (int mu = 0 ; mu < v_goodMuons.size () ; mu++)
	      {
		if (v_j.DeltaR(v_goodMuons.at(mu))<0.4) goodSeparation=false;
		//cout << "mu" << mu << "pT ="<< v_goodMuons.at(mu).Pt()<< endl;
		//cout << "dR(j" <<j <<",mu"<<mu<<")="<< v_j.DeltaR(v_goodMuons.at(mu)) <<endl;
	      }
	    for (int ph = 0 ; ph < v_goodPhotons.size () ; ph++)
	      {
		if (v_j.DeltaR(v_goodPhotons.at(ph))<0.4) goodSeparation=false;
		//cout << "mu" << mu << "pT ="<< v_goodMuons.at(mu).Pt()<< endl;
		//cout << "dR(j" <<j <<",mu"<<mu<<")="<< v_j.DeltaR(v_goodMuons.at(mu)) <<endl;
	      }
	  }
	if(!goodSeparation) continue;
	++nsel[5];


	TLorentzVector v_ee;
	TLorentzVector v_mm;
	TLorentzVector v_ll;
	TLorentzVector v_ZZ;
	TLorentzVector v_WZ;
	TLorentzVector v_W;
	vector<TLorentzVector> v_Z;	
	vector<TLorentzVector> v_VB;	

	  
	if (chan =="VZZ"||chan=="ZZG")
	  {
	    if ( v_goodElectrons.size()<2 || v_goodMuons.size()<2 ) continue;

	    v_ee = v_goodElectrons[0]+v_goodElectrons[1];
	    v_mm =     v_goodMuons[0]+    v_goodMuons[1];
	    v_ZZ = v_ee + v_mm;
	    v_Z.push_back(v_ee);
	    v_Z.push_back(v_mm);
	    v_VB.push_back(v_ee);
	    v_VB.push_back(v_mm);
	      

	    //boson mass selection
	    //   cout << "mee = " << v_ee.M() << " m_mm =" << v_mm.M()<<endl;
	    if (v_ee.M() <60 || v_ee.M() >120) continue;
	    if (v_mm.M() <60 || v_mm.M() >120) continue;
	  }
	if ( (chan =="VZZ"|| chan=="VZG") && (mjj <50 || mjj >120) ) continue;
	if ( chan =="VZG" )
	  {
	    if( v_goodElectrons.size()<2 && v_goodMuons.size()<2 ) continue;
	    if( v_goodElectrons.size()<2 ) v_ll = v_goodMuons[0]+v_goodMuons[1];
	    else v_ll = v_goodElectrons[0]+v_goodElectrons[1];
	    v_Z.push_back(v_ll);
	    if (v_ll.M() <60 || v_ll.M() >120) continue;
	    v_VB.push_back(v_ll);
	  }
	if ( chan =="WZG" )//mu+ vm e+ e- G 
	  {
	    if( v_goodMuons.size()<1 ) continue; 
	    if( v_goodElectrons.size()<2 ) continue;
	    v_ee = v_goodElectrons[0]+v_goodElectrons[1];
	    if (v_ee.M()<60 || v_ee.M() >120) continue;
	    v_Z.push_back(v_ee);
	    v_W = v_f_neutrinos.at(0).second + v_goodMuons[0];
	    if (v_W.M() <50 || v_W.M() >110) continue;
	    v_WZ = v_W + v_Z.at(0);
	    v_VB.push_back(v_W);

	  }

	++nsel[6];

	//deltaR cuts
	/*  TLorentzVector v_e1  = v_goodElectrons[0] ;
	    TLorentzVector v_e2  = v_goodElectrons[1] ;
	    TLorentzVector v_mu1 = v_goodMuons    [0] ;
	    TLorentzVector v_mu2 = v_goodMuons    [1] ;

	    if (v_e1.DeltaR(v_e2)<0.02  || v_mu1.DeltaR(v_mu2)<0.02) cout << "boosted Z topology"<< endl;
	    if (v_e1.DeltaR(v_mu1)<0.05 ||  v_e1.DeltaR(v_mu2)<0.05 || v_e2.DeltaR(v_mu1)<0.05 || v_e2.DeltaR(v_mu2)<0.05) cout << "e-mu pair collimated"<<endl; 
	*/

	
	if (chan=="WZG" && ME.Pt () < 30) continue ;//MET selection 
	++nsel[7];
	

	vector <TLorentzVector> finalState;

	if(chan=="VZZ"||chan=="VZG")
	  {
	    for (int i=0; i<2; i++) finalState.push_back(v_f_jets[i]);
	    v_VB.push_back(v_f_jets[0]+v_f_jets[1]);

	    ptV = (v_f_jets[0]+v_f_jets[1]).Pt();

	    ptj1RelZ1   = v_f_jets.at(0).Pt( v_Z.at(0).Vect() );
	    ptj2RelZ1   = v_f_jets.at(1).Pt( v_Z.at(0).Vect() );

	    
	    H0sjj = FWM(0, 's', v_f_jets[0], v_f_jets[1] );
	    H0tjj = FWM(0, 't', v_f_jets[0], v_f_jets[1] );
	    H0zjj = FWM(0, 'z', v_f_jets[0], v_f_jets[1] );
	    H1sjj = FWM(1, 's', v_f_jets[0], v_f_jets[1] );
	    H1tjj = FWM(1, 't', v_f_jets[0], v_f_jets[1] );
	    H1zjj = FWM(1, 'z', v_f_jets[0], v_f_jets[1] );
	    H2sjj = FWM(2, 's', v_f_jets[0], v_f_jets[1] );
	    H2tjj = FWM(2, 't', v_f_jets[0], v_f_jets[1] );
	    H2zjj = FWM(2, 'z', v_f_jets[0], v_f_jets[1] );
	    H6tjj = FWM(6, 't', v_f_jets[0], v_f_jets[1] );

	    ptVRelZ1 = (v_f_jets[0]+v_f_jets[1]).Pt( v_Z.at(0).Vect() );
	    //mjj = (v_f_jets[0]+v_f_jets[1]).M();//implemented before
	    
	  }

	if(chan=="VZZ"||chan=="ZZG")
	  {
	    //Main variables ptZ and m4l
	    ptee   = v_ee.Pt();
	    ptmumu = v_mm.Pt();

	    mee   = v_ee.M();
	    mmumu = v_mm.M();
	    if(fabs(mee-massZ)>fabs(mmumu-massZ)) swap(v_Z[0],v_Z[1]);

	    ptZ    = fabs(massZ -v_ee.M()) < fabs(massZ -v_mm.M()) ? v_ee.Pt() : v_mm.Pt();
	    m4l    = (v_goodElectrons[0]+v_goodElectrons[1]+v_goodMuons[0]+v_goodMuons[1]).M();

	    ptemuplus  = ( v_f_leptons.at(v_leptonPlus[0]).second  + v_f_leptons.at(v_leptonPlus[1]).second ).Pt();
	    ptemuminus = ( v_f_leptons.at(v_leptonMinus[0]).second + v_f_leptons.at(v_leptonMinus[1]).second ).Pt();

	    ptl1RelZZ   = v_f_leptons.at(0).second.Pt( v_ZZ.Vect() );
	    ptl2RelZZ   = v_f_leptons.at(1).second.Pt( v_ZZ.Vect() );
	    pteeRelZZ   = v_ee.Pt( v_ZZ.Vect() );
	    ptl1RelZ2   = v_f_leptons.at(0).second.Pt( v_Z.at(1).Vect() );
	    ptl2RelZ2   = v_f_leptons.at(1).second.Pt( v_Z.at(1).Vect() );
	      
	    ptZ1RelZ2   = v_Z.at(0).Pt( v_Z.at(1).Vect() );
	    
	    for (int i=0; i<2; i++) finalState.push_back(v_goodElectrons[i]);
	    for (int i=0; i<2; i++) finalState.push_back(v_goodMuons[i]);

	      
	    //cout << "indices ++ = "<< v_leptonPlus[0] <<v_leptonPlus[1] << " pTemu++ = " << ptemuplus << endl;
	    //cout << "indices -- = "<< v_leptonMinus[0]<<v_leptonMinus[1] << " pTemu-- = " << ptemuminus << endl;

	    // cout << "fabs(massZ -m_ee) ="<< fabs(massZ -v_ee.M()) << " fabs(massZ -m_mm) ="<< fabs(massZ -v_mm.M()) << endl;
	    // cout <<" m4l =" <<m4l<< " ptZ =" << ptZ <<" ptee ="<<v_ee.Pt() << " ptmumu =" << v_mm.Pt() << endl; 
	  }
	
	if(chan=="ZZG"||chan=="WZG"||chan=="VZG")
	  {
	    v_VB.push_back(v_goodPhotons[0]);

	    finalState.push_back(v_goodPhotons[0]);
	    ptGamma  = v_goodPhotons.at(0).Pt();
	    etaGamma = v_goodPhotons.at(0).Eta();
	    phiGamma = v_goodPhotons.at(0).Phi();
	    ptGRelZ1 = v_goodPhotons.at(0).Pt( v_Z.at(0).Vect() );
	  }


	if(chan=="VZZ")
	  {
	    m4ljj  = (v_goodElectrons[0]+v_goodElectrons[1]+v_goodMuons[0]+v_goodMuons[1]+v_f_jets[0]+v_f_jets[0]).M();
	    ptj1RelZZ   = v_f_jets.at(0).Pt( v_ZZ.Vect() );
	    ptj2RelZZ   = v_f_jets.at(1).Pt( v_ZZ.Vect() );
	    pteeRelZZ   = v_ee.Pt( ( v_ZZ.Vect() ) );
	    ptj1RelZ2   = v_f_jets.at(0).Pt( v_Z.at(1).Vect() );
	    ptj2RelZ2   = v_f_jets.at(1).Pt( v_Z.at(1).Vect() );
	    ptVRelZ2    = (v_f_jets[0]+v_f_jets[1]).Pt( v_Z.at(1).Vect() );
	    ptVRelZZ    = (v_f_jets[0]+v_f_jets[1]).Pt( v_ZZ.Vect() );

	  }


	if(chan=="ZZG")
	  {
	    ptGRelZZ = v_goodPhotons.at(0).Pt( v_ZZ.Vect() );
	  }

	if(chan=="WZG")
	  {	    	    
	    for (int i=0; i<2; i++) finalState.push_back(v_goodElectrons[i]);
	    finalState.push_back(v_goodMuons[0]);
	    finalState.push_back(v_f_neutrinos.at(0).second);

	    ptZ = v_Z.at(0).Pt();

	    ptW = v_W.Pt();
	    met = ME.Pt();
	    mWlept  = v_W.M();
	    mtWlept = v_W.Mt();

	    ptl1RelWZ   = v_f_leptons.at(0).second.Pt( v_WZ.Vect() );
	    ptl2RelWZ   = v_f_leptons.at(1).second.Pt( v_WZ.Vect() );
	    //pteeRelWZ   = v_ee.Pt( v_WZ.Vect() );
	    //ptl1RelZ    = v_f_leptons.at(0).second.Pt( v_Z.at(0).Vect() );
	    //ptl2RelZ    = v_f_leptons.at(1).second.Pt( v_Z.at(0).Vect() );
	    ptl1RelW    = v_f_leptons.at(0).second.Pt( v_W.Vect() );
	    ptl2RelW    = v_f_leptons.at(1).second.Pt( v_W.Vect() );
	      
	    ptWRelZ     = v_W.Pt( v_Z.at(0).Vect() );
	    ptWRelG     = v_W.Pt( v_goodPhotons.at(0).Vect() );
	  }


	if(chan=="VZG")
	  {
	    for (int i=0; i<2; i++) finalState.push_back(v_f_leptons.at(i).second);

	    ptZ = v_Z.at(0).Pt();

	    ptl1RelG   = v_f_leptons.at(0).second.Pt( v_goodPhotons.at(0).Vect() );
	    ptl2RelG   = v_f_leptons.at(1).second.Pt( v_goodPhotons.at(0).Vect() );
	    ptj1RelG   = v_goodJets.at(0).Pt( v_goodPhotons.at(0).Vect() );
	    ptj2RelG   = v_goodJets.at(1).Pt( v_goodPhotons.at(0).Vect() );
	    ptGRelV    = v_goodPhotons.at(0).Pt( ( v_goodJets.at(0)+v_goodJets.at(1) ).Vect() );
	    ptGRelVZ   = v_goodPhotons.at(0).Pt( ( v_goodJets.at(0)+v_goodJets.at(1)+v_Z.at(0) ).Vect() );

	    ptjjRelZG  = ( v_goodJets.at(0)+v_goodJets.at(1) ).Pt( (v_goodPhotons.at(0)+v_Z.at(0)  ).Vect() );
	  }


	ptl1RelZ1   = v_f_leptons.at(0).second.Pt( v_Z.at(0).Vect() );
	ptl2RelZ1   = v_f_leptons.at(1).second.Pt( v_Z.at(0).Vect() );


	H0sll = FWM(0, 's', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H0tll = FWM(0, 't', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H0zll = FWM(0, 'z', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H1sll = FWM(1, 's', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H1tll = FWM(1, 't', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H1zll = FWM(1, 'z', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H2sll = FWM(2, 's', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H2tll = FWM(2, 't', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H2zll = FWM(2, 'z', v_f_leptons.at(0).second, v_f_leptons.at(1).second);
	H6tll = FWM(6, 't', v_f_leptons.at(0).second, v_f_leptons.at(1).second);

	H0sVB = SumFWM(0, 's', v_VB);
	H0tVB = SumFWM(0, 't', v_VB);
	H0zVB = SumFWM(0, 'z', v_VB);
	H1sVB = SumFWM(1, 's', v_VB);
	H1tVB = SumFWM(1, 't', v_VB);
	H1zVB = SumFWM(1, 'z', v_VB);
	H2sVB = SumFWM(2, 's', v_VB);
	H2tVB = SumFWM(2, 't', v_VB);
	H2zVB = SumFWM(2, 'z', v_VB);
	H6tVB = SumFWM(6, 't', v_VB);


	TotH0s = SumFWM(0, 's', finalState);
	TotH0t = SumFWM(0, 't', finalState);
	TotH0z = SumFWM(0, 'z', finalState);
	TotH1s = SumFWM(1, 's', finalState);
	TotH1t = SumFWM(1, 't', finalState);
	TotH1z = SumFWM(1, 'z', finalState);
	TotH2s = SumFWM(2, 's', finalState);
	TotH2t = SumFWM(2, 't', finalState);
	TotH2z = SumFWM(2, 'z', finalState);
	TotH6t = SumFWM(6, 't', finalState);

	//No Zeta* cuts in VBS ZZ analysis
	//if (fabs (zetaStar (v_f_jets.at (0).Eta (), v_f_jets.at (1).Eta (), v_f_leptons.at (0).second.Eta ())) > 0.75) continue ;
	//if (fabs (zetaStar (v_f_jets.at (0).Eta (), v_f_jets.at (1).Eta (), v_f_leptons.at (1).second.Eta ())) > 0.75) continue ;
	   
      } //end apply cuts loop

      //PG fill variables
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
      
      if(chan=="VZZ"){
	Ntuple.setvalue ("m4ljj", m4ljj) ;
	Ntuple.setvalue ("ptl1RelZZ", ptl1RelZZ) ;
	Ntuple.setvalue ("ptl2RelZZ", ptl2RelZZ) ;
	Ntuple.setvalue ("ptj1RelZZ", ptj1RelZZ) ;
	Ntuple.setvalue ("ptj2RelZZ", ptj2RelZZ) ;
	Ntuple.setvalue ("pteeRelZZ", pteeRelZZ);
	//Ntuple.setvalue ("ptmumuRel", ptmumuRel);
	//Ntuple.setvalue ("pteeRelZ1", pteeRelZ1) ;
	Ntuple.setvalue ("ptl1RelZ2", ptl1RelZ2) ;
	Ntuple.setvalue ("ptl2RelZ2", ptl2RelZ2) ;
	Ntuple.setvalue ("ptj1RelZ2", ptj1RelZ2) ;
	Ntuple.setvalue ("ptj2RelZ2", ptj2RelZ2) ;
	Ntuple.setvalue ("ptVRelZ2", ptVRelZ2) ;
	Ntuple.setvalue ("ptVRelZZ", ptVRelZZ) ;
      }

      if(chan=="VZZ" || chan=="ZZG"){
	Ntuple.setvalue ("ptl1RelZZ", ptl1RelZZ) ;
	Ntuple.setvalue ("ptl2RelZZ", ptl2RelZZ) ;
	Ntuple.setvalue ("pteeRelZZ", pteeRelZZ);
	Ntuple.setvalue ("ptl1RelZ2", ptl1RelZ2) ;
	Ntuple.setvalue ("ptl2RelZ2", ptl2RelZ2) ;
	Ntuple.setvalue ("ptZ1RelZ2", ptZ1RelZ2) ;
	Ntuple.setvalue ("m4l", m4l) ;
	//Ntuple.setvalue ("ptZ", ptZ) ;
	Ntuple.setvalue ("ptee", ptee) ;
	Ntuple.setvalue ("ptmumu", ptmumu) ;
	Ntuple.setvalue ("mee", mee) ; 
	Ntuple.setvalue ("mmumu", mmumu) ;

	Ntuple.setvalue ("ptemuplus" , ptemuplus  ) ;
	Ntuple.setvalue ("ptemuminus", ptemuminus ) ;
	
      }

      if(chan=="VZZ" || chan=="VZG"){
	Ntuple.setvalue ("mjj", mjj) ;
	Ntuple.setvalue ("ptj1", ptj1) ;
	Ntuple.setvalue ("ptj2", ptj2) ;
	Ntuple.setvalue ("etaj1", etaj1) ;
	Ntuple.setvalue ("etaj2", etaj2) ;
	Ntuple.setvalue ("phij1", phij1) ;
	Ntuple.setvalue ("phij2", phij2) ;
	//Ntuple.setvalue ("ptl1", ptl1) ;
	//Ntuple.setvalue ("ptl2", ptl2) ;
	//Ntuple.setvalue ("etal1", etal1) ;
	//Ntuple.setvalue ("etal2", etal2) ;

	Ntuple.setvalue ("ptj1RelZ1", ptj1RelZ1) ;
	Ntuple.setvalue ("ptj2RelZ1", ptj2RelZ1) ;

	Ntuple.setvalue ("deltaetajj", fabs (etaj1 - etaj2)) ;
	Ntuple.setvalue ("deltaphijj", deltaPhi (phij1, phij2)) ;

	//CT: New variables
	Ntuple.setvalue ("ptV", ptV) ;
	Ntuple.setvalue ("ptVRelZ1", ptVRelZ1) ;
	Ntuple.setvalue ("H0sjj", H0sjj) ;
	Ntuple.setvalue ("H0tjj", H0tjj) ;
	Ntuple.setvalue ("H0zjj", H0zjj) ;
	Ntuple.setvalue ("H1sjj", H1sjj) ;
	Ntuple.setvalue ("H1tjj", H1tjj) ;
	Ntuple.setvalue ("H1zjj", H1zjj) ;
	Ntuple.setvalue ("H2sjj", H2sjj) ;
	Ntuple.setvalue ("H2tjj", H2tjj) ;
	Ntuple.setvalue ("H2zjj", H2zjj) ;
	Ntuple.setvalue ("H6tjj", H6tjj) ;

      }
	
      if(chan=="ZZG"||chan=="WZG"||chan=="VZG")
	{
	  Ntuple.setvalue ("ptGamma", ptGamma) ;
	  Ntuple.setvalue ("etaGamma", etaGamma) ;
	  Ntuple.setvalue ("phiGamma", phiGamma) ;
	  Ntuple.setvalue ("ptGRelZ1", ptGRelZ1) ;
	}
      if(chan=="ZZG")	  Ntuple.setvalue ("ptGRelZZ", ptGRelZZ) ;

      if(chan=="WZG") 
	{
	  Ntuple.setvalue ("ptW", ptW) ;      
	  Ntuple.setvalue ("mWlept", mWlept) ;      
	  Ntuple.setvalue ("mtWlept", mtWlept) ;      
	  Ntuple.setvalue ("ptl1RelWZ", ptl1RelWZ) ;      
	  Ntuple.setvalue ("ptl2RelWZ", ptl2RelWZ) ;      
	  Ntuple.setvalue ("ptl1RelW", ptl1RelW) ;      
	  Ntuple.setvalue ("ptl2RelW", ptl2RelW) ;      
	  Ntuple.setvalue ("ptWRelG", ptWRelG) ;      
	  Ntuple.setvalue ("ptWRelZ", ptWRelZ) ;      

	}

      if(chan=="VZG")
	{
	  Ntuple.setvalue ("ptl1RelG", ptl1RelG) ;      
	  Ntuple.setvalue ("ptl2RelG", ptl2RelG) ;      
	  Ntuple.setvalue ("ptj1RelG", ptj1RelG) ;      
	  Ntuple.setvalue ("ptj2RelG", ptj2RelG) ;      
	  Ntuple.setvalue ("ptGRelV" , ptGRelV ) ;      
	  Ntuple.setvalue ("ptGRelVZ", ptGRelVZ) ;      
	  Ntuple.setvalue ("ptjjRelZG" , ptjjRelZG) ;      

	}


      Ntuple.setvalue ("mll", mll) ;      
      Ntuple.setvalue ("ptll", ptll ) ;
      Ntuple.setvalue ("ptZ", ptZ) ;

      Ntuple.setvalue ("ptl1RelZ1", ptl1RelZ1) ;
      Ntuple.setvalue ("ptl2RelZ1", ptl2RelZ1) ;

      Ntuple.setvalue ("H0sll", H0sll) ;
      Ntuple.setvalue ("H0tll", H0tll) ;
      Ntuple.setvalue ("H0zll", H0zll) ;
      Ntuple.setvalue ("H1sll", H1sll) ;
      Ntuple.setvalue ("H1tll", H1tll) ;
      Ntuple.setvalue ("H1zll", H1zll) ;
      Ntuple.setvalue ("H2sll", H2sll) ;
      Ntuple.setvalue ("H2tll", H2tll) ;
      Ntuple.setvalue ("H2zll", H2zll) ;

      Ntuple.setvalue ("H6tll", H6tll) ;

      Ntuple.setvalue ("TotH0t", TotH0t) ;
      Ntuple.setvalue ("TotH0z", TotH0z) ;
      Ntuple.setvalue ("TotH0s", TotH0s) ;
      Ntuple.setvalue ("TotH1t", TotH1t) ;
      Ntuple.setvalue ("TotH1z", TotH1z) ;
      Ntuple.setvalue ("TotH1s", TotH1s) ;
      Ntuple.setvalue ("TotH2t", TotH2t) ;
      Ntuple.setvalue ("TotH2z", TotH2z) ;
      Ntuple.setvalue ("TotH2s", TotH2s) ;
      Ntuple.setvalue ("TotH6t", TotH6t) ;
      //Ntuple.setvalue ("ptemuplus", ptemuplus);
      //Ntuple.setvalue ("ptemuminus", ptemuminus); //stored in the next step
      



      // Ordering v_f_Leptons according to Pt.

      //      cout << "--------------------------"<< endl;
      // cout << v_f_leptons.at(0).second.Pt() << " " << v_f_leptons.at(1).second.Pt() << " " << v_f_leptons.at(2).second.Pt() <<" " <<  v_f_leptons.at(3).second.Pt() << endl ;
      sort (v_f_leptons.begin (), v_f_leptons.end (), largerPt ) ;     
      //cout << v_f_leptons.at(0).second.Pt() << " " << v_f_leptons.at(1).second.Pt() << " " << v_f_leptons.at(2).second.Pt() <<" " <<  v_f_leptons.at(3).second.Pt() << endl ;

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

      if(chan=="WZG") Ntuple.setvalue ("met", ME.Pt ()) ;
      //Ntuple.setvalue ("ptll", ptll ) ;
      //Ntuple.setvalue ("ptemuplus" , ptemuplus  ) ;
      //Ntuple.setvalue ("ptemuminus", ptemuminus ) ;

      //CT: TEST rwgt  
      Ntuple.fill (eventWeight) ;
      /*
	for(int wgt_idx = 1; wgt_idx <= NUMBER_OF_REWEIGHTS; ++wgt_idx){
	Ntuple.setvalue(Form("rwgt_%d", wgt_idx), reader.hepeup.namedweights[wgt_idx-1].weights[0]);
	}
      */
      /**************
      NUMBER_OF_RWGT: 
      VZG_ewk: 21 
      VZG_qcd:  6
      VZG_ewk: 
      VZG_qcd: 

      **************/
      for(int wgt_idx = 1; wgt_idx <= 6; ++wgt_idx){
	Ntuple.setvalue(Form("rwgt_%d", wgt_idx), reader.hepeup.namedweights[wgt_idx-1].weights[0]);
      }


      if (max > 0 && max < events) 
        {
          cout << max << " events reached, exiting" << endl ;
          break ;
        }

    } //PG loop over input events

  //Print cutflow summary
  double fracAbs[10];
  double fracRel[10];
  std::string cuts[10];
  
/* 
  cuts[0] = "nJet and nLeptons";
  cuts[1] = "Jet kinematics";
  cuts[2] = "detajj and mjj sel";
  cuts[3] = "Lepton kinematics";
  cuts[4] = "Lepton-jet deltaR(l;j)";
  cuts[5] = "mZ selection";
  cuts[6] = "mZZ selection";
*/

  cuts[0] = "Final state signature";
  cuts[1] = "Jets kinematics";
  cuts[2] = "Jets VBS cuts";
  cuts[3] = "Lepton kinematics";
  cuts[4] = "deltaR jets separations";
  cuts[5] = "mass selection";
  cuts[6] = "photon selection";
  cuts[7] = "MET selection";
  
  printf ("%-23s  %10s  %10s  %10s \n", std::string("Cut flow").c_str(), std::string("# events").c_str(), std::string("absolute").c_str(), std::string("relative").c_str() ); 
  for ( int i = 0; i < 8; ++i ) 
    {
      fracAbs[i] = double(nsel[i])/nsel[0];
      if ( i>0 )
	fracRel[i] = double(nsel[i])/nsel[i-1];
      else
	fracRel[i] = fracAbs[i];
      printf ("%-23s  %10d  %10.3f  %10.3f \n", cuts[i].c_str(), nsel[i], fracAbs[i], fracRel[i] ); 
    }

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




