#ifndef sputils_h
#define sputils_h

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TTree.h"
#include "TString.h"
#include "TCut.h"
#include "TList.h"
#include "TGraph2D.h"

#include "dcutils.h"

struct entry
{
  public:
    entry (std::string o, std::string v, float a, float b, float c, float d) :
      op  (o), 
      var (v),
      osm (a),
      osM (b),
      osD (0.5 * (b-a)),
      tsm (c),
      tsM (d),
      tsD (0.5 * (d-c)) {}
    entry (const entry & orig) :
      op  (orig.op), 
      var (orig.var),
      osm (orig.osm),
      osM (orig.osM),
      osD (orig.osD),
      tsm (orig.tsm),
      tsM (orig.tsM),
      tsD (orig.tsD) {}
    entry () :
      op  ("dummy"), 
      var ("dummy"),
      osm (0.),
      osM (0.),
      osD (0.),
      tsm (0.),
      tsM (0.),
      tsD (0.) {}


    std::string op ;  // operator       
    std::string var ; // variable
    float osm ;  // one sigma min    
    float osM ;  // one sigma max   
    float osD ;  // one sigma half width
    float tsm ;  // two sigma min   
    float tsM ;  // two sigma max   
    float tsD ;  // two sigma half width   

    void print () ;
} ;

void
removeSpaces (std::vector<std::string> & vec) ;

std::vector<entry>
readCSV (std::string filename) ;

std::vector<std::string>
getOps (const std::vector<entry> & ds) ;

bool
sortByOpAndOS (const entry & a, const entry & b) ;

bool
equalByOp (const entry & a, const entry & b) ;

std::vector <float>
findOp (const std::vector<entry> & coll, const std::string & op) ;

template <class T>
void hardunique (std::vector<T> & vect)
{
  typename std::vector<T>::iterator it = std::unique (vect.begin (), vect.end ()) ;
  vect.resize (std::distance (vect.begin (), it)) ;
  return ;
}

template <class T, class BinaryPredicate>
void hardunique (std::vector<T> & vect,
                 BinaryPredicate pred)
{
  typename std::vector<T>::iterator it = std::unique (vect.begin (), vect.end (), pred) ;
  vect.resize (std::distance (vect.begin (), it)) ;
  return ;
}

TH2 *treeToHist2D(TTree *t, TString x, TString y, TString name, TCut cut, double xmin, double xmax, double ymin, double ymax, int xbins, int ybins) ;

TGraph2D * 
treeToGraph2D (TTree *t, TString x, TString y, TCut cut) ;

TList* contourFromTH2(TH2 *h2in, double threshold, int minPoints=20) ;

TH2D* frameTH2D(TH2D *in, double threshold) ;

#endif 