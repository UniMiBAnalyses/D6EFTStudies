# Make reading LHE produced with gridpacks blazing fast

## Preliminary step


In order to make the script work, you should first create a symlink to your `EFT_LHE` inside the lheReaderFolder. 
Just do:

```cd lheReaderFolder; ln -s /eos/user/initial/username/EFT_LHE; cd ..```

The main script will work with a subfolder of EFT\_LHE called `gp_ntuples`, if you already have this folder you should consider changing its name because the script will use it to store a lot of .root files.

## First step: use PostProcess


`postProcessGP.py` is a modified version of postProcess.py from the generation folder that will do the same things (a little bit quicker) and save each xs +/- errors for each LHE file. This step is necessary for the readLheThreading script.
You should take advantage of the symlink to EFT\_LHE created before for the path to the lhe folder.


## Main program: readLheThreading.py


Once you do that you're ready to use the main script: `readLheThreading.py`.

In order to read and parse LHE fast the script submits to condor_q a multithreading script ( `readLheThreadingScript.py` ) which will use `read_03` from the analysis folder of this repo, so please, first make sure that you can run `./read_03` inside analysis.

In order to read reweights from a LHE one should first edit utils/autils.cc, at line 559 you should add this:
```
      for(int wgt_idx = 1; wgt_idx <= NUMBER_OF_REWEIGHTS; ++wgt_idx){
         Ntuple.setvalue(Form("rwgt_%d", wgt_idx), reader.hepeup.namedweights[wgt_idx-1].weights[0]);
      }
```
If you don't know the number of reweights of a given LHE (because you're a lazy person), use the formula:
` n_reweights = n(n+3)/2 + 1 ` where n is the number of operators.

Once you have the correct compiled version of read_03 then just execute:

```python3 readLheThreading.py -cfg PATH_TO_CFG_FROM_POSTPROCESS -b=1```
where `-b=1` is the shorthand for ```-doBatch=1``` which will submit reading to condor

Finally when all the jobs are completed and the gp_ntuples folder is full of .root files then just merge them together with
```python3 readLheThreading.py -cfg SAME_PATH_AS_BEFORE --doHadd=1 --nThreads=10``` 
which will create a new complete ntuple called: `ntuple_SAMPLE_NAME.root`




## What is happening?

The script readLheThreading.py submit multiple jobs to htcondor each of which will read and parse LHE files with multithreading. 
In order to run the script one should specify the path to the read\_03 config file created with the provided postProcess.py. The latter the script is a modified
version of the one in generation, the main difference is that is saves in the read cfg file, xs and xs errors for all lhe.
In order to have just one complete ntuple, one should, after having read the lhe files with ```python3 readLheThreading.py -cfg path_to_cfg --doBatch=1```, one should run the script
again with the option `--doHadd=1` which will merge all the previous ntuples in just one and recalculate the cross section



