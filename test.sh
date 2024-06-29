#!/bin/bash

sizes=(1000 100000 1000000)
ks=(10 100 1000 100000)
isOrdered=(0 1)
isLinear=(0 1)
testTime=10

for size in ${sizes[@]}; do
    for k in ${ks[@]}; do
        if [ $size -eq 1000 ] && [ $k -eq 100000 ]; then
            continue
        fi
        for ordered in ${isOrdered[@]}; do
            for linear in ${isLinear[@]}; do
                echo "Running test with size=$size, k=$k, isOrdered=$ordered, isLinear=$linear, testTime=$testTime"
                ./test $size $k $ordered $testTime $linear
            done
        done
    done
done