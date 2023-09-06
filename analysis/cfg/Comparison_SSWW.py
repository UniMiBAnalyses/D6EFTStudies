import ROOT


cut = "1==1"
samples = {


   "SM": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/SSWW_SM_results/ntuple_SSWW_SM.root", "nameHR": "SM SSWW", "color": 111, "weight": "({})*".format(cut) + "w"}, 

   },
}

variables = {
   'mW1': {'min': 50, 'max': 100, 'bins': 50},
   'mW2': {'min': 50, 'max': 100, 'bins': 50},   
}

output = "plots_SSWW"

luminosity = 60

showIntegral = True
