#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TObject.h"
#include "THStack.h"
#include<string.h>
#include<vector>
#include<algorithm>
using std::string;
using namespace std;

void getBTagEff()
{

	TFile *file1 = new TFile("Run2016B_Jul11/outFile.root");
	TH2F *htmpd = new TH2F("htmpd","htmpd",1,0.,1.,1,0.,1.);
	TH2F *htmpn = new TH2F("htmpn","htmpn",1,0.,1.,1,0.,1.);

	std::vector<string> sampset;
	sampset.push_back("TTbar");
	sampset.push_back("WJets_HT400to600_HF");
	sampset.push_back("WJets_HT600to800_HF");
	sampset.push_back("WJets_HT800to1200_HF");
	sampset.push_back("WJets_HT1200to2500_HF");
	sampset.push_back("WJets_HT2500toInf_HF");
	sampset.push_back("WJets_HT400to600_LF");
	sampset.push_back("WJets_HT600to800_LF");
	sampset.push_back("WJets_HT800to1200_LF");
	sampset.push_back("WJets_HT1200to2500_LF");
	sampset.push_back("WJets_HT2500toInf_LF");
	sampset.push_back("ZJets");

	std::vector<string> systset;
	systset.push_back("EffB");
	systset.push_back("EffL");

	double lowbins[] = {0.0,30.0,40.0,50.0,60.0,70.0,100.0,140.0,200.0,300.0,670.0,1000.0};
	std::vector<double> xlow (lowbins, lowbins + sizeof(lowbins) / sizeof(double) );
	
	/*for (unsigned int i = 0; i < sampset.size(); i++) {
		std::cout<<"------ "<<sampset[i]<<" ------"<<std::endl;
		for (unsigned int k = 0; k < systset.size(); k++) {
			std::cout<<"Systematic: "<<systset[k]<<std::endl;
			htmp = (TH2F*)file1->Get(Form("h_%s_vs_Mtb_%s",systset[k].c_str(),sampset[i].c_str()));
			for (unsigned int j = 0; j < xlow.size()-1; j++) {
				htmp->GetYaxis()->SetRange(xlow[j],xlow[j+1]);
				std::cout<<"\tFor range "<<xlow[j]<<" to "<<xlow[j+1]<<" , RMS = "<<htmp->GetStdDev()<<std::endl;
			}
		}
	}*/
	for (unsigned int k = 0; k < systset.size(); k++) {
		TCanvas *can = new TCanvas("can","can");
		//can->SetLogy();
		for (unsigned int i = 0; i < sampset.size(); i++) {
			std::cout<<"\n"<<sampset[i]<<" "<<systset[k]<<" ------"<<std::endl;
			//std::cout<<"Systematic: "<<systset[k]<<std::endl;
			htmpd = (TH2F*)file1->Get(Form("h_%sDen_%s",systset[k].c_str(),sampset[i].c_str()));
			TH1D* pxd = htmpd->ProjectionX("pxd");
			htmpn = (TH2F*)file1->Get(Form("h_%sNum_%s",systset[k].c_str(),sampset[i].c_str()));
			TH1D* pxn = htmpn->ProjectionX("pxn");

			TH1D* effhn = (TH1D*)pxn->Rebin(xlow.size()-1,"effhn",lowbins);
			TH1D* effhd = (TH1D*)pxd->Rebin(xlow.size()-1,"effhd",lowbins);
			effhn->Divide(effhd);
			effhn->SetLineColor(i+1);
			effhn->SetMinimum(effhn->GetMinimum()*0.5);
			
			effhn->Draw("same");
			effhn->GetXaxis()->SetRangeUser(10.,1000.);

			int binlo = 1;
			int binhi = 1;
			for (unsigned int j = 1; j < xlow.size(); j++) {
				binhi = pxd->GetNbinsX();
				for (int n = 1; n <= pxd->GetNbinsX(); n++) {
					if (pxd->GetBinLowEdge(n)>=xlow[j]) {
						binhi = n-1;
						break;
					}
				}
				//std::cout<<"binlo = "<<binlo<<"   binhi = "<<binhi<<std::endl;
				//std::cout<<"  xlo = "<<pxd->GetBinLowEdge(binlo)<<"     xhi = "<<pxd->GetBinLowEdge(binhi)<<std::endl;
				//std::cout<<"\tFor range "<<xlow[j-1]<<" to "<<xlow[j]<<" , Mean = "<<pxd->GetMean()<<" and RMS = "<<pxd->GetStdDev()<<std::endl;
				//std::cout<<pxn->Integral(binlo,binhi)/pxd->Integral(binlo,binhi);
				std::cout<<pxn->Integral(binlo,binhi)/pxd->Integral(binlo,binhi)<<" ("<<xlow[j]<<")";
				if (j != xlow.size()-1) std::cout<<", ";
				binlo=binhi+1;
			}
		}
		can->SetLogx();
		can->SaveAs(Form("%s_All.png",systset[k].c_str()));
	}
	std::cout<<std::endl;

}
