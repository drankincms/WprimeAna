#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TObject.h"
#include "THStack.h"
#include "TString.h"
#include<string.h>
#include<vector>
#include<algorithm>
#include "tdrstyle.C"
using std::string;
using namespace std;

void makePlot(TString dir, TString bcat, TString chan, TString evty, TString fileb = "", TString filec = "", TString filee = "")
{

        setTDRStyle();

	if (fileb=="") fileb = bcat;
	if (filec=="") filec = chan;
	if (filee=="") filee = evty;

	TFile *prefile  = new TFile(dir+"/comb_mWprime_Limits_WprimeRight_HardSoft_Histos-ex1or2btags.root");
	TFile *postfile = new TFile(dir+"/histos_"+filec+"_"+fileb+"_"+filee+"-mle.root");
	TH1F *prew = new TH1F("prew","prew",1,0.,1.);
	TH1F *pret = new TH1F("pret","pret",1,0.,1.);
	TH1F *pred = new TH1F("pred","pred",1,0.,1.);
	TH1F *postw = new TH1F("postw","postw",1,0.,1.);
	TH1F *postt = new TH1F("postt","postt",1,0.,1.);

	TH1F *uperr = new TH1F("uperr","uperr",1,0.,1.);
	TH1F *downerr = new TH1F("downerr","downerr",1,0.,1.);

	prew = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__wjets");
	pret = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__ttbar");
	pred = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__DATA");
	postw = (TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__wjets");
	postt = (TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__ttbar");

	sig1 = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__wp2000");
	sig2 = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__wp2500");
	sig3 = (TH1F*)prefile->Get(chan+"_"+evty+"_"+bcat+"__wp3000");

	uperr = (TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__wjets_UP");
	downerr = (TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__wjets_DOWN");
	uperr->Add((TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__ttbar_UP"));
	downerr->Add((TH1F*)postfile->Get(chan+"_"+evty+"_"+bcat+"__ttbar_DOWN"));

	for (int i = 1; i <= pred->GetNbinsX(); i++) {
		prew->SetBinContent(i,postw->GetBinContent(i));
		pret->SetBinContent(i,postt->GetBinContent(i));
                uperr->SetBinContent(i,TMath::Sqrt(uperr->GetBinContent(i)));
                downerr->SetBinContent(i,TMath::Sqrt(downerr->GetBinContent(i)));
                //std::cout<<"uperr = "<<uperr->GetBinContent(i)<<", ibin = "<<i<<std::endl;
                //std::cout<<"downerr = "<<downerr->GetBinContent(i)<<", ibin = "<<i<<std::endl;
	}
	

	THStack *mystack = new THStack("mystack","Post-Fit M(tb)");

	TH1F* h_SumAll = (TH1F*)pret->Clone("h_SumAll"); 
	h_SumAll->Add(prew);
	
	TH1F* h_lumiBand = (TH1F*) h_SumAll->Clone("h_lumiBand");
	TH1F* dummy = (TH1F*) h_lumiBand->Clone("dummy");

	for (int ibin = 1; ibin < h_lumiBand->GetNbinsX()+1; ibin++){

		h_lumiBand->SetBinContent(ibin, h_lumiBand->GetBinContent(ibin)+((uperr->GetBinContent(ibin)-downerr->GetBinContent(ibin))/2.));
		h_lumiBand->SetBinError(ibin, (uperr->GetBinContent(ibin)+downerr->GetBinContent(ibin))/2.);
		if (h_lumiBand->GetBinContent(ibin)<0.) {
			h_lumiBand->SetBinError(ibin, h_lumiBand->GetBinError(ibin)+h_lumiBand->GetBinContent(ibin));
			h_lumiBand->SetBinContent(ibin, 0.000001);
		}
		if (pred->GetBinContent(ibin)<dummy->GetBinContent(ibin)) dummy->SetBinError(ibin,downerr->GetBinContent(ibin));
		else dummy->SetBinError(ibin,uperr->GetBinContent(ibin));
                if (pred->GetBinContent(ibin)==0) pred->SetBinError(ibin,1.8);
	}

	(pred)->SetBinErrorOption(TH1::kPoisson);
	TH1F* ratio = (TH1F*)pred->Clone("ratio");
	ratio->Sumw2();
	dummy->Sumw2();
	ratio->Add(dummy, -1);
	for (int ibin = 1; ibin < ratio->GetNbinsX() + 1; ibin++){
		//if (pred->GetBinContent(ibin) != 0) {
		//	ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / (ratio->GetBinError(ibin)));
		//}
		//else                                           ratio->SetBinContent(ibin, 0.);
		ratio->SetBinContent(ibin, ratio->GetBinContent(ibin) / ratio->GetBinError(ibin));
	}

	    
	//Format data histogram
	pred->SetMarkerStyle(20);
	pred->SetLineColor(kBlack);  
	/*for (int ibin = 0; ibin < pred->GetNbinsX() + 1; ibin++){
		if (pred->GetBinContent(ibin) == 0) pred->SetBinContent(ibin, -10);
	}*/

	TLegend * leg = new TLegend(0.55, 0.6, 0.80, 0.92, "","brNDC");
//   TLegend * leg = new TLegend(0.67, 0.58, 0.89, 0.93, "","brNDC");
	leg->SetFillColor(10);
	leg->SetTextSize(0.03);
	leg->SetTextFont(62);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetLineStyle(0);

	leg->AddEntry(pred, "Data", "lep");

	prew->SetFillColor(kGreen-3);
	prew->SetLineColor(kBlack);
	leg->AddEntry(prew, "W#rightarrowl#nu + Z/#gamma*#rightarrowl^{+}l^{-} + VV", "f");
	mystack->Add(prew);

	//Top BG
	pret->SetFillColor(kRed-7);
	pret->SetLineColor(kBlack);
	leg->AddEntry(pret, "t#bar{t} + single t", "f");
	mystack->Add(pret);

	//Work around idiotic ROOT convention of setting the maximum of an empty histo to -100 and causing problems with sqrt
	double hmax = mystack->GetMaximum();
	if (hmax < 0) hmax = 0;
	   
	bool changedMax = false;
	if (pred->GetMaximum() > hmax){
		hmax = pred->GetMaximum() + sqrt(pred->GetMaximum());
		changedMax = true;
	}
	  
	if (not changedMax) hmax = hmax + sqrt(hmax);
	  
	//Make room for the other stuff on the plot
	hmax = hmax * 20;
	  
	TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
	canv->cd();

	TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 1.0, 1.0);
	pad1->SetBottomMargin(0.3);
	pad1->Draw();
	pad1->cd();
	
	mystack->SetMaximum(hmax*100.);
	mystack->SetMinimum(0.1);

	mystack->Draw("hist");

	leg->AddEntry(h_lumiBand, "background uncertainty","f");

        sig1->Scale(0.114590*1.);
        sig2->Scale(0.033343*1.);
        sig3->Scale(0.010756*1.);
        sig1->SetLineColor(kBlack);
        sig2->SetLineColor(kBlack);
        sig3->SetLineColor(kBlack);
        sig1->SetLineWidth(3);
        sig2->SetLineWidth(3);
        sig3->SetLineWidth(3);
        sig1->SetLineStyle(2);
        sig2->SetLineStyle(3);
        sig3->SetLineStyle(4);
        //leg->AddEntry(sig1, "W'_{R}, m = 2.0 TeV #times 5","l");
        //leg->AddEntry(sig2, "W'_{R}, m = 2.5 TeV #times 5","l");
        //leg->AddEntry(sig3, "W'_{R}, m = 3.0 TeV #times 5","l");
        leg->AddEntry(sig1, "W'_{R} at 2000 GeV","l");
        leg->AddEntry(sig2, "W'_{R} at 2500 GeV","l");
        leg->AddEntry(sig3, "W'_{R} at 3000 GeV","l");

        sig1->Draw("hist same");
        sig2->Draw("hist same");
        sig3->Draw("hist same");

	TAxis * yAxis = mystack->GetYaxis();
	//yAxis->SetTitle("Events / "+rebin+" GeV");
	yAxis->SetTitle("Events / bin");
	yAxis->SetTitleFont(42);
	yAxis->SetTitleSize(0.042);
	yAxis->SetTitleOffset(1.4);
	yAxis->SetLabelFont(42);
	yAxis->SetLabelSize(0.035);
	yAxis->SetLabelOffset(0.007);

//   if (inPref != "SS_2L_ZQV_2J") hmax *= 10;

//   for (int uitx = 1; uitx < pred->GetNbinsX() + 1; uitx++){
//     cout<<pred->GetBinContent(uitx)<<endl;
//   }
//   cout<<mystack->GetHistogram()->GetMaximum()<<'\t'<<mystack->GetHistogram()->GetMinimum()<<endl;

	//pred->Draw("P E0 X0 same");  
	pred->Draw("P E0 same");  

	gStyle->SetHatchesLineWidth(1);
	gStyle->SetErrorX(0.5);

	h_lumiBand->SetFillStyle(3344);
	h_lumiBand->SetFillColor(1);
        h_lumiBand->SetLineColor(kWhite);

	h_lumiBand->Draw("samee2");

	leg->Draw();

	//Vertical error bar on top of legend
	//TLine *vel = new TLine();
	//vel->SetLineColor(kBlack);
	//vel->DrawLineNDC(0.656, 0.889, 0.656, 0.913);

	//Mandatory text boxes
	TLatex* textcms;
	//textcms = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS}");
	textcms = new TLatex(1.570061,23.08044,"#scale[1.2]{CMS}");
	textcms->SetNDC();
	textcms->SetTextAlign(13);
	textcms->SetX(0.2);
	textcms->SetY(0.89);
	textcms->SetTextFont(62);
	textcms->SetTextSizePixels(20);
	textcms->Draw();

	//TLatex* text1 = new TLatex(1.570061,23.08044,"#scale[0.75]{#it{Preliminary}                           35.9 fb^{-1}  (13 TeV)}");
	TLatex* text1 = new TLatex(1.570061,23.08044,"#scale[0.75]{                                            35.9 fb^{-1}  (13 TeV)}");
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
	TString schan = "#mu";
	if (chan == "elec") schan = "e";

	char *charpref = (char*)(const char*)bcat;
	char *chkge1   = strstr(charpref, "ge1btags");
	char *chkex1   = strstr(charpref, "ex1btags");
	char *chkex2   = strstr(charpref, "ex2btags");

	TPaveText text3(0.20, 0.7, 0.4, 0.78,"NDC");
	text3.SetTextAlign(12);
	text3.SetTextFont(42);

	text3.AddText(schan+"+jets");

	text3.SetFillColor(kWhite);
	TString tchan = "";
	if (evty == "hard") tchan = "Type B";
	if (evty == "soft") tchan = "Type A";
	if (charpref == chkge1) text3.AddText(tchan+", N_{b tags}#geq1");
	else if (charpref == chkex1) text3.AddText(tchan+", N_{b tags}=1");
	else if (charpref == chkex2) text3.AddText(tchan+", N_{b tags}=2");
	else text3.AddText(tchan);
	text3.SetBorderSize(0);
	text3.Draw();

	gPad->SetLogy(1);

	TPad* pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.28);
	pad2->SetTopMargin(0.0);
	pad2->SetBottomMargin(0.41);
	pad2->Draw();
	pad2->cd();
	ratio->Draw("hist");

	ratio->SetMinimum(-2.9);
	ratio->SetMaximum(2.9);

	TAxis * xAxis = ratio->GetXaxis();
	xAxis->SetTitle("M_{tb} (GeV)");
	xAxis->SetTitleFont(42);
	xAxis->SetTitleSize(0.14);
	xAxis->SetTitleOffset(1.0);
	xAxis->SetLabelFont(42);
	xAxis->SetLabelSize(0.15);
	xAxis->SetLabelOffset(0.007);
	xAxis->SetNdivisions(505);

	TAxis * yAxis2 = ratio->GetYaxis();
	yAxis2->SetNdivisions(305);
	//yAxis2->SetTitle("(Data-Bkg)/#sigma_{tot}");
	//yAxis2->SetTitleSize(0.09);
	//yAxis2->SetTitleFont(42);
	//yAxis2->SetTitleOffset(0.52);
	yAxis2->SetLabelFont(42);
	yAxis2->SetLabelSize(0.1);
	yAxis2->SetLabelOffset(0.007);
	//yAxis2->CenterTitle();
	
	TPaveText text4(0.04, 0.5, 0.07, 0.9,"NDC");
	text4.SetFillColor(kWhite);
	TText *text = text4.AddText("#frac{Data-Bkg}{Tot. unc.}");
	text4.SetBorderSize(0);
	text->SetTextAngle(90);
	text->SetTextSize(0.12);
	text->SetTextFont(42);
	text4.Draw();


	ratio->SetLineColor(kBlack);
	ratio->SetFillColor(kBlue);

	canv->SaveAs(dir+"/PostFit_"+chan+"_"+bcat+"_"+evty+"_"+filec+"_"+fileb+"_"+filee+".png");
	canv->SaveAs(dir+"/PostFit_"+chan+"_"+bcat+"_"+evty+"_"+filec+"_"+fileb+"_"+filee+".pdf");

}

void plotPostFit()
{

	TString direc = "Jul18_35p9fb_1D_NewLUM";

	makePlot(direc,"ex1btags","mu","soft","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex2btags","mu","soft","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex1btags","elec","soft","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex2btags","elec","soft","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex1btags","mu","hard","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex2btags","mu","hard","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex1btags","elec","hard","ex1or2btags","comb","hardsoft");
	makePlot(direc,"ex2btags","elec","hard","ex1or2btags","comb","hardsoft");

	//makePlot(direc,"ex2btags","elec","hard","ex2btags","electron","hard");

}
