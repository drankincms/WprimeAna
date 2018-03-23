import json, math
import ROOT, sys, os, re, string
from ROOT import *
import numpy as n

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetOptStat(0);

def loadJson(direc, chan, bcat, evty):

    with open(direc+chan+'_'+bcat+'_'+evty+'_cov.json') as data_file:
        data = json.load(data_file)

    with open(direc+chan+'_'+bcat+'_'+evty+'_names.json') as name_file:
        names = json.load(name_file)

    nuissize = len(data[0])
    
    hist = TH2F("hist","hist",nuissize,-0.5,float(nuissize)-0.5,nuissize,-0.5,float(nuissize)-0.5)

    for binx in range(1,nuissize+1):
        for biny in range(1,nuissize+1):
            hist.SetBinContent(binx,biny,float('%.2f'%(data[binx-1][biny-1])))

    bini = 1
    for name in names:
        #print name, names[name][0]

        hist.GetXaxis().SetBinLabel(bini,name)
        hist.GetYaxis().SetBinLabel(bini,name)
        bini = bini + 1

    hist.SetTitle("Nuisance Parameter Covariance Matrix")

    can = TCanvas("can","can",700,900)

    hist.Draw("colz text45")

    can.SaveAs(direc+"PostFitCovariance_"+chan+"_"+bcat+"_"+evty+".png")
    can.SaveAs(direc+"PostFitCovariance_"+chan+"_"+bcat+"_"+evty+".pdf")

loadJson('./','comb','ex1or2btags','hardsoft')
