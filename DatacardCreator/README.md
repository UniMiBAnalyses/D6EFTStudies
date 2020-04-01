# create datacards for the Combine tool

## datacard\_creator
It reads .cfg file, file\_W.cfg is an example. From the .cfg it reads which .root file contains the SM, 
interference and BSM terms, cuts and also which variable to analyse.

It creates a datacard.txt for each variable specified and also a .root which will be passed to Combine.

It also creates different type of plots(stack, log and shape), putting the three distributions together.

## datacard\_creator\_2

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
    needed by Combine. 
    The script should be sourced from an environment built according to the indications
    present in EFT/Fit.

## datacard\_twoOp\_creator
It is used to study two operators together creating, as datacard\_creator both datacard files and plots.

## range_vs_kineticvar
Reads a .txt containing the confidence interval's bounds found with Combine. It sorts the list of variables and plots the width of the C.I.
