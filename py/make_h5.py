#!/usr/bin/env python
# import pandas as pd
# import numpy as np
# import matplotlib.pyplot as plt
# import sys, os
from rootpy import root2hdf5

# script, first, second = sys.argv


def read_file( read_list, filename ):

    for each in read_list:
        first = each
        with open(filename+first+'.dat','r') as f:
            r, t = [], []
            i = 1
            for line in f:
                if i < 3:
                    print(line)
                    i += 1
                    continue
                t.append(float(line.split()[0]))
                r.append(float(line.split()[3]))
        
        plt.plot(t,r)
    return

def main():

    root2hdf5.root2hdf5("/user/hermanse/analysis/rootfiles_test/run2126-00.root","/user/hermanse/analysis/rootfiles_test/run2126-00.h5")

if __name__ == "__main__":
    main()
