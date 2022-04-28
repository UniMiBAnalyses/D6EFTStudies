#!/usr/bin/env python

import os
import sys
#import ConfigParser
import subprocess

if __name__ == "__main__":

    #switchOn = ['2','4','5','7','9','21','22','24','25','29','30','31','32','33','34']
    #switchOn = ['2', '4', '7', '21', '24', '25', '31', '32', '33', '34'] # production succeeded
    #switchOn = [str(i) for i in range(16, 61)]
    switchOn = [str(i) for i in range(1, 61)]
    params = [( '1' , 'cG'),
            ('2' , 'cW'),
            ('3' , 'cH'),
            ('4' , 'cHbox'),
            ('5' , 'cHDD'),
            ('6' , 'cHG'),
            ('7' , 'cHW'),
            ('8' , 'cHB'),
            ('9' , 'cHWB'),
            ('10' , 'ceHRe'),
            ('11' , 'cuHRe'),
            ('12' , 'cdHRe'),
            ('13' , 'ceWRe'),
            ('14' , 'ceBRe'),
            ('15' , 'cuGRe'),
            ('16' , 'cuWRe'),
            ('17' , 'cuBRe'),
            ('18' , 'cdGRe'),
            ('19' , 'cdWRe'),
            ('20' , 'cdBRe'),
            ('21' , 'cHl1'),
            ('22' , 'cHl3'),
            ('23' , 'cHe'),
            ('24' , 'cHq1'),
            ('25' , 'cHq3'),
            ('26' , 'cHu'),
            ('27' , 'cHd'),
            ('28' , 'cHudRe'),
            ('29' , 'cll'),
            ('30' , 'cll1'),
            ('31' , 'cqq1'),
            ('32' , 'cqq11'),
            ('33' , 'cqq3'),
            ('34' , 'cqq31'),
            ('35' , 'clq1'),
            ('36' , 'clq3'),
            ('37' , 'cee'),
            ('38' , 'cuu'),
            ('39' , 'cuu1'),
            ('40' , 'cdd'),
            ('41' , 'cdd1'),
            ('42' , 'ceu'),
            ('43' , 'ced'),
            ('44' , 'cud1'),
            ('45' , 'cud8'),
            ('46' , 'cle'),
            ('47' , 'clu'),
            ('48' , 'cld'),
            ('49' , 'cqe'),
            ('50' , 'cqu1'),
            ('51' , 'cqu8'),
            ('52' , 'cqd1'),
            ('53' , 'cqd8'),
            ('54' , 'cledqRe'),
            ('55' , 'cquqd1Re'),
            ('56' , 'cquqd11Re'),
            ('57' , 'cquqd8Re'),
            ('58' , 'cquqd81Re'),
            ('59' , 'clequ1Re'),
            ('60' , 'clequ3Re')]
    non_null_operators = []
    for param in params:
        if param[0] not in switchOn : continue
        if 'launch_Zjj_' + param[1] + '_QU.txt' in os.listdir('.'): continue

        f_launchfile = open ('launch_Zjj_' + param[1] + '_QU.txt', 'w')
        f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-' + param[1] + '_massless\n')
        f_launchfile.write('define p = g u c d s b u~ c~ d~ s~ b~\n')
        f_launchfile.write('define j = p\n')
        f_launchfile.write('define l+ = e+ mu+ ta+\n')
        f_launchfile.write('define l- = e- mu- ta-\n')
        f_launchfile.write ('generate p p > l+ l- j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==2\n')
        f_launchfile.write ('output Zjj_' + param[1] + '_QU')
        f_launchfile.close ()
        p = 'Zjj_'+param[1]+'_QU'
        cmd = subprocess.call("../MG5_aMC_v2_6_5/bin/mg5_aMC launch_"+p+".txt", shell=True)
        if not p in os.listdir('.'):
            print("folder for LI was not created by madgraph, exit")
            continue
        else:
            non_null_operators.append(param[0]+"QU")

        if 'launch_Zjj_' + param[1] + '_LI.txt' in os.listdir('.'): continue

        f_launchfile = open ('launch_Zjj_' + param[1] + '_LI.txt', 'w')
        f_launchfile.write ('import model SMEFTsim_U35_MwScheme_UFO-' + param[1] + '_massless\n')
        f_launchfile.write('define p = g u c d s b u~ c~ d~ s~ b~\n')
        f_launchfile.write('define j = p\n')
        f_launchfile.write('define l+ = e+ mu+ ta+\n')
        f_launchfile.write('define l- = e- mu- ta-\n')
        f_launchfile.write ('generate p p > l+ l- j j SMHLOOP=0 QED=99 QCD=0 NP=1 NP^2==1\n')
        f_launchfile.write ('output Zjj_' + param[1] + '_LI')
        f_launchfile.close ()
        p = 'Zjj_'+param[1]+'_LI'
        cmd = subprocess.call("../../MG5_aMC_v2_6_5/bin/mg5_aMC launch_"+p+".txt", shell=True)
        if not p in os.listdir('.'):
            print("folder for LI was not created by madgraph, exit")
        else:
            non_null_operators.append(param[0]+"LI")

    print(non_null_operators)
