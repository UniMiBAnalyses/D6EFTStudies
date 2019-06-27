#!/bin/bash

# input variables
# ---.---.---.---.---.---.---.---.---

# $1 = ClusterId
# $2 = ProcId
# $3 = process_name
# $4 = events_folder
# $5 = gridpack
# $6 = results_folder
# $7 = cmssw_folder
# $8 = events_number

# copy the madgraph folder in the job temporary folder
# and enter the folder
# ---.---.---.---.---.---.---.---.---

# create the output sub-folder if not existing
mkdir -p $6/$1/$4 # create a folder if it does not exist
touch $6/$1/$4_running

# setup the environment
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

# copy and unpack the gridpack
echo "copying from "$5 >> $6/$1/$4_running
cp -r $5 .
tar xzf $5

# submit the job
echo "submitting: ./run.sh $events_number $RANDOM" >> $6/$1/$4_running
./run.sh $events_number $RANDOM

# copy the LHE file back in the results folder
echo "copying events to "$6"/"$1"/"$4 >> $6/$1/$4_running
cp events.lhe.gz $6/$1/$4
mv $6/$1/$4_running $6/$1/$4.log


