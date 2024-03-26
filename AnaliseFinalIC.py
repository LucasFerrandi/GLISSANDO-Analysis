#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar  1 00:50:15 2022

@author: lucas
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
#Faz gráficos utilizando os resultados do .c

Nevts= 250 #n de eventos
r_2= 0.2 #raio do hotspot
r_2_10=int(10*r_2)
rstr=str(r_2_10)

local1="/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeeds/r2_hs="
local2=rstr
local3="e-1/ResultHS-Total.txt"
local=local1+local2+local3 #local a serem salvos os resultados
print("AAAA",local)



data = pd.read_csv(local, sep=";",header=None) #função do Pandas que transforma um arquivos em dados


data.columns=(
["epsFixFireball",  #0
"epsPartFireball",  #1
"epsFixHSMean",     #2
"σ_epsFixHSMean",   #3
"epsPartHSMean",    #4
"σ_epsPartHSMean",  #5
"EnergyHSMean",     #6
"σ_EnergyHSMean",   #7
"Ψ_R",              #8
"σ_Ψ_R"])           #9

print(data.columns[0])
plt.figure(dpi=150)

x= 1 #eixo x
y= 4 #eixo y
b=0 #parametro de impacto
    
X=data.iloc[:,x] #dados do Pandas sendo transformados em array do python
Y=data.iloc[:,y]
    
for i in range (0,8*Nevts,Nevts):
        b=b+2
        plt.scatter(X[i:(i+Nevts)],Y[i:(i+Nevts)],label="b={}".format(b-2), s=8)  #plot do grafico
plt.title("{} Vs. {} - r_hs={}fm".format(data.columns[y],data.columns[x],r_2))
plt.xlabel(data.columns[x])
plt.ylabel(data.columns[y])
z = np.polyfit(X,Y,1) #ajuste de polinomio de grau 1, linear
p = np.poly1d(z)
plt.plot(X, p(X), "b-o",markersize='0', label=p)
plt.legend()
#plt.show()
nome="r_hs="+rstr
plt.savefig(nome)

fout=open("coefs.txt",'a')
fout.write(str(r_2)+";"+str(z[0])+";"+str(z[1])+"\n")
