#!/bin/tcsh -f

set dir = /store/user/drankin/LJMet/Jun10_All/
set suffix = Run2016B
set outdir = ${suffix}_Jun13

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  #eval rm ${outdir}/*.root
endif
#foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600to800 WJets_HT800to1200 WJets_HT1200to2500 WJets_HT2500toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right SingleLep WW WZ ZZ QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400)
foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600to800 WJets_HT800to1200 WJets_HT1200to2500 WJets_HT2500toInf Wprime1500Right Wprime2000Right Wprime2500Right SingleLep QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400)
  eval xrdcp -f root://cmseos.fnal.gov/${dir}${sample}.root ${outdir}/tmp.root
  if ($sample == SingleLep) then
    eval ./WprimeAnalyzer.o Data Data ${outdir}/tmp.root $suffix
    eval echo "Moving :"
    eval ls anOutFile_Data_${suffix}_*.root
    eval mv anOutFile_Data_${suffix}_*.root $outdir
  else 
    if ( `echo $sample | grep -c "WJets"` ) then
      eval ./WprimeAnalyzer.o MC ${sample}_LF ${outdir}/tmp.root $suffix
      eval mv anOutFile_${sample}_LF_$suffix.root $outdir
      eval ./WprimeAnalyzer.o MC ${sample}_HF ${outdir}/tmp.root $suffix
      eval mv anOutFile_${sample}_HF_$suffix.root $outdir
    else
      eval ./WprimeAnalyzer.o MC $sample ${outdir}/tmp.root $suffix
      eval mv anOutFile_${sample}_$suffix.root $outdir
    endif
  endif
end
eval rm ${outdir}/tmp.root
eval hadd -f $outdir/outFile.root $outdir/anOutFile*.root
echo "Done!"
