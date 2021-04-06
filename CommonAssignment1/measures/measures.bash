TIME_STAMP=$(date +%s)
NMEASURES=500

ARRAY_RC=(2000 5000 8000 10000)
ARRAY_THS=(0 1 2 4 8)

trap "exit" INT
mkdir measure 2> /dev/null 

for ths in "${ARRAY_THS[@]}"; do
	for size in "${ARRAY_RC[@]}"; do
		OUT_FILE=measure/SIZE-$size-NTH-$ths-$TIME_STAMP.csv
		echo $OUT_FILE
		echo "row,columns,threads,user,elapsed,sys,pCPU" >$OUT_FILE
		for ((i = 0 ; i < $NMEASURES	; i++)); do
			if [[ $ths -ne 0 ]]; then 
				/usr/bin/time -f "$size,$size,$ths,%U,%e,%S,%P" -o $OUT_FILE --append ../build/program $size $size $ths
			else
				/usr/bin/time -f "$size,$size,$ths,%U,%e,%S,%P" -o $OUT_FILE --append ../build/program_seq $size $size $ths
			fi
			printf "\r> %d/%d %3.1d%% " $i $NMEASURES $(expr \( $i \* 100 \) / $NMEASURES)
			printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
		done
		printf "\n"
	done
done

