import json, math
import ROOT, sys, os, re, string
from ROOT import *
import numpy as n

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetOptStat(0);

def loadJson(direc, chan, bcat, evty):

    with open(direc+chan+'_'+bcat+'_'+evty+'_postfit.json') as data_file:
        data = json.load(data_file)

    nuissize = len(data)-1

    x = n.ones(2,dtype=float)
    y = n.ones(2,dtype=float)
    ex = n.ones(2,dtype=float)
    ey = n.ones(2,dtype=float)
    
    x[0] = -1.
    y[0] = 0.
    ex[0] = 0.5
    ey[0] = 1.
    x[1] = float(nuissize)+1.
    y[1] = 0.
    ex[1] = 0.5
    ey[1] = 1.

    cengr = TGraph(len(x), x, y)
    bandgr1 = TGraphErrors(len(x), x, y, ex, ey)
    bandgr2 = TGraphErrors(len(x), x, y, 2*ex, 2*ey)
    cengr.SetMarkerStyle(20)
    cengr.SetLineWidth(3)
    cengr.SetLineStyle(2)
    #bandgr.SetFillColor(15)
    bandgr1.SetFillColorAlpha(3,1.)
    bandgr2.SetFillColorAlpha(5,1.)
    #bandgr.SetFillStyle(3001);

    hist = TH1F("hist","hist",nuissize,-0.5,float(nuissize)-0.5)

    bini = 1
    for name in data:
        if (name.startswith('_')): continue
        #print name, data[name][0]

        #tmpsum = 0.
        #tmpval = []
        #for i in range(0, len(data[name])):
        #    tmpval.append(data[name][i][0])
        #    tmpsum += data[name][i][0]
        #tmpval.sort()

        #hist.SetBinContent(bini,tmpsum/float(len(data[name])))
        #hist.SetBinError(bini,(tmpval[int(0.84*float(len(data[name])))]-tmpval[int(0.16*float(len(data[name])))])/2.)
        hist.SetBinContent(bini,data[name][0][0])
        hist.SetBinError(bini,data[name][0][1])
        hist.GetXaxis().SetBinLabel(bini,name)
        bini = bini + 1

    nuisgr = TGraphErrors(hist)
    nuisgr.SetMarkerStyle(20)
    nuisgr.SetMarkerSize(1.2)
    nuisgr.SetLineWidth(2)
    nuisgr.SetLineColor(1)

    mg = TMultiGraph()
    mg.SetTitle("Post-fit Nuisance Parameters")

    can = TCanvas("can","can",700,900)

    mg.Add(bandgr2,"L3")
    mg.Add(bandgr1,"L3")
    mg.Add(cengr,"L")
    mg.Add(nuisgr,"P")

    hist.GetYaxis().SetRangeUser(-3.,3.)
    hist.GetYaxis().SetTitle("#theta")
    hist.Draw("axis")
    mg.Draw("")
    #nuisgr.Draw("AC*")
    #hist.Draw("axis same")
    
    bini = 1
    for name in data:
        if (name.startswith('_')): continue
        #mg.GetXaxis().SetBinLabel(bini,name)
        bini = bini + 1

    can.SaveAs(direc+"PostFitNuisances_"+chan+"_"+bcat+"_"+evty+".png")
    can.SaveAs(direc+"PostFitNuisances_"+chan+"_"+bcat+"_"+evty+".pdf")

    with open(direc+chan+'_'+bcat+'_'+evty+'_unc.json') as unc_file:
        histunc = json.load(unc_file)

    clist = []
    if (not chan=='muon'):
        clist.append('elec')
    if (not chan=='electron'):
        clist.append('mu')

    blist = []
    if (not bcat=='ex1btags'):
        blist.append('ex2btags')
    if (not bcat=='ex2btags'):
        blist.append('ex1btags')

    elist = []
    if (not evty=='soft'):
        elist.append('hard')
    if (not evty=='hard'):
        elist.append('soft')

    for cname in clist:
        for bname in blist:
            for ename in elist:
	
	        fullfile = ROOT.TFile(direc+'comb_mWprime_Limits_WprimeRight_HardSoft_Histos-ex1or2btags.root')
                histtpost = fullfile.Get(cname+'_'+ename+'_'+bname+'__ttbar')
                histtpost.SetDirectory(0)
                histwpost = fullfile.Get(cname+'_'+ename+'_'+bname+'__wjets')
                histwpost.SetDirectory(0)

	        histtp = histtpost.Clone("histtp")
	        histtp.SetDirectory(0)
	        histtm = histtpost.Clone("histtm")
	        histtm.SetDirectory(0)
	        histwp = histwpost.Clone("histwp")
	        histwp.SetDirectory(0)
	        histwm = histwpost.Clone("histwm")
	        histwm.SetDirectory(0)

	        for xbin in range(1,histtpost.GetNbinsX()+1):
	            tmperrtp = histtpost.GetBinError(xbin)*histtpost.GetBinError(xbin)
	            tmperrtm = histtpost.GetBinError(xbin)*histtpost.GetBinError(xbin)

	            tmperrwp = histwpost.GetBinError(xbin)*histwpost.GetBinError(xbin)
	            tmperrwm = histwpost.GetBinError(xbin)*histwpost.GetBinError(xbin)

	            histtp.SetBinContent(xbin,tmperrtp + histunc[cname+'_'+ename+'_'+bname][1][xbin-1]*histunc[cname+'_'+ename+'_'+bname][1][xbin-1]/1.4)
	            histtm.SetBinContent(xbin,tmperrtm + histunc[cname+'_'+ename+'_'+bname][1][xbin-1]*histunc[cname+'_'+ename+'_'+bname][1][xbin-1]/1.4)
	
	            histwp.SetBinContent(xbin,tmperrwp + histunc[cname+'_'+ename+'_'+bname][1][xbin-1]*histunc[cname+'_'+ename+'_'+bname][1][xbin-1]/1.4)
	            histwm.SetBinContent(xbin,tmperrwm + histunc[cname+'_'+ename+'_'+bname][1][xbin-1]*histunc[cname+'_'+ename+'_'+bname][1][xbin-1]/1.4)

                del histtpost
                del histwpost
	
	        mlefile = ROOT.TFile(direc+"histos_"+chan+"_"+bcat+"_"+evty+"-mle.root","UPDATE")
	        mlefile.cd()
	        upt = histtp.Clone("upt")
	        upt.SetName(cname+'_'+ename+'_'+bname+'__ttbar_UP')
	        downt = histtm.Clone("downt")
	        downt.SetName(cname+'_'+ename+'_'+bname+'__ttbar_DOWN')
	        upw = histwp.Clone("upw")
	        upw.SetName(cname+'_'+ename+'_'+bname+'__wjets_UP')
	        downw = histwm.Clone("downw")
	        downw.SetName(cname+'_'+ename+'_'+bname+'__wjets_DOWN')
	    
	        mlefile.Write()


direc = 'Jul18_35p9fb_1D_NewLUM/'

#loadJson(direc, "comb", "ex1or2btags", "hardsoft")

for chan in ["electron", "muon", "comb"]:
    for bcat in ["ex1or2btags", "ex1btags", "ex2btags"]:
        for evty in ["soft", "hard", "hardsoft"]:
            loadJson(direc, chan, bcat, evty)
