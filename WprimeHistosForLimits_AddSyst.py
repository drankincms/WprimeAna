import ROOT, sys, os, re, string
from ROOT import *

from array import array
import copy
import math


#indir = "Run2016BCD_Jul27_Slim/RootFiles_For2DLimits/"
indir = "Run2016_Feb27_Slim/RootFiles_For2DLimits/"

#All ------ TTbar ------
ttpdfasup = [ 1, 1.0226 , 1.02443 , 1.02785 , 1.03115 , 1.03426 , 1.03791 , 1.04194 , 1.04651 , 1.04915 , 1.05602 , 1.06822 , 1.09506 ]
ttpdfasdown = [ 1, 0.97648 , 0.974929 , 0.97195 , 0.96875 , 0.965783 , 0.962237 , 0.958053 , 0.953872 , 0.950922 , 0.944704 , 0.931775 , 0.900269 ]

#All ------ WJets ------
wjpdfasup = [ 1, 1.05454 , 1.05448 , 1.05544 , 1.05676 , 1.05848 , 1.06029 , 1.06268 , 1.06479 , 1.0671 , 1.06921 , 1.07288 , 1.07943 ]
wjpdfasdown = [ 1, 0.943789 , 0.944106 , 0.943239 , 0.942025 , 0.940188 , 0.937666 , 0.934148 , 0.930491 , 0.927018 , 0.923885 , 0.91838 , 0.907974 ]


#ttpdfasup = [1.+(x-1.)*0.5 for x in ttpdfasup] 
#ttpdfasdown = [1.+(x-1.)*0.5 for x in ttpdfasdown] 
#wjpdfasup = [1.+(x-1.)*0.5 for x in wjpdfasup] 
#wjpdfasdown = [1.+(x-1.)*0.5 for x in wjpdfasdown] 

#All ------ Wprime1500Right ------
wp1500pdfasup = [ 1, 1.0952 , 1.08697 , 1.08668 , 1.08792 , 1.08795 , 1.08895 , 1.09009 , 1.09109 , 1.09066 , 1.08751 , 1.09284 , 1.08238 ]
wp1500pdfasdown = [ 1, 0.972123 , 0.975323 , 0.975276 , 0.974603 , 0.973735 , 0.973286 , 0.972519 , 0.971682 , 0.969566 , 0.966436 , 0.966453 , 0.969858 ]

#All ------ Wprime2000Right ------
wp2000pdfasup = [ 1, 1.10881 , 1.0901 , 1.08859 , 1.09197 , 1.09517 , 1.09472 , 1.09655 , 1.09692 , 1.09814 , 1.09945 , 1.10106 , 1.11211 ]
wp2000pdfasdown = [ 1, 0.958141 , 0.967919 , 0.967557 , 0.963779 , 0.959931 , 0.958212 , 0.955096 , 0.954128 , 0.953919 , 0.95042 , 0.949812 , 0.953747 ]

#All ------ Wprime2500Right ------
wp2500pdfasup = [ 1, 1.11378 , 1.09003 , 1.09239 , 1.095 , 1.09859 , 1.10241 , 1.1031 , 1.10518 , 1.10567 , 1.10663 , 1.10893 , 1.11056 ]
wp2500pdfasdown = [ 1, 0.949236 , 0.96205 , 0.962216 , 0.959444 , 0.951371 , 0.941264 , 0.937858 , 0.934508 , 0.932671 , 0.929647 , 0.928223 , 0.925005 ]

#wp1500pdfasup = [1.+(x-1.)*0.5 for x in wp1500pdfasup] 
#wp1500pdfasdown = [1.+(x-1.)*0.5 for x in wp1500pdfasdown] 
#wp2000pdfasup = [1.+(x-1.)*0.5 for x in wp2000pdfasup] 
#wp2000pdfasdown = [1.+(x-1.)*0.5 for x in wp2000pdfasdown] 
#wp2500pdfasup = [1.+(x-1.)*0.5 for x in wp2500pdfasup] 
#wp2500pdfasdown = [1.+(x-1.)*0.5 for x in wp2500pdfasdown] 

for fn in os.listdir(indir):
     if (fn[-4:]=="root"):
         file0 = TFile(indir+fn,"UPDATE")
         file0.cd()
         dirList = gDirectory.GetListOfKeys()
         for k1 in dirList: 
             h1 = k1.ReadObj()
             if (h1.ClassName() == "TH1F"):
                 if (h1.GetName().endswith("DATA") or h1.GetName().endswith("plus") or h1.GetName().endswith("minus")):
                     #if (h1.GetName().endswith("00__murmuf__plus") or h1.GetName().endswith("00__murmuf__minus")):
                     #    gDirectory.Delete(h1.GetName()+";1")
                     continue
                 if (h1.GetName().endswith("ttbar")):
                     h2 = h1.Clone("h2")
                     h3 = h1.Clone("h3")
                     for ibin in range(1,h1.GetNbinsX()+1):
                         h2.SetBinContent(ibin,h2.GetBinContent(ibin)*ttpdfasup[ibin])
                         h3.SetBinContent(ibin,h3.GetBinContent(ibin)*ttpdfasdown[ibin])
                     #print h2.Integral()," ",h3.Integral()," ",h1.Integral()
                     h2.SetName(h1.GetName()+"__pdfas__plus")
                     h2.SetTitle(h1.GetTitle()+"__pdfas__plus")
                     h3.SetName(h1.GetName()+"__pdfas__minus")
                     h3.SetTitle(h1.GetTitle()+"__pdfas__minus")
                     h2.Write()
                     h3.Write()
                         
                 elif (h1.GetName().endswith("wjets")):
                     h2 = h1.Clone("h2")
                     h3 = h1.Clone("h3")
                     for ibin in range(1,h1.GetNbinsX()):
                         h2.SetBinContent(ibin,h2.GetBinContent(ibin)*wjpdfasup[ibin])
                         h3.SetBinContent(ibin,h3.GetBinContent(ibin)*wjpdfasdown[ibin])
                     #print h2.Integral()," ",h3.Integral()," ",h1.Integral()
                     h2.SetName(h1.GetName()+"__pdfas__plus")
                     h2.SetTitle(h1.GetTitle()+"__pdfas__plus")
                     h3.SetName(h1.GetName()+"__pdfas__minus")
                     h3.SetTitle(h1.GetTitle()+"__pdfas__minus")
                     h2.Write()
                     h3.Write()
                         
                 elif (h1.GetName().endswith("00")):
                     sigpdfasup = []
                     sigpdfasdown = []
                     if (int(h1.GetName()[-4:])<=1500):
                         sigpdfasup = wp1500pdfasup
                         sigpdfasdown = wp1500pdfasdown
                     elif (int(h1.GetName()[-4:])<=2000):
                         sigpdfasup = wp2000pdfasup
                         sigpdfasdown = wp2000pdfasdown
                     else:
                         sigpdfasup = wp2500pdfasup
                         sigpdfasdown = wp2500pdfasdown
                     h2 = h1.Clone("h2")
                     h3 = h1.Clone("h3")
                     for ibin in range(1,h1.GetNbinsX()):
                         h2.SetBinContent(ibin,h2.GetBinContent(ibin)*sigpdfasup[ibin])
                         h3.SetBinContent(ibin,h3.GetBinContent(ibin)*sigpdfasdown[ibin])
                     #print h2.Integral()," ",h3.Integral()," ",h1.Integral()
                     if (h2.Integral()>0): h2.Scale(h1.Integral()/h2.Integral())
                     if (h3.Integral()>0): h3.Scale(h1.Integral()/h3.Integral())
                     h2.SetName(h1.GetName()+"__sigpdfas__plus")
                     h2.SetTitle(h1.GetTitle()+"__sigpdfas__plus")
                     h3.SetName(h1.GetName()+"__sigpdfas__minus")
                     h3.SetTitle(h1.GetTitle()+"__sigpdfas__minus")
                     h2.Write()
                     h3.Write()
                         
                 else:
                     h2 = h1.Clone("h2")
                     h3 = h1.Clone("h3")
                     h2.SetName(h1.GetName()+"__pdfas__plus")
                     h2.SetTitle(h1.GetTitle()+"__pdfas__plus")
                     h3.SetName(h1.GetName()+"__pdfas__minus")
                     h3.SetTitle(h1.GetTitle()+"__pdfas__minus")
                     h2.Write()
                     h3.Write()

'''for fn in os.listdir(indir):
     if (fn[-4:]=="root"):
         file0 = TFile(indir+fn,"UPDATE")
         file0.cd()
         dirList = gDirectory.GetListOfKeys()
         for k1 in dirList: 
             h1 = k1.ReadObj()
             if (h1.ClassName() == "TH1F"):
                 if (not h1.GetName().endswith("00")):
                     continue
                 h4 = h1.Clone("h4")
                 h5 = h1.Clone("h5")
                 for ibin in range(1,h1.GetNbinsX()):
                     h4.SetBinContent(ibin,h4.GetBinContent(ibin)*(1+float(h1.GetName()[-4:])*0.00004))
                     h5.SetBinContent(ibin,h5.GetBinContent(ibin)*(1-float(h1.GetName()[-4:])*0.00004))
                 h4.SetName(h1.GetName()+"__murmuf__plus")
                 h4.SetTitle(h1.GetTitle()+"__murmuf__plus")
                 h5.SetName(h1.GetName()+"__murmuf__minus")
                 h5.SetTitle(h1.GetTitle()+"__murmuf__minus")
                 h4.Write()
                 h5.Write()'''
