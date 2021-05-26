#!/usr/bin/env python

import os
from itertools import combinations

if __name__ == "__main__":

    switchOn = ['2', '4', '5', '7', '9', '21', '22', '24', '25', '29', '30', '31', '32', '33', '34' ]

    params = [  [ '1' , 'cG' ],
                [ '2' , 'cW' ],
                [ '3' , 'cH' ],
                [ '4' , 'cHbox' ],
                [ '5' , 'cHDD' ],
                [ '6' , 'cHG' ],
                [ '7' , 'cHW' ],
                [ '8' , 'cHB' ],
                [ '9' , 'cHWB' ],
                [ '10' , 'ceHRe' ],
                [ '11' , 'cuHRe' ],
                [ '12' , 'cdHRe' ],
                [ '13' , 'ceWRe' ],
                [ '14' , 'ceBRe' ],
                [ '15' , 'cuGRe' ],
                [ '16' , 'cuWRe' ],
                [ '17' , 'cuBRe' ],
                [ '18' , 'cdGRe' ],
                [ '19' , 'cdWRe' ],
                [ '20' , 'cdBRe' ],
                [ '21' , 'cHl1' ],
                [ '22' , 'cHl3' ],
                [ '23' , 'cHe' ],
                [ '24' , 'cHq1' ],
                [ '25' , 'cHq3' ],
                [ '26' , 'cHu' ],
                [ '27' , 'cHd' ],
                [ '28' , 'cHudRe' ],
                [ '29' , 'cll' ],
                [ '30' , 'cll1' ],
                [ '31' , 'cqq1' ],
                [ '32' , 'cqq11' ],
                [ '33' , 'cqq3' ],
                [ '34' , 'cqq31' ],
                [ '35' , 'clq1' ],
                [ '36' , 'clq3' ],
                [ '37' , 'cee' ],
                [ '38' , 'cuu' ],
                [ '39' , 'cuu1' ],
                [ '40' , 'cdd' ],
                [ '41' , 'cdd1' ],
                [ '42' , 'ceu' ],
                [ '43' , 'ced' ],
                [ '44' , 'cud1' ],
                [ '45' , 'cud8' ],
                [ '46' , 'cle' ],
                [ '47' , 'clu' ],
                [ '48' , 'cld' ],
                [ '49' , 'cqe' ],
                [ '50' , 'cqu1' ],
                [ '51' , 'cqu8' ],
                [ '52' , 'cqd1' ],
                [ '53' , 'cqd8' ],
                [ '54' , 'cledqRe' ],
                [ '55' , 'cquqd1Re' ],
                [ '56' , 'cquqd11Re' ],
                [ '57' , 'cquqd8Re' ],
                [ '58' , 'cquqd81Re' ],
                [ '59' , 'clequ1Re' ],
                [ '60' , 'clequ3Re' ]
              ]

    proc_IDs = [('ZWm', 'w-'), ('ZWp', 'w+'), ('ZZ', 'z')]
    reweight_coefs = [(1.0,1.0), (0.0,0.0), (1.0,0.0), (0.0,1.0)]
    suffix = "_".join([i[1] for i in  params if i[0] in switchOn])

    ops_combo = list(combinations(switchOn, 2)) # no repetition pairs

    for proc in proc_IDs:
        prefix = '{0}_{1}'.format(proc[0], suffix)
        dirname = './' + prefix + '/'
        if os.path.isdir(dirname):
            os.system('rm -rf ' + dirname)
        os.mkdir(dirname)
        # cards names
        runcard = dirname + prefix + '_run_card.dat'
        extramodels = dirname + prefix + '_extramodels.dat'
        proccard = dirname + prefix + '_proc_card.dat'
        madspincard = dirname + prefix + '_madspin_card.dat'
        customize = dirname + prefix + '_customizecards.dat'
        reweightcard = dirname + prefix + '_reweight_card.dat'

        # extramodels card
        with open(extramodels, 'w') as e:
            e.write('SMEFTsim_U35_MwScheme_UFO.tar.gz')

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
            p.write('import model SMEFTsim_U35_MwScheme_UFO-{0}_massless\n'.format(suffix))
            p.write('generate p p > z {0} j j QCD=0 NP=1 SMHLOOP=0\n'.format(proc[1]))
            p.write('output ' + prefix)

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
            for i in switchOn:
                c.write('set param_card SMEFT {0} 0.0\n'.format(i))

        # reweight card
        count = 0
        with open(reweightcard, 'w') as r:
            r.write('change helicity False\n')
            r.write('change rwgt_dir rwgt\n')
            for combo in ops_combo:
                r.write('# {0}\n'.format(combo[0]))
                r.write('# {0}\n'.format(combo[1]))
                for i in reweight_coefs:
                    r.write('launch\n')
                    r.write('    set SMEFT {0} {1}\n'.format(combo[0], i[0]))
                    r.write('    set SMEFT {0} {1}\n'.format(combo[1], i[1]))

                    for j in switchOn:
                        if j not in combo:
                            r.write('    set SMEFT {0} 0.0\n'.format(j))

                    count += 1
                    
                    r.write('\n')

        print("@INFO: Successful card creation for  {}, total number of reeweight: {}".format(proc[0], count))
