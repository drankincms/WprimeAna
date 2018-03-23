#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_Feb20_Slim/)
  foreach chan (Mu El)
    eval ./DrawOneHistoSystUnc.o $dir $chan GE0BTags nAK4Jets 1 '${po}\ jets' 1
    eval ./DrawOneHistoSystUnc.o $dir $chan GE0BTags nBTags 1 '${po}\ b-tags' 1
    eval ./DrawOneHistoSystUnc.o $dir $chan GE0BTags nBTags_First2 1 '${po}\ b-tags\ ${lp}leading\ 2\ p_\{T\}\ jets${rp}' 1

    foreach prefix (GE1BTags_Final GE1BTags Ex0BTags Ex1BTags Ex2BTags)
    #foreach prefix (GE1BTags_Final GE1BTags Ex0BTags)
    #foreach prefix (Ex0BTags)
    #foreach prefix (GE0BTags GE1BTags)
    #foreach prefix (Ex0BTags)

       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 50
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50
       #eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2
       if ($chan == Mu ) eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25
       if ($chan == El ) eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix AK4Jet3Pt 1 'p_\{T\}${lp}jet3${rp}' 50
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix nAK4Jets 1 '${po}\ jets' 1
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix nBTags 1 '${po}\ b-tags' 1
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix nBTags_First2 1 '${po}\ b-tags\ ${lp}leading\ 2\ p_\{T\}\ jets${rp}' 1
       eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix AK4Jet1Jet2Pt 1 'p_\{T\}${lp}jet1,jet2${rp}' 50



       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50
       #eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2
       if ($chan == Mu ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25
       if ($chan == El ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25
       if ($chan == Mu ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepEta 0 '${po}eta${lp}${po}mu${rp}' 25
       if ($chan == El ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepEta 0 '${po}eta${lp}e${rp}' 25
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet1Eta 0 '${po}eta${lp}jet1${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet2Eta 0 '${po}eta${lp}jet2${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet3Pt 1 'p_\{T\}${lp}jet3${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix nAK4Jets 1 '${po}\ jets' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix nBTags 1 '${po}\ b-tags' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix nBTags_First2 1 '${po}\ b-tags\ ${lp}leading\ 2\ p_\{T\}\ jets${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet1Jet2Pt 1 'p_\{T\}${lp}jet1,jet2${rp}' 50
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 20
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepJetpTrel 1 'p_\{T\}^\{rel\}${lp}lepton,closest\ jet${rp}' 25

    end

  end
end
echo "Done!"
