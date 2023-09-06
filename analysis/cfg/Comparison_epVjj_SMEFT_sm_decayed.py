import ROOT


cut = "mjj > 100 && ptj1>40 && ptj2>40"


samples = {

   "SM": {

            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_nockm_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_nockm.root", "nameHR": "lvjjjj sm-ckm_no_b_mass", "color": 2, "weight": "({})*".format(cut) + "w"},

            "Davide_WpWm_SMEFT": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "lnjjjj SMEFTsim-SMlimit", "color": 2, "weight": "({})*".format(cut) + "w"},

   }

}


variables = {
   'ptj1': {'min': 30, 'max': 300, 'bins': 50},
   'ptj2': {'min': 30, 'max': 300, 'bins': 50},
   'ptj3': {'min': 30, 'max': 600, 'bins': 50},
   'ptj4': {'min': 30, 'max': 600, 'bins': 50},
   'etaj1': {'min': -5, 'max': 5, 'bins': 50},
   'etaj2': {'min': -5, 'max': 5, 'bins': 50},
   'etaj3': {'min': -5, 'max': 5, 'bins': 50},
   'etaj4': {'min': -5, 'max': 5, 'bins': 50}, 
   'Ej1': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej2': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej3': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej4': {'min': 0, 'max': 1000, 'bins': 50},
   'mjj': {'min': 0, 'max': 1000, 'bins': 50},   
}

output = "plots"

luminosity = 60

showIntegral = True
