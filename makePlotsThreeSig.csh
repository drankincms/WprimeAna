#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"

foreach dir (Test_Chiral/)
  foreach chan (Mu El All)
    foreach prefix (Presel Ex1BTags_Final Ex2BTags_Final)
      foreach mass (1500 2000 2500)
#       if ($chan == Mu ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 
#       if ($chan == Mu ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepEta 0 '${po}eta${lp}${po}mu${rp}' 40 
#       if ($chan == Mu ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepPhi 0 '${po}phi${lp}${po}mu${rp}' 40 
#       if ($chan == El ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 
#       if ($chan == El ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepEta 0 '${po}eta${lp}e${rp}' 40 
#       if ($chan == El ) eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepPhi 0 '${po}phi${lp}e${rp}' 40 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet1Eta 0 '${po}eta${lp}jet1${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet1Phi 0 '${po}phi${lp}jet1${rp}' 40 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet1CSV 1 'CSV\ ${lp}jet1${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet2Eta 0 '${po}eta${lp}jet2${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet2Phi 0 '${po}phi${lp}jet2${rp}' 40 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet2CSV 1 'CSV\ ${lp}jet2${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet3Pt 1 'p_\{T\}${lp}jet3${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet3Eta 0 '${po}eta${lp}jet3${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet3Phi 0 '${po}phi${lp}jet3${rp}' 40 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet3CSV 1 'CSV\ ${lp}jet3${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix AK4Jet1Jet2Pt 1 'p_\{T\}${lp}jet1+jet2${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix HT 1 'H_\{T\}' 200 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 50 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50 
       eval ./DrawOneHistoSignal.o 1 $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 $mass
       eval ./DrawOneHistoSignal.o 1 $dir $chan $prefix mGenWprime 1 'gen\ M${lp}W${po}prime${rp}' 100 $mass
       eval ./DrawOneHistoSignal.o 1 $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 $mass
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix nBTags 1 '${po}\ b-tags' 1 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix nBTags_First2 0 '${po}\ b-tags\ ${lp}leading\ 2\ p_\{T\}\ jets${rp}' 1 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix nAK4Jets 1 '${po}\ jets' 1 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 20 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix LepJetpTrel 1 'p_\{T\}^\{rel\}${lp}lepton,closest\ jet${rp}' 25 
#       eval ./DrawOneHistoSignal.o 0 $dir $chan $prefix METLepdPhi 1 '${po}Delta${po}phi${lp}MET,lepton${rp}' 50 

    end
  end
end
echo "Done!"
