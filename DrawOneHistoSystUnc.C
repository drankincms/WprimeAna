#ifndef DrawOneHistoSystUnc_cxx
#define DrawOneHistoSystUnc_cxx

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

  const unsigned int NSYST = 16;
  //TString opts[NSYST] = {"","_pup","_pum","_mum","_mup","_tpp","_tqp","_tqm","_btp","_btm","_mtp","_mtm","_jrm","_jrp","_jsm","_jsp","_wjp","_wjm"};
  TString opts[NSYST] = {"","_pup","_pum","_mum","_mup","_tpp","_btp","_btm","_mtp","_mtm","_jrm","_jrp","_jsm","_jsp","_wjp","_wjm"};
    
  const unsigned int NCHAN    = 3;
  const unsigned int NSAMPLES = totalSamples;


  TString fileName = "outFile.root";
  TFile *files[NSYST];
  for (unsigned int isys = 0; isys < NSYST; isys++){
    fileName = "outFile"+opts[isys]+".root";
    files[isys] = TFile::Open(inDir+fileName);
  }

  TH1F* histArray[NSAMPLES][NCHAN][NSYST];
  vector <TH1F*> histos[NSYST];

  TH1F* emptyHisto = new TH1F ("emptyHisto", "emptyHisto", 1, -0.5, 0.5);


  for (unsigned int isys = 0; isys < NSYST; isys++){
    for (unsigned int isam = 0; isam < NSAMPLES; isam++){
      files[isys]->cd();
      if (isam == Data) {
        fileName = "outFile.root";
        if (isys>0) {
          histos[isys].push_back(emptyHisto);
          continue;
        }
      }
      else fileName = "outFile"+opts[isys]+".root";

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
	isam == TTbar_scaleup or
	isam == TTbar_scaledown or
	isam == T_s or
	isam == T_t or
	isam == Tbar_t or
	isam == T_tW or
	isam == Tbar_tW or
	isam == Wprime2000Right or
	isam == Wprime2500Right or
	isam == Wprime3000Right
        )) {
        if (isys==0 and (
          isam == WJets_Pt100to250_LF or
          isam == WJets_Pt250to400_LF or
          isam == WJets_Pt400to600_LF or
          isam == WJets_Pt600toInf_LF or 
          isam == WJets_Pt100to250_HF or
          isam == WJets_Pt250to400_HF or
          isam == WJets_Pt400to600_HF or
          isam == WJets_Pt600toInf_HF
          )) {}
        else {
          histos[isys].push_back(emptyHisto);
          continue;
        }
      }
      if (opts[isys]=="_tqm"){
        if (isam == TTbar or isam == TTbar_scaleup) {
          histos[isys].push_back(emptyHisto);
          continue;
        }
      }
      else if (opts[isys]=="_tqp"){
        if (isam == TTbar or isam == TTbar_scaledown) {
          histos[isys].push_back(emptyHisto);
          continue;
        }
      }
      else {
        if (isam == TTbar_scaleup or isam == TTbar_scaledown) {
          histos[isys].push_back(emptyHisto);
          continue;
        }
      }
      histArray[isam][0][isys] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/Mu/"+inName+"_"+inPref+"_"+allNames[isam]+"_Mu");
      histArray[isam][1][isys] = (TH1F*)gDirectory->Get(inDir+fileName+":/"+inPref+"/El/"+inName+"_"+inPref+"_"+allNames[isam]+"_El");
      //std::cout<<inDir+fileName+":/"+inPref+"/Mu/"+inName+"_"+inPref+"_"+allNames[isam]+"_Mu"<<std::endl;
      //std::cout<<"\t"<<histArray[isam][0][isys]->Integral()<<std::endl;
  
      if (rebin != 1){
        histArray[isam][0][isys]->Rebin(rebin);
        histArray[isam][1][isys]->Rebin(rebin);
      }
  
      histArray[isam][2][isys] = (TH1F*)histArray[isam][0][isys]->Clone("histArray_"+allNames[isam]+opts[isys]);
      histArray[isam][2][isys]->Add(histArray[isam][1][isys]);
      
      if      (inSuff == "Mu") histos[isys].push_back(histArray[isam][0][isys]);
      else if (inSuff == "El") histos[isys].push_back(histArray[isam][1][isys]);
      else if (inSuff == "All")  histos[isys].push_back(histArray[isam][2][isys]);
      else{
        cout<<"Invalid suffix"<<endl;
        return 1;
      }
    }
    cout << "Got " << opts[isys] << endl;
  }
  cout << "Got histos..." << endl;

  //Global post-processing of histos: get rid of negative values
  //and add overflow to final bin
  for (unsigned int isys = 0; isys < NSYST; isys++){
    for (unsigned int uh = 0; uh < histos[isys].size(); uh++){
      unsigned int nb = histos[isys].at(uh)->GetNbinsX();
      for (unsigned int ub = 1; ub < nb + 1; ub++){
        if (histos[isys].at(uh)->GetBinContent(ub) < 0){
	  histos[isys].at(uh)->SetBinContent(ub, 0.0);
	  histos[isys].at(uh)->SetBinError(ub, 0.0);
        }
      }
      double vOF = histos[isys].at(uh)->GetBinContent(nb + 1);
      double vLB = histos[isys].at(uh)->GetBinContent(nb);
      //std::cout << "Sys=" << opts[isys] << " , hist=" << uh << " , vLB = " << vLB << " , vOF = " << vOF << std::endl;

      double eOF = histos[isys].at(uh)->GetBinError(nb + 1);
      double eLB = histos[isys].at(uh)->GetBinError(nb);
      if (vOF > 0){
        histos[isys].at(uh)->SetBinContent(nb, vLB + vOF);
        histos[isys].at(uh)->SetBinError(nb, sqrt(eLB*eLB + eOF*eOF));
      }
    }
  }

  //Calculate systematics
  vector <TH1F*> selectedHistos[NSYST];
  for (unsigned int isys = 0; isys < NSYST; isys++){
    if (opts[isys]=="_tqm") selectedHistos[isys].push_back(histos[isys].at(TTbar_scaledown));
    else if (opts[isys]=="_tqp") selectedHistos[isys].push_back(histos[isys].at(TTbar_scaleup));
    else selectedHistos[isys].push_back(histos[isys].at(TTbar));
    selectedHistos[isys].push_back(histos[isys].at(WW));  
    selectedHistos[isys].push_back(histos[isys].at(WZ));  
    selectedHistos[isys].push_back(histos[isys].at(ZZ));  
    //selectedHistos[isys].push_back(histos[isys].at(WJets)); 
    selectedHistos[isys].push_back(histos[isys].at(ZJets)); 
    selectedHistos[isys].push_back(histos[isys].at(WJets_Pt100to250)); 
    selectedHistos[isys].push_back(histos[isys].at(WJets_Pt250to400)); 
    selectedHistos[isys].push_back(histos[isys].at(WJets_Pt400to600)); 
    selectedHistos[isys].push_back(histos[isys].at(WJets_Pt600toInf)); 
    /*selectedHistos[isys].push_back(histos[isys].at(ZJets_HT100to200)); 
    selectedHistos[isys].push_back(histos[isys].at(ZJets_HT200to400)); 
    selectedHistos[isys].push_back(histos[isys].at(ZJets_HT400to600)); 
    selectedHistos[isys].push_back(histos[isys].at(ZJets_HT600toInf)); */
    selectedHistos[isys].push_back(histos[isys].at(T_s)); 
    selectedHistos[isys].push_back(histos[isys].at(T_t)); 
    selectedHistos[isys].push_back(histos[isys].at(T_tW));  
    //selectedHistos[isys].push_back(histos[isys].at(Tbar_s));  
    selectedHistos[isys].push_back(histos[isys].at(Tbar_t));
    selectedHistos[isys].push_back(histos[isys].at(Tbar_tW));
  }

  double mcUnc = sqrt (0.02 * 0.02 + //Trigger
                       0.02 * 0.02 + //Lepton Efficiency
                       0.026 * 0.026); //Luminosity
  double topUnc = sqrt(0.08*0.08+mcUnc*mcUnc);
  double vbUnc = sqrt(0.10*0.10+mcUnc*mcUnc);
  //double topUnc = mcUnc;
  //double vbUnc = mcUnc;

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

  TH1F* h_SumAll = (TH1F*)histos[0].at(TTbar)->Clone("h_SumAll"); 
  for (unsigned int ih = 1; ih < selectedHistos[0].size(); ih++){
    h_SumAll->Add(selectedHistos[0].at(ih));
  }

  TH1F* h_lumiBand = (TH1F*) h_SumAll->Clone("h_lumiBand");
  for (int ibin = 1; ibin < h_lumiBand->GetNbinsX()+1; ibin++){

    double uncStat = 0;
    double uncSyst = 0;
    double uncTot  = 0;

    for (unsigned int ih = 0; ih < selectedHistos[0].size(); ih++){
      uncStat += selectedHistos[0].at(ih)->GetBinError(ibin)*selectedHistos[0].at(ih)->GetBinError(ibin);
      uncSyst += selectedHistos[0].at(ih)->GetBinContent(ibin)*selectedHistos[0].at(ih)->GetBinContent(ibin)*selectedUnc.at(ih)*selectedUnc.at(ih);    
    }
 
    uncTot = sqrt(uncStat + uncSyst);    
    h_lumiBand->SetBinError(ibin, uncTot);
  }
  

  (histos[0].at(Data))->SetBinErrorOption(TH1::kPoisson);
  TH1F* ratio = (TH1F*)histos[0].at(Data)->Clone("ratio");
  /*for (unsigned int ih = 0; ih < selectedHistos[0].size(); ih++){
    for (unsigned int isys = 1; isys < NSYST; isys++){
      for (int ibin = 1; ibin < h_lumiBand->GetNbinsX()+1; ibin++){
        std::cout<<opts[isys]<<" bin"<<ibin<<" histo"<<ih<<" diff = "<<selectedHistos[isys].at(ih)->GetBinContent(ibin)<<"-"<<selectedHistos[0].at(ih)->GetBinContent(ibin)<<"/"<<selectedHistos[0].at(ih)->GetBinContent(ibin)<<"="<<(selectedHistos[isys].at(ih)->GetBinContent(ibin)-selectedHistos[0].at(ih)->GetBinContent(ibin))/selectedHistos[0].at(ih)->GetBinContent(ibin)<<std::endl;
      }
    }
  }*/
  for (int ibin = 1; ibin < h_lumiBand->GetNbinsX()+1; ibin++){
    double dnErr = 0.;
    double upErr = 0.;
    for (unsigned int isys = 1; isys < NSYST; isys++){
      for (unsigned int ih = 0; ih < selectedHistos[0].size(); ih++){
        double tmpdiff = selectedHistos[isys].at(ih)->GetBinContent(ibin)-selectedHistos[0].at(ih)->GetBinContent(ibin);
        if (tmpdiff>0.) upErr = sqrt(tmpdiff*tmpdiff+upErr*upErr);
        else           dnErr = sqrt(tmpdiff*tmpdiff+dnErr*dnErr);
      }
    }
    upErr = sqrt(upErr*upErr + h_lumiBand->GetBinError(ibin)*h_lumiBand->GetBinError(ibin));
    dnErr = sqrt(dnErr*dnErr + h_lumiBand->GetBinError(ibin)*h_lumiBand->GetBinError(ibin));

    double ratuperr = sqrt(upErr*upErr+histos[0].at(Data)->GetBinError(ibin)*histos[0].at(Data)->GetBinError(ibin));
    double ratdnerr = sqrt(dnErr*dnErr+histos[0].at(Data)->GetBinError(ibin)*histos[0].at(Data)->GetBinError(ibin));
    if (histos[0].at(Data)->GetBinContent(ibin)==0) {
      ratuperr = sqrt(upErr*upErr+1.8*1.8);
      ratdnerr = sqrt(dnErr*dnErr+1.8*1.8);
    }
    
    if (histos[0].at(Data)->GetBinContent(ibin)>h_SumAll->GetBinContent(ibin)) ratio->SetBinContent(ibin, (histos[0].at(Data)->GetBinContent(ibin) - h_SumAll->GetBinContent(ibin)) / (ratuperr));
    else ratio->SetBinContent(ibin, (histos[0].at(Data)->GetBinContent(ibin) - h_SumAll->GetBinContent(ibin)) / (ratdnerr));

    h_lumiBand->SetBinContent(ibin, h_lumiBand->GetBinContent(ibin)+((upErr-dnErr)/2.));
    h_lumiBand->SetBinError(ibin, ((upErr+dnErr)/2.));
  }
  std::cout<<"KS Test Statistic = "<<histos[0].at(Data)->KolmogorovTest(h_SumAll)<<", Using lumi band = "<<histos[0].at(Data)->KolmogorovTest(h_lumiBand)<<std::endl;

      
  //Format data histogram
  histos[0].at(Data)->SetMarkerStyle(20);
  histos[0].at(Data)->SetLineColor(kBlack);  
  for (int ibin = 0; ibin < histos[0].at(Data)->GetNbinsX() + 1; ibin++){
    if (histos[0].at(Data)->GetBinContent(ibin) == 0) histos[0].at(Data)->SetBinContent(ibin, -10);
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

  leg->AddEntry(histos[0].at(Data), "Data", "ep");

  //Vector Boson BG
  //TH1F* vbBG = (TH1F*)histos.at(WJets)->Clone("vbBG");
  TH1F* vbBG = (TH1F*)histos[0].at(WJets_Pt600toInf_LF)->Clone("vbBG");
  vbBG->Add(histos[0].at(WJets_Pt100to250_LF));
  vbBG->Add(histos[0].at(WJets_Pt250to400_LF));
  vbBG->Add(histos[0].at(WJets_Pt400to600_LF));
  /*vbBG->Add(histos[0].at(ZJets_HT100to200));
  vbBG->Add(histos[0].at(ZJets_HT200to400));
  vbBG->Add(histos[0].at(ZJets_HT400to600));
  vbBG->Add(histos[0].at(ZJets_HT600toInf));*/
  vbBG->Add(histos[0].at(ZJets));
  vbBG->Add(histos[0].at(WW));
  vbBG->Add(histos[0].at(WZ));
  vbBG->Add(histos[0].at(ZZ));

  vbBG->SetFillColor(kGreen-3);
  vbBG->SetLineColor(kBlack);
  leg->AddEntry(vbBG, "W#rightarrowl#nu (LF) + Z/#gamma*#rightarrowl^{+}l^{-} + VV", "f");
  mystack->Add(vbBG);

  //Vector Boson BG
  TH1F* vbBGhf = (TH1F*)histos[0].at(WJets_Pt600toInf_HF)->Clone("vbBGhf");
  vbBGhf->Add(histos[0].at(WJets_Pt100to250_HF));
  vbBGhf->Add(histos[0].at(WJets_Pt250to400_HF));
  vbBGhf->Add(histos[0].at(WJets_Pt400to600_HF));

  vbBGhf->SetFillColor(kBlue);
  vbBGhf->SetLineColor(kBlack);
  leg->AddEntry(vbBGhf, "W#rightarrowl#nu (HF)", "f");
  mystack->Add(vbBGhf);

  //Top BG
  TH1F* topBG = (TH1F*)histos[0].at(TTbar)->Clone("topBG");
  topBG->Add(histos[0].at(T_s));
  topBG->Add(histos[0].at(T_t));
  topBG->Add(histos[0].at(T_tW));
  //topBG->Add(histos[0].at(Tbar_s));
  topBG->Add(histos[0].at(Tbar_t));
  topBG->Add(histos[0].at(Tbar_tW));

  topBG->SetFillColor(kRed-7);
  topBG->SetLineColor(kBlack);
  leg->AddEntry(topBG, "t#bar{t} + Single-Top", "f");
  mystack->Add(topBG);

  //Signal
  histos[0].at(Wprime2000Right)->Scale(20);
  histos[0].at(Wprime2000Right)->SetLineStyle(3);
  histos[0].at(Wprime2000Right)->SetLineWidth(2);
  histos[0].at(Wprime2000Right)->SetLineColor(kBlack);
  leg->AddEntry(histos[0].at(Wprime2000Right), "W'_{R}, m=2.0 TeV #times 20", "l");

  histos[0].at(Wprime2500Right)->Scale(20);
  histos[0].at(Wprime2500Right)->SetLineStyle(4);
  histos[0].at(Wprime2500Right)->SetLineWidth(2);
  histos[0].at(Wprime2500Right)->SetLineColor(kBlack);
  leg->AddEntry(histos[0].at(Wprime2500Right), "W'_{R}, m=2.5 TeV #times 20", "l");

  histos[0].at(Wprime3000Right)->Scale(20);
  histos[0].at(Wprime3000Right)->SetLineStyle(2);
  histos[0].at(Wprime3000Right)->SetLineWidth(2);
  histos[0].at(Wprime3000Right)->SetLineColor(kBlack);
  leg->AddEntry(histos[0].at(Wprime3000Right), "W'_{R}, m=3.0 TeV #times 20", "l");

  if (inName=="mWprime_Limits") {
    histos[0].at(Wprime2000Right)->Scale(0.11459);
    histos[0].at(Wprime2500Right)->Scale(0.033343);
    histos[0].at(Wprime3000Right)->Scale(0.010756);
  }
    
  //Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
  double hmax = mystack->GetMaximum();
  if (hmax < 0) hmax = 0;
     
  bool changedMax = false;
  if (histos[0].at(Data)->GetMaximum() > hmax){
    hmax = histos[0].at(Data)->GetMaximum() + sqrt(histos[0].at(Data)->GetMaximum());
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

//   for (int uitx = 1; uitx < histos[0].at(Data)->GetNbinsX() + 1; uitx++){
//     cout<<histos[0].at(Data)->GetBinContent(uitx)<<endl;
//   }
//   cout<<mystack->GetHistogram()->GetMaximum()<<'\t'<<mystack->GetHistogram()->GetMinimum()<<endl;

  histos[0].at(Data)->Draw("P E X0 same");  

  histos[0].at(Wprime2000Right) ->Draw("hist same");
  histos[0].at(Wprime2500Right) ->Draw("hist same");
  histos[0].at(Wprime3000Right) ->Draw("hist same");

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
  TLatex* textcms;
  textcms = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS}");
  textcms->SetNDC();
  textcms->SetTextAlign(13);
  textcms->SetX(0.16);
  textcms->SetY(0.9955);
  textcms->SetTextFont(62);
  textcms->SetTextSizePixels(20);
  textcms->Draw();

  TLatex* text1;
  text1 = new TLatex(1.570061,23.08044,"#scale[0.75]{#it{Preliminary}                           35.9 fb^{-1}  (13 TeV)}");
  text1->SetNDC();
  text1->SetTextAlign(13);
  text1->SetX(0.26);
  text1->SetY(1.0);
  text1->SetTextFont(42);
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

  if (strstr(charpref, "Soft") != NULL) schan = "Type A, "+schan;
  else if (strstr(charpref, "Hard") != NULL) schan = "Type B, "+schan;

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

  canv->SaveAs(inDir+outName+"_SystUnc.pdf");
  canv->SaveAs(inDir+outName+"_SystUnc.png");
  canv->SaveAs(inDir+outName+"_SystUnc.C");

}

#endif
