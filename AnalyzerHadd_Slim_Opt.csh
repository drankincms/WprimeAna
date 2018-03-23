#!/bin/tcsh -f

eval set `sed -n 9p AnalyzerSubmit_Slim_Opt.py`
eval set `sed -n 10p AnalyzerSubmit_Slim_Opt.py`
eval set `sed -n 11p AnalyzerSubmit_Slim_Opt.py | sed 's/suffix/\$&/g' | sed 's/logdir/\$&/g' | sed 's/\+//g'`
eval set `sed -n 12p AnalyzerSubmit_Slim_Opt.py | sed 's/outdir/\$&/g' | sed 's/\+//g'`
echo $logdir
#echo $suffix
#echo $outdir
#echo $eosdir

foreach opt ( `sed -n 19p AnalyzerSubmit_Slim_Opt.py | sed 's/opts = \[\(.*\)\]/\1/g' | sed "s/'//g" | sed 's/,/ /g'` )
  #eval hadd -f ${suffix}_${logdir}/outFile${opt}.root `xrdfs root://cmseos.fnal.gov ls -u ${outdir}/ | grep ".root" | grep "aSlimOutFile${opt}"`
  #eval xrdcp -f ${suffix}_${logdir}/outFile${opt}.root ${eosdir}/outFile${opt}.root
  eval xrdcp -f ${eosdir}/outFile${opt}.root ${suffix}_${logdir}/outFile${opt}.root
end
