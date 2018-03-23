#!/bin/sh

DIRS=`pwd`
echo $DIRS

prefix="Wprime_Histos_"
#prefix="Wprime_GE1Histos_"

suffix=".root"
mid="-"

mv ${prefix}mu* MUONS/
mv ${prefix}elec* ELECTRONS/

for ((ar_step=0; ar_step <=10; ar_step++)) 
do
  for ((al_step=0; al_step <=10; al_step++)) 
  do
    lepton="mu"
    fileNameMU=${prefix}${lepton}${mid}${al_step}${mid}${ar_step}${suffix}
    lepton='elec'
    fileNameELEC=${prefix}${lepton}${mid}${al_step}${mid}${ar_step}${suffix}
    lepton='combined'
    fileNameCOMB=${prefix}${lepton}${mid}${al_step}${mid}${ar_step}${suffix}

    hadd -f COMBINED/${fileNameCOMB} ELECTRONS/${fileNameELEC}  MUONS/${fileNameMU}

  done 
done
