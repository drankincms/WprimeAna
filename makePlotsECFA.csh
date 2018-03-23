#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 232.558

set dir = Run2016BCD_Jul27_Slim/
foreach chan (Mu El)
  foreach prefix (GE1BTags_Final Ex1BTags_Final Ex2BTags_Final)
    eval ./DrawOneHistoECFA.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 1 0 ${scale}
    eval ./DrawOneHistoECFA.o $dir $chan $prefix mWprime_Limits 0 'M${lp}tb${rp}' 1 0 ${scale}
    if ($chan == Mu ) eval ./DrawOneHistoECFA.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale}
    if ($chan == El ) eval ./DrawOneHistoECFA.o $dir $chan $prefix LepPt 0 'p_\{T\}${lp}e${rp}' 25 0 ${scale}
  end
end
echo "Done!"
