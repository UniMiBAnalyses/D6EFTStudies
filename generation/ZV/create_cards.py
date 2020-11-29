#!/usr/bin/env python

import os
import ConfigParser

if __name__ == "__main__":

    switchOn = ['21','24','25','28','32','45','46','48','49','53','54','55','56','57','58']

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
              ('82', 'clequ3Abs')
              ]

    config = ConfigParser.ConfigParser()
    config.readfp(open(r'reweightingConfig.cfg'))

    proc_IDs = [('ZWm', 'w-'), ('ZWp', 'w+'), ('ZZ', 'z')]

    for param in params:
        if param[0] not in switchOn: continue
        # get reweight coefficients from config
        if param[1].lower() not in config.options('reweighting'):
            coefficients = ['0.0', '-1.0', '1.0'] # default
        else:
            coefficients = [x.strip(' ') for x in config.get('reweighting', param[1].lower()).split(',')]
        # loop over processes
        rm_dir = True
        for proc in proc_IDs:
            # clean directory
            prefix = '{0}_{1}'.format(proc[0], param[1])
            dirname = './' + param[1] + '/'
            if rm_dir:
                if os.path.isdir(dirname):
                    os.system('rm -rf ' + dirname)
                os.mkdir(dirname)
            rm_dir = False
            dirname = dirname + '{0}_{1}/'.format(proc[0], param[1])
            os.mkdir(dirname)
            # cards names
            runcard = dirname + prefix + '_run_card.dat'
            extramodels = dirname + prefix + '_extramodels.dat'
            proccard = dirname + prefix + '_proc_card.dat'
            madspincard = dirname + prefix + '_madspin_card.dat'
            customize = dirname + prefix + '_customizecards.dat'
            reweightcard = dirname + prefix + '_reweight_card.dat'
            # run card
            os.system('cp ./run_card.dat ' + runcard)
            # extramodels card
            with open(extramodels, 'w') as e:
                e.write('SMEFTsim_A_U35_MwScheme_UFO_v3_1.tar.gz')
            # proc card
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
                p.write('import model SMEFTsim_A_U35_MwScheme_UFO_v3_1-{0}_massless\n'.format(param[1]))
                p.write('generate p p > z {0} j j QCD=0 NP=1 SMHLOOP=0\n'.format(proc[1]))
                p.write('output ' + prefix)
                #p.write('launch') # to be used if madspin is not needed
            # madspin card
            with open(madspincard, 'w') as m:
                m.write('set ms_dir ./madspingrid\n')
                m.write('set BW_cut 15 # cut on how far the particle can be off-shell\n')
                m.write('set max_running_process 1\n')
                m.write('define p = g u c d s b u~ c~ d~ s~ b~\n')
                m.write('define j = p\n')
                m.write('define l+ = e+ mu+ ta+\n')
                m.write('define l- = e- mu- ta-\n')
                m.write('define vl = ve vm vt\n')
                m.write('define vl~ = ve~ vm~ vt~\n')
                m.write('decay z > l+ l-\n')
                m.write('decay {0} > j j\n'.format(proc[1]))
                m.write('launch')
            # customize card
            with open(customize, 'w') as c:
                c.write('set param_card frblock {0} 0.0'.format(param[0]))
            # reweight card
            with open(reweightcard, 'w') as r:
                r.write('change helicity False\n')
                r.write('change rwgt_dir rwgt\n')
                r.write('# {0} {1}\n'.format(param[0], param[1]))
                for coeff in coefficients:
                    r.write('launch\n    set frblock {0} {1}\n'.format(param[0], coeff))
