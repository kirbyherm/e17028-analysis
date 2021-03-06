#!/usr/bin/env bash
#
#SBATCH --job-name=make_plots_parallel
#SBATCH --ntasks=10

# Updated by MKS Mar 2019

#### USER REQUIRED INPUTS #####

# Change the experiment number
exp_number=e17028

#Change the ddas version if needed
ver=5.1

# Change path to runConverter
runconverterpath=/mnt/analysis/${exp_number}/rootfiles_test

###################################


#remember where ddasdumper lives
ddas_bin=/usr/opt/ddas/${ver}/bin

# Loads Root
#module load root

module load root/gnu/6.06.02
module load anaconda/python2.7
export PATH=${HOME}/.local/bin${PATH:+:$PATH}   

# Prompt run number
root_name="run2126-00.root"
h5_name="run2126-00.h5"

for i in $(seq 2045 1 2170)
do
    python read_root_to_pandas.py $i
done
