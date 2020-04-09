/*
c++ -o summaryPlots `root-config --glibs --cflags` ../utils/dcutils.cc ../utils/sputils.cc -lm summaryPlots.cpp
*/

#include <vector>
#include <iostream>

#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TF1.h"

#include "../utils/dcutils.h"
#include "../utils/sputils.h"

using namespace std ;


float func (float x)
{
  return x ;
//  return cbrt (x) ;
}


float invfunc (float x)
{
  return x ;
//  return pow (x,3) ;
}


Double_t axistransform (Double_t *x, Double_t *par)
{
//  return invfunc (x[0]) ;
  return x[0] ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


int main (int argc, char ** argv)
{

  // assuming that all operators tested for inWW are present for SSWW as well
  // i.e. the operators tested for SSWW are a larger set wrt inWW

  if (argc < 3)
    {
      cerr << "Forgot to add the input files with scan results " << endl ;
      cerr << "usage: ./summaryPlots SSWWresults inWWresults" << endl ;

      return 1 ;
    }

  vector<entry> d_SSWW = readCSV (argv[1]) ;
  sort (d_SSWW.begin (), d_SSWW.end (), sortByOpAndOS) ;
  hardunique (d_SSWW, equalByOp) ;
  sort (d_SSWW.begin (), d_SSWW.end (), sortByOS) ;
  vector<string> op_tot = getOps (d_SSWW) ;
  
  vector<entry> d_inWW = readCSV (argv[2]) ;
  sort (d_inWW.begin (), d_inWW.end (), sortByOpAndOS) ;
  hardunique (d_inWW, equalByOp) ;
  sort (d_inWW.begin (), d_inWW.end (), sortByOS) ;
  vector<string> op_inWW = getOps (d_inWW) ;

  // // create the complete list of used operators
  // op_tot.insert (op_tot.end (), op_inWW.begin (), op_inWW.end ()) ;
  // sort (op_tot.begin (), op_tot.end ()) ;
  // hardunique (op_tot) ;

  TH1F OSB_SSWW ("OSB_SSWW", "", op_tot.size (), 0, op_tot.size ()) ;
  TH1F TSB_SSWW ("TSB_SSWW", "", op_tot.size (), 0, op_tot.size ()) ;
  TH1F OSB_inWW ("OSB_inWW", "", op_tot.size (), 0, op_tot.size ()) ;
  TH1F TSB_inWW ("TSB_inWW", "", op_tot.size (), 0, op_tot.size ()) ;

  float maxy = -1. ;
  float miny = 1. ;
  for (int i = 0 ; i < op_tot.size () ; ++i)
    {
      vector<float> found = findOp (d_SSWW, op_tot.at (i)) ;
      if (found.at (0) > 0.1)
        {
          if (func (found.at (2)) > maxy) maxy = func (found.at (2)) ;
          OSB_SSWW.Fill (i, func (found.at (1))) ;
          TSB_SSWW.Fill (i, func (found.at (2))) ;
        }

      found = findOp (d_inWW, op_tot.at (i)) ;
      if (found.at (0) > 0.1)
        {
          if (-1 * func (found.at (2)) < miny) miny = -1 * func (found.at (2)) ;
          OSB_inWW.Fill (i, -1 * func (found.at (1))) ;
          TSB_inWW.Fill (i, -1 * func (found.at (2))) ;
        }
    }

  TCanvas clims ("clims", "", 600, 600) ;
  TH1F * dh = setDummyHisto (0, miny * 1.1, op_tot.size (), maxy * 1.1, op_tot) ;
  dh->SetStats (0) ;
  dh->GetYaxis ()->SetTitle ("sensitivity") ;
  dh->Draw ("hist") ;

  // TGaxis newaxis (
  //   dh->GetXaxis ()->GetXmin (), 
  //   dh->GetMinimum (),
  //   dh->GetXaxis ()->GetXmin (), 
  //   dh->GetMaximum (),
  //   dh->GetMinimum (),
  //   dh->GetMaximum (),
  //   4) ;

  cout << dh->GetXaxis ()->GetXmin ()<<endl; 
  cout << dh->GetMinimum ()<<endl;
  cout << dh->GetXaxis ()->GetXmin ()<<endl; 
  cout << dh->GetMaximum ()<<endl;

  TF1 transform ("transform", axistransform, dh->GetMaximum (), dh->GetMaximum (), 0) ;
  TGaxis newaxis (
    dh->GetXaxis ()->GetXmin (), 
    dh->GetMinimum (),
    dh->GetXaxis ()->GetXmin (), 
    dh->GetMaximum (),
    "transform", 505, "") ;
                  // 10,  
                  // "",
                  // Double_t gridlength)
  newaxis.SetLineColor (kRed) ;
  newaxis.SetLabelColor (kRed) ;
  newaxis.Draw () ;

  OSB_SSWW.SetFillColor (kOrange+1) ;
  OSB_inWW.SetFillColor (kOrange+1) ;
  TSB_SSWW.SetFillColor (kCyan-3) ;
  TSB_inWW.SetFillColor (kCyan-3) ;

  TSB_SSWW.Draw ("hist same") ;
  TSB_inWW.Draw ("hist same") ;
  OSB_SSWW.Draw ("hist same") ;
  OSB_inWW.Draw ("hist same") ;

  TLegend legend (0.10, 0.70, 0.40, 0.90) ;
  legend.AddEntry (&OSB_SSWW, "one sigma bound", "f") ;
  legend.AddEntry (&TSB_SSWW, "two sigma bound", "f") ;
  legend.SetBorderSize (0) ;
  legend.SetFillStyle (0) ;
  legend.Draw () ;

//  clims.RedrawAxis () ;
  clims.SaveAs ("cfr.png") ;

  return 0 ;
}