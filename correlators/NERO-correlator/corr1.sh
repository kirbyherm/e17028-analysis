#!/bin/bash
#SBATCH --output=parallel_plot.txt

#./correlator $1 true 1 md 10 0 sc55 
#./correlator $1 true 1 md 10 0 sc56 
#./correlator $1 true 1 md 10 0 sc57 
#./correlator $1 true 1 md 10 0 ti57 
#./correlator $1 true 1 md 10 0 ti58 
#./correlator $1 true 1 md 10 0 ti59 
#./correlator $1 true 1 md 10 0 v59 
#./correlator $1 true 1 md 10 0 v60 
#./correlator $1 true 1 md 1 0 v61 
#./correlator $1 true 1 md 10 0 v62 
#./correlator $1 true 1 md 10 0 cr62 
#./correlator $1 true 1 md 10 0 cr63 
./correlator2 $1 true 1 md 1 10 ti59 
./correlator2 $1 true 1 md 1 10 v61 
./correlator2 $1 true 1 md 1 10 ca55 
./correlator2 $1 true 1 md 1 10 sc55 
./correlator2 $1 true 1 md 1 10 sc56 
./correlator2 $1 true 1 md 1 10 sc57 
./correlator2 $1 true 1 md 1 10 ti57 
./correlator2 $1 true 1 md 1 10 ti58 
./correlator2 $1 true 1 md 1 10 ti60 
./correlator2 $1 true 1 md 1 10 v59 
./correlator2 $1 true 1 md 1 10 v60 
./correlator2 $1 true 1 md 1 10 v62 
./correlator2 $1 true 1 md 1 10 cr62 
./correlator2 $1 true 1 md 1 10 cr63 
#./correlator $1 true 0.5 md 1 0 sc55 
#./correlator $1 true 0.5 md 1 0 sc56 
#./correlator $1 true 0.5 md 1 0 sc57 
#./correlator $1 true 0.5 md 1 0 ti57 
#./correlator $1 true 0.5 md 1 0 ti58 
#./correlator $1 true 0.5 md 1 0 ti59 
#./correlator $1 true 0.5 md 1 0 v59 
#./correlator $1 true 0.5 md 1 0 v60 
#./correlator $1 true 0.5 md 1 0 v61 
#./correlator $1 true 0.5 md 1 0 v62 
#./correlator $1 true 0.5 md 1 0 cr62 
#./correlator $1 true 0.5 md 1 0 cr63 
#./correlator $1 true 0.3 md 1 0 sc55 
#./correlator $1 true 0.3 md 1 0 sc56 
#./correlator $1 true 0.3 md 1 0 sc57 
#./correlator $1 true 0.3 md 1 0 ti57 
#./correlator $1 true 0.3 md 1 0 ti58 
#./correlator $1 true 0.3 md 1 0 ti59 
#./correlator $1 true 0.3 md 1 0 v59 
#./correlator $1 true 0.3 md 1 0 v60 
#./correlator $1 true 0.3 md 1 0 v61 
#./correlator $1 true 0.3 md 1 0 v62 
#./correlator $1 true 0.3 md 1 0 cr62 
#./correlator $1 true 0.3 md 1 0 cr63 
