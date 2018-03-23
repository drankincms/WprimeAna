import json, math
import ROOT, sys, os, re, string
from ROOT import *
import numpy as n

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetOptStat(0);

def loadTxt(direc, chan, bcat, evty, param):

    lines = []
    with open(direc+chan+'_'+bcat+'_'+evty+'_'+param+'scan.txt','r') as data_file:
        next(data_file)
        lines = [line.strip().split() for line in data_file]

    nbin = len(lines)

    hist = TH1F("hist",param,nbin,float(lines[0][0]),float(lines[nbin-1][0]))

    for ibin in xrange(1,nbin+1):
        hist.SetBinContent(ibin, float(lines[int(ibin)-1][1]))

    hist.GetYaxis().SetTitle('NLL')
    hist.GetXaxis().SetTitle('#Delta(#sigma)')
    hist.SetLineWidth(2)

    can = TCanvas("can","can",700,900)

    hist.Draw()

    can.SaveAs(direc+"NLL_"+chan+"_"+bcat+"_"+evty+"_"+param+".png")
    can.SaveAs(direc+"NLL_"+chan+"_"+bcat+"_"+evty+"_"+param+".pdf")

direc = 'Feb20_35p9fb_1D/'

for chan in ["electron", "muon", "comb"]:
    for bcat in ["ex1or2btags", "ex1btags", "ex2btags"]:
        for evty in ["soft", "hard", "hardsoft"]:
             for param in ["toppt", "murmuf", "pdfas", "btag", "mtag", "jes", "q2scale"]:
                loadTxt(direc, chan, bcat, evty, param)
