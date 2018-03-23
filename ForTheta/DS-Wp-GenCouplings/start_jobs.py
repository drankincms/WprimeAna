#!/usr/bin/python

import os
import re
import fileinput
import subprocess

rel_base = os.environ['CMSSW_BASE']
cmssw = 'CMSSW_7_4_14'
logdir = 'Jul18_35p9fb'
suffix = '2D_NewLUM'
outdir = '/eos/uscms/store/user/drankin/theta/'+logdir+'_'+suffix
eosdir = 'root://cmseos.fnal.gov/'+outdir
indir = '/uscms_data/d3/drankin/Wprimetotb/CMSSW_8_0_26_patch2/src/UserCode/drankin/WprimeAna/Run2016_Feb27_Slim/RootFiles_For2DLimits/'

locdir = logdir+'_'+suffix

#make local directory
os.system('mkdir -p  %s' %locdir)
#make eos directory
#os.system('eosmkdir -p  %s' %eosdir)

pwd = os.getcwd()

#make tarball and move to eos
os.chdir(indir)
os.system('tar -cvf thetaroot.tar *.root')
os.system('xrdcp -f thetaroot.tar root://cmseos.fnal.gov//store/user/drankin/thetaroot.tar')
os.system('rm thetaroot.tar')
os.chdir(pwd)

os.system('tar -cvf theta.tar *.py removefirstlines')
os.system('xrdcp -f theta.tar root://cmseos.fnal.gov//store/user/drankin/theta_2d.tar')

chanlist = ['combined','elec','mu']

for al in range(0,11):
    for ar in range (0,11):
        for chan in chanlist:
            chandir = 'COMBINED'
            if (chan == 'elec'): chandir = 'ELECTRONS'
            if (chan == 'mu'): chandir = 'MUONS'
   
            condor_templ_file = open(rel_base+"/src/theta/utils2/test/Wprime/DS-Wp-GenCouplings/Thetacondor.templ")
            csh_templ_file    = open(rel_base+"/src/theta/utils2/test/Wprime/DS-Wp-GenCouplings/Thetacsh.templ")
    
            localcondor = logdir+'_'+suffix+'/Wprime2DLimits_'+chan+'_'+str(al)+'-'+str(ar)+'.condor'
            condor_file = open(localcondor,"w")
            for line in condor_templ_file:
                line=line.replace('DIRECTORY',locdir)
                line=line.replace('PREFIX','Wprime2DLimits_'+chan+'_'+str(al)+'-'+str(ar))
                condor_file.write(line)
            condor_file.close()
    
            localcsh = logdir+'_'+suffix+'/Wprime2DLimits_'+chan+'_'+str(al)+'-'+str(ar)+'.csh'
            csh_file = open(localcsh,"w")
            for line in csh_templ_file:
                if (chan == 'elec'):
                    line=line.replace('ELCOM','')
                    line=line.replace('MUCOM','#')
                elif (chan == 'mu'):
                    line=line.replace('ELCOM','#')
                    line=line.replace('MUCOM','')
                else:
                    line=line.replace('ELCOM','')
                    line=line.replace('MUCOM','')
                line=line.replace('CMSSWBASE',rel_base)
                line=line.replace('CMSSWVERSION',cmssw)
                line=line.replace('CHAN',chan)
                line=line.replace('DIRNAME',chandir)
                line=line.replace('AL',str(al))
                line=line.replace('AR',str(ar))
                line=line.replace('EOSOUT',eosdir)
                line=line.replace('LOCDIR',locdir)
                line=line.replace('XX','')
                csh_file.write(line)
            csh_file.close()
    
            os.system('chmod u+x '+locdir+'/Wprime2DLimits_'+chan+'_'+str(al)+'-'+str(ar)+'.csh')
            print 'condor file is: '+localcondor
            if (os.path.exists('%s.log'  % localcondor)):
                os.system('rm %s.log' % localcondor)
            os.system('condor_submit %s' % localcondor)
    
            condor_templ_file.close()
            csh_templ_file.close()
