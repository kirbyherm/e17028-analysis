import numpy as np
import matplotlib.pyplot as plt
import sys
inputfile = open(sys.argv[1])
(m1,m2,m3,m4,m5,m6,m7,m8,m9) = np.loadtxt(inputfile, unpack = True)
tot = []
tot.append(m1*100/(sum(m1)))
tot.append(m2*100/(sum(m2)))
tot.append(m3*100/(sum(m3)))
tot.append(m4*100/(sum(m4)))
tot.append(m5*100/(sum(m5)))
tot.append(m6*100/(sum(m6)))
tot.append(m7*100/(sum(m7)))
tot.append(m8*100/(sum(m8)))
tot.append(m9*100/(sum(m9)))

tot2d = np.transpose(tot)
print tot2d

plt.plot(tot2d[0],label="mult=0")
plt.plot(tot2d[1],label="mult=1")
plt.plot(tot2d[2],label="mult=2")
plt.plot(tot2d[3],label="mult=3")
plt.plot(tot2d[4],label="mult=4")
#plt.plot(tot[0],label="m1")
#plt.plot(tot[1],label="m2")
#plt.plot(tot[2],label="m3")
#plt.plot(tot[3],label="m4")
#plt.plot(tot[4],label="m5")
#plt.plot(tot[5],label="m6")
#plt.plot(tot[6],label="m7")
#plt.plot(tot[7],label="m8")
#plt.plot(tot[8],label="m9")
plt.legend()
plt.show()
