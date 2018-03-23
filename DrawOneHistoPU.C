#ifndef DrawOneHistoThreeSig_cxx
#define DrawOneHistoThreeSig_cxx

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
  
  int qcdin = 0;
  if (argc > 8) qcdin = atoi(argv[8]);
  bool useQCD = false;
  if (qcdin>0) useQCD = true;

  double scale = 1.0;
  if (argc > 9) scale = atof(argv[9]);

  TString fileSuff = "";
  if (argc > 10) fileSuff = argv[10];

  int label = 0;
  if (argc > 11) label = atoi(argv[11]);

  cout<<inDir<<endl;
  cout<<inSuff<<endl;
  cout<<inPref<<endl;
  cout<<inName<<endl;
  cout<<yLog<<endl;
  cout<<xTitle<<endl;
  cout<<rebin<<endl;
  cout<<fileSuff<<endl;
    
  const unsigned int NCHAN    = 3;
  const unsigned int NSAMPLES = totalSamples;

  TString fileName0 = "outFile.root";
  TString fileName1 = "outFile"+fileSuff+".root";
  TFile *file0 = TFile::Open(inDir+fileName0);
  TFile *file1 = TFile::Open(inDir+fileName1);

  TH1F* histArray[NSAMPLES][NCHAN];
  vector <TH1F*> histos;

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);

  for (unsigned int isam = 0; isam < NSAMPLES; isam++){

    if (allNames[isam]=="Data") file0->cd();
    else file1->cd();

    TString fileName = fileName1;
    if (allNames[isam]=="Data") fileName = fileName0;

    if (!useQCD and not (
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
      isam == Wprime2000Right or
      isam == Wprime2500Right or 
      isam == Wprime3000Right
    )) {
      histos.push_back(emptyHisto);
      continue;
    }
    if (useQCD and not (
      isam == Data or
      isam == ZJets or
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
      isam ==  QCD_Pt_120to170 or
      isam ==  QCD_Pt_170to300 or
      isam ==  QCD_Pt_300to470 or
      isam ==  QCD_Pt_470to600 or
      isam ==  QCD_Pt_600to800 or
      isam ==  QCD_Pt_800to1000 or
      isam ==  QCD_Pt_1000to1400 or
      isam == Wprime2000Right or
      isam == Wprime2500Right or 
      isam == Wprime3000Right
    )) {
      histos.push_back(emptyHisto);
      continue;
    }
    histArray[isam][0] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/Mu/"+inName+"_"+inPref+"_"+allNames[isam]+"_Mu");
    histArray[isam][1] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/El/"+inName+"_"+inPref+"_"+allNames[isam]+"_El");

    if (rebin != 1){
      histArray[isam][0]->Rebin(rebin);
      histArray[isam][1]->Rebin(rebin);
    }

    histArray[isam][2] = (TH1F*)histArray[isam][0]->Clone("histArray_"+allNames[isam]);
    histArray[isam][2]->Add(histArray[isam][1]);
    
    if      (inSuff == "Mu") histos.push_back(histArray[isam][0]);
    else if (inSuff == "El") histos.push_back(histArray[isam][1]);
    else if (inSuff == "All")  histos.push_back(histArray[isam][2]);
    else{
      cout<<"Invalid suffix"<<endl;
      return 1;
    }
  }
  cout << "Got histos..." << endl;

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
  //selectedHistos.push_back(histos.at(WW));  
  //selectedHistos.push_back(histos.at(WZ));  
  //selectedHistos.push_back(histos.at(ZZ));  
  //selectedHistos.push_back(histos.at(WJets)); 
  selectedHistos.push_back(histos.at(ZJets)); 
  selectedHistos.push_back(histos.at(WJets_Pt100to250)); 
  selectedHistos.push_back(histos.at(WJets_Pt250to400)); 
  selectedHistos.push_back(histos.at(WJets_Pt400to600)); 
  selectedHistos.push_back(histos.at(WJets_Pt600toInf)); 
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
  double qcdUnc = sqrt(0.50*0.50+mcUnc*mcUnc);
  double topUnc = sqrt(0.15*0.15+mcUnc*mcUnc);
  double vbUnc = sqrt(0.20*0.20+mcUnc*mcUnc);

  vector <double> selectedUnc;
  selectedUnc.push_back(topUnc); //TTbar
  //selectedUnc.push_back(vbUnc); //WW
  //selectedUnc.push_back(vbUnc); //WZ
  //selectedUnc.push_back(vbUnc); //ZZ
  //selectedUnc.push_back(vbUnc); //WJets
  selectedUnc.push_back(vbUnc); //ZJets
  selectedUnc.push_back(vbUnc); //WJets_Pt100to200
  selectedUnc.push_back(vbUnc); //WJets_Pt200to400
  selectedUnc.push_back(vbUnc); //WJets_Pt400to600
  selectedUnc.push_back(vbUnc); //WJets_Pt600to800
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

  //(histos.at(Data))->SetBinErrorOption(TH1::kPoisson);
  TH1F* ratio = (TH1F*)histos.at(Data)->Clone("ratio");
  ratio->Add(h_lumiBand, -1);
  for (int ibin = 1; ibin < ratio->GetNbinsX() + 1; ibin++){
    if (histos.at(Data)->GetBinContent(ibin) != 0) ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / ratio->GetBinError(ibin));
    else                                           ratio->SetBinContent(ibin, 0.);
    //ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / ratio->GetBinError(ibin));
  }

      
  //Format data histogram
  histos.at(Data)->SetMarkerStyle(20);
  histos.at(Data)->SetLineColor(kBlack);  
  for (int ibin = 0; ibin < histos.at(Data)->GetNbinsX() + 1; ibin++){
    if (histos.at(Data)->GetBinContent(ibin) == 0) histos.at(Data)->SetBinContent(ibin, -10);
  }

  THStack* mystack = new THStack("mystack","mystack");

  TLegend * leg = new TLegend(0.65, 0.7, 0.80, 0.92, "","brNDC");
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
    TH1F* qcdBG = (TH1F*)histos.at(QCD_Pt_120to170)->Clone("qcdBG");
    qcdBG->Add(histos.at(QCD_Pt_170to300));
    qcdBG->Add(histos.at(QCD_Pt_300to470));
    qcdBG->Add(histos.at(QCD_Pt_470to600));
    qcdBG->Add(histos.at(QCD_Pt_600to800));
    qcdBG->Add(histos.at(QCD_Pt_800to1000));
    qcdBG->Add(histos.at(QCD_Pt_1000to1400));

    qcdBG->SetFillColor(kYellow+1);
    qcdBG->SetLineColor(kBlack);
    leg->AddEntry(qcdBG, "QCD", "f");
    qcdBG->Scale(scale);
    mystack->Add(qcdBG);
  }

  //Vector Boson BG
  //TH1F* vbBG = (TH1F*)histos.at(WJets)->Clone("vbBG");
  TH1F* vbBG = (TH1F*)histos.at(WJets_Pt600toInf)->Clone("vbBG");
  vbBG->Add(histos.at(WJets_Pt100to250));
  vbBG->Add(histos.at(WJets_Pt250to400));
  vbBG->Add(histos.at(WJets_Pt400to600));
  /*vbBG->Add(histos.at(ZJets_HT100to200));
  vbBG->Add(histos.at(ZJets_HT200to400));
  vbBG->Add(histos.at(ZJets_HT400to600));
  vbBG->Add(histos.at(ZJets_HT600toInf));*/
  vbBG->Add(histos.at(ZJets));
  //vbBG->Add(histos.at(WW));
  //vbBG->Add(histos.at(WZ));
  //vbBG->Add(histos.at(ZZ));

  vbBG->SetFillColor(kGreen-3);
  vbBG->SetLineColor(kBlack);
  leg->AddEntry(vbBG, "W#rightarrowl#nu (LF) + Z/#gamma*#rightarrowl^{+}l^{-} + VV", "f");
  vbBG->Scale(scale);
  mystack->Add(vbBG);

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

  //Signal
  histos.at(Wprime2000Right)->Scale(20*scale);
  histos.at(Wprime2000Right)->SetLineStyle(3);
  histos.at(Wprime2000Right)->SetLineWidth(2);
  histos.at(Wprime2000Right)->SetLineColor(kBlack);
  leg->AddEntry(histos.at(Wprime2000Right), "W'_{R}, m=2.0 TeV #times 20", "l");

  histos.at(Wprime2500Right)->Scale(20*scale);
  histos.at(Wprime2500Right)->SetLineStyle(4);
  histos.at(Wprime2500Right)->SetLineWidth(2);
  histos.at(Wprime2500Right)->SetLineColor(kBlack);
  leg->AddEntry(histos.at(Wprime2500Right), "W'_{R}, m=2.5 TeV #times 20", "l");

  histos.at(Wprime3000Right)->Scale(20*scale);
  histos.at(Wprime3000Right)->SetLineStyle(2);
  histos.at(Wprime3000Right)->SetLineWidth(2);
  histos.at(Wprime3000Right)->SetLineColor(kBlack);
  leg->AddEntry(histos.at(Wprime3000Right), "W'_{R}, m=3.0 TeV #times 20", "l");

  if (inName=="mWprime_Limits") {
    histos.at(Wprime2000Right)->Scale(0.11459);
    histos.at(Wprime2500Right)->Scale(0.033343);
    histos.at(Wprime3000Right)->Scale(0.010756);
  }
    
  //Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
  double hmax = mystack->GetMaximum();
  if (hmax < 0) hmax = 0;
     
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
  pad1->SetBottomMargin(0.26);
  pad1->Draw();
  pad1->cd();
  
  mystack->Draw("hist");

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

//   for (int uitx = 1; uitx < histos.at(Data)->GetNbinsX() + 1; uitx++){
//     cout<<histos.at(Data)->GetBinContent(uitx)<<endl;
//   }
//   cout<<mystack->GetHistogram()->GetMaximum()<<'\t'<<mystack->GetHistogram()->GetMinimum()<<endl;

  histos.at(Data)->Draw("P E X0 same");  

  histos.at(Wprime2000Right) ->Draw("hist same");
  histos.at(Wprime2500Right) ->Draw("hist same");
  histos.at(Wprime3000Right) ->Draw("hist same");

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
  TLatex* text1;
  if (label==0) text1 = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS Preliminary   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}");
  else text1 = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS (Unpublished)   L = 35.9 fb^{-1}, #sqrt{s} = 13 TeV}");
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

  TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.23);
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

  ratio->SetLineColor(kBlack);
  ratio->SetFillColor(kRed);

  TString outName = "h_"+inPref+"_"+inName;
  if (inSuff != "All") outName += "_"+inSuff;  

  if (useQCD) {
    if (label==0) {
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD.pdf");
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD.png");
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD.C");
    }
    else {
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD_Unpub.pdf");
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD_Unpub.png");
      canv->SaveAs(inDir+outName+fileSuff+"_wQCD_Unpub.C");
    }
  }
  else {
    if (label==0) {
      canv->SaveAs(inDir+outName+fileSuff+".pdf");
      canv->SaveAs(inDir+outName+fileSuff+".png");
      canv->SaveAs(inDir+outName+fileSuff+".C");
    }
    else {
      canv->SaveAs(inDir+outName+fileSuff+"_Unpub.pdf");
      canv->SaveAs(inDir+outName+fileSuff+"_Unpub.png");
      canv->SaveAs(inDir+outName+fileSuff+"_Unpub.C");
    }
  }

}

#endif
