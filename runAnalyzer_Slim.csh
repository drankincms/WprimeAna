#!/bin/tcsh -f

set dir = /eos/uscms/store/user/drankin/LJMet/17Dec_HEEP_All/
#set dir = /uscms_data/d3/drankin/13TeV/Samples/14Dec_All/
set suffix = Run2015CD_Slim
set outdir = ${suffix}_Dec15_HEEP

eval mkdir ${outdir}
set chk = `ls ${outdir}/*.root | wc -l`
if (${chk} != 0) then
  #eval rm ${outdir}/*.root
endif
#foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right Wprime3000Right SingleLep WW WZ ZZ QCD_Pt_120to170 QCD_Pt_170to300 QCD_Pt_300to470 QCD_Pt_470to600 QCD_Pt_600to800 QCD_Pt_800to1000 QCD_Pt_1000to1400 Wprime1000Right Wprime1100Right Wprime1200Right Wprime1300Right Wprime1400Right Wprime1500Right Wprime1600Right Wprime1700Right Wprime1800Right Wprime1900Right Wprime2000Right Wprime2100Right Wprime2200Right Wprime2300Right Wprime2400Right Wprime2500Right Wprime2600Right Wprime2700Right Wprime2800Right Wprime2900Right Wprime3000Right)
foreach sample ( ZJets TTbar WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf T_s T_t T_tW Tbar_tW Wprime1600Right Wprime2000Right Wprime2500Right Wprime3000Right SingleLep WW WZ ZZ Wprime800Right Wprime900Right Wprime1000Right Wprime1100Right Wprime1200Right Wprime1300Right Wprime1400Right Wprime1500Right Wprime1600Right Wprime1700Right Wprime1800Right Wprime1900Right Wprime2000Right Wprime2100Right Wprime2200Right Wprime2300Right Wprime2400Right Wprime2500Right Wprime2600Right Wprime2700Right Wprime2800Right Wprime2900Right Wprime3000Right)
#foreach sample ( WJets_HT100to200 WJets_HT200to400 WJets_HT400to600 WJets_HT600toInf SingleLep )
  
  if ($sample == SingleElectron || $sample == SingleMuon || $sample == SingleLep) then
    eval ./WprimeAnalyzer_Slim.o Data Data ${dir}${sample}.root $suffix
    eval echo "Moving :"
    eval ls aSlimOutFile_Data_${suffix}_*.root
    eval mv aSlimOutFile_Data_${suffix}_*.root $outdir
  else 
    foreach system (NOM)
    #foreach system (NOM _JESUP _JESDOWN _JERUP _JERDOWN _BTAGUP _BTAGDOWN)
      if ($system == NOM) then
        eval ./WprimeAnalyzer_Slim.o MC $sample ${dir}${sample}.root $suffix
        eval mv aSlimOutFile_${sample}_${suffix}.root $outdir
      else
        eval ./WprimeAnalyzer_Slim.o MC $sample ${dir}${sample}${system}.root ${suffix}${system}
        eval mv aSlimOutFile_${sample}_${suffix}${system}.root $outdir
      endif
    end
  endif
end
eval hadd -f $outdir/outFile.root $outdir/aSlimOutFile*Slim.root ${outdir}/aSlimOutFile_Data*.root
eval hadd -f $outdir/outFile_JESUP.root $outdir/aSlimOutFile*Slim_JESUP.root
eval hadd -f $outdir/outFile_JESDOWN.root $outdir/aSlimOutFile*Slim_JESDOWN.root
eval hadd -f $outdir/outFile_JERUP.root $outdir/aSlimOutFile*Slim_JERUP.root
eval hadd -f $outdir/outFile_JERDOWN.root $outdir/aSlimOutFile*Slim_JERDOWN.root
eval hadd -f $outdir/outFile_BTAGUP.root $outdir/aSlimOutFile*Slim_BTAGUP.root
eval hadd -f $outdir/outFile_BTAGDOWN.root $outdir/aSlimOutFile*Slim_BTAGDOWN.root
echo "Done!"
