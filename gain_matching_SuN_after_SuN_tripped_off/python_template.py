#!/usr/bin/env python3
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys, os

# script, first, second = sys.argv


def read_file( read_list, filename ):

    for each in read_list:
        first = each
        with open('centroids.txt','r') as f:
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

    print("hello world")

if __name__ == "__main__":
    main()
