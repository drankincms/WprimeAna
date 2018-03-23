#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TObject.h"
#include "THStack.h"
#include<string.h>
#include<vector>
#include<algorithm>
using std::string;
using namespace std;

void Plotter(string channel, string histname, string suffix = "", bool manscale = false, bool normalize = false)
{

	TFile *file1 = new TFile("Run2016_Jan31/outFile.root");
	TH2D *hdata = new TH2D("hdata","hdata",1,0.,1.,1,0.,1.);
	TH2D *hmc = new TH2D("hmc","hmc",1,0.,1.,1,0.,1.);
	TH2D *htmp = new TH2D("htmp","htmp",1,0.,1.,1,0.,1.);

	hdata = (TH2D*)file1->Get(Form("h_%s_%s%s_Data",histname.c_str(),channel.c_str(),suffix.c_str()));
	hmc = (TH2D*)file1->Get(Form("h_%s_%s%s_TTbar",histname.c_str(),channel.c_str(),suffix.c_str()));
        hmc->SetTitle(histname.c_str());
	if (manscale) hmc->Scale(831.76);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT100to200_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,1629.87*0.998056);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT200to400_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,435.6*0.978569);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT400to600_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,59.2658*0.928054);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT600to800_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,14.5805*0.856705);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT800to1200_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,6.65621*0.757463);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT1200to2500_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,1.60809*0.608292);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT2500toInf_LF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,0.0389136*0.454246);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT100to200_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,1629.87*0.998056);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT200to400_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,435.6*0.978569);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT400to600_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,59.2658*0.928054);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT600to800_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,14.5805*0.856705);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT800to1200_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,6.65621*0.757463);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT1200to2500_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,1.60809*0.608292);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WJets_HT2500toInf_HF",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,0.0389136*0.454246);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_ZJets",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,6025.2);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_T_s",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,3.44);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_T_t",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,45.34);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_Tbar_t",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,26.98);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_T_tW",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,35.6);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_Tbar_tW",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,35.6);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WW",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,115.0);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_WZ",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,47.3);
	htmp = (TH2D*)file1->Get(Form("h_%s_%s%s_ZZ",histname.c_str(),channel.c_str(),suffix.c_str()));
	if (manscale) hmc->Add(htmp,16.523);

        if (normalize) hdata->Scale(1/hdata->Integral());
	//hdata->RebinY(10);
	//hdata->RebinX(2);
        if (normalize) hmc->Scale(1/hmc->Integral());
	//hmc->RebinY(10);
	//hmc->RebinX(2);

	TH2D *hcomppos = (TH2D*)hmc->Clone();
	hcomppos->Reset();
	TH2D *hcompneg = (TH2D*)hmc->Clone();
	hcompneg->Reset();
	TH2D *hcomp = (TH2D*)hmc->Clone();
	hcomp->Reset();
	TH2D *hratio = (TH2D*)hmc->Clone();
	hratio->Reset();

        Double_t maxval = -999999.;
        Double_t minval = 999999.;
        Double_t maxrat = 10.;
        Double_t minrat = 0.1;

	for (int i = 1; i<=hcomppos->GetNbinsX(); i++) {
		for (int j = 1; j<=hcomppos->GetNbinsY(); j++) {
                        Double_t tmp = hdata->GetBinContent(i,j)-hmc->GetBinContent(i,j);
			if (tmp > maxval) maxval = tmp;
			if (tmp < minval) minval = tmp;
			if (tmp>0.) hcomppos->SetBinContent(i,j,tmp);
			if (tmp<0.) hcompneg->SetBinContent(i,j,-tmp);
			if (tmp==0. and hdata->GetBinContent(i,j)==0.) hcomp->SetBinContent(i,j,-999999.);
			else hcomp->SetBinContent(i,j,tmp);
			
			if (hmc->GetBinContent(i,j)==0. or hdata->GetBinContent(i,j)==0.) hratio->SetBinContent(i,j,-999999.);
			else {
                        	Double_t tmpdiv = hdata->GetBinContent(i,j)/hmc->GetBinContent(i,j);
				hratio->SetBinContent(i,j,tmpdiv);
				//if (tmpdiv > maxrat && tmpdiv < 10.) maxrat = tmpdiv;
				//if (tmpdiv < minrat) minrat = tmpdiv;
			}
		}
	}

	//std::cout<<"min = "<<hcomp->GetMinimum()<<" , max = "<<hcomp->GetMaximum()<<std::endl;
	TCanvas *cst = new TCanvas("cst","cst",1000,700);
	gStyle->SetOptStat(0);
	gStyle->SetPalette(55);
	cst->SetLogz();
	//cst->SetLogy();
	//cst->SetLogx();

	hcomppos->Draw("colz");
	//cst->SaveAs(Form("%s_%s%s_data_vs_mc_pos.png",histname.c_str(),channel.c_str(),suffix.c_str()));
	cst->Clear();
	hcompneg->Draw("colz");
	//cst->SaveAs(Form("%s_%s%s_data_vs_mc_neg.png",histname.c_str(),channel.c_str(),suffix.c_str()));
	cst->Clear();
	cst->SetLogz(0);
	//Double_t levels[] = {-0.002, -0.001, -0.0009, -0.0008, -0.0007, -0.0006, -0.0005, -0.0004, -0.0003, -0.0002, -0.0001, -0.00001, 0.00001, 0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.001, 0.002, 0.003, 0.004, 0.005, 0.006, 0.007, 0.008, 0.009, 0.01, 0.1};
        const int NLVL = 51;
	Double_t levels[NLVL];
        for (int i = 0; i < NLVL; i++) {
		levels[i] = minval + (maxval-minval)*(Double_t)(i)/((Double_t)(NLVL));
		//std::cout << i << " " << levels[i] << std::endl;
	}
	hcomp->SetContour((sizeof(levels)/sizeof(Double_t)),levels);
	hcomp->Draw("colz");
        hcomp->GetZaxis()->SetRangeUser(minval,maxval);
	cst->SaveAs(Form("%s_%s%s_data_vs_mc.png",histname.c_str(),channel.c_str(),suffix.c_str()));
	//gStyle->SetPalette(56);
	const Int_t Number = 4;
	Double_t Red[Number]    = { 1.00, 0.00, 1.00, 1.00};
	Double_t Green[Number]  = { 1.00, 0.00, 1.00, 0.00};
	Double_t Blue[Number]   = { 1.00, 1.00, 0.00, 0.00};
	Double_t Length[Number] = { 0.00, 0.46, 0.54, 1.00 };
	Int_t nb=50;
	TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

        for (int i = 0; i < NLVL; i++) {
		//levels[i] = minrat + (maxrat-minrat)*(log((exp(1)-1.)*((Double_t)(i))/((Double_t)(NLVL))+1.));
		levels[i] = minrat + (maxrat-minrat)*(Double_t)(i)/((Double_t)(NLVL));
	}
	//hratio->SetContour((sizeof(levels)/sizeof(Double_t)),levels);
	hratio->SetContour(nb);
	cst->SetLogz();
	hratio->Draw("colz");
        hratio->GetZaxis()->SetRangeUser(minrat,maxrat);
	//std::cout<<minrat<<endl;
	cst->SaveAs(Form("%s_%s%s_data_vs_mc_ratio.png",histname.c_str(),channel.c_str(),suffix.c_str()));

	delete cst;
}

void dataMChistComp()
{
	Plotter("Mu","topM_vs_MinLepAK4JetDR");
	Plotter("El","topM_vs_MinLepAK4JetDR");
 	Plotter("El","topM_vs_MET");
 	Plotter("Mu","topM_vs_MET");
 	Plotter("El","topM_vs_LepPt");
 	Plotter("Mu","topM_vs_LepPt");
 	Plotter("El","topM_vs_LepEta");
 	Plotter("Mu","topM_vs_LepEta");
 	Plotter("El","topM_vs_J1Pt");
 	Plotter("Mu","topM_vs_J1Pt");
 	Plotter("El","topM_vs_J2Pt");
 	Plotter("Mu","topM_vs_J2Pt");
 	Plotter("El","topM_vs_TopJetPt");
 	Plotter("Mu","topM_vs_TopJetPt");
 	Plotter("El","topM_vs_TopJetEta");
 	Plotter("Mu","topM_vs_TopJetEta");
 	Plotter("El","topM_vs_Mtb");
 	Plotter("Mu","topM_vs_Mtb");
 	Plotter("El","topM_vs_Njets");
 	Plotter("Mu","topM_vs_Njets");
 	Plotter("El","MinLepAK4JetDR_vs_Mtb_0b");
 	Plotter("Mu","MinLepAK4JetDR_vs_Mtb_0b");
 	Plotter("El","MinLepAK4JetDR_vs_Mtb_1b");
 	Plotter("Mu","MinLepAK4JetDR_vs_Mtb_1b");
 	Plotter("El","MinLepAK4JetDR_vs_Mtb_2b");
 	Plotter("Mu","MinLepAK4JetDR_vs_Mtb_2b");
        Plotter("El","LepdR_vs_pTRel","_PostSel");
        Plotter("Mu","LepdR_vs_pTRel","_PostSel");
        Plotter("El","topPt_vs_Mtb");
        Plotter("Mu","topPt_vs_Mtb");
}
