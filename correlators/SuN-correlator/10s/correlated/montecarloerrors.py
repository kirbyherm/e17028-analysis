import numpy as np
import matplotlib.pyplot as plt
import math 
from scipy.stats import chisquare

dc = open("decaycurveV.dat")
binno, h = np.loadtxt(dc,unpack=True)

daughterl = 0.693/(237/2)
ndaughterl = 0.693/(490/2)
gdaughterl = 0.693/(709/2)
ngdaughterl = 0.693/(280/2)

#pn = 0.15
#bg = 790
#ia = 7484


def bateman(x,y):
      
      C1 = ia #Initial parent activity
   
      #Bateman equations for daughter
      dC1 = daughterl*C1/((daughterl-y))
      dC2 = daughterl*C1/((y-daughterl))
   
      #Bateman equation for neutron daughter
      ndC1 = ndaughterl*C1/((ndaughterl-y))
      ndC2 = ndaughterl*C1/((y-ndaughterl))

      #Bateman equation for grand-daughter
      gdC1 = gdaughterl*daughterl*C1/((daughterl-y)*(gdaughterl-y))
      gdC2 = gdaughterl*daughterl*C1/((y-daughterl)*(gdaughterl-daughterl))
      gdC3 = gdaughterl*daughterl*C1/((y-gdaughterl)*(daughterl-gdaughterl))
      
      #Bateman equation for neutron grand-daughter
      ngdC1 = ndaughterl*ngdaughterl*C1/((ndaughterl-y)*(ngdaughterl-y))
      ngdC2 = ndaughterl*ngdaughterl*C1/((y-ndaughterl)*(ngdaughterl-ndaughterl))
      ngdC3 = ndaughterl*ngdaughterl*C1/((y-ngdaughterl)*(ndaughterl-ngdaughterl))

      N = C1*(np.exp(-y*x)) + (1-pn)*(dC1*np.exp(-y*x) + dC2*np.exp(-daughterl*x)) + (pn)*(ndC1*np.exp(-y*x) + ndC2*np.exp(-ndaughterl*x)) + bg + (1-pn)*(gdC1*np.exp(-y*x)+ gdC2*np.exp(-daughterl*x) + gdC3*np.exp(-gdaughterl*x)) + (pn)*(ngdC1*np.exp(-y*x)+ ngdC2*np.exp(-ndaughterl*x) + ngdC3*np.exp(-ngdaughterl*x))

#nDaughter
      return N

def chisq(x,y):
  delta = 0
  chisqval=0
  for index,item in enumerate(x):
    if item <=0:
     delta = 0
    else:
     delta = (item-y[index])/np.sqrt(item)
    chisqval += delta*delta
    return chisqval

listo = []
decaypl = []
for j in np.arange(0,1000):
  fc = []
  decayp = np.random.normal(0.0313,0.00015)
  decaypl.append(decayp)
  ia = np.random.normal(1928,26.663)
  bg = np.random.normal(310,6.8)
  pn = np.random.normal(0.132,0.002)
  for k in binno:
    fc.append(bateman(k+0.5,decayp))
    #fcarray = np.asarray(fc)
  chisqu = chisquare(h,f_exp=fc)
  
  listo.append(chisqu[0])
plt.scatter(decaypl,listo)
#plt.ylim(100,1000)
plt.show()
print np.mean(listo), np.std(listo)

#newlist = []
#intpl = []
#for intp in np.linspace(0.029,0.031,1000):
#  intpl.append(intp)
#  fcurve=[]
#  for l in binno:
#    fcurve.append(bateman((l+0.5),intp))
#  v = chisquare(h,f_exp=fcurve)
#  newlist.append(v[0])
#plt.scatter(intpl,newlist)
#plt.show()
#plt.hist(listo)
#herr = np.sqrt(h)
#plt.plot(h)
#plt.errorbar(binno,h,yerr=herr)
#plt.plot(fc)
#plt.show()
