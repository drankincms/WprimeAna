import ROOT, sys, os, re, string
from ROOT import *

from array import array
from math import *
import copy
import decimal

#from LoadDataBDT_LPC_Dylan_NewList import *
#from LoadDataBDT_LPC_Dylan_OldList import *
#from LoadDataBDT_LPC_David_7TeV import *
#from LoadDataBDT_LPC_David_7TeV_GenTopPt import *
#from LoadDataBDT_LPC_David_minKS0pt1_GenTopPt import *
#from LoadDataBDT_LPC_David_minKS0pt1_80percentYields import *
#from LoadDataBDT_LPC_minKS0pt1_Fix_60percentYields import *
#from LoadDataBDT_LPC_minKS0pt1_Fix_top15_60percentYields import *
from LoadDataBDT_LPC_minKS0pt1_minimal_newWjets import *

#from comb_yields_May16 import *
from comb_yields_Jun06_ScaleGenTopPt import *

def Rebin(channel, varName, wprime, btags):

    List_Bkg = ['wjets','ww','ttbar','zjets','t','bt','tw','btw','s','bs']

    if (channel == 'electron'): ch = '_el'
    if (channel == 'muon'): ch = '_mu'

    RootFilesBDT = {}
    Trees = {}
    for Type in List_Bkg:
        RootFilesBDT[wprime+'_'+Type+ch] = TFile(BDTDir+'BDT_GE1BTag_'+wprime+'_'+Type+ch+'.root')
        Trees[wprime+'_'+Type+ch] = RootFilesBDT[wprime+'_'+Type+ch].Get(Type)


    if (channel == 'electron'):
        cut = 'jet_0_pt_WprimeCalc >= 120 && jet_1_pt_WprimeCalc >= 40 && elec_1_pt_WprimeCalc > 50 && abs(elec_1_eta_WprimeCalc) < 2.5 && elec_1_RelIso_WprimeCalc < 0.1 && corr_met_WprimeCalc > 20 && Muon_DeltaR_LjetsTopoCalcNew > 0.3'

    if (channel == 'muon'):
        cut = 'jet_0_pt_WprimeCalc >= 120 && jet_1_pt_WprimeCalc >= 40 && muon_1_pt_WprimeCalc > 50 && abs(muon_1_eta_WprimeCalc) < 2.1 && muon_1_RelIso_WprimeCalc < 0.12 && corr_met_WprimeCalc > 20 && Muon_DeltaR_LjetsTopoCalcNew > 0.3'

    if btags == 'zerobtags': cutbtag = ' && ((jet_0_tag_WprimeCalc+jet_1_tag_WprimeCalc)==0) '
    if btags == 'onebtags': cutbtag = ' && ((jet_0_tag_WprimeCalc+jet_1_tag_WprimeCalc)==1) '
    if btags == 'ge1btags': cutbtag = ' && ((jet_0_tag_WprimeCalc+jet_1_tag_WprimeCalc)>=1) '
    if btags == 'twobtags': cutbtag = ' && ((jet_0_tag_WprimeCalc+jet_1_tag_WprimeCalc)==2) '
    if btags == 'ge2btags': cutbtag = ' && ( (jet_0_tag_WprimeCalc==1 && (jet_1_tag_WprimeCalc + jet_2_tag_WprimeCalc + jet_3_tag_WprimeCalc + jet_4_tag_WprimeCalc + jet_5_tag_WprimeCalc + jet_6_tag_WprimeCalc + jet_7_tag_WprimeCalc + jet_8_tag_WprimeCalc + jet_9_tag_WprimeCalc) >= 1 ) || (jet_1_tag_WprimeCalc==1 && (jet_0_tag_WprimeCalc + jet_2_tag_WprimeCalc + jet_3_tag_WprimeCalc + jet_4_tag_WprimeCalc + jet_5_tag_WprimeCalc + jet_6_tag_WprimeCalc + jet_7_tag_WprimeCalc + jet_8_tag_WprimeCalc + jet_9_tag_WprimeCalc) >= 1 ) ) '

    if btags == 'final': cutbtag = ' && ( (jet_0_tag_WprimeCalc+jet_1_tag_WprimeCalc ) >= 1 )'
    if btags == 'final': cut = cut + ' && BestTop_LjetsTopoCalcNew > 130 && BestTop_LjetsTopoCalcNew < 210 &&  BestTop_Pt_LjetsTopoCalcNew > 75  && Jet1Jet2_Pt_LjetsTopoCalcNew > 100 '


    var = varName
    print 'Rebinning of variable ',varName,' for wprime = ',wprime,' and ',btags

    global binfile
    binfile = open('bins_'+var+'_'+btags+'_minKS0pt1_minimal_newWjets.txt', 'a')      
          
    minbkg = 1.0
    maxerrbkg = 9999.0  

    print 'min background prediction per bin is ',minbkg
               
    cutwbb = ' && n_Bjets_WprimeCalc > 0' # Wb(b)                                                                                                      
    cutwcc = ' && n_Bjets_WprimeCalc==0 && n_Cjets_WprimeCalc>0' # Wc(c)                                                                               
    cutwjj = ' && n_Bjets_WprimeCalc==0 && n_Cjets_WprimeCalc==0' # W+light              

    doTTbarWeight = 'True'
    if (doTTbarWeight == 'True'):
        SFWjmu = 0.82        ## myHF120lep50, scale ttbar
        SFWcmu = 1.0*1.66   ## myHF120lep50, scale ttbar                                                                                              
        SFWbmu = 1.0*1.21   ## myHF120lep50, scale ttbar                                                                                              

        SFWjmuPlus = 0.82*0.87        ## myHF120lep50, scale ttbar                                                                                     
        SFWcmuPlus = 1.0*1.66*1.15   ## myHF120lep50, scale ttbar                                                                                     
        SFWbmuPlus = 1.0*1.21*1.15   ## myHF120lep50, scale ttbar                                                                                     

        SFWjmuMinus = 1.0*1.13        ## myHF120lep50, scale ttbar                                                                                    
        SFWcmuMinus = 1.0*1.66*0.85   ## myHF120lep50, scale ttbar                                                                                    
        SFWbmuMinus = 1.0*1.21*0.85   ## myHF120lep50, scale ttbar                                                                                    

        weight_ttbarminus = 'weight_TopPt_WprimeCalc'
        weight_ttbarplus = '1.0'
        weight_ttbar = 'weight_TopPt_WprimeCalc'

    if (doTTbarWeight == 'False'):
        SFWjmu = 0.82        ## myHF120lep50                                                                                                           
        SFWcmu = 0.93*1.66   ## myHF120lep50                                                                                                           
        SFWbmu = 0.93*1.21   ## myHF120lep50                                                                                                           

        SFWjmuPlus = 0.82*0.87        ## myHF120lep50                                                                                                  
        SFWcmuPlus = 0.93*1.66*1.15   ## myHF120lep50                                                                                                  
        SFWbmuPlus = 0.93*1.21*1.15   ## myHF120lep50                                                                                                  

        SFWjmuMinus = 0.82*1.13        ## myHF120lep50                                                                                                 
        SFWcmuMinus = 0.93*1.66*0.85   ## myHF120lep50                                                                                                 
        SFWbmuMinus = 0.93*1.21*0.85   ## myHF120lep50                                                                                                 

        weight_ttbarminus = '1.0'
        weight_ttbarplus = '1.0'
        weight_ttbar = '1.0'



    # This is the starting point for the binning search (smart choices speed up the initial bg search)
    xlowbg  = array('d',[-1.0,-0.95,-0.90,-0.85,-0.80,-0.75,-0.70,-0.65,-0.60,-0.55,-0.50,-0.45,-0.40,-0.35,-0.30,-0.25,-0.20,-0.15,-0.10,-0.05,-0.0,0.05,0.10,0.15,0.20,0.25,0.30,0.35,0.40,0.45,0.50,0.55,0.60,0.65,0.70,0.75,0.80,0.85,0.90,0.95,1.0])


    foundbinningbg = 'False'
    while (foundbinningbg=='False'):

        binningbadbg = 'False'  

        binbg = len(xlowbg)-1   

        Variables = {}
        bgadded = TH1D('bgadded', 'bgadded',binbg,xlowbg)
 
        for Type in List_Bkg:
        
            Type = wprime+'_'+Type+ch

            Variables[Type] = TH1D(Type, Type, binbg, xlowbg) 
            Variables[Type].Sumw2()
        
            if (channel=='electron'):
                #weight = 'weight_PU_ABC_PileUpCalc*weight_ElectronEff_WprimeCalc'                                                                         
                weight = '( ((0.973*weight_PU_ABCD_PileUpCalc*weight_ElectronEff_53x_WprimeCalc)*(abs(elec_1_eta_WprimeCalc)<1.5)) + ((1.02*weight_PU_ABCD_PileUpCalc*weight_ElectronEff_53x_WprimeCalc)*(abs(elec_1_eta_WprimeCalc)>1.5 && abs(elec_1_eta_WprimeCalc)<2.5)) )'
                SF = 1.0
            if (channel=='muon'):
                weight = 'weight_PU_ABCD_PileUpCalc*weight_MuonEff_WprimeCalc'
                SF = 1.0

            if (Type == wprime+'_'+'wjets'+ch):
                WccHist = TH1D('WccHist', 'WccHist', binbg,xlowbg)
                WbbHist = TH1D('WbbHist', 'WbbHist', binbg,xlowbg)
                WccHist.Sumw2()
                WbbHist.Sumw2()


            if (Type.startswith(wprime+'_wjets')):                                                                             
                Trees[Type].Draw(var+" >> "+Type,"("+weight+")*("+str(SFWjmu)+")*("+cut+cutwjj+cutbtag+")",'goff')
                Trees[Type].Draw(var+" >> "+"WbbHist","("+weight+")*("+str(SFWbmu)+")*("+cut+cutwbb+cutbtag+")",'goff')
                Trees[Type].Draw(var+" >> "+"WccHist","("+weight+")*("+str(SFWcmu)+")*("+cut+cutwcc+cutbtag+")",'goff')
                Variables[Type].Add(WbbHist)
                Variables[Type].Add(WccHist)
            elif (not Type.startswith(wprime+'_t')):
                Trees[Type].Draw(var + " >> " + Type, "("+weight+")*("+cut+cutbtag+")", 'goff')
            elif (Type.startswith(wprime+'_ttbar')):
                Trees[Type].Draw(var + " >> " + Type, "("+weight+")*("+weight_ttbar+")*("+cut+cutbtag+")", 'goff')
            else:
                Trees[Type].Draw(var + " >> " + Type, "("+weight+")*("+cut+cutbtag+")", 'goff')


            #if (channel=='electron'): lumi = lumi_el
            #if (channel=='muon'): lumi = lumi_mu

            if Variables[Type].Integral() != 0:
                Variables[Type].Scale ( yields[Type+'_'+btags]/Variables[Type].Integral() )
                   
            bgadded.Add(Variables[Type])

        for x in range(1,binbg+1):  
            if ( (bgadded.GetBinContent(binbg+1-x) < minbkg or bgadded.GetBinError(binbg+1-x)/bgadded.GetBinContent(binbg+1-x) > maxerrbkg) and binningbadbg == 'False' ):
                binningbadbg = 'True'
                print 'B.G. for bin ',binbg+1-x,' is ',bgadded.GetBinContent(binbg+1-x),', So merging'
                if ((binbg+1-x)>1): xlowbg.pop(binbg-x)
                else: xlowbg.pop(1)
 
        if (binningbadbg=='True'):
            print 'Had to merge so will start over with next xlow: '
            print '[',
            for b in range(len(xlowbg)):
                if (b == (len(xlowbg)-1)): print round(decimal.Decimal(str(float(xlowbg[b]))),2),
                else: print round(decimal.Decimal(str(float(xlowbg[b]))),2),',',
            print ']'
        if (binningbadbg=='False'): 
            foundbinningbg = 'True' 
            print 'Finished!!!!  xlowbg is '      
            print '[',
            for b in range(len(xlowbg)):
                if (b == (len(xlowbg)-1)): print round(decimal.Decimal(str(float(xlowbg[b]))),2),
                else: print round(decimal.Decimal(str(float(xlowbg[b]))),2),',',
            print ']'
            
        del Variables
        del bgadded
        del WccHist
        del WbbHist
      
  

    binfile.write("# "+wprime+" "+varName+" "+channel+" \n")
    binfile.write("channel = '"+channel+"' \n")
    binfile.write("btags = '"+btags+"' \n")
    binfile.write("wprime = '"+wprime+"' \n")
    binfile.write("xlow = [")
    for b in range(len(xlowbg)):
        if (b == (len(xlowbg)-1)):binfile.write(str(round(decimal.Decimal(str(float(xlowbg[b]))),2)))
        else: binfile.write(str(round(decimal.Decimal(str(float(xlowbg[b]))),2))+",")
    binfile.write("] \n")
    binfile.write("bins = len(xlow)-1 \n")
    binfile.write("plot_DataVsMc(channel, var, bins, xlow, high, yaxis, xaxis, save, wprime, btags) ")
    binfile.write("\n")
    binfile.write("\n")
    binfile.close()
            
    del RootFilesBDT
    del Trees



Rebin('electron', 'MVA_BDT', 'wp800R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp1000R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp1200R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp1400R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp1600R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp1800R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp2000R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp2200R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp2400R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp2600R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp2800R', 'ge1btags')
Rebin('electron', 'MVA_BDT', 'wp3000R', 'ge1btags')

Rebin('muon', 'MVA_BDT', 'wp800R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp1000R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp1200R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp1400R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp1600R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp1800R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp2000R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp2200R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp2400R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp2600R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp2800R', 'ge1btags')
Rebin('muon', 'MVA_BDT', 'wp3000R', 'ge1btags')

Rebin('electron', 'MVA_BDT', 'wp800R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp1000R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp1200R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp1400R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp1600R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp1800R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp2000R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp2200R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp2400R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp2600R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp2800R', 'onebtags')
Rebin('electron', 'MVA_BDT', 'wp3000R', 'onebtags')

Rebin('muon', 'MVA_BDT', 'wp800R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp1000R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp1200R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp1400R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp1600R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp1800R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp2000R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp2200R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp2400R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp2600R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp2800R', 'onebtags')
Rebin('muon', 'MVA_BDT', 'wp3000R', 'onebtags')

Rebin('electron', 'MVA_BDT', 'wp800R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp1000R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp1200R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp1400R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp1600R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp1800R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp2000R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp2200R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp2400R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp2600R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp2800R', 'twobtags')
Rebin('electron', 'MVA_BDT', 'wp3000R', 'twobtags')

Rebin('muon', 'MVA_BDT', 'wp800R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp1000R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp1200R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp1400R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp1600R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp1800R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp2000R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp2200R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp2400R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp2600R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp2800R', 'twobtags')
Rebin('muon', 'MVA_BDT', 'wp3000R', 'twobtags')
