#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar  1 00:50:15 2022

@author: lucas
"""
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


r_2=np.array([0.2,
              0.8,
              #2.4, 5, 10, 15
              ])
r_2_10=10*r_2 #Usado para nome de arquivo
numb=7    #num de b presente no txt de input
numr2=6   #num de r2 presente no txt de input
Nevts=250
bmax=12   #bmax presente no txt de input
x=1
y=4
massa=2.54

bmaxplot=int(input("Qual bmax para o plot?"))  #bmax que desejo no plot
numbplot=(bmaxplot/2)+1 #num de b que desejo no plot


data = pd.read_csv(
'/home/lucas/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/DiferentesSeedscc/ResultHS_Total.txt',
sep=";", header=None)

data.columns=(
["$\epsilon_{Fix}^{Fireball}$",           #0
"$\epsilon_{Part}^{Fireball}$",           #1
"Mean $\epsilon_{Fix}^{HS}$",             #2
"Mean $\sigma(\epsilon_{Fix}^{HS})$",     #3
"Mean $\epsilon_{Part}^{HS}$",            #4
"$\sigma($Mean $\epsilon_{Part}^{HS})$",  #5
"$Energy^{HS}_{Mean}$",                   #6
"$\sigma(Energy^{HS}_{Mean})$",           #7
"$\Psi_R$",                               #8
"$\sigma(\Psi_R)$"])                      #9

Nomes=[  #Usado para título dos arq
   "epsFixFireball",   #0
   "epsPartFireball",  #1
   "epsFixHS",         #2
   "sigFixHS",         #3
   "epsPartHS",          #4
   "sigPartHS",        #5
   "EnergyHS",         #6
   "sigEnergyHS",      #7
   "Psi",              #8
   "sigPsi"            #9
   ]

#plt.figure(figsize=(10,7))

minbias=bool(int(input("minimum bias? (1/0)")))
X=data.iloc[:,x]  #Toda a coluna x
Y=data.iloc[:,y]  #Toda a coluna y

'''
i=1500
f=i+249
print("i=",i)
print("f=",f)
plt.scatter(X[i:f],
        Y[i:f],label="b={}".format(b-2))
'''

kr=0 #kontador de r
colors=['r', 'darkorange', 'gold', 'g', 'lightseagreen', 'b', 'purple']

if not minbias:    
   print("not minbias")
   for r in r_2:
      b=0
      fig=plt.figure(dpi=130)
      Npontos=np.array([])
      kb=0 #"numero" do b atual
      MEDX=[] #Media de x de cada nuvem
      MEDY=[]
      Xr=np.array([]) #todas cord x para esse r
      Yr=np.array([])
      for i in range(
      kr*int(numb*Nevts),
      int(kr*(numb*Nevts)+(numbplot*Nevts)),
      Nevts
      ): #primeira linha com o b considerado
         f=int(i+Nevts-1)  #ultima linha
         print("i=",i)
         print("f=",f)
         medx=np.sum(X[i:f])/Nevts
         #soma dos valores de x para cada nuvem
         medy=np.sum(Y[i:f])/Nevts
         #soma dos valores de y para cada nuvem
         MEDX.append(medx)
         MEDY.append(medy)
         b=b+2
         Xr=np.append(Xr, X[i:f])
         Yr=np.append(Yr, Y[i:f])             
         plt.scatter(  
            X[i:f],
            Y[i:f],
            label="b={}".format(b-2),
            s=5,
            c=colors[kb],
            alpha=0.5
            )            
         plt.scatter(
            medx,
            medy,
            c=colors[kb],
            s=100,
            edgecolors='black',
            alpha=1,
            #edgewidth=5
            #marker='+'
            )    
         kb=kb+1            
      plt.title(
         "{} Vs. {} for $r={}$fm and $mass={}$GeV".format(
            data.columns[y], data.columns[x], r, massa
            )
         )
      plt.xlabel(data.columns[x])
      plt.ylabel(data.columns[y])
      
      z = np.polyfit(Xr,Yr,1)
      p = np.poly1d(z)
      plt.plot(
            X, p(X), "-",
            color="orangered", markersize='0.5',
            label="Fit through all points({})".format(p),
            alpha=0.3
            )
        
      print("medx=",MEDX,"\n medy=", MEDY)
      z2 = np.polyfit(MEDX,MEDY,1)
      p2 = np.poly1d(z2)
      plt.plot(
         X, p2(X),
         "-k",markersize='1.5',
         label="Fit through the mean points({})".format(p2)
         )
      plt.legend()
      plt.show()
      fig.savefig(
            "{}Vs{}_r={}_mass={}GeV.png".format(
                  Nomes[y], Nomes[x], r, massa
                  )
            )      
      kr=kr+1      
    
      
      
if minbias:   
   print("minbias")
   for r in r_2:
      b=2
      fig=plt.figure(dpi=130)
      Npontos=np.array([])
      for B in range (2,bmaxplot+2,2): #itera em cada b para calcular Npontos e centralidade
         print("B=",B)
         Npontos=np.append(Npontos, np.array([Nevts*(B/bmaxplot)])) #Essência do minbias
      pontostotal=np.sum(Npontos) 
      kb=0 #"numero" do b atual 
      centr=[0] #primeira centr começa no 0%
      MEDX=[]
      MEDY=[]
      Xr=np.array([]) #todas cord x para esse r
      Yr=np.array([])
                 
      for i in range(
      Nevts+int(kr*(numb*Nevts)),
      int(kr*(numb*Nevts)+(numbplot*Nevts)),
      Nevts): #primeira linha com o b considerado
         #para minbias, b=0 nao faz sentido
         f=int(i+Npontos[kb]-1)  #ultima linha
         print("i=",i)
         print("f=",f)
         medx=np.sum(X[i:f])/Npontos[kb]
         #soma dos valores de x para cada nuvem
         medy=np.sum(Y[i:f])/Npontos[kb]
         #soma dos valores de y para cada nuvem
         MEDX.append(medx)
         MEDY.append(medy)
         centr.append(100*(Npontos[kb]/pontostotal))
         #centralidde do b em questao
         b=b+2
         Xr=np.append(Xr, X[i:f])
         Yr=np.append(Yr, Y[i:f])         
         plt.scatter(
            X[i:f],
            Y[i:f],
            label="b={}, {} - {}%".format(
                   b-2,
                   round(centr[kb],1),
                   round(centr[kb+1],1)
                   ),
             s=5,
             c=colors[kb]
             )
         plt.scatter(
             medx,
             medy,
             c=colors[kb],
             s=100,
             edgecolors='black',
             alpha=1,
             )
         kb=kb+1
      
      plt.title(
            "{} Vs. {} for $r={}$fm and $mass={}$GeV (minbias)".format(
                  data.columns[y], data.columns[x], r, massa
                  )
            )
      plt.xlabel(data.columns[x])
      plt.ylabel(data.columns[y])
      
      z = np.polyfit(Xr,Yr,1) #Fit por todos pontos
      p = np.poly1d(z)
      plt.plot(
         X, p(X),
         "-", color="orangered", markersize='0.5',
         label="Fit through all points({})".format(p),
         alpha=0.3
         )        

      z2 = np.polyfit(MEDX,MEDY,1) #Fit só pelos pontos medios
      p2 = np.poly1d(z2)
      plt.plot(
         X, p2(X), "-k", markersize='1.5',
         label="Fit through the mean points({})".format(p2)
         )
      
      plt.legend()
      plt.show()
      fig.savefig(
            "{}Vs{}_r={}_mass={}GeV_minbias.png".format(
                  Nomes[y], Nomes[x], r, massa
                  )
            )
      kr=kr+1
    

