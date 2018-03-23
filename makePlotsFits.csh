#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

set min = 250

foreach dir (Run2016_Jan13/)
  #foreach chan (Mu El All)
  foreach chan (All)
    #foreach prefix (Ex0BTags_PostLowJet1 Ex0BTags_PostLowJet1LowLep)
    foreach prefix (Ex0BTags_PostLowJet1LowLep)
    #foreach prefix (GE0BTags GE1BTags)
    #foreach prefix (Ex0BTags)
      foreach qcd ( 0 )

        eval ./DrawOneHistoFit.o $dir $chan $prefix AllJetPt 1 'p_\{T\}${lp}all\ jets${rp}' 50 ${qcd} ${scale} ${min}
        #eval ./DrawOneHistoFit.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 ${qcd} ${scale} ${min}

        #foreach max ( 350. 450. 800. )
        #foreach max ( 800. 1000. 1200. )
          
          #eval ./DrawOneHistoFit.o $dir $chan $prefix AllJetPt 1 'p_\{T\}${lp}all\ jets${rp}' 25 ${qcd} ${scale} ${min} ${max}
          #eval ./DrawOneHistoFit.o $dir $chan $prefix AK4Jet1Pt 1 'p_\{T\}${lp}jet1${rp}' 50 ${qcd} ${scale} ${min} ${max}

        #end
      end
    end
  end
end
echo "Done!"
