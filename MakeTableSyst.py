import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math

base = "Run2016_Feb27_Slim/"
name = base+"RootFiles_For2DLimits/comb_mWprime_Limits_WprimeRight_HardSoft_Histos-ex1or2btags.root"
outdir = base+"SystVarPlots/"
os.system('mkdir -p '+outdir)

systlist = ["jes","jer","btag","mtag","wjhf","toppt","pileup","pdfas","murmuf"]#,"q2scale","mues","muer"]
SystName = ["J.E.S.","J.E.R.","B-Tag","Mistag","H.F.","Top $p_{T}$","PU","PDF","$Q^2$ ME Scale"]#,"$Q^2$ PM Scale","Muon Energy Scale","Muon Energy Resolution"]
#systlist = ["jes","jer","btag","mtag","wjhf","toppt","pileup","pdfas","murmuf"]#,"q2scale"],"mues","muer"]
#SystName = ["J.E.S.","J.E.R.","B-Tag","Mistag","H.F.","Top $p_{T}$","PU","PDF","$Q^2$ ME Scale"]#,"$Q^2$ PM Scale"],"Muon Energy Scale","Muon Energy Resolution"]
#systlist = ["toppt","pileup","pdfas","murmuf","q2scale"]
#SystName = ["Top $p_{T}$","PU","PDF","$Q^2$ ME Scale","$Q^2$ PM Scale"]

noDOWN = ["toppt","muer"]
noUP = []

ex1muA = []
ex2muA = []
ex1elA = []
ex2elA = []
ex1muB = []
ex2muB = []
ex1elB = []
ex2elB = []

ROOT.gROOT.SetBatch(ROOT.kTRUE)

file0 = TFile(name)
file0.cd()
dirList = gDirectory.GetListOfKeys()
for k1 in dirList: 
    h1 = k1.ReadObj()
    if (h1.ClassName() == "TH1F"):
        if (h1.GetName().endswith("DATA") or h1.GetName().endswith("plus") or h1.GetName().endswith("minus")):
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
                if (h1.GetName().find("soft")>-1) :
                    if (h1.GetName().startswith("elec") and h1.GetName().find("ex1")>-1):
	                ex1elA.append(wuprat)
	                ex1elA.append(wdownrat)
	                ex1elA.append(tuprat)
	                ex1elA.append(tdownrat)
                    if (h1.GetName().startswith("elec") and h1.GetName().find("ex2")>-1):
	                ex2elA.append(wuprat)
	                ex2elA.append(wdownrat)
	                ex2elA.append(tuprat)
	                ex2elA.append(tdownrat)
                    if (h1.GetName().startswith("mu") and h1.GetName().find("ex1")>-1):
	                ex1muA.append(wuprat)
	                ex1muA.append(wdownrat)
	                ex1muA.append(tuprat)
	                ex1muA.append(tdownrat)
                    if (h1.GetName().startswith("mu") and h1.GetName().find("ex2")>-1):
	                ex2muA.append(wuprat)
	                ex2muA.append(wdownrat)
	                ex2muA.append(tuprat)
	                ex2muA.append(tdownrat)
                if (h1.GetName().find("hard")>-1) :
                    if (h1.GetName().startswith("elec") and h1.GetName().find("ex1")>-1):
	                ex1elB.append(wuprat)
	                ex1elB.append(wdownrat)
	                ex1elB.append(tuprat)
	                ex1elB.append(tdownrat)
                    if (h1.GetName().startswith("elec") and h1.GetName().find("ex2")>-1):
	                ex2elB.append(wuprat)
	                ex2elB.append(wdownrat)
	                ex2elB.append(tuprat)
	                ex2elB.append(tdownrat)
                    if (h1.GetName().startswith("mu") and h1.GetName().find("ex1")>-1):
	                ex1muB.append(wuprat)
	                ex1muB.append(wdownrat)
	                ex1muB.append(tuprat)
	                ex1muB.append(tdownrat)
                    if (h1.GetName().startswith("mu") and h1.GetName().find("ex2")>-1):
	                ex2muB.append(wuprat)
	                ex2muB.append(wdownrat)
	                ex2muB.append(tuprat)
	                ex2muB.append(tdownrat)
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
            h0.SetMinimum(0.5)
            maxi = h0.GetMaximum() + math.sqrt(h0.GetMaximum())
            maxi = maxi + math.sqrt(maxi)
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

            #text1 = TLatex(1.570061,23.08044,"#scale[0.6]{CMS Preliminary   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}")
            text1 = TLatex(1.570061,23.08044,"#scale[0.6]{                   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}")
            text1.SetNDC()
            text1.SetTextAlign(13)
            text1.SetX(0.35)
            text1.SetY(0.93)
            text1.SetTextFont(62)
            text1.SetTextSizePixels(15)
            text1.Draw()

            h0.GetXaxis().SetLabelOffset(999)
            h0.GetXaxis().SetLabelSize(0)
            h0.GetXaxis().SetTitle("M(tb)")

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

            h0.SetMaximum(maxi*6)
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
            xAxis.SetTitle("M(tb)")
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


for x in range (0,len(systlist)):
    print SystName[x],
    print (abs(((ex1muA[x*4]-ex1muA[x*4+1])/2.))*0.2+abs(((ex1muA[x*4+2]-ex1muA[x*4+3])/2.))*0.2+abs(((ex2muA[x*4]-ex2muA[x*4+1])/2.))*0.03+abs(((ex2muA[x*4+2]-ex2muA[x*4+3])/2.))*0.07+abs(((ex1elA[x*4]-ex1elA[x*4+1])/2.))*0.2+abs(((ex1elA[x*4+2]-ex1elA[x*4+3])/2.))*0.2+abs(((ex2elA[x*4]-ex2elA[x*4+1])/2.))*0.03+abs(((ex2elA[x*4+2]-ex2elA[x*4+3])/2.))*0.07)*0.5 + (abs(((ex1muB[x*4]-ex1muB[x*4+1])/2.))*0.2+abs(((ex1muB[x*4+2]-ex1muB[x*4+3])/2.))*0.2+abs(((ex2muB[x*4]-ex2muB[x*4+1])/2.))*0.03+abs(((ex2muB[x*4+2]-ex2muB[x*4+3])/2.))*0.07+abs(((ex1elB[x*4]-ex1elB[x*4+1])/2.))*0.2+abs(((ex1elB[x*4+2]-ex1elB[x*4+3])/2.))*0.2+abs(((ex2elB[x*4]-ex2elB[x*4+1])/2.))*0.03+abs(((ex2elB[x*4+2]-ex2elB[x*4+3])/2.))*0.07)*0.5
        
print "\n\n"
                
print "\\begin{table}[h]"
print "\\begin{center}"
print "\\caption{Fractional change in rate from shape systematics in the 1 b-tag channel for Type A events.}"
print "\\label{table:1bsysA}"
print "\\begin{tabular}{l|clll|llll}"
print "& \\mc{4}{c}{$V$+jets} & \\mc{4}{c}{$t\\bar{t}$+single-$t$}\\\\\\hline\\hline"
print "& \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets} & \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets}\\\\\\hline"
print "Systematic & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$}\\\\\\hline"
for x in range (0,len(systlist)):
    for j in range(0,4):
        if (ex1muA[x*4+j]==1.): ex1muA[x*4+j]="\\textemdash"
        else: ex1muA[x*4+j]='%.4f' % (ex1muA[x*4+j])
        if (ex1elA[x*4+j]==1.): ex1elA[x*4+j]="\\textemdash"
        else: ex1elA[x*4+j]='%.4f' % (ex1elA[x*4+j])
    print SystName[x],"& \\mc{1}{c}{",ex1muA[x*4],"} & \\mc{1}{c}{",ex1muA[x*4+1],"} & \\mc{1}{c}{",ex1elA[x*4],"} & \\mc{1}{c}{",ex1elA[x*4+1],"} & \\mc{1}{c}{",ex1muA[x*4+2],"} & \\mc{1}{c}{",ex1muA[x*4+3],"} & \\mc{1}{c}{",ex1elA[x*4+2],"} & \\mc{1}{c}{",ex1elA[x*4+3],"}\\\\\\hline"
print "\\end{tabular}"
print "\\end{center}"
print "\\end{table}"

print "\n\n"
   
print "\\begin{table}[h]"
print "\\begin{center}"
print "\\caption{Fractional change in rate from shape systematics in the 2 b-tag channel for Type A events.}"
print "\\label{table:2bsysA}"
print "\\begin{tabular}{l|clll|llll}"
print "& \\mc{4}{c}{$V$+jets} & \\mc{4}{c}{$t\\bar{t}$+single-$t$}\\\\\\hline\\hline"
print "& \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets} & \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets}\\\\\\hline"
print "Systematic & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$}\\\\\\hline"
for x in range (0,len(systlist)):
    for j in range(0,4):
        if (ex2muA[x*4+j]==1.): ex2muA[x*4+j]="\\textemdash"
        else: ex2muA[x*4+j]='%.4f' % (ex2muA[x*4+j])
        if (ex2elA[x*4+j]==1.): ex2elA[x*4+j]="\\textemdash"
        else: ex2elA[x*4+j]='%.4f' % (ex2elA[x*4+j])
    print SystName[x],"& \\mc{1}{c}{",ex2muA[x*4],"} & \\mc{1}{c}{",ex2muA[x*4+1],"} & \\mc{1}{c}{",ex2elA[x*4],"} & \\mc{1}{c}{",ex2elA[x*4+1],"} & \\mc{1}{c}{",ex2muA[x*4+2],"} & \\mc{1}{c}{",ex2muA[x*4+3],"} & \\mc{1}{c}{",ex2elA[x*4+2],"} & \\mc{1}{c}{",ex2elA[x*4+3],"}\\\\\\hline"
print "\\end{tabular}"
print "\\end{center}"
print "\\end{table}"

print "\n\n"

print "\\begin{table}[h]"
print "\\begin{center}"
print "\\caption{Fractional change in rate from shape systematics in the 1 b-tag channel for Type B events.}"
print "\\label{table:1bsysB}"
print "\\begin{tabular}{l|clll|llll}"
print "& \\mc{4}{c}{$V$+jets} & \\mc{4}{c}{$t\\bar{t}$+single-$t$}\\\\\\hline\\hline"
print "& \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets} & \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets}\\\\\\hline"
print "Systematic & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$}\\\\\\hline"
for x in range (0,len(systlist)):
    for j in range(0,4):
        if (ex1muB[x*4+j]==1.): ex1muB[x*4+j]="\\textemdash"
        else: ex1muB[x*4+j]='%.4f' % (ex1muB[x*4+j])
        if (ex1elB[x*4+j]==1.): ex1elB[x*4+j]="\\textemdash"
        else: ex1elB[x*4+j]='%.4f' % (ex1elB[x*4+j])
    print SystName[x],"& \\mc{1}{c}{",ex1muB[x*4],"} & \\mc{1}{c}{",ex1muB[x*4+1],"} & \\mc{1}{c}{",ex1elB[x*4],"} & \\mc{1}{c}{",ex1elB[x*4+1],"} & \\mc{1}{c}{",ex1muB[x*4+2],"} & \\mc{1}{c}{",ex1muB[x*4+3],"} & \\mc{1}{c}{",ex1elB[x*4+2],"} & \\mc{1}{c}{",ex1elB[x*4+3],"}\\\\\\hline"
print "\\end{tabular}"
print "\\end{center}"
print "\\end{table}"

print "\n\n"
   
print "\\begin{table}[h]"
print "\\begin{center}"
print "\\caption{Fractional change in rate from shape systematics in the 2 b-tag channel for Type B events.}"
print "\\label{table:2bsysB}"
print "\\begin{tabular}{l|clll|llll}"
print "& \\mc{4}{c}{$V$+jets} & \\mc{4}{c}{$t\\bar{t}$+single-$t$}\\\\\\hline\\hline"
print "& \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets} & \\mc{2}{c}{$\\mu$+jets} & \\mc{2}{c}{$e$+jets}\\\\\\hline"
print "Systematic & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$} & \\mc{1}{c}{+$1\\sigma$} & \\mc{1}{c}{-$1\\sigma$}\\\\\\hline"
for x in range (0,len(systlist)):
    for j in range(0,4):
        if (ex2muB[x*4+j]==1.): ex2muB[x*4+j]="\\textemdash"
        else: ex2muB[x*4+j]='%.4f' % (ex2muB[x*4+j])
        if (ex2elB[x*4+j]==1.): ex2elB[x*4+j]="\\textemdash"
        else: ex2elB[x*4+j]='%.4f' % (ex2elB[x*4+j])
    print SystName[x],"& \\mc{1}{c}{",ex2muB[x*4],"} & \\mc{1}{c}{",ex2muB[x*4+1],"} & \\mc{1}{c}{",ex2elB[x*4],"} & \\mc{1}{c}{",ex2elB[x*4+1],"} & \\mc{1}{c}{",ex2muB[x*4+2],"} & \\mc{1}{c}{",ex2muB[x*4+3],"} & \\mc{1}{c}{",ex2elB[x*4+2],"} & \\mc{1}{c}{",ex2elB[x*4+3],"}\\\\\\hline"
print "\\end{tabular}"
print "\\end{center}"
print "\\end{table}"

