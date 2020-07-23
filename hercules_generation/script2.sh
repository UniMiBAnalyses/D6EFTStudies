#!/bin/bash
# $1 is distribution name and must be the some of the project folder name

mkdir -p /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/$1
mkdir -p /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/results_$1
cp /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/SSWW_RcW_bsm/SSWW_RcW_bsm_run_card.dat /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/$1/$1_run_card.dat
cp gen3/$1/Cards/param_card.dat /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/$1/$1_param_card.dat
cp gen3/$1/Cards/proc_card_mg5.dat /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/$1/$1_proc_card.dat
cp /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/SSWW_RcW_bsm/SSWW_RcW_bsm_extramodels.dat /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/cards/eft/$1/${1}_extramodels.dat

cd /gwpool/users/gpizzati/genproductions/bin/MadGraph5_aMCatNLO/
cp temp.sub sub_${1}.sub
sed -i '11 i\process_name\t\t='$1 sub_${1}.sub
./submit_condor_gridpack_generation.sh $1 cards/eft/$1
