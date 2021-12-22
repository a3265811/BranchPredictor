#! /bin/bash

make

filelist=$(ls ./testbench/)

read -p "input mode(g or l): " mode

if [ ${mode} = "g" ]
then
	for m in $(seq 1 9)
	do
		p=$((10-${m}))
		echo "m = " ${m} ",p = " ${p}
		echo "m = " ${m} ",p = " ${p} >> result_g.csv
		echo "testbench,#mispredicted,#instruction,miss rate" >> result_g.csv
		for file in ${filelist}
		do
			./M11007428.o g ./testbench/${file} ${m} ${p} >> result_g.csv
			echo ${file}" finished"
		done
	done
elif [ ${mode} = "l" ]
then
	for k in $(seq 3 5)
	do
		temp=$(echo '2^'${k} | bc)
		m=$((64/${temp}))
		p=$((10-${m}))
		echo "k = " ${k} ",m = " ${m} ",p = " ${p}
		echo "k = " ${k} ",m = " ${m} ",p = " ${p} >> result_l.csv
		echo "testbench,#mispredicted,#instruction,miss rate" >> result_l.csv
		for file in ${filelist}
		do
			./M11007428.o l ./testbench/${file} ${k} ${m} ${p} >> result_l.csv
			echo ${file}" finished"
		done
	done
fi
