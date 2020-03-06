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
# $9 = operator number
# $10 = wilson coeff. value
# copy the madgraph folder in the job temporary folder
# and enter the folder
# ---.---.---.---.---.---.---.---.---
# reading variables

file="/gwpool/users/gpizzati/gen3/input_$2.txt"
while IFS=, read -r f1 f2 f3
do
	process_name=$f1
	madgraph_folder=/gwpool/users/gpizzati/gen3/${process_name}
	results_folder=/gwpool/users/gpizzati/gen3/$2
	events_folder=${process_name}_$2
	events_num=10000
	operator_number=$f2
	wilson_coeff=$f3
done <"$file"

mkdir -p $results_folder/$1 # create a folder if it does not exist
touch $results_folder/$1/${events_folder}_running
echo "copying from "$madgraph_folder >>$results_folder/$1/${events_folder}_running

cp -r $madgraph_folder .
cd $process_name

if ! [ -z "$3" ]
  then
	source /cvmfs/cms.cern.ch/cmsset_default.sh
	#source  /gwpool/initcms/root6-SLC6-standalone.sh	
	export SCRAM_ARCH=slc7_amd64_gcc820
	cd $3
	eval `scramv1 runtime -sh`
	#eval `scram run -sh`
  	cd -
    echo "CMSSW env setup from :"$3 >>$results_folder/$1/${events_folder}_running
fi

events_number=10000
if ! [ -z "$events_num" ]
  then
  	events_number=$events_num
fi

echo "working dir:" >> $results_folder/$1/${events_folder}_running
pwd >> $results_folder/$1/${events_folder}_running

# run the event production
# ---.---.---.---.---.---.---.---.---
specific=`date +"%Y_%m_%d_%H_%M"`
echo "submitting: printf \"0\\n set iseed $RANDOM\\n set nevents $events_number\\n set FRBLOCK $operator_number $wilson_coeff\\n done\\n\" |./bin/generate_events "$events_folder >> $results_folder/$1/${events_folder}_running
printf "0\n set iseed $RANDOM\n set nevents $events_number\n set FRBLOCK $operator_number $wilson_coeff\n done\n" | ./bin/generate_events ${events_folder}_$specific 
echo "event generation done" >> $results_folder/$1/${events_folder}_running
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

echo "copying events to "$results_folder"/"$1 >> $results_folder/$1/${events_folder}_running

cp -r Events/$events_folder $results_folder/$1
mv $results_folder/$1/${events_folder}_running $results_folder/$1/${events_folder}.log


# FIXME get more details on generate_events options
