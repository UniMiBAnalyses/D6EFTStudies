#!/usr/bin/env python

import os
import ConfigParser
import argparse

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Command line parser')
    parser.add_argument('--v', dest='verbose', help='Verbose prints', default = False, action = 'store_true', required = False)
    parser.add_argument('--qcd', dest='qcd', help='QCD production. Default is EWK', default = False, action = 'store_true', required = False)
    parser.add_argument('--diagfilter', dest='diagfilter', help='Force QCD==2', default = False, action = 'store_true', required = False)
    args = parser.parse_args()

    #switchOn = ['2','4','5','7','9','21','22','24','25','29','30','31','32','33','34']
    #switchOn = ['2', '7', '21', '24', '25', '31', '32', '33', '34']
    switchOn = ['2', '5', '21', '24', '25']
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
              ('52' , 'cqd1')] 

    if args.qcd:
        proc_IDs = [('ZWmQCD', 'w-'), ('ZWpQCD', 'w+'), ('ZZQCD', 'z')]
        MG_constr = 'QCD=99'
    else:
        proc_IDs = [('ZWm', 'w-'), ('ZWp', 'w+'), ('ZZ', 'z')]
        MG_constr = 'QCD=0'
    print ('Generating ' + ' and '.join(zip(*proc_IDs)[0]) + ' cards with constraint ' + MG_constr)

    for i in range(len(params)):
        for j in range(i+1, len(params)):
            if params[i][0] not in switchOn or params[j][0] not in switchOn: continue
            # reweight coefficients
            coefficients = [('0.0','0.0'), ('1.0','0.0'), ('0.0','1.0'), ('1.0','1.0'), ('0.0','-1.0'), ('-1.0','0.0')] # default
            # loop over processes
            for proc in proc_IDs:
                # clean directory
                prefix = '{0}_{1}_{2}_SM_LI_QU_IN'.format(proc[0], params[i][1], params[j][1])
                dirname = './{0}_{1}_{2}_SM_LI_QU_IN/'.format(proc[0], params[i][1], params[j][1])
                if os.path.isdir(dirname):
                    os.system('rm -rf ' + dirname)
                os.mkdir(dirname)
                # cards names
                runcard = dirname + prefix + '_run_card.dat'
                extramodels = dirname + prefix + '_extramodels.dat'
                proccard = dirname + prefix + '_proc_card.dat'
                customize = dirname + prefix + '_customizecards.dat'
                reweightcard = dirname + prefix + '_reweight_card.dat'
                # run card
                if args.verbose:
                    print ('\n[INFO] producing ' + prefix + '_run_card.dat')
                os.system('cp ./run_card.dat ' + runcard)
                # extramodels card
                if args.verbose:
                    print ('[INFO] producing ' + prefix + '_extramodels.dat')
                with open(extramodels, 'w') as e:
                    e.write('SMEFTsim_U35_MwScheme_UFO.tar.gz')
                # proc card
                if args.verbose:
                    print ('[INFO] producing ' + prefix + '_proc_card.dat')
                with open(proccard, 'w') as p:
                    p.write('set group_subprocesses Auto\n')
                    p.write('set ignore_six_quark_processes False\n')
                    p.write('set loop_optimized_output True\n')
                    p.write('set complex_mass_scheme False\n')
                    p.write('define p = g u c d s b u~ c~ d~ s~ b~\n')
                    p.write('define j = p\n')
                    p.write('define l+ = e+ mu+ ta+\n')
                    p.write('define l- = e- mu- ta-\n')
                    p.write('define vl = ve vm vt\n')
                    p.write('define vl~ = ve~ vm~ vt~\n')
                    p.write('import model SMEFTsim_U35_MwScheme_UFO-{0}_{1}_massless\n'.format(params[i][1], params[j][1]))
                    if args.diagfilter:
                        p.write('generate p p > z {0} j j {1} SMHLOOP=0 NP=1, z > l+ l- NP=1, {0} > j j NP=1 @0 NP=1 --diagram_filter\n'.format(proc[1], MG_constr))
                    else:
                        p.write('generate p p > z {0} j j {1} SMHLOOP=0 NP=1, z > l+ l- NP=1, {0} > j j NP=1 @0 NP=1\n'.format(proc[1], MG_constr))
                    p.write('output ' + prefix)
                # customize card
                if args.verbose:
                    print ('[INFO] producing ' + prefix + '_customizecards.dat')
                with open(customize, 'w') as c:
                    c.write('set param_card SMEFT {0} 1.0\n'.format(params[i][0]))
                    c.write('set param_card SMEFT {0} 1.0'.format(params[j][0]))
                # reweight card
                if args.verbose:
                    print ('[INFO] producing ' + prefix + '_reweight_card.dat\n')
                with open(reweightcard, 'w') as r:
                    if args.qcd:
                        r.write('change model SMEFTsim_U35_MwScheme_UFO-{0}_{1}_massless\n'.format(params[i][1], params[j][1]))
                    r.write('change helicity False\n')
                    if proc[0] == 'ZZ' or proc[0] == 'ZZQCD':
                        r.write('change keep_ordering True\n')
                    r.write('change rwgt_dir rwgt\n')
                    r.write('# {0} {1}\n'.format(params[i][0], params[i][1]))
                    r.write('# {0} {1}\n'.format(params[j][0], params[j][1]))
                    for coeff in coefficients:
                        r.write('launch\n')
                        r.write('    set SMEFT {0} {1}\n'.format(params[i][0], coeff[0]))
                        r.write('    set SMEFT {0} {1}\n'.format(params[j][0], coeff[1]))

    print ('Cards created successfully')
