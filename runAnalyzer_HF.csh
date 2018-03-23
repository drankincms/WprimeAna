#!/bin/tcsh -f

set dir = /eos/uscms/store/user/drankin/LJMet/28Nov_All/
set suffix = Run2015CD_HF
set outdir = ${suffix}_Nov30_TPR

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  eval rm ${outdir}/*.root
endif
foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW SingleLep WW WZ ZZ)
  
  if ($sample == SingleElectron || $sample == SingleMuon || $sample == SingleLep) then
    eval ./WprimeAnalyzer_HF.o Data Data ${dir}${sample}.root $suffix
    eval echo "Moving :"
    eval ls aHFOutFile_Data_${suffix}_*.root
    eval mv aHFOutFile_Data_${suffix}_*.root $outdir
  else 
    foreach system (NOM _BTAGUP _BTAGDOWN)
      if ($system == NOM) then
        eval ./WprimeAnalyzer_HF.o MC $sample ${dir}${sample}.root $suffix
        eval mv aHFOutFile_${sample}_${suffix}.root $outdir
      else
        eval ./WprimeAnalyzer_HF.o MC $sample ${dir}${sample}${system}.root ${suffix}${system}
        eval mv aHFOutFile_${sample}_${suffix}${system}.root $outdir
      endif
    end
  endif
end
eval hadd -f $outdir/outFile.root $outdir/aHFOutFile*HF.root ${outdir}/aHFOutFile_Data*.root
eval hadd -f $outdir/outFile_BTAGUP.root $outdir/aHFOutFile*HF_BTAGUP.root ${outdir}/aHFOutFile_Data*.root
eval hadd -f $outdir/outFile_BTAGDOWN.root $outdir/aHFOutFile*HF_BTAGDOWN.root ${outdir}/aHFOutFile_Data*.root
echo "Done!"
