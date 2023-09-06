import ROOT


# mjj and mVBS should be the same thing now 

drVBS = " sqrt( pow(eta_vbs_1 - eta_vbs_2 ,2) + pow(phi_vbs_1 - phi_vbs_2,2)  )"

# cut = "mVBS > 300 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10 && {} > 0.4 && mindrjl > 0.4 && mindrjj > 0.4 && minmjj > 40".format(drVBS)
cut = "minmjj > 40 && mindrjl > 0.4"

samples = {

   "SM": {
          
            # Benchmark: From EFT gridpack p p > e ve j j j j,  Gamma top = 1.33, PDF = 325300, DynScale = 3
            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_results/ntuple_epVewk_SM.root", "nameHR": "epVjj SMEFTsim #Gamma_{t} = 1.33", "color": 2, "weight": "({})*".format(cut) + "w"},


            # Davide gridpack samples. Full lepton flavours, 0.327 is branching fraction (precise pdg) for Gamma(W -> e ve )/Gamma_TotLep
            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 gridpack"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/Davide_SMP_20_013/Nominal/WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 gridpack"},

            # From EFT gridpack p p > e ve j j j j, Gamma top = 1.49, PDF = 325300, DynScale = 3
            "epVjj_cm_no_b": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "epVjj SMEFTsim #Gamma_{t} = 1.49", "color": ROOT.kRed, "weight": "({})*".format(cut) + "w"},

            # From EFT gridpack p p > e ve j j j j, Gamma top = 1.33, PDF = 306000, DynScale = 3
            "epVjj_SMEFTsim_pdf": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_306000_results/ntuple_epVewk_SM_306000.root", "nameHR": "epVjj SMEFTsim PDF=306000", "color": ROOT.kOrange+1, "weight": "({})*".format(cut) + "w"},

            # From EFT gridpack p p > e ve j j j j, Gamma top = 1.33, PDF = 325300, DynScale = -1
            "epVjj_SMEFTsim_dyn": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_dynscalem1_results/ntuple_epVewk_SM_dynscalem1.root", "nameHR": "epVjj SMEFTsim DynScale=-1", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},      

            # From EFT gridpack p p > e ve j j j j, Gamma top = 1.33, PDF = 306000, DynScale = -1
            "epVjj_SMEFTsim_pdf_dyn": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_306000_dynscalem1_results/ntuple_epVewk_SM_306000_dynscalem1.root", "nameHR": "epVjj SMEFTsim DynScale=-1 PDF=306000", "color": ROOT.kGreen+2, "weight": "({})*".format(cut) + "w"},
   
            # From EFT gridpack p p > e ve j j j j, gamma top = 1.49, PDF = 306000, DynScale = -1
            "epVjj_SMEFTsim_gamma_pdf_dyn": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_306000_dynscalem1_gamma_results/ntuple_epVewk_SM_306000_dynscalem1_gamma.root" , "nameHR": "epVjj SMEFTsim #Gamma_{t}=1.5 DS=-1 PDF=306000", "color": ROOT.kBlack, "weight": "({})*".format(cut) + "w"},

   }

}


variables = {
   'ptj1': {'min': 40, 'max': 100, 'bins': 20},
   'ptj2': {'min': 40, 'max': 300, 'bins': 20},
   'ptj3': {'min': 0, 'max': 400, 'bins': 20},
   'ptj4': {'min': 0, 'max': 800, 'bins': 50},
   'etaj1': {'min': -5, 'max': 5, 'bins': 20},
   'etaj2': {'min': -5, 'max': 5, 'bins': 20},
   'etaj3': {'min': -5, 'max': 5, 'bins': 20},
   'etaj4': {'min': -5, 'max': 5, 'bins': 20},
   'Ej1': {'min': 50, 'max': 700, 'bins': 20},
   'Ej2': {'min': 100, 'max': 800, 'bins': 20},
   'Ej3': {'min': 100, 'max': 2000, 'bins': 20},
   'Ej4': {'min': 100, 'max': 3000, 'bins': 20},
   'mjj': {'min': 300, 'max': 1000, 'bins': 20},
   'pxl1': {'min': -200, 'max': 200, 'bins': 20},
   'pyl1': {'min': -200, 'max': 200, 'bins': 20},
   'pzl1': {'min': -150, 'max': 150, 'bins': 20},
   'met': {'min': 0, 'max': 300, 'bins': 20},
   'mV' : {'min': 60, 'max': 110, 'bins': 150},
   'mVBS' : {'min': 0, 'max': 500, 'bins': 50},
   'events': {'min': 0, 'max': 2, 'bins': 1},
   'minmjj': {'min': 0, 'max': 100, 'bins': 20},
   'mindrjj': {'min': 0, 'max': 2, 'bins': 20},
   'mindrjl': {'min': 0, 'max': 2, 'bins': 20},
   'nbQuarks': {'min': 0, 'max': 4, 'bins': 4},
   'eta_vbs_1 - eta_vbs_2': {'min': -10, 'max': 10, 'bins': 20},
}

output = "plots_beckmarking_looseLHECuts"

luminosity = 60

showIntegral = True
