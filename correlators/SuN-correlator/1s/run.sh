#!/bin/bash

while read p;
do
  ./../correlator $p true 1 md 1 0 all V
  echo $p
done < runlist
