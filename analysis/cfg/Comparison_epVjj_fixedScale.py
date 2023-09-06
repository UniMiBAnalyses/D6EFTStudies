import ROOT


# mjj and mVBS should be the same thing now 

drVBS = " sqrt( pow(eta_vbs_1 - eta_vbs_2 ,2) + pow(phi_vbs_1 - phi_vbs_2,2)  )"

# cut = "mVBS > 300 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10 && {} > 0.4 && mindrjl > 0.4 && mindrjj > 0.4 && minmjj > 40".format(drVBS)
cut = "nbQuarks==0 && mVBS_new > 300 && ptj1 > 30 && ptj2 > 30 && ptj3 > 50 && ptj4 > 50 && ptl1 > 30 && met > 30 && etal1 < 3.0 && etal1 > -3.0 && minmjj > 40 && mindrjl > 0.4  && mindrjj > 0.4"
cut1b = "nbQuarks==1 && mVBS_new > 300 && ptj1 > 30 && ptj2 > 30 && ptj3 > 50 && ptj4 > 50 && ptl1 > 30 && met > 30 && etal1 < 3.0 && etal1 > -3.0 && minmjj > 40 && mindrjl > 0.4  && mindrjj > 0.4"
cut2b = "nbQuarks==2 && mVBS_new > 300 && ptj1 > 30 && ptj2 > 30 && ptj3 > 50 && ptj4 > 50 && ptl1 > 30 && met > 30 && etal1 < 3.0 && etal1 > -3.0 && minmjj > 40 && mindrjl > 0.4  && mindrjj > 0.4"

samples = {

   "0bjets": {       
            # Benchmark: From EFT gridpack p p > e ve j j j j,  Gamma top = 1.33, PDF = 325300, DynScale = 3
            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_fixedScale_results/ntuple_epVewk_SM_fixedScale.root", "nameHR": "epVjj 2#rightarrow 6", "color": 2, "weight": "({})*".format(cut) + "w"},
    
            # Davide gridpack samples. Full lepton flavours, 0.327 is branching fraction (precise pdg) for Gamma(W -> e ve )/Gamma_TotLep
            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWMhadJJ_fixedScale_results/ntuple_WPlepWMhadJJ_fixedScale.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWPhadJJ_fixedScale_results/ntuple_WPlepWPhadJJ_fixedScale.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepZhadJJ_fixedScale_results/ntuple_WPlepZhadJJ_fixedScale.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
   
   },

   "1bjets": {
            # Benchmark: From EFT gridpack p p > e ve j j j j,  Gamma top = 1.33, PDF = 325300, DynScale = 3
            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_fixedScale_results/ntuple_epVewk_SM_fixedScale.root", "nameHR": "epVjj 2#rightarrow 6", "color": 2, "weight": "({})*".format(cut1b) + "w"},

            # Davide gridpack samples. Full lepton flavours, 0.327 is branching fraction (precise pdg) for Gamma(W -> e ve )/Gamma_TotLep
            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWMhadJJ_fixedScale_results/ntuple_WPlepWMhadJJ_fixedScale.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut1b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWPhadJJ_fixedScale_results/ntuple_WPlepWPhadJJ_fixedScale.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut1b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepZhadJJ_fixedScale_results/ntuple_WPlepZhadJJ_fixedScale.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut1b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},

   },

   "2bjets": {
            # Benchmark: From EFT gridpack p p > e ve j j j j,  Gamma top = 1.33, PDF = 325300, DynScale = 3
            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_fixedScale_results/ntuple_epVewk_SM_fixedScale.root", "nameHR": "epVjj 2#rightarrow 6", "color": 2, "weight": "({})*".format(cut2b) + "w"},

            # Davide gridpack samples. Full lepton flavours, 0.327 is branching fraction (precise pdg) for Gamma(W -> e ve )/Gamma_TotLep
            "Davide_WpWm_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWMhadJJ_fixedScale_results/ntuple_WPlepWMhadJJ_fixedScale.root", "nameHR": "wpwm gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut2b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpWp_grid": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWPhadJJ_fixedScale_results/ntuple_WPlepWPhadJJ_fixedScale.root", "nameHR": "wpwp gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut2b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},
            "Davide_WpZ_grid" : {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepZhadJJ_fixedScale_results/ntuple_WPlepZhadJJ_fixedScale.root", "nameHR": "wpz gridpack", "color": ROOT.kMagenta+1, "weight": "0.327*({})*".format(cut2b) + "w", "stack":"SMP-20-013 2#rightarrow 4 fixedScale"},

   },

}


variables = {
   'events': {'min': 0, 'max': 2, 'bins': 1},
   'mVBS_new': {'min': 0, 'max': 3000, 'bins': 40},
   'mV_new': {'min': 60, 'max': 110, 'bins': 30},
   'met' : {'min': 0, 'max': 300, 'bins': 30},
   'eta_vbs_1 - eta_vbs_2' : {'min': -10, 'max': 10, 'bins': 40},
   'ptj1': {'min': 40, 'max': 100, 'bins': 20},
   'ptj2': {'min': 40, 'max': 300, 'bins': 20},
   'ptj3': {'min': 0, 'max': 400, 'bins': 20},
   'ptj4': {'min': 0, 'max': 800, 'bins': 20},
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
   'ptl1': {'min': 0, 'max': 300, 'bins': 20},
   'minmjj': {'min': 0, 'max': 100, 'bins': 20},
   'mindrjj': {'min': 0, 'max': 2, 'bins': 20},
   'mindrjl': {'min': 0, 'max': 2, 'bins': 20},
}

output = "plots_fixedScale"

legposx = 0.4
luminosity = 60

showIntegral = True
