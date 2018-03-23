import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math
import decimal


indir = "Run2016_Feb20_Slim/"

######################################################
RootFiles = {}
Histos = {}
HistosPre = {}

#Backgrounds = ['WJets','W1Jets','W2Jets','W3Jets','W4Jets','WW','TTbar_Madgraph','TTbar_Powheg','ZJets_M50','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s','QCD_Pt_80_170_EM','QCD_Pt_170_250_EM','QCD_Pt_250_350_EM','QCD_Pt_350_EM']
#Backgrounds = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
#Backgrounds = ['WJets_HT100to200','WJets_HT200to400','WJets_HT400to600','WJets_HT600toInf','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s','QCD_HT250to500','QCD_HT500to1000','QCD_HT1000toInf']
Backgrounds = ['WJets_Pt100to250','WJets_Pt250to400','WJets_Pt400to600','WJets_Pt600toInf','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','WW','WZ','ZZ']
#Backgrounds = ['WJets','TTbar']
Channels = ['El','Mu']
BTags = ['Ex1BTags','Ex2BTags']
Options = ['Soft','Hard']
RootFiles['Base'] = TFile(indir+"outFile.root")
#RootFiles['JESUP'] = TFile(indir+"JESUP/"+bg+"_JESUP.root")
#RootFiles['JESDOWN'] = TFile(indir+"JESDOWN/"+bg+"_JESDOWN.root")
#RootFiles['JERUP'] = TFile(indir+"JERUP/"+bg+"_JERUP.root")
#RootFiles['JERDOWN'] = TFile(indir+"JERDOWN/"+bg+"_JERDOWN.root")
#RootFiles['BTAGUP'] = TFile(indir+"BTAGUP/"+bg+"_BTAGUP.root")
#RootFiles['BTAGDOWN'] = TFile(indir+"BTAGDOWN/"+bg+"_BTAGDOWN.root")

#Histos['Data_ge1b_el']  = RootFiles['Data_el'].Get('GE1BTags_'+opt+'/'+chan+'/mWprime_GE1BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)
#Histos['Data_ge1b_mu']  = RootFiles['Data_mu'].Get('GE1BTags_'+opt+'/'+chan+'/mWprime_GE1BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)
#Histos['Data_ex1b_el']  = RootFiles['Data_el'].Get('Ex1BTags_'+opt+'/'+chan+'/mWprime_Ex1BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)
#Histos['Data_ex1b_mu']  = RootFiles['Data_mu'].Get('Ex1BTags_'+opt+'/'+chan+'/mWprime_Ex1BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)
#Histos['Data_ex2b_el']  = RootFiles['Data_el'].Get('Ex2BTags_'+opt+'/'+chan+'/mWprime_Ex2BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)
#Histos['Data_ex2b_mu']  = RootFiles['Data_mu'].Get('Ex2BTags_'+opt+'/'+chan+'/mWprime_Ex2BTags_'+opt+'_Wprime'+mass+coup+'_'+chan)

for chan in Channels:
    for btag in BTags:
        for opt in Options:
            if (btag == 'GE1BTags'): btaglabel = 'ge1b'
            if (btag == 'Ex1BTags'): btaglabel = 'ex1b'
            if (btag == 'Ex2BTags'): btaglabel = 'ex2b'
            for bg in Backgrounds:
            
                Histos[bg+'_'+btaglabel+'_'+chan+'_'+opt] = RootFiles['Base'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
                #Histos[bg+'_'+btaglabel+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_'+bg+'_'+chan)
           
                if (not bg.startswith('T')):
                    HistosPre[bg+'_'+chan+'_'+opt] = RootFiles['Base'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
                    #HistosPre[bg+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get('GE0BTags_'+opt+'/'+chan+'/mWprime_GE0BTags_'+opt+'_'+bg+'_'+chan)
            
                #RootFiles['TTbar_'+btaglabel+'_MATCHINGUP'] = TFile(indir+"TTbar_matchingup.root")
                #RootFiles['TTbar_'+btaglabel+'_MATCHINGDOWN'] = TFile(indir+"TTbar_matchingdown.root")
                #RootFiles['TTbar_'+btaglabel+'_SCALEUP'] = TFile(indir+"TTbar_scaleup.root")
                #RootFiles['TTbar_'+btaglabel+'_SCALEDOWN'] = TFile(indir+"TTbar_scaledown.root")
            
            #Histos['TTbar_'+btaglabel+'_'+chan+'_MATCHINGUP'] = RootFiles['TTbar_MATCHINGUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_MATCHINGDOWN'] = RootFiles['TTbar_MATCHINGDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_SCALEUP'] = RootFiles['TTbar_SCALEUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_TTbar_'+chan)
            #Histos['TTbar_'+btaglabel+'_'+chan+'_SCALEDOWN'] = RootFiles['TTbar_SCALEDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_TTbar_'+chan)
            
            #masses = ['800','900','1000','1100','1200','1300','1400','1500','1600','1700','1800','1900','2000','2100','2200','2300','2400','2500','2600','2700','2800','2900','3000']
            masses = ['2500','3000','3500']
            
            for mass in masses:
                #for coup in ['Right','Left','Mix']:
                for coup in ['Right']:
            
                    Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_'+opt] = RootFiles['Base'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JESUP'] = RootFiles['JESUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JESDOWN'] = RootFiles['JESDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JERUP'] = RootFiles['JERUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_JERDOWN'] = RootFiles['JERDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_BTAGUP'] = RootFiles['BTAGUP'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)
                    #Histos['Wprime'+mass+coup+'_'+btaglabel+'_'+chan+'_BTAGDOWN'] = RootFiles['BTAGDOWN'].Get(btag+'_'+opt+'/'+chan+'/mWprime_'+btag+'_'+opt+'_Wprime'+mass+coup+'_'+chan)

#List_DataEl = ['Data_ge1b_el']
#List_DataMu = ['Data_ge1b_mu']
List_DataEl = []
List_DataMu = []

#List_Bg = ['WJets','TTbar','ZJets','T_t','Tbar_t','T_tW','Tbar_tW','T_s','Tbar_s']
#List_Bg = ['WJets','TTbar']
List_Bg = Backgrounds


List_Right = ['Wprime2500Right','Wprime3000Right','Wprime3500Right']
List_Sig = List_Right 

minbkg = 0.0000001
maxerrbkg = 0.3  

global binfile
binfile = open('binning.txt', 'w')

def Rebin(btaglabel,chan,opt):

    wprime='wp3000'

    # This is the starting point for the binning search (smart choices speed up the initial bg search)
    #xlowbg  = array('d',[0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700,1750,1800,1850,1900,1950,2000,2050,2100,2150,2200,2250,2300,2350,2400,2450,2500,2550,2600,2650,2700,2750,2800,2850,2900,2950,3000,3050,3100,3150,3200,3250,3300,3350,3400,3450,3500,3550,3600,3650,3700,3750,3800,3850,3900,3950,4000])
    xlowbg  = array('d',[0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,4000,4100,4200,4200,4300,4400,4500,4600,4700,4800,4900,5000])

    foundbinningbg = 'False'
    while (foundbinningbg=='False'):
    
        binningbadbg = 'False'  
    
        binbg = len(xlowbg)-1   
  
        bgadded = TH1D('bgadded', 'bgadded',binbg,xlowbg)
   
        for Type in List_Bg:
            #print Type
            #print Type+'_'+btaglabel+'_'+chan+'_'+opt
            for i in range(1,Histos[Type+'_'+btaglabel+'_'+chan+'_'+opt].GetNbinsX()+1):
                for j in range(0,len(xlowbg)):
                    if (Histos[Type+'_'+btaglabel+'_'+chan+'_'+opt].GetBinCenter(i)<xlowbg[j]):
                        bgadded.SetBinContent(j,bgadded.GetBinContent(j)+Histos[Type+'_'+btaglabel+'_'+chan+'_'+opt].GetBinContent(i))
                        break
    
        for x in range(1,binbg+1):  
            if ( (bgadded.GetBinContent(binbg+1-x) < minbkg or bgadded.GetBinError(binbg+1-x)/bgadded.GetBinContent(binbg+1-x) > maxerrbkg) and binningbadbg == 'False' ):
                binningbadbg = 'True'
                #print 'B.G. for bin ',binbg+1-x,' is ',bgadded.GetBinContent(binbg+1-x),', So merging'
                if ((binbg+1-x)>1): xlowbg.pop(binbg-x)
                else: xlowbg.pop(1)
     
        #if (binningbadbg=='True'):
            #print 'Had to merge so will start over with next xlow: '
            #print '[',
            #for b in range(len(xlowbg)):
                #if (b == (len(xlowbg)-1)): print round(decimal.Decimal(str(float(xlowbg[b]))),2),
                #else: print round(decimal.Decimal(str(float(xlowbg[b]))),2),',',
            #print ']'
        if (binningbadbg=='False'): 
            foundbinningbg = 'True' 
            print 'Finished!!!!  xlowbg is '      
            print '[',
            for b in range(len(xlowbg)):
                if (b == (len(xlowbg)-1)): print round(decimal.Decimal(str(float(xlowbg[b]))),2),
                else: print round(decimal.Decimal(str(float(xlowbg[b]))),2),',',
            print ']'

        del bgadded
    
    binfile.write("# "+wprime+" "+btaglabel+" "+chan+" \n")
    binfile.write("channel = '"+chan+"' \n")
    binfile.write("btags = '"+btaglabel+"' \n")
    binfile.write("wprime = '"+wprime+"' \n")
    binfile.write("xlow = [")
    for b in range(len(xlowbg)):
        if (b == (len(xlowbg)-1)):binfile.write(str(round(decimal.Decimal(str(float(xlowbg[b]))),2)))
        else: binfile.write(str(round(decimal.Decimal(str(float(xlowbg[b]))),2))+",")
    binfile.write("] \n")
    binfile.write("\n")

btaglist = ['ex1b','ex2b']
optlist = ['Hard','Soft']
chanlist = ['El','Mu']
wprimelist = ['Right']

for b in btaglist:
    for c in chanlist:
        for o in optlist:
            Rebin(b,c,o)

binfile.close()
