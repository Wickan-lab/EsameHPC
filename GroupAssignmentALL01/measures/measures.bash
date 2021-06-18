#!/bin/bash
# 
# Course: High Performance Computing 2020/2021
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Capitani	Giuseppe	0622701085	g.capitani@studenti.unisa.it               
# Falanga	Armando		0622701140  a.falanga13@studenti.unisa.it 
# Terrone	Luigi		0622701071  l.terrone2@studenti.unisa.it
#
# Copyright (C) 2021 - All Rights Reserved 
#
# This file is part of CommonAssignmentMPI01.
#
# CommonAssignmentMPI01 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignmentMPI01 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CommonAssignmentMPI01.  If not, see <http://www.gnu.org/licenses/>.
#

TIME_STAMP=$(date +%s)
NMEASURES=25

ARRAY_RC=(16384 65536 262144)
ARRAY_THS=(0 1 2 4 8 16)
TEST_POINT=(3 3)
CLUSTER_NUM=(5 5 5)
ARRAY_K_VALUES=(128 256 512)
TIMEFORMAT='%3U;%3E;%3S;%P'

trap "exit" INT

# Get script path
SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

for ((j = 0; j < "${#ARRAY_RC[@]}"; j++)); do
	#$1/program_seq $size $size $size $size $ver
	for ths in "${ARRAY_THS[@]}"; do
		ths_str=$(printf "%02d" $ths)
		
		OUT_FILE=$SCRIPTPATH/measure/SIZE-${ARRAY_RC[$j]}-K${ARRAY_K_VALUES[$j]}/SIZE-${ARRAY_RC[$j]}-NTH-$ths_str-K${ARRAY_K_VALUES[$j]}.csv

		mkdir -p $(dirname $OUT_FILE) 2> /dev/null
		
		echo $(basename $OUT_FILE)
		#if [[ $ver -gt 1 && $ths -eq 0 ]]; then
		#	OLD_OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-V1/SIZE-$size-NP-$ths_str-V1.csv
		#	ln -srf -T $OLD_OUT_FILE $OUT_FILE
		#	echo Created symbolic link to $(basename $OLD_OUT_FILE)
		#	continue
		#fi
		echo "NPoints,K,NClusters,processes,generate,classify,total,user,elapsed,sys,pCPU" >$OUT_FILE
		
		for ((i = 0 ; i < $NMEASURES; i++)); do
			if [[ $ths -eq 0 ]]; then
				(time $1/program_seq ${ARRAY_RC[$j]} ${ARRAY_K_VALUES[$j]} ${CLUSTER_NUM[$j]} $ths)2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
			else
				#mpirun.mpich -np $ths $1/program_O3_V$ver $size $size $size $size >> $OUT_FILE
				(time $1/program_omp_bitonic ${ARRAY_RC[$j]} ${ARRAY_K_VALUES[$j]} ${CLUSTER_NUM[$j]} $ths)2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
			fi
		done
		printf "\n"
	done
done


