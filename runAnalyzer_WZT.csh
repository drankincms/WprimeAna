#!/bin/tcsh -f

set dir = /uscms_data/d3/drankin/13TeV/Samples/15Sep_All/
set suffix = WQCD
set outdir = ${suffix}_15Sep

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  eval rm ${outdir}/*.root
endif
foreach sample ( ZJets TTbar WJets SingleLep QCD_HT200to300 QCD_HT300to500 QCD_HT500to700 QCD_HT700to1000 QCD_HT1000to1500)
  if ($sample == SingleElectron || $sample == SingleMuon || $sample == SingleLep) then
    eval ./WprimeAnalyzer.o Data Data ${dir}${sample}_2015B.root $suffix
    eval echo "Moving :"
    eval ls anOutFile_Data_${suffix}_*.root
    eval mv anOutFile_Data_${suffix}_*.root $outdir
  else 
    eval ./WprimeAnalyzer.o MC $sample ${dir}${sample}.root $suffix
    eval mv anOutFile_${sample}_$suffix.root $outdir
  endif
end
eval hadd -f $outdir/outFile.root $outdir/anOutFile*.root
echo "Done!"
