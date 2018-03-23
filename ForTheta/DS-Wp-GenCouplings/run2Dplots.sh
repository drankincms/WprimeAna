#!/bin/sh

direc="Jul18_35p9fb_2D_NewLUM/"

#for lepton in COMBINED
for lepton in ELECTRONS MUONS COMBINED
#for lepton in ELECTRONS MUONS
  do
  lepname="combined"
  if [[ ${lepton} == ELECTRONS ]]; then
    lepname="electron"
  fi
  if [[ ${lepton} == MUONS ]]; then
    lepname="muon"
  fi
  cd ${direc}${lepton}
  for type in observed expected
  #for type in expected
    do
    root -l -b -q ../../alVSar_2D_plots_${type}.C\(\"${lepname}\"\)
    short="exp"
    if [[ ${type} == observed ]]; then
      short="obs"
    fi
    root -l -b -q ../../${short}150.C\(\"${lepname}\"\)
  done
  cd -
done
