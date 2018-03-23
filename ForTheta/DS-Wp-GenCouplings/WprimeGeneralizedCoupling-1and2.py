import ROOT, sys, os, re, string
from ROOT import TCanvas, TFile, TProfile, TNtuple, TH1F, TH1D, TH2F,TF1, TPad, TPaveLabel, TPaveText, TLegend, TLatex, THStack
from ROOT import gROOT, gBenchmark, gRandom, gSystem, Double, gPad
from optparse import OptionParser


parser = OptionParser()
parser.add_option("-l", "--al",
                  action="store", type="int", dest="al",
                  help="a_L", metavar="AL")
parser.add_option("-r", "--ar",
                  action="store", type="int", dest="ar",
                  help="a_R", metavar="AR")
(options, args) = parser.parse_args()


xsec_RT = {'800':6.1959,'900':3.9635,'1000':2.6218,'1100':1.7833,'1200':1.2420,'1300':0.88188,'1400':0.63665,'1500':0.46602,'1600':0.34533,'1700':0.25886,'1800':0.19584,'1900':0.14922,'2000':0.11459,'2100':0.088534,'2200':0.068860,'2300':0.053847,'2400':0.042275,'2500':0.033343,'2600':0.026400,'2700':0.020986,'2800':0.016738,'2900':0.013397,'3000':0.010756,'3100':0.008669,'3200':0.006997,'3300':0.005672,'3400':0.004608,'3500':0.003758,'3600':0.003074,'3700':0.002523,'3800':0.002078,'3900':0.001718,'4000':0.001425}
	  
xsec_MT = {'800':14.029,'900':9.7876,'1000':4.8530,'1100':4.0787,'1200':3.5912,'1300':3.2735,'1400':3.0576,'1500':2.9107,'1600':2.8126,'1700':2.7431,'1800':2.6946,'1900':2.6614,'2000':2.6378,'2100':2.6209,'2200':2.6093,'2300':2.6019,'2400':2.5976,'2500':2.5948,'2600':2.5921,'2700':2.5917,'2800':2.5910,'2900':2.5918,'3000':2.5929,'3100':2.5931,'3200':2.5942,'3300':2.5977,'3400':2.5976,'3500':2.5993,'3600':2.6011,'3700':2.6024,'3800':2.6032,'3900':2.6048,'4000':2.6066}

xsec_LT = {'800':7.8894,'900':5.8370,'1000':4.6217,'1100':3.8865,'1200':3.4221,'1300':3.1252,'1400':2.9292,'1500':2.7983,'1600':2.7107,'1700':2.6508,'1800':2.6107,'1900':2.5848,'2000':2.5677,'2100':2.5567,'2200':2.5502,'2300':2.5469,'2400':2.5454,'2500':2.5457,'2600':2.5474,'2700':2.5493,'2800':2.5519,'2900':2.5551,'3000':2.5578,'3100':2.5620,'3200':2.5650,'3300':2.5672,'3400':2.5704,'3500':2.5733,'3600':2.5764,'3700':2.5790,'3800':2.5813,'3900':2.5837,'4000':2.5861}

xsec_SM = 3.44
Lumi=1

ListWprimeMass = ['wp1000','wp1100','wp1200','wp1300','wp1400','wp1500','wp1600','wp1700','wp1800','wp1900','wp2000','wp2100','wp2200','wp2300','wp2400','wp2500','wp2600','wp2700','wp2800','wp2900','wp3000','wp3100','wp3200','wp3300','wp3400','wp3500','wp3600','wp3700','wp3800','wp3900','wp4000']

RootFiles = {}

inDir = "../RootFiles_For2DLimits/"

RootFiles['Right_elec'] = TFile(inDir+"electron_mWprime_Limits_WprimeModRight_HardSoft_Histos-ex1or2btags.root")
RootFiles['Left_elec'] =  TFile(inDir+"electron_mWprime_Limits_WprimeLeft_HardSoft_Histos-ex1or2btags.root")
RootFiles['Mix_elec'] =   TFile(inDir+"electron_mWprime_Limits_WprimeMix_HardSoft_Histos-ex1or2btags.root")

RootFiles['Right_mu'] = TFile(inDir+"muon_mWprime_Limits_WprimeModRight_HardSoft_Histos-ex1or2btags.root")
RootFiles['Left_mu'] =  TFile(inDir+"muon_mWprime_Limits_WprimeLeft_HardSoft_Histos-ex1or2btags.root")
RootFiles['Mix_mu'] =   TFile(inDir+"muon_mWprime_Limits_WprimeMix_HardSoft_Histos-ex1or2btags.root")


def makeHistos_al_ar(als, ars, lepton):
       
    CombinedHist = {}
    keys = []

    al_step=als
    ar_step=ars
    al= 0.10000*al_step
    ar= 0.10000*ar_step
    #print  ' al =', al, ' ar =',ar
    #print '----'

    prefix = 'Wprime_Histos_'
    suffix = '.root'
    mid = '-'
    fileName= prefix + lepton + mid + `al_step` + mid  +`ar_step` + suffix
    #print ' output to FILE', fileName
    f = TFile(fileName, "RECREATE")

    rt_wgt=0.
    lt_wgt=0.
    st_wgt=1.
    mt_wgt=0.
    if (al>0. or ar>0.):
        rt_wgt=ar*ar*(ar*ar - al*al)/(al*al + ar*ar)
        lt_wgt=al*al*(al*al - ar*ar)/(al*al + ar*ar)
        st_wgt=(1 - al*al - 2*al*al*ar*ar/(al*al+ar*ar)) 
        mt_wgt=4*al*al*ar*ar/(al*al + ar*ar)

    #print ' ar,al,mix,sm weights ', rt_wgt, ' ', lt_wgt, ' ', mt_wgt, ' ', st_wgt
    #if (rt_wgt <0) : print ' NEGATIVE WEIGHT R', al,' ', ar
    #if (lt_wgt <0) : print ' NEGATIVE WEIGHT L', al,' ', ar
    #if (mt_wgt <0) : print ' NEGATIVE WEIGHT M', al,' ', ar
    #if (st_wgt <0) : print ' NEGATIVE WEIGHT S', al,' ', ar
 
#def dummyfunc ():
    for key in RootFiles['Mix_'+lepton].GetListOfKeys():

        if keys.count( str(key.GetName()) ) == 1:
            #print ' already used ', str(key.GetName()) 
            continue
        keys . append( str(key.GetName()) )
        #print 'Name / Title = ',key.GetName(),' / ',key.GetTitle()

        hist = {}
        name = str(key.GetName())
        #print  ' Name ', name
        
        hist['Right'] = RootFiles['Right_'+lepton].Get( str(key.GetName()) )
        hist['Left'] =  RootFiles['Left_'+lepton].Get(  str(key.GetName()) )
        hist['Mix'] =   RootFiles['Mix_'+lepton].Get(   str(key.GetName()) )

        isOneBtag = name.find("ex1btags")
        isSoft = name.find("soft")
        isHard = name.find("hard")

        if (name.find("jes__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__jes__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__jes__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__jes__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__jes__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__jes__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__jes__plus" )
        elif (name.find("jes__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__jes__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__jes__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__jes__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__jes__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__jes__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__jes__minus" )
        elif (name.find("btag__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__btag__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__btag__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__btag__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__btag__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__btag__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__btag__plus" )
        elif (name.find("btag__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__btag__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__btag__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__btag__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__btag__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__btag__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__btag__minus" )
        elif (name.find("mtag__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__mtag__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__mtag__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__mtag__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__mtag__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__mtag__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__mtag__plus" )
        elif (name.find("mtag__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__mtag__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__mtag__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__mtag__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__mtag__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__mtag__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__mtag__minus" )
        elif (name.find("jer__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__jer__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__jer__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__jer__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__jer__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__jer__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__jer__plus" )
        elif (name.find("jer__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__jer__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__jer__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__jer__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__jer__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__jer__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__jer__minus" )
        elif (name.find("pileup__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__pileup__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__pileup__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__pileup__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__pileup__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__pileup__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__pileup__plus" )
        elif (name.find("pileup__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__pileup__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__pileup__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__pileup__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__pileup__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__pileup__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__pileup__minus" )
        elif (name.find("murmuf__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__murmuf__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__murmuf__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__murmuf__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__murmuf__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__murmuf__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__murmuf__plus" )
        elif (name.find("murmuf__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__murmuf__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__murmuf__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__murmuf__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__murmuf__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__murmuf__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__murmuf__minus" )
        elif (name.find("pdfas__plus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__pdfas__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__pdfas__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__pdfas__plus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__pdfas__plus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__pdfas__plus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__pdfas__plus" )
        elif (name.find("pdfas__minus") > 0) :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb__pdfas__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb__pdfas__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb__pdfas__minus" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb__pdfas__minus" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb__pdfas__minus" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb__pdfas__minus" )
        else :
            if (isOneBtag>0):
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex1btags__topstb" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex1btags__topstb" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex1btags__topstb" )
            else: 
                if (isSoft>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_soft_ex2btags__topstb" )
                elif (isHard>0): hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_hard_ex2btags__topstb" )
                else: hist['SM']=     RootFiles['Right_'+lepton].Get(lepton+"_invmass_ex2btags__topstb" )
            
        isThereWprime = name.find("wp")
        #print ' wp is there? ',isThere


        for Mass in ListWprimeMass:
            wp = name.find(Mass)
            #print ' wp mass ', wp
            if (wp>0) :
                rt_xsec = xsec_RT[Mass.lstrip('wp')]
                lt_xsec = xsec_LT[Mass.lstrip('wp')]
                mt_xsec = xsec_MT[Mass.lstrip('wp')]
                st_xsec = xsec_SM
                #print ' xsec ', Mass ,' ', rt_xsec ,' ', lt_xsec ,' ', mt_xsec ,' ', st_xsec
                
        if ( isThereWprime >0 ) :
            #print 'Name / Title = ',key.GetName(),' / ',key.GetTitle()
            #print 'Hist = ', hist['SM1b'].Integral(), ' ',   hist['Right'].Integral(), ' ',  hist['Left'].Integral(), ' ', hist['Mix'].Integral()
            NewHist = hist['SM']
            #print ' NewHist = ', NewHist.Integral()
            #ast_wgt=(st_wgt*st_xsec*Lumi)-1
            ast_wgt=st_wgt-1
            #ast_wgt=st_wgt
            NewHist.Add ( hist['SM'], ast_wgt )
            #print ' Hist = ', NewHist.Integral()
            NewHist.Add ( hist['Right'], rt_wgt*Lumi*rt_xsec)
            #print ' Hist = ', NewHist.Integral()
            NewHist.Add ( hist['Mix'], mt_wgt*Lumi*mt_xsec )
            #print ' Hist = ', NewHist.Integral()
            NewHist.Add ( hist['Left'], lt_wgt*Lumi*lt_xsec  )
            #print ' Hist = ', NewHist.Integral()
            #NewHist.SetName( str(key.GetTitle()) )
            NewHist.SetName( str(hist['Mix'].GetTitle()) )
            NewHist.SetTitle( str(hist['Mix'].GetTitle()) )

            #print ' ar,al,mix,sm weights ', rt_wgt, ' ', lt_wgt, ' ', mt_wgt, ' ', st_wgt
            #print 'Combined Hist = ',NewHist.Integral(), ' ',hist['SM'].Integral(), ' ',hist['Right'].Integral(),' ',hist['Left'].Integral(),' ', hist['Mix'].Integral()

            nbins = NewHist.GetXaxis().GetNbins()
            #print "Number of bins    ", str(nbins)
            ntotzero=0
            anegContent=0.
            allContent=0.
#            print ' inits check ',ntotzero, ' ', anegContent, ' ', allContent
            allContent=(NewHist.Integral())
            for j in range(1,nbins+1):
                binc = NewHist.GetBinContent(j)
                if (binc < 0.): 
                    #print "Negative bin content  ",str(j)," ",binc, ' ', hist['Right'].GetBinContent(j), ' ', hist['Left'].GetBinContent(j), ' ', hist['Mix'].GetBinContent(j), ' ',  rt_wgt*hist['Right'].GetBinContent(j), ' ',lt_wgt* hist['Left'].GetBinContent(j), ' ', mt_wgt*hist['Mix'].GetBinContent(j)
                    ntotzero+=1
                    anegContent+=binc
                    xa=0.0000001
                    NewHist.SetBinContent(j,xa)
                    binc = NewHist.GetBinContent(j)
                    #print "set bin content to zero ",str(j)," ",binc
                    #if (binc !=0.0000001) : print 'something went wrong in setting the bin content to zero'
            if (ntotzero>0):
                #print  ' Name ', name
                #print 'numbins negative ',ntotzero, '  ', anegContent, '  ', allContent
                zerofrac=anegContent/(allContent)
                #if (zerofrac < -0.02 ) :
                #    print 'zerofrac ', zerofrac

            CombinedHist[ str(key.GetTitle()) ] = NewHist
            
            del hist
            del NewHist
            
        else :
            CombinedHist[ str(key.GetTitle()) ] =  hist['Mix']
            del hist
            #print ''

            
        CombinedHist[ str(key.GetTitle()) ].Write()
        #print 'Wrote ',str(key.GetTitle())
al=0.
ar=1.
al_step=0
ar_step=0
#lepton="elec"
#for ar_step in range(0,11):
#for ar_step in range(options.ar, options.ar+1):
#    for al_step in range(0 , 11):
        #print 'al_step = ',al_step, ' ar_step =',ar_step
        #makeHistos_al_ar(al_step, ar_step, lepton)
#lepton="mu"
#for ar_step in range(0,11):
#for ar_step in range(options.ar, options.ar+1):
#    for al_step in range(0 , 11):
        #print 'al_step = ',al_step, ' ar_step =',ar_step
        #makeHistos_al_ar(al_step, ar_step, lepton)

makeHistos_al_ar(options.al,options.ar,"mu")
makeHistos_al_ar(options.al,options.ar,"elec")
