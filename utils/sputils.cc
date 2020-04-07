#include "sputils.h"

#include <fstream>
#include <sstream>

#include "TPad.h"

using namespace std ;


void 
entry::print ()
{ 
  cout << op << ",\t" << var << ",\t" << osm << ",\t" << osM << ",\t" << osD
       << ",\t" << tsm << ",\t" << tsM << ",\t" << tsD << "\n" ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void
removeSpaces (vector<string> & vec)
{
  for (int i = 0 ; i < vec.size () ; ++i)
     {
	   vec.at (i).erase (
	   	   std::remove_if (vec.at (i).begin(), vec.at (i).end (), ::isspace), 
	   	   vec.at (i).end ()
	   	   ) ;
     }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector<entry>
readCSV (string filename)
{
  ifstream file (filename) ;
 
//  vector<vector<string> > dataList;
 
  vector<entry> result ; 
  string line = "";
  // get the line headers 
  getline (file, line) ;
  // Iterate through each line and split the content using delimeter
  while (getline (file, line))
  {
    vector<string> raw = split (line, ',') ;
    removeSpaces (raw) ;
    result.push_back (entry (
      raw.at (0), raw.at (1), 
      stof (raw.at (2)),
      stof (raw.at (3)),
      stof (raw.at (4)),
      stof (raw.at (5))
    )) ;
  }
  // Close the File
  file.close () ;
  return result ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector<string>
getOps (const vector<entry> & ds)
{ 
  vector<string> ops ;
  for (int i = 0 ; i < ds.size () ; ++i)
    ops.push_back (ds.at (i).op) ;
  return ops ;   
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool
sortByOpAndOS (const entry & a, const entry & b)
{	 
  if (a.op < b.op) return true ;
  if ((a.op == b.op && a.osD < b.osD)) return true ;
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool
equalByOp (const entry & a, const entry & b)
{	 
  return (a.op == b.op) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


std::vector <float>
findOp (const vector<entry> & coll, const string & op)
  {
     vector<float> result = {0., 1., 1} ;

     for (int i = 0 ; i < coll.size () ; ++i)
       {
         if (coll.at (i).op == op)
           {
              result[0] = 1. ;
              result[1] = coll.at (i).osD ;              
              result[2] = coll.at (i).tsD ;
              break ;
           }
       }
     return result ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// copied from here https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/74x-root6/test/plotting/contours2D.cxx
TH2 *treeToHist2D(TTree *t, TString x, TString y, TString name, TCut cut, double xmin, double xmax, double ymin, double ymax, int xbins, int ybins) {
    t->Draw(Form("2*deltaNLL:%s:%s>>%s_prof(%d,%10g,%10g,%d,%10g,%10g)", y.Data(), x.Data(), name.Data(), xbins, xmin, xmax, ybins, ymin, ymax), cut + "deltaNLL != 0", "PROF");
    TH2 *prof = (TH2*) gROOT->FindObject(name+"_prof");
    TH2D *h2d = new TH2D(name, name, xbins, xmin, xmax, ybins, ymin, ymax);
    for (int ix = 1; ix <= xbins; ++ix) {
        for (int iy = 1; iy <= ybins; ++iy) {
             double z = prof->GetBinContent(ix,iy);
             if (z != z) z = (name.Contains("bayes") ? 0 : 999); // protect agains NANs
             h2d->SetBinContent(ix, iy, z);
        }
    }
    h2d->GetXaxis()->SetTitle(x);
    h2d->GetYaxis()->SetTitle(y);
    h2d->SetDirectory(0);
    return h2d;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TGraph2D * 
treeToGraph2D (TTree *t, TString x, TString y, TCut cut)
{
    Float_t c1, c2, dllr ;
    t->SetBranchAddress (x, &c1) ;
    t->SetBranchAddress (y, &c2) ;
    t->SetBranchAddress ("deltaNLL", &dllr) ;

    TGraph2D * g = new TGraph2D () ;
    int nentries = t->GetEntries () ;

    for (int i = 0 ; i < nentries ; ++i) 
      {
        t->GetEntry (i) ;
        g->SetPoint (i, c1, c2, 2 * dllr) ; 
      }
    
//    g->Draw () ; // attempt to get the axes
    g->GetXaxis ()->SetTitle (x) ;
    g->GetYaxis ()->SetTitle (y) ;
    g->SetTitle ("") ;
    return g ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// copied from here https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/74x-root6/test/plotting/contours2D.cxx
TList* contourFromTH2(TH2 *h2in, double threshold, int minPoints) {
    std::cout << "Getting contour at threshold " << threshold << " from " << h2in->GetName() << std::endl;
    //http://root.cern.ch/root/html/tutorials/hist/ContourList.C.html
    Double_t contours[1];
    contours[0] = threshold;
    if (h2in->GetNbinsX() * h2in->GetNbinsY() > 10000) minPoints = 50;
    if (h2in->GetNbinsX() * h2in->GetNbinsY() <= 100) minPoints = 10;

    TH2D *h2 = frameTH2D((TH2D*)h2in,threshold);

    h2->SetContour(1, contours);

    // Draw contours as filled regions, and Save points
    h2->Draw("CONT Z LIST");
    gPad->Update(); // Needed to force the plotting and retrieve the contours in TGraphs


    // Get Contours
    TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
    TList* contLevel = NULL;

    if (conts == NULL || conts->GetSize() == 0){
        printf("*** No Contours Were Extracted!\n");
        return 0;
    }

    TList *ret = new TList();
    for(int i = 0; i < conts->GetSize(); i++){
        contLevel = (TList*)conts->At(i);
        //printf("Contour %d has %d Graphs\n", i, contLevel->GetSize());
        for (int j = 0, n = contLevel->GetSize(); j < n; ++j) {
            TGraph *gr1 = (TGraph*) contLevel->At(j);
            gr1->SetTitle ("") ;
            //printf("\t Graph %d has %d points\n", j, gr1->GetN());
            if (gr1->GetN() > minPoints) ret->Add(gr1->Clone());
            //break;
        }
    }
    return ret;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// copied from here https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit/blob/74x-root6/test/plotting/contours2D.cxx
TH2D* frameTH2D(TH2D *in, double threshold){
  // NEW LOGIC:
  //   - pretend that the center of the last bin is on the border if the frame
  //   - add one tiny frame with huge values
  double frameValue = 1000;
  if (TString(in->GetName()).Contains("bayes")) frameValue = -1000;

  Double_t xw = in->GetXaxis()->GetBinWidth(1);
  Double_t yw = in->GetYaxis()->GetBinWidth(1);

  Int_t nx = in->GetNbinsX();
  Int_t ny = in->GetNbinsY();

  Double_t x0 = in->GetXaxis()->GetXmin();
  Double_t x1 = in->GetXaxis()->GetXmax();

  Double_t y0 = in->GetYaxis()->GetXmin();
  Double_t y1 = in->GetYaxis()->GetXmax();
  Double_t xbins[999], ybins[999]; 
  double eps = 0.1;

  xbins[0] = x0 - eps*xw - xw; xbins[1] = x0 + eps*xw - xw;
  for (int ix = 2; ix <= nx; ++ix) xbins[ix] = x0 + (ix-1)*xw;
  xbins[nx+1] = x1 - eps*xw + 0.5*xw; xbins[nx+2] = x1 + eps*xw + xw;

  ybins[0] = y0 - eps*yw - yw; ybins[1] = y0 + eps*yw - yw;
  for (int iy = 2; iy <= ny; ++iy) ybins[iy] = y0 + (iy-1)*yw;
  ybins[ny+1] = y1 - eps*yw + yw; ybins[ny+2] = y1 + eps*yw + yw;
        
  TH2D *framed = new TH2D(
      Form("%s framed",in->GetName()),
      Form("%s framed",in->GetTitle()),
      nx + 2, xbins,
      ny + 2, ybins 
      );

  //Copy over the contents
  for(int ix = 1; ix <= nx ; ix++){
    for(int iy = 1; iy <= ny ; iy++){
      framed->SetBinContent(1+ix, 1+iy, in->GetBinContent(ix,iy));
    }
  }
  //Frame with huge values
  nx = framed->GetNbinsX();
  ny = framed->GetNbinsY();
  for(int ix = 1; ix <= nx ; ix++){
    framed->SetBinContent(ix,  1, frameValue);
    framed->SetBinContent(ix, ny, frameValue);
  }
  for(int iy = 2; iy <= ny-1 ; iy++){
    framed->SetBinContent( 1, iy, frameValue);
    framed->SetBinContent(nx, iy, frameValue);
  }

  return framed;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


pair<float, float>
getMinCoordinates (TTree *t, TString x, TString y)
{
  float xmin;
  float ymin;
  
  t->SetBranchAddress (x, & xmin);
  t->SetBranchAddress (y, & ymin);
  
  t->GetEntry(0);
              
  return pair<float, float> (xmin, ymin) ;          
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float
getAreaWithinCL (TH2F * histo, float threshold)
{
  float area = 0. ;
  for (int i = 1 ; i < histo->GetNbinsX () + 1 ; ++i)
    for (int j = 1 ; j < histo->GetNbinsY () + 1 ; ++j)
      if (histo->GetBinContent (i,j) < threshold) ++area ;

  area *= histo->GetXaxis ()->GetBinWidth (1) * histo->GetYaxis ()->GetBinWidth (1) ;

  return area ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


bool
sortByArea (const pair<string, cont> & a, const pair<string, cont> & b)
{
  return a.second.area < b.second.area ;
}

