import ROOT

samples = {
   "SM": {
      "Main": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "SM AD", "color": 111, "weight": "w"},
      "300k": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "SM Rew (cW)", "color": ROOT.kAzure+2, "weight": "rwgt_1"},
      "cwctj1cjj11": {"path": "../generation/ntuple_OSWW_cW_ctj1_cjj11.root", "nameHR": "SM Rew (cW-ctj1-cjj11)", "color": ROOT.kSpring+2, "weight": "rwgt_1"},
   },
   "cHj3_LI": {
      "Main": {"path": "../generation/ntuple_OSWW_cHj3_LI.root", "nameHR": "cHj3 LI AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "cHj3 LI Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_6 - rwgt_7)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "cHj3 LI Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_6 - rwgt_7)"}
   }, 
   "cHj3_QU": {
      "Main": {"path": "../generation/ntuple_OSWW_cHj3_QU.root", "nameHR": "cHj3 LI AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "cHj3 QU Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_6 + rwgt_7 - 2*rwgt_1)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "cHj3 QU Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_6 + rwgt_7 - 2*rwgt_1)"}
   },
   "ctWRe_LI": {
      "Main": {"path": "../generation/ntuple_OSWW_ctWRe_LI.root", "nameHR": "ctWRe LI AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "ctWRe LI Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_2 - rwgt_3)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "ctWRe LI Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_2 - rwgt_3)"}
   },
   "ctWRe_QU": {
      "Main": {"path": "../generation/ntuple_OSWW_ctWRe_QU.root", "nameHR": "ctWRe QU AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "ctWRe QU Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_2 + rwgt_3 - 2*rwgt_1)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "ctWRe QU Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_2 + rwgt_3 - 2*rwgt_1)"}
   },
   "ctj1_LI": {
      "Main": {"path": "../generation/ntuple_OSWW_ctj1_LI.root", "nameHR": "ctj1 LI AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "ctj1 LI Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_10 - rwgt_11)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "ctj1 LI Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_10 - rwgt_11)"}
   },
   "ctj1_QU": {
      "Main": {"path": "../generation/ntuple_OSWW_ctj1_QU.root", "nameHR": "ctj1 QU AD", "color": 111, "weight": "w"},
      "cW": {"path": "../generation/ntuple_OSWW_cW.root", "nameHR": "ctj1 QU Rew (cW)", "color": ROOT.kAzure+2, "weight": "0.5*(rwgt_10 + rwgt_11 - 2*rwgt_1)"},
      "sm": {"path": "../generation/ntuple_OSWW_SM.root", "nameHR": "ctj1 QU Rew (SM)", "color": ROOT.kRed+2, "weight": "0.5*(rwgt_10 + rwgt_11 - 2*rwgt_1)"}
   },
}

variables = {
   "mjj": {"min": 40, "max": 1000, 'bins': 50},
   "etaj1": {"min": -3, "max": 3, 'bins': 50},
   'deltaetajj': {"min": 0, "max": 8, 'bins': 50, 'legpos': "upperright"},
   'deltaphijj': {"min": 0, "max": 3.14, 'bins': 50, 'legpos': "upperleft"},
   'etaj2': {"min": -3, "max": 3, 'bins': 50},
   'etal1': {"min": -3, "max": 3, 'bins': 50},
   'met': {"min": 0, "max": 1000, 'bins': 50},
   'mll': {'min': 0, 'max': 500, 'bins': 50},
   'ptl1': {'min': 0, 'max': 200, 'bins': 50},
   'ptl2': {'min': 0, 'max': 200, 'bins': 50},
   'ptll': {'min': 0, 'max': 200, 'bins': 50}
}

output = "plots"

luminosity = 58

showIntegral = True
