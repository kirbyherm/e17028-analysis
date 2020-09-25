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

#for TOSSES in $(seq 172 1 172) 
for TOSSES in $(seq 2050 1 2168) 
do
  QSUB="/mnt/analysis/e17028/hermansen/sh/"
  JOB_OUT=$QSUB"job_outputs/run"$TOSSES".job"
  ERROR_OUT=$QSUB"error_outputs/run"$TOSSES".error"
  JOB_NAME=$TOSSES"pid_cleanup"
  sbatch -J $JOB_NAME --output=$JOB_OUT --error=$ERROR_OUT array_batchjob.sh $TOSSES 2060
done

