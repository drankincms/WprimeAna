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

void getNPVweights()
{

	TFile *file1 = new TFile("Run2015D_Nov05/outFile.root");
	TH1F *hdata = new TH1F("hdata","hdata",1,0.,1.);
	TH1F *hmc = new TH1F("hmc","hmc",1,0.,1.);
	TH1F *htmp = new TH1F("htmp","htmp",1,0.,1.);

	hdata = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_Data_Mu");
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_Data_El");
	hdata->Add(htmp);
	hmc = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_TTbar_Mu");
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_TTbar_El");
	hmc->Add(htmp);
	/*htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WJets_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WJets_El");
	hmc->Add(htmp);*/
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WJets_HT100to200_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WJets_HT100to200_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WJets_HT200to400_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WJets_HT200to400_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WJets_HT400to600_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WJets_HT400to600_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WJets_HT600toInf_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WJets_HT600toInf_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_ZJets_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_ZJets_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_120to170_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_120to170_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_170to300_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_170to300_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_300to470_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_300to470_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_470to600_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_470to600_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_600to800_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_600to800_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_800to1000_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_800to1000_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_Pt_1000to1400_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_Pt_1000to1400_El");
	hmc->Add(htmp);
	/*htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT200to300_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT200to300_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT300to500_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT300to500_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT500to700_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT500to700_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT700to1000_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT700to1000_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT1000to1500_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT1000to1500_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT1500to2000_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT1500to2000_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_QCD_HT2000toInf_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_QCD_HT2000toInf_El");
	hmc->Add(htmp);*/
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_T_s_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_T_s_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_T_t_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_T_t_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_T_tW_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_T_tW_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_Tbar_tW_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_Tbar_tW_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WW_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WW_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_WZ_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_WZ_El");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/Mu/nPV_GE0BTags_PostTrig_ZZ_Mu");
	hmc->Add(htmp);
	htmp = (TH1F*)file1->Get("GE0BTags_PostTrig/El/nPV_GE0BTags_PostTrig_ZZ_El");
	hmc->Add(htmp);

        hdata->Scale(1/hdata->Integral());
        hmc->Scale(1/hmc->Integral());

	std::cout<<"    double weightpu[] = {";
	for (int i = 1; i<hmc->GetNbinsX(); i++) {
		//std::cout<<"    if (teve->nPV == "<<(int)hmc->GetBinCenter(i)<<") weightpu = "<<hmc->GetBinContent(i)<<";"<<std::endl;
		if (i==1) {
			if (hmc->GetBinContent(i)!=0.) std::cout<<hdata->GetBinContent(i)/hmc->GetBinContent(i);
			else std::cout<<"0.";
		}
		else if (hmc->GetBinContent(i)!=0.) std::cout<<" , "<<hdata->GetBinContent(i)/hmc->GetBinContent(i);
		else std::cout<<" , 0.";
	}
	std::cout<<"};"<<std::endl;

}
