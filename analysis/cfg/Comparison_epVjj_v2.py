import ROOT


cut = "ptj1>100 && ptj2>100"
samples = {

   "cHB_LI": {
      
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHB LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHB - cHB_m1)"},
   },

   "cHB_QU": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHB QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHB + cHB_m1 -2*SM)"},
   },

   "cW_LI": {

      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cW LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cW - cW_m1)"},
   },

   "cW_QU": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cW QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cW + cW_m1 -2*SM)"},
   },


   "cHW_LI": {

      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHW LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHW - cHW_m1)"},
   },

   "cHW_QU": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHW QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHW + cHW_m1 -2*SM)"},
   },


   "cHWB_LI": {

      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHWB LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHWB - cHWB_m1)"},
   },

   "cHWB_QU": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHWB QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHWB + cHWB_m1 -2*SM)"},
   },


}

variables = {
   'ptj1': {'min': 0, 'max': 200, 'bins': 50},
   'ptj2': {'min': 0, 'max': 200, 'bins': 50},
   'ptj3': {'min': 0, 'max': 200, 'bins': 50},
   'ptj4': {'min': 0, 'max': 200, 'bins': 50},
   'etaj1': {'min': -5, 'max': 5, 'bins': 50},
   'etaj2': {'min': -5, 'max': 5, 'bins': 50},
   'etaj3': {'min': -5, 'max': 5, 'bins': 50},
   'etaj4': {'min': -5, 'max': 5, 'bins': 50}, 
   'Ej1': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej2': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej3': {'min': 0, 'max': 1000, 'bins': 50},
   'Ej4': {'min': 0, 'max': 1000, 'bins': 50},
   
}

output = "plots"

luminosity = 58

showIntegral = True