import ROOT


# mjj and mVBS should be the same thing now 

drVBS = " sqrt( pow(eta_vbs_1 - eta_vbs_2 ,2) + pow(phi_vbs_1 - phi_vbs_2,2)  )"

cut_grid = "mVBS > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10 && {} > 0.4 && mindrjl > 0.4 && mindrjj > 0.4 && minmjj > 40".format(drVBS)

cut = "mVBS > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5"

samples = {

   "SM": {

            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "epVjj SM SMEFTsim #Gamma_{t} = 1.5", "color": 2, "weight": "({})*".format(cut_grid) + "w"},
            

            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut_grid) + "w", "stack":"SMP-20-013 gridpack"},

            "epVjj_sm_no_b_mass": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_sm_no_b_mass_results/ntuple_epVjj_sm_no_b_mass.root", "nameHR": "epVjj SM sm_no_b_mass", "color": ROOT.kGreen+2, "weight": "({})*".format(cut_grid) + "w"},
   

            "epVjj_SMEFTsim_306000": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_306000_results/ntuple_epVewk_SM_306000.root", "nameHR": "epVjj SM 306000", "color": ROOT.kAzure +1, "weight": "({})*".format(cut_grid) + "w"},

            "epVjj_sm_ckm_306000": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_sm_no_b_mass_306000_results/ntuple_epVjj_sm_no_b_mass_306000.root", "nameHR": "epVjj sm_no_b_mass  306000", "color": ROOT.kBlack, "weight": "({})*".format(cut_grid) + "w"},
   
            "epVjj_sm_ckm_306000_dynscalem1": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_sm_no_b_mass_306000_dynScalem1_results/ntuple_epVjj_sm_no_b_mass_306000_dynScalem1.root", "nameHR": "epVjj SM 306000 dynscale=-1", "color": ROOT.kOrange +1, "weight": "({})*".format(cut_grid) + "w"},

   }

}


variables = {
   'ptj1': {'min': 40, 'max': 300, 'bins': 50},
   'ptj2': {'min': 40, 'max': 600, 'bins': 50},
   'ptj3': {'min': 0, 'max': 800, 'bins': 50},
   'ptj4': {'min': 0, 'max': 1000, 'bins': 50},
   'etaj1': {'min': -5, 'max': 5, 'bins': 50},
   'etaj2': {'min': -5, 'max': 5, 'bins': 50},
   'etaj3': {'min': -5, 'max': 5, 'bins': 50},
   'etaj4': {'min': -5, 'max': 5, 'bins': 50}, 
   'Ej1': {'min': 50, 'max': 1500, 'bins': 50},
   'Ej2': {'min': 100, 'max': 1500, 'bins': 50},
   'Ej3': {'min': 100, 'max': 1500, 'bins': 50},
   'Ej4': {'min': 100, 'max': 2000, 'bins': 50},
   'mjj': {'min': 300, 'max': 1000, 'bins': 50},
   'pxl1': {'min': -200, 'max': 200, 'bins': 50},
   'pyl1': {'min': -200, 'max': 200, 'bins': 50},
   'pzl1': {'min': -150, 'max': 150, 'bins': 50},
   'met': {'min': 0, 'max': 500, 'bins': 50},
   'mV' : {'min': 60, 'max': 120, 'bins': 50},
   'mVBS' : {'min': 0, 'max': 4000, 'bins': 50}, 
   'events': {'min': 0, 'max': 2, 'bins': 1},  
   'minmjj': {'min': 0, 'max': 100, 'bins': 50},
   'mindrjj': {'min': 0, 'max': 2, 'bins': 50},
   'mindrjl': {'min': 0, 'max': 2, 'bins': 50},
}

output = "plots"

luminosity = 60

showIntegral = True
