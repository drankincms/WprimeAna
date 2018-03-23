#!/bin/tcsh -f

#set dir = Run2016_Feb27_Slim/RootFiles_For2DLimits/
set dir = Run2016_Feb27_Slim/RootFiles_For2DLimits_Bare/

foreach coup ( Right ModRight Left Mix )
#foreach coup ( Right )
  #foreach opt ( _Final_ )
  foreach opt ( _Final_ _Hard_ _Soft_)
  #foreach opt ( _Hard_ _Soft_)
    eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root
    eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root
    #eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ge1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ge1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ge1btags.root
    eval hadd -f ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ex1or2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root
    eval hadd -f ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ex1or2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root
    eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ex1or2btags.root ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ex1btags.root ${dir}comb_mWprime_Limits_Wprime${coup}${opt}Histos-ex2btags.root
  end
  eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1btags.root
  eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex2btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Hard_Histos-ex2btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Soft_Histos-ex2btags.root
  eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1or2btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1or2btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1or2btags.root
  #eval hadd -f ${dir}comb_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ge1btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Hard_Histos-ge1btags.root ${dir}comb_mWprime_Limits_Wprime${coup}_Soft_Histos-ge1btags.root
  eval hadd -f ${dir}muon_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1btags.root
  eval hadd -f ${dir}muon_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Hard_Histos-ex2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Soft_Histos-ex2btags.root
  eval hadd -f ${dir}muon_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1or2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1or2btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1or2btags.root
  #eval hadd -f ${dir}muon_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ge1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Hard_Histos-ge1btags.root ${dir}muon_mWprime_Limits_Wprime${coup}_Soft_Histos-ge1btags.root
  eval hadd -f ${dir}electron_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1btags.root
  eval hadd -f ${dir}electron_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Hard_Histos-ex2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Soft_Histos-ex2btags.root
  eval hadd -f ${dir}electron_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ex1or2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Hard_Histos-ex1or2btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Soft_Histos-ex1or2btags.root
  #eval hadd -f ${dir}electron_mWprime_Limits_Wprime${coup}_HardSoft_Histos-ge1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Hard_Histos-ge1btags.root ${dir}electron_mWprime_Limits_Wprime${coup}_Soft_Histos-ge1btags.root
end

