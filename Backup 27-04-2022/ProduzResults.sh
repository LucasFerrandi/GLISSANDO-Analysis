#!/usr/bin/env bash 

#echo Qual valor de b?
#read b

#Esse programa faz análise das colisões Pb-Pb @5520GeV (14TeV, 2760GeV) geradas pelo GLISSANDO3 para b=(0, 2, 4, 6, 8, 10, 12)fm e para 250 diferentes SEEDs, salvas em arquivos .points. A análise é feita por meio do analiseHotSpots.C para diferentes raios de influência r2_hs, que salva as grandezas em arquivos ResultHS-Total.txt. Depois, scatter plots são gerados pelo AnaliseFinalIC.py, funções são ajustadas e seus coeficientes angulares e lineares são salvos em arquivos Coefs___...___.txt.
#Comentario escrito em 01-04-2022


echo Quantos eventos?
read Nevts

echo -e "

Qual Massa(GeV)?

D⁰: 1.865
D⁺, D⁻: 1.870
Pi⁰: 0.135
Pi⁺, Pi⁻: 0.140
u: 0.00216
d: 0.00467
s: 0.093
c: 1.27
b: 4.18
t:172.76
"
read MassaNova

echo -e "

qual eixo x (relevante só para o .py)?

epsFixFireball  #0
epsPartFireball #1
epsFixHSMean    #2
σ_epsFixHSMean  #3
epsPartHSMean   #4
σ_epsPartHSMean #5
EnergyHSMean    #6
σ_EnergyHSMean  #7
Ψ_R             #8
σ_Ψ_R           #9"
read eixox

echo -e "

qual eixo y (relevante só para o .py)?

epsFixFireball  #0
epsPartFireball #1
epsFixHSMean    #2
σ_epsFixHSMean  #3
epsPartHSMean   #4
σ_epsPartHSMean #5
EnergyHSMean    #6
σ_EnergyHSMean  #7
Ψ_R             #8
σ_Ψ_R           #9"
read eixoy


numr2=0 #conta quantos r2

cd  ~/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/"Macros e .py"
	 	
			sed -i 's/Double_t Massa=/Double_t Massa=delete/g' analiseHotSpots.C
			sed -i '/Double_t Massa=delete/a Double_t Massa=novo' analiseHotSpots.C
			sed -i '/^Double_t Massa=delete/d' analiseHotSpots.C
			sed -i "s/Double_t Massa=novo/Double_t Massa= $MassaNova;/g" analiseHotSpots.C		
	 	
sed -i 's/Nevts=/Nevts=delete/g' AnaliseFinalIC.py
sed -i '/Nevts=delete/a Nevts=novo' AnaliseFinalIC.py
sed -i '/^Nevts=delete/d' AnaliseFinalIC.py
sed -i "s/Nevts=novo/Nevts=$Nevts/g" AnaliseFinalIC.py
	
for r2_hs in 0.2 0.8 2.4 5 10 15
	do
	let numr2++
	done
	
bmax=0

for r2_hs in 0.2 0.8 2.4 5 10 15
	do	
	numb=0 #conta quantos b
	for ((b=0;b<=12;b=b+2))
		do
		let numb++
		let bmax=$b
		declare -i UltSeed=100*$Nevts+900		
		for ((seed=1000;seed<=UltSeed;seed=seed+100)) 
			do
			echo " "
			echo "########### NOVA SIMULAÇÃO ####################"
			echo "###############################################"
			echo "r_hs="$r2_hs
			echo "b="$b
			echo "SEED="$seed

									
			sed -i 's/Int_t b=/Int_t b=delete/g' analiseHotSpots.C
			sed -i '/Int_t b=delete/a Int_t b=novo' analiseHotSpots.C
			sed -i '/^Int_t b=delete/d' analiseHotSpots.C
			sed -i "s/Int_t b=novo/Int_t b= $b;/g" analiseHotSpots.C
						
			sed -i 's/Int_t SEED=/Int_t SEED=delete/g' analiseHotSpots.C
			sed -i '/Int_t SEED=delete/a Int_t SEED=novo' analiseHotSpots.C
			sed -i '/^Int_t SEED=delete/d' analiseHotSpots.C
			sed -i "s/Int_t SEED=novo/Int_t SEED= $seed;/g" analiseHotSpots.C
				
			sed -i 's/double r2_hs=/double r2_hs=delete/g' analiseHotSpots.C
			sed -i '/double r2_hs=delete/a double r2_hs=novo' analiseHotSpots.C
			sed -i '/^double r2_hs=delete/d' analiseHotSpots.C
			sed -i "s/double r2_hs=novo/double r2_hs= $r2_hs;/g" analiseHotSpots.C			
				
				
			#<< Comentario	
				root -l -q analiseHotSpots.C;

			#Comentario	
			done #SEED
		done #b
	echo "bmax="$bmax	
	echo "numb="$numb
	echo "numr2="$numr2
		
	sed -i 's/bmax=/bmax=delete/g' AnaliseFinalIC.py
	sed -i '/bmax=delete/a bmax=novo' AnaliseFinalIC.py
	sed -i '/^bmax=delete/d' AnaliseFinalIC.py
	sed -i "s/bmax=novo/bmax=$bmax/g" AnaliseFinalIC.py	
		
	sed -i 's/numb=/numb=delete/g' AnaliseFinalIC.py
	sed -i '/numb=delete/a numb=novo' AnaliseFinalIC.py
	sed -i '/^numb=delete/d' AnaliseFinalIC.py
	sed -i "s/numb=novo/numb=$numb/g" AnaliseFinalIC.py	
	
	sed -i 's/numr2=/numr2=delete/g' AnaliseFinalIC.py
	sed -i '/numr2=delete/a numr2=novo' AnaliseFinalIC.py
	sed -i '/^numr2=delete/d' AnaliseFinalIC.py
	sed -i "s/numr2=novo/numr2=$numr2/g" AnaliseFinalIC.py
	
	#sed -i 's/r_2=/r_2=delete/g' AnaliseFinalIC.py
	#sed -i '/r_2=delete/a r_2=novo' AnaliseFinalIC.py
	#sed -i '/^r_2=delete/d' AnaliseFinalIC.py
	#sed -i "s/r_2=novo/r_2=$r2_hs/g" AnaliseFinalIC.py	

	sed -i 's/x=/x=delete/g' AnaliseFinalIC.py
	sed -i '/x=delete/a x=novo' AnaliseFinalIC.py
	sed -i '/^x=delete/d' AnaliseFinalIC.py
	sed -i "s/x=novo/x=$eixox/g" AnaliseFinalIC.py	
				
	sed -i 's/y=/y=delete/g' AnaliseFinalIC.py
	sed -i '/y=delete/a y=novo' AnaliseFinalIC.py
	sed -i '/^y=delete/d' AnaliseFinalIC.py
	sed -i "s/y=novo/y=$eixoy/g" AnaliseFinalIC.py	
	
	#python3 AnaliseFinalIC.py;	
	done #r2
	
