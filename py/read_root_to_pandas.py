#!/usr/bin/env python
#SBATCH --output=2045_90.txt
#SBATCH -n 1

import matplotlib
matplotlib.use('agg')

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys, os
import root_numpy as rnp
from ROOT import TChain
from ROOT import TFile
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

script, first = sys.argv


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
    return df

def get_df( start, stop=-1 ):
    if stop==-1:
        stop = start+1
    df = pd.DataFrame()
    for i in range(start, stop):
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
    # print(start, df.shape)
    return df


def main():

    # reduced_data = PCA(n_components=2).fit_transform(data)
    df = get_df(int(first)) 
    print(first, df.shape[0])
    if df.shape[0] < 50000:
        return
    reduced_data = np.asarray(df[['energy_TAC_PIN1_I2N_corr','energy_PIN1']])
    # print(reduced_data)
    # scaler = StandardScaler()
    # reduced_data = scaler.fit_transform(reduced_data.astype(np.float64))
    # want to mess around with the tolerance parameter, will probably help the clustering
    kmeans = KMeans(init='k-means++', n_clusters=90, n_init=10)
    kmeans.fit(reduced_data)
    
    # Step size of the mesh. Decrease to increase the quality of the VQ.
    h = 0.01     # point in the mesh [x_min, x_max]x[y_min, y_max].
    h = 10.0
 
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
    print(x_min, x_max, y_min, y_max)
    x_min = 14000
    x_max = 21000
    y_min = 4500
    y_max = 8500
    plt.xlim(x_min, x_max)
    plt.ylim(y_min, y_max)
    plt.ylabel('dE in PIN1 (arb. units)')
    plt.xlabel('corrected TOF I2N-PIN1 (arb. units)')
    plt.xticks(())
    plt.yticks(())
    plt.savefig("pngs/"+first+"test.png")
    # plt.show()
    np.savetxt("centroids/"+first+"_centroids.csv",centroids,delimiter=',')

    # plt.hist2d( df['energy_TAC_PIN1_I2N_corr'], df['energy_PIN1'], bins=1000, range=[[13000,22000],[4000,8500]])
    # plt.show()

    return

if __name__ == "__main__":
    main()
