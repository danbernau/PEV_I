#!/bin/bash
# Automatic testing of various parameters to explore their impact on the unavailability
# Usage: ./test.sh


t=10000000
seed=56234
mlt=100
targetMlt=1100

d=0.9
targetD=1

echo `date` >> ./testResults/allComps.csv
echo 'T:' $t  >> ./testResults/allComps.csv
echo 'seed:' $seed >> ./testResults/allComps.csv

while [ $mlt -le $targetMlt ]
do
    mrt=1
    targetMrt=1000
    while [ $mrt -le $targetMrt ]
    do
        dProxy=90
        targetDProxy=100
        while [ $dProxy -le $targetDProxy ]
        do
            delta=10
            targetDelta=100
            while [ $delta -le $targetDelta ]
                do
                    echo $mlt';'$mrt';0,'$dProxy';'$delta';'`./twoUnitWithInsp_v2 $mlt $mrt 0.$dProxy $delta $t $seed | sed -n -e 's/^.*= //p' | sed '/[0-9]\./s/\./,/g'` >> ./testResults/allComps.csv
                let delta=$delta+10
                done
        let dProxy=$dProxy+1        
        done
    let mrt=$mrt+50
    done
let mlt=$mlt+50
done