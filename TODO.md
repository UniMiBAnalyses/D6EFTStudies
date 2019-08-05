# status of the work and TODO list

## events generation

  * Lambda set at 1 TeV in all generations

### VBS SS ntuples 

_existing samples so far_

  * same-sign W+W+ with one electron and one muon

 | cW  |  cHW  |  EFT level  |  MG folder    |  events  |  ntuple |
 |-----|-------|-------------|---------------|----------|---------|
 |  0  |   0   |     SM      | SSeu_SMlimit  |    1M    |   [link](http://govoni.web.cern.ch/govoni/EFT/ntuple_SMlimit_HS.root) |
 | 0.3 |   0   |   interf    | SSeu_RcW_int  |    1M    |   [link](http://govoni.web.cern.ch/govoni/EFT/ntuple_RcW_0p3_HS.root) |
 | 0.3 |   0   | quadratic   | SSeu_RcW_bsm  |    1M    |   [link](http://govoni.web.cern.ch/govoni/EFT/ntuple_RcW_0p3_HS.root) |
 |  0  |  0.3  |   interf    | SSeu_RcHW_int |    1M    |   [link](http://govoni.web.cern.ch/govoni/EFT/ntuple_RcHW_0p3.root) |
 |  0  |  0.3  | quadratic   | SSeu_RcHW_bsm |    1M    |   [link](http://govoni.web.cern.ch/govoni/EFT/ntuple_RcHW_0p3.root) |
# |  0  |   0   |     SM      | SSeu_SMlimit  |    1M    |   [link]() |
 

_samples with two coefficients at a time_

  * generate the events of interference and pure bsm with both the coefficients cW and cHW at 0.3
    * the interference contains terms with power 1/lambda^2 (1 coefficient and SM)
    * the interference should contain also terms with 1/lambda^4 (2 different coefficients)
      * how can I isolate this term?
      * how does it compare to the terms with two EFT operators in the same diagram, 
        which are usually neglected?
    * the pure bsm contains terms with power 1/lambda^4
  * compare to the shapes obtained starting from standard model interference and pure bsm generated
    with the two coefficients separately
    * can one determine whether terms 1/lambda^4 are relevant?
    * can one determine whether terms 1/lambda^4 present in the interference are relevant?
    * if they are, how can one generate only those?


### VBS OS ntuples (with VBF Higgs focus as well)

_attempts to produce VBF in production times decay, lessons learned_

  * in any production x decay process structure, the EFT operators cannot enter the decay part
    * this is a problem in case, for example, of cHW and H > WW, the decay of the Higgs is in the decay
    * this is a problem in case of cH3l, which enters in the W > lv vertex, when the W is decayed
  * in any production x decay process structure, the NP^2==n directive cannot be inserted
    * either generate the full opposite-sign VBS, or leave vector bosons undecayed and use pythia later on 
      (depending on the type of operators one wants to vary)
      * need to prepare a pythia decay file in the production process

_generation attempts_

  * VBS generation with W decays
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-SMlimit_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > q q W+ W- QCD=0 SMHLOOP=0, W+ > l+ vl, W- > l- vl~
    output OSWW_SMlimit
    quit
    ```
  * VBS generation with stable W's, quadratic EFT term only for the cW coefficient, 
    does not work because of the W decays
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cW_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > q q W+ W- QCD=0 SMHLOOP=0 NP=1 NP^2==1, W+ > l+ vl, W- > l- vl~ 
    output OSWW_RcW_int
    quit
    ```
  * VBS generation with stable fermions in the final state:
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cW_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > e+ ve mu- vm~ p p QCD=0 NP=1
    generate p p > e- ve~ mu+ vm p p QCD=0 NP=1
    output OSeu_RcW_test
    quit
    ```
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cHW_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > e+ ve mu- vm~ p p QCD=0 NP=1
    generate p p > e- ve~ mu+ vm p p QCD=0 NP=1
    output OSeu_RcHW_test
    quit
    ```
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cHl3_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > e+ ve mu- vm~ p p QCD=0 NP=1
    generate p p > e- ve~ mu+ vm p p QCD=0 NP=1
    output OSeu_RcHl3_test
    quit
    ```
  * folders ready, TODO:
    * check the diagrams: 
      * where are the EFT operators in each of the three cases?
      * is NP=1 respected?
    * if diagrams OK, prepare interference and quadratic separated
    * and then submit event generation
