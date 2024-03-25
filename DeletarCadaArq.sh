#!/usr/bin/env bash 

for r2 in 0.2 0.8 2.4 5 10 15
	do
	for b in 0 2 4 6 8 10 12
		do
		cd ~/Downloads/root/Macro\-Fev2022/ResultadosFinaisIC/DiferentesSeeds/r2_hs=$r2/b=$b
			for arquivo in *
			do
			rm $arquivo
		done
	done
done	
