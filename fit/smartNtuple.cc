#include "smartNtuple.h"

#include "TObjArray.h"

using namespace std ;

smartNtuple::smartNtuple (const string & filename, const string & ntuplename) :
  m_f (new TFile (filename.c_str ())),
  m_nt ((TNtuple *) m_f->Get (ntuplename.c_str ()))
{
  // get list of variables
  TObjArray * leaves = m_nt->GetListOfLeaves();
  int nleaves = leaves ? leaves->GetEntriesFast () : 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


smartNtuple::~smartNtuple () 
{}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


float smartNtuple::getVar (const string & varname) 
{
	return 0. ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void smartNtuple::loadEntry ()  //FIXME to be checked
{}