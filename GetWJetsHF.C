#ifndef GetWJetsHF_cxx
#define GetWJetsHF_cxx

#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TH1.h>
#include <TPaveStats.h>
#include <TFitResult.h>

#include "WprimeLoadData2016.C"
#include "tdrstyle.C"

using namespace std;

static TH1F* h1;
static TH1F* h2;
double wjets_func(double* x, double* pars);

int main( int argc, const char* argv[] ){

  if (argc < 2){
    cout<<"Need at least 1 arguments. Only "<<argc<<" found."<<endl;
    return 1;
  }
  
  setTDRStyle();

  //Root file information
  TString inDir  = argv[1];

  TString suff = "";
  if (argc > 2) suff = argv[2];

  int qcdin = 0;
  if (argc > 3) qcdin = atoi(argv[3]);
  bool useQCD = false;
  if (qcdin>0) useQCD = true;
  float scale = 1.0;
  if (argc > 4) scale = atof(argv[4]);
  int systi = 0;
  if (argc > 5) systi = atoi(argv[5]);
  TString syst = "";
  if (systi==1) syst = "_BUP";
  if (systi==2) syst = "_BDOWN";
  if (systi==3) syst = "_MUP";
  if (systi==4) syst = "_MDOWN";

  std::cout<<"Syst = "<<syst<<std::endl;

  //cout<<inDir<<endl;
    
  const unsigned int NCHAN    = 4;
  const unsigned int NSAMPLES = totalSamples;

  TString fileName = "outFile"+suff+".root";
  TFile *file0 = TFile::Open(inDir+fileName);
  file0->cd();

  TH1F* histArray[NSAMPLES][NCHAN];
  TH1F* histArraytmp[NSAMPLES][NCHAN];
  vector <TH1F*> histosEX0hf, histosGE0hf, histosEX0lf, histosGE0lf;

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (not (
      isam == Data or
      isam == ZJets or
      //isam == WJets_HT100to200 or
      //isam == WJets_HT200to400 or
      //isam == WJets_HT400to600 or
      //isam == WJets_HT600to800 or
      //isam == WJets_HT800to1200 or
      //isam == WJets_HT1200to2500 or
      //isam == WJets_HT2500toInf or
      isam == WJets_Pt100to250 or
      isam == WJets_Pt250to400 or
      isam == WJets_Pt400to600 or
      isam == WJets_Pt600toInf or
      isam == WW or
      isam == WZ or
      isam == ZZ or
      isam == TTbar or
      isam == T_s or
      isam == T_t or
      isam == Tbar_t or
      isam == T_tW or
      isam == Tbar_tW or
      (useQCD and (
      isam == QCD_Pt_120to170 or
      isam == QCD_Pt_170to300 or
      isam == QCD_Pt_300to470 or
      isam == QCD_Pt_470to600 or
      isam == QCD_Pt_600to800 or
      isam == QCD_Pt_800to1000 or
      isam == QCD_Pt_1000to1400
      ))
      )) {
      histosEX0hf.push_back(emptyHisto);
      histosEX0lf.push_back(emptyHisto);
      histosGE0hf.push_back(emptyHisto);
      histosGE0lf.push_back(emptyHisto);
      continue;
    }
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_LF"+syst+"/Mu/mWprime_Ex0BTags_LF"+syst+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_HF"+syst+"/Mu/mWprime_Ex0BTags_HF"+syst+"_"+allNames[isam]+"_Mu");
    histArray[isam][2] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_LF/Mu/mWprime_GE0BTags_LF_"+allNames[isam]+"_Mu");
    histArray[isam][3] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_HF/Mu/mWprime_GE0BTags_HF_"+allNames[isam]+"_Mu");
    /*histArraytmp[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_LF"+syst+"/El/mWprime_Ex0BTags_LF"+syst+"_"+allNames[isam]+"_El");
    histArraytmp[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_HF"+syst+"/El/mWprime_Ex0BTags_HF"+syst+"_"+allNames[isam]+"_El");
    histArraytmp[isam][2] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_LF/El/mWprime_GE0BTags_LF_"+allNames[isam]+"_El");
    histArraytmp[isam][3] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_HF/El/mWprime_GE0BTags_HF_"+allNames[isam]+"_El");
    histArray[isam][0]->Add(histArraytmp[isam][0]);
    histArray[isam][1]->Add(histArraytmp[isam][1]);
    histArray[isam][2]->Add(histArraytmp[isam][2]);
    histArray[isam][3]->Add(histArraytmp[isam][3]);*/
    /*histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_LF_Boost"+syst+"/Mu/mWprime_Ex0BTags_LF_Boost"+syst+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/Ex0BTags_HF_Boost"+syst+"/Mu/mWprime_Ex0BTags_HF_Boost"+syst+"_"+allNames[isam]+"_Mu");
    histArray[isam][2] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_LF_Boost/Mu/mWprime_GE0BTags_LF_Boost_"+allNames[isam]+"_Mu");
    histArray[isam][3] = (TH1F*)gDirectory->Get(inDir+fileName+":/GE0BTags_HF_Boost/Mu/mWprime_GE0BTags_HF_Boost_"+allNames[isam]+"_Mu");*/

    histosEX0lf.push_back(histArray[isam][0]);
    histosEX0hf.push_back(histArray[isam][1]);
    histosGE0lf.push_back(histArray[isam][2]);
    histosGE0hf.push_back(histArray[isam][3]);
  }


  //Global post-processing of histos: get rid of negative values
  //and add overflow to final bin
  /*for (unsigned int uh = 0; uh < histosEX0lf.size(); uh++){
    unsigned int nb = histosEX0lf.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosEX0lf.at(uh)->GetBinContent(ub) < 0){
	histosEX0lf.at(uh)->SetBinContent(ub, 0.0);
	histosEX0lf.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosEX0lf.at(uh)->GetBinContent(nb + 1);
    double vLB = histosEX0lf.at(uh)->GetBinContent(nb);

    double eOF = histosEX0lf.at(uh)->GetBinError(nb + 1);
    double eLB = histosEX0lf.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosEX0lf.at(uh)->SetBinContent(nb, vLB + vOF);
      histosEX0lf.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  for (unsigned int uh = 0; uh < histosGE0lf.size(); uh++){
    unsigned int nb = histosGE0lf.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosGE0lf.at(uh)->GetBinContent(ub) < 0){
	histosGE0lf.at(uh)->SetBinContent(ub, 0.0);
	histosGE0lf.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosGE0lf.at(uh)->GetBinContent(nb + 1);
    double vLB = histosGE0lf.at(uh)->GetBinContent(nb);

    double eOF = histosGE0lf.at(uh)->GetBinError(nb + 1);
    double eLB = histosGE0lf.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosGE0lf.at(uh)->SetBinContent(nb, vLB + vOF);
      histosGE0lf.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  for (unsigned int uh = 0; uh < histosEX0hf.size(); uh++){
    unsigned int nb = histosEX0hf.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosEX0hf.at(uh)->GetBinContent(ub) < 0){
	histosEX0hf.at(uh)->SetBinContent(ub, 0.0);
	histosEX0hf.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosEX0hf.at(uh)->GetBinContent(nb + 1);
    double vLB = histosEX0hf.at(uh)->GetBinContent(nb);

    double eOF = histosEX0hf.at(uh)->GetBinError(nb + 1);
    double eLB = histosEX0hf.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosEX0hf.at(uh)->SetBinContent(nb, vLB + vOF);
      histosEX0hf.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  for (unsigned int uh = 0; uh < histosGE0hf.size(); uh++){
    unsigned int nb = histosGE0hf.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosGE0hf.at(uh)->GetBinContent(ub) < 0){
	histosGE0hf.at(uh)->SetBinContent(ub, 0.0);
	histosGE0hf.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosGE0hf.at(uh)->GetBinContent(nb + 1);
    double vLB = histosGE0hf.at(uh)->GetBinContent(nb);

    double eOF = histosGE0hf.at(uh)->GetBinError(nb + 1);
    double eLB = histosGE0hf.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosGE0hf.at(uh)->SetBinContent(nb, vLB + vOF);
      histosGE0hf.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }*/

  //QCD
  //
  //
  std::cout<<"Data = "<<histosEX0lf.at(Data)->Integral();
  std::cout<<" | "<<histosGE0lf.at(Data)->Integral()<<std::endl;
  if (useQCD){
    /*TH1F* qcdBG_EX0lf = (TH1F*)histosEX0lf.at(QCD_HT200to300)->Clone("qcdBG_EX0lf");
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT300to500));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT500to700));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT700to1000));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT1000to1500));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT1500to2000));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_HT2000toInf));*/
    TH1F* qcdBG_EX0lf = (TH1F*)histosEX0lf.at(QCD_Pt_120to170)->Clone("qcdBG_EX0lf");
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_170to300));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_300to470));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_470to600));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_600to800));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_800to1000));
    qcdBG_EX0lf->Add(histosEX0lf.at(QCD_Pt_1000to1400));

    histosEX0lf.at(Data)->Add(qcdBG_EX0lf,-1.*scale);

    /*TH1F* qcdBG_GE0lf = (TH1F*)histosGE0lf.at(QCD_HT200to300)->Clone("qcdBG_GE0lf");
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT300to500));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT500to700));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT700to1000));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT1000to1500));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT1500to2000));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_HT2000toInf));*/
    TH1F* qcdBG_GE0lf = (TH1F*)histosGE0lf.at(QCD_Pt_120to170)->Clone("qcdBG_GE0lf");
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_170to300));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_300to470));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_470to600));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_600to800));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_800to1000));
    qcdBG_GE0lf->Add(histosGE0lf.at(QCD_Pt_1000to1400));

    histosGE0lf.at(Data)->Add(qcdBG_GE0lf,-1.*scale);

    /*TH1F* qcdBG_EX0hf = (TH1F*)histosEX0hf.at(QCD_HT200to300)->Clone("qcdBG_EX0hf");
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT300to500));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT500to700));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT700to1000));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT1000to1500));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT1500to2000));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_HT2000toInf));*/
    TH1F* qcdBG_EX0hf = (TH1F*)histosEX0hf.at(QCD_Pt_120to170)->Clone("qcdBG_EX0hf");
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_170to300));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_300to470));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_470to600));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_600to800));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_800to1000));
    qcdBG_EX0hf->Add(histosEX0hf.at(QCD_Pt_1000to1400));

    histosEX0hf.at(Data)->Add(qcdBG_EX0hf,-1.*scale);

    /*TH1F* qcdBG_GE0hf = (TH1F*)histosGE0hf.at(QCD_HT200to300)->Clone("qcdBG_GE0hf");
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT300to500));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT500to700));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT700to1000));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT1000to1500));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT1500to2000));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_HT2000toInf));*/
    TH1F* qcdBG_GE0hf = (TH1F*)histosGE0hf.at(QCD_Pt_120to170)->Clone("qcdBG_GE0hf");
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_170to300));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_300to470));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_470to600));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_600to800));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_800to1000));
    qcdBG_GE0hf->Add(histosGE0hf.at(QCD_Pt_1000to1400));

    histosGE0hf.at(Data)->Add(qcdBG_GE0hf,-1.*scale);
  }

  //Vector Boson BG_EX0lf
  //TH1F* vbBG_EX0lf = (TH1F*)histosEX0lf.at(WJets)->Clone("vbBG_EX0lf");
  TH1F* vbBG_EX0lf = (TH1F*)histosEX0lf.at(WJets_Pt600toInf)->Clone("vbBG_EX0lf");
  vbBG_EX0lf->Add(histosEX0lf.at(WJets_Pt100to250));
  vbBG_EX0lf->Add(histosEX0lf.at(WJets_Pt250to400));
  vbBG_EX0lf->Add(histosEX0lf.at(WJets_Pt400to600));
  vbBG_EX0lf->Scale(scale);
  std::cout<<"WJLF = "<<vbBG_EX0lf->Integral();
  /*vbBG_EX0lf->Add(histosEX0lf.at(ZJets_HT100to200));
  vbBG_EX0lf->Add(histosEX0lf.at(ZJets_HT200to400));
  vbBG_EX0lf->Add(histosEX0lf.at(ZJets_HT400to600));
  vbBG_EX0lf->Add(histosEX0lf.at(ZJets_HT600toInf));*/
  TH1F* vbM_EX0lf = (TH1F*)histosEX0lf.at(ZJets)->Clone("vbM_EX0lf");
  vbM_EX0lf->Add(histosEX0lf.at(WW));
  vbM_EX0lf->Add(histosEX0lf.at(WZ));
  vbM_EX0lf->Add(histosEX0lf.at(ZZ));

  histosEX0lf.at(Data)->Add(vbM_EX0lf,-1.*scale);

  //Vector Boson BG_GE0lf
  //TH1F* vbBG_GE0lf = (TH1F*)histosGE0lf.at(WJets)->Clone("vbBG_GE0lf");
  TH1F* vbBG_GE0lf = (TH1F*)histosGE0lf.at(WJets_Pt600toInf)->Clone("vbBG_GE0lf");
  vbBG_GE0lf->Add(histosGE0lf.at(WJets_Pt100to250));
  vbBG_GE0lf->Add(histosGE0lf.at(WJets_Pt250to400));
  vbBG_GE0lf->Add(histosGE0lf.at(WJets_Pt400to600));
  vbBG_GE0lf->Scale(scale);
  std::cout<<" | "<<vbBG_GE0lf->Integral()<<std::endl;
  /*vbBG_GE0lf->Add(histosGE0lf.at(ZJets_HT100to200));
  vbBG_GE0lf->Add(histosGE0lf.at(ZJets_HT200to400));
  vbBG_GE0lf->Add(histosGE0lf.at(ZJets_HT400to600));
  vbBG_GE0lf->Add(histosGE0lf.at(ZJets_HT600toInf));*/
  TH1F* vbM_GE0lf = (TH1F*)histosGE0lf.at(ZJets)->Clone("vbM_GE0lf");
  vbM_GE0lf->Add(histosGE0lf.at(WW));
  vbM_GE0lf->Add(histosGE0lf.at(WZ));
  vbM_GE0lf->Add(histosGE0lf.at(ZZ));

  histosGE0lf.at(Data)->Add(vbM_GE0lf,-1.*scale);

  //Top BG_EX0lf
  TH1F* topBG_EX0lf = (TH1F*)histosEX0lf.at(TTbar)->Clone("topBG_EX0lf");
  topBG_EX0lf->Add(histosEX0lf.at(T_s));
  topBG_EX0lf->Add(histosEX0lf.at(T_t));
  topBG_EX0lf->Add(histosEX0lf.at(T_tW));
  //topBG_EX0lf->Add(histosEX0lf.at(Tbar_s));
  topBG_EX0lf->Add(histosEX0lf.at(Tbar_t));
  topBG_EX0lf->Add(histosEX0lf.at(Tbar_tW));

  histosEX0lf.at(Data)->Add(topBG_EX0lf,-1.*scale);

  //Top BG_GE0lf
  TH1F* topBG_GE0lf = (TH1F*)histosGE0lf.at(TTbar)->Clone("topBG_GE0lf");
  topBG_GE0lf->Add(histosGE0lf.at(T_s));
  topBG_GE0lf->Add(histosGE0lf.at(T_t));
  topBG_GE0lf->Add(histosGE0lf.at(T_tW));
  //topBG_GE0lf->Add(histosGE0lf.at(Tbar_s));
  topBG_GE0lf->Add(histosGE0lf.at(Tbar_t));
  topBG_GE0lf->Add(histosGE0lf.at(Tbar_tW));

  histosGE0lf.at(Data)->Add(topBG_GE0lf,-1.*scale);

  //Vector Boson BG_EX0hf
  //TH1F* vbBG_EX0hf = (TH1F*)histosEX0hf.at(WJets)->Clone("vbBG_EX0hf");
  TH1F* vbBG_EX0hf = (TH1F*)histosEX0hf.at(WJets_Pt600toInf)->Clone("vbBG_EX0hf");
  vbBG_EX0hf->Add(histosEX0hf.at(WJets_Pt100to250));
  vbBG_EX0hf->Add(histosEX0hf.at(WJets_Pt250to400));
  vbBG_EX0hf->Add(histosEX0hf.at(WJets_Pt400to600));
  std::cout<<"WJHF = "<<vbBG_EX0hf->Integral();
  vbBG_EX0hf->Scale(scale);
  /*vbBG_EX0hf->Add(histosEX0hf.at(ZJets_HT100to200));
  vbBG_EX0hf->Add(histosEX0hf.at(ZJets_HT200to400));
  vbBG_EX0hf->Add(histosEX0hf.at(ZJets_HT400to600));
  vbBG_EX0hf->Add(histosEX0hf.at(ZJets_HT600toInf));*/
  TH1F* vbM_EX0hf = (TH1F*)histosEX0hf.at(ZJets)->Clone("vbM_EX0hf");
  vbM_EX0hf->Add(histosEX0hf.at(WW));
  vbM_EX0hf->Add(histosEX0hf.at(WZ));
  vbM_EX0hf->Add(histosEX0hf.at(ZZ));

  histosEX0hf.at(Data)->Add(vbM_EX0hf,-1.*scale);

  //Vector Boson BG_GE0hf
  //TH1F* vbBG_GE0hf = (TH1F*)histosGE0hf.at(WJets)->Clone("vbBG_GE0hf");
  TH1F* vbBG_GE0hf = (TH1F*)histosGE0hf.at(WJets_Pt600toInf)->Clone("vbBG_GE0hf");
  vbBG_GE0hf->Add(histosGE0hf.at(WJets_Pt100to250));
  vbBG_GE0hf->Add(histosGE0hf.at(WJets_Pt250to400));
  vbBG_GE0hf->Add(histosGE0hf.at(WJets_Pt400to600));
  vbBG_GE0hf->Scale(scale);
  std::cout<<" | "<<vbBG_GE0hf->Integral()<<std::endl;
  /*vbBG_GE0hf->Add(histosGE0hf.at(ZJets_HT100to200));
  vbBG_GE0hf->Add(histosGE0hf.at(ZJets_HT200to400));
  vbBG_GE0hf->Add(histosGE0hf.at(ZJets_HT400to600));
  vbBG_GE0hf->Add(histosGE0hf.at(ZJets_HT600toInf));*/
  TH1F* vbM_GE0hf = (TH1F*)histosGE0hf.at(ZJets)->Clone("vbM_GE0hf");
  vbM_GE0hf->Add(histosGE0hf.at(WW));
  vbM_GE0hf->Add(histosGE0hf.at(WZ));
  vbM_GE0hf->Add(histosGE0hf.at(ZZ));

  histosEX0hf.at(Data)->Add(vbM_EX0hf,-1.*scale);

  //Top BG_EX0hf
  TH1F* topBG_EX0hf = (TH1F*)histosEX0hf.at(TTbar)->Clone("topBG_EX0hf");
  topBG_EX0hf->Add(histosEX0hf.at(T_s));
  topBG_EX0hf->Add(histosEX0hf.at(T_t));
  topBG_EX0hf->Add(histosEX0hf.at(T_tW));
  //topBG_EX0hf->Add(histosEX0hf.at(Tbar_s));
  topBG_EX0hf->Add(histosEX0hf.at(Tbar_t));
  topBG_EX0hf->Add(histosEX0hf.at(Tbar_tW));

  histosEX0hf.at(Data)->Add(topBG_EX0hf,-1.*scale);

  //Top BG_GE0hf
  TH1F* topBG_GE0hf = (TH1F*)histosGE0hf.at(TTbar)->Clone("topBG_GE0hf");
  topBG_GE0hf->Add(histosGE0hf.at(T_s));
  topBG_GE0hf->Add(histosGE0hf.at(T_t));
  topBG_GE0hf->Add(histosGE0hf.at(T_tW));
  //topBG_GE0hf->Add(histosGE0hf.at(Tbar_s));
  topBG_GE0hf->Add(histosGE0hf.at(Tbar_t));
  topBG_GE0hf->Add(histosGE0hf.at(Tbar_tW));

  histosGE0hf.at(Data)->Add(topBG_GE0hf,-1.*scale);

  /*std::cout<<"Counts:\n\tData  (  0 b-tag) : "<<(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())<<std::endl;
  std::cout<<"\tData  (>=0 b-tag) : "<<(histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())<<std::endl;
  std::cout<<"\tWJ LF (  0 b-tag) : "<<(vbBG_EX0lf->Integral())<<std::endl;
  std::cout<<"\tWJ LF (>=0 b-tag) : "<<(vbBG_GE0lf->Integral())<<std::endl;
  std::cout<<"\tWJ HF (  0 b-tag) : "<<(vbBG_EX0hf->Integral())<<std::endl;
  std::cout<<"\tWJ HF (>=0 b-tag) : "<<(vbBG_GE0hf->Integral())<<std::endl;*/

  /*std::cout<<"Inclusive template fit:"<<std::endl;
  h1 = (TH1F*)vbBG_GE0hf->Clone("h1");
  h2 = (TH1F*)vbBG_GE0lf->Clone("h2");
  TH1F* hdata = (TH1F*)histosGE0hf.at(Data)->Clone("hdata");
  hdata->Add(histosGE0lf.at(Data));
  //h1->Rebin(100);
  //h2->Rebin(100);
  //hdata->Rebin(100);
  TF1 fitfunc("wjets_func", wjets_func, 0., 5000., 1);
  fitfunc.SetParameter(0,1.);
  fitfunc.SetParLimits(0,0.1,10.);
  TFitResultPtr fitres = hdata->Fit(&fitfunc, "QL0S");*/
  
  std::cout<<"Control Region template fit:"<<std::endl;
  h1 = (TH1F*)vbBG_EX0hf->Clone("h1");
  h2 = (TH1F*)vbBG_EX0lf->Clone("h2");
  TH1F* hdata = (TH1F*)histosEX0hf.at(Data)->Clone("hdata");
  hdata->Add(histosEX0lf.at(Data));
  h1->Rebin(100);
  h2->Rebin(100);
  hdata->Rebin(100);
  TF1 fitfunc("wjets_func", wjets_func, 0., 5000., 1);
  fitfunc.SetParameter(0,1.);
  fitfunc.SetParLimits(0,0.1,10.);
  TFitResultPtr fitres = hdata->Fit(&fitfunc, "QL0S");
  
  std::cout<<"\tWJHFSF = "<<fitres->Parameter(0)<<";"<<std::endl;
  std::cout<<"\tWJLFSF = "<<1./fitres->Parameter(0)<<";\n"<<std::endl;
  std::cout<<"Signal Region template fit:"<<std::endl;
  h1 = (TH1F*)vbBG_GE0hf->Clone("h1");
  h2 = (TH1F*)vbBG_GE0lf->Clone("h2");
  h1->Add(vbBG_EX0hf,-1.);
  h2->Add(vbBG_EX0lf,-1.);
  hdata = (TH1F*)histosGE0hf.at(Data)->Clone("hdata");
  hdata->Add(histosGE0lf.at(Data));
  hdata->Add(histosEX0lf.at(Data),-1.);
  hdata->Add(histosEX0hf.at(Data),-1.);
  h1->Rebin(100);
  h2->Rebin(100);
  hdata->Rebin(100);
  fitfunc.SetParameter(0,1.);
  fitfunc.SetParLimits(0,0.1,10.);
  fitres = hdata->Fit(&fitfunc, "QL0S");
  
  std::cout<<"\tWJHFSF = "<<fitres->Parameter(0)<<";"<<std::endl;
  std::cout<<"\tWJLFSF = "<<1./fitres->Parameter(0)<<";\n"<<std::endl;

  double diffLF, diffHF;
  double diff = 100.;
  int count = 0;
  double WLF = 1.;
  double WLFtmp = 1.;
  double WHF = 1.;
  double WHFtmp = 1.;
  std::cout<<"Initial Data/MC:\n\t0 b-tag: "<<(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())/(WLFtmp*(vbBG_EX0lf->Integral())+WHFtmp*(vbBG_EX0hf->Integral()))<<"\n\t>=0 b-tag: "<<(histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral()))<<std::endl;

  vbBG_EX0lf->Scale((histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral())));
  vbBG_EX0hf->Scale((histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral())));
  double tmpscale = (histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral()));
  vbBG_GE0lf->Scale(tmpscale);
  vbBG_GE0hf->Scale(tmpscale);
  std::cout<<"Sanity check Data/MC:\n\t0 b-tag: "<<(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())/(WLFtmp*(vbBG_EX0lf->Integral())+WHFtmp*(vbBG_EX0hf->Integral()))<<"\n\t>=0 b-tag: "<<(histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral()))<<std::endl;

  std::cout<<"Solving gives:\n\tW+jets Heavy Flavor SF = "<<((histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())*vbBG_EX0lf->Integral()-(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())*vbBG_GE0lf->Integral())/(vbBG_GE0hf->Integral()*vbBG_EX0lf->Integral()-vbBG_EX0hf->Integral()*vbBG_GE0lf->Integral())<<"\n\tW+jets Light Flavor SF = "<<((histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())-vbBG_GE0hf->Integral()*(((histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())*vbBG_EX0lf->Integral()-(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())*vbBG_GE0lf->Integral())/(vbBG_GE0hf->Integral()*vbBG_EX0lf->Integral()-vbBG_EX0hf->Integral()*vbBG_GE0lf->Integral())))/vbBG_GE0lf->Integral()<<std::endl;

  while (diff > 0.001) {
    //std::cout<<"After "<<count<<" iterations absolute diff is "<<diff<<":\n\tW+jets Heavy Flavor SF = "<<WHF<<"\n\tW+jets Light Flavor SF = "<<WLF<<std::endl;
    WLFtmp = (histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())/(WLFtmp*(vbBG_EX0lf->Integral())+WHFtmp*(vbBG_EX0hf->Integral()));
    //scale everything according to new WLF
    vbBG_EX0lf->Scale(WLFtmp);
    vbBG_GE0lf->Scale(WLFtmp);

    WHFtmp = (histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/(WLFtmp*(vbBG_GE0lf->Integral())+WHFtmp*(vbBG_GE0hf->Integral()));
    //scale everything according to new WHF
    vbBG_EX0hf->Scale(WHFtmp);
    vbBG_GE0hf->Scale(WHFtmp);
    
    //diffLF = fabs(1.-WLFtmp);
    //diffHF = fabs(1.-WHFtmp);
    //if (diffHF<diffLF) diff = diffHF;
    //else diff = diffLF;
    diff = fabs(1.-WHFtmp);
    WHF = WHF * WHFtmp;
    WLF = WLF * WLFtmp;

    count++;
    if (count==100) {
      std::cout<<"Forcing stop after 100 iterations!"<<std::endl;
      break;
    }
  }

  std::cout<<"After "<<count<<" iterations converged:\n\tW+jets Heavy Flavor SF = "<<WHF<<"\n\tW+jets Light Flavor SF = "<<WLF<<std::endl;
  std::cout<<"Final Data/MC:\n\t0 b-tag: "<<(histosEX0lf.at(Data)->Integral()+histosEX0hf.at(Data)->Integral())/((vbBG_EX0lf->Integral())+(vbBG_EX0hf->Integral()))<<"\n\t>=0 b-tag: "<<(histosGE0lf.at(Data)->Integral()+histosGE0hf.at(Data)->Integral())/((vbBG_GE0lf->Integral())+(vbBG_GE0hf->Integral()))<<std::endl;

}

double wjets_func(double* x, double* pars) {
  const double xx = x[0]; // use x[1] to get 2nd dimension, x[2] for 3rd ...
  // the fit parameters, i.e. the histogram weights
  const double w1 = pars[0];
  // get content of the histograms for this point
  const double y1 = h1->GetBinContent(h1->GetXaxis()->FindFixBin(xx));
  const double y2 = h2->GetBinContent(h2->GetXaxis()->FindFixBin(xx));
  //return y1*w1 + y2*(1.+(h1->Integral()*(1.-w1)/h2->Integral()));
  return y1/w1 + y2*w1;
}

#endif
