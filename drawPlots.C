#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TObject.h"
#include "THStack.h"
#include<string.h>
#include<vector>
#include<algorithm>
using std::string;
using namespace std;

void plot(std::string chan, std::string sample)
{

	TFile *file1 = new TFile("Run2016_Jan31/outFile.root");
	TH2D *htmp = new TH2D("htmp","htmp",1,0.,1.,1,0.,1.);
	gStyle->SetOptStat(0);

	htmp = (TH2D*)file1->Get(Form("h_LepdR_vs_pTRel_%s_PreSel_%s",chan.c_str(),sample.c_str()));
	//htmp = (TH2D*)file1->Get(Form("h_LepdR_vs_pTRel_%s_PostSel_%s",chan.c_str(),sample.c_str()));
	htmp->RebinY(4);
	htmp->SetTitle("");
	htmp->GetXaxis()->SetRangeUser(0,1.5);
	htmp->GetXaxis()->SetTitle("min#DeltaR(lepton, closest jet)");
	htmp->GetYaxis()->SetRangeUser(0,100);
	htmp->GetYaxis()->SetTitle("p_{T}^{rel}(lepton, closest jet)");

	TCanvas *can = new TCanvas("can","can");
	//if(chan=="Mu") can->SetLogz();
	can->SetLogz();
	htmp->Draw("colz");

	std::string suff;
	if (sample=="Data") suff = "Preliminary";
	else suff = "Simulation";
	TLatex* text1 = new TLatex(1.570061,23.08044,Form("#scale[0.85]{CMS %s   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}",suff.c_str()));
	text1->SetNDC();
	text1->SetTextAlign(13);
	text1->SetX(0.35);
	text1->SetY(0.96);
	text1->SetTextFont(62);
	text1->SetTextSizePixels(20);
	text1->Draw();

	TPaveText text3(0.65, 0.15, 0.75, 0.2,"NDC");
	text3.SetFillColor(kWhite);
	if (chan=="El") text3.AddText("e+jets");
	else text3.AddText("#mu+jets");
	text3.SetBorderSize(0);
	text3.Draw();

	can->SaveAs(Form("Dist2Dcut_%s_%s.pdf",chan.c_str(),sample.c_str()));
	//can->SaveAs(Form("Dist2Dcut_PostSel_%s_%s.pdf",chan.c_str(),sample.c_str()));

}

void drawPlots() {
	plot("El","Data");
	plot("Mu","Data");
	plot("El","QCD_Pt_1000to1400");
	plot("Mu","QCD_Pt_1000to1400");
	plot("El","QCD_Pt_800to1000");
	plot("Mu","QCD_Pt_800to1000");
	plot("El","QCD_Pt_600to800");
	plot("Mu","QCD_Pt_600to800");
	plot("El","Wprime2500Right");
	plot("Mu","Wprime2500Right");
	plot("El","TTbar");
	plot("Mu","TTbar");
	plot("El","WJets_HT600to800_LF");
	plot("Mu","WJets_HT600to800_LF");
	plot("El","WJets_HT600to800_HF");
	plot("Mu","WJets_HT600to800_HF");
}
