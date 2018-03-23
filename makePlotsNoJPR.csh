#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_Jan31/)
  foreach chan (Mu El)
    foreach prefix (Ex0BTags)

       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 50 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 20 0 ${scale}
       if ($chan == Mu ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale}
       if ($chan == El ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
       eval ./DrawOneHistoThreeSig.o $dir $chan $prefix nAK4Jets 1 '${po}\ jets' 1 0 ${scale}

       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 50 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 50 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 20 0 ${scale}
       if ($chan == Mu ) eval ./DrawOneHistoSigRatio.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale}
       if ($chan == El ) eval ./DrawOneHistoSigRatio.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 50 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 50 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
       eval ./DrawOneHistoSigRatio.o $dir $chan $prefix nAK4Jets 1 '${po}\ jets' 1 0 ${scale}
    end
  end
end
echo "Done!"
