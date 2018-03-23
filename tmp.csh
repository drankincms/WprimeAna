#!/bin/tcsh -f

set dir = /uscms_data/d3/drankin/13TeV/Samples/28Nov_All/
set suffix = Run2015CD_Slim
set outdir = ${suffix}_Nov30

foreach sample ( WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf )
  eval ./WprimeAnalyzer_Slim.o MC ${sample}_LF ${dir}${sample}.root $suffix
  eval mv aSlimOutFile_${sample}_LF_${suffix}.root $outdir
  eval ./WprimeAnalyzer_Slim.o MC ${sample}_HF ${dir}${sample}.root $suffix
  eval mv aSlimOutFile_${sample}_HF_${suffix}.root $outdir
end
eval hadd -f $outdir/outFile.root $outdir/aSlimOutFile*Slim.root ${outdir}/aSlimOutFile_Data*.root
echo "Done!"
