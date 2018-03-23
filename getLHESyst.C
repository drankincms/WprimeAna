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

void getLHESyst()
{

        std::string direc = "Run2016_Jan13";
	TFile *file1 = new TFile(Form("%s_LHE/outFile.root",direc.c_str()));
	TFile *file2 = new TFile(Form("%s_Slim/outFile.root",direc.c_str()));
	TH2F *htmp = new TH2F("htmp","htmp",1,0.,1.,1,0.,1.);
	//TH1F *htmp1 = new TH1F("htmp1","htmp1",1,0.,1.);

	std::vector<string> sampset;
	std::vector<string> sampsetw;
	std::vector<string> sampsett;
	sampset.push_back("TTbar");
	sampset.push_back("WJets");

	sampsetw.push_back("WJets_HT100to200");
	sampsetw.push_back("WJets_HT200to400");
	sampsetw.push_back("WJets_HT400to600");
	sampsetw.push_back("WJets_HT600to800");
	sampsetw.push_back("WJets_HT800to1200");
	sampsetw.push_back("WJets_HT1200to2500");
	sampsetw.push_back("WJets_HT2500toInf");
	sampsetw.push_back("ZJets");
	sampsetw.push_back("WW");
	sampsetw.push_back("WZ");
	sampsetw.push_back("ZZ");

	sampsett.push_back("TTbar");
	sampsett.push_back("T_t");
	sampsett.push_back("Tbar_t");
	sampsett.push_back("T_s");
	sampsett.push_back("T_tW");
	sampsett.push_back("Tbar_tW");

	std::vector<string> systset;
	systset.push_back("PDF");
	systset.push_back("alphaSm");
	systset.push_back("alphaSp");
	//systset.push_back("muR1muF2");
	//systset.push_back("muR1muF0p5");
	//systset.push_back("muR2muF1");
	//systset.push_back("muR0p5muF1");
	//systset.push_back("muR0p5muF0p5");
	//systset.push_back("muR2muF2");

	std::vector<string> chanset;
	//chanset.push_back("El");
	//chanset.push_back("Mu");
	chanset.push_back("All");

        //double lowbins[]  = {0.0, 100.0,400.0,600.0,800.0,1000.0,1200.0,1400.0,1600.0,1800.0,2000.0,2200.0,2600.0,3600.0,5000.0};
        double lowbins[]  = {0.0, 100.0,500.0,700.0,900.0,1100.0,1300.0,1500.0,1700.0,1900.0,2100.0,2400.0,3000.0,5000.0};
	//double lowbins[] = {0.0,100.0,400.0,600.0,800.0,1000.0,1200.0,1400.0,1600.0,1800.0,2000.0,2200.0,2600.0,4000.0};
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
	for (unsigned int c = 0; c<chanset.size(); c++) {
		std::vector<double> pdfsystp;
		std::vector<double> pdfsystm;
		std::vector<double> alphasp;
		std::vector<double> alphasm;
		std::vector<double> muRp;
		std::vector<double> muRm;
		std::vector<double> muFp;
		std::vector<double> muFm;
		std::vector<double> muRmuFp;
		std::vector<double> muRmuFm;
		for (unsigned int i = 0; i < sampset.size(); i++) {
			pdfsystp.clear();
			pdfsystm.clear();
			alphasp.clear();
			alphasm.clear();
			muRp.clear();
			muRm.clear();
			muFp.clear();
			muFm.clear();
			muRmuFp.clear();
			muRmuFm.clear();
			std::cout<<"\n#"<<chanset[c]<<" ------ "<<sampset[i]<<" ------"<<std::endl;
			for (unsigned int k = 0; k < systset.size(); k++) {
				//std::cout<<"Systematic: "<<systset[k]<<std::endl;
				std::vector<std::string> vtmps;
				if (sampset[i]=="TTbar") vtmps = sampsett;
				else if (sampset[i]=="WJets") vtmps = sampsetw;
                                else {
					std::cout<<"Unrecognized sample set "<<sampset[i]<<", exiting..."<<std::endl;
					return;
				}
				if (vtmps.size()<1) {
					std::cout<<"Empty sample set, exiting..."<<std::endl;
					return;
				}

				if (chanset[c]=="All") {
					htmp = (TH2F*)file1->Get(Form("h_%s_vs_Mtb_El_%s",systset[k].c_str(),vtmps[0].c_str()));
                	                htmp->Scale(((TH1F*)file2->Get(Form("GE1BTags_Final/El/mWprime_Limits_GE1BTags_Final_%s_El",vtmps[0].c_str())))->Integral());
					htmp->Add((TH2F*)file1->Get(Form("h_%s_vs_Mtb_Mu_%s",systset[k].c_str(),vtmps[0].c_str())),((TH1F*)file2->Get(Form("GE1BTags_Final/Mu/mWprime_Limits_GE1BTags_Final_%s_Mu",vtmps[0].c_str())))->Integral());
				}
				else {
					htmp = (TH2F*)file1->Get(Form("h_%s_vs_Mtb_%s_%s",systset[k].c_str(),chanset[c].c_str(),vtmps[0].c_str()));
                	                htmp->Scale(((TH1F*)file2->Get(Form("GE1BTags_Final/%s/mWprime_Limits_GE1BTags_Final_%s_%s",chanset[c].c_str(),vtmps[0].c_str(),chanset[c].c_str())))->Integral());
				}
                                for (unsigned int s = 1; s < vtmps.size(); s++) {
					if (chanset[c]=="All") {
						htmp->Add((TH2F*)file1->Get(Form("h_%s_vs_Mtb_El_%s",systset[k].c_str(),vtmps[s].c_str())),((TH1F*)file2->Get(Form("GE1BTags_Final/El/mWprime_Limits_GE1BTags_Final_%s_El",vtmps[s].c_str())))->Integral());
						htmp->Add((TH2F*)file1->Get(Form("h_%s_vs_Mtb_Mu_%s",systset[k].c_str(),vtmps[s].c_str())),((TH1F*)file2->Get(Form("GE1BTags_Final/El/mWprime_Limits_GE1BTags_Final_%s_El",vtmps[s].c_str())))->Integral());
					}
					else htmp->Add((TH2F*)file1->Get(Form("h_%s_vs_Mtb_%s_%s",systset[k].c_str(),chanset[c].c_str(),vtmps[s].c_str())),((TH1F*)file2->Get(Form("GE1BTags_Final/El/mWprime_Limits_GE1BTags_Final_%s_El",vtmps[s].c_str())))->Integral());
				}
				TH1D* py = htmp->ProjectionY("py");
	
				int binlo = 1;
				int binhi = 1;
				for (unsigned int j = 1; j < xlow.size(); j++) {
					binhi = py->GetNbinsX();
					for (int n = 1; n <= py->GetNbinsX(); n++) {
						if (py->GetBinLowEdge(n)>=xlow[j]) {
							binhi = n-1;
							break;
						}
					}
					//std::cout<<"binlo = "<<binlo<<"   binhi = "<<binhi<<std::endl;
					//std::cout<<"  xlo = "<<py->GetBinLowEdge(binlo)<<"     xhi = "<<py->GetBinLowEdge(binhi)<<std::endl;
					//TH1D* px = htmp->ProjectionX("px",binlo,binhi);
					//std::cout<<"\tFor range "<<xlow[j-1]<<" to "<<xlow[j]<<" , Mean = "<<px->GetMean()<<" and RMS = "<<px->GetStdDev()<<std::endl;
					if (systset[k]=="PDF") {
						double quant[2] = {0.16,0.84};
						double sigmas[2];
						//pdfsystp.push_back(htmp->ProjectionX("px",binlo,binhi)->GetStdDev());
						TH1D* px = htmp->ProjectionX("px",binlo,binhi);
						if (px->Integral() > 0.) px->GetQuantiles(2,sigmas,quant);
						else {
							sigmas[0] = 1.;
							sigmas[1] = 1.;
						}
						pdfsystm.push_back(fabs(1-sigmas[0]));
						pdfsystp.push_back(fabs(1-sigmas[1]));
					}
					if (systset[k]=="alphaSm" && sampset[i]=="TTbar") alphasm.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="alphaSp" && sampset[i]=="TTbar") alphasp.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR2muF1") muRp.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR0p5muF1") muRm.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR1muF2") muFp.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR1muF0p5") muFm.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR2muF2") muRmuFp.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					if (systset[k]=="muR0p5muF0p5") muRmuFm.push_back(fabs(1-(htmp->ProjectionX("px",binlo,binhi)->GetMean())));
					binlo=binhi+1;
				}
			}
			if (sampset[i]=="TTbar") {
				std::cout<<"ttpdfasup = [ ";
				for (unsigned int l = 0; l<pdfsystp.size(); l++) {
					std::cout<<1+sqrt(pdfsystp[l]*pdfsystp[l]+alphasp[l]*alphasp[l]*0.75*0.75)<<" ";
					if (l<pdfsystp.size()-1) std::cout<<", ";
				}
				std::cout<<"]"<<std::endl;
				std::cout<<"ttpdfasdown = [ ";
				for (unsigned int l = 0; l<pdfsystm.size(); l++) {
					std::cout<<1-sqrt(pdfsystm[l]*pdfsystm[l]+alphasm[l]*alphasm[l]*0.75*0.75)<<" ";
					if (l<pdfsystm.size()-1) std::cout<<", ";
				}
				std::cout<<"]"<<std::endl;
			}
			else {
				std::cout<<"wjpdfasup = [ ";
				for (unsigned int l = 0; l<pdfsystp.size(); l++) {
					std::cout<<1+sqrt(pdfsystp[l]*pdfsystp[l])<<" ";
					if (l<pdfsystp.size()-1) std::cout<<", ";
				}
				std::cout<<"]"<<std::endl;
				std::cout<<"wjpdfasdown = [ ";
				for (unsigned int l = 0; l<pdfsystm.size(); l++) {
					std::cout<<1-sqrt(pdfsystm[l]*pdfsystm[l])<<" ";
					if (l<pdfsystm.size()-1) std::cout<<", ";
				}
				std::cout<<"]"<<std::endl;
			}
			/*std::cout<<"muRup = [ ";
			for (unsigned int l = 0; l<muRp.size(); l++) {
				std::cout<<1+muRp[l]<<" ";
				if (l<muRp.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;
			std::cout<<"muRdown = [ ";
			for (unsigned int l = 0; l<muRm.size(); l++) {
				std::cout<<1-muRm[l]<<" ";
				if (l<muRm.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;
			std::cout<<"muFup = [ ";
			for (unsigned int l = 0; l<muFp.size(); l++) {
				std::cout<<1+muFp[l]<<" ";
				if (l<muFp.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;
			std::cout<<"muFdown = [ ";
			for (unsigned int l = 0; l<muFm.size(); l++) {
				std::cout<<1-muFm[l]<<" ";
				if (l<muFm.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;
			std::cout<<"muRmuFup = [ ";
			for (unsigned int l = 0; l<muRmuFp.size(); l++) {
				std::cout<<1+muRmuFp[l]<<" ";
				if (l<muRmuFp.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;
			std::cout<<"muRmuFdown = [ ";
			for (unsigned int l = 0; l<muRmuFm.size(); l++) {
				std::cout<<1-muRmuFm[l]<<" ";
				if (l<muRmuFm.size()-1) std::cout<<", ";
			}
			std::cout<<"]"<<std::endl;*/
		}
	}

}
