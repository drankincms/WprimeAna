#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

#foreach dir (Run2016_Jan13_Slim/)
foreach dir (Run2016_Feb20_Slim/)
  foreach chan (Mu El)
    foreach prefix (Ex0BTags_Soft Ex0BTags_Hard Ex0BTags_Final )
      eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
      #eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
    end
    foreach prefix (Ex1BTags_Final Ex2BTags_Final Ex1BTags_Soft Ex2BTags_Soft Ex1BTags_Hard Ex2BTags_Hard)
      #eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale} \"\" 0 1
      #eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale} \"\" 0 1
      eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
      #eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
    end
  end
end
echo "Done!"
