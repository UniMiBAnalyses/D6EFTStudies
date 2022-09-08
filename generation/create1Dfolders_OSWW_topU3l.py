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


  switchOn = [str(i) for i in range(2, 180)]
  print(switchOn)
  
  params = [(" 1", "cG"),
("2", "cW"),
("3", "cH"),
("4", "cHbox"),
("5", "cHDD"),
("6", "cHG"),
("7", "cHW"),
("8", "cHB"),
("9", "cHWB"),
("10", "cuHRe"),
("11", "ctHRe"),
("12", "cdHRe"),
("13", "cbHRe"),
("14", "cuGRe"),
("15", "ctGRe"),
("16", "cuWRe"),
("17", "ctWRe"),
("18", "cuBRe"),
("19", "ctBRe"),
("20", "cdGRe"),
("21", "cbGRe"),
("22", "cdWRe"),
("23", "cbWRe"),
("24", "cdBRe"),
("25", "cbBRe"),
("26", "cHj1"),
("27", "cHQ1"),
("28", "cHj3"),
("29", "cHQ3"),
("30", "cHu"),
("31", "cHt"),
("32", "cHd"),
("33", "cHbq"),
("34", "cHudRe"),
("35", "cHtbRe"),
("36", "cjj11"),
("37", "cjj18"),
("38", "cjj31"),
("39", "cjj38"),
("40", "cQj11"),
("41", "cQj18"),
("42", "cQj31"),
("43", "cQj38"),
("44", "cQQ1"),
("45", "cQQ8"),
("46", "cuu1"),
("47", "cuu8"),
("48", "ctt"),
("49", "ctu1"),
("50", "ctu8"),
("51", "cdd1"),
("52", "cdd8"),
("53", "cbb"),
("54", "cbd1"),
("55", "cbd8"),
("56", "cud1"),
("57", "ctb1"),
("58", "ctd1"),
("59", "cbu1"),
("60", "cud8"),
("61", "ctb8"),
("62", "ctd8"),
("63", "cbu8"),
("64", "cutbd1Re"),
("65", "cutbd8Re"),
("66", "cju1"),
("67", "cQu1"),
("68", "cju8"),
("69", "cQu8"),
("70", "ctj1"),
("71", "ctj8"),
("72", "cQt1"),
("73", "cQt8"),
("74", "cjd1"),
("75", "cjd8"),
("76", "cQd1"),
("77", "cQd8"),
("78", "cbj1"),
("79", "cbj8"),
("80", "cQb1"),
("81", "cQb8"),
("82", "cjQtu1Re"),
("83", "cjQtu8Re"),
("84", "cjQbd1Re"),
("85", "cjQbd8Re"),
("86", "cjujd1Re"),
("87", "cjujd8Re"),
("88", "cjujd11Re"),
("89", "cjujd81Re"),
("90", "cQtjd1Re"),
("91", "cQtjd8Re"),
("92", "cjuQb1Re"),
("93", "cjuQb8Re"),
("94", "cQujb1Re"),
("95", "cQujb8Re"),
("96", "cjtQd1Re"),
("97", "cjtQd8Re"),
("98", "cQtQb1Re"),
("99", "cQtQb8Re"),
("100", "ceHRe"),
("101", "ceWRe"),
("102", "ceBRe"),
("103", "cHl1"),
("104", "cHl3"),
("105", "cHe"),
("106", "cll"),
("107", "cll1"),
("108", "clj1"),
("109", "clj3"),
("110", "cQl1"),
("111", "cQl3"),
("112", "cee"),
("113", "ceu"),
("114", "cte"),
("115", "ced"),
("116", "cbe"),
("117", "cje"),
("118", "cQe"),
("119", "clu"),
("120", "ctl"),
("121", "cld"),
("122", "cbl"),
("123", "cle"),
("124", "cledjRe"),
("125", "clebQRe"),
("126", "cleju1Re"),
("127", "cleQt1Re"),
("128", "cGtil"),
("129", "cWtil"),
("130", "cHGtil"),
("131", "cHWtil"),
("132", "cHBtil"),
("133", "cHWBtil"),
("134", "cuGIm"),
("135", "ctGIm"),
("136", "cuWIm"),
("137", "ctWIm"),
("138", "cuBIm"),
("139", "ctBIm"),
("140", "cdGIm"),
("141", "cbGIm"),
("142", "cdWIm"),
("143", "cbWIm"),
("144", "cdBIm"),
("145", "cbBIm"),
("146", "cuHIm"),
("147", "ctHIm"),
("148", "cdHIm"),
("149", "cbHIm"),
("150", "cHudIm"),
("151", "cHtbIm"),
("152", "cutbd1Im"),
("153", "cutbd8Im"),
("154", "cjQtu1Im"),
("155", "cjQtu8Im"),
("156", "cjQbd1Im"),
("157", "cjQbd8Im"),
("158", "cjujd1Im"),
("159", "cjujd8Im"),
("160", "cjujd11Im"),
("161", "cjujd81Im"),
("162", "cQtjd1Im"),
("163", "cQtjd8Im"),
("164", "cjuQb1Im"),
("165", "cjuQb8Im"),
("166", "cQujb1Im"),
("167", "cQujb8Im"),
("168", "cjtQd1Im"),
("169", "cjtQd8Im"),
("170", "cQtQb1Im"),
("171", "cQtQb8Im"),
("172", "ceHIm"),
("173", "ceWIm"),
("174", "ceBIm"),
("175", "cledjIm"),
("176", "clebQIm"),
("177", "cleju1Im"),
("178", "cleju3Im"),
("179", "cleQt1Im"),
    ]

  # generate the linear component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_OSWW_' + param[1] + '_LI.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_topU3l_MwScheme_UFO_b_massless-' + param[1] + '_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+ mu+ ta+\n")
    f_launchfile.write ("define l- = e- mu- ta-\n")
    f_launchfile.write ("define vl = ve vm vt\n")
    f_launchfile.write ("define vl~ = ve~ vm~ vt~\n")
    f_launchfile.write ('generate p p > l+ l- vl vl~ j j QCD=0 SMHLOOP=0 NP=1 NP^2==1\n')
    f_launchfile.write ('output OSWW_' + param[1] + '_LI')
    f_launchfile.close ()

  # generate the quadratic component folders
  for param in params:
    if param[0] not in switchOn : continue   

    f_launchfile = open ('launch_OSWW_' + param[1] + '_QU.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_topU3l_MwScheme_UFO_b_massless-' + param[1] + '_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+ mu+ ta+\n")
    f_launchfile.write ("define l- = e- mu- ta-\n")
    f_launchfile.write ("define vl = ve vm vt\n")
    f_launchfile.write ("define vl~ = ve~ vm~ vt~\n")
    f_launchfile.write ('generate p p > l+ l- vl vl~ j j QCD=0 SMHLOOP=0 NP=1 NP^2==2\n')
    f_launchfile.write ('output OSWW_' + param[1] + '_QU')
    f_launchfile.close ()

  # generate the SM component
  if (len (sys.argv) > 1):
    f_launchfile = open ('launch_OSWW_SM.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-SMlimit_massless\n')
    f_launchfile.write ("define p = g u c d s u~ c~ d~ s~ b b~\n")
    f_launchfile.write ("define j = p\n")
    f_launchfile.write ("define l+ = e+ mu+ ta+\n")
    f_launchfile.write ("define l- = e- mu- ta-\n")
    f_launchfile.write ("define vl = ve vm vt\n")
    f_launchfile.write ("define vl~ = ve~ vm~ vt~\n")
    f_launchfile.write ('generate p p > l+ l- vl vl~ j j QCD=0 SMHLOOP=0\n')
    f_launchfile.write ('output OSWW_SM')    
    f_launchfile.close ()

