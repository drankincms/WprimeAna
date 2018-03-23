#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
#set scale = 1.04976
set scale = 1.0

foreach dir (Run2016_Feb20_Slim/)
  foreach chan (Mu El)
    foreach prefix (GE1BTags Ex0BTags)
    #foreach prefix (Ex0BTags Ex0BTags_Soft Ex0BTags_Hard Ex0BTags_Final)
      eval ./DrawWJetsShape.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale}
      eval ./DrawWJetsShape.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
      #foreach suff ( _tpp )
      #    eval ./DrawWJetsShape.o $dir $chan $prefix mWprime 1 'M${lp}tb${rp}' 100 0 ${scale} ${suff}
      #    eval ./DrawWJetsShape.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale} ${suff}
      #end
    end
  end
end
echo "Done!"
