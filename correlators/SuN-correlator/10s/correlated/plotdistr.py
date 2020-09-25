import numpy as np
import matplotlib.pyplot as plt
import sys

f=open(sys.argv[1])
x,y,z,l = np.loadtxt(f,usecols=(0,1,2,3),unpack=True)
print np.std(y)
plt.hist(y,bins=100)
plt.show()
