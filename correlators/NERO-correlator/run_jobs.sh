#!/bin/bash
# Runs with $ sh quickrun.sh # # # ...
#
# Compile code.
#
# make
#
# Execute tests.
#
# echo -e "version\tn\tmax\tl2norm\ttime" 

for TOSSES in $(seq 1036 1 1068) 
do
    JOB_NAME=$1$TOSSES
    sbatch -J $JOB_NAME --output="slurmfiles/"$TOSSES"_1.txt" ./corr1.sh $TOSSES
    sbatch -J $JOB_NAME --output="slurmfiles/"$TOSSES"_3.txt" ./corr3.sh $TOSSES
    sbatch -J $JOB_NAME --output="slurmfiles/"$TOSSES"_5.txt" ./corr5.sh $TOSSES
done
