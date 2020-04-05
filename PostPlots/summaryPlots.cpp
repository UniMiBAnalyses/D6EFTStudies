/*
c++ -o summaryPlots `root-config --glibs --cflags` ../utils/sputils.cc -lm summaryPlots.cpp

*/

#include <vector>
#include <iostream>

#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "../utils/sputils.h"

using namespace std ;

int main (int argc, char ** argv)
{
  // read the boundaries for the SSWW production and inWW production, 
  // isolate the best variable for each opertor
  // and the list of operators studied
  vector<entry> d_SSWW = readCSV ("SSWW_CSV_CSV.txt") ;
  sort (d_SSWW.begin (), d_SSWW.end (), sortByOpAndOS) ;
  hardunique (d_SSWW, equalByOp) ;
  vector<string> op_tot = getOps (d_SSWW) ;
  
  vector<entry> d_inWW = readCSV ("inWW_CSV_CSV.txt") ;
  sort (d_inWW.begin (), d_inWW.end (), sortByOpAndOS) ;
  hardunique (d_inWW, equalByOp) ;
  vector<string> op_inWW = getOps (d_inWW) ;

  // create the complete list of used operators
  op_tot.insert (op_tot.end (), op_inWW.begin (), op_inWW.end ()) ;
  sort (op_tot.begin (), op_tot.end ()) ;
  hardunique (op_tot) ;

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
          if (found.at (2) > maxy) maxy = found.at (2) ;
          OSB_SSWW.Fill (i, found.at (1)) ;
          TSB_SSWW.Fill (i, found.at (2)) ;
        }

      found = findOp (d_inWW, op_tot.at (i)) ;
      if (found.at (0) > 0.1)
        {
          if (-1 * found.at (2) < miny) miny = -1 * found.at (2) ;
          OSB_inWW.Fill (i, -1 * found.at (1)) ;
          TSB_inWW.Fill (i, -1 * found.at (2)) ;
        }
    }

  TCanvas clims ("clims", "", 600, 600) ;
  TH1F * dh = setDummyHisto (0, miny * 1.1, op_tot.size (), maxy * 1.1, op_tot) ;
  dh->GetYaxis ()->SetTitle ("sensitivity") ;
  dh->Draw ("hist") ;

  OSB_SSWW.SetFillColor (kOrange+1) ;
  OSB_inWW.SetFillColor (kOrange+1) ;
  TSB_SSWW.SetFillColor (kCyan-3) ;
  TSB_inWW.SetFillColor (kCyan-3) ;

  TSB_SSWW.Draw ("hist same") ;
  TSB_inWW.Draw ("hist same") ;
  OSB_SSWW.Draw ("hist same") ;
  OSB_inWW.Draw ("hist same") ;

  TLegend legend (0.70, 0.80, 0.90, 0.90) ;
  legend.AddEntry (&OSB_SSWW, "one sigma bound", "f") ;
  legend.AddEntry (&TSB_SSWW, "two sigma bound", "f") ;
  legend.SetBorderSize (0) ;
  legend.SetFillStyle (0) ;
  legend.Draw () ;

  clims.RedrawAxis () ;
  clims.SaveAs ("cfr.png") ;



  return 0 ;
}