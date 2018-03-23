#ifndef DrawOneHistoFit_cxx
#define DrawOneHistoFit_cxx

#include <algorithm>
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH1.h>
#include <TF1.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TPaveStats.h>

#include "WprimeLoadData2016.C"
#include "tdrstyle.C"

using namespace std;

int main( int argc, const char* argv[] ){

  if (argc < 7){
    cout<<"Need at least 6 arguments. Only "<<argc<<" found."<<endl;
    return 1;
  }
  
  setTDRStyle();

  //Root file information
  TString inDir  = argv[1];
  TString inSuff = argv[2];
  TString inPref = argv[3];
  TString inName = argv[4];

  //Drawing information
  int yLog = atoi(argv[5]);
  TString xTitle = argv[6];

  int rebin = 1;
  if (argc > 7) rebin = atoi(argv[7]);
  
  int qcdin = 0;
  if (argc > 8) qcdin = atoi(argv[8]);
  bool useQCD = false;
  if (qcdin>0) useQCD = true;

  double scale = 1.0;
  if (argc > 9) scale = atof(argv[9]);

  double minrange = 0.;
  bool rangeset1 = false;
  if (argc > 10) {
    minrange = atof(argv[10]);
    rangeset1 = true;
  }
  double maxrange = 0.;
  bool rangeset2 = false;
  if (argc > 11) {
    maxrange = atof(argv[11]);
    rangeset2 = true;
  }

  cout<<inDir<<endl;
  cout<<inSuff<<endl;
  cout<<inPref<<endl;
  cout<<inName<<endl;
  cout<<yLog<<endl;
  cout<<xTitle<<endl;
  cout<<rebin<<endl;
    
  const unsigned int NCHAN    = 3;
  const unsigned int NSAMPLES = totalSamples;

  double WJHFSF = 1.;
  double WJLFSF = 1.;

  TString fileName = "outFile.root";
  TFile *file0 = TFile::Open(inDir+fileName);
  file0->cd();

  TH1F* histArray[NSAMPLES][NCHAN];
  vector <TH1F*> histos;

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);
  std::vector<double> binvec;
  double xbins[23] = {0., 50., 100., 150., 200., 250., 300., 350., 400., 450., 500., 550., 600., 650., 700., 750., 800., 850., 900., 950., 1000., 1100., 1250.};

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (!useQCD and (isam == ZJets_HT100to200 or isam == ZJets_HT200to400 or isam == ZJets_HT400to600 or isam == ZJets_HT600to800 or isam == ZJets_HT800to1200 or isam == ZJets_HT1200to2500 or isam == ZJets_HT2500toInf or isam == WJets_HT100to200 or isam == WJets_HT200to400 or isam == WJets_HT400to600 or isam == WJets_HT600to800 or isam == WJets_HT800to1200 or isam == WJets_HT1200to2500 or isam == WJets_HT2500toInf or isam == WJets or isam == Wprime800Right or isam == Wprime900Right or isam == Wprime1000Right or isam == Wprime1100Right or isam == Wprime1200Right or isam == Wprime1300Right or isam == Wprime1400Right or isam == Wprime1600Right or isam == Wprime1700Right or isam == Wprime1800Right or isam == Wprime1900Right or isam == Wprime2100Right or isam == Wprime2200Right or isam == Wprime2300Right or isam == Wprime2400Right or isam == Wprime2600Right or isam == Wprime2700Right or isam == Wprime2800Right or isam == Wprime2900Right or isam == Wprime3000Right or isam == QCD_HT200to300 or isam == QCD_HT300to500 or isam == QCD_HT500to700 or isam == QCD_HT700to1000 or isam == QCD_HT1000to1500 or isam == QCD_HT1500to2000 or isam == QCD_HT2000toInf or isam == QCD_Pt_5to10 or isam == QCD_Pt_10to15 or isam == QCD_Pt_15to30 or isam == QCD_Pt_30to50 or isam == QCD_Pt_50to80 or isam == QCD_Pt_80to120 or isam == QCD_Pt_120to170 or isam == QCD_Pt_170to300 or isam == QCD_Pt_300to470 or isam == QCD_Pt_470to600 or isam == QCD_Pt_600to800 or isam == QCD_Pt_800to1000 or isam == QCD_Pt_1000to1400 or isam == QCD_Pt_1400to1800 or isam == QCD_Pt_1800to2400
    or isam == TTbar_scaleup
    or isam == TTbar_scaledown
    or isam == Tbar_s
    or isam == Wprime800Mix
    or isam == Wprime900Mix
    or isam == Wprime1000Mix
    or isam == Wprime1100Mix
    or isam == Wprime1200Mix
    or isam == Wprime1300Mix
    or isam == Wprime1400Mix
    or isam == Wprime1500Mix
    or isam == Wprime1600Mix
    or isam == Wprime1700Mix
    or isam == Wprime1800Mix
    or isam == Wprime1900Mix
    or isam == Wprime2000Mix
    or isam == Wprime2100Mix
    or isam == Wprime2200Mix
    or isam == Wprime2300Mix
    or isam == Wprime2400Mix
    or isam == Wprime2500Mix
    or isam == Wprime2600Mix
    or isam == Wprime2700Mix
    or isam == Wprime2800Mix
    or isam == Wprime2900Mix
    or isam == Wprime3000Mix
    or isam == Wprime800Left
    or isam == Wprime900Left
    or isam == Wprime1000Left
    or isam == Wprime1100Left
    or isam == Wprime1200Left
    or isam == Wprime1300Left
    or isam == Wprime1400Left
    or isam == Wprime1500Left
    or isam == Wprime1600Left
    or isam == Wprime1700Left
    or isam == Wprime1800Left
    or isam == Wprime1900Left
    or isam == Wprime2000Left
    or isam == Wprime2100Left
    or isam == Wprime2200Left
    or isam == Wprime2300Left
    or isam == Wprime2400Left
    or isam == Wprime2500Left
    or isam == Wprime2600Left
    or isam == Wprime2700Left
    or isam == Wprime2800Left
    or isam == Wprime2900Left
    or isam == Wprime3000Left
    or isam == Wprime1000Right
    or isam == Wprime1100Right
    or isam == Wprime1200Right
    or isam == Wprime1300Right
    or isam == Wprime1400Right
    or isam == Wprime1500Right
    or isam == Wprime1600Right
    or isam == Wprime1700Right
    or isam == Wprime1800Right
    or isam == Wprime1900Right
    or isam == Wprime2000Right
    or isam == Wprime2100Right
    or isam == Wprime2200Right
    or isam == Wprime2300Right
    or isam == Wprime2400Right
    or isam == Wprime2500Right
    or isam == Wprime2600Right
    or isam == Wprime2700Right
    or isam == Wprime2800Right
    or isam == Wprime2900Right
    or isam == Wprime3000Right
    or isam == Wprime3100Right
    or isam == Wprime3200Right
    or isam == Wprime3300Right
    or isam == Wprime3400Right
    or isam == Wprime3500Right
    or isam == Wprime3600Right
    or isam == Wprime3700Right
    or isam == Wprime3800Right
    or isam == Wprime3900Right
    or isam == Wprime4000Right
    or isam == Wprime3100Mix
    or isam == Wprime3200Mix
    or isam == Wprime3300Mix
    or isam == Wprime3400Mix
    or isam == Wprime3500Mix
    or isam == Wprime3600Mix
    or isam == Wprime3700Mix
    or isam == Wprime3800Mix
    or isam == Wprime3900Mix
    or isam == Wprime4000Mix
    or isam == Wprime3100Left
    or isam == Wprime3200Left
    or isam == Wprime3300Left
    or isam == Wprime3400Left
    or isam == Wprime3500Left
    or isam == Wprime3600Left
    or isam == Wprime3700Left
    or isam == Wprime3800Left
    or isam == Wprime3900Left
    or isam == Wprime4000Left
    )){
      histos.push_back(emptyHisto);
      continue;
    }
    if (useQCD and (isam == ZJets_HT100to200 or isam == ZJets_HT200to400 or isam == ZJets_HT400to600 or isam == ZJets_HT600to800 or isam == ZJets_HT800to1200 or isam == ZJets_HT1200to2500 or isam == ZJets_HT2500toInf or isam == WJets_HT100to200 or isam == WJets_HT200to400 or isam == WJets_HT400to600 or isam == WJets_HT600to800 or isam == WJets_HT800to1200 or isam == WJets_HT1200to2500 or isam == WJets_HT2500toInf or isam == WJets or isam == Wprime800Right or isam == Wprime900Right or isam == Wprime1000Right or isam == Wprime1100Right or isam == Wprime1200Right or isam == Wprime1300Right or isam == Wprime1400Right or isam == Wprime1600Right or isam == Wprime1700Right or isam == Wprime1800Right or isam == Wprime1900Right or isam == Wprime2100Right or isam == Wprime2200Right or isam == Wprime2300Right or isam == Wprime2400Right or isam == Wprime2600Right or isam == Wprime2700Right or isam == Wprime2800Right or isam == Wprime2900Right or isam == Wprime3000Right or isam == QCD_HT200to300 or isam == QCD_HT300to500 or isam == QCD_HT500to700 or isam == QCD_HT700to1000 or isam == QCD_HT1000to1500 or isam == QCD_HT1500to2000 or isam == QCD_HT2000toInf or isam == QCD_Pt_5to10 or isam == QCD_Pt_10to15 or isam == QCD_Pt_15to30 or isam == QCD_Pt_30to50 or isam == QCD_Pt_50to80 or isam == QCD_Pt_80to120 or isam == QCD_Pt_1400to1800 or isam == QCD_Pt_1800to2400
    or isam == TTbar_scaleup
    or isam == TTbar_scaledown
    or isam == Tbar_s
    or isam == Wprime800Mix
    or isam == Wprime900Mix
    or isam == Wprime1000Mix
    or isam == Wprime1100Mix
    or isam == Wprime1200Mix
    or isam == Wprime1300Mix
    or isam == Wprime1400Mix
    or isam == Wprime1500Mix
    or isam == Wprime1600Mix
    or isam == Wprime1700Mix
    or isam == Wprime1800Mix
    or isam == Wprime1900Mix
    or isam == Wprime2000Mix
    or isam == Wprime2100Mix
    or isam == Wprime2200Mix
    or isam == Wprime2300Mix
    or isam == Wprime2400Mix
    or isam == Wprime2500Mix
    or isam == Wprime2600Mix
    or isam == Wprime2700Mix
    or isam == Wprime2800Mix
    or isam == Wprime2900Mix
    or isam == Wprime3000Mix
    or isam == Wprime800Left
    or isam == Wprime900Left
    or isam == Wprime1000Left
    or isam == Wprime1100Left
    or isam == Wprime1200Left
    or isam == Wprime1300Left
    or isam == Wprime1400Left
    or isam == Wprime1500Left
    or isam == Wprime1600Left
    or isam == Wprime1700Left
    or isam == Wprime1800Left
    or isam == Wprime1900Left
    or isam == Wprime2000Left
    or isam == Wprime2100Left
    or isam == Wprime2200Left
    or isam == Wprime2300Left
    or isam == Wprime2400Left
    or isam == Wprime2500Left
    or isam == Wprime2600Left
    or isam == Wprime2700Left
    or isam == Wprime2800Left
    or isam == Wprime2900Left
    or isam == Wprime3000Left
    or isam == Wprime1000Right
    or isam == Wprime1100Right
    or isam == Wprime1200Right
    or isam == Wprime1300Right
    or isam == Wprime1400Right
    or isam == Wprime1500Right
    or isam == Wprime1600Right
    or isam == Wprime1700Right
    or isam == Wprime1800Right
    or isam == Wprime1900Right
    or isam == Wprime2000Right
    or isam == Wprime2100Right
    or isam == Wprime2200Right
    or isam == Wprime2300Right
    or isam == Wprime2400Right
    or isam == Wprime2500Right
    or isam == Wprime2600Right
    or isam == Wprime2700Right
    or isam == Wprime2800Right
    or isam == Wprime2900Right
    or isam == Wprime3000Right
    or isam == Wprime3100Right
    or isam == Wprime3200Right
    or isam == Wprime3300Right
    or isam == Wprime3400Right
    or isam == Wprime3500Right
    or isam == Wprime3600Right
    or isam == Wprime3700Right
    or isam == Wprime3800Right
    or isam == Wprime3900Right
    or isam == Wprime4000Right
    or isam == Wprime3100Mix
    or isam == Wprime3200Mix
    or isam == Wprime3300Mix
    or isam == Wprime3400Mix
    or isam == Wprime3500Mix
    or isam == Wprime3600Mix
    or isam == Wprime3700Mix
    or isam == Wprime3800Mix
    or isam == Wprime3900Mix
    or isam == Wprime4000Mix
    or isam == Wprime3100Left
    or isam == Wprime3200Left
    or isam == Wprime3300Left
    or isam == Wprime3400Left
    or isam == Wprime3500Left
    or isam == Wprime3600Left
    or isam == Wprime3700Left
    or isam == Wprime3800Left
    or isam == Wprime3900Left
    or isam == Wprime4000Left
    )){
      histos.push_back(emptyHisto);
      continue;
    }
    //std::cout<<"\t"+inName+"_"+inPref+"_"+allNames[isam]<<std::endl;
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/Mu/"+inName+"_"+inPref+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/El/"+inName+"_"+inPref+"_"+allNames[isam]+"_El");

    if (rebin != 1){
      histArray[isam][0]->Rebin(rebin);
      histArray[isam][1]->Rebin(rebin);
    }

    histArray[isam][2] = (TH1F*)histArray[isam][0]->Clone("histArray_"+allNames[isam]);
    histArray[isam][2]->Add(histArray[isam][1]);
    
    if (isam == WJets_HT100to200_LF or isam == WJets_HT200to400_LF or isam == WJets_HT400to600_LF or isam == WJets_HT600to800_LF or isam == WJets_HT800to1200_LF or isam == WJets_HT1200to2500_LF or isam == WJets_HT2500toInf_LF) {
      histArray[isam][0]->Scale(WJLFSF);
      histArray[isam][1]->Scale(WJLFSF);
      histArray[isam][2]->Scale(WJLFSF);
    }
    else if (isam == WJets_HT100to200_HF or isam == WJets_HT200to400_HF or isam == WJets_HT400to600_HF or isam == WJets_HT600to800_HF or isam == WJets_HT800to1200_HF or isam == WJets_HT1200to2500_HF or isam == WJets_HT2500toInf_HF) {
      histArray[isam][0]->Scale(WJHFSF);
      histArray[isam][1]->Scale(WJHFSF);
      histArray[isam][2]->Scale(WJHFSF);
    }
    
    if      (inSuff == "Mu") histos.push_back(histArray[isam][0]);
    else if (inSuff == "El") histos.push_back(histArray[isam][1]);
    else if (inSuff == "All")  histos.push_back(histArray[isam][2]);
    else{
      cout<<"Invalid suffix"<<endl;
      return 1;
    }

  }

  //Global post-processing of histos: get rid of negative values
  //and add overflow to final bin
  for (unsigned int uh = 0; uh < histos.size(); uh++){
    unsigned int nb = histos.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histos.at(uh)->GetBinContent(ub) < 0){
	histos.at(uh)->SetBinContent(ub, 0.0);
	histos.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histos.at(uh)->GetBinContent(nb + 1);
    double vLB = histos.at(uh)->GetBinContent(nb);

    double eOF = histos.at(uh)->GetBinError(nb + 1);
    double eLB = histos.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histos.at(uh)->SetBinContent(nb, vLB + vOF);
      histos.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  //Calculate systematics
  vector <TH1F*> selectedHistos;
  selectedHistos.push_back(histos.at(TTbar));
  selectedHistos.push_back(histos.at(WW));  
  selectedHistos.push_back(histos.at(WZ));  
  selectedHistos.push_back(histos.at(ZZ));  
  //selectedHistos.push_back(histos.at(WJets)); 
  selectedHistos.push_back(histos.at(ZJets)); 
  selectedHistos.push_back(histos.at(WJets_HT100to200_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT200to400_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT400to600_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT600to800_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT800to1200_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT1200to2500_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT2500toInf_HF)); 
  selectedHistos.push_back(histos.at(WJets_HT100to200_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT200to400_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT400to600_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT600to800_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT800to1200_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT1200to2500_LF)); 
  selectedHistos.push_back(histos.at(WJets_HT2500toInf_LF)); 
  /*selectedHistos.push_back(histos.at(ZJets_HT100to200)); 
  selectedHistos.push_back(histos.at(ZJets_HT200to400)); 
  selectedHistos.push_back(histos.at(ZJets_HT400to600)); 
  selectedHistos.push_back(histos.at(ZJets_HT600toInf)); */
  selectedHistos.push_back(histos.at(T_s)); 
  selectedHistos.push_back(histos.at(T_t)); 
  selectedHistos.push_back(histos.at(T_tW));  
  //selectedHistos.push_back(histos.at(Tbar_s));  
  selectedHistos.push_back(histos.at(Tbar_t));
  selectedHistos.push_back(histos.at(Tbar_tW));
  if (useQCD){
    /*selectedHistos.push_back(histos.at(QCD_HT200to300));
    selectedHistos.push_back(histos.at(QCD_HT300to500));
    selectedHistos.push_back(histos.at(QCD_HT500to700));
    selectedHistos.push_back(histos.at(QCD_HT700to1000));
    selectedHistos.push_back(histos.at(QCD_HT1000to1500));
    selectedHistos.push_back(histos.at(QCD_HT1500to2000));
    selectedHistos.push_back(histos.at(QCD_HT2000toInf));*/
    selectedHistos.push_back(histos.at(QCD_Pt_120to170));
    selectedHistos.push_back(histos.at(QCD_Pt_170to300));
    selectedHistos.push_back(histos.at(QCD_Pt_300to470));
    selectedHistos.push_back(histos.at(QCD_Pt_470to600));
    selectedHistos.push_back(histos.at(QCD_Pt_600to800));
    selectedHistos.push_back(histos.at(QCD_Pt_800to1000));
    selectedHistos.push_back(histos.at(QCD_Pt_1000to1400));
  }

  double mcUnc = sqrt (0.02 * 0.02 + //Trigger
                       0.02 * 0.02 + //Lepton Efficiency
                       0.12 * 0.12); //Luminosity
  double qcdUnc = sqrt(0.40*0.40+mcUnc*mcUnc);
  double topUnc = sqrt(0.15*0.15+mcUnc*mcUnc);
  double vbUnc = sqrt(0.20*0.20+mcUnc*mcUnc);

  vector <double> selectedUnc;
  selectedUnc.push_back(topUnc); //TTbar
  selectedUnc.push_back(vbUnc); //WW
  selectedUnc.push_back(vbUnc); //WZ
  selectedUnc.push_back(vbUnc); //ZZ
  //selectedUnc.push_back(vbUnc); //WJets
  selectedUnc.push_back(vbUnc); //ZJets
  selectedUnc.push_back(vbUnc); //WJets_HT100to200
  selectedUnc.push_back(vbUnc); //WJets_HT200to400
  selectedUnc.push_back(vbUnc); //WJets_HT400to600
  selectedUnc.push_back(vbUnc); //WJets_HT600to800
  selectedUnc.push_back(vbUnc); //WJets_HT800to1200
  selectedUnc.push_back(vbUnc); //WJets_HT1200to2500
  selectedUnc.push_back(vbUnc); //WJets_HT2500toInf
  selectedUnc.push_back(vbUnc); //WJets_HT100to200
  selectedUnc.push_back(vbUnc); //WJets_HT200to400
  selectedUnc.push_back(vbUnc); //WJets_HT400to600
  selectedUnc.push_back(vbUnc); //WJets_HT600to800
  selectedUnc.push_back(vbUnc); //WJets_HT800to1200
  selectedUnc.push_back(vbUnc); //WJets_HT1200to2500
  selectedUnc.push_back(vbUnc); //WJets_HT2500toInf
  /*selectedUnc.push_back(vbUnc); //ZJets_HT100to200
  selectedUnc.push_back(vbUnc); //ZJets_HT200to400
  selectedUnc.push_back(vbUnc); //ZJets_HT400to600
  selectedUnc.push_back(vbUnc); //ZJets_HT600toInf*/
  selectedUnc.push_back(topUnc); //T_s
  selectedUnc.push_back(topUnc); //T_t 
  selectedUnc.push_back(topUnc);  //T_tW 
  //selectedUnc.push_back(topUnc);  //Tbar_s  
  selectedUnc.push_back(topUnc);  //Tbar_t
  selectedUnc.push_back(topUnc);  //Tbar_tW
  if (useQCD){
    selectedUnc.push_back(qcdUnc);  //QCD_HT200to300
    selectedUnc.push_back(qcdUnc);  //QCD_HT300to500
    selectedUnc.push_back(qcdUnc);  //QCD_HT500to700
    selectedUnc.push_back(qcdUnc);  //QCD_HT700to1000
    selectedUnc.push_back(qcdUnc);  //QCD_HT1000to1500
    selectedUnc.push_back(qcdUnc);  //QCD_HT1500to2000
    selectedUnc.push_back(qcdUnc);  //QCD_HT2000toInf
  }

  TH1F* h_SumAll = (TH1F*)histos.at(TTbar)->Clone("h_SumAll"); 
  for (unsigned int ih = 1; ih < selectedHistos.size(); ih++){
    h_SumAll->Add(selectedHistos.at(ih));
  }
  
  TH1F* h_lumiBand = (TH1F*) h_SumAll->Clone("h_lumiBand");
  h_lumiBand->Scale(scale);
  
  for (int ibin = 1; ibin < h_lumiBand->GetNbinsX()+1; ibin++){

    double uncStat = 0;
    double uncSyst = 0;
    double uncTot  = 0;

    for (unsigned int ih = 0; ih < selectedHistos.size(); ih++){
      uncStat += selectedHistos.at(ih)->GetBinError(ibin)*selectedHistos.at(ih)->GetBinError(ibin);
      uncSyst += selectedHistos.at(ih)->GetBinContent(ibin)*selectedHistos.at(ih)->GetBinContent(ibin)*selectedUnc.at(ih)*selectedUnc.at(ih);    
    }
 
    uncTot = sqrt(uncStat + uncSyst);    
    h_lumiBand->SetBinError(ibin, uncTot);
  }

  THStack* mystack = new THStack("mystack","mystack");

  TLegend * leg = new TLegend(0.7, 0.7, 0.88, 0.92, "","brNDC");
//   TLegend * leg = new TLegend(0.67, 0.58, 0.89, 0.93, "","brNDC");
  leg->SetFillColor(10);
  leg->SetTextSize(0.02);
  leg->SetTextFont(62);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);

  leg->AddEntry(histos.at(Data), "Data", "ep");

  //QCD
  if (useQCD){
    /*TH1F* qcdBG = (TH1F*)histos.at(QCD_HT200to300)->Clone("qcdBG");
    qcdBG->Add(histos.at(QCD_HT300to500));
    qcdBG->Add(histos.at(QCD_HT500to700));
    qcdBG->Add(histos.at(QCD_HT700to1000));
    qcdBG->Add(histos.at(QCD_HT1000to1500));
    qcdBG->Add(histos.at(QCD_HT1500to2000));
    qcdBG->Add(histos.at(QCD_HT2000toInf));*/
    TH1F* qcdBG = (TH1F*)histos.at(QCD_Pt_800to1000)->Clone("qcdBG");
    qcdBG->Add(histos.at(QCD_Pt_120to170));
    qcdBG->Add(histos.at(QCD_Pt_170to300));
    qcdBG->Add(histos.at(QCD_Pt_300to470));
    qcdBG->Add(histos.at(QCD_Pt_470to600));
    qcdBG->Add(histos.at(QCD_Pt_600to800));
    qcdBG->Add(histos.at(QCD_Pt_1000to1400));

    qcdBG->SetFillColor(kYellow+1);
    qcdBG->SetLineColor(kBlack);
    leg->AddEntry(qcdBG, "QCD", "f");
    qcdBG->Scale(scale);
    mystack->Add(qcdBG);
  }

  //Vector Boson BG
  //TH1F* vbBG = (TH1F*)histos.at(WJets)->Clone("vbBG");
  TH1F* vbBG = (TH1F*)histos.at(WJets_HT2500toInf_HF)->Clone("vbBG");
  vbBG->Scale(WJHFSF);
  vbBG->Add(histos.at(WJets_HT100to200_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT200to400_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT400to600_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT600to800_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT800to1200_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT1200to2500_HF),WJHFSF);
  vbBG->Add(histos.at(WJets_HT100to200_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT200to400_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT400to600_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT600to800_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT800to1200_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT1200to2500_LF),WJLFSF);
  vbBG->Add(histos.at(WJets_HT2500toInf_LF),WJLFSF);
  /*vbBG->Add(histos.at(ZJets_HT100to200));
  vbBG->Add(histos.at(ZJets_HT200to400));
  vbBG->Add(histos.at(ZJets_HT400to600));
  vbBG->Add(histos.at(ZJets_HT600toInf));*/
  vbBG->Add(histos.at(ZJets));

  TH1F* vvbBG = (TH1F*)histos.at(WW)->Clone("vvbBG");
  vvbBG->Add(histos.at(WZ));
  vvbBG->Add(histos.at(ZZ));

  vvbBG->SetFillColor(kMagenta+2);
  vvbBG->SetLineColor(kBlack);
  leg->AddEntry(vvbBG, "VV", "f");
  vvbBG->Scale(scale);
  mystack->Add(vvbBG);

  //Top BG
  TH1F* topBG = (TH1F*)histos.at(TTbar)->Clone("topBG");
  topBG->Add(histos.at(T_s));
  topBG->Add(histos.at(T_t));
  topBG->Add(histos.at(T_tW));
  //topBG->Add(histos.at(Tbar_s));
  topBG->Add(histos.at(Tbar_t));
  topBG->Add(histos.at(Tbar_tW));

  topBG->SetFillColor(kRed-7);
  topBG->SetLineColor(kBlack);
  leg->AddEntry(topBG, "t#bar{t} + Single-Top", "f");
  topBG->Scale(scale);
  mystack->Add(topBG);

  vbBG->SetFillColor(kGreen-3);
  vbBG->SetLineColor(kBlack);
  leg->AddEntry(vbBG, "W#rightarrowl#nu + Z/#gamma*#rightarrowl^{+}l^{-}", "f");
  vbBG->Scale(scale);
  mystack->Add(vbBG);

  std::cout<<"W+jets purity = "<<vbBG->Integral(vbBG->FindBin(250.),-1)/(vbBG->Integral(vbBG->FindBin(250.),-1)+vvbBG->Integral(vvbBG->FindBin(250.),-1)+topBG->Integral(topBG->FindBin(250.),-1))<<std::endl;
  std::cout<<"Top contamination = "<<topBG->Integral(topBG->FindBin(250.),-1)/(vbBG->Integral(vbBG->FindBin(250.),-1)+vvbBG->Integral(vvbBG->FindBin(250.),-1)+topBG->Integral(topBG->FindBin(250.),-1))<<std::endl;
  std::cout<<"VV contamination = "<<vvbBG->Integral(vbBG->FindBin(250.),-1)/(vbBG->Integral(vbBG->FindBin(250.),-1)+vvbBG->Integral(vvbBG->FindBin(250.),-1)+topBG->Integral(topBG->FindBin(250.),-1))<<std::endl;

  //std::cout<<"Checking top contamination:";
  for (int ibin = 1; ibin < topBG->GetNbinsX()+1; ibin++){
    //std::cout<<"\tBin "<<ibin<<"\t = "<<topBG->GetBinContent(ibin)/(topBG->GetBinContent(ibin)+vbBG->GetBinContent(ibin))<<std::endl;
  }

  //Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
  double hmax = mystack->GetMaximum();
  if (hmax < 0) hmax = 0;
     
  histos.at(Data)->SetMarkerStyle(20);
  histos.at(Data)->SetLineColor(kBlack);  

  TH1F* ratio = (TH1F*)histos.at(Data)->Clone("ratio");

  ratio->Divide(h_lumiBand);
      
  bool changedMax = false;
  if (histos.at(Data)->GetMaximum() > hmax){
    hmax = histos.at(Data)->GetMaximum() + sqrt(histos.at(Data)->GetMaximum());
    changedMax = true;
  }
    
  if (not changedMax) hmax = hmax + sqrt(hmax);
    
  //Make room for the other stuff on the plot
  if (yLog == 0) hmax = hmax * 1.4;
  else           hmax = hmax * 20;
    
//   if (inName == "LepInvM"){
//     if (yLog == 0) hmax = hmax * 1.1;
//     else           hmax = hmax * 10;
//   }  

  TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
  canv->cd();

  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->SetBottomMargin(0.46);
  pad1->Draw();
  pad1->cd();
  
  mystack->Draw("hist");
  mystack->GetXaxis()->SetRangeUser((rangeset1 ? minrange : mystack->GetXaxis()->GetXmin()),mystack->GetXaxis()->GetXmax());

  TAxis * yAxis = mystack->GetYaxis();
  //yAxis->SetTitle("Events / "+rebin+" GeV");
  yAxis->SetTitle("Events / bin");
  yAxis->SetTitleFont(42);
  yAxis->SetTitleSize(0.047);
  yAxis->SetTitleOffset(1.4);
  yAxis->SetLabelFont(42);
  yAxis->SetLabelSize(0.035);
  yAxis->SetLabelOffset(0.007);

  if (yLog == 0) mystack->SetMinimum(0);
  else           mystack->SetMinimum(0.1);
  
//   if (inPref != "SS_2L_ZQV_2J") hmax *= 10;

  mystack->SetMaximum(hmax);
  mystack->SetMinimum(0.1);

  //Format data histogram
  for (int ibin = 0; ibin < histos.at(Data)->GetNbinsX() + 1; ibin++){
    if (histos.at(Data)->GetBinContent(ibin) == 0) histos.at(Data)->SetBinContent(ibin, -10);
  }

  histos.at(Data)->Draw("P E X0 same");  

  gStyle->SetHatchesLineWidth(1);
  gStyle->SetErrorX(0.5);

  h_lumiBand->SetFillStyle(3344);
  h_lumiBand->SetFillColor(1);
  h_lumiBand->Draw("samee2");

  leg->Draw();

  //Vertical error bar on top of legend
  //TLine *vel = new TLine();
  //vel->SetLineColor(kBlack);
  //vel->DrawLineNDC(0.656, 0.889, 0.656, 0.913);

  //Mandatory text boxes
  TLatex* text1 = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS Preliminary   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.16);
  text1->SetY(1.0);
  text1->SetTextFont(62);
  text1->SetTextSizePixels(20);
  text1->Draw();
  
  mystack->GetXaxis()->SetLabelOffset(999);
  mystack->GetXaxis()->SetLabelSize(0);
  /*//copied from pull plot
  TAxis * xAxis = mystack->GetXaxis();
  xAxis->SetTitle(xTitle);
  xAxis->SetTitleFont(42);
  xAxis->SetTitleSize(0.047);
  xAxis->SetTitleOffset(1.0);
  xAxis->SetLabelFont(42);
  xAxis->SetLabelSize(0.035);
  xAxis->SetLabelOffset(0.007);*/

  // TString lumiStr = "578.5 pb^{-1} at #sqrt{s} = 13 TeV";

   /*TLatex* text2 = new TLatex(3.570061,23.08044, lumiStr);
   text2->SetNDC();
   text2->SetTextAlign(13);
   text2->SetX(0.184);
   text2->SetY(0.872);
 //   text2->SetY(0.9);
   text2->SetTextFont(42);
   text2->SetTextSizePixels(24);
   text2->Draw();*/

  //Larger description of channel
  TString schan = "";
  if (inSuff == "El") schan = "e";
  if (inSuff == "Mu") schan = "#mu";
  if (inSuff == "All")  schan = "e/#mu";

  char *charpref = (char*)(const char*)inPref;
  char *chkge1 = strstr(charpref, "GE1BTags");
  char *chkge2 = strstr(charpref, "GE2BTags");
  char *chkex0 = strstr(charpref, "Ex0BTags");
  char *chkex1 = strstr(charpref, "Ex1BTags");
  char *chkex2 = strstr(charpref, "Ex2BTags");

  TPaveText text3(0.20, 0.85, 0.44, 0.92,"NDC");
  text3.SetFillColor(kWhite);
  if (charpref == chkge1) text3.AddText(schan+"+jets N_{b tags}#geq1");
  else if (charpref == chkge2) text3.AddText(schan+"+jets N_{b tags}#geq2");
  else if (charpref == chkex0) text3.AddText(schan+"+jets N_{b tags}=0");
  else if (charpref == chkex1) text3.AddText(schan+"+jets N_{b tags}=1");
  else if (charpref == chkex2) text3.AddText(schan+"+jets N_{b tags}=2");
  else text3.AddText(schan+"+jets");
  text3.SetBorderSize(0);
  text3.Draw();

  gPad->SetLogy(yLog);


  TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.43);

  gStyle->SetOptFit(1111);

  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.41);
  pad2->Draw();
  pad2->cd();

  TH1F* ratio_plus = (TH1F*)ratio->Clone("ratio_plus");
  TH1F* ratio_minus = (TH1F*)ratio->Clone("ratio_minus");
  TH1F* ratio_alt = (TH1F*)ratio->Clone("ratio_alt");
  for (int ibin = 1; ibin < ratio->GetNbinsX()+1; ibin++){
    ratio_plus->SetBinContent(ibin,ratio_plus->GetBinContent(ibin)+ratio_plus->GetBinError(ibin));
    ratio_minus->SetBinContent(ibin,ratio_minus->GetBinContent(ibin)-ratio_minus->GetBinError(ibin));
  }

  //ratio->Draw("P E X0");

  //TF1 *fitfunc = new TF1("fitfunc","[0]+[1]*x",ratio->GetXaxis()->GetXmin(),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc = new TF1("fitfunc","[0]+[1]*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc_plus = new TF1("fitfunc_plus","[0]+[1]*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc_minus = new TF1("fitfunc_minus","[0]+[1]*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc_alt = new TF1("fitfunc_alt","[0]+[1]*x+[2]*x*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc_alt_plus = new TF1("fitfunc_alt_plus","[0]+[1]*x+[2]*x*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  TF1 *fitfunc_alt_minus = new TF1("fitfunc_alt_minus","[0]+[1]*x+[2]*x*x",(rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()));
  //TF1 *fitfunc_high_plus = new TF1("fitfunc_high_plus","[0]+[1]*x",(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()),ratio->GetXaxis()->GetXmax());
  //TF1 *fitfunc_high_minus = new TF1("fitfunc_high_minus","[0]+[1]*x",(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()),ratio->GetXaxis()->GetXmax());
  TF1 *fitfunc_high_plus = new TF1("fitfunc_high_plus","[0]",(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()),ratio->GetXaxis()->GetXmax());
  TF1 *fitfunc_high_minus = new TF1("fitfunc_high_minus","[0]",(rangeset2 ? maxrange : ratio->GetXaxis()->GetXmax()),ratio->GetXaxis()->GetXmax());
  fitfunc->SetLineColor(kBlue);
  fitfunc->SetLineWidth(2);
  fitfunc_plus->SetLineColor(kRed);
  fitfunc_plus->SetLineWidth(2);
  fitfunc_minus->SetLineColor(kRed);
  fitfunc_minus->SetLineWidth(2);
  fitfunc_high_plus->SetLineColor(kGreen);
  fitfunc_high_plus->SetLineWidth(2);
  fitfunc_high_minus->SetLineColor(kGreen);
  fitfunc_high_minus->SetLineWidth(2);
  fitfunc_alt->SetLineColor(kOrange);
  fitfunc_alt->SetLineWidth(2);
  fitfunc_alt_plus->SetLineColor(kGreen);
  fitfunc_alt_plus->SetLineWidth(2);
  fitfunc_alt_minus->SetLineColor(kGreen);
  fitfunc_alt_minus->SetLineWidth(2);
  //ratio->Fit(fitfunc,"R","P E X0");
  //ratio->GetFunction("fitfunc")->SetLineColor(kBlue);
  ratio->Fit(fitfunc_alt,"R","P E X0");
  std::cout<<"Nominal Chi2/NDOF = \t"<<fitfunc_alt->GetChisquare()<<" / "<<fitfunc_alt->GetNDF()<<std::endl;
  ratio->GetFunction("fitfunc_alt")->SetLineColor(kOrange);
  ratio_plus->Fit(fitfunc_plus,"R0");
  ratio_minus->Fit(fitfunc_minus,"R0");
  //ratio_plus->Fit(fitfunc_high_plus,"R0");
  //ratio_minus->Fit(fitfunc_high_minus,"R0");
  ratio_plus->Fit(fitfunc_alt_plus,"R0");
  std::cout<<"Plus Chi2/NDOF = \t"<<fitfunc_alt_plus->GetChisquare()<<" / "<<fitfunc_alt_plus->GetNDF()<<std::endl;
  ratio_minus->Fit(fitfunc_alt_minus,"R0");
  std::cout<<"Minus Chi2/NDOF = \t"<<fitfunc_alt_minus->GetChisquare()<<" / "<<fitfunc_alt_minus->GetNDF()<<std::endl;
   
  ratio->GetXaxis()->SetRangeUser((rangeset1 ? minrange : ratio->GetXaxis()->GetXmin()),ratio->GetXaxis()->GetXmax());
  //ratio_alt->Fit(fitfunc_alt,"R","sames");
  //ratio_alt->GetFunction("fitfunc_alt")->SetLineColor(kOrange);
  //fitfunc_plus->Draw("same");
  //fitfunc_minus->Draw("same");
  //fitfunc_alt->Draw("same");
  fitfunc_alt_plus->Draw("same");
  fitfunc_alt_minus->Draw("same");
  //fitfunc_high_plus->Draw("same");
  //fitfunc_high_minus->Draw("same");
  canv->Update();

  //TPaveStats *p1 = (TPaveStats*)ratio->FindObject("stats");
  //p1->SetY1NDC(0.05);
  //p1->SetY2NDC(0.45);
  //p1->SetX1NDC(0.05);
  //p1->SetX2NDC(0.35);
  //p1->SetTextColor(kBlue);
  TPaveStats *p2 = (TPaveStats*)ratio->FindObject("stats");
  //p2->SetY1NDC(0.05);
  //p2->SetY2NDC(0.45);

  p2->SetY1NDC(0.05);
  p2->SetY2NDC(0.45);
  p2->SetX1NDC(0.05);
  p2->SetX2NDC(0.35);
  p2->SetTextColor(kOrange);
  canv->Modified();

  ratio->SetMinimum(0.2);
  ratio->SetMaximum(1.2);

  TAxis * xAxis = ratio->GetXaxis();
  xAxis->SetTitle(xTitle);
  xAxis->SetTitleFont(42);
  xAxis->SetTitleSize(0.06);
  xAxis->SetTitleOffset(1.0);
  xAxis->SetLabelFont(42);
  xAxis->SetLabelSize(0.05);
  xAxis->SetLabelOffset(0.007);

  TAxis * yAxis2 = ratio->GetYaxis();
  yAxis2->SetNdivisions(3);
  yAxis2->SetTitle("Data/MC");
  yAxis2->SetTitleSize(0.05);
  yAxis2->SetTitleFont(42);
  yAxis2->SetTitleOffset(0.32);
  yAxis2->SetLabelFont(42);
  yAxis2->SetLabelSize(0.04);
  yAxis2->SetLabelOffset(0.007);
  yAxis2->CenterTitle();

  canv->Update();
  canv->Modified();

  gPad->SetGridy(1);

  TString outName = "h_"+inPref+"_"+inName;
  if (inSuff != "All") outName += "_"+inSuff+"_Fit";
  else outName += "_Fit";
  if (rangeset2) outName += "_"+(TString)(Form("%i",(int)(maxrange)));

  if (useQCD) {
    canv->SaveAs(inDir+outName+"_wQCD.pdf");
    canv->SaveAs(inDir+outName+"_wQCD.png");
    canv->SaveAs(inDir+outName+"_wQCD.C");
  }
  else {
    canv->SaveAs(inDir+outName+".pdf");
    canv->SaveAs(inDir+outName+".png");
    canv->SaveAs(inDir+outName+".C");
  }

}

#endif
