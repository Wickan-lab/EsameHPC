#!/bin/bash
# 
# Copyright (C) 2021 - All Rights Reserved 
#
# This file is part of EsameHPC.
#
# EsameHPC is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EsameHPC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with EsameHPC.  If not, see <http://www.gnu.org/licenses/>.
#

TIME_STAMP=$(date +%s)
NMEASURES=200

ARRAY_RC=(5000 8000 10000 20000)
ARRAY_THS=(1 2 4 8 16 32)
TIMEFORMAT='%3U;%3E;%3S;%P'
ARRAY_OPT=(1 2 3)

trap "exit" INT

for size in "${ARRAY_RC[@]}"; do
	OUT_SEQ=measure/SIZE-$size/SIZE-$size-NTH-00-O0-$TIME_STAMP.csv
	mkdir -p $(dirname $OUT_SEQ) 2> /dev/null
	
	echo $(basename $OUT_SEQ)
	echo "row,columns,threads,init,dotprod,user,elapsed,sys,pCPU" >$OUT_SEQ
	for ((i = 0 ; i < $NMEASURES	; i++)); do
		(time ../../build/CommonAssignment1/program_seq_O0 $size $size 0 )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_SEQ
			printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
	done
	printf "\n"

	for ths in "${ARRAY_THS[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
			ths_str=$(printf "%02d" $ths)
			OUT_FILE=measure/SIZE-$size-O$opt/SIZE-$size-NTH-$ths_str-O$opt-$TIME_STAMP.csv
			mkdir -p $(dirname $OUT_FILE) 2> /dev/null
			ln -srf $OUT_SEQ $(dirname $OUT_FILE)/$(basename $OUT_SEQ)
			echo $(basename $OUT_FILE)
			echo "row,columns,threads,init,dotprod,user,elapsed,sys,pCPU" >$OUT_FILE
			for ((i = 0 ; i < $NMEASURES	; i++)); do
				(time ../../build/CommonAssignment1/program_O$opt $size $size $ths )2>&1 | sed -e ':a' -e 'N' -e '$!ba' -e 's/\n/;/g' -e 's/,/./g' -e 's/;/,/g' >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
			done

			printf "\n"
		done
	done
done

