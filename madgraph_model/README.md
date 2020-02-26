# addons for the SMEFT UFO

## generating the restrict files

The script ```buildRestrict.py``` creates a full set of restriction files,
each of which switches on only one EFT operator,
to accelerate the gridpack generation process by minimising the number of diagrams to be calculated
by Madgraph.
It requires the presence of the two files ```restrict_before.txt``` and ```restrict_and.txt```, 
which should contain the immutable part of the restriction file, 
while the script itself simply adds the central part, 
which is the values for each Wilson coefficient.

## folders

  * ```old``` contains the restrict files working with the first version of SEMEFTSim
  * ```v3``` contains the restrict files working with the version *v3* of SMEFTSim