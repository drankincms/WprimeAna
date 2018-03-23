#ifndef DrawOneHistoOneSample_cxx
#define DrawOneHistoOneSample_cxx

#include <iostream>
#include <iomanip>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TGaxis.h>

#include "tdrstyle.C"

using namespace std;

int main( int argc, const char* argv[] ){

  if (argc < 8){
    cout<<"Need at least 7 arguments. Only "<<argc<<" found."<<endl;
    return 1;
  }
  
  setTDRStyle();

  //Root file information
  int normToOne  = atoi(argv[1]);

  TString inDir = argv[2];
  TString inSuff = argv[3];
  TString inPref = argv[4];
  TString inName = argv[5];

  //Drawing information
  int yLog = atoi(argv[6]);
  TString xTitle = argv[7];

  int rebin = 1;
  if (argc > 8) rebin = atoi(argv[8]);

  int mass = 2000;
  if (argc > 9) mass = atoi(argv[9]);

  cout<<inDir<<endl;
  cout<<inSuff<<endl;
  cout<<inPref<<endl;
  cout<<inName<<endl;
  cout<<yLog<<endl;
  cout<<xTitle<<endl;
  cout<<rebin<<endl;
  cout<<mass<<endl;

  const unsigned int NCHAN    = 3;
  const unsigned int NSAMPLES = 3;

  TString fileName = inDir+"outFile.root";
  TFile *file0 = TFile::Open(fileName);
  file0->cd();

  TH1F* histArray[NSAMPLES][NCHAN];
  vector <TH1F*> histos;
    
  TString mlab;

  std::vector<double> limscale;

  if (mass==1500) {
    mlab = "1500";
    limscale.push_back(0.46602);
    limscale.push_back(3.2763);
    limscale.push_back(2.8024);
  }
  else if (mass==2500) {
    mlab = "2500";
    limscale.push_back(0.033343);
    limscale.push_back(2.5873);
    limscale.push_back(2.5475);
  }
  else {
    mlab = "2000";
    limscale.push_back(0.11459);
    limscale.push_back(2.6888);
    limscale.push_back(2.5695);
  }

  TString ssname[NSAMPLES]  = {"Wprime"+mlab+"Right","Wprime"+mlab+"Mix","Wprime"+mlab+"Left"};

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){
    histArray[isam][0] = (TH1F*)gDirectory->Get(fileName+":/"+inPref+"/Mu/"+inName+"_"+inPref+"_"+ssname[isam]+"_"+"Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(fileName+":/"+inPref+"/El/"+inName+"_"+inPref+"_"+ssname[isam]+"_"+"El");
    
    if (rebin != 1){
      histArray[isam][0]->Rebin(rebin);
      histArray[isam][1]->Rebin(rebin);
    }

    if (inName=="mWprime_Limits") {
      histArray[isam][0]->Scale(limscale[isam]);
      histArray[isam][1]->Scale(limscale[isam]);
    }

    histArray[isam][2] = (TH1F*)histArray[isam][0]->Clone("histArray_"+ssname[isam]);
    histArray[isam][2]->Add(histArray[isam][1]);

    if      (inSuff == "Mu") histos.push_back(histArray[isam][0]);
    else if (inSuff == "El") histos.push_back(histArray[isam][1]);
    else if (inSuff == "All")  histos.push_back(histArray[isam][2]);
  }

  if (yLog == 0) histos.at(0)->SetMinimum(0);
  else           histos.at(0)->SetMinimum(0.1);
  histos.at(0)->GetXaxis()->SetTitle(xTitle);
  histos.at(0)->GetYaxis()->SetTitleOffset(1.5);
  histos.at(0)->GetYaxis()->SetTitle("Events");
  if (normToOne != 0)  histos.at(0)->GetYaxis()->SetTitle("Events normalized to 1");;

  histos.at(0)->SetLineColor(kBlack);
  histos.at(1)->SetLineColor(kBlue);
  histos.at(2)->SetLineColor(kRed);
  histos.at(0)->SetLineWidth(2);
  histos.at(1)->SetLineWidth(2);
  histos.at(2)->SetLineWidth(2);

  TLegend * leg = new TLegend(0.56, 0.77, 0.78, 0.94, "","brNDC");
  leg->SetFillColor(10);
  leg->SetTextSize(0.04);
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){
    if (normToOne != 0) histos.at(isam)->Scale(1.0 / histos.at(isam)->Integral());
    leg->AddEntry(histos.at(isam), ssname[isam], "lp");
  }

  for (unsigned int isam = 1; isam < NSAMPLES; isam++){
    if (histos.at(isam)->GetMaximum() > histos.at(0)->GetMaximum() + sqrt(histos.at(0)->GetMaximum())){
      if (yLog == 0) histos.at(0)->SetMaximum((histos.at(isam)->GetMaximum() + sqrt(histos.at(0)->GetMaximum())) * 1.05);
      else           histos.at(0)->SetMaximum((histos.at(isam)->GetMaximum() + sqrt(histos.at(0)->GetMaximum())) * 1.40);
    }
  }

  //Make room for the other stuff on the plot
  if (yLog == 0) histos.at(0)->SetMaximum(histos.at(0)->GetMaximum() * 1.5);
  else           histos.at(0)->SetMaximum(histos.at(0)->GetMaximum() * 10);

  // TGaxis::SetMaxDigits(3);
  
  TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
  canv->cd();

  histos.at(0)->Draw("hist e");
  histos.at(1)->Draw("hist e same");
  histos.at(2)->Draw("hist e same");

  leg->Draw();

  gPad->SetLogy(yLog);

  TString outName = inDir+"h_"+inPref+"_"+inName+"_"+mlab;
  if (inSuff != "All") outName += "_"+inSuff;

  canv->SaveAs(outName+"_Sig.png");
//   canv->SaveAs(outName+"_Sig.pdf");

}

#endif
