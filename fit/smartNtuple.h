#ifndef smartNtuple_h
#define smartNtuple_h

#include <string>
#include <map>

#include "TFile.h"
#include "TNtuple.h"

/** 
  read TNtuples automatically implementing all the root branch stuff
*/
class smartNtuple
{
  public:

    smartNtuple (const std::string & filename, const std::string & ntuplename) ;
    float getVar (const std::string & varname) ;
    void loadEntry () ; //FIXME to be checked
  
    ~smartNtuple () ;

  private: 

    TFile * m_f ;
    TNtuple * m_nt ;
    std::map<std::string, float> m_vars ;

} ;


#endif