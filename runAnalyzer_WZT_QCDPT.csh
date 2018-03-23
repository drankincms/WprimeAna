#!/bin/tcsh -f

set dir = /uscms_data/d3/drankin/13TeV/Samples/15Sep_All/
set suffix = WQCD
set outdir = ${suffix}_15Sep

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  eval rm ${outdir}/*.root
endif
foreach sample ( ZJets TTbar WJets SingleLep QCD_Pt_5to10 QCD_Pt_10to15 QCD_Pt_15to30 QCD_Pt_30to50 QCD_Pt_50to80 QCD_Pt_80to120 QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400 QCD_Pt_1400to1800 QCD_Pt_1800to2400 QCD_Pt_2400to3200 QCD_Pt_3200toInf)
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
