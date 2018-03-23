#!/bin/tcsh -f

eval set `sed -n 9p AnalyzerSubmit_TopPt.py`
eval set `sed -n 10p AnalyzerSubmit_TopPt.py`
#eval set `sed -n 11p AnalyzerSubmit_TopPt.py | sed 's/suffix/\$&/g' | sed 's/logdir/\$&/g' | sed 's/\+//g'`
#eval set `sed -n 12p AnalyzerSubmit_TopPt.py | sed 's/outdir/\$&/g' | sed 's/\+//g'`

#echo $logdir
#echo $sufbase
#echo $outdir
#echo $eosdir

foreach opt ( NOM _TPR )
  if ($opt == NOM) then
    set outdir = /store/user/drankin/WprimeToTB/${sufbase}_${logdir}
    set eosdir = root://cmseos.fnal.gov/${outdir}
    eval hadd -f ${sufbase}_${logdir}/outFile.root `xrdfs root://cmseos.fnal.gov ls -u ${outdir}/ | grep ".root" | grep "topPtOutFile"`
    eval xrdcp -f ${sufbase}_${logdir}/outFile.root ${eosdir}/outFile.root
  else
    set outdir = /eos/uscms/store/user/drankin/WprimeToTB/${sufbase}${opt}_${logdir}
    set eosdir = root://cmseos.fnal.gov/${outdir}
    eval hadd -f ${sufbase}${opt}_${logdir}/outFile.root `xrdfs root://cmseos.fnal.gov ls -u ${outdir}/ | grep ".root" | grep "topPtOutFile"`
    eval xrdcp -f ${sufbase}${opt}_${logdir}/outFile.root ${eosdir}/outFile.root
  endif
end

