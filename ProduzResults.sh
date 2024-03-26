#!/usr/bin/env bash 
# Script que altera o .py e o .c, roda o .c e depois o .py
#echo Qual valor de b?
#read b
#a

echo Quantos eventos?
read Nevts

echo qual eixo x? - [0,9]
read eixox

echo qual eixo y? - [0,9]
read eixoy



cd  ~/Downloads/root/Macro-Fev2022/ResultadosFinaisIC/"Macros e .py"	 	
			sed -i 's/Nevts=/Nevts=delete/g' AnaliseFinalIC.py
			sed -i '/Nevts=delete/a Nevts=novo' AnaliseFinalIC.py
			sed -i '/^Nevts=delete/d' AnaliseFinalIC.py
			sed -i "s/Nevts=novo/Nevts= $Nevts/g" AnaliseFinalIC.py
	
	
for r2_hs in 0.2 0.8 2.4 5 10 15 #fm
				do
				echo 
				sed -i 's/r_2=/r_2=delete/g' AnaliseFinalIC.py
				sed -i '/r_2=delete/a r_2=novo' AnaliseFinalIC.py
				sed -i '/^r_2=delete/d' AnaliseFinalIC.py
				sed -i "s/r_2=novo/r_2= $r2_hs/g" AnaliseFinalIC.py	

				sed -i 's/x=/x=delete/g' AnaliseFinalIC.py
				sed -i '/x=delete/a x=novo' AnaliseFinalIC.py
				sed -i '/^x=delete/d' AnaliseFinalIC.py
				sed -i "s/x=novo/x= $eixox/g" AnaliseFinalIC.py			
				
				sed -i 's/y=/y=delete/g' AnaliseFinalIC.py
				sed -i '/y=delete/a y=novo' AnaliseFinalIC.py
				sed -i '/^y=delete/d' AnaliseFinalIC.py
				sed -i "s/y=novo/y= $eixoy/g" AnaliseFinalIC.py					


		for ((b=0;b<=12;b=b+2))
			do
			declare -i UltSeed=100*$Nevts+900
			for ((seed=1000;seed<=UltSeed;seed=seed+100)) 
				do
				echo "b=",b
				echo "r_hs=",r2_hs					
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
				
				#sed -i 's/double r2_hs=/double r2_hs=delete/g' analiseHotSpots.C
				#sed -i '/double r2_hs=delete/a double r2_hs=novo' analiseHotSpots.C
				#sed -i '/^double r2_hs=delete/d' analiseHotSpots.C
				#sed -i "s/double r2_hs=novo/double r2_hs= $r;/g" analiseHotSpots.C	
				
			#<< 'MULTILINE-COMMENT'	
				root  -q .x analiseHotSpots.C;
				#$r2_hs;

			#MULTILINE-COMMENT	
			done	
		done
	python3 AnaliseFinalIC.py;	
done	
	
