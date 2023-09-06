import ROOT


cut = "mjj > 500 && ptj1>40 && ptj2>40 && ptj3>40 && ptj4>40"
samples = {


   "SM": {
      "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_results/ntuple_epVewk_SM.root", "nameHR": "SM AD", "color": 111, "weight": "3*({})*".format(cut) + "w"}, 

      "Davide_WpWm": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/Davide_SMP_20_013/WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWMhadJJ_EWK_LO_SM_mjj100_pTj10.root", "nameHR": "WpWm Davide", "color": 2, "weight": "({})*".format(cut) + "w", "stack": "SMP-20-013"},
      
      "Davide_WpWp": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/Davide_SMP_20_013/WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepWPhadJJ_EWK_LO_SM_mjj100_pTj10.root" , "nameHR": "WpWp Davide", "color": 3, "weight": "({})*".format(cut) + "w", "stack": "SMP-20-013"},

      "Davide_WpZ": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/Davide_SMP_20_013/WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10_results/ntuple_WPlepZhadJJ_EWK_LO_SM_mjj100_pTj10.root" , "nameHR": "WpZ Davide", "color": 4, "weight": "({})*".format(cut) + "w", "stack": "SMP-20-013"},

      "cW_LI": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "SM rew", "color": ROOT.kAzure+2, "weight": "3*({})*".format(cut) + "SM"},
      "ADPDF30600": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/epVewk_SM_306000_results/ntuple_epVewk_SM_306000.root", "nameHR": "SM AD PDF=30600", "color": ROOT.kGreen+2, "weight": "3*({})*".format(cut) + "w"}

   },

#    "cW_QU": {
#       "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cW_QU.root", "nameHR": "cW QU", "color": 111, "weight": "({})*".format(cut) + "w"},
#       "cW_QU": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cW QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cW + cW_m1 -2*SM)"},
#    },


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
