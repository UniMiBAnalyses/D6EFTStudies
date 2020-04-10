/*
c++ -o testaxis `root-config --glibs --cflags` -lm testaxis.cpp
*/

#include <iostream>
#include <tgmath.h>

#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TF1.h"

using namespace std ;


float func (float x)
{
//  return x ;
//  return cbrt (x) ;
  return log10 (x) ;
}


float invfunc (float x)
{
//  return x ;
//  return pow (x,3) ;
  return pow (10, x) ; 
}


Double_t axistransform (Double_t *x, Double_t *par)
{
  return invfunc (x[0]) ;
//  return x[0] ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


int main (int argc, char ** argv)
{

  TCanvas clims ("clims", "", 600, 600) ;
  TH1F * dh = new TH1F ("dh", "dh", 4, 0, 2) ;
  dh->Fill (0.25, func (10.)) ;
  dh->Fill (0.75, func (100.)) ;

  dh->Fill (1.25, func (1000.)) ;
  dh->Fill (1.75, func (10000.)) ;

  dh->SetStats (0) ;
  dh->Draw ("hist") ;

  dh->SetLabelSize(0, "Y");
  dh->SetTickLength(0, "Y");
  
  TF1 *transform = new TF1("transform", axistransform,
                           dh->GetMinimum(), dh->GetMaximum(), 0) ;

//  TF1 transform ("transform", axistransform, dh->GetMaximum (), dh->GetMaximum (), 0) ;


  //https://root.cern.ch/doc/v608/classTHistPainter.html
  // TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
  //  gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
  // THESE ARE BUGGED
  // TGaxis newaxis (
    // clims.GetUxmax (), 
    // clims.GetUymin (),
    // clims.GetUxmax (), 
    // clims.GetUymax (),

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

  clims.SaveAs ("problem.png") ;

  return 0 ;
}