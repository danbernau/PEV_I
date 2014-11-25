#!/bin/bash
# Automatic testing of various parameters to explore their impact on the unavailability
# Usage: ./test.sh
#decrease lambda, decrease mikro, increase d, increase delta

t=20000000
seed=56234
mlt=100
mrt=1
d=0.9
delta=10

#Testcase 1: Increase delta

bash ./crawl_twoUnitWithInsp_v2.sh $mlt $mrt 0.9 $delta $t $seed 4 5 30 IncDelta

#Testcase 2: Increase mlt

bash ./crawl_twoUnitWithInsp_v2.sh $mlt $mrt 0.9 $delta $t $seed 1 5 100 IncMlt

#Testcase 3: Increase mrt

bash ./crawl_twoUnitWithInsp_v2.sh $mlt $mrt 0.9 $delta $t $seed 2 5 100 IncMrt

#Testcase 4: Increase d

dProxy=80
targetD=100
echo `date` >> ./testResults/IncD.csv
echo 'mlt:' $mlt >> ./testResults/IncD.csv
echo 'mrt:' $mrt >> ./testResults/IncD.csv 
echo 'd:' 0.90 >> ./testResults/IncD.csv
echo 'Delta:' $delta >> ./testResults/IncD.csv
echo 'T:' $t >> ./testResults/IncD.csv
echo 'seed:' $seed >> ./testResults/IncD.csv
echo 'dynamic parameter $' 4 >> ./testResults/IncD.csv
echo 'incr by:' 0.01 >> ./testResults/IncD.csv
echo 'incr rnd:' 20 >> ./testResults/IncD.csv
        while [ $dProxy -le $targetD ]
        do
        echo $dProxy`./twoUnitWithInsp_v2 $mlt $mrt 0.$dProxy $delta $t $seed | sed -n -e 's/^.*= /;/p' | sed '/[0-9]\./s/\./,/g'` >> ./testResults/IncD.csv                
        let dProxy=$dProxy+1
    done        