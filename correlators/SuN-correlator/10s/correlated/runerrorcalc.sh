#!/bin/bash

for i in $(seq 0 10000);do
  root -l -q -b ../../../analysis-scripts/montecarloVerror.C
done
