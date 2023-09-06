# prepare command files to be passed to Madgraph to produce the folders
# for the event generation, for linear and quadratic BSM components
# when a single EFT operator is turned on.
# The script does not submit the folder event generation,
# since I am not sure that the right environment would be setup.
# to submit the folder generation:  for fil in `ls | grep launch_` ; do ./bin/mg5_aMC $fil ;done

import os


if __name__ == "__main__":

# THIS ARE THE ONES WHO SHOLD PASS THE SELECTION --> FOLDERS _LI & _QU SHOULD BE CREATED
  # switchOn = ['63', '64', '69', '62', '65', '68']
  # # 'cuu1' 'cdd' 'cud8' 'cuu' 'cdd1' 'cud1' 

# THIS ARE ALL THE PROBLEMATIC 14 OPERATORS FROM THE ORGINAL 39 
  # switchOn = ['83', '84', '85', '86', '63', '64', '69', '62', '65', '68', '24', '28', '77', '75']
  # cdHRe' 'cdBRe' 'cdWRe' 'cHudRe' 'cuu1' 'cdd' 'cud8' 'cuu' 'cdd1' 'cud1' 'cHbox' 'cHW' 'cqd8' 'cqu8'
  # switchOn = ['83']
  switchOn = ['77', '75']
  # the following operators do not contribute at LO in the W_EWK production process
  #  switchOn = ['53', '55', '56', '58']
  #  switchOn = ['21']
  # switchOn = ['46','54']
  params = [(' 1', 'ceWPh'),      
      (' 2', 'ceBPh'),     
      (' 3', 'cuGPh'),     
      (' 4', 'cuWPh'),     
      (' 5', 'cuBPh'),     
      (' 6', 'cdGPh'),     
      (' 7', 'cdWPh'),     
      (' 8', 'cdBPh'),     
      (' 9', 'cHudPh'),        
      ('10', 'ceHPh'),      
      ('11', 'cuHPh'),      
      ('12', 'cdHPh'),      
      ('13', 'cledqPh'),          
      ('14', 'cquqd1Ph'),            
      ('15', 'cquqd8Ph'),            
      ('16', 'clequ1Ph'),            
      ('17', 'clequ3Ph'),            
      ('19', 'cG'),
      ('20', 'cGtil'),  
      ('21', 'cW'),  
      ('22', 'cWtil'),  
      ('23', 'cH'),
      ('24', 'cHbox'),  
      ('25', 'cHDD'),
      ('26', 'cHG'),
      ('27', 'cHGtil'),  
      ('28', 'cHW'),  
      ('29', 'cHWtil'),    
      ('30', 'cHB'),
      ('31', 'cHBtil'),    
      ('32', 'cHWB'),
      ('33', 'cHWBtil'),      
      ('34', 'ceHAbs'),               
      ('35', 'cuHAbs'),    
      ('36', 'cdHAbs'),    
      ('37', 'ceWAbs'),    
      ('38', 'ceBAbs'),    
      ('39', 'cuGAbs'),    
      ('40', 'cuWAbs'),    
      ('41', 'cuBAbs'),    
      ('42', 'cdGAbs'),    
      ('43', 'cdWAbs'),    
      ('44', 'cdBAbs'),    
      ('45', 'cHl1'),
      ('46', 'cHl3'),
      ('47', 'cHe'),
      ('48', 'cHq1'),
      ('49', 'cHq3'),
      ('50', 'cHu'),
      ('51', 'cHd'),
      ('52', 'cHudAbs'),      
      ('53', 'cll'),  
      ('54', 'cll1'),    
      ('55', 'cqq1'),    
      ('56', 'cqq11'),      
      ('57', 'cqq3'),    
      ('58', 'cqq31'),      
      ('59', 'clq1'),    
      ('60', 'clq3'),    
      ('61', 'cee'),  
      ('62', 'cuu'),  
      ('63', 'cuu1'),    
      ('64', 'cdd'),  
      ('65', 'cdd1'),    
      ('66', 'ceu'),  
      ('67', 'ced'),  
      ('68', 'cud1'),    
      ('69', 'cud8'),    
      ('70', 'cle'),  
      ('71', 'clu'),  
      ('72', 'cld'),  
      ('73', 'cqe'),  
      ('74', 'cqu1'),    
      ('75', 'cqu8'),    
      ('76', 'cqd1'),    
      ('77', 'cqd8'),    
      ('78', 'cledqAbs'),            
      ('79', 'cquqd1Abs'),        
      ('80', 'cquqd8Abs'),             
      ('81', 'clequ1Abs'),         
      ('82', 'clequ3Abs'),
      ('83', 'cdHRe'),
      ('84', 'cdBRe'),
      ('85', 'cdWRe'),
      ('86', 'cHudRe')
    ]

  f_launchfile = open ('launch_W_EWK_SM.txt', 'w')
  f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-SMlimit_massless\n')
  f_launchfile.write ('define p = g u c d s b u~ c~ d~ s~ b~\n')
  f_launchfile.write ('define j = p\n')
  f_launchfile.write ('define l+ = e+ mu+ ta+\n')
  f_launchfile.write ('define l- = e- mu- ta-\n')
  f_launchfile.write ('define vl~ = ve~ vm~ vt~\n')
  f_launchfile.write ('define vl = ve vm vt\n')
  f_launchfile.write ('generate p p > l+ vl j j SMHLOOP=0 QED=99 QCD=0\n')
  f_launchfile.write ('add process p p > l- vl~ j j SMHLOOP=0 QED=99 QCD=0\n')
  f_launchfile.write ('output W_EWK_SM')
  f_launchfile.close ()

  # generate the linear component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_W_EWK_' + param[1] + '_LI.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-' + param[1] + '_massless\n')
    f_launchfile.write ('define p = g u c d s b u~ c~ d~ s~ b~\n')
    f_launchfile.write ('define j = p\n')
    f_launchfile.write ('define l+ = e+ mu+ ta+\n')
    f_launchfile.write ('define l- = e- mu- ta-\n')
    f_launchfile.write ('define vl~ = ve~ vm~ vt~\n')
    f_launchfile.write ('define vl = ve vm vt\n')
    f_launchfile.write ('generate p p > l+ vl j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==1\n')
    f_launchfile.write ('add process p p > l- vl~ j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==1\n')
    f_launchfile.write ('output W_EWK_' + param[1] + '_LI')
    f_launchfile.close ()

  # generate the quadratic component folders
  for param in params:
    if param[0] not in switchOn : continue   
    f_launchfile = open ('launch_W_EWK_' + param[1] + '_QU.txt', 'w')
    f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-' + param[1] + '_massless\n')
    f_launchfile.write ('define p = g u c d s b u~ c~ d~ s~ b~\n')
    f_launchfile.write ('define j = p\n')
    f_launchfile.write ('define l+ = e+ mu+ ta+\n')
    f_launchfile.write ('define l- = e- mu- ta-\n')
    f_launchfile.write ('define vl~ = ve~ vm~ vt~\n')
    f_launchfile.write ('define vl = ve vm vt\n')
    f_launchfile.write ('generate p p > l+ vl j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==2\n')
    f_launchfile.write ('add process p p > l- vl~ j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==2\n')
    f_launchfile.write ('output W_EWK_' + param[1] + '_QU')
    f_launchfile.close ()
