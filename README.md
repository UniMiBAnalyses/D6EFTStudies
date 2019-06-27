# D6EFTStudies

EFT studies with Dim6 Warsaw basis

## folder structure and content

### madgraph_model

Modification to the existing SMEFTSim () Madgraph model used for the generation of events.
To produce a sample:
  * source a recent environment, for example outside of the D6EFTStudies project do:
    ```
    cmsrel CMSSW_10_6_0
    cd CMSSW_10_6_0/src
    cmsenv
    cd -
    ```
  * outside of the project, download a Madgraph release and the existing SMEFTSim model:
    ```
    wget https://cms-project-generators.web.cern.ch/cms-project-generators/MG5_aMC_v2.6.5.tar.gz
    cd MG5_aMC_v2_6_0/models
    wget http://feynrules.irmp.ucl.ac.be/raw-attachment/wiki/SMEFT/SMEFTsim_A_U35_MwScheme_UFO_v2.1.tar.gz
    tar xzf SMEFTsim_A_U35_MwScheme_UFO_v2.1.tar.gz
    ```
  * copy in the model folder the additional files present in the D6EFTStudies project:
    ```
    cp ../../../D6EFTStudies/madgraph_model/* .
    ```
  * get back to the Madgraph folder and prepare the process folder:
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cW_massless
    generate p p > e+ ve mu+ vm j j QCD=0 NP=1
    output SSeu_RcW_test
    quit
    ```
The comments issued in the Madgraph shell correspond to the model used, 
the process generated, the type of generation:
  * the model used for the generation is issued with the command `import`, 
    which takes as an argument the model name ad possible restrictions,
    as coded in restriction files. 
    In the example, the model is the one contained in the folder `SMEFTsim_A_U35_MwScheme_UFO_v2_1`,
    while the restriction is the one in the file `restrict_cW_massless.dat`.
    The restriction files present in the D6EFTStudies project 
    produce events with massless light leptons and quarks,
    diagonal CKM matrix and rigorously zero value for all the Wilson coefficients, 
    but the one present in the file name.
  * the process is generated according to the Madgraph syntax. In particular:
    * NP=1 means that no more than a single BSM operator is introduced in each diagram
    * NP^2==1, when present, means that only the interference is calculated
    * NP^2==2, when present, means that only the squared bsm part is calculated

### generation

Scripts to submit jobs to condor, for the generation of events in Madgraph.
To submit a job:
  * copy the folder present in the Madgraph directory in this folder:
   ```
   cp -r ../../MG5_aMC_v2_6_0/SSeu_RcW_test .
   ```
  * create a cfg file starting from the example `SSeu_RcW.sub`.
    Here, the `process_name` corresponds to he madgraph folder name, 
    and full paths have to be specified when needed.
  * the results folder has to exist before submitting any job
  * the line `queue 100` specifies how many identical jobs will be submitted
    (random seeds are changed though)
  * to actually submit jobs, run `condor_submit SSeu_RcW.sub`

### analysis

  * `read_02.cpp` reads LHE files and produces sets of histograms
  * `read_03.cpp` reads LHE files and produces sets of ntuples



## useful commands

  * unzip madgraph results within a folder
   ```
   for fil in  `find  . -name "*gz"` ; do gunzip $fil ; done
   ```
  * prepare list of lhe files for cfg files (rem unalias ls)
   ```
   find . -name "*lhe" | sed -e s%\.%`pwd`% | tr "\n" ","
   ```




