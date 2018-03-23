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
indir = 'root://cmseos.fnal.gov//store/user/drankin/LJMet/Feb17_All'

#################################################
### Names to give to your output root files
#################################################

opts = ['_pup','_pum','_mum','_mup','_tpp','_tqp','_tqm','_btp','_btm','_mtp','_mtm','_jrm','_jrp','_jsm','_jsp','_wjp','_wjm']

samples = [
    #'WJets',
    #'WJets_HT100to200',
    #'WJets_HT200to400',
    #'WJets_HT400to600',
    #'WJets_HT600to800',
    #'WJets_HT800to1200',
    #'WJets_HT1200to2500',
    #'WJets_HT2500toInf',
    'WJets_Pt100to250',
    'WJets_Pt250to400',
    'WJets_Pt400to600',
    'WJets_Pt600toInf',
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

rmasses = [
'1000',
'1100',
'1200',
'1300',
'1400',
'1500',
'1600',
'1700',
'1800',
'1900',
'2000',
'2100',
'2200',
'2300',
'2400',
'2500',
'2600',
'2700',
'2800',
'2900',
'3000',
'3100',
'3200',
'3300',
'3400',
'3500',
'3600',
'3700',
'3800',
'3900',
'4000',
]

samples = []

for i in rmasses:
    #samples.extend(['Wprime'+i+'Right'])
    samples.extend(['Wprime'+i+'Mix'])
    samples.extend(['Wprime'+i+'Left'])

#samples = ['Wprime3100Right','Wprime3200Right','Wprime3300Right','Wprime3400Right','Wprime3500Right','Wprime3600Right','Wprime3700Right','Wprime3800Right','Wprime3900Right','Wprime4000Right']
#samples = ['TTbar']

### Write the files you wish to run over for each job    

#make tarball and move to eos
os.system('tar -cvf wprimeanalyzer.tar ./*.C ./*.o ./*.txt tools/* Makefile setup.csh DataPU_Jul21_2016BCD.root DataPU_Jul21_2016BCD_minus.root DataPU_Jul21_2016BCD_plus.root DataPU_Jan04_2016.root DataPU_Jan04_2016_minus.root DataPU_Jan04_2016_plus.root Moriond17MC_25ns_PUScenarioV1.root Spring16MC_25ns_PUScenarioV1.root')
os.system('xrdcp -f wprimeanalyzer.tar root://cmseos.fnal.gov//store/user/drankin/wprimeanalyzer_'+cmssw+'.tar')

locdir = suffix+'_'+logdir
#make local directory
os.system('mkdir -p  %s' %(locdir))
#make eos directory
os.system('eosmkdir -p  %s' %(eosdir))
    
for o in opts:
    for i in range(len(samples)):

        if ((o=='_tpm' or o=='_tpp') and samples[i]!='TTbar'):
            os.system('xrdcp -f '+eosdir+'/aSlimOutFile_base_'+samples[i]+'_'+suffix+'.root '+eosdir+'/aSlimOutFile'+o+'_'+samples[i]+'_'+suffix+'.root')
            continue
        if ((o=='_tqm' or o=='_tqp') and samples[i]!='TTbar'):
            os.system('xrdcp -f '+eosdir+'/aSlimOutFile_base_'+samples[i]+'_'+suffix+'.root '+eosdir+'/aSlimOutFile'+o+'_'+samples[i]+'_'+suffix+'.root')
            continue
        if ((o=='_wjm' or o=='_wjp') and not samples[i].startswith('WJets')):
            os.system('xrdcp -f '+eosdir+'/aSlimOutFile_base_'+samples[i]+'_'+suffix+'.root '+eosdir+'/aSlimOutFile'+o+'_'+samples[i]+'_'+suffix+'.root')
            continue
        if ((o=='_mum' or o=='_mup') and samples[i].startswith('Wprime')):
            os.system('xrdcp -f '+eosdir+'/aSlimOutFile_base_'+samples[i]+'_'+suffix+'.root '+eosdir+'/aSlimOutFile'+o+'_'+samples[i]+'_'+suffix+'.root')
            continue

        condor_templ_file = open(rel_base+"/src/UserCode/drankin/WprimeAna/Analyzercondor.templ")
        csh_templ_file    = open(rel_base+"/src/UserCode/drankin/WprimeAna/Analyzercsh.templ")
    
        localcondor = locdir+'/'+samples[i]+o+".condor"
        condor_file = open(localcondor,"w")
        for line in condor_templ_file:
            line=line.replace('DIRECTORY',locdir)
            line=line.replace('PREFIX',samples[i]+o)
            condor_file.write(line)
        condor_file.close()
    
        ftype = 'MC'
        filename = samples[i]
        if (samples[i]=='Data'):
            ftype = 'Data'
            filename = 'SingleLep'
        if (samples[i].endswith('_HF') or samples[i].endswith('_LF')):
            filename = samples[i][:-3]
    
        localcsh=locdir+'/'+samples[i]+o+".csh"
        csh_file = open(localcsh,"w")
        for line in csh_templ_file:
            line=line.replace('CMSSWBASE',rel_base)
            line=line.replace('TYPE',ftype)
            line=line.replace('SUFFIX',suffix)
            line=line.replace('SAMLAB',samples[i])
            line=line.replace('CMSSWVERSION',cmssw)
            if (o=='_tqp'):
                line=line.replace('SAMPLE',samples[i]+'_scaleup')
                line=line.replace('INFILE',filename+'_scaleup')
                line=line.replace('SYSOPT','')
            elif (o=='_tqm'):
                line=line.replace('SAMPLE',samples[i]+'_scaledown')
                line=line.replace('INFILE',filename+'_scaledown')
                line=line.replace('SYSOPT','')
            elif (o=='_jrm'):
                line=line.replace('SAMPLE',samples[i])
                line=line.replace('INFILE',filename+'_JERDOWN')
                line=line.replace('SYSOPT','')
            elif (o=='_jrp'):
                line=line.replace('SAMPLE',samples[i])
                line=line.replace('INFILE',filename+'_JERUP')
                line=line.replace('SYSOPT','')
            elif (o=='_jsm'):
                line=line.replace('SAMPLE',samples[i])
                line=line.replace('INFILE',filename+'_JESDOWN')
                line=line.replace('SYSOPT','')
            elif (o=='_jsp'):
                line=line.replace('SAMPLE',samples[i])
                line=line.replace('INFILE',filename+'_JESUP')
                line=line.replace('SYSOPT','')
            else:
                line=line.replace('SAMPLE',samples[i])
                line=line.replace('INFILE',filename)
                line=line.replace('SYSOPT',o[1:])
            line=line.replace('ANALYZER','WprimeAnalyzer_Slim_Opt')
            line=line.replace('OUTNAME','aSlimOutFile')
            line=line.replace('OPTSTR',o)
            line=line.replace('EOSDIR',indir)
            line=line.replace('EOSOUT',eosdir)
            csh_file.write(line)
        csh_file.close()
    
        os.system('chmod u+x '+locdir+'/'+samples[i]+o+'.csh')
        print 'condor file is: '+localcondor
        if (os.path.exists('%s.log'  % localcondor)):
            os.system('rm %s.log' % localcondor)
        os.system('condor_submit %s' % localcondor)
    
        condor_templ_file.close()
        csh_templ_file.close()
    
