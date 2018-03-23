#!/bin/tcsh -f

eval set `sed -n 10p start_jobs.py`
eval set `sed -n 11p start_jobs.py`
eval set `sed -n 12p start_jobs.py | sed 's/suffix/\$&/g' | sed 's/logdir/\$&/g' | sed 's/\+//g'`
eval set `sed -n 13p start_jobs.py | sed 's/outdir/\$&/g' | sed 's/\+//g'`

#echo $logdir
#echo $suffix
#echo $outdir
#echo $eosdir

eval xrdcp -f -r ${eosdir}/ ${logdir}_${suffix}/
