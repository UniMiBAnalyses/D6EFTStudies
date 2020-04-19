// utilities for the datacard creator

#ifndef dcutils_h
#define dcutils_h

#include "TH1F.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLegend.h"

#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "CfgParser.h"

typedef unsigned long ulong ;

typedef std::pair<std::string, std::vector<float> > limits_var ;
typedef std::vector<limits_var> limits_var_v ;
typedef std::pair<std::string, limits_var_v > limits_op ;
typedef std::vector<limits_op> limits_op_v ;

const std::vector<std::string> gAllCoeff 
  {
    "cG",
    "cGtil",
    "cH",
    "cHB",
    "cHBtil",
    "cHDD",
    "cHG",
    "cHGtil",
    "cHW",
    "cHWB",
    "cHWBtil",
    "cHWtil",
    "cHbox",
    "cHd",
    "cHe",
    "cHl1",
    "cHl3",
    "cHq1",
    "cHq3",
    "cHu",
    "cHudAbs",
    "cHudPh",
    "cW",
    "cWtil",
    "cdBAbs",
    "cdBPh",
    "cdGAbs",
    "cdGPh",
    "cdHAbs",
    "cdHPh",
    "cdWAbs",
    "cdWPh",
    "cdd",
    "cdd1",
    "ceBAbs",
    "ceBPh",
    "ceHAbs",
    "ceHPh",
    "ceWAbs",
    "ceWPh",
    "ced",
    "cee",
    "ceu",
    "cld",
    "cle",
    "cledqAbs",
    "cledqPh",
    "clequ1Abs",
    "clequ1Ph",
    "clequ3Abs",
    "clequ3Ph",
    "cll",
    "cll1",
    "clq1",
    "clq3",
    "clu",
    "cqd1",
    "cqd8",
    "cqe",
    "cqq1",
    "cqq11",
    "cqq3",
    "cqq31",
    "cqu1",
    "cqu8",
    "cquqd1Abs",
    "cquqd1Ph",
    "cquqd8Abs",
    "cquqd8Ph",
    "cuBAbs",
    "cuBPh",
    "cuGAbs",
    "cuGPh",
    "cuHAbs",
    "cuHPh",
    "cuWAbs",
    "cuWPh",
    "cud1",
    "cud8",
    "cuu",
    "cuu1"
  } ;

const std::map<std::string, std::string> gvarNames 
  {
    std::pair<std::string, std::string> ("met"       , "MET"            ),
    std::pair<std::string, std::string> ("mjj"       , "m_{jj}"         ),
    std::pair<std::string, std::string> ("mll"       , "m_{#ell #ell}"  ),
    std::pair<std::string, std::string> ("ptl1"      , "p_{T}^{#ell_{1}}"   ),
    std::pair<std::string, std::string> ("ptl2"      , "p_{T}^{#ell_{2}}"   ),
    std::pair<std::string, std::string> ("ptll"      , "p_{T}^{#ell#ell}" ),
    std::pair<std::string, std::string> ("ptj1"      , "p_{T}^{j_{1}}"      ),
    std::pair<std::string, std::string> ("ptj2"      , "p_{T}^{j_{2}}"      ),
    std::pair<std::string, std::string> ("etaj1"     , "#eta_{j_{1}}"      ),
    std::pair<std::string, std::string> ("etaj2"     , "#eta_{j_{2}}"      ),
    std::pair<std::string, std::string> ("deltaetajj", "#Delta#eta_{jj}"),
    std::pair<std::string, std::string> ("deltaphijj", "#Delta#phi_{jj}"),
    std::pair<std::string, std::string> ("noshape"   , "XS"             )
  } ;


void setTDRStyle () ;

void replaceChar (std::string & str, char ch1, char ch2) ;

std::vector<std::string> 
split (const std::string & s, char delimiter) ;

std::string
merge (const std::vector<std::string> & tokens, char delimiter) ;

bool 
replace (std::string& str, const std::string& from, const std::string& to) ;

std::vector<float> 
calcBinEdges (std::string varname, 
              float min, float max, int nbins) ;

std::map<std::string, TH1F *> 
readNtupleFile (
    std::string rootFileName, 
    std::string ntupleName,
    std::string hTag, 
    std::string hTitle,
    CfgParser * gConfigParser) ;

void 
scaleAllHistos (std::map<std::string, TH1F *> & hMap, float value) ;

void 
checkEmptyBins (std::map<std::string, TH1F *> & hMap) ;

std::vector <std::string>
prepareFreeze (std::vector<std::string> activeCoeff, 
               float range = 2.) ;

std::string
merge (std::vector<std::string> list, const std::string & joint) ;

std::pair <std::string, std::string>
createDataCard (TH1F * h_SM, 
                std::map<std::string, TH1F *> h_eftInput, 
                std::string destinationfolder, 
                std::string prefix, 
                std::string varname,
                std::vector<std::string> active_coeffs, 
                std::vector<std::string> active_ranges, 
                CfgParser * gConfigParser) ;

std::string 
findAfter (const std::vector<std::string> & command, const std::string & token) ;

void
createCondorScripts (std::pair <std::string, std::string> fittingCommands,
                     std::string output_folder,
                     std::string output_last_folder,
                     std::string cmssw_folder,
                     std::string varname, 
                     std::string queue = "espresso") ;

int 
plotHistos (TH1F * h_SM, std::map<std::string, TH1F *> h_eftInput,
            std::string destinationfolder, std::string prefix, std::string varname, 
            std::vector<float> h_rescales, 
            bool log = false) ;

// set the likelihood scan aestetics
void setLSaspect (TGraph * graphScan, std::string variable) ;

std::pair<float, float> 
getXrange (TGraph * graphScan) ;

// find the minimum of a likelihood scan
float 
getLSminimum (TGraph * graphScan) ;

// get the list of crossings between a horizontal line and a tgraph
// points should be dense enough
std::vector <float>
getLSintersections (TGraph * graphScan, float val) ;

bool 
sortBySensitivity (const limits_var & a ,
                   const limits_var & b) ;

TH1F * 
setDummyHisto (float xmin, float ymin, float xmax, float ymax, 
               std::vector<std::string> labels) ;

void 
drawSensitivities (std::string op, 
                   limits_var_v limits, 
                   std::string basefilename,
                   bool message = false) ;

void 
writeCSVlimits (limits_op_v all_limits, 
                std::string basefilename,
                bool message = false) ;


struct bridgeElem
{
  public:
    bridgeElem (std::string v_v1 = "dummy", float v_v2 = 0, float v_v3 = 0, std::string v_v4 = ""):
      v1 (v_v1),  
      v2 (v_v2),     
      v3 (v_v3),     
      v4 (v_v4) {}
  
    std::string  v1 ; // coeff name
    float        v2 ;    // coeff value for plotting
    float        v3 ;    // coeff value in generation
    std::string  v4 ;    // coeff range for fitting
} ;


bool
sortByFirstElem (const bridgeElem & a , const bridgeElem & b) ;


struct bridge 
{
  public:
    bridge (std::vector<std::string> &v1, std::vector<float> &v2, std::vector<float> &v3, std::vector<std::string> &v4) ;
    void pour (std::vector<std::string> &v1, std::vector<float> &v2, std::vector<float> &v3, std::vector<std::string> &v4) ;
    std::vector<bridgeElem> m_container ;
//    std::vector<std::pair<std::string, std::vector<float> > > m_container ;
} ;

void
jointSort (std::vector<std::string> &v1, std::vector<float> &v2, std::vector<float> &v3, std::vector<std::string> &v4) ;

void 
copyFile (const std::string & destination, const std::string & source) ;

// histogram absolute value
void 
habs (TH1F * original) ;

// split histo in positive and negative bins
std::pair<TH1F *, TH1F *> 
splitH (TH1F * input) ;

class plotter
{
  public:
    plotter (std::string v_axistitle) ;
    void addHisto (TH1F * h, std::string label, std::string legendmode, int linecolor, int fillcolor = 0) ;
    void plot (std::string name, std::string type, bool log = false) ;


  private:
    std::string axistitle ;
    TLegend leg ;
    TH1F * highest ;
    float maxbinheight ;
    float minmaxbinheight ;
    std::vector<TH1F *> histos ;
} ;


#endif