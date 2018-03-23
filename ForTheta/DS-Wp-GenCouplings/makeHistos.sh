#!/bin/bash

for rl in `seq 0 10`;
do
    eval python WprimeGeneralizedCoupling-1and2.py --ar=${rl} &
done

wait
