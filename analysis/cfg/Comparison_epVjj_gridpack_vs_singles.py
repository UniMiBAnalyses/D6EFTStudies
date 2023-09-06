import ROOT


# mjj and mVBS should be the same thing now 

drVBS = " sqrt( pow(eta_vbs_1 - eta_vbs_2 ,2) + pow(phi_vbs_1 - phi_vbs_2,2)  )"

# cut = "mVBS > 300 && etal1 < 3.0 && etal1 > -3.0 && ptl1 > 10 && {} > 0.4 && mindrjl > 0.4 && mindrjj > 0.4 && minmjj > 40".format(drVBS)
cut = "mVBS_new > 500 && ptj1 > 30 && ptj2 > 30 && ptj3 > 50 && ptj4 > 50 && ptl1 > 30 && met > 30 && etal1 < 3.0 && etal1 > -3.0 && minmjj > 40 && mindrjl > 0.4  && mindrjj > 0.4"

samples = {

   "SM": {
          
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "SM rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "SM"},
   },

   "cW_LI": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cW LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cW - cW_m1)"},
   
        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cW_LI.root",  "nameHR": "cW LI AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"}, 
   },

   "cW_QU": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cW QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cW + cW_m1 - 2*SM)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cW_QU.root",  "nameHR": "cW QU AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"}, 
   },

   "cHW_LI": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHW LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHW - cHW_m1)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHW_LI.root",  "nameHR": "cHW LI AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },


   "cHW_QU": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHW QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHW + cHW_m1 -2*SM)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHW_QU.root",  "nameHR": "cHW QU AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },


   "cHB_QU": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHB QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHB + cHB_m1 -2*SM)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHB_QU.root",  "nameHR": "cHB QU AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },

   "cHB_LI": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHB LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHB - cHB_m1)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHB_LI.root",  "nameHR": "cHB LI AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },


   "cHl1_QU": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHl1 QU rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHl1 + cHl1_m1 -2*SM)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHl1_QU.root",  "nameHR": "cHl1 QU AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },

   "cHl1_LI": {
        "Main": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/ntuple-utils/lhe/EFT_LHE/cW_cHWB_cHbox_cHW_cHl1_cHB_cHQ1_cHj1_starting/ntuple_epVjj_ewk_dim6.root", "nameHR": "cHl1 LI rew", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "0.5*(cHl1 - cHl1_m1)"},

        "single": {"path": "/afs/cern.ch/work/g/gboldrin/public/public/CMSSW_10_6_4/src/D6EFTStudies/generation/EFT/WV/SingleComponents/ntuples/ntuple_epVewk_cHl1_LI.root",  "nameHR": "cHl1 LI AD", "color": ROOT.kAzure+2, "weight": "({})*".format(cut) + "w"},
   },
}


variables = {
   'events': {'min': 0, 'max': 2, 'bins': 1},
   'mVBS_new': {'min': 0, 'max': 3000, 'bins': 15},
   'mV_new': {'min': 60, 'max': 110, 'bins': 12},
   'met' : {'min': 0, 'max': 300, 'bins': 12},
   'ptj1': {'min': 40, 'max': 100, 'bins': 10},
   'ptj2': {'min': 40, 'max': 300, 'bins': 10},
   'ptj3': {'min': 0, 'max': 400, 'bins': 10},
   'ptj4': {'min': 0, 'max': 800, 'bins': 10},
   'etaj1': {'min': -5, 'max': 5, 'bins': 10},
   'etaj2': {'min': -5, 'max': 5, 'bins': 10},
   'etaj3': {'min': -5, 'max': 5, 'bins': 10},
   'etaj4': {'min': -5, 'max': 5, 'bins': 10},
   'Ej1': {'min': 50, 'max': 700, 'bins': 10},
   'Ej2': {'min': 100, 'max': 800, 'bins': 10},
   'Ej3': {'min': 100, 'max': 2000, 'bins': 10},
   'Ej4': {'min': 100, 'max': 3000, 'bins': 10},
   'mjj': {'min': 300, 'max': 1000, 'bins': 10},
   'pxl1': {'min': -200, 'max': 200, 'bins': 10},
   'pyl1': {'min': -200, 'max': 200, 'bins': 10},
   'pzl1': {'min': -150, 'max': 150, 'bins': 10},
   'ptl1': {'min': 0, 'max': 300, 'bins': 10},
   'minmjj': {'min': 0, 'max': 100, 'bins': 10},
   'mindrjj': {'min': 0, 'max': 2, 'bins': 10},
   'mindrjl': {'min': 0, 'max': 2, 'bins': 10},
}

output = "plots_gridpack_vs_singles_EFT"

legposx = 0.4
luminosity = 60

showIntegral = True
