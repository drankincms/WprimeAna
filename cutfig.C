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

void cutfig()
{

	TFile *file1 = new TFile("Run2016B_Jul04/outFile.root");
	TH1F *htmp = new TH1F("htmp","htmp",1,0.,1.);
	float scale = 1.0;

	std::vector<string> sampset;
	sampset.push_back("Wprime2000Right");
	//sampset.push_back("Wprime2500Right");
	//sampset.push_back("Wprime3000Right");
	//sampset.push_back("T_t");
	sampset.push_back("Tbar_t");
	sampset.push_back("T_tW");
	sampset.push_back("Tbar_tW");
	sampset.push_back("T_s");
	sampset.push_back("TTbar");
	sampset.push_back("WJets_HT100to200_LF");
	sampset.push_back("WJets_HT200to400_LF");
	sampset.push_back("WJets_HT400to600_LF");
	sampset.push_back("WJets_HT600to800_LF");
	sampset.push_back("WJets_HT800to1200_LF");
	sampset.push_back("WJets_HT1200to2500_LF");
	sampset.push_back("WJets_HT2500toInf_LF");
	sampset.push_back("WW");
	sampset.push_back("WZ");
	sampset.push_back("ZZ");
	sampset.push_back("ZJets");
	sampset.push_back("WJets_HT100to200_HF");
	sampset.push_back("WJets_HT200to400_HF");
	sampset.push_back("WJets_HT400to600_HF");
	sampset.push_back("WJets_HT600to800_HF");
	sampset.push_back("WJets_HT800to1200_HF");
	sampset.push_back("WJets_HT1200to2500_HF");
	sampset.push_back("WJets_HT2500toInf_HF");
	/*sampset.push_back("QCD_Pt_120to170");
	sampset.push_back("QCD_Pt_170to300");
	sampset.push_back("QCD_Pt_300to470");
	sampset.push_back("QCD_Pt_470to600");
	sampset.push_back("QCD_Pt_600to800");
	sampset.push_back("QCD_Pt_800to1000");
	sampset.push_back("QCD_Pt_1000to1400");*/
	//sampset.push_back("Wprime1600Right");
	std::vector<string> chan;
	chan.push_back("Mu");
	chan.push_back("El");
	std::vector<string> cutset;
	//cutset.push_back("GE0BTags_PostTrig");
	cutset.push_back("GE0BTags_PostLep");
	cutset.push_back("GE0BTags_PostJet1");
	cutset.push_back("GE0BTags_PostJet2");
	cutset.push_back("GE0BTags_PostMET");
	cutset.push_back("GE0BTags_Post2D");
	cutset.push_back("GE0BTags_PostTriangle");
	cutset.push_back("GE0BTags_PostMTop");
	cutset.push_back("GE0BTags_PostPtTop");
	cutset.push_back("GE0BTags_Final");
	cutset.push_back("Ex1BTags_Final");
	cutset.push_back("Ex2BTags_Final");

	double tmpsum = 0;
	int totsum = 0;

	for (int j=0; j<chan.size(); j++) {
		std::cout<<"\\begin{table}[h]"<<std::endl;
		std::cout<<"\\footnotesize"<<std::endl;
		std::cout<<"\\begin{center}"<<std::endl;
		if (chan[j]=="El") {
			std::cout<<"\\caption{Number of selected data and background events in the electron channel after various cuts. The expectation is computed corresponding to an integrated luminosity of \\usedLumi.}"<<std::endl;
			std::cout<<"\\label{table:el_cuts}"<<std::endl;
		}
		if (chan[j]=="Mu") {
			std::cout<<"\\caption{Number of selected data and background events in the muon channel after various cuts. The expectation is computed corresponding to an integrated luminosity of \\usedLumi.}"<<std::endl;
			std::cout<<"\\label{table:mu_cuts}"<<std::endl;
		}
		std::cout<<"\\begin{tabular}{l|clllll}"<<std::endl;
		std::cout<<"Cut & \\mc{5}{c}{Number of Events} & \\\\\\hline\\hline"<<std::endl;
		std::cout<<"& \\mc{4}{c}{Process} \\vline & \\mc{1}{c}{Total Background} \\vline& \\mc{1}{c}{$S/\\sqrt{B}$}\\\\"<<std::endl;
		std::cout<<" & \\mc{1}{c}{M$(\\PWpr_{R})$ 2 TeV} & \\mc{1}{c}{top} & \\mc{1}{c}{$V$+ jets (LF)} & \\mc{1}{c}{$W$+ jets (HF)}\\vline & \\mc{2}{c}{}\\\\\\hline\\hline"<<std::endl;
		for (int k=0; k<cutset.size(); k++) {
			if ((cutset[k]=="GE0BTags_PostMTop" && chan[j]=="El") or (cutset[k]=="GE0BTags_PostTriangle" && chan[j]=="Mu")) continue;
			double sigtot = 0;
			if (cutset[k]=="GE0BTags_PostTrig") std::cout<<"Trigger";
			else if(cutset[k]=="GE0BTags_PostLep") std::cout<<"Lepton $p_{T}$";
			else if(cutset[k]=="GE0BTags_PostMET") std::cout<<"$E_T^{miss}$";
			else if(cutset[k]=="GE0BTags_PostJet1") std::cout<<"Leading Jet $p_{T}$";
			else if(cutset[k]=="GE0BTags_PostJet2") std::cout<<"Subleading Jet $p_{T}$";
			else if(cutset[k]=="GE0BTags_Post2D") std::cout<<"Lepton-Jet 2D Cut";
			else if(cutset[k]=="GE0BTags_PostTriangle") std::cout<<"$|\\Delta\\Phi(MET,e)|$";
			else if(cutset[k]=="GE0BTags_PostMTop") std::cout<<"Top Mass";
			else if(cutset[k]=="GE0BTags_PostPtTop") std::cout<<"Top $p_{T}$";
			else if(cutset[k]=="GE0BTags_PostPtJ1J2" or cutset[k]=="GE0BTags_Final") std::cout<<"Dijet System $p_{T}$";
			else if(cutset[k]=="Ex1BTags_Final") std::cout<<"Exactly 1 b-tag";
			else if(cutset[k]=="Ex2BTags_Final") std::cout<<"Exactly 2 b-tag";
			for (int i=0; i<sampset.size(); i++) {
				htmp = (TH1F*)file1->Get(Form("%s/%s/LepPt_%s_%s_%s",cutset[k].c_str(),chan[j].c_str(),cutset[k].c_str(),sampset[i].c_str(),chan[j].c_str()));
				if (((strncmp(sampset[i].c_str(),"W",1)==0 || strncmp(sampset[i].c_str(),"Z",1)==0) && sampset[i]!="ZJets" && sampset[i]!="WJets_HT2500toInf_HF" && !(strncmp(sampset[i].c_str(),"Wprime",5)==0)) || ((strncmp(sampset[i].c_str(),"T",1)==0) && sampset[i]!="TTbar") || ((strncmp(sampset[i].c_str(),"QCD",3)==0) && sampset[i]!="QCD_Pt_1000to1400")) {
					tmpsum += htmp->Integral();
				}
				else {
					if (sampset[i]=="Wprime2000Right") sigtot = htmp->Integral();
					tmpsum += htmp->Integral();
					tmpsum *= scale;
					tmpsum += 0.5;
					std::cout <<" & \\mc{1}{c}{"<<(int)tmpsum<<"}\\vline";
					if (!(strncmp(sampset[i].c_str(),"Wprime",5)==0) ) totsum += (int)tmpsum;
					tmpsum = 0;
				}
			}
			std::cout<<" & \\mc{1}{c}{"<<totsum<<"}\\vline & \\mc{1}{c}{"<<sigtot/sqrt(totsum)<<"}\\\\\\hline"<<std::endl;
			totsum = 0;
			//htmp = (TH1F*)file1->Get(Form("%s/%s/LepPt_%s_Data_%s",cutset[k].c_str(),chan[j].c_str(),cutset[k].c_str(),chan[j].c_str()));
			//std::cout <<" & \\mc{1}{c}{"<<(int)(htmp->Integral())<<"}\\vline\\\\";
		}
		std::cout<<"\\end{tabular}"<<std::endl;
		std::cout<<"\\end{center}"<<std::endl;
		std::cout<<"\\end{table}"<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
		std::cout<<std::endl;
	}

}
