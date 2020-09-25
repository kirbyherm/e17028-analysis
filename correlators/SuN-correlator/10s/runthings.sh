#!/bin/bash

echo "I am forward correlating with tight Fe cut"
mpirun --host jinacalc:64 --app corr_Fe
echo "I finished forward correlating with tight Fe cut"
echo "Moving on..."

echo "I am backward correlating with tight Fe cut"
mpirun --host jinacalc:64 --app backcorr_Fe
echo "I finished backward correlating with tight Fe cut"
echo "Moving on..."

echo "I am backward correlating with V cut"
mpirun --host jinacalc:64 --app backcorr_V
echo "I finished backward correlating with V cut"
echo "Moving on..."

echo "I am forward correlating with V cut"
mpirun --host jinacalc:64 --app corr_V
echo "I finished forward correlating with V cut"
echo "Moving on..."

echo "I am backward correlating with Mn cut"
mpirun --host jinacalc:64 --app backcorr_Mn
echo "I finished backward correlating with Mn cut"
echo "Moving on..."

echo "I am forward correlating with Mn cut"
mpirun --host jinacalc:64 --app corr_Mn
echo "I finished forward correlating with Mn cut"
echo "Moving on..."

