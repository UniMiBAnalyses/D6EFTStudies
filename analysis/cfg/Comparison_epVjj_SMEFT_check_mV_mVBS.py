import ROOT




cut = "ptj1>40 && ptj2>40 && abs( etaj1 - etaj2 ) < 2.5 && abs( etaj1 - etaj2 ) > -2.5"


samples = {

   "SM": {

            "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6_results/ntuple_epVjj_SMlimit_sm_no_b_mass_oldmasses_oldSMInputs_oldDecays_6.root", "nameHR": "epVjj SM SMEFTsim #Gamma_{t} = 1.5", "color": 2, "weight": "({})*".format(cut) + "w"},
            

   }

}


variables = {
   'events': {'min': 0, 'max': 2, 'bins': 1},
   'mV' : {'min': 60, 'max': 120, 'bins': 50},
   'mVBS' : {'min': 100, 'max': 4000, 'bins': 50},  
   'mjj' : {'min': 300, 'max': 2000, 'bins': 50}
}

output = "plots"

luminosity = 60

showIntegral = True
