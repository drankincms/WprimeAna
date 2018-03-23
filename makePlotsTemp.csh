#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_Feb20_Slim/)
  foreach chan (Mu El)
    foreach prefix (GE1BTags_Final GE1BTags_Hard GE1BTags_Soft GE1BTags Ex0BTags)

       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix ptTop 1 'p_\{T\}${lp}W,best\ jet${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mTop 1 'M${lp}W,best\ jet${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 1
       if ($chan == Mu ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 1
       if ($chan == El ) eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet2Pt 1 'p_\{T\}${lp}jet2${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix MET 1 'E_\{T\}^\{miss\}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix AK4Jet1Jet2Pt 1 'p_\{T\}${lp}jet1,jet2${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix MinLepAK4dR 1 'min${po}DeltaR${lp}lepton,jet${rp}' 1
       eval ./DrawOneHistoSystUnc.o $dir $chan $prefix LepJetpTrel 1 'p_\{T\}^\{rel\}${lp}lepton,closest\ jet${rp}' 1

    end

  end
end
echo "Done!"
