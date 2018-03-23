#!/bin/tcsh -f

eval set `sed -n 9p AnalyzerSubmit_LHE.py`
eval set `sed -n 10p AnalyzerSubmit_LHE.py`
eval set `sed -n 11p AnalyzerSubmit_LHE.py | sed 's/suffix/\$&/g' | sed 's/logdir/\$&/g' | sed 's/\+//g'`
eval set `sed -n 12p AnalyzerSubmit_LHE.py | sed 's/outdir/\$&/g' | sed 's/\+//g'`

#echo $logdir
#echo $suffix
#echo $outdir
#echo $eosdir

eval hadd -f ${suffix}_${logdir}/outFile.root `xrdfs root://cmseos.fnal.gov ls -u ${outdir}/ | grep ".root" | grep "anLHEOutFile"`
eval xrdcp -f ${suffix}_${logdir}/outFile.root ${eosdir}/outFile.root
