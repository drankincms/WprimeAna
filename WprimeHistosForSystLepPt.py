import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math

import os.path

indir = "Run2016BCD_Neg_Jul26_Slim/"
os.system('mkdir -p '+indir+'RootFiles_ForSyst')

#lumi = 2.0
REBIN = 100

######################################################
RootFiles = {}
Histos = {}
HistosPre = {}

FullSyst = ['_','_jsp_','_jsm_','_jrp_','_jrm_','_btp_','_btm_','_pup_','_pum_','_tpp_','_tpm_','_wjp_','_wjm_','_mup_','_mum_','_tqp_','_tqm_','_mtp_','_mtm_','_msp_','_msm_','_mrp_','_mrm_']
Backgrounds = ['WJets_HT100to200','WJets_HT200to400','WJets_HT400to600','WJets_HT600to800','WJets_HT800to1200','WJets_HT1200to2500','WJets_HT2500toInf','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','WW','WZ','ZZ']
#Backgrounds = ['WJets','TTbar']
masses = ['1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000']
Channels = ['El','Mu']
BTags = ['GE1BTags','Ex1BTags','Ex2BTags']
Systematics = ['_','_msp_','_msm_']
#Systematics = FullSyst


RootFiles['Base'] = TFile(indir+"outFile.root")
for chk in FullSyst:
    if (chk=='_'): continue
    if (os.path.isfile(indir+"outFile_"+chk.replace('_','')+".root")):
        RootFiles[chk.replace('_','')] = TFile(indir+"outFile_"+chk.replace('_','')+".root")
    else:
        RootFiles[chk.replace('_','')] = TFile(indir+"outFile.root")

def loadHistos(opt):

    for chan in Channels:
        for btag in BTags:
            if (btag == 'GE1BTags'): btaglabel = 'ge1b'
            if (btag == 'Ex1BTags'): btaglabel = 'ex1b'
            if (btag == 'Ex2BTags'): btaglabel = 'ex2b'

            if (chan=='El'): Histos['Data_'+btaglabel+'_el']  = RootFiles['Base'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Data'+'_'+chan)
            if (chan=='Mu'): Histos['Data_'+btaglabel+'_mu']  = RootFiles['Base'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Data'+'_'+chan)
    
            for bg in Backgrounds:
                Histos[bg+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jsp_'+btaglabel+'_'+chan] = RootFiles['jsp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jsm_'+btaglabel+'_'+chan] = RootFiles['jsm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jrp_'+btaglabel+'_'+chan] = RootFiles['jrp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jrm_'+btaglabel+'_'+chan] = RootFiles['jrm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mup_'+btaglabel+'_'+chan] = RootFiles['mup'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mum_'+btaglabel+'_'+chan] = RootFiles['mum'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_pup_'+btaglabel+'_'+chan] = RootFiles['pup'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_pum_'+btaglabel+'_'+chan] = RootFiles['pum'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_tpp_'+btaglabel+'_'+chan] = RootFiles['tpp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_tpm_'+btaglabel+'_'+chan] = RootFiles['tpm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_btp_'+btaglabel+'_'+chan] = RootFiles['btp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_btm_'+btaglabel+'_'+chan] = RootFiles['btm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mtp_'+btaglabel+'_'+chan] = RootFiles['mtp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mtm_'+btaglabel+'_'+chan] = RootFiles['mtm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_wjp_'+btaglabel+'_'+chan] = RootFiles['wjp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_wjm_'+btaglabel+'_'+chan] = RootFiles['wjm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_msp_'+btaglabel+'_'+chan] = RootFiles['msp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_msm_'+btaglabel+'_'+chan] = RootFiles['msm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mrp_'+btaglabel+'_'+chan] = RootFiles['mrp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mrm_'+btaglabel+'_'+chan] = RootFiles['mrm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                if (bg=='TTbar'):
                    Histos[bg+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_scaleup_'+chan)
                    Histos[bg+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_scaledown_'+chan)
                else:
                    Histos[bg+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
                    Histos[bg+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+bg+'_'+chan)
           
                if (not bg.startswith('T') and not bg.startswith('QCD')):
                    HistosPre[bg+'_'+chan] = RootFiles['Base'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jsp_'+chan] = RootFiles['jsp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jsm_'+chan] = RootFiles['jsm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jrp_'+chan] = RootFiles['jrp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jrm_'+chan] = RootFiles['jrm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mup_'+chan] = RootFiles['mup'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mum_'+chan] = RootFiles['mum'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_pup_'+chan] = RootFiles['pup'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_pum_'+chan] = RootFiles['pum'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tpp_'+chan] = RootFiles['tpp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tpm_'+chan] = RootFiles['tpm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_btp_'+chan] = RootFiles['btp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_btm_'+chan] = RootFiles['btm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mtp_'+chan] = RootFiles['mtp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mtm_'+chan] = RootFiles['mtm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_wjp_'+chan] = RootFiles['wjp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_wjm_'+chan] = RootFiles['wjm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_msp_'+chan] = RootFiles['msp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_msm_'+chan] = RootFiles['msm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mrm_'+chan] = RootFiles['mrm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tqp_'+chan] = RootFiles['tqp'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tqm_'+chan] = RootFiles['tqm'].Get('GE0BTags'+opt+'/'+chan+'/LepPt_GE0BTags'+opt+'_'+bg+'_'+chan)
            
            for mass in masses:
                #for coup in ['Right','Left','Mix']:
                for coup in ['Right']:
            
                    Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jsp_'+btaglabel+'_'+chan] = RootFiles['jsp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jsm_'+btaglabel+'_'+chan] = RootFiles['jsm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jrp_'+btaglabel+'_'+chan] = RootFiles['jrp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jrm_'+btaglabel+'_'+chan] = RootFiles['jrm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mup_'+btaglabel+'_'+chan] = RootFiles['mup'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mum_'+btaglabel+'_'+chan] = RootFiles['mum'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_pup_'+btaglabel+'_'+chan] = RootFiles['pup'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_pum_'+btaglabel+'_'+chan] = RootFiles['pum'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tpp_'+btaglabel+'_'+chan] = RootFiles['tpp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tpm_'+btaglabel+'_'+chan] = RootFiles['tpm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_btp_'+btaglabel+'_'+chan] = RootFiles['btp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_btm_'+btaglabel+'_'+chan] = RootFiles['btm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mtp_'+btaglabel+'_'+chan] = RootFiles['mtp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mtm_'+btaglabel+'_'+chan] = RootFiles['mtm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_wjp_'+btaglabel+'_'+chan] = RootFiles['wjp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_wjm_'+btaglabel+'_'+chan] = RootFiles['wjm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_msp_'+btaglabel+'_'+chan] = RootFiles['msp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_msm_'+btaglabel+'_'+chan] = RootFiles['msm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mrp_'+btaglabel+'_'+chan] = RootFiles['mrp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mrm_'+btaglabel+'_'+chan] = RootFiles['mrm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/LepPt_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)

List_DataEl = ['Data_ex1b_el','Data_ex2b_el','Data_ge1b_el']
List_DataMu = ['Data_ex1b_mu','Data_ex2b_mu','Data_ge1b_mu']

#List_Bg = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
#List_Bg = ['WJets','TTbar']
List_Bg = Backgrounds


List_Right = []
List_Right.extend(['Wprime' + s + 'Right' for s in masses])


def makeHistos(channel, save, wprime, btags, List_to_use):


    print wprime

    UseQCD = False
 
    if (btags == 'ge1btags'): btaglabel = 'ge1b'
    if (btags == 'ex1btags'): btaglabel = 'ex1b'
    if (btags == 'ex2btags'): btaglabel = 'ex2b'

    background = 0

    for Type in List_to_use:
    
        if (channel == 'electron'):
            Chan = 'El'
        if (channel == 'muon'):
            Chan = 'Mu'

        print Type

        ######################################### 
        ### Set 0 B.G. Bins to something !=0 
        #########################################

        if (not (Type.startswith('Data'))):
            for sys in Systematics:
                if ( not Type.startswith('Wprime') ):
                    for x in range(1,Histos[Type+sys+btaglabel+'_'+Chan].GetNbinsX()+1):
                        if (Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(x) < 0.000001 ): 
                            #print 'Setting ',Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(x),' to 10E-6 for bin ',x,' of ',Type
                            Histos[Type+sys+btaglabel+'_'+Chan].SetBinContent(x,0.000001)
    
                #Histos[Type+sys+btaglabel+'_'+Chan].Scale(lumi)
    
                if (sys!='_'): continue
                print 'SCALED EVENTS FOR ',Type,sys,'  = ',Histos[Type+sys+btaglabel+'_'+Chan].Integral()
                if ( (not Type.startswith('T')) and (not Type.startswith('Wprime')) ):
                    if (scaleWJets):
                        #HistosPre[Type+sys+Chan].Scale(lumi)
                        print 'Pre EVENTS FOR ',Type,sys,'  = ',HistosPre[Type+sys+Chan].Integral()          
    
                if ( (not Type.startswith('Data')) and (not Type.startswith('Wprime')) and (sys == '_')):
                    background = background + Histos[Type+sys+btaglabel+'_'+Chan].Integral()


    if (channel == 'electron'):
        chan = 'elec_'
        Histos['Data_'+btaglabel+'_el'].SetName(chan+"invmass_"+btags+"__DATA")
        Histos['Data_'+btaglabel+'_el'].SetTitle(chan+"invmass_"+btags+"__DATA")
        Histos['Data_'+btaglabel+'_el'].Rebin(REBIN)
        Histos['Data_'+btaglabel+'_el'].Write()
    if (channel == 'muon'):
        chan = 'mu_'
        Histos['Data_'+btaglabel+'_mu'].SetName(chan+"invmass_"+btags+"__DATA")
        Histos['Data_'+btaglabel+'_mu'].SetTitle(chan+"invmass_"+btags+"__DATA")
        Histos['Data_'+btaglabel+'_mu'].Rebin(REBIN)
        Histos['Data_'+btaglabel+'_mu'].Write()

    for sys in Systematics:
        suffix = ''
        if (sys == '_jsp_'): suffix = '__jes__plus'
        if (sys == '_jsm_'): suffix = '__jes__minus'
        if (sys == '_jrp_'): suffix = '__jer__plus'
        if (sys == '_jrm_'): suffix = '__jer__minus'
        if (sys == '_msp_'): suffix = '__mues__plus'
        if (sys == '_msm_'): suffix = '__mues__minus'
        if (sys == '_mrp_'): suffix = '__muer__plus'
        if (sys == '_mrm_'): suffix = '__muer__minus'
        if (sys == '_btp_'): suffix = '__btag__plus'
        if (sys == '_btm_'): suffix = '__btag__minus'
        if (sys == '_mtp_'): suffix = '__mtag__plus'
        if (sys == '_mtm_'): suffix = '__mtag__minus'
        if (sys == '_wjp_'): suffix = '__wjhf__plus'
        if (sys == '_wjm_'): suffix = '__wjhf__minus'
        if (sys == '_mup_'): suffix = '__murmuf__plus'
        if (sys == '_mum_'): suffix = '__murmuf__minus'
        if (sys == '_pup_'): suffix = '__pileup__plus'
        if (sys == '_pum_'): suffix = '__pileup__minus'
        if (sys == '_tpp_'): suffix = '__toppt__plus'
        if (sys == '_tpm_'): suffix = '__toppt__minus'
        if (sys == '_tqp_'): suffix = '__q2scale__plus'
        if (sys == '_tqm_'): suffix = '__q2scale__minus'

        Histos['TTbar'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__ttbar"+suffix)
        Histos['TTbar'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__ttbar"+suffix)
        if ('T_t' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['T_t'+sys+btaglabel+'_'+Chan])
        if ('Tbar_t' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['Tbar_t'+sys+btaglabel+'_'+Chan])
        if ('T_tW' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['T_tW'+sys+btaglabel+'_'+Chan])
        if ('Tbar_tW' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['Tbar_tW'+sys+btaglabel+'_'+Chan])

        if (wprime == 'Left' or wprime == 'Mix'): 
            Histos['TTbar'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
            Histos['TTbar'+sys+btaglabel+'_'+Chan].Write()
        else:
            if ( wprime == 'ModRight' ):
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Write()
                Histos['T_s'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__topstb"+suffix)
                Histos['T_s'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__topstb"+suffix)
                Histos['T_s'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['T_s'+sys+btaglabel+'_'+Chan].Write()
            elif (wprime == 'Right'):
                if ('T_s' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['T_s'+sys+btaglabel+'_'+Chan])
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Write()

        Histos['WJets'+sys+btaglabel+'_'+Chan] = Histos['WJets_HT100to200'+sys+btaglabel+'_'+Chan]
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT200to400'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT400to600'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT600to800'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT800to1200'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT1200to2500'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_HT2500toInf'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['ZJets'+sys+btaglabel+'_'+Chan])
        if ('WW' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WW'+sys+btaglabel+'_'+Chan])
        if ('WZ' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WZ'+sys+btaglabel+'_'+Chan])
        if ('ZZ' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['ZZ'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wjets"+suffix)
        Histos['WJets'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wjets"+suffix)
        if (scaleWJets) :
            HistosPre['WJets'+sys+Chan] = HistosPre['WJets_HT100to200'+sys+Chan]
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT200to400'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT400to600'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT600to800'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT800to1200'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT1200to2500'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_HT2500toInf'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['ZJets'+sys+Chan])
            if ('WW' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['WW'+sys+Chan])
            if ('WZ' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['WZ'+sys+Chan])
            if ('ZZ' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['ZZ'+sys+Chan])
            HistosPre['WJets'+sys+Chan].SetName(chan+"invmass_"+btags+"__wjets"+suffix)
            HistosPre['WJets'+sys+Chan].SetTitle(chan+"invmass_"+btags+"__wjets"+suffix)
    
        ######################################### 
        ### RESCALE W+Jets    
        #########################################
        #print 'WJets ge1/pre: ',Histos['WJets'+sys+btaglabel+'_'+Chan].Integral()/HistosPre['WJets'+sys+Chan].Integral()
        if (scaleWJets):
            HistosPre['WJets'+sys+Chan].Scale( Histos['WJets'+sys+btaglabel+'_'+Chan].Integral()/HistosPre['WJets'+sys+Chan].Integral() )
        else:
            HistosPre['WJets'+sys+Chan] = Histos['WJets'+sys+btaglabel+'_'+Chan]
        #print 'WJets after rescaling: ',HistosPre['WJets'+sys+Chan].Integral()
        #HistosPre['WJets'+sys+Chan].Rebin(REBIN)
        #HistosPre['WJets'+sys+Chan].Write()
        #Histos['WJets'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__ge1bwjets"+suffix)
        #Histos['WJets'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__ge1bwjets"+suffix)
        Histos['WJets'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
        Histos['WJets'+sys+btaglabel+'_'+Chan].Write()

        if (UseQCD):
            Histos['QCD'+sys+btaglabel+'_'+Chan] = Histos['QCD_Pt_120to170'+sys+btaglabel+'_'+Chan]
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_170to300'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_300to470'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_470to600'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_600to800'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_800to1000'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].Add(Histos['QCD_Pt_1000to1400'+sys+btaglabel+'_'+Chan])
            Histos['QCD'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__qcd"+suffix)
            Histos['QCD'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__qcd"+suffix)
            Histos['QCD'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
            Histos['QCD'+sys+btaglabel+'_'+Chan].Write()

    if (channel == 'electron'): ch = '_el' 
    if (channel == 'muon'): ch = '_mu' 

    print channel,' data = ', Histos['Data_'+btaglabel+ch].Integral()
    if (UseQCD):
        #print 'Total background = ', Histos['TTbar_'+btaglabel+'_'+Chan].Integral()+HistosPre['WJets_'+Chan].Integral()+Histos['QCD_'+btaglabel+'_'+Chan].Integral()
        print 'Total background = ', Histos['TTbar_'+btaglabel+'_'+Chan].Integral()+Histos['WJets_'+btaglabel+'_'+Chan].Integral()+Histos['QCD_'+btaglabel+'_'+Chan].Integral()
    else:
        #print 'Total background = ', Histos['TTbar_'+btaglabel+'_'+Chan].Integral()+HistosPre['WJets_'+Chan].Integral()
        print 'Total background = ', Histos['TTbar_'+btaglabel+'_'+Chan].Integral()+Histos['WJets_'+btaglabel+'_'+Chan].Integral()
    #print 'WJets ',HistosPre['WJets_'+Chan].Integral()
    print 'WJets ',Histos['WJets_'+btaglabel+'_'+Chan].Integral()
    print 'TTbar ',Histos['TTbar_'+btaglabel+'_'+Chan].Integral()
    if (UseQCD): print 'QCD ',Histos['QCD_'+btaglabel+'_'+Chan].Integral()
    print 'Data / Background = ', Histos['Data_'+btaglabel+ch].Integral()/(Histos['TTbar_'+btaglabel+'_'+Chan].Integral()+Histos['WJets_'+btaglabel+'_'+Chan].Integral())
                
    for sys in Systematics:
        suffix = ''
        if (sys == '_jsp_'): suffix = '__jes__plus'
        if (sys == '_jsm_'): suffix = '__jes__minus'
        if (sys == '_jrp_'): suffix = '__jer__plus'
        if (sys == '_jrm_'): suffix = '__jer__minus'
        if (sys == '_msp_'): suffix = '__mues__plus'
        if (sys == '_msm_'): suffix = '__mues__minus'
        if (sys == '_mrp_'): suffix = '__muer__plus'
        if (sys == '_mrm_'): suffix = '__muer__minus'
        if (sys == '_btp_'): suffix = '__btag__plus'
        if (sys == '_btm_'): suffix = '__btag__minus'
        if (sys == '_mtp_'): suffix = '__mtag__plus'
        if (sys == '_mtm_'): suffix = '__mtag__minus'
        if (sys == '_wjp_'): suffix = '__wjhf__plus'
        if (sys == '_wjm_'): suffix = '__wjhf__minus'
        if (sys == '_mup_'): suffix = '__murmuf__plus'
        if (sys == '_mum_'): suffix = '__murmuf__minus'
        if (sys == '_pup_'): suffix = '__pileup__plus'
        if (sys == '_pum_'): suffix = '__pileup__minus'
        if (sys == '_tpp_'): suffix = '__toppt__plus'
        if (sys == '_tpm_'): suffix = '__toppt__minus'
        if (sys == '_tqp_'): suffix = '__q2scale__plus'
        if (sys == '_tqm_'): suffix = '__q2scale__minus'
        if (wprime == 'Right'):
            masses = ['1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000']
        else:
            masses = ['800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2700','2800','2900','3000']
        
        for mass in masses:
            if (wprime != 'ModRight'):
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].Write()
            else:
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].Write()
                
    
List_DataBgEl_RightWprime = copy.copy(List_DataEl) 
List_DataBgEl_RightWprime.extend(List_Bg) 
List_DataBgEl_RightWprime.extend(List_Right) 

List_DataBgMu_RightWprime = copy.copy(List_DataMu) 
List_DataBgMu_RightWprime.extend(List_Bg) 
List_DataBgMu_RightWprime.extend(List_Right) 

btaglist = ['ge1btags','ex1btags','ex2btags']
#btaglist = ['ge1btags']
#btaglist = ['ex1btags','ex2btags']
chanlist = ['muon','electron']
wprimelist = ['Right']

save = 'LepPt'

scaleWJets = False

options = ['_Final']

for opt in options:
    loadHistos(opt)
    for btags in btaglist:
        for channel in chanlist:
            for wprime in wprimelist:
                f = TFile(indir+"RootFiles_ForSyst/"+channel+"_"+save+"_Wprime"+wprime+opt+"_Histos-"+btags+".root","RECREATE")
                f.cd() 
                if (channel=='electron'): makeHistos(channel, save, wprime, btags, List_DataBgEl_RightWprime)
                if (channel=='muon'): makeHistos(channel, save, wprime, btags, List_DataBgMu_RightWprime)
                print '-------------------------------------'
                print '^^^^',
                print opt,
                print '^^^^',
                print btags,
                print '^^^^',
                print channel,
                print '^^^^',
                print wprime,
                print '^^^^'
                print '-------------------------------------'
