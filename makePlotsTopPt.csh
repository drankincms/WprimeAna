#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_TopPt_TPR_Feb20/ Run2016_TopPt_Feb20/)
#foreach dir (Run2016_TopPt_TPR_Feb20/)
  foreach chan (El Mu All)
  #foreach chan (All)
    #foreach prefix (GE2BTags_TopPt GE2BTags_Lep150_TopPt)
    foreach prefix (GE2BTags_TopPt)
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}lep${rp}' 20 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix LepEta 0 '${po}eta${lp}lep${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix LepPhi 0 '${po}phi${lp}lep${rp}' 20 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 40 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet1Eta 0 '${po}eta${lp}jet1${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet1Phi 0 '${po}phi${lp}jet1${rp}' 20 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet1CSV 1 'CSV\ ${lp}jet1${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet2Eta 0 '${po}eta${lp}jet2${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet2Phi 0 '${po}phi${lp}jet2${rp}' 20 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet2CSV 1 'CSV\ ${lp}jet2${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet3Pt 1 'p_\{T\}${lp}jet3${rp}' 40 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet3Eta 0 '${po}eta${lp}jet3${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet3Phi 0 '${po}phi${lp}jet3${rp}' 20 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix AK4Jet3CSV 1 'CSV\ ${lp}jet3${rp}' 30 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix HT 1 'H_\{T\}' 80 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 40 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 40 0 ${scale}
       eval ./DrawOneHistoRatio.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix nBTags 1 '${po}\ b-tags' 1 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix nBTags_First2 1 '${po}\ b-tags\ ${lp}leading\ 2\ p_\{T\}\ jets${rp}' 1 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 8 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix LepJetpTrel 1 'p_\{T\}^\{rel\}${lp}lepton,closest\ jet${rp}' 10 0 ${scale}
       #eval ./DrawOneHistoRatio.o $dir $chan $prefix nPV 1 '${po}\ primary\ vertices' 1 0 ${scale}

       #eval ./DrawOneHistoRatio.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 40 0 ${scale} 1
    end
  end
end
echo "Done!"
