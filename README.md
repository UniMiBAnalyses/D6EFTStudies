# D6EFTStudies

EFT studies with Dim6 Warsaw basis

## madgraph_model

Modification to the existing SMEFTSim () Madgraph model used for the generation of events.
To produce a sample:
  * to prepare the environment
    on lxplus is to use [LCG environments](http://lcginfo.cern.ch/release_packages/x86_64-centos7-gcc62-opt/96/),
    for example (this alredy has a version of lhapdf as well):
    ```
    source /cvmfs/sft.cern.ch/lcg/views/LCG_96/x86_64-centos7-gcc8-opt/setup.sh 
    ``` 
  * change the location of the pdf data cards
    ``` 
    export LHAPDF_DATA_PATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/
    export LHAPDF_DATA_PATH=/cvmfs/sft.cern.ch/lcg/views/LCG_96/x86_64-centos7-gcc8-opt/share/LHAPDF/:$LHAPDF_DATA_PATH
    ``` 
  * download a Madgraph release and the existing SMEFTSim model:
    ```
    wget https://cms-project-generators.web.cern.ch/cms-project-generators/MG5_aMC_v2.6.5.tar.gz
    cd MG5_aMC_v2_6_5/models
    wget http://feynrules.irmp.ucl.ac.be/raw-attachment/wiki/SMEFT/SMEFTsim_A_U35_MwScheme_UFO_v2.1.tar.gz
    tar xzf SMEFTsim_A_U35_MwScheme_UFO_v2.1.tar.gz
    ```
  * copy in the model folder the additional files present in the D6EFTStudies project, 
    from the right folder depending on the version of the UFO:
    ```
    cp ../../../D6EFTStudies/madgraph_model/[folder]/* .
    ```
  * trivial examples of Madgraph syntax can be found [here](https://www.niu.edu/spmartin/madgraph/madsyntax.html)
  * get back to the Madgraph folder and prepare the process folder:
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cW_massless
    generate p p > e+ ve mu+ vm j j QCD=0 NP=1
    output SSeu_RcW_test
    quit
    ```
    Example of generation of VBF Higgs > WW > fully leptonic.
    The following syntax allows for having EFT entering both in the production
    and decay vertices of the Higgs boson, 
    while it remains not present in the W decays.
      * NB1 with the W decay, no restrictions on NP^2 are accepted by Madgraph apparently
      * NB2 SMHLOOP not zero turns on loops coupling with gluons and bosons,
            hence one should most probably turn it off with no expected big impact,
            as its impact is O(alpha^8)
    ```
    ./bin/mg5_aMC
    import model SMEFTsim_A_U35_MwScheme_UFO_v2_1-cHW_massless
    define q = u c d s u~ c~ d~ s~
    generate p p > q q W+ W- QCD=0 SMHLOOP=0 NP=1, W+ > l+ vl, W- > l- vl~ 
    output VBFHWW_RcHW_WWdecay_fs
    quit
    ```
    One operator of interest which enters in the W decay is Hl3, 
    therefore for productions involving that operator
    one needs to generate the full VBS process:
    ```
    working on it
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
  * the restriction `restrict_VBS_massless.dat` allows to modify cW, cHW, cHDD, cHbox, cHB, cHWB, cll, cHl3, 
    which are the ones marked as relevant for TGC/QGC and hVV in I. Brivio sildes at 19/01/2019 VBSCan WG1 meeting.
  * the process is generated according to the Madgraph syntax. In particular:
    * NP=1 means that no more than a single BSM operator is introduced in each diagram
    * NP^2==1, when present, means that only the interference is calculated
    * NP^2==2, when present, means that only the squared bsm part is calculated
  * the default run_card.dat produced by Madgraph may be changed by modifying the file
    ```Template/LO/Cards/run_card.dat``` in the Madgraph folder
  * besides the interactive operation, Madgraph may be used with instruction scripts, containing for example:
    ```
    import model SMEFTsim_A_U35_MwScheme_UFO_v3_1-cW_massless
    generate p p > e+ ve mu- vm~ NP=1 NP^2==1
    add process p p > e- ve~ mu+ vm NP=1 NP^2==1
    output WW_LI
    ```  
  * the script ```create1Dfolders.py``` generates the Madgpraph code for the event generation
    with one Wilson coefficient different from zero, for the linear and quadratic EFT terms,
    according to the list of operators switched on in the script.
  * the script ```create2Dfolders.py``` generates the Madgpraph code for the event generation
    with two Wilson coefficients interfering between each other,
    according to the list of operators switched on in the script.

## generation

Scripts to submit jobs to condor, for the generation of events in Madgraph.
To submit a job:
  * copy the folder present in the Madgraph directory in this folder:
   ```
   cp -r ../../MG5_aMC_v2_6_0/SSeu_RcW_test .
   ```
  * run the submission script
   ```
   ./submit.py SSeu_RcW_test
   ```
Config file examples can be found in the subfolders `generation/SSeu_RcW/Cards/`, 
`generation/SSeu_RcW_int/Cards/`, `generation/SSeu_RcW_bsm/Cards/`, 
where the `param_card.dat`, `run_card.dat` and `proc_card.dat` examples for same-sign WW
and `cW` non zero are reported. The three folders refer to the SM case, the interference between SM and BSM
and the BSM case respectively. 
Operators of interest are listed [here](https://www.dropbox.com/s/e5yvvzzo98bwdg3/2019-06-ongoing_dim6.pdf?dl=0)
  * The script ```postProcess.py``` takes as input a ```*_results``` folder,
    controls some basic parameters for the success of the generation,
    unpacks the LHE files, creates a summary of the run,
    and cleans the folder from unnecessary log files when called with the ```clean``` option
    (keeping those of failed jobs).
    It also creates the input cfg file for ```read_03.cpp```,
    adding the cfg file to the folder itself

## analysis

  * `read_02.cpp` reads LHE files and produces sets of histograms, NOT MAINTAINED
  * `read_03.cpp` reads LHE files and produces sets of ntuples
    * takes as input argument a config file: ```./read_03 cfg/ntuple_SSeu_RcW_0p3.cfg```
    * an additional argument sets the total number of events read per sample: ```./read_03 cfg/ntuple_SSeu_RcW_0p3.cfg 100```
    * all variables used later on are created here, for the time being
  * `checkEntries.cpp` checks the number of entries of all ntuples stored in a root file

<!-- ### fit

This is where the plain ntuples get analysed and fits performed.
 -->
## DatacarCreator

### datacard\_creator\_2.cpp

The code has been mostly taken from ```datacard_creator.cpp```. 
Fragmented in subfunctions implemented in ```dcutils.*```,
reads a config file containing the list of variables to be studied,
the selections to be applied,
the files to be read and some detailed configuration parameters
  * files have to be produced with ```read_03.cpp```, 
    which is where variables are built. 
    If a new variable is needed,
    for the time being a new run of ```read_03.cpp``` is required.
  * for each variable as an input in the cfg file,   
    ```datacard_creator_2.cpp``` produces a pair of files:
    a root file containing histograms and a datacard for Combine.
  * In the folder where datacards are stored, a shell script gets created,
    that contains the commands to be run to create the roofit workspaces
    needed by Combine, as well as one to issue the combine fit.
    The script should be sourced from an environment built according to the indications
    present in EFT/Fit.
    Single .sub and .sh files to be launched with condor get created as well.

### datacard\_TwoOp\_creator\_2.cpp

  * Prepares the combine datacards for 2D ```combine``` fits.
  * In the folder where datacards are stored, a shell script gets created,
    that contains the commands to be run to create the roofit workspaces
    needed by Combine, as well as one to issue the combine fit.
    The script should be sourced from an environment built according to the indications
    present in EFT/Fit.
    Single .sub and .sh files to be launched with condor get created as well.

### Condor submission

To submit to condor the creation of the workspaces and the ```combine``` fitting procedure,
launch ```condor_submit``` with as argument the submit\*.sub file of interest
from the created datacard folders.

### read_results.cpp

Runs over the rootfiles resulting from the ```combine``` fits and produces 1D likelihood scans

## PostPlots

  * summary plots from the existing results produced by ```read_results.cpp```

## useful commands

  * unzip madgraph results within a folder
   ```
   for fil in  `find  . -name "*gz"` ; do gunzip $fil ; done
   ```
  * prepare list of lhe files for cfg files (rem unalias ls)
   ```
   find . -name "*lhe" | sed -e s%\.%`pwd`% | tr "\n" ","
   ```
  * calculate the cross-section of the sample from the generated events, in the generation folder:
   ```
   ./postProcess.py [folder containing the job outputs and lhe files]
   ```



