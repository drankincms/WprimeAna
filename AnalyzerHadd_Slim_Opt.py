#!/usr/bin/python

import os
import re
import fileinput

rel_base = os.environ['CMSSW_BASE']
cmssw = 'CMSSW_8_0_26_patch2'
logdir = 'Feb27_Slim'
suffix = 'Run2016'
outdir = '/store/user/drankin/WprimeToTB/'+suffix+'_'+logdir
eosdir = 'root://cmseos.fnal.gov/'+outdir

#################################################
### Names to give to your output root files
#################################################

opts = ['_pup','_pum','_mum','_mup','_tpp','_tqp','_tqm','_btp','_btm','_mtp','_mtm','_jrm','_jrp','_jsm','_jsp','_wjp','_wjm']

### Write the files you wish to run over for each job    

locdir = suffix+'_'+logdir
#make local directory
os.system('mkdir -p  %s' %(locdir))
#make eos directory
os.system('eosmkdir -p  %s' %(eosdir))
    
for o in opts:
    condor_templ_file = open(rel_base+"/src/UserCode/drankin/WprimeAna/Haddcondor.templ")
    csh_templ_file    = open(rel_base+"/src/UserCode/drankin/WprimeAna/Haddcsh.templ")

    localcondor = locdir+'/Hadd'+o+".condor"
    condor_file = open(localcondor,"w")
    for line in condor_templ_file:
        line=line.replace('DIRECTORY',locdir)
        line=line.replace('PREFIX','Hadd'+o)
        condor_file.write(line)
    condor_file.close()
    
    localcsh=locdir+'/Hadd'+o+".csh"
    csh_file = open(localcsh,"w")
    for line in csh_templ_file:
        line=line.replace('CMSSWBASE',rel_base)
        line=line.replace('CMSSWVERSION',cmssw)
        line=line.replace('HADD','''hadd -f outFile'''+o+'''.root `xrdfs root://cmseos.fnal.gov ls -u '''+outdir+'''/ | grep ".root" | grep "aSlimOutFile'''+o+'''"`''')
        line=line.replace('XRDCP','xrdcp -f outFile'+o+'.root '+eosdir+'/outFile'+o+'.root')
        csh_file.write(line)
    csh_file.close()
    
    os.system('chmod u+x '+locdir+'/Hadd'+o+'.csh')
    print 'condor file is: '+localcondor
    if (os.path.exists('%s.log'  % localcondor)):
        os.system('rm %s.log' % localcondor)
    os.system('condor_submit %s' % localcondor)
    
    condor_templ_file.close()
    csh_templ_file.close()
    
print 'HADDING '+suffix+'_'+logdir

