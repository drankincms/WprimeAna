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

void paperyields()
{

	TFile *file2 = new TFile("Run2016_Feb20_Slim/outFile.root");
	TH1F *htmp = new TH1F("htmp","htmp",1,0.,1.);
	htmp->Sumw2();
	TH1F *hdata = new TH1F("hdata","hdata",1,0.,1.);
        std::string luminosity = "35.9";
	float scale = 1.;

        bool blind = false;

	double mcUnc = 0.02*0.02 + 0.02*0.02 + 0.026*0.026;
	double qcdUnc = 0.4*0.4 + mcUnc;
	double topUnc = 0.08*0.08 + mcUnc;
	double vbUnc = 0.1*0.1 + mcUnc;

	std::vector<string> bcat;
	//bcat.push_back("GE1BTags");
	bcat.push_back("Ex1BTags");
	bcat.push_back("Ex2BTags");
	//bcat.push_back("GE1BTags_Final");
	bcat.push_back("Ex1BTags_Final");
	bcat.push_back("Ex2BTags_Final");
	std::vector<string> chan;
	chan.push_back("El");
	chan.push_back("Mu");
	std::vector<string> sample;
	std::vector<string> sigsample;
	sample.push_back("TTbar");
	sample.push_back("T_t");
	sample.push_back("Tbar_t");
	sample.push_back("T_tW");
	sample.push_back("Tbar_tW");
	sample.push_back("T_s");
	sample.push_back("WJets_Pt100to250_LF");
	sample.push_back("WJets_Pt250to400_LF");
	sample.push_back("WJets_Pt400to600_LF");
	sample.push_back("WJets_Pt600toInf_LF");
	sample.push_back("WJets_Pt100to250_HF");
	sample.push_back("WJets_Pt250to400_HF");
	sample.push_back("WJets_Pt400to600_HF");
	sample.push_back("WJets_Pt600toInf_HF");
	sample.push_back("ZJets");
	sample.push_back("WW");
	sample.push_back("WZ");
	sample.push_back("ZZ");
	/*sample.push_back("QCD_Pt_120to170");
	sample.push_back("QCD_Pt_170to300");
	sample.push_back("QCD_Pt_300to470");
	sample.push_back("QCD_Pt_470to600");
	sample.push_back("QCD_Pt_600to800");
	sample.push_back("QCD_Pt_800to1000");
	sample.push_back("QCD_Pt_1000to1400");*/
	/*sigsample.push_back("Wprime1000Right");
	sigsample.push_back("Wprime1100Right");
	sigsample.push_back("Wprime1200Right");
	sigsample.push_back("Wprime1300Right");*/
	//sigsample.push_back("Wprime1400Right");
	/*sigsample.push_back("Wprime1500Right");
	sigsample.push_back("Wprime1600Right");
	sigsample.push_back("Wprime1700Right");
	sigsample.push_back("Wprime1800Right");
	sigsample.push_back("Wprime1900Right");*/
	sigsample.push_back("Wprime2000Right");
	/*sigsample.push_back("Wprime2100Right");
	sigsample.push_back("Wprime2200Right");
	sigsample.push_back("Wprime2300Right");
	sigsample.push_back("Wprime2400Right");
	sigsample.push_back("Wprime2500Right");*/
	sigsample.push_back("Wprime2600Right");
	/*sigsample.push_back("Wprime2700Right");
	sigsample.push_back("Wprime2800Right");
	sigsample.push_back("Wprime2900Right");
	sigsample.push_back("Wprime3000Right");*/
	sigsample.push_back("Wprime3200Right");

	std::vector<double> totsum, tmpsum, datavec, tmperr;

	std::cout<<"\\begin{table}[h]"<<std::endl;
	//std::cout<<"\\footnotesize"<<std::endl;
	std::cout<<"\\begin{center}"<<std::endl;
	std::cout<<"\\caption{Number of selected data, signal, and background events. The expectation for signal and background events is computed corresponding to an integrated luminosity of "<<luminosity<<" fb$^{-1}$. ''Final selection'' refers to the additional cuts of $p_{T}^{top}>250$, $p_{T}^{j_1+j_2}>350$, and $100<m_{top}<250$. The quoted uncertainty does not include shape-based systematics.}"<<std::endl;
	std::cout<<"\\label{table:lep_yields}"<<std::endl;
	std::cout<<"\\resizebox{\\textwidth}{!}{"<<std::endl;
	std::cout<<"\\begin{tabular}{l|cc|cc|cc|cc}"<<std::endl;
	std::cout<<" & \\mc{8}{c}{Number of selected events}\\\\\\hline"<<std::endl;
	std::cout<<" & \\mc{4}{c}{Electron channel} \\vline& \\mc{4}{c}{Muon channel} \\\\"<<std::endl;
	std::cout<<" & \\mc{2}{c}{Object Selection} \\vline& \\mc{2}{c}{Final Selection} \\vline& \\mc{2}{c}{Object Selection} \\vline& \\mc{2}{c}{Final Selection}\\\\"<<std::endl;
	std::cout<<"Process & \\mc{1}{c}{$=1$ b-tags} & \\mc{1}{c}{$=2$ b-tags} \\vline& \\mc{1}{c}{$=1$ b-tags} & \\mc{1}{c}{$=2$ b-tags} \\vline& \\mc{1}{c}{$=1$ b-tags} & \\mc{1}{c}{$=2$ b-tags} \\vline& \\mc{1}{c}{$=1$ b-tags} & \\mc{1}{c}{$=2$ b-tags}\\\\\\hline"<<std::endl;
	std::cout<<"\\textbf{Signal:} & \\mc{8}{c}{} \\\\\\hline"<<std::endl;
	for (int j=0; j<chan.size(); j++) {
		for (int i=0; i<bcat.size(); i++) {
			totsum.push_back(0.);
			tmpsum.push_back(0.);
			tmperr.push_back(0.);
			datavec.push_back(0.);
		}
	}
	for (int k=0; k<sigsample.size(); k++) {
		std::cout<<"$M(\\PWpr_{R}=)$ "<<sigsample[k].substr(6,sigsample[k].find("Right")-6)<<" GeV";
		for (int j=0; j<chan.size(); j++) {
			for (int i=0; i<bcat.size(); i++) {
				htmp = (TH1F*)file2->Get(Form("%s/%s/mWprime_%s_%s_%s",bcat[i].c_str(),chan[j].c_str(),bcat[i].c_str(),sigsample[k].c_str(),chan[j].c_str()));
				tmpsum[i+(bcat.size()*j)] = htmp->Integral();
				tmpsum[i+(bcat.size()*j)] *= scale;
				tmpsum[i+(bcat.size()*j)] += 0.5;
				std::cout <<" & \\mc{1}{c}{"<<(int)tmpsum[i+(bcat.size()*j)]<<"}";
				if (i%2==1 && !(i==(bcat.size()-1) && j==(chan.size()-1))) std::cout<<"\\vline";
				tmpsum[i+(bcat.size()*j)] = 0.;
			}
		}
		std::cout<<"\\\\"<<std::endl;
	}
	std::cout<<"\\hline\\hline"<<std::endl;
	std::cout<<"\\textbf{Background:} & \\mc{8}{c}{} \\\\\\hline"<<std::endl;
	for (int k=0; k<sample.size(); k++) {
		if (sample[k]=="WJets_Pt600toInf_LF") std::cout<<"W$(\\rightarrow\\ell\\nu)$+jj";
		else if (sample[k]=="WJets_Pt600toInf_HF") std::cout<<"W$(\\rightarrow\\ell\\nu)$+bb/cc";
		else if (sample[k]=="ZJets") std::cout<<"Z$(\\rightarrow\\ell\\ell)$+jets";
		else if (sample[k]=="TTbar") std::cout<<"$t\\bar{t}$";
		else if (sample[k]=="T_s") std::cout<<"$tb$";
		else if (sample[k]=="T_t") std::cout<<"$tqb$";
		else if (sample[k]=="Tbar_t") std::cout<<"$\\bar{t}q\\bar{b}$";
		else if (sample[k]=="T_tW") std::cout<<"$tW$";
		else if (sample[k]=="Tbar_tW") std::cout<<"$\\bar{t}W$";
		else if (sample[k]=="ZZ") std::cout<<"$VV$";
		else if (sample[k]=="QCD_Pt_1000to1400") std::cout<<"$QCD$";
		for (int j=0; j<chan.size(); j++) {
			for (int i=0; i<bcat.size(); i++) {
				htmp = (TH1F*)file2->Get(Form("%s/%s/mWprime_%s_%s_%s",bcat[i].c_str(),chan[j].c_str(),bcat[i].c_str(),sample[k].c_str(),chan[j].c_str()));
				double error;
				if ((strncmp(sample[k].c_str(),"WJ",2)==0) && sample[k]!="WJets_Pt600toInf_LF" && sample[k]!="WJets_Pt600toInf_HF") {
					tmpsum[i+(bcat.size()*j)] += htmp->IntegralAndError(0,-1,error);
					tmperr[i+(bcat.size()*j)] += error*error+vbUnc*htmp->Integral()*htmp->Integral();
				}
				else if ((strncmp(sample[k].c_str(),"QCD",3)==0) && sample[k]!="QCD_Pt_1000to1400") {
					tmpsum[i+(bcat.size()*j)] += htmp->IntegralAndError(0,-1,error);
					tmperr[i+(bcat.size()*j)] += error*error+qcdUnc*htmp->Integral()*htmp->Integral();
				}
                                else if (sample[k]=="WW" or sample[k]=="WZ") {
					tmpsum[i+(bcat.size()*j)] += htmp->IntegralAndError(0,-1,error);
					tmperr[i+(bcat.size()*j)] += error*error+vbUnc*htmp->Integral()*htmp->Integral();
				}
				else {
					tmpsum[i+(bcat.size()*j)] += htmp->IntegralAndError(0,-1,error);
					if (strncmp(sample[k].c_str(),"QCD",3)==0) tmperr[i+(bcat.size()*j)] += error*error+qcdUnc*htmp->Integral()*htmp->Integral();
					else if (strncmp(sample[k].c_str(),"T",1)==0) tmperr[i+(bcat.size()*j)] += error*error+topUnc*htmp->Integral()*htmp->Integral();
					else tmperr[i+(bcat.size()*j)] += error*error+vbUnc*htmp->Integral()*htmp->Integral();
					tmpsum[i+(bcat.size()*j)] *= scale;
					tmpsum[i+(bcat.size()*j)] += 0.5;
					std::cout <<" & \\mc{1}{c}{"<<(int)tmpsum[i+(bcat.size()*j)]<<"}";
					if (i%2==1 && !(i==(bcat.size()-1) && j==(chan.size()-1))) std::cout<<"\\vline";
					totsum[i+(bcat.size()*j)] += (int)tmpsum[i+(bcat.size()*j)];
					tmpsum[i+(bcat.size()*j)] = 0;
				}
			}
		}
		if (!((strncmp(sample[k].c_str(),"WJ",2)==0) && sample[k]!="WJets_Pt600toInf_LF" && sample[k]!="WJets_Pt600toInf_HF") && !((strncmp(sample[k].c_str(),"QCD",3)==0) && sample[k]!="QCD_Pt_1000to1400") && !(sample[k]=="WW" or sample[k]=="WZ")) std::cout<<"\\\\"<<std::endl;
	}
	std::cout<<"\\hline\\textbf{Total Background}";
	for (int j=0; j<chan.size(); j++) {
		for (int i=0; i<bcat.size(); i++) {
			std::cout<<" & \\mc{1}{c}{"<<totsum[i+(bcat.size()*j)]<<"$\\pm$"<<(int)(sqrt(tmperr[i+(bcat.size()*j)])+0.5)<<"}";
			if (i%2==1 && !(i==(bcat.size()-1) && j==(chan.size()-1))) std::cout<<"\\vline";
		}
	}
	std::cout<<"\\\\"<<std::endl;
	std::cout<<"\\hline\\hline\\textbf{Data:}";
	totsum.clear();
	for (int j=0; j<chan.size(); j++) {
		for (int i=0; i<bcat.size(); i++) {
			hdata = (TH1F*)file2->Get(Form("%s/%s/mWprime_%s_Data_%s",bcat[i].c_str(),chan[j].c_str(),bcat[i].c_str(),chan[j].c_str()));
			datavec[i+(bcat.size()*j)] = hdata->Integral();
			if (blind) std::cout <<" & \\mc{1}{c}{\\textemdash}";
			else std::cout <<" & \\mc{1}{c}{"<<(int)datavec[i+(bcat.size()*j)]<<"}";
			if (i%2==1 && !(i==(bcat.size()-1) && j==(chan.size()-1))) std::cout<<"\\vline";
		}
	}
	std::cout<<"\\\\"<<std::endl;
	/*std::cout<<"\\hline\\textbf{Data/MC}";
	for (int i=0; i<bcat.size(); i++) {
		std::cout<<" & \\mc{1}{c}{"<<datavec[i+(bcat.size()*j)]/totsum[i+(bcat.size()*j)]<<"}";
		if (i==3) std::cout<<"\\vline";
	}*/
	std::cout<<"\\hline"<<std::endl;
	std::cout<<"\\end{tabular}"<<std::endl;
	std::cout<<"}"<<std::endl;
	std::cout<<"\\end{center}"<<std::endl;
	std::cout<<"\\end{table}"<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;
	std::cout<<std::endl;

}
