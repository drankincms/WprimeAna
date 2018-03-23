#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_Jan13_Slim/)
  foreach chan (Mu El)
    #foreach prefix (Presel GE0BTags GE1BTags_Final GE1BTags GE0BTags_FailMET GE0BTags_FailMETPass2D GE0BTags_Fail2D)
    foreach prefix (Ex0BTags_Final Ex0BTags)

       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50 1 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 1 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 20 1 ${scale}
       if ($chan == Mu ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 1 ${scale}
       if ($chan == El ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 1 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 1 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50 1 ${scale}
       if ($chan == Mu ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepEta 0 '${po}eta${lp}${po}mu${rp}' 25 1 ${scale}
       if ($chan == El ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepEta 0 '${po}eta${lp}e${rp}' 25 1 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50 1 ${scale}
    end
  end
end
echo "Done!"
