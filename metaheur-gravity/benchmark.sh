#!/bin/bash

#constants
FILE_EXTENSION=".txt"
MAX_ITERATIONS=5
GRAPH=(graph100 frb30-15)
VERTICES=(100 450)
INSTANCES=(01 03 05)
NUM_ITERATIONS=10000
TIME_UNVRS=(1 10000)
BETA_VALUES=(0.1 0.9)

count_script=0
max_script=$[${#LANGUAGES[@]} * $MAX_ITERATIONS * ${#MATRIX_DIM[@]}]
index=0

for i in ${GRAPH[@]};
do
	for j in "${INSTANCES[@]}";
	do
		for time in "${TIME_UNVRS[@]}";
		do
			for beta in "${BETA_VALUES[@]}";
			do
				for vert in `seq 1 $[$index+1]`;
				do
					mkdir -p $i"/shell/" #guarantees that path exists
					mkdir -p $i"/internal/" #guarantees that path exists
					FILE_SHELL_PREFIX=$i"/shell/"$i"-"$j
					FILE_STD_OUTPUT_PREFIX=$i"/internal/"$i"-"$j
					count_iterations=0
					#runs the program the desired number of times
					echo -e '\n\n'--------------------BEGIN $i-$j--------------------
					for k in `seq 1 $MAX_ITERATIONS`;
					do
						FILE_SHELL=$FILE_SHELL_PREFIX"-"$k$FILE_EXTENSION
						FILE_STD_OUTPUT=$FILE_STD_OUTPUT_PREFIX"-"$k$FILE_EXTENSION
						rm -f $FILE_SHELL #so it doesnt print unnecessary info
						rm -f $FILE_STD_OUTPUT #so it doesnt print unnecessary info
						#choose the matrices randomly since the order for square matrices does not matter
						ARGUMENT1="../"$i"/"$i"-"$j$FILE_EXTENSION
						ARGUMENT2=${VERTICES[$vert]}
						ARGUMENT3=$time

						start=$( date -I'ns' )
						"./gravity" $ARGUMENT1 $ARGUMENT2 $NUM_ITERATIONS $time $beta >> $FILE_STD_OUTPUT
						end=$( date -I'ns' )
						echo "" >> $FILE_STD_OUTPUT
						echo "-------------------------ITERATION #"$k"-------------------------" >> $FILE_SHELL
						echo "Start time: "$start >> $FILE_SHELL
						echo "Finished time: "$end >> $FILE_SHELL
						echo "Duration: "$(($(($(date -d $end +%s%N) - $(date -d $start +%s%N)))/1000)) "microsecond(s)" >> $FILE_SHELL
						echo"" >> $FILE_SHELL

						count_script=$[$count_script + 1]
						count_iterations=$[$count_iterations + 1]
						perc_script=$(bc -l <<< 'scale=2; '$count_script*100/$max_script)
						#perc_iterations=$(bc -l <<< 'scale=1; '$count_iterations*100/$MAX_ITERATIONS)
						echo -e $i $j $[$count_iterations*100/$MAX_ITERATIONS]% '\t'"Script progress: "$count_script/$max_script '('$perc_script%')'
					done
				done
			done
		done
	done

	index=$[$index + 1]
done