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

for TOSSES in $(seq 0 1 99) 
do
  QSUB="/mnt/home/herman67/skynet_pp/run_sh/run"$TOSSES"_0.sh"
#  QSUB2="/mnt/scratch/herman67/skynet_post_processing/run_sep12_"$TOSSES"/run2.qsub"
  sbatch $QSUB 
#  sh $QSUB
done

