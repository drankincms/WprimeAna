import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math

#name = "Run2016BCD_Jul21_Slim/RootFiles_ForSyst/muon_LepPt_WprimeRight_Final_Histos-ex1or2btags.root"
name = "Run2016BCD_Neg_Jul26_Slim/RootFiles_ForSyst/muon_LepPt_WprimeRight_Final_Histos-ge1btags.root"
outdir = "Run2016BCD_Neg_Jul26_Slim/LepPtSystPlots/"
os.system('mkdir -p '+outdir)

#systlist = ["jes","jer","btag","mtag","wjhf","toppt","pileup","pdfas","murmuf","q2scale","mues","muer"]
#SystName = ["J.E.S.","J.E.R.","B-Tag","Mistag","H.F.","Top $p_{T}$","PU","PDF","$Q^2$ ME Scale","$Q^2$ PM Scale","Muon Energy Scale","Muon Energy Resolution"]
#systlist = ["jes","jer","btag","mtag","wjhf","toppt","pileup","murmuf","q2scale","mues","muer"]
#SystName = ["J.E.S.","J.E.R.","B-Tag","Mistag","H.F.","Top $p_{T}$","PU","$Q^2$ ME Scale","$Q^2$ PM Scale","Muon Energy Scale","Muon Energy Resolution"]
systlist = ["mues"]
SystName = ["Muon Energy Scale"]

noDOWN = ["toppt","muer"]
noUP = []

ex1mu = []
ex2mu = []
ex1el = []
ex2el = []

ROOT.gROOT.SetBatch(ROOT.kTRUE)

file0 = TFile(name)
file0.cd()
dirList = gDirectory.GetListOfKeys()
for k1 in dirList: 
    h1 = k1.ReadObj()
    if (h1.ClassName() == "TH1F"):
        if (h1.GetName().endswith("DATA") or h1.GetName().endswith("plus") or h1.GetName().endswith("minus")):
            continue
        if (h1.GetName().startswith("elec")):
            continue
        for syst in systlist:
            #if (not (h1.GetName().endswith("500") or h1.GetName().endswith("000") or h1.GetName().endswith("ttbar"))):
            if (not (h1.GetName().endswith("000") or h1.GetName().endswith("ttbar"))):
                continue
            if (h1.GetName().endswith("000") and syst=="pdfas"):
                syst = "sig"+syst
            h0 = h1.Clone("h0")
            h0.SetTitle("")
            if (syst not in noUP): h2 = file0.Get(h1.GetName()+"__"+syst+"__plus")
            if (syst not in noDOWN): h3 = file0.Get(h1.GetName()+"__"+syst+"__minus")
            if (h1.GetName().endswith("ttbar")):
                if (syst not in noUP): tuprat = file0.Get(h1.GetName()+"__"+syst+"__plus").Integral()/file0.Get(h1.GetName()).Integral()
                else: tuprat = 1
                if (syst not in noDOWN): tdownrat = file0.Get(h1.GetName()+"__"+syst+"__minus").Integral()/file0.Get(h1.GetName()).Integral()
                else: tdownrat = 1
                if (syst not in noUP): wuprat = file0.Get(h1.GetName()[:-5]+"wjets__"+syst+"__plus").Integral()/file0.Get(h1.GetName()[:-5]+"wjets").Integral()
                else: wuprat = 1
                if (syst not in noDOWN): wdownrat = file0.Get(h1.GetName()[:-5]+"wjets__"+syst+"__minus").Integral()/file0.Get(h1.GetName()[:-5]+"wjets").Integral()
                else: wdownrat = 1
                print tdownrat, tuprat
                print wdownrat, wuprat
                h0.Add(file0.Get(h1.GetName()[:-5]+"wjets"))
                if (syst not in noUP): h2.Add(file0.Get(h1.GetName()[:-5]+"wjets__"+syst+"__plus"))
                if (syst not in noDOWN): h3.Add(file0.Get(h1.GetName()[:-5]+"wjets__"+syst+"__minus"))
                if (h1.GetName().startswith("elec") and h1.GetName().find("ex1")>-1):
	            ex1el.append(wuprat)
	            ex1el.append(wdownrat)
	            ex1el.append(tuprat)
	            ex1el.append(tdownrat)
                if (h1.GetName().startswith("elec") and h1.GetName().find("ex2")>-1):
	            ex2el.append(wuprat)
	            ex2el.append(wdownrat)
	            ex2el.append(tuprat)
	            ex2el.append(tdownrat)
                if (h1.GetName().startswith("mu") and h1.GetName().find("ex1")>-1):
	            ex1mu.append(wuprat)
	            ex1mu.append(wdownrat)
	            ex1mu.append(tuprat)
	            ex1mu.append(tdownrat)
                if (h1.GetName().startswith("mu") and h1.GetName().find("ex2")>-1):
	            ex2mu.append(wuprat)
	            ex2mu.append(wdownrat)
	            ex2mu.append(tuprat)
	            ex2mu.append(tdownrat)
            print h1.Integral(),"-",h0.Integral(),"/",h0.Integral()
            if (syst not in noUP):
                ratio1 = h2.Clone("ratio1")
                ratio1.SetTitle("")
                ratio1.Sumw2()
                ratio1.Add(h0,-1.)
                ratio1.Divide(h0)
            if (syst not in noDOWN):
                ratio2 = h3.Clone("ratio2")
                ratio2.SetTitle("")
                ratio2.Sumw2()
                ratio2.Add(h0,-1.)
                ratio2.Divide(h0)

            can = ROOT.TCanvas("can","can",700,900)
            can.SetLogy()
            h0.SetLineColor(1)
            if (syst not in noUP): h2.SetLineColor(2)
            if (syst not in noDOWN): h3.SetLineColor(4)
            h0.SetLineWidth(2)
            if (syst not in noUP): h2.SetLineWidth(1)
            if (syst not in noDOWN): h3.SetLineWidth(1)
            h0.SetStats(0)
            h0.SetMinimum(0.001)
            maxi = h0.GetMaximum() + math.sqrt(h0.GetMaximum())
            maxi = maxi + math.sqrt(maxi)
            h0.SetMaximum(maxi*6)
            can.cd()
            pad1 = TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0)
            pad1.SetBottomMargin(0.26)
            pad1.Draw()
            pad1.cd()
            pad1.SetLogy()
            h0.Draw()
                        
            yAxis = h0.GetYaxis()
            yAxis.SetTitle("Events / bin")
            yAxis.SetTitleFont(42)
            yAxis.SetTitleSize(0.037)
            yAxis.SetTitleOffset(1.2)
            yAxis.SetLabelFont(42)
            yAxis.SetLabelSize(0.035)
            yAxis.SetLabelOffset(0.007)

            text1 = TLatex(1.570061,23.08044,"#scale[0.6]{CMS Preliminary   L = 12.9 fb^{-1}, #sqrt{s} = 13 TeV}")
            text1.SetNDC()
            text1.SetTextAlign(13)
            text1.SetX(0.35)
            text1.SetY(0.93)
            text1.SetTextFont(62)
            text1.SetTextSizePixels(15)
            text1.Draw()

            h0.GetXaxis().SetLabelOffset(999)
            h0.GetXaxis().SetLabelSize(0)
            h0.GetXaxis().SetTitle("p_{T}(#mu) [GeV]")

            text3 = TPaveText(0.20, 0.8, 0.44, 0.85,"NDC")
            text3.SetFillColor(kWhite)
            if (h1.GetName().startswith("elec") and h1.GetName().find("ex1")>-1):
                text3.AddText("e+jets N_{b tags}=1")
            if (h1.GetName().startswith("elec") and h1.GetName().find("ex2")>-1):
                text3.AddText("e+jets N_{b tags}=2")
            if (h1.GetName().startswith("mu") and h1.GetName().find("ex1")>-1):
                text3.AddText("#mu+jets N_{b tags}=1")
            if (h1.GetName().startswith("mu") and h1.GetName().find("ex2")>-1):
                text3.AddText("#mu+jets N_{b tags}=2")
            text3.SetBorderSize(0)
            text3.Draw()

            if (syst not in noUP): h2.Draw("same")
            if (syst not in noDOWN): h3.Draw("same")
            leg = TLegend(0.6,0.7,0.9,0.9)
            leg.AddEntry(h0,"Nominal","l")
            if (syst not in noUP): leg.AddEntry(h2,syst.upper()+" Up","l")
            if (syst not in noDOWN): leg.AddEntry(h3,syst.upper()+" Down","l")
            leg.Draw()

            pad2 = TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.23)
            pad2.SetTopMargin(0.0)
            pad2.SetBottomMargin(0.41)
            pad2.Draw()
            pad2.cd()
            pad2.SetGridy(1)

            if (syst not in noUP):
                ratio1.SetLineColor(2)
                ratio1.SetLineWidth(2)
                ratio1.SetStats(0)
                ratio1.Draw("E");
            if (syst not in noDOWN):
                ratio2.SetLineColor(4)
                ratio2.SetLineWidth(2)
                ratio2.SetStats(0)
                ratio2.Draw("ESAME");
            
            if (h1.GetName().endswith("000")):
                if (syst not in noUP):
                    ratio1.SetMinimum(-0.6)
                    ratio1.SetMaximum(0.6)
                else:
                    ratio2.SetMinimum(-0.6)
                    ratio2.SetMaximum(0.6)
            else:
                if (syst not in noUP):
                    ratio1.SetMinimum(-0.6)
                    ratio1.SetMaximum(1.)
                else:
                    ratio2.SetMinimum(-0.6)
                    ratio2.SetMaximum(1.)
            
            if (syst not in noUP): xAxis = ratio1.GetXaxis()
            else: xAxis = ratio2.GetXaxis()
            xAxis.SetTitle("p_{T}(#mu) [GeV]")
            xAxis.SetTitleFont(42)
            xAxis.SetTitleSize(0.185)
            xAxis.SetTitleOffset(1.0)
            xAxis.SetLabelFont(42)
            xAxis.SetLabelSize(0.15)
            xAxis.SetLabelOffset(0.007)
            
            if (syst not in noUP): yAxis2 = ratio1.GetYaxis()
            else: yAxis2 = ratio2.GetYaxis()
            yAxis2.SetNdivisions(7)
            yAxis2.SetTitle("#frac{sys-nom}{nom}")
            yAxis2.SetTitleSize(0.10)
            yAxis2.SetTitleFont(42)
            yAxis2.SetTitleOffset(0.32)
            yAxis2.SetLabelFont(42)
            yAxis2.SetLabelSize(0.12)
            yAxis2.SetLabelOffset(0.007)
            yAxis2.CenterTitle()
            
            if (h1.GetName().endswith("ttbar")):
                can.SaveAs(outdir+h1.GetName()[:-6]+syst+".pdf")
                can.SaveAs(outdir+h1.GetName()[:-6]+syst+".png")
            else:
                can.SaveAs(outdir+h1.GetName()+"_"+syst+".pdf")
                can.SaveAs(outdir+h1.GetName()+"_"+syst+".png")
            del can
                
