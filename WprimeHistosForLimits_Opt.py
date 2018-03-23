import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math


indir = "Run2015D_RefOpt/"
lumi = 1.0/1.264

######################################################
RootFiles = {}
Histos = {}
HistosPre = {}

#Backgrounds = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
Backgrounds = ['WJets_HT100to200','WJets_HT200to400','WJets_HT400to600','WJets_HT600toInf','TTbar']
#Backgrounds = ['WJets_HT100to200','WJets_HT200to400','WJets_HT400to600','WJets_HT600toInf','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s','QCD_HT250to500','QCD_HT500to1000','QCD_HT1000toInf']
#Backgrounds = ['WJets','TTbar']
Channels = ['El','Mu']
BTags = ['GE1BTags']
RootFiles['Base'] = TFile(indir+"outFile.root")
#RootFiles['JESUP'] = TFile(indir+"JESUP/"+bg+"_JESUP.root")
#RootFiles['JESDOWN'] = TFile(indir+"JESDOWN/"+bg+"_JESDOWN.root")
#RootFiles['JERUP'] = TFile(indir+"JERUP/"+bg+"_JERUP.root")
#RootFiles['JERDOWN'] = TFile(indir+"JERDOWN/"+bg+"_JERDOWN.root")
#RootFiles['BTAGUP'] = TFile(indir+"BTAGUP/"+bg+"_BTAGUP.root")
#RootFiles['BTAGDOWN'] = TFile(indir+"BTAGDOWN/"+bg+"_BTAGDOWN.root")

def loadHistos(opt):

    #Histos['Data_ge1b_el']  = RootFiles['Data_el'].Get('GE1BTags'+opt+'/'+chan+'/mWprime_Limits_GE1BTags'+opt'_'+'Data'+'_'+chan)
    #Histos['Data_ge1b_mu']  = RootFiles['Data_mu'].Get('GE1BTags'+opt+'/'+chan+'/mWprime_Limits_GE1BTags'+opt'_'+'Data'+'_'+chan)
    #Histos['Data_ex1b_el']  = RootFiles['Data_el'].Get('Ex1BTags'+opt+'/'+chan+'/mWprime_Limits_Ex1BTags'+opt'_'+'Data'+'_'+chan)
    #Histos['Data_ex1b_mu']  = RootFiles['Data_mu'].Get('Ex1BTags'+opt+'/'+chan+'/mWprime_Limits_Ex1BTags'+opt'_'+'Data'+'_'+chan)
    #Histos['Data_ex2b_el']  = RootFiles['Data_el'].Get('Ex2BTags'+opt+'/'+chan+'/mWprime_Limits_Ex2BTags'+opt'_'+'Data'+'_'+chan)
    #Histos['Data_ex2b_mu']  = RootFiles['Data_mu'].Get('Ex2BTags'+opt+'/'+chan+'/mWprime_Limits_Ex2BTags'+opt'_'+'Data'+'_'+chan)
    
    for chan in Channels:
        for btag in BTags:
            if (btag == 'GE1BTags'): btaglabel = 'ge1b'
            if (btag == 'Ex1BTags'): btaglabel = 'ex1b'
            if (btag == 'Ex2BTags'): btaglabel = 'ex2b'
            for bg in Backgrounds:
                Histos[bg+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+bg+'_'+chan)
           
                if (not bg.startswith('T')):
                    HistosPre[bg+'_'+chan] = RootFiles['Base'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get('GE0BTags'+opt+'/'+chan+'/mWprime_Limits_GE0BTags'+opt+'_'+bg+'_'+chan)
            
                #RootFiles['TTbar_'+btaglabel+'_MATCHINGUP'] = TFile(indir+"TTbar_matchingup.root")
                #RootFiles['TTbar_'+btaglabel+'_MATCHINGDOWN'] = TFile(indir+"TTbar_matchingdown.root")
                #RootFiles['TTbar_'+btaglabel+'_SCALEUP'] = TFile(indir+"TTbar_scaleup.root")
                #RootFiles['TTbar_'+btaglabel+'_SCALEDOWN'] = TFile(indir+"TTbar_scaledown.root")
            
            #Histos['TTbar_'+btaglabel+'_'+chan+'_MATCHINGUP'] = RootFiles['TTbar_MATCHINGUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_MATCHINGDOWN'] = RootFiles['TTbar_MATCHINGDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_SCALEUP'] = RootFiles['TTbar_SCALEUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_SCALEDOWN'] = RootFiles['TTbar_SCALEDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'TTbar_'+chan)
            
            #masses = ['800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000']
            masses = ['2000','2500','3000']
            
            for mass in masses:
                #for coup in ['Right','Left','Mix']:
                for coup in ['Right']:
            
                    Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan] = RootFiles['Base'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get(btag+opt+'/'+chan+'/mWprime_Limits_'+btag+opt+'_'+'Wprime'+mass+coup+'_'+chan)

#List_DataEl = ['Data_ge1b_el']
#List_DataMu = ['Data_ge1b_mu']
List_DataEl = []
List_DataMu = []

#List_Bg = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
#List_Bg = ['WJets','TTbar']
List_Bg = Backgrounds


List_Right = ['Wprime2000Right','Wprime2500Right','Wprime3000Right'
] 


def makeHistos(channel, save, wprime, btags, List_to_use):


    print wprime

    UseQCD = False
 
    if (btags == 'ge1btags'): btaglabel = 'ge1b'
    if (btags == 'ex1btags'): btaglabel = 'ex1b'
    if (btags == 'ex2btags'): btaglabel = 'ex2b'

    List = List_to_use
    
    background = 0

    for Type in List:
    
        if (channel == 'electron'):
            prefix = 'elec_invmass_' + btags + '__'
            Chan = 'El'
        if (channel == 'muon'):
            prefix = 'mu_invmass_' + btags + '__'
            Chan = 'Mu'

        suffix = ''
        
        if (Type=='Data_el' or Type=='Data_mu'): suffix = 'DATA' + Type
        if (Type.startswith('WJets')): suffix = 'wjets' + Type
        if (Type=='WW'): suffix = 'scaledntb' + Type
        if (Type=='ZJets' or Type=='WW' or Type=='T_t' or Type=='Tbar_t' or Type=='T_tW' or Type=='Tbar_tW'): suffix = 'scaledntb' + Type
        if (Type=='TTbar'): suffix = 'ttbar' + Type
        if (Type=='T_s' or Type=='Tbar_s'): suffix = 'tb' + Type
        if (Type.startswith('QCD')): suffix = 'qcd' + Type

        w_suffix = 'wp'

        if (wprime != 'ModRight'):
            if (Type.startswith('Wprime800' + wprime)): suffix = w_suffix+'800'
            if (Type.startswith('Wprime900' + wprime)): suffix = w_suffix+'900'
            if (Type.startswith('Wprime1000' + wprime)): suffix = w_suffix+'1000'
            if (Type.startswith('Wprime1100' + wprime)): suffix = w_suffix+'1100'
            if (Type.startswith('Wprime1200' + wprime)): suffix = w_suffix+'1200'
            if (Type.startswith('Wprime1300' + wprime)): suffix = w_suffix+'1300'
            if (Type.startswith('Wprime1400' + wprime)): suffix = w_suffix+'1400'
            if (Type.startswith('Wprime1500' + wprime)): suffix = w_suffix+'1500'
            if (Type.startswith('Wprime1600' + wprime)): suffix = w_suffix+'1600'
            if (Type.startswith('Wprime1700' + wprime)): suffix = w_suffix+'1700'
            if (Type.startswith('Wprime1800' + wprime)): suffix = w_suffix+'1800'
            if (Type.startswith('Wprime1900' + wprime)): suffix = w_suffix+'1900'
            if (Type.startswith('Wprime2000' + wprime)): suffix = w_suffix+'2000'
            if (Type.startswith('Wprime2100' + wprime)): suffix = w_suffix+'2100'
            if (Type.startswith('Wprime2200' + wprime)): suffix = w_suffix+'2200'
            if (Type.startswith('Wprime2300' + wprime)): suffix = w_suffix+'2300'
            if (Type.startswith('Wprime2400' + wprime)): suffix = w_suffix+'2400'
            if (Type.startswith('Wprime2500' + wprime)): suffix = w_suffix+'2500'
            if (Type.startswith('Wprime2600' + wprime)): suffix = w_suffix+'2600'
            if (Type.startswith('Wprime2700' + wprime)): suffix = w_suffix+'2700'
            if (Type.startswith('Wprime2800' + wprime)): suffix = w_suffix+'2800'
            if (Type.startswith('Wprime2900' + wprime)): suffix = w_suffix+'2900'
            if (Type.startswith('Wprime3000' + wprime)): suffix = w_suffix+'3000'

            if (Type.endswith('_JESUP')): suffix += '__jes__plus'
            if (Type.endswith('_JESDOWN')): suffix += '__jes__minus'
            if (Type.endswith('_JERUP')): suffix += '__jer__plus'
            if (Type.endswith('_JERDOWN')): suffix += '__jer__minus'
            if (Type.endswith('_BTAGUP')): suffix += '__btag__plus'
            if (Type.endswith('_BTAGDOWN')): suffix += '__btag__minus'

        else:

            if (Type.startswith('Wprime800Right')): suffix = w_suffix+'800'
            if (Type.startswith('Wprime900Right')): suffix = w_suffix+'900'
            if (Type.startswith('Wprime1000Right')): suffix = w_suffix+'1000'
            if (Type.startswith('Wprime1100Right')): suffix = w_suffix+'1100'
            if (Type.startswith('Wprime1200Right')): suffix = w_suffix+'1200'
            if (Type.startswith('Wprime1300Right')): suffix = w_suffix+'1300'
            if (Type.startswith('Wprime1400Right')): suffix = w_suffix+'1400'
            if (Type.startswith('Wprime1500Right')): suffix = w_suffix+'1500'
            if (Type.startswith('Wprime1600Right')): suffix = w_suffix+'1600'
            if (Type.startswith('Wprime1700Right')): suffix = w_suffix+'1700'
            if (Type.startswith('Wprime1800Right')): suffix = w_suffix+'1800'
            if (Type.startswith('Wprime1900Right')): suffix = w_suffix+'1900'
            if (Type.startswith('Wprime2000Right')): suffix = w_suffix+'2000'
            if (Type.startswith('Wprime2100Right')): suffix = w_suffix+'2100'
            if (Type.startswith('Wprime2200Right')): suffix = w_suffix+'2200'
            if (Type.startswith('Wprime2300Right')): suffix = w_suffix+'2300'
            if (Type.startswith('Wprime2400Right')): suffix = w_suffix+'2400'
            if (Type.startswith('Wprime2500Right')): suffix = w_suffix+'2500'
            if (Type.startswith('Wprime2600Right')): suffix = w_suffix+'2600'
            if (Type.startswith('Wprime2700Right')): suffix = w_suffix+'2700'
            if (Type.startswith('Wprime2800Right')): suffix = w_suffix+'2800'
            if (Type.startswith('Wprime2900Right')): suffix = w_suffix+'2900'
            if (Type.startswith('Wprime3000Right')): suffix = w_suffix+'3000'

            if (Type.endswith('_JESUP')): suffix += '__jes__plus'
            if (Type.endswith('_JESDOWN')): suffix += '__jes__minus'
            if (Type.endswith('_JERUP')): suffix += '__jer__plus'
            if (Type.endswith('_JERDOWN')): suffix += '__jer__minus'
            if (Type.endswith('_BTAGUP')): suffix += '__btag__plus'
            if (Type.endswith('_BTAGDOWN')): suffix += '__btag__minus'

      
        #if (Type == 'WJets'):
            #WccHist = TH1D('WccHist', 'WccHist', bin,array('d',xlow))
            #WbbHist = TH1D('WbbHist', 'WbbHist', bin,array('d',xlow))
            #WccHist.Sumw2()
            #WbbHist.Sumw2()
            #WccHistPre = TH1D('WccHistPre', 'WccHistPre', bin,array('d',xlow))
            #WbbHistPre = TH1D('WbbHistPre', 'WbbHistPre', bin,array('d',xlow))
            #WccHistPre.Sumw2()
            #WbbHistPre.Sumw2()

        print Type

        ######################################### 
        ### Set 0 B.G. Bins to something !=0 
        #########################################

        if (not (Type.startswith('Data'))):
            if ( not Type.startswith('Wprime') ):
                for x in range(1,Histos[Type+'_'+btaglabel+'_'+Chan].GetNbinsX()+1):
                    if (Histos[Type+'_'+btaglabel+'_'+Chan].GetBinContent(x) < 0.000001 ): 
                        #print 'Setting ',Histos[Type+'_'+btaglabel+'_'+Chan].GetBinContent(x),' to 10E-6 for bin ',x,' of ',Type
                        Histos[Type+'_'+btaglabel+'_'+Chan].SetBinContent(x,0.000001)

            scalefortau = False
            if (scalefortau and Type.startswith('Wprime')): Histos[Type+'_'+btaglabel+'_'+Chan].Scale((2.*1.116)/3.)

            Histos[Type+'_'+btaglabel+'_'+Chan].Scale(lumi)

            print 'SCALED EVENTS FOR ',Type,'  = ',Histos[Type+'_'+btaglabel+'_'+Chan].Integral()
            if ( (not Type.startswith('T')) and (not Type.startswith('Wprime')) ):
                if (scaleWJets):
                    HistosPre[Type+'_'+Chan].Scale(lumi)
                    print 'Pre EVENTS FOR ',Type,'  = ',HistosPre[Type+'_'+Chan].Integral()          

            if ( (not Type.startswith('Data')) and (not Type.startswith('Wprime')) and (not Type.endswith('UP')) and (not Type.endswith('Down'))):
                background = background + Histos[Type+'_'+btaglabel+'_'+Chan].Integral()


    if (channel == 'electron'):
        chan = 'elec_'
        #Histos['Data_'+btaglabel+'_el'].SetName(chan+"invmass_"+btags+"__DATA")
        #Histos['Data_'+btaglabel+'_el'].SetTitle(chan+"invmass_"+btags+"__DATA")
        #Histos['Data_'+btaglabel+'_el'].Write()
    if (channel == 'muon'):
        chan = 'mu_'
        #Histos['Data_'+btaglabel+'_mu'].SetName(chan+"invmass_"+btags+"__DATA")
        #Histos['Data_'+btaglabel+'_mu'].SetTitle(chan+"invmass_"+btags+"__DATA")
        #Histos['Data_'+btaglabel+'_mu'].Write()

    Histos['WJets'+'_'+btaglabel+'_'+Chan] = Histos['WJets_HT100to200'+'_'+btaglabel+'_'+Chan]
    Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['WJets_HT200to400'+'_'+btaglabel+'_'+Chan])
    Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['WJets_HT400to600'+'_'+btaglabel+'_'+Chan])
    Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['WJets_HT600toInf'+'_'+btaglabel+'_'+Chan])
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['ZJets_HT100to200'+'_'+btaglabel+'_'+Chan])
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['ZJets_HT200to400'+'_'+btaglabel+'_'+Chan])
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['ZJets_HT400to600'+'_'+btaglabel+'_'+Chan])
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['ZJets_HT600toInf'+'_'+btaglabel+'_'+Chan])
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].Add(Histos['ZJets'+'_'+btaglabel+'_'+Chan])
    Histos['WJets'+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wjets")
    Histos['WJets'+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wjets")
    if (scaleWJets) :
        HistosPre['WJets'+'_'+Chan] = HistosPre['WJets_HT100to200'+'_'+Chan]
        HistosPre['WJets'+'_'+Chan].Add(HistosPre['WJets_HT200to400'+'_'+Chan])
        HistosPre['WJets'+'_'+Chan].Add(HistosPre['WJets_HT400to600'+'_'+Chan])
        HistosPre['WJets'+'_'+Chan].Add(HistosPre['WJets_HT600toInf'+'_'+Chan])
        #HistosPre['WJets'+'_'+Chan].Add(HistosPre['ZJets_HT100to200'+'_'+Chan])
        #HistosPre['WJets'+'_'+Chan].Add(HistosPre['ZJets_HT200to400'+'_'+Chan])
        #HistosPre['WJets'+'_'+Chan].Add(HistosPre['ZJets_HT400to600'+'_'+Chan])
        #HistosPre['WJets'+'_'+Chan].Add(HistosPre['ZJets_HT600toInf'+'_'+Chan])
        #HistosPre['WJets'+'_'+Chan].Add(HistosPre['ZJets'+'_'+Chan])
        HistosPre['WJets'+'_'+Chan].SetName(chan+"invmass_"+btags+"__wjets")
        HistosPre['WJets'+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wjets")

    ######################################### 
    ### RESCALE W+Jets    
    #########################################
    #print 'WJets ge1/pre: ',Histos['WJets'+'_'+btaglabel+'_'+Chan].Integral()/HistosPre['WJets'+'_'+Chan].Integral()
    if (scaleWJets):
        HistosPre['WJets'+'_'+Chan].Scale( Histos['WJets'+'_'+btaglabel+'_'+Chan].Integral()/HistosPre['WJets'+'_'+Chan].Integral() )
    else:
        HistosPre['WJets'+'_'+Chan] = Histos['WJets'+'_'+btaglabel+'_'+Chan]
    #print 'WJets after rescaling: ',HistosPre['WJets'+'_'+Chan].Integral()
    #HistosPre['WJets'+'_'+Chan].Write()
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__ge1bwjets")
    #Histos['WJets'+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__ge1bwjets")
    Histos['WJets'+'_'+btaglabel+'_'+Chan].Write()

    Histos['TTbar'+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__ttbar")
    Histos['TTbar'+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__ttbar")
    #Histos['T_t'+'_'+btaglabel+'_'+Chan].Add(Histos['Tbar_t'+'_'+btaglabel+'_'+Chan])
    #Histos['T_t'+'_'+btaglabel+'_'+Chan].Add(Histos['T_tW'+'_'+btaglabel+'_'+Chan])
    #Histos['T_t'+'_'+btaglabel+'_'+Chan].Add(Histos['Tbar_tW'+'_'+btaglabel+'_'+Chan])
    #Histos['TTbar'+'_'+btaglabel+'_'+Chan].Add( Histos['T_t'+'_'+btaglabel+'_'+Chan])

    #Histos['T_s'+'_'+btaglabel+'_'+Chan].Add(Histos['Tbar_s'+'_'+btaglabel+'_'+Chan]) 
   
    if (wprime == 'Left' or wprime == 'Mix'): 
        Histos['TTbar'+'_'+btaglabel+'_'+Chan].Write()
    else:
        if ( wprime == 'ModRight' ):
            Histos['TTbar'+'_'+btaglabel+'_'+Chan].Write()
            Histos['T_s'+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__topstb")
            Histos['T_s'+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__topstb")
            Histos['T_s'+'_'+btaglabel+'_'+Chan].Write()
        elif (wprime == 'Right'):
            #Histos['TTbar'+'_'+btaglabel+'_'+Chan].Add(Histos['T_s'+'_'+btaglabel+'_'+Chan])
            Histos['TTbar'+'_'+btaglabel+'_'+Chan].Write()

    if (UseQCD):
        Histos['QCD'+'_'+btaglabel+'_'+Chan] = Histos['QCD_HT250to500'+'_'+btaglabel+'_'+Chan]
        Histos['QCD'+'_'+btaglabel+'_'+Chan].Add(Histos['QCD_HT500to1000'+'_'+btaglabel+'_'+Chan])
        Histos['QCD'+'_'+btaglabel+'_'+Chan].Add(Histos['QCD_HT1000toInf'+'_'+btaglabel+'_'+Chan])
        Histos['QCD'+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__qcd")
        Histos['QCD'+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__qcd")
        Histos['QCD'+'_'+btaglabel+'_'+Chan].Write()

    if (channel == 'electron'): ch = '_el' 
    if (channel == 'muon'): ch = '_mu' 

    #print channel,' data = ', Histos['Data_'+btaglabel+ch].Integral()
    if (UseQCD):
        #print 'Total background = ', Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()+HistosPre['WJets'+'_'+Chan].Integral()+Histos['QCD'+'_'+btaglabel+'_'+Chan].Integral()
        print 'Total background = ', Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()+Histos['WJets'+'_'+btaglabel+'_'+Chan].Integral()+Histos['QCD'+'_'+btaglabel+'_'+Chan].Integral()
    else:
        #print 'Total background = ', Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()+HistosPre['WJets'+'_'+Chan].Integral()
        print 'Total background = ', Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()+Histos['WJets'+'_'+btaglabel+'_'+Chan].Integral()
    #print 'WJets ',HistosPre['WJets'+'_'+Chan].Integral()
    print 'WJets ',Histos['WJets'+'_'+btaglabel+'_'+Chan].Integral()
    print 'TTbar ',Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()
    if (UseQCD): print 'QCD ',Histos['QCD'+'_'+btaglabel+'_'+Chan].Integral()
    #print 'Background / Data = ', (Histos['TTbar'+'_'+btaglabel+'_'+Chan].Integral()+HistosPre['WJets'+'_'+Chan].Integral())/Histos['Data_'+btaglabel+ch].Integral()
                
    if (wprime == 'Right'):
        #masses = ['800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000']
        masses = ['2000','2500','3000']
    else:
        masses = ['800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2700','2800','2900']
    
    for mass in masses:
        if (wprime != 'ModRight'):
            Histos['Wprime' + mass + wprime+'_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wp"+mass)
            Histos['Wprime' + mass + wprime+'_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wp"+mass)
            Histos['Wprime' + mass + wprime+'_'+btaglabel+'_'+Chan].Write()
        else:
            Histos['Wprime' + mass + 'Right_'+btaglabel+'_'+Chan].SetName(chan+"invmass_"+btags+"__wp"+mass)
            Histos['Wprime' + mass + 'Right_'+btaglabel+'_'+Chan].SetTitle(chan+"invmass_"+btags+"__wp"+mass)
            Histos['Wprime' + mass + 'Right_'+btaglabel+'_'+Chan].Write()
            
    
  
List_DataBgEl_RightWprime = copy.copy(List_DataEl) 
List_DataBgEl_RightWprime.extend(List_Bg) 
List_DataBgEl_RightWprime.extend(List_Right) 

List_DataBgMu_RightWprime = copy.copy(List_DataMu) 
List_DataBgMu_RightWprime.extend(List_Bg) 
List_DataBgMu_RightWprime.extend(List_Right) 

#btaglist = ['ge1btags','ex1btags','ex2btags']
btaglist = ['ge1btags']
#btaglist = ['ex1btags','ex2btags']
chanlist = ['muon','electron']
wprimelist = ['Right']

save = 'mWprime_Limits'

'''ptopopt = ['100','150','250']
mtopopt = ['none','100250','130210']
pjjopt = ['250','300','350']

options = []
for j in ptopopt:
    for k in mtopopt:
        for l in pjjopt:
            options.append('_TopPt'+j+'TopM'+k+'JetJet'+l)'''

jet1opt = ['250','350','450']
lepopt = ['120','150','180']
metopt = ['50','100','150']

options = []
for i in jet1opt:
    for j in lepopt:
        for k in metopt:
            options.append('_Jet'+i+'Lep'+j+'MET'+k)

scaleWJets = False

#options = ['_Final']

for opt in options:
    loadHistos(opt)
    for btags in btaglist:
        for channel in chanlist:
            for wprime in wprimelist:
                f = TFile(indir+"RootFiles_For2DLimits/"+channel+"_"+save+"_Wprime"+wprime+opt+"_Histos-"+btags+".root","RECREATE")
                f.cd() 
                makeHistos(channel, save, wprime, btags, List_DataBgEl_RightWprime)
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
