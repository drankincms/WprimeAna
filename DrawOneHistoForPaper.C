#ifndef DrawOneHistoForPaper_cxx
#define DrawOneHistoForPaper_cxx

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

  cout<<inDir<<endl;
  cout<<inSuff<<endl;
  cout<<inPref<<endl;
  cout<<inName<<endl;
  cout<<yLog<<endl;
  cout<<xTitle<<endl;
  cout<<rebin<<endl;
    
  const unsigned int NCHAN    = 4;
  const unsigned int NSAMPLES = totalSamples;

  TString fileName = "outFile.root";
  TFile *file0 = TFile::Open(fileName);
  file0->cd();

  TH1F* histArray[NSAMPLES][NCHAN];
  vector <TH1F*> histos;

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (isam == TTbar or isam == ZJets or isam == WJets){
      histos.push_back(emptyHisto);
      continue;
    }
      
    histArray[isam][0] = (TH1F*)gDirectory->Get(fileName+":/"+inPref+"/MuMu/"+inName+"_"+inPref+"_"+allNames[isam]+"_"+"MuMu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(fileName+":/"+inPref+"/ElMu/"+inName+"_"+inPref+"_"+allNames[isam]+"_"+"ElMu");
    histArray[isam][2] = (TH1F*)gDirectory->Get(fileName+":/"+inPref+"/ElEl/"+inName+"_"+inPref+"_"+allNames[isam]+"_"+"ElEl");

    if (rebin != 1){
      histArray[isam][0]->Rebin(rebin);
      histArray[isam][1]->Rebin(rebin);
      histArray[isam][2]->Rebin(rebin);
    }
    
    histArray[isam][3] = (TH1F*)histArray[isam][0]->Clone("histArray_"+allNames[isam]);
    histArray[isam][3]->Add(histArray[isam][1]);
    histArray[isam][3]->Add(histArray[isam][2]);
    
    if      (inSuff == "MuMu") histos.push_back(histArray[isam][0]);
    else if (inSuff == "ElMu") histos.push_back(histArray[isam][1]);
    else if (inSuff == "ElEl") histos.push_back(histArray[isam][2]);
    else if (inSuff == "All")  histos.push_back(histArray[isam][3]);
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
  selectedHistos.push_back(histos.at(ChargeMisID));
  selectedHistos.push_back(histos.at(WZ));                                      
  selectedHistos.push_back(histos.at(ZZ));                                      
  selectedHistos.push_back(histos.at(WWp));                                     
  selectedHistos.push_back(histos.at(WWm));                                     
  selectedHistos.push_back(histos.at(WWW));                                     
  selectedHistos.push_back(histos.at(TTW));                                     
  selectedHistos.push_back(histos.at(TTWW));                                      
  selectedHistos.push_back(histos.at(TTZ));                                      
  selectedHistos.push_back(histos.at(FakeRate));                              

  vector <double> selectedUnc;
  selectedUnc.push_back(0.2); //Charge MisID
  selectedUnc.push_back(0.17); //WZ                                     
  selectedUnc.push_back(0.1); //ZZ                                      
  selectedUnc.push_back(0.5); //WWp                                     
  selectedUnc.push_back(0.5);  //WWm                                     
  selectedUnc.push_back(0.5); //WWW                                     
  selectedUnc.push_back(0.32); //TTW                                     
  selectedUnc.push_back(0.5);  //TTWW                   
  selectedUnc.push_back(0.5);  //TTZ                             
  selectedUnc.push_back(0.5);  //FakeRate   


  TH1F* h_SumAll = (TH1F*)histos.at(ChargeMisID)->Clone("h_SumAll"); 
  for (unsigned int ih = 1; ih < selectedHistos.size(); ih++){
    h_SumAll->Add(selectedHistos.at(ih));
  }
  
  TH1F* h_lumiBand = (TH1F*) h_SumAll->Clone("h_lumiBand");
  
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

  TH1F* ratio = (TH1F*)histos.at(Data)->Clone("ratio");
  ratio->Add(h_lumiBand, -1);
  for (int ibin = 1; ibin < ratio->GetNbinsX() + 1; ibin++){
     if (histos.at(Data)->GetBinContent(ibin) != 0) ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / ratio->GetBinError(ibin));
     else                                           ratio->SetBinContent(ibin, 0);
//     ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / ratio->GetBinError(ibin));
  }

      
  //Format data histogram
  histos.at(Data)->SetMarkerStyle(20);
  histos.at(Data)->SetLineColor(kBlack);  
  for (int ibin = 0; ibin < histos.at(Data)->GetNbinsX() + 1; ibin++){
    if (histos.at(Data)->GetBinContent(ibin) == 0) histos.at(Data)->SetBinContent(ibin, -10);
  }

  THStack* mystack = new THStack("mystack","mystack");

  TLegend * leg = new TLegend(0.67, 0.58, 0.89, 0.93, "","brNDC");
  leg->SetFillColor(10);
  leg->SetTextSize(0.035);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);

  leg->AddEntry(histos.at(Data), "Data", "ep");

  //ZZ
  histos.at(ZZ)->SetFillColor(kAzure-2);
  leg->AddEntry(histos.at(ZZ), "ZZ", "f");
  mystack->Add(histos.at(ZZ));

  //ssWW
  TH1F* ssWW = (TH1F*)histos.at(WWm)->Clone("ssWW");
  ssWW->Add(histos.at(WWp));

  ssWW->SetFillColor(kGreen-10);
  leg->AddEntry(ssWW, "W^{#pm}W^{#pm}", "f");
  mystack->Add(ssWW);

  //WWW
  histos.at(WWW)->SetFillColor(kGreen+2);
  leg->AddEntry(histos.at(WWW), "WWW", "f");
  mystack->Add(histos.at(WWW));

  //TTW
  histos.at(TTW)->SetFillColor(kRed+1);
  leg->AddEntry(histos.at(TTW), "t#bar{t}W", "f");
  mystack->Add(histos.at(TTW));

  //TTWW
  histos.at(TTWW)->SetFillColor(kOrange);
  leg->AddEntry(histos.at(TTWW), "t#bar{t}WW", "f");
  mystack->Add(histos.at(TTWW));

  //TTZ
  histos.at(TTZ)->SetFillColor(kOrange+1);
  leg->AddEntry(histos.at(TTZ), "t#bar{t}Z", "f");
  mystack->Add(histos.at(TTZ));

  //WZ
  histos.at(WZ)->SetFillColor(kViolet+1);
  leg->AddEntry(histos.at(WZ), "WZ", "f");
  mystack->Add(histos.at(WZ));

  //Charge MisID
  if (inSuff != "MuMu"){
    histos.at(ChargeMisID)->SetFillColor(kGray+2);
    leg->AddEntry(histos.at(ChargeMisID), "Charge misid.", "f");
    mystack->Add(histos.at(ChargeMisID));
  }

  //Fakes
  histos.at(FakeRate)->SetFillColor(kGray);
  histos.at(FakeRate)->SetLineColor(kBlack);
  leg->AddEntry(histos.at(FakeRate), "Non-prompt", "f");
  mystack->Add(histos.at(FakeRate));

  //Signal
  unsigned int iSig = T53m800;

  histos.at(iSig)->SetLineStyle(2);
  histos.at(iSig)->SetLineWidth(4);
  histos.at(iSig)->SetLineColor(kBlack);
  leg->AddEntry(histos.at(iSig), "T_{5/3} (0.8 TeV)", "l");

  //Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
  double hmax = mystack->GetMaximum();
  if (hmax < 0) hmax = 0;
     
  bool changedMax = false;
  if (histos.at(Data)->GetMaximum() > hmax){
    hmax = histos.at(Data)->GetMaximum() + sqrt(histos.at(Data)->GetMaximum());
    changedMax = true;
  }
    
  if (histos.at(iSig)->GetMaximum() > hmax){
    hmax = histos.at(iSig)->GetMaximum() + sqrt(histos.at(iSig)->GetMaximum());
    changedMax = true;
  }

  if (not changedMax) hmax = hmax + sqrt(hmax);
    
  //Make room for the other stuff on the plot
//   if (yLog == 0) hmax = hmax * 1.5;
//   else           hmax = hmax * 5;
    
  if (inName == "LepInvM"){
    if (yLog == 0) hmax = hmax * 1.1;
    else           hmax = hmax * 10;
  }  

  TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
  canv->cd();
  // canv->SetGrayscale();

  TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
  pad1->SetBottomMargin(0.25);
  pad1->Draw();
  pad1->cd();
  
  mystack->Draw("hist");

  TAxis * yAxis = mystack->GetYaxis();
  if (inPref == "SS_2L_ZQV_2J") yAxis->SetTitle("Events / 125 GeV");
  else                          yAxis->SetTitle("Events / 200 GeV");
  yAxis->SetTitleFont(42);
  yAxis->SetTitleSize(0.047);
  yAxis->SetTitleOffset(1.4);
  yAxis->SetLabelFont(42);
  yAxis->SetLabelSize(0.035);
  yAxis->SetLabelOffset(0.007);

  if (yLog == 0) mystack->SetMinimum(0);
  else           mystack->SetMinimum(0.1);
  
  if (inPref != "SS_2L_ZQV_2J"){
    hmax *= 10;
    if (yLog == 0) hmax /= 8;
  }

  mystack->SetMaximum(hmax);
  mystack->SetMinimum(0.1);

//   for (int uitx = 1; uitx < histos.at(Data)->GetNbinsX() + 1; uitx++){
//     cout<<histos.at(Data)->GetBinContent(uitx)<<endl;
//   }
//   cout<<mystack->GetHistogram()->GetMaximum()<<'\t'<<mystack->GetHistogram()->GetMinimum()<<endl;

  histos.at(Data)->Draw("P E X0 same");  

  histos.at(iSig)->Draw("hist same");
  
  gStyle->SetHatchesLineWidth(4);
  gStyle->SetErrorX(0.5);

  h_lumiBand->SetFillStyle(3005);
  h_lumiBand->SetFillColor(1);
  h_lumiBand->Draw("samee2");

  leg->Draw();

  //Vertical error bar on top of legend
  TLine *vel = new TLine();
  vel->SetLineColor(kBlack);
  vel->DrawLineNDC(0.6975, 0.902, 0.6975, 0.926);

  //Mandatory text boxes
  TString lumiStr = "";
  TLatex* text1 = new TLatex(3.570061,23.08044,"CMS      L = 19.5 fb^{-1}      #sqrt{s} = 8 TeV");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.21);
  text1->SetY(1.0);
  text1->SetTextFont(42);
  text1->SetTextSizePixels(24);
  text1->Draw();

  // if (inSuff == "ElEl") lumiStr = "4.9 fb^{-1} at #sqrt{s} = 8 TeV";

//   TLatex* text2 = new TLatex(3.570061,23.08044, lumiStr);
//   text2->SetNDC();
//   text2->SetTextAlign(13);
//   text2->SetX(0.184);
//   text2->SetY(0.872);
// //   text2->SetY(0.9);
//   text2->SetTextFont(42);
//   text2->SetTextSizePixels(24);
//   text2->Draw();

  //Larger description of channel
  TString schan = "";
  if (inSuff == "ElEl") schan = "ee";
  if (inSuff == "ElMu") schan = "e#mu";
  if (inSuff == "MuMu") schan = "#mu#mu";
  if (inSuff == "All")  schan = "ee+e#mu+#mu#mu";

  TPaveText text3(0.40, 0.73, 0.64, 0.80,"NDC");
  text3.SetFillColor(kWhite);
  text3.AddText(schan);
  text3.SetBorderSize(0);
//   text3.Draw();

  gPad->SetLogy(yLog);

  TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.25);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.41);
  pad2->Draw();
  pad2->cd();
  ratio->Draw("hist");

  ratio->SetMinimum(-2.5);
  ratio->SetMaximum(2.5);

  TAxis * xAxis = ratio->GetXaxis();
  xAxis->SetTitle(xTitle);
  xAxis->SetTitleFont(42);
  xAxis->SetTitleSize(0.185);
  xAxis->SetTitleOffset(1.0);
  xAxis->SetLabelFont(42);
  xAxis->SetLabelSize(0.15);
  xAxis->SetLabelOffset(0.007);

  TAxis * yAxis2 = ratio->GetYaxis();
  yAxis2->SetNdivisions(5);
  yAxis2->SetTitle("Pull");
  yAxis2->SetTitleSize(0.2);
  yAxis2->SetTitleFont(42);
  yAxis2->SetTitleOffset(0.32);
  yAxis2->SetLabelFont(42);
  yAxis2->SetLabelSize(0.15);
  yAxis2->SetLabelOffset(0.007);
  yAxis2->CenterTitle();

  ratio->SetLineColor(kRed);
  ratio->SetFillColor(kRed);

  TString outName = "h_"+inPref+"_"+inName;
  if (inSuff != "All") outName += "_"+inSuff;  

  canv->SaveAs(outName+".pdf");
  canv->SaveAs(outName+".png");
  canv->SaveAs(outName+".C");

}

#endif
