  //#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<cmath>
  //#include<stdio.h>

using namespace std;


void obs150(std::string chan) {

 TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR"); 
 // For the canvas:


 tdrStyle->SetCanvasBorderMode(0);
 tdrStyle->SetCanvasColor(0); // must be kWhite but I dunno how to do that in PyROOT
 tdrStyle->SetCanvasDefH(600); //Height of canvas
 tdrStyle->SetCanvasDefW(600); //Width of canvas
 tdrStyle->SetCanvasDefX(0);   //POsition on screen
 tdrStyle->SetCanvasDefY(0);


 // For the Pad:
 tdrStyle->SetPadBorderMode(0);
 // tdrStyle->SetPadBorderSize(Width_t size = 1);
 tdrStyle->SetPadColor(0); // kWhite
 tdrStyle->SetPadGridX(0); //false
 tdrStyle->SetPadGridY(0); //false
 tdrStyle->SetGridColor(0);
 tdrStyle->SetGridStyle(3);
 tdrStyle->SetGridWidth(1);

 // For the frame:
 tdrStyle->SetFrameBorderMode(0);
 tdrStyle->SetFrameBorderSize(1);
 tdrStyle->SetFrameFillColor(0);
 tdrStyle->SetFrameFillStyle(0);
 tdrStyle->SetFrameLineColor(1);
 tdrStyle->SetFrameLineStyle(1);
 tdrStyle->SetFrameLineWidth(1);

 // For the histo:
 // tdrStyle->SetHistFillColor(1);
 // tdrStyle->SetHistFillStyle(0);
 tdrStyle->SetHistLineColor(1);
 tdrStyle->SetHistLineStyle(0);
 tdrStyle->SetHistLineWidth(1);
 // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
 // tdrStyle->SetNumberContours(Int_t number = 20);

 tdrStyle->SetEndErrorSize(2);
 //tdrStyle->SetErrorMarker(20);   /// I COMMENTED THIS OUT
 //tdrStyle->SetErrorX(0.);

 //tdrStyle->SetMarkerStyle(20);


 //For the fit/function:
 tdrStyle->SetOptFit(1011);
 tdrStyle->SetFitFormat("5.4g");
 tdrStyle->SetFuncColor(2);
 tdrStyle->SetFuncStyle(1);
 tdrStyle->SetFuncWidth(1);

 //For the date:
 tdrStyle->SetOptDate(0);
 // tdrStyle->SetDateX(Float_t x = 0.01);
 // tdrStyle->SetDateY(Float_t y = 0.01);

 // For the statistics box:
 tdrStyle->SetOptFile(0);
 tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
 tdrStyle->SetStatColor(0); // kWhite
 tdrStyle->SetStatFont(22);
 //tdrStyle->SetStatFontSize(0.025);
 tdrStyle->SetStatFontSize(0.04);
 tdrStyle->SetStatTextColor(1);
 tdrStyle->SetStatFormat("6.4g");
 tdrStyle->SetStatBorderSize(1);
 tdrStyle->SetStatH(0.1);
 tdrStyle->SetStatW(0.15);
 // tdrStyle->SetStatStyle(Style_t style = 1001);
 // tdrStyle->SetStatX(Float_t x = 0);
 // tdrStyle->SetStatY(Float_t y = 0);

 // For the Global title:

 tdrStyle->SetOptTitle(0);
 tdrStyle->SetTitleFont(22);
 tdrStyle->SetTitleColor(1);
 tdrStyle->SetTitleTextColor(1);
 tdrStyle->SetTitleFillColor(10);
 tdrStyle->SetTitleFontSize(0.05);
 // tdrStyle->SetTitleH(0); // Set the height of the title box
 // tdrStyle->SetTitleW(0); // Set the width of the title box
 // tdrStyle->SetTitleX(0); // Set the position of the title box
 // tdrStyle->SetTitleY(0.985); // Set the position of the title box
 // tdrStyle->SetTitleStyle(Style_t style = 1001);
 // tdrStyle->SetTitleBorderSize(2);

 // For the axis titles:

 tdrStyle->SetTitleColor(1, "XYZ");
 tdrStyle->SetTitleFont(22, "XYZ");
 tdrStyle->SetTitleSize(0.06, "XYZ");
 tdrStyle->SetTitleOffset(0.8, "XY"); // Another way to set the Offset
 tdrStyle->SetTitleOffset(1.3, "Z"); // Another way to set the Offset

 // For the axis labels:

 tdrStyle->SetLabelColor(1, "XYZ");
 tdrStyle->SetLabelFont(22, "XYZ");
 tdrStyle->SetLabelOffset(0.007, "XYZ");
 tdrStyle->SetLabelSize(0.05, "XYZ");

 // For the axis:

 tdrStyle->SetAxisColor(1, "XYZ");
 tdrStyle->SetStripDecimals(1); // kTRUE
 tdrStyle->SetTickLength(0.03, "XYZ");
 tdrStyle->SetNdivisions(510, "XY");
 tdrStyle->SetNdivisions(-510, "Z");
 tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
 tdrStyle->SetPadTickY(1);

 // Change for log plots:
 tdrStyle->SetOptLogx(0);
 tdrStyle->SetOptLogy(0);
 tdrStyle->SetOptLogz(0);

 // Postscript options:
 tdrStyle->SetPaperSize(20.,20.);




    tdrStyle->SetPalette(1);
 
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  tdrStyle->SetNumberContours(NCont);

  //TLatex *lab = new TLatex(0.70,0.85, "CMS 2008");
  //lab->SetNDC();
  //lab->SetTextFont(22);
  //lab->SetTextSize(0.05);
  //lab->Draw("same");

  gROOT -> ForceStyle();

  tdrStyle->cd();


  //Double_t m[12]={1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200};
  Double_t m[13]={1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600};

  //  Double_t m[14]={800,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400};

 TFile *file=new TFile("plots_observed.root");
 gStyle->SetTextFont(22);

 TH2F* obs1 = new TH2F(* ((TH2F*) file->Get("obs1")));

 // TH2 *obs1 = new TH2F("obs1","ar vs al", 101,-0.005,1.005, 101,-0.005, 1.005);

  
 TCanvas *c1 = new TCanvas("c1","c1",0,0,600,600);
 gStyle->SetOptStat(kFALSE);
 gStyle->SetOptTitle(kFALSE);
color=(TColor*)(gROOT->GetListOfColors()->At(100));
color->SetRGB(1,0,0);
color=(TColor*)(gROOT->GetListOfColors()->At(99));
color->SetRGB(1,0.3,0.3);
color=(TColor*)(gROOT->GetListOfColors()->At(98));
color->SetRGB(1,0.4,0.4);
color=(TColor*)(gROOT->GetListOfColors()->At(97));
color->SetRGB(1,0.5,0.5);
color=(TColor*)(gROOT->GetListOfColors()->At(96));
color->SetRGB(1,0.6,0.6);
color=(TColor*)(gROOT->GetListOfColors()->At(95));
color->SetRGB(1,0.65,0.65);
color=(TColor*)(gROOT->GetListOfColors()->At(94));
color->SetRGB(1,0.7,0.7);
color=(TColor*)(gROOT->GetListOfColors()->At(93));
color->SetRGB(1,0.75,0.75);
color=(TColor*)(gROOT->GetListOfColors()->At(92));
color->SetRGB(1,0.8,0.8);
color=(TColor*)(gROOT->GetListOfColors()->At(91));
color->SetRGB(1,0.85,0.85);
color=(TColor*)(gROOT->GetListOfColors()->At(90));
color->SetRGB(1,0.9,0.9);
color=(TColor*)(gROOT->GetListOfColors()->At(89));
color->SetRGB(1,1,1);
Int_t color_indices[12]={89,90,91,92,93,94,95,96,97,98,99,100};
gStyle->SetPalette(12,color_indices);

 c1->SetFillColor(kWhite);                                                                                           
 obs1->Draw("cont3zcol");
 obs1->SetLineWidth(3);
 c1->SetTopMargin(0.1);
 c1->SetBottomMargin(0.12);
 c1->SetRightMargin(0.22);
 c1->SetLeftMargin(0.12);

 obs1->SetContour(13,m);
 // obs1->Draw("cont2same");  

 obs1->GetXaxis()->SetTitle("a_{L}");

 obs1->GetYaxis()->SetTitle("a_{R}");

 obs1->GetZaxis()->SetTitle("M_{W'} (GeV)");
 obs1->GetZaxis()->SetRangeUser(1200,3600);
 obs1->GetZaxis()->SetNdivisions(12,false);
 obs1->GetZaxis()->SetTitleOffset(1.45);

TLatex *t1 = new TLatex();
float x1=0.2;
float y1=.85;

t1->SetNDC();
t1->SetTextFont(22);
t1->SetTextColor(1);
t1->SetTextAlign(11);
t1->SetTextSize(0.06);
t1->SetTextFont(22);

 TLatex* textcms;
 textcms = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS}");
 textcms->SetNDC();
 textcms->SetTextAlign(13);
 textcms->SetX(0.14);
 textcms->SetY(0.9455);
 textcms->SetTextFont(62);
 textcms->SetTextSizePixels(20);
 textcms->Draw();

 //TLatex* latex2 = new TLatex(1.570061,23.08044,"#scale[0.75]{#it{Preliminary}          35.9 fb^{-1}  (13 TeV)}");
 TLatex* latex2 = new TLatex(1.570061,23.08044,"#scale[0.75]{                           35.9 fb^{-1}  (13 TeV)}");
 latex2->SetNDC();
 latex2->SetTextAlign(13);
 latex2->SetX(0.24);
 latex2->SetY(0.9465);
 latex2->SetTextFont(42);
 latex2->SetTextSizePixels(20);
 latex2->Draw();

 TLatex *latex1=new TLatex();
 latex1->SetNDC();
 latex1->SetTextSize(0.04);
 latex1->SetTextAlign(31); // align right
 // latex1->DrawLatex(0.85, 0.86, " M(W') [GeV]");


 TLatex *latex4=new TLatex();
 latex4->SetNDC();
 latex4->SetTextSize(0.035);
 latex4->SetTextFont(22);
 latex4->SetTextAlign(22); // center
 if (chan=="electron") latex4->DrawLatex(0.27, 0.20, "e+jets");
 else if (chan=="muon") latex4->DrawLatex(0.27, 0.20, "#mu+jets");
 //else latex4->DrawLatex(0.27, 0.20, "e/#mu+jets");
 //latex4->DrawLatex(0.34, 0.23, "Invariant Mass Analysis");
 latex4->DrawLatex(0.27, 0.16, "95% CL Observed");



c1->Update();
c1->Print(Form("contour_arVSal-%s_observed150_final.pdf",chan.c_str()));
//c1->Print(Form("contour_arVSal-%s_observed150_final.C",chan.c_str()));
c1->Print(Form("contour_arVSal-%s_observed150_final.png",chan.c_str()));

    
}//file
  
