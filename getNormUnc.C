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

void getNormUnc()
{

	TFile *file1 = new TFile("Run2015D_Oct25/outFile.root");
	TH1F *hmce = new TH1F("hmce","hmce",1,0.,1.);
	TH1F *hmcm = new TH1F("hmcm","hmcm",1,0.,1.);
	TH1F *htmp = new TH1F("htmp","htmp",1,0.,1.);

	hmcm = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_TTbar_Mu");
	hmce = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_TTbar_El");
	/*htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WJets_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WJets_El");
	hmce->Add(htmp);*/
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WJets_HT100to200_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WJets_HT100to200_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WJets_HT200to400_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WJets_HT200to400_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WJets_HT400to600_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WJets_HT400to600_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WJets_HT600toInf_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WJets_HT600toInf_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_ZJets_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_ZJets_El");
	hmce->Add(htmp);
	/*htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_120to170_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_120to170_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_170to300_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_170to300_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_300to470_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_300to470_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_470to600_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_470to600_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_600to800_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_600to800_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_800to1000_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_800to1000_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_Pt_1000to1400_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_Pt_1000to1400_El");
	hmce->Add(htmp);*/
	/*htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT200to300_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT200to300_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT300to500_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT300to500_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT500to700_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT500to700_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT700to1000_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT700to1000_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT1000to1500_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT1000to1500_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT1500to2000_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT1500to2000_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_QCD_HT2000toInf_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_QCD_HT2000toInf_El");
	hmce->Add(htmp);*/
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_T_s_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_T_s_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_T_t_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_T_t_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_T_tW_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_T_tW_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_Tbar_tW_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_Tbar_tW_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WW_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WW_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_WZ_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_WZ_El");
	hmce->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/LepEta_GE0BTags_PostTrig_ZZ_Mu");
	hmcm->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/LepEta_GE0BTags_PostTrig_ZZ_El");
	hmce->Add(htmp);

        hmce->Scale(1/hmce->Integral());
        hmcm->Scale(1/hmcm->Integral());

        double mubins[13] = {-2.1,-1.6,-1.2,-0.9,-0.3,-0.2,0.,0.2,0.3,0.9,1.2,1.6,2.1};
        double elbins[11] = {-2.5,-2.,-1.566,-1.4442,-0.8,0.,0.8,1.4442,1.566,2.,2.5};

        double sum = 0;
        int k = 0;
        std::cout<<"El:"<<std::endl;
	for (int i = 1; i<hmce->GetNbinsX(); i++) {
                if (hmce->GetBinCenter(i)<elbins[k]) sum += hmce->GetBinContent(i);
                else {
                        std::cout<<"\t "<<elbins[k]<<"\t"<<sum<<std::endl;
                        sum=hmce->GetBinContent(i);
                        k++;
                }
                if (k>10) break;
	}
        sum = 0;
        k = 0;
        std::cout<<"\n\nMu:"<<std::endl;
	for (int i = 1; i<hmcm->GetNbinsX(); i++) {
                if (hmcm->GetBinCenter(i)<mubins[k]) sum += hmcm->GetBinContent(i);
                else {
                        std::cout<<"\t "<<mubins[k]<<"\t"<<sum<<std::endl;
                        sum=hmcm->GetBinContent(i);
                        k++;
                }
                if (k>12) break;
	}

}
