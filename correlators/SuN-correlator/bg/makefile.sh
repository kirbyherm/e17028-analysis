#!/bin/bash

g++ backcorrelator_newsuncalib.cpp $(root-config --cflags) $(root-config --glibs) -std=c++11 -O3 -o backcorrelator_newsuncalib
