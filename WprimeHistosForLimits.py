import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math

import os.path

indir = "Run2016_Feb27_Slim/"
#dirname = "RootFiles_For2DLimits"
dirname = "RootFiles_For2DLimits_Bare"
os.system('mkdir -p '+indir+dirname)

lumi = 1.0
REBIN = 2

######################################################
RootFiles = {}
Histos = {}
HistosPre = {}

FullSyst = ['_','_jsp_','_jsm_','_jrp_','_jrm_','_btp_','_btm_','_pup_','_pum_','_tpp_','_tpm_','_wjp_','_wjm_','_mup_','_mum_','_tqp_','_tqm_','_mtp_','_mtm_','_msp_','_msm_','_mrp_','_mrm_','_jpp_','_jpm_']
Backgrounds = ['WJets_Pt100to250','WJets_Pt250to400','WJets_Pt400to600','WJets_Pt600toInf','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','WW','WZ','ZZ']
#Backgrounds = ['WJets','TTbar']
masses = ['1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000','3100','3200','3300','3400','3500','3600','3700','3800','3900','4000']
Channels = ['El','Mu']
BTags = ['GE1BTags','Ex1BTags','Ex2BTags']
#Systematics = ['_','_jsp_','_jsm_','_jrp_','_jrm_','_btp_','_btm_','_pup_','_pum_','_tpp_','_tpm_','_wjp_','_wjm_','_mup_','_mum_','_tqp_','_tqm_','_mtp_','_mtm_']
#Systematics = ['_','_jsp_','_jsm_','_jrp_','_jrm_','_btp_','_btm_','_pup_','_pum_','_tpp_','_tpm_','_wjp_','_wjm_','_mup_','_mum_','_mtp_','_mtm_']
Systematics = ['_']
#Systematics = FullSyst


RootFiles['Base'] = TFile(indir+"outFile.root")
for chk in FullSyst:
    if (chk=='_'): continue
    if (os.path.isfile(indir+"outFile_"+chk.replace('_','')+".root")):
        RootFiles[chk.replace('_','')] = TFile(indir+"outFile_"+chk.replace('_','')+".root")
    else:
        RootFiles[chk.replace('_','')] = TFile(indir+"outFile.root")

def loadHistos(opt,hist = "mWprime_Limits"):

    for chan in Channels:
        for btag in BTags:
            if (btag == 'GE1BTags'): btaglabel = 'ge1b'
            if (btag == 'Ex1BTags'): btaglabel = 'ex1b'
            if (btag == 'Ex2BTags'): btaglabel = 'ex2b'

            if (chan=='El'): Histos['Data_'+btaglabel+'_el']  = RootFiles['Base'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Data'+'_'+chan)
            if (chan=='Mu'): Histos['Data_'+btaglabel+'_mu']  = RootFiles['Base'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Data'+'_'+chan)
    
            for bg in Backgrounds:
                Histos[bg+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jsp_'+btaglabel+'_'+chan] = RootFiles['jsp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jsm_'+btaglabel+'_'+chan] = RootFiles['jsm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jrp_'+btaglabel+'_'+chan] = RootFiles['jrp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jrm_'+btaglabel+'_'+chan] = RootFiles['jrm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jpp_'+btaglabel+'_'+chan] = RootFiles['jpp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_jpm_'+btaglabel+'_'+chan] = RootFiles['jpm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mup_'+btaglabel+'_'+chan] = RootFiles['mup'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mum_'+btaglabel+'_'+chan] = RootFiles['mum'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_pup_'+btaglabel+'_'+chan] = RootFiles['pup'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_pum_'+btaglabel+'_'+chan] = RootFiles['pum'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_tpp_'+btaglabel+'_'+chan] = RootFiles['tpp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_tpm_'+btaglabel+'_'+chan] = RootFiles['tpm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_btp_'+btaglabel+'_'+chan] = RootFiles['btp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_btm_'+btaglabel+'_'+chan] = RootFiles['btm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mtp_'+btaglabel+'_'+chan] = RootFiles['mtp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mtm_'+btaglabel+'_'+chan] = RootFiles['mtm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_wjp_'+btaglabel+'_'+chan] = RootFiles['wjp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_wjm_'+btaglabel+'_'+chan] = RootFiles['wjm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_msp_'+btaglabel+'_'+chan] = RootFiles['msp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_msm_'+btaglabel+'_'+chan] = RootFiles['msm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mrp_'+btaglabel+'_'+chan] = RootFiles['mrp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                Histos[bg+'_mrm_'+btaglabel+'_'+chan] = RootFiles['mrm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                if (bg=='TTbar'):
                    Histos[bg+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_scaleup_'+chan)
                    Histos[bg+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_scaledown_'+chan)
                else:
                    Histos[bg+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
                    Histos[bg+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+bg+'_'+chan)
           
                if (not bg.startswith('T') and not bg.startswith('QCD')):
                    HistosPre[bg+'_'+chan] = RootFiles['Base'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jsp_'+chan] = RootFiles['jsp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jsm_'+chan] = RootFiles['jsm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jrp_'+chan] = RootFiles['jrp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jrm_'+chan] = RootFiles['jrm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jpp_'+chan] = RootFiles['jpp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_jpm_'+chan] = RootFiles['jpm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mup_'+chan] = RootFiles['mup'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mum_'+chan] = RootFiles['mum'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_pup_'+chan] = RootFiles['pup'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_pum_'+chan] = RootFiles['pum'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tpp_'+chan] = RootFiles['tpp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tpm_'+chan] = RootFiles['tpm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_btp_'+chan] = RootFiles['btp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_btm_'+chan] = RootFiles['btm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mtp_'+chan] = RootFiles['mtp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mtm_'+chan] = RootFiles['mtm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_wjp_'+chan] = RootFiles['wjp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_wjm_'+chan] = RootFiles['wjm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_msp_'+chan] = RootFiles['msp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_msm_'+chan] = RootFiles['msm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_mrm_'+chan] = RootFiles['mrm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tqp_'+chan] = RootFiles['tqp'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
                    HistosPre[bg+'_tqm_'+chan] = RootFiles['tqm'].Get('GE0BTags'+opt+'/'+chan+'/'+hist+'_GE0BTags'+opt+'_'+bg+'_'+chan)
            
            for mass in masses:
                #for coup in ['Right']:
                for coup in ['Right','Left','Mix']:
            
                    Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jsp_'+btaglabel+'_'+chan] = RootFiles['jsp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jsm_'+btaglabel+'_'+chan] = RootFiles['jsm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jrp_'+btaglabel+'_'+chan] = RootFiles['jrp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jrm_'+btaglabel+'_'+chan] = RootFiles['jrm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jpp_'+btaglabel+'_'+chan] = RootFiles['jpp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_jpm_'+btaglabel+'_'+chan] = RootFiles['jpm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mup_'+btaglabel+'_'+chan] = RootFiles['mup'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mum_'+btaglabel+'_'+chan] = RootFiles['mum'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_pup_'+btaglabel+'_'+chan] = RootFiles['pup'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_pum_'+btaglabel+'_'+chan] = RootFiles['pum'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tpp_'+btaglabel+'_'+chan] = RootFiles['tpp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tpm_'+btaglabel+'_'+chan] = RootFiles['tpm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_btp_'+btaglabel+'_'+chan] = RootFiles['btp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_btm_'+btaglabel+'_'+chan] = RootFiles['btm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mtp_'+btaglabel+'_'+chan] = RootFiles['mtp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mtm_'+btaglabel+'_'+chan] = RootFiles['mtm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_wjp_'+btaglabel+'_'+chan] = RootFiles['wjp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_wjm_'+btaglabel+'_'+chan] = RootFiles['wjm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_msp_'+btaglabel+'_'+chan] = RootFiles['msp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_msm_'+btaglabel+'_'+chan] = RootFiles['msm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mrp_'+btaglabel+'_'+chan] = RootFiles['mrp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_mrm_'+btaglabel+'_'+chan] = RootFiles['mrm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tqp_'+btaglabel+'_'+chan] = RootFiles['tqp'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    Histos['Wprime'+mass+coup+'_tqm_'+btaglabel+'_'+chan] = RootFiles['tqm'].Get(btag+opt+'/'+chan+'/'+hist+'_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)

List_DataEl = ['Data_ex1b_el','Data_ex2b_el','Data_ge1b_el']
List_DataMu = ['Data_ex1b_mu','Data_ex2b_mu','Data_ge1b_mu']

#List_Bg = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
#List_Bg = ['WJets','TTbar']
List_Bg = Backgrounds


List_Right = []
List_Right.extend(['Wprime' + s + 'Right' for s in masses])

List_Mix = []
List_Mix.extend(['Wprime' + s + 'Mix' for s in masses])

List_Left = []
List_Left.extend(['Wprime' + s + 'Left' for s in masses])


def makeHistos(channel, save, wprime, btags, List_to_use, opt):


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

        #print Type

        ######################################### 
        ### Set 0 B.G. Bins to something !=0 
        #########################################

        if (not (Type.startswith('Data'))):
            for sys in Systematics:
                Histos[Type+sys+btaglabel+'_'+Chan].SetBinContent(Histos[Type+sys+btaglabel+'_'+Chan].GetNbinsX(), Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(Histos[Type+sys+btaglabel+'_'+Chan].GetNbinsX())+Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(Histos[Type+sys+btaglabel+'_'+Chan].GetNbinsX()+1))
                if ( not Type.startswith('Wprime') ):
                    for x in range(1,Histos[Type+sys+btaglabel+'_'+Chan].GetNbinsX()+1):
                        if (Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(x) < 0.000001 ): 
                            #print 'Setting ',Histos[Type+sys+btaglabel+'_'+Chan].GetBinContent(x),' to 10E-6 for bin ',x,' of ',Type
                            Histos[Type+sys+btaglabel+'_'+Chan].SetBinContent(x,0.000001)
    
                Histos[Type+sys+btaglabel+'_'+Chan].Scale(lumi)
    
                if (sys!='_'): continue
                print 'SCALED EVENTS FOR ',Type,sys,'  = ',Histos[Type+sys+btaglabel+'_'+Chan].Integral()
                if ( (not Type.startswith('T')) and (not Type.startswith('Wprime')) ):
                    if (scaleWJets):
                        HistosPre[Type+sys+Chan].Scale(lumi)
                        print 'Pre EVENTS FOR ',Type,sys,'  = ',HistosPre[Type+sys+Chan].Integral()          
    
                if ( (not Type.startswith('Data')) and (not Type.startswith('Wprime')) and (sys == '_')):
                    background = background + Histos[Type+sys+btaglabel+'_'+Chan].Integral()
        else:
            Histos['Data_'+btaglabel+'_'+channel[:2]].SetBinContent(Histos['Data_'+btaglabel+'_'+channel[:2]].GetNbinsX(), Histos['Data_'+btaglabel+'_'+channel[:2]].GetBinContent(Histos['Data_'+btaglabel+'_'+channel[:2]].GetNbinsX())+Histos['Data_'+btaglabel+'_'+channel[:2]].GetBinContent(Histos['Data_'+btaglabel+'_'+channel[:2]].GetNbinsX()+1))


    option = "invmass_"
    if (opt=='_Soft'):
        option = "soft_"
    if (opt=='_Hard'):
        option = "hard_"
    if (channel == 'electron'):
        chan = 'elec_'
        Histos['Data_'+btaglabel+'_el'].SetName(chan+option+btags+"__DATA")
        Histos['Data_'+btaglabel+'_el'].SetTitle(chan+option+btags+"__DATA")
        Histos['Data_'+btaglabel+'_el'].Rebin(REBIN)
        Histos['Data_'+btaglabel+'_el'].Write()
    if (channel == 'muon'):
        chan = 'mu_'
        Histos['Data_'+btaglabel+'_mu'].SetName(chan+option+btags+"__DATA")
        Histos['Data_'+btaglabel+'_mu'].SetTitle(chan+option+btags+"__DATA")
        Histos['Data_'+btaglabel+'_mu'].Rebin(REBIN)
        Histos['Data_'+btaglabel+'_mu'].Write()

    for sys in Systematics:
        suffix = ''
        if (sys == '_jsp_'): suffix = '__jes__plus'
        if (sys == '_jsm_'): suffix = '__jes__minus'
        if (sys == '_jrp_'): suffix = '__jer__plus'
        if (sys == '_jrm_'): suffix = '__jer__minus'
        if (sys == '_jpp_'): suffix = '__jetpt__plus'
        if (sys == '_jpm_'): suffix = '__jetpt__minus'
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

        Histos['TTbar'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__ttbar"+suffix)
        Histos['TTbar'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__ttbar"+suffix)
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
                Histos['T_s'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__topstb"+suffix)
                Histos['T_s'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__topstb"+suffix)
                Histos['T_s'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['T_s'+sys+btaglabel+'_'+Chan].Write()
            elif (wprime == 'Right'):
                if ('T_s' in Backgrounds): Histos['TTbar'+sys+btaglabel+'_'+Chan].Add(Histos['T_s'+sys+btaglabel+'_'+Chan])
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['TTbar'+sys+btaglabel+'_'+Chan].Write()

        Histos['WJets'+sys+btaglabel+'_'+Chan] = Histos['WJets_Pt100to250'+sys+btaglabel+'_'+Chan]
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_Pt250to400'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_Pt400to600'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WJets_Pt600toInf'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['ZJets'+sys+btaglabel+'_'+Chan])
        if ('WW' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WW'+sys+btaglabel+'_'+Chan])
        if ('WZ' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['WZ'+sys+btaglabel+'_'+Chan])
        if ('ZZ' in Backgrounds): Histos['WJets'+sys+btaglabel+'_'+Chan].Add(Histos['ZZ'+sys+btaglabel+'_'+Chan])
        Histos['WJets'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__wjets"+suffix)
        Histos['WJets'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__wjets"+suffix)
        if (scaleWJets) :
            HistosPre['WJets'+sys+Chan] = HistosPre['WJets_Pt100to250'+sys+Chan]
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_Pt250to400'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_Pt400to600'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['WJets_Pt600toInf'+sys+Chan])
            HistosPre['WJets'+sys+Chan].Add(HistosPre['ZJets'+sys+Chan])
            if ('WW' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['WW'+sys+Chan])
            if ('WZ' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['WZ'+sys+Chan])
            if ('ZZ' in Backgrounds): HistosPre['WJets'+sys+Chan].Add(HistosPre['ZZ'+sys+Chan])
            HistosPre['WJets'+sys+Chan].SetName(chan+option+btags+"__wjets"+suffix)
            HistosPre['WJets'+sys+Chan].SetTitle(chan+option+btags+"__wjets"+suffix)
    
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
        #Histos['WJets'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__ge1bwjets"+suffix)
        #Histos['WJets'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__ge1bwjets"+suffix)
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
            Histos['QCD'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__qcd"+suffix)
            Histos['QCD'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__qcd"+suffix)
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
        if (sys == '_jpp_'): suffix = '__jetpt__plus'
        if (sys == '_jpm_'): suffix = '__jetpt__minus'
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
            masses = ['1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000','3100','3200','3300','3400','3500','3600','3700','3800','3900','4000']
        else:
            masses = ['1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000','3100','3200','3300','3400','3500','3600','3700','3800','3900','4000']
        
        for mass in masses:
            if (wprime != 'ModRight'):
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['Wprime' + mass + wprime+sys+btaglabel+'_'+Chan].Write()
            else:
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].SetName(chan+option+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].SetTitle(chan+option+btags+"__wp"+mass+suffix)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].Rebin(REBIN)
                Histos['Wprime' + mass + 'Right'+sys+btaglabel+'_'+Chan].Write()
                
    
List_DataBgEl_RightWprime = copy.copy(List_DataEl) 
List_DataBgEl_RightWprime.extend(List_Bg) 
List_DataBgEl_RightWprime.extend(List_Right) 

List_DataBgMu_RightWprime = copy.copy(List_DataMu) 
List_DataBgMu_RightWprime.extend(List_Bg) 
List_DataBgMu_RightWprime.extend(List_Right) 

List_DataBgEl_MixWprime = copy.copy(List_DataEl) 
List_DataBgEl_MixWprime.extend(List_Bg) 
List_DataBgEl_MixWprime.extend(List_Mix) 

List_DataBgMu_MixWprime = copy.copy(List_DataMu) 
List_DataBgMu_MixWprime.extend(List_Bg) 
List_DataBgMu_MixWprime.extend(List_Mix) 

List_DataBgEl_LeftWprime = copy.copy(List_DataEl) 
List_DataBgEl_LeftWprime.extend(List_Bg) 
List_DataBgEl_LeftWprime.extend(List_Left) 

List_DataBgMu_LeftWprime = copy.copy(List_DataMu) 
List_DataBgMu_LeftWprime.extend(List_Bg) 
List_DataBgMu_LeftWprime.extend(List_Left) 

btaglist = ['ge1btags','ex1btags','ex2btags']
#btaglist = ['ge1btags']
#btaglist = ['ex1btags','ex2btags']
chanlist = ['muon','electron']
wprimelist = ['Right','Left','Mix','ModRight']
#wprimelist = ['Right']

#savelist = ['mWprime_Limits','mWprime_wB']
savelist = ['mWprime_Limits']

scaleWJets = False

#options = ['_Final']
options = ['_Final','_Hard','_Soft']

for opt in options:
    for save in savelist:
        for btags in btaglist:
            for channel in chanlist:
                for wprime in wprimelist:
                    loadHistos(opt,save)
                    f = TFile(indir+dirname+"/"+channel+"_"+save+"_Wprime"+wprime+opt+"_Histos-"+btags+".root","RECREATE")
                    f.cd() 
                    if (channel=='electron'):
                        if(wprime=='Right' or wprime=='ModRight'): makeHistos(channel, save, wprime, btags, List_DataBgEl_RightWprime, opt)
                        if(wprime=='Mix'): makeHistos(channel, save, wprime, btags, List_DataBgEl_MixWprime, opt)
                        if(wprime=='Left'): makeHistos(channel, save, wprime, btags, List_DataBgEl_LeftWprime, opt)
                    if (channel=='muon'):
                        if(wprime=='Right' or wprime=='ModRight'): makeHistos(channel, save, wprime, btags, List_DataBgMu_RightWprime, opt)
                        if(wprime=='Mix'): makeHistos(channel, save, wprime, btags, List_DataBgMu_MixWprime, opt)
                        if(wprime=='Left'): makeHistos(channel, save, wprime, btags, List_DataBgMu_LeftWprime, opt)
                    print '-------------------------------------'
                    print '^^^^',
                    print opt,
                    print '^^^^',
                    print save,
                    print '^^^^',
                    print btags,
                    print '^^^^',
                    print channel,
                    print '^^^^',
                    print wprime,
                    print '^^^^'
                    print '-------------------------------------'
