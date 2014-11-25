#!/bin/bash
# Automatic call of the two system simulator with varying params
# Usage: ./twoUnitWithInsp_v2  mlt  mrt  d   Delta  T  seed
# SED: http://unix.stackexchange.com/questions/24140/
# return-only-the-portion-of-a-line-after-a-matching-pattern
# $1 = mlt
# $2 = mrt
# $3 = d
# $4 = Delta
# $5 = T
# $6 = seed
# $7 = index of dynamic parameter
# $8 = increase parameter by
# $9 = number of increases
# $10 = output file name

i="0"
echo `date` >> ./testResults/${10}.csv
echo 'mlt:' $1 >> ./testResults/${10}.csv
echo 'mrt:' $2 >> ./testResults/${10}.csv
echo 'd:' $3 >> ./testResults/${10}.csv
echo 'Delta:' $4 >> ./testResults/${10}.csv
echo 'T:' $5 >> ./testResults/${10}.csv
echo 'seed:' $6 >> ./testResults/${10}.csv
echo 'dynamic parameter $' $7 >> ./testResults/${10}.csv
echo 'incr by:' $8 >> ./testResults/${10}.csv
echo 'incr rnd:' $9 >> ./testResults/${10}.csv

initial=${!7}
plus=$(($8*$9))
target=$(( $initial + $plus ))

mlt=$1
mrt=$2
d=$3
delta=$4
t=$5
seed=$6

case $7 in
    1 )
        var="mlt";;
    2 )
        var="mrt";;
    3 )
       var="d";;
    4 )
        var="delta";;
    5 )
        var="t";;
esac

check=${!var}
while [ $check -lt $target ]
	do
        echo $check`./twoUnitWithInsp_v2 $mlt $mrt $d $delta $t $seed | sed -n -e 's/^.*= /;/p' | sed '/[0-9]\./s/\./,/g'` >> ./testResults/${10}.csv
        check=$(( $check + $8 ))
		let $var=$check
	done