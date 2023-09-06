#!/usr/bin/env python

# prepare command files to be passed to Madgraph to produce the folders
# for the event generation, for linear and quadratic BSM components
# when a single EFT operator is turned on.
# The script does not submit the folder event generation,
# since I am not sure that the right environment would be setup.
# to submit the folder generation:  for fil in `ls | grep launch_WZ` ; do ./bin/mg5_aMC $fil ;done

import os
import sys


if __name__ == "__main__":


  switchOn = ['21','24','25','28','32','45','46','48','49','53','54','55','56','57','58']
#  switchOn = ['21']
  switchOn = [str(i) for i in range(83)]

  params = [('0', 'cG'),
('1', 'cW'),
('2', 'cH'),
('3', 'cHbox'),
('4', 'cHDD'),
('5', 'cHG'),
('6', 'cHW'),
('7', 'cHB'),
('8', 'cHWB'),
('9', 'ceHRe'),
('10', 'cuHRe'),
('11', 'cdHRe'),
('12', 'ceWRe'),
('13', 'ceBRe'),
('14', 'cuGRe'),
('15', 'cuWRe'),
('16', 'cuBRe'),
('17', 'cdGRe'),
('18', 'cdWRe'),
('19', 'cdBRe'),
('20', 'cHl1'),
('21', 'cHl3'),
('22', 'cHe'),
('23', 'cHq1'),
('24', 'cHq3'),
('25', 'cHu'),
('26', 'cHd'),
('27', 'cHudRe'),
('28', 'cll'),
('29', 'cll1'),
('30', 'cqq1'),
('31', 'cqq11'),
('32', 'cqq3'),
('33', 'cqq31'),
('34', 'clq1'),
('35', 'clq3'),
('36', 'cee'),
('37', 'cuu'),
('38', 'cuu1'),
('39', 'cdd'),
('40', 'cdd1'),
('41', 'ceu'),
('42', 'ced'),
('43', 'cud1'),
('44', 'cud8'),
('45', 'cle'),
('46', 'clu'),
('47', 'cld'),
('48', 'cqe'),
('49', 'cqu1'),
('50', 'cqu8'),
('51', 'cqd1'),
('52', 'cqd8'),
('53', 'cledqRe'),
('54', 'cquqd1Re'),
('55', 'cquqd11Re'),
('56', 'cquqd8Re'),
('57', 'cquqd81Re'),
('58', 'clequ1Re'),
('59', 'clequ3Re'),     
    ]

  proc_ID = 'VBSOSWW'

  # # generate the linear component folders
  # for param in params:
  #   if param[0] not in switchOn : continue   
  #   f_launchfile = open ('launch_' + proc_ID + '_' + param[1] + '_LI.txt', 'w')
  #   f_launchfile.write ('import model SMEFTsim_A_U35_MwScheme_UFO_v3_1-' + param[1] + '_massless\n')
  #   f_launchfile.write ('generate    p p > e+ e- mu+ vm  j j QCD=0 NP=1 NP^2==1 SMHLOOP=0\n')
  #   f_launchfile.write ('add process p p > e+ e- mu- vm~ j j QCD=0 NP=1 NP^2==1 SMHLOOP=0\n')
  #   # f_launchfile.write ('add process p p > e+ e- e- ve~  j j QCD=0 NP=1 NP^2==1 SMHLOOP=0\n')
  #   # f_launchfile.write ('add process p p > e+ e- e+ ve   j j QCD=0 NP=1 NP^2==1 SMHLOOP=0\n')
  #   f_launchfile.write ('output ' + proc_ID + '_' + param[1] + '_LI')
  #   f_launchfile.close ()

  # generate the quadratic component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_' + proc_ID + '_' + param[1] + '_QU.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO_b_massless-' + param[1] + '_massless\n')
    f_launchfile.write ('generate    p p > e+ ve e- ve~  j j QCD=0 NP=1 NP^2==2 SMHLOOP=0\n')
    f_launchfile.write ('generate    p p > e+ ve mu- vm~  j j QCD=0 NP=1 NP^2==2 SMHLOOP=0\n')
    f_launchfile.write ('output ' + proc_ID + '_' + param[1] + '_QU')
    f_launchfile.close ()

  # # generate the SM component
  # if (len (sys.argv) > 1):
  #   f_launchfile = open ('launch_' + proc_ID + '_SM.txt', 'w')
  #   f_launchfile.write ('import model SMEFTsim_A_U35_MwScheme_UFO_v3_1-SMlimit_massless\n')
  #   f_launchfile.write ('generate    p p > e+ e- mu+ vm  j j QCD=0 SMHLOOP=0\n')
  #   f_launchfile.write ('add process p p > e+ e- mu- vm~ j j QCD=0 SMHLOOP=0\n')
  #   # f_launchfile.write ('add process p p > e+ e- e- ve~  j j QCD=0 SMHLOOP=0\n')
  #   # f_launchfile.write ('add process p p > e+ e- e+ ve   j j QCD=0 SMHLOOP=0\n')
  #   f_launchfile.write ('output ' + proc_ID + '_SM')
  #   f_launchfile.close ()

