#!/usr/bin/env python

#SBATCH --output=parallel_plot.txt
#SBATCH -n 10

import matplotlib
matplotlib.use('agg')

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys, os
import root_numpy as rnp
from ROOT import TChain
from ROOT import TFile
from sklearn.cluster import KMeans
import multiprocessing

script, first = sys.argv

reduced_data = np.loadtxt("full_run.csv", delimiter=",")
#reduced_data = np.zeros((100,2))

def read_file( filename ):

    rfile = TFile(filename)
    intree = rfile.Get('t')
    #arr = rnp.root2array(filename,'t')
    #good_labels = []
    #for i in range(len(arr.dtype)):
    #    if arr.dtype[i] not in [np.dtype('int32'), np.dtype('float64')]:
    #        continue
    #    good_labels.append(arr.dtype.names[i])
    good_labels = ['energy_PIN1', 'energy_TAC_PIN1_I2N', 'energy_TAC_I2N_I2S']
    array = rnp.tree2array(intree,branches=good_labels)
    df = pd.DataFrame(array)
    df = df[(df != 0).all(1)]
    df['energy_TAC_PIN1_I2N_corr'] = (df['energy_TAC_PIN1_I2N'] + df['energy_TAC_I2N_I2S']*1.3) + 1500
    df = df[(df['energy_TAC_PIN1_I2N_corr'] > 13000) & (df['energy_TAC_PIN1_I2N_corr'] < 22000)]
    df = df[(df['energy_PIN1'] > 4000) & (df['energy_PIN1'] < 9000)]
#    print(df)
    return df

def get_df( start, stop=-1 ):
    if stop==-1:
        stop = start+1
    df_full = pd.DataFrame()
    for i in range(start, stop):
        df = pd.DataFrame()
        run_str = str(i)
        rootfiles = "/mnt/analysis/e17028/rootfiles/"
        filepath = rootfiles + "run" + run_str + "-*.root"
        run_subs = int(os.popen("ls " + filepath + " | wc -l").read().strip())
        for j in range(run_subs):
            filename = ""
            if j > 9:
                filename = rootfiles + "run" + run_str + "-"+str(j) + ".root" 
            else:
                filename = rootfiles + "run" + run_str + "-0"+str(j) + ".root" 
            print(filename)
            df_temp = read_file(filename)
            df = df.append(df_temp,ignore_index=True)
        if df.shape[0] < 50000:
            continue
        else:
            print(i, df.shape[0])
            df_full = df_full.append(df, ignore_index=True) 

    return df_full

def make_plot( n ):

    kmeans = KMeans(init='k-means++', n_clusters=n, n_init=10)
    kmeans.fit(reduced_data)
    
    # Step size of the mesh. Decrease to increase the quality of the VQ.
    h = 10.0     # point in the mesh [x_min, x_max]x[y_min, y_max].
    
    # Plot the decision boundary. For that, we will assign a color to each
    x_min, x_max = reduced_data[:, 0].min() - 1, reduced_data[:, 0].max() + 1
    y_min, y_max = reduced_data[:, 1].min() - 1, reduced_data[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))
    
    # Obtain labels for each point in mesh. Use last trained model.
    Z = kmeans.predict(np.c_[xx.ravel(), yy.ravel()])
    
    # Put the result into a color plot
    Z = Z.reshape(xx.shape)
    plt.figure(1)
    plt.clf()
    plt.imshow(Z, interpolation='nearest',
               extent=(xx.min(), xx.max(), yy.min(), yy.max()),
               cmap=plt.cm.Paired,
               aspect='auto', origin='lower')
    
    plt.hist2d(reduced_data[:, 0], reduced_data[:, 1], bins=1000, cmin=1)
    # plt.plot(reduced_data[:, 0], reduced_data[:, 1], 'k.', markersize=2)
    # Plot the centroids as a white X
    centroids = kmeans.cluster_centers_
    plt.scatter(centroids[:, 0], centroids[:, 1],
                marker='x', s=169, linewidths=3,
                color='w', zorder=10)
    plt.title('K-means clustering on particle ID\n'
              'Centroids are marked with white cross')
#    x_min = 14000
#    x_max = 21000
#    y_min = 4500
#    y_max = 8500
    plt.xlim(x_min, x_max)
    plt.ylim(y_min, y_max)
    print(x_min, x_max, y_min, y_max)
    plt.ylabel('dE in PIN1 (arb. units)')
    plt.xlabel('corrected TOF I2N-PIN1 (arb. units)')
    plt.xticks(())
    plt.yticks(())
    plt.savefig("pngs/"+first+"_"+str(n)+"test.png")

    return

def test_print( i ):
    print( i )
    print( reduced_data.shape )
    return

def main():

    # reduced_data = PCA(n_components=2).fit_transform(data)
    # df = get_df(2045,2171) 
    # reduced_data = np.asarray(df[['energy_TAC_PIN1_I2N_corr','energy_PIN1']])
    # np.savetxt("full_run.csv",reduced_data, delimiter=",")
    # reduced_data = reduced_data[:100]
    # print(reduced_data)
    print("Complete")

    return

if __name__ == "__main__":
    p = multiprocessing.Pool( processes = 10 )
    n_range = range(56,91)
    p.map_async(make_plot, n_range)
#    p.map_async(test_print, n_range)
    p.close()
    p.join()
    main()
