#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

set prefix = GE0BTags
#set prefix = Ex0BTags

set dir = Run2016_Feb20_Slim/
foreach chan (Mu El)
  eval ./DrawOneHistoPU.o ${dir} $chan $prefix nPV 1 '${po}\ primary\ vertices' 1 0 ${scale}
  #eval ./DrawOneHistoPU.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale}
  #if ($chan == Mu ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale}
  #if ($chan == El ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 0 ${scale}
  eval ./DrawOneHistoPU.o ${dir} $chan $prefix nPV 1 '${po}\ primary\ vertices' 1 0 ${scale} _pup
  #eval ./DrawOneHistoPU.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale} _pup
  #if ($chan == Mu ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale} _pup
  #if ($chan == El ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 0 ${scale} _pup
  eval ./DrawOneHistoPU.o ${dir} $chan $prefix nPV 1 '${po}\ primary\ vertices' 1 0 ${scale} _pum
  #eval ./DrawOneHistoPU.o $dir $chan $prefix mWprime_Limits 1 'M${lp}tb${rp}' 2 0 ${scale} _pum
  #if ($chan == Mu ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}${po}mu${rp}' 25 0 ${scale} _pum
  #if ($chan == El ) eval ./DrawOneHistoPU.o $dir $chan $prefix LepPt 1 'p_\{T\}${lp}e${rp}' 25 0 ${scale} _pum
end
echo "Done!"


#set dir = Run2016_Jan31/
#
#foreach chan (Mu El)
#  foreach prefix (Presel PreselPU)
#    eval ./DrawOneHistoThreeSig.o ${dir} $chan $prefix nPV 1 '${po}\ primary\ vertices' 1 1 ${scale}
#  end
#end
#echo "Done!"
