import ROOT


# mjj and mVBS should be the same thing now 

drVBS = " sqrt( pow(eta_vbs_1 - eta_vbs_2 ,2) + pow(phi_vbs_1 - phi_vbs_2,2)  )"

cut = "mVBS > 300 && ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10 && {} > 0.4 && mindrjl > 0.4 && mindrjj > 0.4 && minmjj > 40".format(drVBS)

samples = {

   "SM": {

            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_results/ntuple_epVewk_SM.root", "nameHR": "epVjj SMEFTsim #Gamma_{t} = 1.33", "color": 2, "weight": "({})*".format(cut) + "w"},
            "epVjj_cm_no_b": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_sm_no_b_mass_results/ntuple_epVjj_sm_no_b_mass.root", "nameHR": "epVjj sm_no_b_mass #Gamma_{t} = 1.49", "color": ROOT.kRed, "weight": "({})*".format(cut) + "w"},

            "epVjj_SMEFTsim_gamma": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "epVjj SMEFTsim #Gamma_{t} = 1.49", "color": ROOT.kAzure+1, "weight": "({})*".format(cut) + "w"},         

   }

}


variables = {
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
   'met': {'min': 0, 'max': 300, 'bins': 20},
   'mV' : {'min': 60, 'max': 120, 'bins': 20},
   'mVBS' : {'min': 0, 'max': 3000, 'bins': 20},
   'events': {'min': 0, 'max': 2, 'bins': 1},
   'minmjj': {'min': 0, 'max': 100, 'bins': 20},
   'mindrjj': {'min': 0, 'max': 2, 'bins': 20},
   'mindrjl': {'min': 0, 'max': 2, 'bins': 20},
}

output = "plots_beckmarking"

luminosity = 60

showIntegral = True
