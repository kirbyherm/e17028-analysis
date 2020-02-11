#!/usr/bin/env python3
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys, os

# script, first, second = sys.argv


def read_file( filename ):

    with open(filename,'r') as f:
        centroid, segment = [], []
        i = 1
        for line in f:
            if i < 3:
                print(line)
                i += 1
                continue
            segment.append(str(line.split()[0]))
            centroid.append(float(line.split()[2]))
    segment = np.array(segment)
    centroid = np.array(centroid)
    print(centroid, segment)
    df = pd.DataFrame(data={ 'segment' : segment, 'centroid' : centroid }) 
    return df

def main():

    df = read_file('centroids.txt')
    middle_segment_index = df.index[df["segment"]=="E_SuN_1_2_2"]
    norm_factor = np.sum(df.iloc[middle_segment_index]['centroid'])
    df['normalized'] = df['centroid']/norm_factor
    df.to_csv('norm_centroids.txt', sep="\t")    


if __name__ == "__main__":
    main()
