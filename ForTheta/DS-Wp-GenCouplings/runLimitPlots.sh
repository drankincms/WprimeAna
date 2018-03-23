#!/bin/sh

DIRS="Feb27_35p9fb_2D/COMBINED/"
#echo $DIRS# x; y
#title="e_mu+jets/BDT"

#for cat in ex1or2 ex1 ex2 ge1
for cat in ex1or2
  do
  category="ge1btags"
  if [[ ${cat} == ex1 ]]; then
    category="ex1btags"
  fi
  if [[ ${cat} == ex2 ]]; then
    category="ex2btags"
  fi
  if [[ ${cat} == ex1or2 ]]; then
    category="ex1or2btags"
  fi
  for il in 0 1 2 3 4 5 6 7 8 9 10
    do
    for ir in 0 1 2 3 4 5 6 7 8 9 10
      do
      for lepton in combined
      #for lepton in electron muon comb 
      #for lepton in electron muon 
        do
  
        title="e/mu+jets"
        if [[ ${lepton} == muon ]]; then
          title="mu+jets"
        fi
        if [[ ${lepton} == electron ]]; then
          title="e+jets"
        fi
        #FILES="${sys}_Wprime_${coup}_${lepton}"
        FILES="${lepton}"
        echo ${DIRS} ${FILES} ${title} ${category}
        root -l -b -q plotLimits.C\(\"${DIRS}\",\"${FILES}\",\"${title}\",\"${il}-${ir}\"\) &
  
      done
    done
  done
done
