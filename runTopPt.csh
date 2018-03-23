#!/bin/tcsh -f

set dir = /eos/uscms/store/user/drankin/LJMet/28Nov_All/
set suffix = Run2015CD
set outdir = ${suffix}_TopPt_Dec10App

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  eval rm ${outdir}/*.root
endif
#foreach sample ( ZJets TTbar WJets WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right Wprime3000Right SingleLep WW WZ ZZ QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400)
foreach sample ( ZJets TTbar WJets WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right Wprime3000Right SingleLep WW WZ ZZ )
#foreach sample ( TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf Wprime2000Right Wprime2500Right Wprime3000Right )
#foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW SingleLep WW WZ ZZ QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400)
  if ($sample == SingleElectron || $sample == SingleMuon || $sample == SingleLep) then
    eval ./WprimeTopPt.o Data Data ${dir}${sample}.root $suffix
    eval echo "Moving :"
    eval ls topPtOutFile_Data_${suffix}_*.root
    eval mv topPtOutFile_Data_${suffix}_*.root $outdir
  else 
    eval ./WprimeTopPt.o MC $sample ${dir}${sample}.root $suffix
    eval mv topPtOutFile_${sample}_$suffix.root $outdir
  endif
end
eval hadd -f $outdir/outFile.root $outdir/topPtOutFile*.root
echo "Done!"
