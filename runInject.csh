#!/bin/tcsh -f

set dir = /eos/uscms/store/user/drankin/LJMet/28Nov_All/
set suffix = Run2015CD
set outdir = ${suffix}_Feb03_Reco
#set sample = ${1}

#foreach sample (Wprime1000Right Wprime1100Right Wprime1200Right Wprime1300Right Wprime1400Right Wprime1700Right Wprime1800Right Wprime1900Right Wprime2100Right Wprime2200Right Wprime2300Right Wprime2400Right Wprime2600Right Wprime2700Right Wprime2800Right Wprime2900Right)
#foreach sample (Wprime1000Right Wprime1100Right Wprime1200Right Wprime1300Right Wprime1400Right Wprime1500Right Wprime1600Right Wprime1700Right Wprime1800Right Wprime1900Right Wprime2000Right Wprime2100Right Wprime2200Right Wprime2300Right Wprime2400Right Wprime2500Right Wprime2600Right Wprime2700Right Wprime2800Right Wprime2900Right Wprime3000Right)
foreach sample (QCD_Pt_80to120 QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400)
#foreach sample (TTbar)
  eval echo ${sample}
  if ($sample == SingleElectron || $sample == SingleMuon || $sample == SingleLep) then
    eval ./WprimeAnalyzer.o Data Data ${dir}${sample}.root $suffix
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
