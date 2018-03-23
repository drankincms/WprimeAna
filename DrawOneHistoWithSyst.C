#ifndef DrawOneHistoWithSyst_cxx
#define DrawOneHistoWithSyst_cxx

#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>

#include "WprimeLoadData2016.C"
#include "tdrstyle.C"

using namespace std;

int main( int argc, const char* argv[] ){

  if (argc < 5){
    cout<<"Need at least 4 arguments. Only "<<argc<<" found."<<endl;
    return 1;
  }
  
  setTDRStyle();

  //Root file information
  TString inDir  = argv[1];
  TString inSuff = argv[2];
  TString inPref = argv[3];
  TString inSys  = argv[4];

  cout<<inDir<<" "<<inSuff<<" "<<inPref<<" "<<inSys<<endl;
    
  const unsigned int NCHAN    = 2;
  const unsigned int NSAMPLES = totalSamples;

  TString fileName = "outFile.root";
  TString fileName_UP = "outFile_"+inSys+"UP.root";
  TString fileName_DOWN = "outFile_"+inSys+"DOWN.root";
  TFile *file0 = TFile::Open(inDir+fileName);
  file0->cd();

  TH1F* histArray[NSAMPLES][NCHAN];
  vector <TH1F*> histos, histosUp, histosDown;

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (isam == Data or isam == QCD_HT200to300 or isam == QCD_HT300to500 or isam == QCD_HT500to700 or isam == QCD_HT700to1000 or isam == QCD_HT1000to1500 or isam == QCD_HT1500to2000 or isam == QCD_HT2000toInf or isam == QCD_Pt_5to10 or isam == QCD_Pt_10to15 or isam == QCD_Pt_15to30 or isam == QCD_Pt_30to50 or isam == QCD_Pt_50to80 or isam == QCD_Pt_80to120 or isam == QCD_Pt_120to170 or isam == QCD_Pt_170to300 or isam == QCD_Pt_300to470 or isam == QCD_Pt_470to600 or isam == QCD_Pt_600to800 or isam == QCD_Pt_800to1000 or isam == QCD_Pt_1000to1400 or isam == QCD_Pt_1400to1800 or isam == QCD_Pt_1800to2400 or isam == WJets or isam == ZJets_HT100to200 or isam == ZJets_HT200to400 or isam == ZJets_HT400to600 or isam == ZJets_HT600to800 or isam == ZJets_HT800to1200 or isam == ZJets_HT1200to2500 or isam == ZJets_HT2500toInf or isam == Tbar_s or isam == Tbar_t or isam == Wprime800Right or isam == Wprime900Right or isam == Wprime1000Right or isam == Wprime1100Right or isam == Wprime1200Right or isam == Wprime1300Right or isam == Wprime1400Right or isam == Wprime1500Right or isam == Wprime1600Right or isam == Wprime1700Right or isam == Wprime1800Right or isam == Wprime1900Right or isam == Wprime2000Right or isam == Wprime2100Right or isam == Wprime2200Right or isam == Wprime2300Right or isam == Wprime2400Right or isam == Wprime2500Right or isam == Wprime2600Right or isam == Wprime2700Right or isam == Wprime2800Right or isam == Wprime2900Right or isam == Wprime3000Right){
      histos.push_back(emptyHisto);
      continue;
    }
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/Mu/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/El/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_El");

    if      (inSuff == "Mu") histos.push_back(histArray[isam][0]);
    else if (inSuff == "El") histos.push_back(histArray[isam][1]);
    else{
      cout<<"Invalid suffix"<<endl;
      return 1;
    }
  }
  TFile *file0up = TFile::Open(inDir+fileName_UP);
  file0up->cd();
  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (isam == Data or isam == QCD_HT200to300 or isam == QCD_HT300to500 or isam == QCD_HT500to700 or isam == QCD_HT700to1000 or isam == QCD_HT1000to1500 or isam == QCD_HT1500to2000 or isam == QCD_HT2000toInf or isam == QCD_Pt_5to10 or isam == QCD_Pt_10to15 or isam == QCD_Pt_15to30 or isam == QCD_Pt_30to50 or isam == QCD_Pt_50to80 or isam == QCD_Pt_80to120 or isam == QCD_Pt_120to170 or isam == QCD_Pt_170to300 or isam == QCD_Pt_300to470 or isam == QCD_Pt_470to600 or isam == QCD_Pt_600to800 or isam == QCD_Pt_800to1000 or isam == QCD_Pt_1000to1400 or isam == QCD_Pt_1400to1800 or isam == QCD_Pt_1800to2400 or isam == WJets or isam == ZJets_HT100to200 or isam == ZJets_HT200to400 or isam == ZJets_HT400to600 or isam == ZJets_HT600to800 or isam == ZJets_HT800to1200 or isam == ZJets_HT1200to2500 or isam == ZJets_HT2500toInf or isam == Tbar_s or isam == Tbar_t or isam == Wprime800Right or isam == Wprime900Right or isam == Wprime1000Right or isam == Wprime1100Right or isam == Wprime1200Right or isam == Wprime1300Right or isam == Wprime1400Right or isam == Wprime1500Right or isam == Wprime1600Right or isam == Wprime1700Right or isam == Wprime1800Right or isam == Wprime1900Right or isam == Wprime2000Right or isam == Wprime2100Right or isam == Wprime2200Right or isam == Wprime2300Right or isam == Wprime2400Right or isam == Wprime2500Right or isam == Wprime2600Right or isam == Wprime2700Right or isam == Wprime2800Right or isam == Wprime2900Right or isam == Wprime3000Right){
      histosUp.push_back(emptyHisto);
      continue;
    }
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName_UP+":/"+inPref+"/Mu/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName_UP+":/"+inPref+"/El/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_El");

    if      (inSuff == "Mu") histosUp.push_back(histArray[isam][0]);
    else if (inSuff == "El") histosUp.push_back(histArray[isam][1]);
    else{
      cout<<"Invalid suffix"<<endl;
      return 1;
    }
  }
  TFile *file0down = TFile::Open(inDir+fileName_DOWN);
  file0down->cd();
  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (isam == Data or isam == QCD_HT200to300 or isam == QCD_HT300to500 or isam == QCD_HT500to700 or isam == QCD_HT700to1000 or isam == QCD_HT1000to1500 or isam == QCD_HT1500to2000 or isam == QCD_HT2000toInf or isam == QCD_Pt_5to10 or isam == QCD_Pt_10to15 or isam == QCD_Pt_15to30 or isam == QCD_Pt_30to50 or isam == QCD_Pt_50to80 or isam == QCD_Pt_80to120 or isam == QCD_Pt_120to170 or isam == QCD_Pt_170to300 or isam == QCD_Pt_300to470 or isam == QCD_Pt_470to600 or isam == QCD_Pt_600to800 or isam == QCD_Pt_800to1000 or isam == QCD_Pt_1000to1400 or isam == QCD_Pt_1400to1800 or isam == QCD_Pt_1800to2400 or isam == WJets or isam == ZJets_HT100to200 or isam == ZJets_HT200to400 or isam == ZJets_HT400to600 or isam == ZJets_HT600to800 or isam == ZJets_HT800to1200 or isam == ZJets_HT1200to2500 or isam == ZJets_HT2500toInf or isam == Tbar_s or isam == Tbar_t or isam == Wprime800Right or isam == Wprime900Right or isam == Wprime1000Right or isam == Wprime1100Right or isam == Wprime1200Right or isam == Wprime1300Right or isam == Wprime1400Right or isam == Wprime1500Right or isam == Wprime1600Right or isam == Wprime1700Right or isam == Wprime1800Right or isam == Wprime1900Right or isam == Wprime2000Right or isam == Wprime2100Right or isam == Wprime2200Right or isam == Wprime2300Right or isam == Wprime2400Right or isam == Wprime2500Right or isam == Wprime2600Right or isam == Wprime2700Right or isam == Wprime2800Right or isam == Wprime2900Right or isam == Wprime3000Right){
      histosDown.push_back(emptyHisto);
      continue;
    }
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName_DOWN+":/"+inPref+"/Mu/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName_DOWN+":/"+inPref+"/El/mWprime_Limits_"+inPref+"_"+allNames[isam]+"_El");

    if      (inSuff == "Mu") histosDown.push_back(histArray[isam][0]);
    else if (inSuff == "El") histosDown.push_back(histArray[isam][1]);
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


  //Global post-processing of histosUp: get rid of negative values
  //and add overflow to final bin
  for (unsigned int uh = 0; uh < histosUp.size(); uh++){
    unsigned int nb = histosUp.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosUp.at(uh)->GetBinContent(ub) < 0){
	histosUp.at(uh)->SetBinContent(ub, 0.0);
	histosUp.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosUp.at(uh)->GetBinContent(nb + 1);
    double vLB = histosUp.at(uh)->GetBinContent(nb);

    double eOF = histosUp.at(uh)->GetBinError(nb + 1);
    double eLB = histosUp.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosUp.at(uh)->SetBinContent(nb, vLB + vOF);
      histosUp.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  //Global post-processing of histosDown: get rid of negative values
  //and add overflow to final bin
  for (unsigned int uh = 0; uh < histosDown.size(); uh++){
    unsigned int nb = histosDown.at(uh)->GetNbinsX();
    for (unsigned int ub = 1; ub < nb + 1; ub++){
      if (histosDown.at(uh)->GetBinContent(ub) < 0){
	histosDown.at(uh)->SetBinContent(ub, 0.0);
	histosDown.at(uh)->SetBinError(ub, 0.0);
      }
    }
    double vOF = histosDown.at(uh)->GetBinContent(nb + 1);
    double vLB = histosDown.at(uh)->GetBinContent(nb);

    double eOF = histosDown.at(uh)->GetBinError(nb + 1);
    double eLB = histosDown.at(uh)->GetBinError(nb);
    if (vOF > 0){
      histosDown.at(uh)->SetBinContent(nb, vLB + vOF);
      histosDown.at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
    }
  }

  //Calculate systematics
  vector <TH1F*> selectedHistos;
  selectedHistos.push_back(histos.at(TTbar));
  selectedHistos.push_back(histos.at(WW));  
  selectedHistos.push_back(histos.at(WZ));  
  selectedHistos.push_back(histos.at(ZZ));  
  selectedHistos.push_back(histos.at(ZJets)); 
  selectedHistos.push_back(histos.at(WJets_HT100to200)); 
  selectedHistos.push_back(histos.at(WJets_HT200to400)); 
  selectedHistos.push_back(histos.at(WJets_HT400to600)); 
  selectedHistos.push_back(histos.at(WJets_HT600toInf)); 
  selectedHistos.push_back(histos.at(T_s)); 
  selectedHistos.push_back(histos.at(T_t)); 
  selectedHistos.push_back(histos.at(T_tW));  
  selectedHistos.push_back(histos.at(Tbar_tW));
  //Calculate systematics
  vector <TH1F*> selectedHistosUp;
  selectedHistosUp.push_back(histosUp.at(TTbar));
  selectedHistosUp.push_back(histosUp.at(WW));  
  selectedHistosUp.push_back(histosUp.at(WZ));  
  selectedHistosUp.push_back(histosUp.at(ZZ));  
  selectedHistosUp.push_back(histosUp.at(ZJets)); 
  selectedHistosUp.push_back(histosUp.at(WJets_HT100to200)); 
  selectedHistosUp.push_back(histosUp.at(WJets_HT200to400)); 
  selectedHistosUp.push_back(histosUp.at(WJets_HT400to600)); 
  selectedHistosUp.push_back(histosUp.at(WJets_HT600toInf)); 
  selectedHistosUp.push_back(histosUp.at(T_s)); 
  selectedHistosUp.push_back(histosUp.at(T_t)); 
  selectedHistosUp.push_back(histosUp.at(T_tW));  
  selectedHistosUp.push_back(histosUp.at(Tbar_tW));
  //Calculate systematics
  vector <TH1F*> selectedHistosDown;
  selectedHistosDown.push_back(histosDown.at(TTbar));
  selectedHistosDown.push_back(histosDown.at(WW));  
  selectedHistosDown.push_back(histosDown.at(WZ));  
  selectedHistosDown.push_back(histosDown.at(ZZ));  
  selectedHistosDown.push_back(histosDown.at(ZJets)); 
  selectedHistosDown.push_back(histosDown.at(WJets_HT100to200)); 
  selectedHistosDown.push_back(histosDown.at(WJets_HT200to400)); 
  selectedHistosDown.push_back(histosDown.at(WJets_HT400to600)); 
  selectedHistosDown.push_back(histosDown.at(WJets_HT600toInf)); 
  selectedHistosDown.push_back(histosDown.at(T_s)); 
  selectedHistosDown.push_back(histosDown.at(T_t)); 
  selectedHistosDown.push_back(histosDown.at(T_tW));  
  selectedHistosDown.push_back(histosDown.at(Tbar_tW));

  double mcUnc = sqrt (0.02 * 0.02 + //Trigger
                       0.02 * 0.02 + //Lepton Efficiency
                       0.12 * 0.12); //Luminosity
  double qcdUnc = sqrt(0.40*0.40+mcUnc*mcUnc);
  double topUnc = sqrt(0.15*0.15+mcUnc*mcUnc);
  double vbUnc = sqrt(0.20*0.20+mcUnc*mcUnc);

  vector <double> selectedUnc, selectedUncDown, selectedUncUp;
  selectedUnc.push_back(topUnc); //TTbar
  selectedUncUp.push_back(topUnc); //TTbar
  selectedUncDown.push_back(topUnc); //TTbar
  selectedUnc.push_back(vbUnc); //WW
  selectedUncUp.push_back(vbUnc); //WW
  selectedUncDown.push_back(vbUnc); //WW
  selectedUnc.push_back(vbUnc); //WZ
  selectedUncUp.push_back(vbUnc); //WZ
  selectedUncDown.push_back(vbUnc); //WZ
  selectedUnc.push_back(vbUnc); //ZZ
  selectedUncUp.push_back(vbUnc); //ZZ
  selectedUncDown.push_back(vbUnc); //ZZ
  selectedUnc.push_back(vbUnc); //ZJets
  selectedUncUp.push_back(vbUnc); //ZJets
  selectedUncDown.push_back(vbUnc); //ZJets
  selectedUnc.push_back(vbUnc); //WJets_HT100to200
  selectedUncUp.push_back(vbUnc); //WJets_HT100to200
  selectedUncDown.push_back(vbUnc); //WJets_HT100to200
  selectedUnc.push_back(vbUnc); //WJets_HT200to400
  selectedUncUp.push_back(vbUnc); //WJets_HT200to400
  selectedUncDown.push_back(vbUnc); //WJets_HT200to400
  selectedUnc.push_back(vbUnc); //WJets_HT400to600
  selectedUncUp.push_back(vbUnc); //WJets_HT400to600
  selectedUncDown.push_back(vbUnc); //WJets_HT400to600
  selectedUnc.push_back(vbUnc); //WJets_HT600toInf
  selectedUncUp.push_back(vbUnc); //WJets_HT600toInf
  selectedUncDown.push_back(vbUnc); //WJets_HT600toInf
  selectedUnc.push_back(topUnc); //T_s
  selectedUncUp.push_back(topUnc); //T_s
  selectedUncDown.push_back(topUnc); //T_s
  selectedUnc.push_back(topUnc); //T_t 
  selectedUncUp.push_back(topUnc); //T_t 
  selectedUncDown.push_back(topUnc); //T_t 
  selectedUnc.push_back(topUnc);  //T_tW 
  selectedUncUp.push_back(topUnc);  //T_tW 
  selectedUncDown.push_back(topUnc);  //T_tW 
  selectedUnc.push_back(topUnc);  //Tbar_tW
  selectedUncUp.push_back(topUnc);  //Tbar_tW
  selectedUncDown.push_back(topUnc);  //Tbar_tW

  TH1F* h_SumAll = (TH1F*)histos.at(TTbar)->Clone("h_SumAll"); 
  for (unsigned int ih = 1; ih < selectedHistos.size(); ih++){
    h_SumAll->Add(selectedHistos.at(ih));
  }
  
  TH1F* h_SumAllUp = (TH1F*)histosUp.at(TTbar)->Clone("h_SumAllUp"); 
  for (unsigned int ih = 1; ih < selectedHistosUp.size(); ih++){
    h_SumAllUp->Add(selectedHistosUp.at(ih));
  }
  
  TH1F* h_SumAllDown = (TH1F*)histosDown.at(TTbar)->Clone("h_SumAllDown"); 
  for (unsigned int ih = 1; ih < selectedHistosDown.size(); ih++){
    h_SumAllDown->Add(selectedHistosDown.at(ih));
  }

  for (int ibin = 1; ibin < h_SumAll->GetNbinsX()+1; ibin++){

    double uncStat = 0;
    double uncSyst = 0;
    double uncTot  = 0;

    for (unsigned int ih = 0; ih < selectedHistos.size(); ih++){
      uncStat += selectedHistos.at(ih)->GetBinError(ibin)*selectedHistos.at(ih)->GetBinError(ibin);
      uncSyst += selectedHistos.at(ih)->GetBinContent(ibin)*selectedHistos.at(ih)->GetBinContent(ibin)*selectedUnc.at(ih)*selectedUnc.at(ih);    
    }
 
    uncTot = sqrt(uncStat + uncSyst);    
    h_SumAll->SetBinError(ibin, uncTot);

    uncStat = 0;
    uncSyst = 0;
    uncTot  = 0;

    for (unsigned int ih = 0; ih < selectedHistosUp.size(); ih++){
      uncStat += selectedHistosUp.at(ih)->GetBinError(ibin)*selectedHistosUp.at(ih)->GetBinError(ibin);
      uncSyst += selectedHistosUp.at(ih)->GetBinContent(ibin)*selectedHistosUp.at(ih)->GetBinContent(ibin)*selectedUncUp.at(ih)*selectedUncUp.at(ih);    
    }
 
    uncTot = sqrt(uncStat + uncSyst);    
    h_SumAllUp->SetBinError(ibin, uncTot);

    uncStat = 0;
    uncSyst = 0;
    uncTot  = 0;

    for (unsigned int ih = 0; ih < selectedHistosDown.size(); ih++){
      uncStat += selectedHistosDown.at(ih)->GetBinError(ibin)*selectedHistosDown.at(ih)->GetBinError(ibin);
      uncSyst += selectedHistosDown.at(ih)->GetBinContent(ibin)*selectedHistosDown.at(ih)->GetBinContent(ibin)*selectedUncDown.at(ih)*selectedUncDown.at(ih);    
    }
 
    uncTot = sqrt(uncStat + uncSyst);    
    h_SumAllDown->SetBinError(ibin, uncTot);
  }

  TLegend * leg = new TLegend(0.7, 0.7, 0.88, 0.92, "","brNDC");
//   TLegend * leg = new TLegend(0.67, 0.58, 0.89, 0.93, "","brNDC");
  leg->SetFillColor(10);
  leg->SetTextSize(0.02);
  leg->SetTextFont(62);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);

  leg->AddEntry(h_SumAll, "Nominal", "l");
  leg->AddEntry(h_SumAllUp, inSys+" Up", "l");
  leg->AddEntry(h_SumAllDown, inSys+" Down", "l");

  //Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
  double hmax = h_SumAll->GetMaximum();
  if (hmax < 0) hmax = 0;
     
  TH1F* ratioUp = (TH1F*)h_SumAllUp->Clone("ratioUp");
  ratioUp->Add(h_SumAll,-1.);
  ratioUp->Divide(h_SumAll);
      
  TH1F* ratioDown = (TH1F*)h_SumAllDown->Clone("ratioDown");
  ratioDown->Add(h_SumAll,-1.);
  ratioDown->Divide(h_SumAll);
      
  hmax = hmax + sqrt(hmax);
    
  //Make room for the other stuff on the plot
  hmax = hmax * 20;
    
  TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
  canv->cd();

  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->SetBottomMargin(0.26);
  pad1->Draw();
  pad1->cd();
  
  gStyle->SetErrorX(0.5);
  h_SumAll->SetLineColor(kBlack);
  h_SumAll->SetLineWidth(2);
  h_SumAll->Draw();

  TAxis * yAxis = h_SumAll->GetYaxis();
  //yAxis->SetTitle("Events / "+rebin+" GeV");
  yAxis->SetTitle("Events / bin");
  yAxis->SetTitleFont(42);
  yAxis->SetTitleSize(0.047);
  yAxis->SetTitleOffset(1.4);
  yAxis->SetLabelFont(42);
  yAxis->SetLabelSize(0.035);
  yAxis->SetLabelOffset(0.007);

  h_SumAllUp->SetLineWidth(2);
  h_SumAllDown->SetLineWidth(2);
  h_SumAllUp->SetLineColor(kRed);
  h_SumAllDown->SetLineColor(kBlue);

  h_SumAllUp->Draw("esame");
  h_SumAllDown->Draw("esame");
  
  h_SumAll->SetMaximum(hmax);
  h_SumAll->SetMinimum(0.1);


  leg->Draw();

  //Vertical error bar on top of legend
  //TLine *vel = new TLine();
  //vel->SetLineColor(kBlack);
  //vel->DrawLineNDC(0.656, 0.889, 0.656, 0.913);

  //Mandatory text boxes
  TLatex* text1 = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS Preliminary   L = 1.26 fb^{-1}, #sqrt{s} = 13 TeV}");
  //TLatex* text1 = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS Preliminary   L = 578.5 fb^{-1}, #sqrt{s} = 13 TeV}");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.16);
  text1->SetY(1.0);
  text1->SetTextFont(62);
  text1->SetTextSizePixels(20);
  text1->Draw();
  
  h_SumAll->GetXaxis()->SetLabelOffset(999);
  h_SumAll->GetXaxis()->SetLabelSize(0);
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

  gPad->SetLogy(1);

  TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.23);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.41);
  pad2->Draw();
  pad2->cd();

  ratioUp->SetLineWidth(2);
  ratioDown->SetLineWidth(2);
  ratioUp->SetLineColor(kRed);
  ratioDown->SetLineColor(kBlue);

  gStyle->SetErrorX(0.5);
  ratioUp->Draw("");
  ratioDown->Draw("same");

  ratioUp->SetMinimum(-0.2);
  ratioUp->SetMaximum(0.2);

  TAxis * xAxis = ratioUp->GetXaxis();
  xAxis->SetTitle("M(tb) [GeV]");
  xAxis->SetTitleFont(42);
  xAxis->SetTitleSize(0.185);
  xAxis->SetTitleOffset(1.0);
  xAxis->SetLabelFont(42);
  xAxis->SetLabelSize(0.15);
  xAxis->SetLabelOffset(0.007);

  TAxis * yAxis2 = ratioUp->GetYaxis();
  yAxis2->SetNdivisions(4);
  yAxis2->SetTitle("#frac{sys-nom}{nom}");
  yAxis2->SetTitleSize(0.15);
  yAxis2->SetTitleFont(42);
  yAxis2->SetTitleOffset(0.32);
  yAxis2->SetLabelFont(42);
  yAxis2->SetLabelSize(0.1);
  yAxis2->SetLabelOffset(0.007);
  yAxis2->CenterTitle();

  gPad->SetGridy(1);

  TString outName = "h_"+inPref+"_"+inSys+"_"+inSuff;

  canv->SaveAs(inDir+outName+".pdf");
  canv->SaveAs(inDir+outName+".png");
  canv->SaveAs(inDir+outName+".C");

}

#endif
