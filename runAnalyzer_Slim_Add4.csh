#!/bin/tcsh -f

set dir = /eos/uscms/store/user/drankin/LJMet/28Nov_All/
set suffix = Run2015CD_Slim
set outdir = ${suffix}_Dec10App
#set outdir = ${suffix}_Nov30_DPhi

set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} == 0) then
  echo "Run this after main runAnalyzer_Slim.csh."
  exit 1
endif
#foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right Wprime3000Right WW WZ ZZ Wprime800Right Wprime900Right Wprime1000Right Wprime1100Right Wprime1200Right Wprime1300Right Wprime1400Right Wprime1500Right Wprime1600Right Wprime1700Right Wprime1800Right Wprime1900Right Wprime2000Right Wprime2100Right Wprime2200Right Wprime2300Right Wprime2400Right Wprime2500Right Wprime2600Right Wprime2700Right Wprime2800Right Wprime2900Right Wprime3000Right)
foreach sample ( WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf )
  foreach system ( _MURMUFDOWN )
    eval ./WprimeAnalyzer_Slim_mum.o MC $sample ${dir}${sample}.root ${suffix}${system}
    eval mv aSlimOutFile_${sample}_${suffix}${system}.root $outdir
  end
end
foreach system ( _MURMUFDOWN )
  eval hadd -f $outdir/outFile${system}.root $outdir/aSlimOutFile*Slim${system}.root
end
echo "Done!"
