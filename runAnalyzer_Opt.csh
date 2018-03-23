#!/bin/tcsh -f

set predir = /eos/uscms/store/user/drankin/ID_
set suffix = FinalCuts
foreach idopt ( TT ML )
  set outdir = ${suffix}_${idopt}
  set dir = ${predir}${idopt}/
  eval mkdir ${outdir}
  set chk = `ls ${outdir}/*.root | wc -l`
  if (${chk} != 0) then
    eval rm ${outdir}/*.root
  endif
  foreach sample ( TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf Wprime2000Right Wprime2500Right Wprime3000Right )
    eval ./WprimeAnalyzer.o MC $sample ${dir}${sample}.root $suffix
    eval mv anOutFile_${sample}_$suffix.root $outdir
  end
  eval hadd -f $outdir/outFile.root $outdir/anOutFile*.root
end
echo "Done!"
