#include "sputils.h"

#include <fstream>
#include <sstream>


using namespace std ;


void 
entry::print ()
{ 
  cout << op << ",\t" << var << ",\t" << osm << ",\t" << osM << ",\t" << osD
       << ",\t" << tsm << ",\t" << tsM << ",\t" << tsD << "\n" ; 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector<string> 
split (const string& s, char delimiter)
{
   vector<string> tokens;
   string token ;
   istringstream tokenStream (s) ;
   while (getline (tokenStream, token, delimiter))
   {
      tokens.push_back (token) ;
   }
   return tokens ;
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


TH1F * setDummyHisto (float xmin, float ymin, float xmax, float ymax, 
                      vector<string> labels)
{
  TH1F * dh = new TH1F ("dh", "", xmax - xmin, xmin, xmax) ;
  dh->Fill (xmin + 0.1 * (xmax - xmin), ymin) ;
  dh->Fill (xmin + 0.9 * (xmax - xmin), ymax) ;
  dh->SetStats (0) ;
  dh->SetFillColor (0) ;
  dh->SetLineColor (10) ;
  for (int i = 1 ; i <= labels.size () ; ++i) 
    dh->GetXaxis ()->SetBinLabel (i, labels[i-1].c_str ()) ;
  return dh ;
}

