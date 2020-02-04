#!/bin/bash

# input variables
# ---.---.---.---.---.---.---.---.---

# $1 = ClusterId
# $2 = ProcId
# $3 = process_name
# $4 = events_folder
# $5 = madgraph_folder
# $6 = results_folder
# $7 = cmssw_folder
# $8 = events_number

# copy the madgraph folder in the job temporary folder
# and enter the folder
# ---.---.---.---.---.---.---.---.---

mkdir -p $6/$1 # create a folder if it does not exist
touch $6/$1/$4_running
echo "copying from "$5 >> $6/$1/$4_running

cp -r $5 .
cd $3

if ! [ -z "$7" ]
  then
  	cd $7
  	eval `scram run -sh`
  	cd -
    echo "CMSSW env setup from :"$7 >> $6/$1/$4_running
fi

events_number=10000
if ! [ -z "$8" ]
  then
  	events_number=$8 
fi

echo "working dir:" >> $6/$1/$4_running
pwd >> $6/$1/$4_running

# run the event production
# ---.---.---.---.---.---.---.---.---

echo "submitting: printf \"0\\n set iseed $RANDOM\\n set nevents $events_number\\n done\\n\" |./bin/generate_events "$4 >> $6/$1/$4_running
printf "0\n set iseed $RANDOM\n set nevents $events_number\n done\n" | ./bin/generate_events $4
echo "event generation done" >> $6/$1/$4_running
# syntax: generate_events [run_name] [options]
# -- Launch the full chain of script for the generation of events
#    Including possible plotting, shower and detector resolution.
#    Those steps are performed if the related program are installed
#    and if the related card are present in the Cards directory.
# -- local options:
#       -f : Use default for all questions.
#       --laststep= : argument might be parton/pythia/pgs/delphes and indicate the last level to be run.
#       -M : in order to add MadSpin
#       -R : in order to add the reweighting module
# -- session options:
#       Note that those options will be kept for the current session
#       --cluster : Submit to the  cluster. Current cluster: condor
#       --multicore : Run in multi-core configuration
#       --nb_core=X : limit the number of core to use to X.
# -- if I want to change an EFT parameter, i.e. a term in the param_card.dat:
#       set FRBLOCK 21 0.05  : this changes cW, i.e. the 21st parameter in the FRBLOCK

# move somewhere else the result
# ---.---.---.---.---.---.---.---.---

echo "copying events to "$6"/"$1 >> $6/$1/$4_running

cp -r Events/$4 $6/$1
mv $6/$1/$4_running $6/$1/$4.log


# FIXME get more details on generate_events options
