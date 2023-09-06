import ROOT


cut = "mjj > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5"
# cut_grid = "mjj > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5 && etal1 < 3.0 && etal1 > -3.0 && sqrt((etaj1 - etaj2)*(etaj1 - etaj2) - (phij1 - phij2)*(phij1-phij2)) > 0.4 && sqrt((etaj1 - etaj3)*(etaj1 - etaj3) - (phij1 - phij3)*(phij1-phij3)) > 0.4 && sqrt((etaj1 - etaj3)*(etaj1 - etaj3) - (phij1 - phij3)*(phij1-phij3)) > 0.4 && sqrt((etaj1 - etaj4)*(etaj1 - etaj4) - (phij1 - phij4)*(phij1-phij4)) > 0.4 && sqrt((etaj2 - etaj3)*(etaj2 - etaj3) - (phij2 - phij3)*(phij2-phij3)) > 0.4 && sqrt((etaj2 - etaj4)*(etaj2 - etaj4) - (phij2 - phij4)*(phij2-phij4)) > 0.4 && sqrt((etaj3 - etaj4)*(etaj3 - etaj4) - (phij3 - phij4)*(phij3-phij4)) > 0.4 && sqrt((etal1 - etaj1)*(etal1 - etaj1) - (phil1 - phij1)*(phil1-phij1)) > 0.4 && sqrt((etal1 - etaj2)*(etal1 - etaj2) - (phil1 - phij2)*(phil1-phij2)) > 0.4 && sqrt((etal1 - etaj3)*(etal1 - etaj3) - (phil1 - phij3)*(phil1-phij3)) > 0.4 && sqrt((etal1 - etaj4)*(etal1 - etaj4) - (phil1 - phij4)*(phil1-phij4)) > 0.4 && ptl1 > 10"

cut_grid = "mjj > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10"

samples = {

   "SM": {

            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "epVjj SM SMEFTsim #Gamma_{t} = 1.5", "color": 2, "weight": "({})*".format(cut) + "w"},
            
            "epVjj_SMEFTsim": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_results/ntuple_epVewk_SM.root", "nameHR": "epVjj SM SMEFTsim #Gamma_{t} = 1.3", "color": ROOT.kGreen+2, "weight": "({})*".format(cut) + "w"},
            "Davide_WpWm_SMEFT": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "wpwm SMEFTsim-SMlimit", "color": 2, "weight": "({})*".format(cut) + "w", "stack":"SMP-20-013"},
            "Davide_WpWp_SMEFT": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root ", "nameHR": "wpwp SMEFTsim-SMlimit", "color": 2, "weight": "({})*".format(cut) + "w", "stack":"SMP-20-013"},
            "Davide_WpZ_SMEFT": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "wpz  SMEFTsim-SMlimit", "color": 2, "weight": "({})*".format(cut) + "w", "stack":"SMP-20-013"},

            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.3333*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.3333*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.3333*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},


   }

}


variables = {
   'ptj1': {'min': 30, 'max': 300, 'bins': 50},
   'ptj2': {'min': 80, 'max': 600, 'bins': 50},
   'ptj3': {'min': 100, 'max': 800, 'bins': 50},
   'ptj4': {'min': 100, 'max': 1000, 'bins': 50},
   'etaj1': {'min': -5, 'max': 5, 'bins': 50},
   'etaj2': {'min': -5, 'max': 5, 'bins': 50},
   'etaj3': {'min': -5, 'max': 5, 'bins': 50},
   'etaj4': {'min': -5, 'max': 5, 'bins': 50}, 
   'Ej1': {'min': 50, 'max': 1500, 'bins': 50},
   'Ej2': {'min': 100, 'max': 1500, 'bins': 50},
   'Ej3': {'min': 100, 'max': 1500, 'bins': 50},
   'Ej4': {'min': 100, 'max': 2000, 'bins': 50},
   'mjj': {'min': 300, 'max': 1000, 'bins': 50},
   'pxl1': {'min': 0, 'max': 300, 'bins': 50},
   'pyl1': {'min': 0, 'max': 300, 'bins': 50},
   'pzl1': {'min': 0, 'max': 500, 'bins': 50},
   'met': {'min': 0, 'max': 500, 'bins': 50}, 
   'events': {'min': 0, 'max': 2, 'bins': 1},  
}

output = "plots"

luminosity = 60

showIntegral = True
