#include<string.h>
#include<stdio.h>
using std::string;

void plotLimits(string limitDIR,  string limitFILE,  string titchan, string category)
{
  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
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
  //  tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
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
  tdrStyle->SetOptStat("emr"); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
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
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  //tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  //tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  //tdrStyle->SetPaintTextFormat(const char* format = "g");
  tdrStyle->SetPalette(1);
  //tdrStyle->SetTimeOffset(Double_t toffset);
  //tdrStyle->SetHistMinimumZero(kTRUE);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  tdrStyle->SetNumberContours(NCont);

  gROOT -> ForceStyle();

  tdrStyle->cd();

  float xxx,yyy,charsize=0.07;

  gStyle->SetErrorX(5); 
  gStyle->SetEndErrorSize(5); 

  float X[35],EXP[35],EXPA[35],EXPB[35],EXPM1[35],EXPP1[35],EXPM2[35],EXPP2[35],OBS[35],OBSERR[35],CS[35],CS2[35],CSP[35],CS2P[35],CSM[35],CS2M[35];

  //  char name[10],expfile[80],obsfile[80],theoryfile[80],dirname[10];
  string expfile, expfileA, expfileB, obsfile,theoryfile, limitName, thTYPE;

  cout << " file name " << limitFILE << endl;

  //string explim = "_cls_limits_expected.txt";
  //string obslim = "_cls_limits_observed.txt";  
  string explim = "_bayesian_limits_expected_"+category+".txt";
  string obslim = "_bayesian_limits_observed_"+category+".txt";
  string thcurve = "_theory_Inv.txt";
  string sRH="_R_";
  string sLH="_L_";
  string sMix="_RL_";
  string mu="muon";
  string elec="electron";

  expfile=limitDIR+limitFILE+explim;
  obsfile=limitDIR+limitFILE+obslim;

  //if (limitFILE.find(sRH)<30) {thTYPE="R";}
  //if (limitFILE.find(sLH)<30) {thTYPE="L";}
  //if (limitFILE.find(sMix)<30) {thTYPE="LR";}

  thTYPE="R";
  char leptonType[80];
  if (category.find("ge1btags")!=std::string::npos) {
    if (limitFILE.find(mu)>=40 && limitFILE.find(elec)>=40) strcpy(leptonType,"#splitline{e/#mu+jets N_{b tags} #geq 1");
    if (limitFILE.find(mu)<40) strcpy(leptonType,"#splitline{#mu+jets N_{b tags} #geq 1");
    if (limitFILE.find(elec)<40) strcpy(leptonType,"#splitline{e+jets N_{b tags} #geq 1");
  }
  if (category.find("ex1btags")!=std::string::npos) {
    if (limitFILE.find(mu)>=40 && limitFILE.find(elec)>=40) strcpy(leptonType,"#splitline{e/#mu+jets N_{b tags} = 1");
    if (limitFILE.find(mu)<40) strcpy(leptonType,"#splitline{#mu+jets N_{b tags} = 1");
    if (limitFILE.find(elec)<40) strcpy(leptonType,"#splitline{e+jets N_{b tags} = 1");
  }
  if (category.find("ex2btags")!=std::string::npos) {
    if (limitFILE.find(mu)>=40 && limitFILE.find(elec)>=40) strcpy(leptonType,"#splitline{e/#mu+jets N_{b tags} = 2");
    if (limitFILE.find(mu)<40) strcpy(leptonType,"#splitline{#mu+jets N_{b tags} = 2");
    if (limitFILE.find(elec)<40) strcpy(leptonType,"#splitline{e+jets N_{b tags} = 2");
  }
  if (category.find("ex1or2btags")!=std::string::npos) {
    if (limitFILE.find(mu)>=40 && limitFILE.find(elec)>=40) strcpy(leptonType,"#splitline{e/#mu+jets N_{b tags} = 1 or 2");
    if (limitFILE.find(mu)<40) strcpy(leptonType,"#splitline{#mu+jets N_{b tags} = 1 or 2");
    if (limitFILE.find(elec)<40) strcpy(leptonType,"#splitline{e+jets N_{b tags} = 1 or 2");
  }
  if (category.find("hardsoft")!=std::string::npos) strcat(leptonType,"}{Type A or Type B}");
  else if (category.find("soft")!=std::string::npos) strcat(leptonType,"}{Type A}");
  else if (category.find("hard")!=std::string::npos) strcat(leptonType,"}{Type B}");
  else strcat(leptonType,"}{No Type Categorization}");
  if (category.find("ex1or2btags")!=std::string::npos && (limitFILE.find(mu)>=40 && limitFILE.find(elec)>=40) && category.find("hardsoft")!=std::string::npos) {leptonType[0] = ' '; leptonType[1] = '\0';}
  if (category.find("ex1or2btags")!=std::string::npos && limitFILE.find(mu)<40 && category.find("hardsoft")!=std::string::npos) {
    leptonType[0] = '#';
    leptonType[1] = 'm';
    leptonType[2] = 'u';
    leptonType[3] = '+';
    leptonType[4] = 'j';
    leptonType[5] = 'e';
    leptonType[6] = 't';
    leptonType[7] = 's';
    for (unsigned int li = 8; li < 20; li++) {leptonType[li] = ' ';}
    leptonType[20] = '\0';
  }
  if (category.find("ex1or2btags")!=std::string::npos && limitFILE.find(elec)<40 && category.find("hardsoft")!=std::string::npos) {
    leptonType[0] = 'e';
    leptonType[1] = '+';
    leptonType[2] = 'j';
    leptonType[3] = 'e';
    leptonType[4] = 't';
    leptonType[5] = 's';
    for (unsigned int li = 6; li < 20; li++) {leptonType[li] = ' ';}
    leptonType[20] = '\0';
  }
  theoryfile=thTYPE+thcurve;
  //cout << " th file name " << theoryfile << endl;
  //cout << " exp file name " << expfile << endl;
  //cout << " obs file name " << obsfile << endl;
  //std::cout<<leptonType<<std::endl;

  int i=0;
  ifstream expFile;
  expFile.open(expfile.c_str());

  i=0;
  double xmass=0.;
  double maxmass = 4000.;
  while( !expFile.eof() ) {
    //string line;
    //getline(expFile,line);
    //if (line[0] == '#') continue;
    expFile >> X[i] >> EXP[i] >> EXPM2[i] >> EXPP2[i] >> EXPM1[i] >> EXPP1[i];
    //cout<<X[i]<<" "<<EXP[i]<<" "<<EXPM1[i]<<" "<<EXPP1[i]<<" "<<EXPM2[i]<<" "<<EXPP2[i]<<endl;
    xmass =X[i];
    i++ ;
    if (xmass==maxmass) break;
  }
  expFile.close();

  ifstream obsFile;
  obsFile.open(obsfile.c_str());

  //cout<<"exp--------obs"<<endl;
  i=0;
  xmass=0;
  while( !obsFile.eof() ){
    //string line;
    //getline(obsFile,line);
    //if (line[0] == '#') continue;
    obsFile >> X[i] >> OBS[i] >> OBSERR[i];
    xmass =X[i];
    //cout << i <<" " << xmass << " " <<OBS[i] <<endl;
    i++ ;
    if (xmass==maxmass) break;
  }

  obsFile.close();

  ifstream theoryFile;
  theoryFile.open(theoryfile.c_str());

  i=0;
  xmass=0.;
  while(!theoryFile.eof() ){
    theoryFile >> X[i] >> CS[i];
    CS[i]=CS[i]*1.25;
    CS2[i]=CS[i]*4/3;
    CSP[i]=CS[i]*(1+sqrt((0.02*X[i]/500)*(0.02*X[i]/500)+(0.06+(0.01*X[i]/500))*(0.06+(0.01*X[i]/500))));
    CSM[i]=CS[i]*(1-sqrt((0.02*X[i]/500)*(0.02*X[i]/500)+(0.005+(0.015*X[i]/500))*(0.005+(0.015*X[i]/500))));
    CS2P[i]=CS2[i]*(1+sqrt((0.02*X[i]/500)*(0.02*X[i]/500)+(0.06+(0.01*X[i]/500))*(0.06+(0.01*X[i]/500))));
    CS2M[i]=CS2[i]*(1-sqrt((0.02*X[i]/500)*(0.02*X[i]/500)+(0.005+(0.015*X[i]/500))*(0.005+(0.015*X[i]/500))));

    xmass =X[i];
    i++ ;
    //cout << i <<" " << xmass<<endl;
    if (xmass==maxmass) break;

  }

  int Ndata=i;
  //cout << "Ndata " << Ndata << endl;

  //cout<< "mass"<<" theory "<<" observed"<<" +2 sigma"<<" +1 sigma"<<" expected"<<"  -1 sigma"<<" -2 sigma"<<endl;

  bool foundObs = false;
  bool foundExp = false;
  bool foundObs2 = false;
  bool foundExp2 = false;
  for (int i=0; i<Ndata-1;i++) {
    //cout<< X[i]<< "  " << CS[i]<< "  "<< " " << OBS[i]<< "  "<<"  "<<EXPP2[i]<<"   "<<EXPP1[i]<<"  "<<EXP[i]<<"  "<<EXPM1[i]<<"  "<<EXPM2[i]<<endl;
    if (!foundObs && OBS[i]<CS[i] && CS[i+1]<OBS[i+1]) {
      cout<<"Observed limit = "<<((OBS[i]-CS[i])*(X[i+1]-X[i])/(CS[i+1]-CS[i]-OBS[i+1]+OBS[i]))+X[i]<<endl;
      foundObs = true;
    }
    if (!foundExp && EXP[i]<CS[i] && CS[i+1]<EXP[i+1]) {
      cout<<"Expected limit = "<<((EXP[i]-CS[i])*(X[i+1]-X[i])/(CS[i+1]-CS[i]-EXP[i+1]+EXP[i]))+X[i]<<endl;
      foundExp = true;
    }
    if (!foundObs2 && OBS[i]<CS2[i] && CS2[i+1]<OBS[i+1]) {
      cout<<"Observed limit = "<<((OBS[i]-CS2[i])*(X[i+1]-X[i])/(CS2[i+1]-CS2[i]-OBS[i+1]+OBS[i]))+X[i]<<endl;
      foundObs2 = true;
    }
    if (!foundExp2 && EXP[i]<CS2[i] && CS2[i+1]<EXP[i+1]) {
      cout<<"Expected limit = "<<((EXP[i]-CS2[i])*(X[i+1]-X[i])/(CS2[i+1]-CS2[i]-EXP[i+1]+EXP[i]))+X[i]<<endl;
      foundExp2 = true;
    }
  }

  //double BR_t_blv = 1.;
  double BR_t_blv = 0.338;
  for (int i=0; i<Ndata; i++) {
    CS[i] /= BR_t_blv;
    CSP[i] /= BR_t_blv;
    CSM[i] /= BR_t_blv;
    CS2[i] /= BR_t_blv;
    CS2P[i] /= BR_t_blv;
    CS2M[i] /= BR_t_blv;
    EXP[i] /= BR_t_blv;
    EXPP1[i] /= BR_t_blv;
    EXPP2[i] /= BR_t_blv;
    EXPM1[i] /= BR_t_blv;
    EXPM2[i] /= BR_t_blv;
    OBS[i] /= BR_t_blv;
  }  

  //cout << Ndata << " " << X[Ndata-1] << " " << OBS[Ndata-1] << endl;

  TGraph *xsec = new TGraph(Ndata,X,CS);
  xsec->SetName("xsec");
  TGraph *xsec2 = new TGraph(Ndata,X,CS2);
  xsec2->SetName("xsec2");
  TGraph *lobs = new TGraph(Ndata,X,OBS);
  lobs->SetName("lobs");
  TGraph *lexp = new TGraph(Ndata,X,EXP);
  lexp->SetName("lexp");

  xsec->SetTitle("");
  xsec->SetLineColor(kRed);

  xsec2->SetTitle("");
  xsec2->SetLineColor(kRed);

  lobs->SetLineColor(kBlack);
  lexp->SetLineColor(kBlack);

  xsec->SetLineWidth(3);
  xsec2->SetLineWidth(3);
  //xsec2->SetLineStyle(5);

  lobs->SetLineWidth(2);
  lexp->SetLineWidth(2);

  lexp->SetLineStyle(2);

  //float xbins[22]={950,1050,1150,1250,1350,1450,1550,1650,1750,1850,1950,2050,2150,2250,2350,2450,2550,2650,2750,2850,2950,3050};
  float xbins[32]={950,1050,1150,1250,1350,1450,1550,1650,1750,1850,1950,2050,2150,2250,2350,2450,2550,2650,2750,2850,2950,3050,3150,3250,3350,3450,3550,3650,3750,3850,3950,4050};
  //float xbins[28]={950,1050,1150,1250,1450,1550,1650,1750,1850,2050,2150,2250,2350,2450,2550,2650,2750,2850,2950,3050,3150,3250,3450,3550,3650,3850,3950,4050};

  TH1D *hr = new TH1D("hr","",Ndata,xbins);

  //  for(int i=0;i<Ndata;i++) {
  //    float center = (EXPP2[i]+EXPM2[i])/2.0;
  //    hrx->SetBinContent(i+1,center);
  //    hrx->SetBinError(i+1,TMath::Abs(EXPM2[i]-center));
  //    hrerr->SetBinContent(i+1,TMath::Abs(EXPM2[i]-center)/center);
  //  }

  for(int i=0;i<Ndata;i++) {
    float center = (EXPP2[i]+EXPM2[i])/2.0;
    float error = TMath::Abs(EXPM2[i]-center);
    hr->SetBinContent(i+1,center);
    hr->SetBinError(i+1,error);
  }

  TH1D *hr2 = new TH1D("hr2","",Ndata,xbins);

  //  for(int i=0;i<Ndata;i++) {
  //    float center = (EXPP1[i]+EXPM1[i])/2.0;
  //    hrx->SetBinContent(i+1,center);
  //    hrx->SetBinError(i+1,TMath::Abs(EXPM1[i]-center));
  //    hrerr->SetBinContent(i+1,TMath::Abs(EXPM1[i]-center)/center);
  //  }

  for(int i=0;i<Ndata;i++) {
    float center = (EXPP1[i]+EXPM1[i])/2.0;
    float error = TMath::Abs(EXPM1[i]-center);
    hr2->SetBinContent(i+1,center);
    hr2->SetBinError(i+1,error);
  }

  TH1D *hrcs = new TH1D("hrcs","",Ndata,xbins);

  for(int i=0;i<Ndata;i++) {
    float center = (CSP[i]+CSM[i])/2.0;
    float error = TMath::Abs(CSM[i]-center);
    hrcs->SetBinContent(i+1,center);
    hrcs->SetBinError(i+1,error);
  }

  TH1D *hrcs2 = new TH1D("hrcs2","",Ndata,xbins);

  for(int i=0;i<Ndata;i++) {
    float center = (CS2P[i]+CS2M[i])/2.0;
    float error = TMath::Abs(CS2M[i]-center);
    hrcs2->SetBinContent(i+1,center);
    hrcs2->SetBinError(i+1,error);
  }

  TCanvas *c1 = new TCanvas("c1","PRL Fig. 3",1000,800);  
  c1->cd();
  gStyle->SetOptStat(0);
  c1->Range(0,0,1,1);
  c1->SetTopMargin(0.06);

  hr->SetFillColor(kOrange);
  hr->SetLineColor(kBlack);
  hr->SetMarkerColor(0);
  hr->SetMarkerSize(0);
  hr->SetLineWidth(0);
  if (limitFILE.find(sRH)<30) {
    //hr->GetYaxis()->SetTitle("#sigma(pp#rightarrowW'_{R}) #times B(W'_{R}#rightarrowtb#rightarrowl#nubb) [pb]");
    hr->GetYaxis()->SetTitle("#sigma_{W'_{R}} #times #bf{#it{#Beta}}(W'_{R}#rightarrowtb) (pb)");
    hr->GetXaxis()->SetTitle("M_{W'_{R}} (GeV)");
  }
  else {
    hr->GetYaxis()->SetTitle("#sigma_{W'_{R}} #times #bf{#it{#Beta}}(W'_{R}#rightarrowtb) (pb)");
    hr->GetXaxis()->SetTitle("M_{W'_{R}} (GeV)");
  }
  hr->SetMaximum(160.0/BR_t_blv);
  hr->SetMinimum(0.0004/BR_t_blv);
  if (limitFILE.find(sRH)<30) {    
    hr->SetMaximum(160.0/BR_t_blv);
    hr->SetMinimum(0.0004/BR_t_blv);
  }

  hr->Draw("E3");

  hr2->SetFillColor(kGreen+1);
  hr2->SetLineColor(kBlack); 
  hr2->SetLineWidth(0); 
  hr2->SetMarkerColor(0);   
  hr2->SetMarkerSize(0);   
  hr2->GetXaxis()->SetLabelOffset(0.0001);
  hr2->Draw("E3same");

  hrcs->SetFillColor(kRed);
  hrcs->SetFillStyle(3001);
  hrcs->SetLineColor(kBlack);
  hrcs->SetMarkerColor(0);
  hrcs->SetMarkerSize(0);
  hrcs->SetLineWidth(0);
  hrcs->Draw("E3same");

  hrcs2->SetFillColor(kBlue);
  hrcs2->SetFillStyle(3001);
  hrcs2->SetLineColor(kBlack);
  hrcs2->SetMarkerColor(0);
  hrcs2->SetMarkerSize(0);
  hrcs2->SetLineWidth(0);
  hrcs2->Draw("E3same");

  lexp->Draw("L");
  xsec->Draw("C");
  xsec2->SetLineColor(kBlue);
  xsec2->Draw("C");
  
  lobs->Draw("CP");

  c1->SetLogy();
    
  /*TLatex *latex2 = new TLatex();
  latex2->SetNDC();
  latex2->SetTextSize(0.04);
  latex2->SetTextAlign(31);// # align right
  latex2->DrawLatex(0.87, 0.95, "CMS Preliminary, L=6.3 fb^{-1} at #sqrt{s} = 13 TeV");*/

  TLatex* textcms;
  //textcms = new TLatex(1.570061,23.08044,"#scale[0.85]{CMS}");
  textcms = new TLatex(1.570061,23.08044,"#scale[1.5]{CMS}");
  textcms->SetNDC();
  textcms->SetTextAlign(13);
  textcms->SetX(0.2);
  textcms->SetY(0.88);
  textcms->SetTextFont(62);
  textcms->SetTextSizePixels(20);
  textcms->Draw();

  //TLatex* latex2 = new TLatex(1.570061,23.08044,"#scale[0.75]{#it{Preliminary}                                              35.9 fb^{-1}  (13 TeV)}");
  TLatex* latex2 = new TLatex(1.570061,23.08044,"#scale[0.75]{                                                               35.9 fb^{-1}  (13 TeV)}");
  latex2->SetNDC();
  latex2->SetTextAlign(13);
  latex2->SetX(0.235);
  latex2->SetY(0.9935);
  latex2->SetTextFont(42);
  latex2->SetTextSizePixels(20);
  latex2->Draw();

  TLatex *latex3 = new TLatex();
  latex3->SetNDC();
  latex3->SetTextSize(0.055);
  latex3->SetTextAlign(13); // # align right
  latex3->DrawLatex(0.2, 0.77, leptonType);

  TLatex *latex4 = new TLatex();
  latex4->SetNDC();
  latex4->SetTextSize(0.05);
  latex4->SetTextAlign(22); // # align right
  //latex4->DrawLatex(0.38, 0.27, "Invariant Mass Analysis");
    
  TLegend *leg = new TLegend(.51,.56,.9,.91,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextFont(62);
  leg->SetLineColor(10);
  leg->SetLineStyle(0);
  leg->SetLineWidth(0);
  leg->SetFillColor(10);
  leg->SetFillStyle(0);
  TLegendEntry *entry=leg->AddEntry("xsec","W'_{R} signal (M_{#nu_{R}}<< M_{W'_{R}})","l");
  entry=leg->AddEntry("xsec2","W'_{R} signal (M_{#nu_{R}}> M_{W'_{R}})","l");
  entry=leg->AddEntry("lobs","Observed limit (95% CL)","lp");
  entry=leg->AddEntry("lexp","Expected limit (95% CL)","l");
  entry=leg->AddEntry(hr2,"68% expected","f");
  entry=leg->AddEntry(hr,"95% expected","f");
  leg->Draw();

  c1->Modified();
  c1->cd();
  c1->SetSelected(c1);
  char pname[100];
  sprintf(pname,"%s/limit_plots/%s_%s_bayesian_Inv.png",limitDIR.c_str(),limitFILE.c_str(),category.c_str());
  c1->Print(pname);
  sprintf(pname,"%s/limit_plots/%s_%s_bayesian_Inv.pdf",limitDIR.c_str(),limitFILE.c_str(),category.c_str());
  c1->Print(pname);
  sprintf(pname,"%s/limit_plots/%s_%s_bayesian_Inv.root",limitDIR.c_str(),limitFILE.c_str(),category.c_str());
  c1->Print(pname);

}
