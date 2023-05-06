#!/bin/bash 

for umb_pot in $(seq -10 1 -25);do
    for clipping in $(seq 0.00001 0.00005 0.001);do
        for umb_R1 in $(seq 0.4 0.01 0.6);do
            echo -n "umb_pot=$umb_pot clipping=$clipping umb_R1=$umb_R1 umb_RMax=0.4 "
            ./scripts/run_get_pitch.sh $umb_pot $clipping $umb_R1 0.4  > /dev/null
            pitch_evaluate pitch_db/train/*f0ref | fgrep TOTAL  
    done
done | sort -t: -k 2n;

exit 0