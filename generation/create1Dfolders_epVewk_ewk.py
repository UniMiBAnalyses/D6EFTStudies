#!/usr/bin/env python

# prepare command files to be passed to Madgraph to produce the folders
# for the event generation, for linear and quadratic BSM components
# when a single EFT operator is turned on.
# The script does not submit the folder event generation,
# since I am not sure that the right environment would be setup.
# to submit the folder generation:  for fil in `ls | grep launch_` ; do ./bin/mg5_aMC $fil ;done

import os
import sys


if __name__ == "__main__":


  # switchOn = ['21','24','25','28','32','45','46','48','49','53','54','55','56','57','58']
  # switchOn = ['57', '58', '55', '56', '24', '28','53' ]
  # switchOn = ['105']
  #switchOn = ["70", "32", "2","4","7","8","9","17","26","27","28","29","30","31","35", "36", "37", "42"]
  #switchOn = [2,4,5,7,8,9,26,27,28,29,30,31,32,33,36,37,38,39,40,41,42,43,44,45,70,71,72,73,103,104,105,107,19,17,139,137,35,151]
  

  #switchOn = ['120', '121', '122' '123', '124', '125', '126', '127', '128', '129','112', '113', '114', '115', '116', '117', '118', '119', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '30', '31', '32', '34', '35'] 
  
  # bosonic [cuHRe,cuHIm,ctHRe,ctHIm,cdHRe,cdHIm,cbHRe,cbHIm,ceHRe,ceHIm]
  L65 = [10,11,12,13,100,148,149,150,151,174]
  L66 = [16,17,18,19,22,23,24,25,101,102,138,139,140,141,144,145,146,147,175,176]
  L67 = [26,27,28,29,30,31,32,33,34,35,103,104,105,152,153]

  gridpack = [ 2, 4, 7, 8, 9, 17, 103 ]
  switchOn = gridpack
  switchOn = [str(i) for i in switchOn]

  params = [
    ("1", "cG",         "SMEFT", "1"),
    ("2", "cW",         "SMEFT","2"),
    ("3", "cH",         "SMEFT", "3"), 
    ("4", "cHbox",      "SMEFT", "4"), 
    ("5", "cHDD",       "SMEFT", "5"),
    ("6", "cHG",        "SMEFT", "6"), 
    ("7", "cHW",        "SMEFT", "7"), 
    ("8", "cHB",        "SMEFT", "8"),
    ("9", "cHWB",       "SMEFT", "9"),
    ("10", "cuHRe",     "SMEFT", "10"),
    ("11", "ctHRe",     "SMEFT", "11"), 
    ("12", "cdHRe",     "SMEFT", "12"), 
    ("13", "cbHRe",     "SMEFT", "13"), 
    ("14", "cuGRe",     "SMEFT", "14"), 
    ("15", "ctGRe",     "SMEFT", "15"), 
    ("16", "cuWRe",     "SMEFT", "16"), 
    ("17", "ctWRe",     "SMEFT", "17"), 
    ("18", "cuBRe",     "SMEFT", "18"), 
    ("19", "ctBRe",     "SMEFT", "19"), 
    ("20", "cdGRe",     "SMEFT", "20"), 
    ("21", "cbGRe",     "SMEFT", "21"), 
    ("22", "cdWRe",     "SMEFT", "22"), 
    ("23", "cbWRe",     "SMEFT", "23"), 
    ("24", "cdBRe",     "SMEFT", "24"), 
    ("25", "cbBRe",     "SMEFT", "25"), 
    ("26", "cHj1",      "SMEFT", "26"), 
    ("27", "cHQ1",      "SMEFT", "27"), 
    ("28", "cHj3",      "SMEFT", "28"), 
    ("29", "cHQ3",      "SMEFT", "29"), 
    ("30", "cHu",       "SMEFT", "30"), 
    ("31", "cHt",       "SMEFT", "31"), 
    ("32", "cHd",       "SMEFT", "32"), 
    ("33", "cHbq",      "SMEFT", "33"),
    ("34", "cHudRe",    "SMEFT", "34"),
    ("35", "cHtbRe",    "SMEFT", "35"), 
    ("36", "cjj11",     "SMEFT", "36"), 
    ("37", "cjj18",     "SMEFT", "37"), 
    ("38", "cjj31",     "SMEFT", "38"), 
    ("39", "cjj38",     "SMEFT", "39"), 
    ("40", "cQj11",     "SMEFT", "40"), 
    ("41", "cQj18",     "SMEFT", "41"), 
    ("42", "cQj31",     "SMEFT", "42"), 
    ("43", "cQj38",     "SMEFT", "43"), 
    ("44", "cQQ1",      "SMEFT", "44"), 
    ("45", "cQQ8",      "SMEFT", "45"), 
    ("46", "cuu1",      "SMEFT", "46"), 
    ("47", "cuu8",      "SMEFT", "47"), 
    ("48", "ctt",       "SMEFT", "48"), 
    ("49", "ctu1",      "SMEFT", "49"), 
    ("50", "ctu8",      "SMEFT", "50"), 
    ("51", "cdd1",      "SMEFT", "51"), 
    ("52", "cdd8",      "SMEFT", "52"), 
    ("53", "cbb",       "SMEFT", "53"), 
    ("54", "cbd1",      "SMEFT", "54"), 
    ("55", "cbd8",      "SMEFT", "55"), 
    ("56", "cud1",      "SMEFT", "56"), 
    ("57", "ctb1",      "SMEFT", "57"), 
    ("58", "ctd1",      "SMEFT", "58"), 
    ("59", "cbu1",      "SMEFT", "59"), 
    ("60", "cud8",      "SMEFT", "60"), 
    ("61", "ctb8",      "SMEFT", "61"), 
    ("62", "ctd8",      "SMEFT", "62"), 
    ("63", "cbu8",      "SMEFT", "63"), 
    ("64", "cutbd1Re",  "SMEFT", "64"), 
    ("65", "cutbd8Re",  "SMEFT", "65"), 
    ("66", "cju1",      "SMEFT", "66"), 
    ("67", "cQu1",      "SMEFT", "67"), 
    ("68", "cju8",      "SMEFT", "68"), 
    ("69", "cQu8",      "SMEFT", "69"), 
    ("70", "ctj1",      "SMEFT", "70"), 
    ("71", "ctj8",      "SMEFT", "71"), 
    ("72", "cQt1",      "SMEFT", "72"), 
    ("73", "cQt8",      "SMEFT", "73"), 
    ("74", "cjd1",      "SMEFT", "74"), 
    ("75", "cjd8",      "SMEFT", "75"), 
    ("76", "cQd1",      "SMEFT", "76"), 
    ("77", "cQd8",      "SMEFT", "77"), 
    ("78", "cbj1",      "SMEFT", "78"), 
    ("79", "cbj8",      "SMEFT", "79"), 
    ("80", "cQb1",      "SMEFT", "80"), 
    ("81", "cQb8",      "SMEFT", "81"), 
    ("82", "cjQtu1Re",  "SMEFT", "82"), 
    ("83", "cjQtu8Re",  "SMEFT", "83"), 
    ("84", "cjQbd1Re",  "SMEFT", "84"), 
    ("85", "cjQbd8Re",  "SMEFT", "85"), 
    ("86", "cjujd1Re",  "SMEFT", "86"), 
    ("87", "cjujd8Re",  "SMEFT", "87"), 
    ("88", "cjujd11Re", "SMEFT", "88"), 
    ("89", "cjujd81Re", "SMEFT", "89"), 
    ("90", "cQtjd1Re",  "SMEFT", "90"), 
    ("91", "cQtjd8Re",  "SMEFT", "91"), 
    ("92", "cjuQb1Re",  "SMEFT", "92"), 
    ("93", "cjuQb8Re",  "SMEFT", "93"), 
    ("94", "cQujb1Re",  "SMEFT", "94"), 
    ("95", "cQujb8Re",  "SMEFT", "95"), 
    ("96", "cjtQd1Re",  "SMEFT", "96"), 
    ("97", "cjtQd8Re",  "SMEFT", "97"), 
    ("98", "cQtQb1Re",  "SMEFT", "98"), 
    ("99", "cQtQb8Re",  "SMEFT", "99"), 
    ("100", "ceHRe",    "SMEFT", "100"), 
    ("101", "ceWRe",    "SMEFT", "101"), 
    ("102", "ceBRe",    "SMEFT", "102"), 
    ("103", "cHl1",     "SMEFT", "103"), 
    ("104", "cHl3",     "SMEFT", "104"), 
    ("105", "cHe",      "SMEFT", "105"), 
    ("106", "cll",      "SMEFT", "106"), 
    ("107", "cll1",     "SMEFT", "107"), 
    ("108", "clj1",     "SMEFT", "108"), 
    ("109", "clj3",     "SMEFT", "109"), 
    ("110", "cQl1",     "SMEFT", "110"), 
    ("111", "cQl3",     "SMEFT", "111"), 
    ("112", "cee",      "SMEFT", "112"), 
    ("113", "ceu",      "SMEFT", "113"), 
    ("114", "cte",      "SMEFT", "114"), 
    ("115", "ced",      "SMEFT", "115"), 
    ("116", "cbe",      "SMEFT", "116"), 
    ("117", "cje",      "SMEFT", "117"), 
    ("118", "cQe",      "SMEFT", "118"), 
    ("119", "clu",      "SMEFT", "119"), 
    ("120", "ctl",      "SMEFT", "120"), 
    ("121", "cld",      "SMEFT", "121"), 
    ("122", "cbl",      "SMEFT", "122"), 
    ("123", "cle",      "SMEFT", "123"), 
    ("124", "cledjRe",  "SMEFT", "124"), 
    ("125", "clebQRe",  "SMEFT", "125"), 
    ("126", "cleju1Re", "SMEFT", "126"), 
    ("127", "cleQt1Re", "SMEFT", "127"), 
    ("128", "cleju3Re", "SMEFT", "128"), 
    ("129", "cleQt3Re", "SMEFT", "129"),
    #SMEFTcpv
    ("130", "cGtil", "SMEFTcpv", "1"),
    ("131", "cWtil", "SMEFTcpv", "2"), 
    ("132", "cHGtil", "SMEFTcpv", "3"),
    ("133", "cHWtil", "SMEFTcpv", "4"),
    ("134", "cHBtil", "SMEFTcpv", "5"),
    ("135", "cHWBtil", "SMEFTcpv", "6"), 
    ("136", "cuGIm", "SMEFTcpv", "7"), 
    ("137", "ctGIm", "SMEFTcpv", "8"), 
    ("138", "cuWIm", "SMEFTcpv", "9"), 
    ("139", "ctWIm", "SMEFTcpv", "10"), 
    ("140", "cuBIm", "SMEFTcpv", "11"), 
    ("141", "ctBIm", "SMEFTcpv", "12"), 
    ("142", "cdGIm", "SMEFTcpv", "13"), 
    ("143", "cbGIm", "SMEFTcpv", "14"), 
    ("144", "cdWIm", "SMEFTcpv", "15"), 
    ("145", "cbWIm", "SMEFTcpv", "16"), 
    ("146", "cdBIm", "SMEFTcpv", "17"), 
    ("147", "cbBIm", "SMEFTcpv", "18"), 
    ("148", "cuHIm", "SMEFTcpv", "19"), 
    ("149", "ctHIm", "SMEFTcpv", "20"), 
    ("150", "cdHIm", "SMEFTcpv", "21"), 
    ("151", "cbHIm", "SMEFTcpv", "22"), 
    ("152", "cHudIm", "SMEFTcpv", "23"), 
    ("153", "cHtbIm", "SMEFTcpv", "24"), 
    ("154", "cutbd1Im", "SMEFTcpv", "25"), 
    ("155", "cutbd8Im", "SMEFTcpv", "26"), 
    ("156", "cjQtu1Im", "SMEFTcpv", "27"), 
    ("157", "cjQtu8Im", "SMEFTcpv", "28"), 
    ("158", "cjQbd1Im", "SMEFTcpv", "29"), 
    ("159", "cjQbd8Im", "SMEFTcpv", "30"), 
    ("160", "cjujd1Im", "SMEFTcpv", "31"), 
    ("161", "cjujd8Im", "SMEFTcpv", "32"), 
    ("162", "cjujd11Im", "SMEFTcpv", "33"), 
    ("163", "cjujd81Im", "SMEFTcpv", "34"), 
    ("164", "cQtjd1Im", "SMEFTcpv", "35"), 
    ("165", "cQtjd8Im", "SMEFTcpv", "36"), 
    ("166", "cjuQb1Im", "SMEFTcpv", "37"), 
    ("167", "cjuQb8Im", "SMEFTcpv", "38"), 
    ("168", "cQujb1Im", "SMEFTcpv", "39"), 
    ("169", "cQujb8Im", "SMEFTcpv", "40"), 
    ("170", "cjtQd1Im", "SMEFTcpv", "41"), 
    ("171", "cjtQd8Im", "SMEFTcpv", "42"), 
    ("172", "cQtQb1Im", "SMEFTcpv", "43"), 
    ("173", "cQtQb8Im", "SMEFTcpv", "44"), 
    ("174", "ceHIm", "SMEFTcpv", "45"), 
    ("175", "ceWIm", "SMEFTcpv", "46"), 
    ("176", "ceBIm", "SMEFTcpv", "47"), 
    ("177", "cledjIm", "SMEFTcpv", "48"), 
    ("178", "clebQIm", "SMEFTcpv", "49"), 
    ("179", "cleju1Im", "SMEFTcpv", "50"), 
    ("180", "cleju3Im", "SMEFTcpv", "51"), 
    ("181", "cleQt1Im", "SMEFTcpv", "52"), 
    ("182", "cleQt3Im", "SMEFTcpv", "53")
  ]
  # generate the linear component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_epVewk_' + param[1] + '_LI.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_topU3l_MwScheme_UFO_b_massless-' + param[1] + '_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+\n")
    f_launchfile.write ("define l- = e-\n")
    f_launchfile.write ("define vl = ve\n")
    f_launchfile.write ("define vl~ = ve~\n")
    f_launchfile.write ('generate p p > l+ vl j j j j QCD=0 SMHLOOP=0 NP=1 NP^2==1\n')
    f_launchfile.write ('output epVewk_' + param[1] + '_LI')
    f_launchfile.close ()

  # generate the quadratic component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_epVewk_' + param[1] + '_QU.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_topU3l_MwScheme_UFO_b_massless-' + param[1] + '_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+\n")
    f_launchfile.write ("define l- = e-\n")
    f_launchfile.write ("define vl = ve\n")
    f_launchfile.write ("define vl~ = ve~\n")
    f_launchfile.write ('generate p p > l+ vl j j j j QCD=0 SMHLOOP=0 NP=1 NP^2==2\n')
    f_launchfile.write ('output epVewk_' + param[1] + '_QU')
    f_launchfile.close ()

  # generate the SM component
  if (len (sys.argv) > 1):
    f_launchfile = open ('launch_epVewk_SM.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_topU3l_MwScheme_UFO_b_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+\n")
    f_launchfile.write ("define l- = e-\n")
    f_launchfile.write ("define vl = ve\n")
    f_launchfile.write ("define vl~ = ve~\n")
    f_launchfile.write ('generate p p > l+ vl j j j j QCD=0 SMHLOOP=0\n')
    f_launchfile.write ('output epVewk_SM')    
    f_launchfile.close ()

