#!/usr/bin/python

import os
import re
import fileinput

rel_base = os.environ['CMSSW_BASE']
cmssw = 'CMSSW_8_0_26_patch2'
logdir = 'Jan13_LHE'
suffix = 'Run2016'
outdir = '/eos/uscms/store/user/drankin/WprimeToTB/'+suffix+'_'+logdir
eosdir = 'root://cmseos.fnal.gov/'+outdir
indir = 'root://cmseos.fnal.gov//store/user/drankin/LJMet/Dec22_All'

#################################################
### Names to give to your output root files
#################################################

samples = [
    #'WJets',
    'WJets_HT100to200',
    'WJets_HT200to400',
    'WJets_HT400to600',
    'WJets_HT600to800',
    'WJets_HT800to1200',
    'WJets_HT1200to2500',
    'WJets_HT2500toInf',
    'TTbar',
    'ZJets',
    'T_s',
    'T_t',
    'Tbar_t',
    'T_tW',
    'Tbar_tW',
    'WW',
    'WZ',
    'ZZ',
    #'QCD_Pt_120to170',
    #'QCD_Pt_170to300',
    #'QCD_Pt_300to470',
    #'QCD_Pt_470to600',
    #'QCD_Pt_600to800',
    #'QCD_Pt_800to1000',
    #'QCD_Pt_1000to1400',
]

#samples = ['T_t']

### Write the files you wish to run over for each job    

#make tarball and move to eos
os.system('tar -cvf wprimeanalyzer.tar ./*.C ./*.o ./*.txt tools/* Makefile setup.csh DataPU_Jul21_2016BCD.root DataPU_Jul21_2016BCD_minus.root DataPU_Jul21_2016BCD_plus.root DataPU_Jan04_2016.root DataPU_Jan04_2016_minus.root DataPU_Jan04_2016_plus.root Moriond17MC_25ns_PUScenarioV1.root Spring16MC_25ns_PUScenarioV1.root')
os.system('xrdcp -f wprimeanalyzer.tar root://cmseos.fnal.gov//store/user/drankin/wprimeanalyzer_'+cmssw+'.tar')

locdir = suffix+'_'+logdir
    
#make local directory
os.system('mkdir -p  %s' %locdir)
#make eos directory
os.system('eosmkdir -p  %s' %eosdir)

for i in range(len(samples)):

        condor_templ_file = open(rel_base+"/src/UserCode/drankin/WprimeAna/Analyzercondor.templ")
        csh_templ_file    = open(rel_base+"/src/UserCode/drankin/WprimeAna/Analyzercsh.templ")
    
        localcondor = locdir+'/'+samples[i]+".condor"
        eoscondor = "root://cmseos.fnal.gov/"+outdir[i]+"/"+samples[i]+".condor"
        condor_file = open(localcondor,"w")
        for line in condor_templ_file:
            line=line.replace('DIRECTORY',locdir)
            line=line.replace('PREFIX',samples[i])
            condor_file.write(line)
        condor_file.close()
    
        #copy local to eos
        #os.system('xrdcp -f %s %s' % (localcondor,eoscondor))
        #remove local copy
        #os.system('rm %s' % localcondor)
    
        ftype = 'MC'
        filename = samples[i]
        if (samples[i]=='Data'):
            ftype = 'Data'
            filename = 'SingleLep'
        if (samples[i].endswith('_HF') or samples[i].endswith('_LF')):
            filename = samples[i][:-3]
    
        eoscsh="root://cmseos.fnal.gov/"+outdir[i]+"/"+samples[i]+".csh"
        localcsh=locdir+'/'+samples[i]+".csh"
        csh_file = open(localcsh,"w")
        for line in csh_templ_file:
            line=line.replace('CMSSWBASE',rel_base)
            line=line.replace('TYPE',ftype)
            line=line.replace('SUFFIX',suffix)
            line=line.replace('SAMPLE',samples[i])
            line=line.replace('SAMLAB',samples[i])
            line=line.replace('CMSSWVERSION',cmssw)
            line=line.replace('INFILE',filename)
            line=line.replace('ANALYZER','WprimeAnalyzer_LHE')
            line=line.replace('OUTNAME','anLHEOutFile')
            line=line.replace('OPTSTR','')
            line=line.replace('SYSOPT','')
            line=line.replace('EOSDIR',indir)
            line=line.replace('EOSOUT',eosdir)
            csh_file.write(line)
        csh_file.close()
    
        #os.system('xrdcp -f %s %s' % (localcsh,eoscsh))
        #os.system('rm %s' %localcsh)
    
        os.system('chmod u+x '+locdir+'/'+samples[i]+'.csh')
        print 'condor file is: '+localcondor
        if (os.path.exists('%s.log'  % localcondor)):
            os.system('rm %s.log' % localcondor)
        os.system('condor_submit %s' % localcondor)
    
        condor_templ_file.close()
        csh_templ_file.close()
    
