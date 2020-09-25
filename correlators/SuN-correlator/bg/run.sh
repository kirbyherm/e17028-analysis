#!/bin/bash

#while read p;
#do
#  ./backcorrelator_newsuncalib $p true 1 md 1 0 all ti57
#  echo $p
#done < runlist
#./backcorrelator_newsuncalib $1 true 3 md 0.2 0 ti57
#./backcorrelator_newsuncalib $1 true 3 md 0.5 0 cr61 
#ar49 ar50 ar51 k51 k52 k53 ca52 ca53 
#for value in ca54 ca55 sc55 sc56 sc57 sc58 ti57 ti58 ti59 ti60 v59 v60 v61 v62 cr61 cr62 cr63 cr64 mn63 mn64 mn65 
#for value in ti57 #sc57
for value in ca54 ca55 sc56 sc57 sc58 ti57 ti58 ti59 ti60 v61 #v62 cr61 cr62 cr63 cr64 mn63 mn64 mn65
do
    ./backcorrelator_newsuncalib $1 true 2 md 1.0 0 $value 
done
echo "Run"$1
