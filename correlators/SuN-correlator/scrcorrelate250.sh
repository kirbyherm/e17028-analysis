#!/bin/bash


#for value in ar49 ar50 ar51 k51 k52 k53 ca52 ca53 #sc55 sc56 sc57 sc58 ti57 ti58 ti59 ti60 v59 v60 v61 v62 cr61 cr62 cr63 cr64 mn63 mn64 mn65
#for value in ti57 #sc57 # ti57
for value in ca54 ca55 sc56 sc57 sc58 ti57 ti58 ti59 ti60 v61 #v62 cr61 cr62 cr63 cr64 mn63 mn64 mn65
do
    ./correlator_newsuncalib $1 true 3 md 0.25 0 $value
done
