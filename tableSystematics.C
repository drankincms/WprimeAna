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

void tableSystematics()
{

        std::string dirName = "Run2015D_Slim_Nov05_NoDPhi/";
        //TFile *filenom = new TFile("Iso_MediumBTag/outFile.root");
        TFile *filenom = new TFile(Form("%s/outFile.root",dirName.c_str()));
        std::vector<TFile*> filesp, filesm;
        filesp.push_back(new TFile(Form("%s/outFile_JESUP.root",dirName.c_str())));
        filesp.push_back(new TFile(Form("%s/outFile_Q2SCALEUP.root",dirName.c_str())));
        filesm.push_back(new TFile(Form("%s/outFile_JESDOWN.root",dirName.c_str())));
        filesm.push_back(new TFile(Form("%s/outFile_Q2SCALEDOWN.root",dirName.c_str())));
        std::vector<std::string> sysnames;
        sysnames.push_back("J.E.S.");
        sysnames.push_back("$Q^2$ Scale");

        std::vector<string> bcat;
        bcat.push_back("Ex1BTags_Final");
        bcat.push_back("Ex2BTags_Final");
        std::vector<string> chan;
        chan.push_back("Mu");
        chan.push_back("El");
        std::vector<string> tsample, vsample;
        tsample.push_back("TTbar");
        tsample.push_back("T_t");
        tsample.push_back("T_tW");
        tsample.push_back("Tbar_tW");
        tsample.push_back("T_s");
        vsample.push_back("WJets_HT100to200");
        vsample.push_back("WJets_HT200to400");
        vsample.push_back("WJets_HT400to600");
        vsample.push_back("WJets_HT600toInf");
        vsample.push_back("ZJets");
        vsample.push_back("WW");
        vsample.push_back("WZ");
        vsample.push_back("ZZ");

        for (int j=0; j<bcat.size(); j++) {
                std::cout<<"\\begin{table}[h]"<<std::endl;
                std::cout<<"\\begin{center}"<<std::endl;
                if (bcat[j]=="Ex1BTags_Final") {
                	std::cout<<"\\caption{Fractional change in rate from shape systematics in the 1 b-tag channel.}"<<std::endl;
                	std::cout<<"\\label{table:1bsys}"<<std::endl;
                }
                if (bcat[j]=="Ex2BTags_Final") {
                	std::cout<<"\\caption{Fractional change in rate from shape systematics in the 2 b-tag channel.}"<<std::endl;
                	std::cout<<"\\label{table:2bsys}"<<std::endl;
                }
                std::cout<<"\\begin{tabular}{l|clll|llll}"<<std::endl;
                std::cout<<"& \\mc{4}{c}{$V$+jets} & \\mc{4}{c}{$t\\bar{t}$+single-$t$}\\\\\\hline\\hline"<<std::endl;
                for (int tmpind = 0; tmpind < 2; tmpind++) {
                        for (int l = 0; l<chan.size(); l++) {
                                if (chan[l]=="El") std::cout<<" & \\mc{2}{c}{$e$+jets}";
                                if (chan[l]=="Mu") std::cout<<" & \\mc{2}{c}{$\\mu$+jets}";
                        }
                }
                std::cout<<"\\\\\\hline"<<std::endl;
                std::cout<<"Systematic";
                for (int tmpind = 0; tmpind < 2; tmpind++) {
                        for (int l = 0; l<chan.size(); l++) {
                                std::cout<<" & \\mc{1}{c}{+$1\\sigma$}";
                                std::cout<<" & \\mc{1}{c}{-$1\\sigma$}";
                        }
                }
                double plusint, minusint, nomint;
                std::cout<<"\\\\\\hline"<<std::endl;
                for (int sysi = 0; sysi < sysnames.size(); sysi++) {
                        std::cout<<sysnames[sysi];
                        plusint = 0;
                        minusint = 0;
                        nomint = 0;
                        for (int k=0; k<vsample.size(); k++) {
                                plusint+=((TH1F*)filesp[sysi]->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                                minusint+=((TH1F*)filesm[sysi]->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                                nomint+=((TH1F*)filenom->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                        }
                        if (plusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<plusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        if (minusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<minusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        plusint = 0;
                        minusint = 0;
                        nomint = 0;
                        for (int k=0; k<vsample.size(); k++) {
                                plusint+=((TH1F*)filesp[sysi]->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                                minusint+=((TH1F*)filesm[sysi]->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                                nomint+=((TH1F*)filenom->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),vsample[k].c_str())))->Integral();
                        }
                        if (plusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<plusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        if (minusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<minusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        plusint = 0;
                        minusint = 0;
                        nomint = 0;
                        for (int k=0; k<tsample.size(); k++) {
                                plusint+=((TH1F*)filesp[sysi]->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                                minusint+=((TH1F*)filesm[sysi]->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                                nomint+=((TH1F*)filenom->Get(Form("%s/Mu/mWprime_%s_%s_Mu",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                        }
                        if (plusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<plusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        if (minusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<minusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        plusint = 0;
                        minusint = 0;
                        nomint = 0;
                        for (int k=0; k<tsample.size(); k++) {
                                plusint+=((TH1F*)filesp[sysi]->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                                minusint+=((TH1F*)filesm[sysi]->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                                nomint+=((TH1F*)filenom->Get(Form("%s/El/mWprime_%s_%s_El",bcat[j].c_str(),bcat[j].c_str(),tsample[k].c_str())))->Integral();
                        }
                        if (plusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<plusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        if (minusint!=nomint) std::cout<<" & \\mc{1}{c}{"<<minusint/nomint<<"}";
                        else std::cout<<" & \\mc{1}{c}{\\textemdash}";
                        std::cout<<"\\\\\\hline"<<std::endl;
                }
                std::cout<<"\\end{tabular}"<<std::endl;
                std::cout<<"\\end{center}"<<std::endl;
                std::cout<<"\\end{table}"<<std::endl;
                std::cout<<std::endl;
                std::cout<<std::endl;
                std::cout<<std::endl;
        }
}
