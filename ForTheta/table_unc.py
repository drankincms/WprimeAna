import json, math
import ROOT, sys, os, re, string
from ROOT import *
import numpy as n

ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetOptStat(0);

def loadJson(direc, chan, bcat, evty):

    with open(direc+chan+'_'+bcat+'_'+evty+'_unc.json') as data_file:
        data = json.load(data_file)

    nchan = len(data)

    fullfile = ROOT.TFile(direc+'comb_mWprime_Limits_WprimeRight_HardSoft_Histos-ex1or2btags.root')
    postfile = ROOT.TFile(direc+"histos_"+chan+"_"+bcat+"_"+evty+"-mle.root")

    for chan in data:
        print chan
        histtpre = fullfile.Get(chan+'__ttbar')
        histtpre.SetDirectory(0)
        histwpre = fullfile.Get(chan+'__wjets')
        histwpre.SetDirectory(0)
        hists1pre = fullfile.Get(chan+'__wp2000')
        hists1pre.SetDirectory(0)
        hists2pre = fullfile.Get(chan+'__wp2600')
        hists2pre.SetDirectory(0)
        hists3pre = fullfile.Get(chan+'__wp3200')
        hists3pre.SetDirectory(0)
        histtpost = postfile.Get(chan+'__ttbar')
        histtpost.SetDirectory(0)
        histwpost = postfile.Get(chan+'__wjets')
        histwpost.SetDirectory(0)
        hists1post = postfile.Get(chan+'__wp2000')
        hists1post.SetDirectory(0)
        hists2post = postfile.Get(chan+'__wp2600')
        hists2post.SetDirectory(0)
        hists3post = postfile.Get(chan+'__wp3200')
        hists3post.SetDirectory(0)

        tmperr = 0.
        for ibin in range(1,histtpre.GetNbinsX()+1):
            tmperr += histtpre.GetBinError(ibin)*histtpre.GetBinError(ibin) + histwpre.GetBinError(ibin)*histwpre.GetBinError(ibin) + data[chan][0][ibin-1]

        print str(sum(data[chan][0])), '+-', str(math.sqrt(tmperr))
        print '\t postfit ttbar = ', str(histtpost.Integral())
        print '\t postfit wjets = ', str(histwpost.Integral())
        print '\t postfit wp2000 = ', str(hists1post.Integral())
        print '\t postfit wp2600 = ', str(hists2post.Integral())
        print '\t postfit wp3200 = ', str(hists3post.Integral())
        print '\t ttbar scale = ', str(histtpost.Integral()/histtpre.Integral())
        print '\t wjets scale = ', str(histwpost.Integral()/histwpre.Integral())

direc = 'Feb27_35p9fb_1D_NoQ2/'

loadJson(direc, "comb", "ex1or2btags", "hardsoft")

#for chan in ["electron", "muon", "comb"]:
#    for bcat in ["ex1or2btags", "ex1btags", "ex2btags"]:
#        for evty in ["soft", "hard", "hardsoft"]:
#            loadJson(direc, chan, bcat, evty)
