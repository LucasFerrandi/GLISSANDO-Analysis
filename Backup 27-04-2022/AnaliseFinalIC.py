#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr 26 20:13:09 2022

@author: lucas
"""

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar  1 00:50:15 2022

@author: lucas
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


r_2=np.array([0.2, 0.8])
r_2_10=10*r_2 #Usado para nome de arquivo
numb=7
numr2=6
Nevts=250
bmax=delete12
x=1
x=1
x=1
x=1
x=1
y=4

data = pd.read_csv(
'/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeedsbb/ResultHS_Total.txt',
sep=";", header=None)

data.columns=(
["$\epsilon_{Fix}^{Fireball}$",  #0
"$\epsilon_{Part}^{Fireball}$",  #1
"Mean $\epsilon_{Fix}^{HS}$",     #2
"Mean $\sigma(\epsilon_{Fix}^{HS})$",   #3
"Mean $\epsilon_{Part}^{HS}$",    #4
"$\sigma($Mean $\epsilon_{Part}^{HS})$",  #5
"$Energy^{HS}_{Mean}$",     #6
"$\sigma(Energy^{HS}_{Mean})$",   #7
"$\Psi_R$",              #8
"$\sigma(\Psi_R)$"])           #9

print(data.columns[0])
plt.figure(figsize=(10,7))

minbias=bool(1)#int(input("minimum bias? (1/0)")))


kr=0 #kontador de r

X=data.iloc[:,x]  #Toda a linha com coluna x
Y=data.iloc[:,y]  #Toda a linha com coluna y

'''
i=4000
f=i+249
print("i=",i)
print("f=",f)
plt.scatter(X[i:f],
        Y[i:f],label="b={}".format(b-2))
'''


    
if minbias:    
    print("minbias")
    for r in r_2:
        b=2
        plt.figure(dpi=70)
        for i in range (Nevts+(kr*(numb*Nevts)), (kr+1)*(numb*Nevts), Nevts): #primeira linha
            #para minbias, b=0 nao faz sentido
            Npontos=Nevts*(b/bmax) #Essência do minbias
            print("Npontos=",Npontos)
            f=int(i+Npontos-1)  #ultima linha
            print("i=",i)
            print("f=",f)
            b=b+2
            plt.scatter(X[i:f],
            Y[i:f],label="b={}".format(b-2))
        plt.title("{} Vs. {} for $r={}$fm".format(
        data.columns[y], data.columns[x], r))
        plt.xlabel(data.columns[x])
        plt.ylabel(data.columns[y])
        z = np.polyfit(X,Y,1)
        p = np.poly1d(z)
        plt.plot(X, p(X), "b-o",markersize='2', label=p)
        print(p)
        plt.legend()
        plt.show()
        kr=kr+1
    
if not minbias:    
    print("minbias")
    for r in r_2:
        b=0
        plt.figure(dpi=70)
        for i in range (Nevts+(kr*(numb*Nevts)), (kr+1)*(numb*Nevts), Nevts): #primeira linha
            #para minbias, b=0 nao faz sentido
            f=int(i+Nevts-1)  #ultima linha
            print("i=",i)
            print("f=",f)
            b=b+2
            plt.scatter(X[i:f],
            Y[i:f],label="b={}".format(b-2))
        plt.title("{} Vs. {} for $r={}$fm".format(
        data.columns[y], data.columns[x], r))
        plt.xlabel(data.columns[x])
        plt.ylabel(data.columns[y])
        z = np.polyfit(X,Y,1)
        p = np.poly1d(z)
        plt.plot(X, p(X), "b-o",markersize='2', label=p)
        print(p)
        plt.legend()
        plt.show()
        kr=kr+1
    


