#!/usr/bin/python

# prepare command files to be passed to Madgraph to produce the folders
# for the event generation, for linear and quadratic BSM components
# when a single EFT operator is turned on.
# The script does not submit the folder event generation,
# since I am not sure that the right environment would be setup.
# to submit the folder generation:  for fil in `ls | grep launch_` ; do ./bin/mg5_aMC $fil ;done


import os
import sys

if __name__ == "__main__":

#  switchOn = ['21','24','25','28','32','45','46','48','49','53','54','55','56','57','58']
  switchOn = ['21', '32', '46', '48', '49', '54']

  if (len (switchOn) < 2):
    print ('at least two operators needed, exiting')
    sys.exit (1)

  params = [[' 1', 'ceWPh'],      
      [' 2', 'ceBPh'],     
      [' 3', 'cuGPh'],     
      [' 4', 'cuWPh'],     
      [' 5', 'cuBPh'],     
      [' 6', 'cdGPh'],     
      [' 7', 'cdWPh'],     
      [' 8', 'cdBPh'],     
      [' 9', 'cHudPh'],        
      ['10', 'ceHPh'],      
      ['11', 'cuHPh'],      
      ['12', 'cdHPh'],      
      ['13', 'cledqPh'],          
      ['14', 'cquqd1Ph'],            
      ['15', 'cquqd8Ph'],            
      ['16', 'clequ1Ph'],            
      ['17', 'clequ3Ph'],            
      ['19', 'cG'],
      ['20', 'cGtil'],  
      ['21', 'cW'],  
      ['22', 'cWtil'],  
      ['23', 'cH'],
      ['24', 'cHbox'],  
      ['25', 'cHDD'],
      ['26', 'cHG'],
      ['27', 'cHGtil'],  
      ['28', 'cHW'],  
      ['29', 'cHWtil'],    
      ['30', 'cHB'],
      ['31', 'cHBtil'],    
      ['32', 'cHWB'],
      ['33', 'cHWBtil'],      
      ['34', 'ceHAbs'],               
      ['35', 'cuHAbs'],    
      ['36', 'cdHAbs'],    
      ['37', 'ceWAbs'],    
      ['38', 'ceBAbs'],    
      ['39', 'cuGAbs'],    
      ['40', 'cuWAbs'],    
      ['41', 'cuBAbs'],    
      ['42', 'cdGAbs'],    
      ['43', 'cdWAbs'],    
      ['44', 'cdBAbs'],    
      ['45', 'cHl1'],
      ['46', 'cHl3'],
      ['47', 'cHe'],
      ['48', 'cHq1'],
      ['49', 'cHq3'],
      ['50', 'cHu'],
      ['51', 'cHd'],
      ['52', 'cHudAbs'],      
      ['53', 'cll'],  
      ['54', 'cll1'],    
      ['55', 'cqq1'],    
      ['56', 'cqq11'],      
      ['57', 'cqq3'],    
      ['58', 'cqq31'],      
      ['59', 'clq1'],    
      ['60', 'clq3'],    
      ['61', 'cee'],  
      ['62', 'cuu'],  
      ['63', 'cuu1'],    
      ['64', 'cdd'],  
      ['65', 'cdd1'],    
      ['66', 'ceu'],  
      ['67', 'ced'],  
      ['68', 'cud1'],    
      ['69', 'cud8'],    
      ['70', 'cle'],  
      ['71', 'clu'],  
      ['72', 'cld'],  
      ['73', 'cqe'],  
      ['74', 'cqu1'],    
      ['75', 'cqu8'],    
      ['76', 'cqd1'],    
      ['77', 'cqd8'],    
      ['78', 'cledqAbs'],            
      ['79', 'cquqd1Abs'],        
      ['80', 'cquqd8Abs'],             
      ['81', 'clequ1Abs'],         
      ['82', 'clequ3Abs']
    ]

  selected = [x for x in params if x[0] in switchOn]

# sort the list alphabetically according to the parameter name
  from operator import itemgetter
  sortedsel = sorted (selected, key = itemgetter (1))

  for i in range (len (sortedsel)):
    for j in range (i+1, len (sortedsel)):
      tag = sortedsel[i][1] + '_' + sortedsel[j][1]

      f_launchfile = open ('launch_inWW_' + tag + '_IN.txt', 'w')
      f_launchfile.write ('import model SMEFTsim_A_U35_MwScheme_UFO_v3_1-' + tag + '_massless\n')
      f_launchfile.write ('generate p p > e+ ve mu- vm~ NP==1  NP' + sortedsel[i][1] + '^2==1   NP' + sortedsel[j][1] + '^2==1\n')
      f_launchfile.write ('add process p p > e- ve~ mu+ vm NP==1  NP' + sortedsel[i][1] + '^2==1   NP' + sortedsel[j][1] + '^2==1\n')
      f_launchfile.write ('output inWW_' + tag + '_IN')
      f_launchfile.close ()

  sys.exit (0)


