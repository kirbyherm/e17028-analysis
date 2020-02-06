#!/usr/bin/env bash

# Change the experiment number
exp_number=e17028

echo -e " ------------------------------------------------------------------ \n"
echo -e "Hello $exp_number\n" 
echo -e "use './watching.sh &' to watch the squeue in real time\n"
echo -e "You can convert multiple runs at the same time \n"
echo -e "Output messages are saved in slurmfiles \n"
echo -e "Check on the status of a run; ./check_job.sh XXXX \n"
echo -e "if anything doesn't make sense, read the slurmfile \n"
echo -e "don't forget about scancel if a process gets hung up\n"
echo -e " ------------------------------------------------------------------ \n"

sleep 2s

run_number="$1"

# Check how many files there are
number_of_segments=$(ls -f /mnt/rawdata/${exp_number}/experiment/run${run_number}/ | grep "evt" | wc -l)
echo "There are ${number_of_segments} evt files in run${run_number}."

for (( segment_number=0;
       segment_number<${number_of_segments};
       segment_number++ )); do
    
    if [ ${segment_number} -lt 10 ]
    then
	file_number=${run_number}-0${segment_number}
    else
	file_number=${run_number}-${segment_number}
    fi

    sbatch --reservation=e17028 --wrap "./convert_1-step_fireside.sh ${run_number} ${file_number}" >>log_run-${run_number}   

done

cat log_run-${run_number}
echo -e " \n"

sleep 2s

if test -f log_run-${run_number}
then
    mv log_* logfiles
    mv slurm-* slurmfiles
fi
