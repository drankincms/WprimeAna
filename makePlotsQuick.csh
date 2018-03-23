#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
#set scale = 1.04976
set scale = 1.0

foreach dir (Run2016_Feb27_Slim/)
  foreach chan (Mu El)
    #foreach prefix (Ex1BTags_Final Ex2BTags_Final Ex0BTags GE1BTags_Final)
    #foreach prefix (Ex0BTags)
    foreach prefix (Ex0BTags GE1BTags GE1BTags_Final Ex1BTags_Hard Ex1BTags_Soft Ex2BTags_Hard Ex2BTags_Soft Ex1BTags_Final Ex2BTags_Final Ex1BTags Ex2BTags )
      #eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
      #eval ./DrawOneHistoThreeSig.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}

      #eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
      #eval ./DrawOneHistoSigRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}

      #eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100
      #eval ./DrawOneHistoSystUncRatio.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2

      eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 
      eval ./DrawOneHistoSystUnc.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2

       #if ($chan == Mu ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepEnergy 1 'E${lp}${po}mu${rp}' 25 0 ${scale}
       #if ($chan == El ) eval ./DrawOneHistoThreeSig.o $dir $chan $prefix LepEnergy 1 'E${lp}e${rp}' 25 0 ${scale}

       #if ($chan == Mu ) eval ./DrawOneHistoSigRatio.o $dir $chan $prefix LepEnergy 1 'E${lp}${po}mu${rp}' 25 0 ${scale}
       #if ($chan == El ) eval ./DrawOneHistoSigRatio.o $dir $chan $prefix LepEnergy 1 'E${lp}e${rp}' 25 0 ${scale}
    end
  end
end
echo "Done!"
