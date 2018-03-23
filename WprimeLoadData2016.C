#ifndef LoadData_cxx
#define LoadData_cxx

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1D.h>
#include <TString.h>
#include <TLorentzVector.h>

#include <iostream>
#include <vector>

#include "Objects/interface/treetop.h"
#include "Objects/interface/WprimeEvent.h"
#include "Objects/interface/FileExists.h"

using namespace std;

//Global variables

double wjlhe_100200 = 0.998056;//0.931355;
double wjlhe_200400 = 0.978569;//0.913146;
double wjlhe_400600 = 0.928054;//0.866418;
double wjlhe_600800 = 0.856705;//0.799251;
double wjlhe_8001200 = 0.757463;//0.706555;
double wjlhe_12002500 = 0.608292;//0.567275;
double wjlhe_2500inf = 0.454246;//0.422828;

//double WJHFSF = 1.6152;//HT bins
//double WJLFSF = 0.711718;//HT bins
double WJHFSF = 2.09583;//Pt bins
double WJLFSF = 0.49128;//Pt bins


enum Samples_t {Data, 
		TTbar,           TTbar_scaleup,   TTbar_scaledown, ZJets,           WJets,     
		ZJets_HT100to200,ZJets_HT200to400,ZJets_HT400to600,ZJets_HT600to800,ZJets_HT800to1200,ZJets_HT1200to2500,ZJets_HT2500toInf,
		WJets_HT100to200,WJets_HT200to400,WJets_HT400to600,WJets_HT600to800,WJets_HT800to1200,WJets_HT1200to2500,WJets_HT2500toInf,
		WJets_HT100to200_LF,WJets_HT200to400_LF,WJets_HT400to600_LF,WJets_HT600to800_LF,WJets_HT800to1200_LF,WJets_HT1200to2500_LF,WJets_HT2500toInf_LF,
		WJets_HT100to200_HF,WJets_HT200to400_HF,WJets_HT400to600_HF,WJets_HT600to800_HF,WJets_HT800to1200_HF,WJets_HT1200to2500_HF,WJets_HT2500toInf_HF,
		WJets_Pt100to250,WJets_Pt250to400,WJets_Pt400to600,WJets_Pt600toInf,
		WJets_Pt100to250_LF,WJets_Pt250to400_LF,WJets_Pt400to600_LF,WJets_Pt600toInf_LF,
		WJets_Pt100to250_HF,WJets_Pt250to400_HF,WJets_Pt400to600_HF,WJets_Pt600toInf_HF,
		WW,              WZ,              ZZ,      
                T_s,             T_t,             T_tW,
                Tbar_s,          Tbar_t,          Tbar_tW,  //38
                //QCD_Pt_20_Mu,
                //QCD_Pt_20_30_EM,  QCD_Pt_30_80_EM,  QCD_Pt_80_170_EM,
                QCD_HT200to300,    QCD_HT300to500,    QCD_HT500to700,    QCD_HT700to1000,   QCD_HT1000to1500,    QCD_HT1500to2000,   QCD_HT2000toInf,
                QCD_Pt_5to10,      QCD_Pt_10to15,     QCD_Pt_15to30,     QCD_Pt_30to50,     QCD_Pt_50to80,
                QCD_Pt_80to120,    QCD_Pt_120to170,   QCD_Pt_170to300,   QCD_Pt_300to470,   QCD_Pt_470to600,
                QCD_Pt_600to800,   QCD_Pt_800to1000,  QCD_Pt_1000to1400, QCD_Pt_1400to1800, QCD_Pt_1800to2400,  //51
                //QCD_Pt_2400to3200, QCD_Pt_3200toInf,
		Wprime800Right,  Wprime900Right,  Wprime1000Right,
		Wprime1100Right, Wprime1200Right, Wprime1300Right,
		Wprime1400Right, Wprime1500Right, Wprime1600Right,
		Wprime1700Right, Wprime1800Right, Wprime1900Right,
		Wprime2000Right, Wprime2100Right, Wprime2200Right,
		Wprime2300Right, Wprime2400Right, Wprime2500Right,
		Wprime2600Right, Wprime2700Right, Wprime2800Right,
		Wprime2900Right, Wprime3000Right,  //74
	        Wprime3100Right, Wprime3200Right,
	        Wprime3300Right, Wprime3400Right, Wprime3500Right,
	        Wprime3600Right, Wprime3700Right, Wprime3800Right,
	        Wprime3900Right, Wprime4000Right, //(+10)
		Wprime800Mix,  Wprime900Mix,  Wprime1000Mix,
		Wprime1100Mix, Wprime1200Mix, Wprime1300Mix,
		Wprime1400Mix, Wprime1500Mix, Wprime1600Mix,
		Wprime1700Mix, Wprime1800Mix, Wprime1900Mix,
		Wprime2000Mix, Wprime2100Mix, Wprime2200Mix,
		Wprime2300Mix, Wprime2400Mix, Wprime2500Mix,
		Wprime2600Mix, Wprime2700Mix, Wprime2800Mix,
		Wprime2900Mix, Wprime3000Mix,  //97
	        Wprime3100Mix, Wprime3200Mix,
	        Wprime3300Mix, Wprime3400Mix, Wprime3500Mix,
	        Wprime3600Mix, Wprime3700Mix, Wprime3800Mix,
	        Wprime3900Mix, Wprime4000Mix, //(+10)
		Wprime800Left,  Wprime900Left,  Wprime1000Left,
		Wprime1100Left, Wprime1200Left, Wprime1300Left,
		Wprime1400Left, Wprime1500Left, Wprime1600Left,
		Wprime1700Left, Wprime1800Left, Wprime1900Left,
		Wprime2000Left, Wprime2100Left, Wprime2200Left,
		Wprime2300Left, Wprime2400Left, Wprime2500Left,
		Wprime2600Left, Wprime2700Left, Wprime2800Left,
		Wprime2900Left, Wprime3000Left, //120
	        Wprime3100Left, Wprime3200Left,
	        Wprime3300Left, Wprime3400Left, Wprime3500Left,
	        Wprime3600Left, Wprime3700Left, Wprime3800Left,
	        Wprime3900Left, Wprime4000Left //(+10)
};

//const unsigned int totalSamples = 131; without 3.1-4.0 TeV sig
const unsigned int totalSamples = 176;

Samples_t allSamples[totalSamples] = {Data, 
				      TTbar,           TTbar_scaleup,   TTbar_scaledown, ZJets,           WJets,     
		                      ZJets_HT100to200,ZJets_HT200to400,ZJets_HT400to600,ZJets_HT600to800,ZJets_HT800to1200,ZJets_HT1200to2500,ZJets_HT2500toInf,
		                      WJets_HT100to200,WJets_HT200to400,WJets_HT400to600,WJets_HT600to800,WJets_HT800to1200,WJets_HT1200to2500,WJets_HT2500toInf,
		                      WJets_HT100to200_LF,WJets_HT200to400_LF,WJets_HT400to600_LF,WJets_HT600to800_LF,WJets_HT800to1200_LF,WJets_HT1200to2500_LF,WJets_HT2500toInf_LF,
		                      WJets_HT100to200_HF,WJets_HT200to400_HF,WJets_HT400to600_HF,WJets_HT600to800_HF,WJets_HT800to1200_HF,WJets_HT1200to2500_HF,WJets_HT2500toInf_HF,
		                      WJets_Pt100to250,WJets_Pt250to400,WJets_Pt400to600,WJets_Pt600toInf,
		                      WJets_Pt100to250_LF,WJets_Pt250to400_LF,WJets_Pt400to600_LF,WJets_Pt600toInf_LF,
		                      WJets_Pt100to250_HF,WJets_Pt250to400_HF,WJets_Pt400to600_HF,WJets_Pt600toInf_HF,
				      WW,              WZ,          ZZ,      
                                      T_s,             T_t,         T_tW,
                                      Tbar_s,          Tbar_t,      Tbar_tW,
                                      //QCD_Pt_20_Mu,
                                      //QCD_Pt_20_30_EM,  QCD_Pt_30_80_EM,  QCD_Pt_80_170_EM,
                                      QCD_HT200to300,  QCD_HT300to500,  QCD_HT500to700,  QCD_HT700to1000,  QCD_HT1000to1500,  QCD_HT1500to2000,  QCD_HT2000toInf,
                                      QCD_Pt_5to10,      QCD_Pt_10to15,     QCD_Pt_15to30,     QCD_Pt_30to50,     QCD_Pt_50to80,
                                      QCD_Pt_80to120,    QCD_Pt_120to170,   QCD_Pt_170to300,   QCD_Pt_300to470,   QCD_Pt_470to600,
                                      QCD_Pt_600to800,   QCD_Pt_800to1000,  QCD_Pt_1000to1400, QCD_Pt_1400to1800, QCD_Pt_1800to2400,
                                      //QCD_Pt_2400to3200, QCD_Pt_3200toInf,
				      Wprime800Right,  Wprime900Right,  Wprime1000Right,
				      Wprime1100Right, Wprime1200Right, Wprime1300Right,
				      Wprime1400Right, Wprime1500Right, Wprime1600Right,
				      Wprime1700Right, Wprime1800Right, Wprime1900Right,
				      Wprime2000Right, Wprime2100Right, Wprime2200Right,
				      Wprime2300Right, Wprime2400Right, Wprime2500Right,
				      Wprime2600Right, Wprime2700Right, Wprime2800Right,
				      Wprime2900Right, Wprime3000Right,
				      Wprime3100Right, Wprime3200Right,
				      Wprime3300Right, Wprime3400Right, Wprime3500Right,
				      Wprime3600Right, Wprime3700Right, Wprime3800Right,
				      Wprime3900Right, Wprime4000Right,
				      Wprime800Mix,  Wprime900Mix,  Wprime1000Mix,
				      Wprime1100Mix, Wprime1200Mix, Wprime1300Mix,
				      Wprime1400Mix, Wprime1500Mix, Wprime1600Mix,
				      Wprime1700Mix, Wprime1800Mix, Wprime1900Mix,
				      Wprime2000Mix, Wprime2100Mix, Wprime2200Mix,
				      Wprime2300Mix, Wprime2400Mix, Wprime2500Mix,
				      Wprime2600Mix, Wprime2700Mix, Wprime2800Mix,
				      Wprime2900Mix, Wprime3000Mix,
				      Wprime3100Mix, Wprime3200Mix,
				      Wprime3300Mix, Wprime3400Mix, Wprime3500Mix,
				      Wprime3600Mix, Wprime3700Mix, Wprime3800Mix,
				      Wprime3900Mix, Wprime4000Mix,
				      Wprime800Left,  Wprime900Left,  Wprime1000Left,
				      Wprime1100Left, Wprime1200Left, Wprime1300Left,
				      Wprime1400Left, Wprime1500Left, Wprime1600Left,
				      Wprime1700Left, Wprime1800Left, Wprime1900Left,
				      Wprime2000Left, Wprime2100Left, Wprime2200Left,
				      Wprime2300Left, Wprime2400Left, Wprime2500Left,
				      Wprime2600Left, Wprime2700Left, Wprime2800Left,
				      Wprime2900Left, Wprime3000Left,
				      Wprime3100Left, Wprime3200Left,
				      Wprime3300Left, Wprime3400Left, Wprime3500Left,
				      Wprime3600Left, Wprime3700Left, Wprime3800Left,
				      Wprime3900Left, Wprime4000Left
};

bool Sig[totalSamples] = {false,
			     false,	false,	false,	false,	false,
			     false,	false,	false,	false,  false,  false,  false,
			     false,	false,	false,	false,  false,  false,  false,
			     false,	false,	false,	false,  false,  false,  false,
			     false,	false,	false,	false,  false,  false,  false,
                             false,     false,  false,  false,
                             false,     false,  false,  false,
                             false,     false,  false,  false,
			     false,	false,  false,
			     false,	false,	false,
			     false,	false,	false,
			     //false,
			     //false,	false,	false,
			     false,	false,	false,	false,  false,	false,  false,
			     false,	false,	false,	false,  false,
			     false,	false,	false,	false,  false,
			     false,	false,	false,	false,  false,
			     //false,	false,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,
			     true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,
			     true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true,
			     true,	true,
			     true,	true,	true,
			     true,	true,	true,
			     true,	true
};

TString allNames[totalSamples] = {"Data",    
				  "TTbar",           "TTbar_scaleup",   "TTbar_scaledown", "ZJets",           "WJets",     
		                  "ZJets_HT100to200","ZJets_HT200to400","ZJets_HT400to600","ZJets_HT600to800","ZJets_HT800to1200","ZJets_HT1200to2500","ZJets_HT2500toInf",
		                  "WJets_HT100to200","WJets_HT200to400","WJets_HT400to600","WJets_HT600to800","WJets_HT800to1200","WJets_HT1200to2500","WJets_HT2500toInf",
		                  "WJets_HT100to200_LF","WJets_HT200to400_LF","WJets_HT400to600_LF","WJets_HT600to800_LF","WJets_HT800to1200_LF","WJets_HT1200to2500_LF","WJets_HT2500toInf_LF",
		                  "WJets_HT100to200_HF","WJets_HT200to400_HF","WJets_HT400to600_HF","WJets_HT600to800_HF","WJets_HT800to1200_HF","WJets_HT1200to2500_HF","WJets_HT2500toInf_HF",
		                  "WJets_Pt100to250","WJets_Pt250to400","WJets_Pt400to600","WJets_Pt600toInf",
		                  "WJets_Pt100to250_LF","WJets_Pt250to400_LF","WJets_Pt400to600_LF","WJets_Pt600toInf_LF",
		                  "WJets_Pt100to250_HF","WJets_Pt250to400_HF","WJets_Pt400to600_HF","WJets_Pt600toInf_HF",
				  "WW",              "WZ",              "ZZ", 
                                  "T_s",             "T_t",             "T_tW",
                                  "Tbar_s",          "Tbar_t",          "Tbar_tW",
                                  //"QCD_Pt_20_Mu",
                                  //"QCD_Pt_20_30_EM",  "QCD_Pt_30_80_EM",  "QCD_Pt_80_170_EM",
                                  "QCD_HT200to300",   "QCD_HT300to500",   "QCD_HT500to700",   "QCD_HT700to1000",  "QCD_HT1000to1500",  "QCD_HT1500to2000",  "QCD_HT2000toInf",
                                  "QCD_Pt_5to10",     "QCD_Pt_10to15",    "QCD_Pt_15to30",    "QCD_Pt_30to50",    "QCD_Pt_50to80",
                                  "QCD_Pt_80to120",   "QCD_Pt_120to170",  "QCD_Pt_170to300",  "QCD_Pt_300to470",  "QCD_Pt_470to600",
                                  "QCD_Pt_600to800",  "QCD_Pt_800to1000", "QCD_Pt_1000to1400","QCD_Pt_1400to1800","QCD_Pt_1800to2400",
                                  //"QCD_Pt_2400to3200","QCD_Pt_3200toInf",
				  "Wprime800Right",  "Wprime900Right",  "Wprime1000Right",
				  "Wprime1100Right", "Wprime1200Right", "Wprime1300Right",
				  "Wprime1400Right", "Wprime1500Right", "Wprime1600Right",
				  "Wprime1700Right", "Wprime1800Right", "Wprime1900Right",
				  "Wprime2000Right", "Wprime2100Right", "Wprime2200Right",
				  "Wprime2300Right", "Wprime2400Right", "Wprime2500Right",
				  "Wprime2600Right", "Wprime2700Right", "Wprime2800Right",
				  "Wprime2900Right", "Wprime3000Right",
				  "Wprime3100Right", "Wprime3200Right",
				  "Wprime3300Right", "Wprime3400Right", "Wprime3500Right",
				  "Wprime3600Right", "Wprime3700Right", "Wprime3800Right",
				  "Wprime3900Right", "Wprime4000Right",
				  "Wprime800Mix",  "Wprime900Mix",  "Wprime1000Mix",
				  "Wprime1100Mix", "Wprime1200Mix", "Wprime1300Mix",
				  "Wprime1400Mix", "Wprime1500Mix", "Wprime1600Mix",
				  "Wprime1700Mix", "Wprime1800Mix", "Wprime1900Mix",
				  "Wprime2000Mix", "Wprime2100Mix", "Wprime2200Mix",
				  "Wprime2300Mix", "Wprime2400Mix", "Wprime2500Mix",
				  "Wprime2600Mix", "Wprime2700Mix", "Wprime2800Mix",
				  "Wprime2900Mix", "Wprime3000Mix",
				  "Wprime3100Mix", "Wprime3200Mix",
				  "Wprime3300Mix", "Wprime3400Mix", "Wprime3500Mix",
				  "Wprime3600Mix", "Wprime3700Mix", "Wprime3800Mix",
				  "Wprime3900Mix", "Wprime4000Mix",
				  "Wprime800Left",  "Wprime900Left",  "Wprime1000Left",
				  "Wprime1100Left", "Wprime1200Left", "Wprime1300Left",
				  "Wprime1400Left", "Wprime1500Left", "Wprime1600Left",
				  "Wprime1700Left", "Wprime1800Left", "Wprime1900Left",
				  "Wprime2000Left", "Wprime2100Left", "Wprime2200Left",
				  "Wprime2300Left", "Wprime2400Left", "Wprime2500Left",
				  "Wprime2600Left", "Wprime2700Left", "Wprime2800Left",
				  "Wprime2900Left", "Wprime3000Left",
				  "Wprime3100Left", "Wprime3200Left",
				  "Wprime3300Left", "Wprime3400Left", "Wprime3500Left",
				  "Wprime3600Left", "Wprime3700Left", "Wprime3800Left",
				  "Wprime3900Left", "Wprime4000Left"
};
   
Color_t color[totalSamples] = {kBlack,				 //Data
			       kRed-3,     kRed-3,    kRed-3,   kOrange-4, kGreen-3,  //TTbar, WJets and ZJets
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  kGreen-3,  kGreen-3, kGreen-3,  //ZJets HT bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  kGreen-3,  kGreen-3, kGreen-3,  //WJets HT bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  kGreen-3,  kGreen-3, kGreen-3,  //WJets HT bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  kGreen-3,  kGreen-3, kGreen-3,  //WJets HT bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  //WJets Pt bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  //WJets Pt bins
			       kGreen-3,   kGreen-3,  kGreen-3, kGreen-3,  //WJets Pt bins
			       kViolet+1,  kAzure+2,  kAzure-2,             //WW, WZ, ZZ       
			       kGreen-9,   kGreen-9,  kGreen+2,  //T_s, T_t, T_tW
			       kRed+1,     kOrange,   kOrange+1, //Tbar_s, Tbar_t, Tbar_tW
			       //kGreen-9,  //QCD_*_MuEn
			       //kRed+1,     kOrange,   kOrange+1, //QCD_*_EMEn
			       kRed+1,     kOrange,   kOrange+1,   kOrange+1,  kOrange+1,   kOrange+1,  kOrange+1, //QCD HT bins
			       kRed+1,     kOrange,   kOrange+1,   kOrange+1,  kOrange+1, //QCD PT bins
			       kRed+1,     kOrange,   kOrange+1,   kOrange+1,  kOrange+1, //QCD PT bins
			       kRed+1,     kOrange,   kOrange+1,   kOrange+1,  kOrange+1, //QCD PT bins
			       //kRed+1,     kOrange,                                       //QCD PT bins
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9,             //
			       kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9,             //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9,             //
			       kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9,             //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9, //
			       kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure,     kAzure-1,  kAzure+10, //
			       kAzure+10,  kAzure+9             //
};


TString      sampleName;
Samples_t    sampleIndex;

treetop* treeClass;

double xSection;
double nEvents;
vector <double> sScale(2);
double systUnc;

vector <TH1D*>  Histos;

void initAll_2015(TString fileName, TString sampleName){

  //double lumi[2] = {12891.00, 12891.00}; // El, Mu
  double lumi[2] = {35867.00, 35867.00}; // El, Mu
  
  bool b_Found = false;
  for (unsigned int i = 0; i<totalSamples;++i){
    //cout << allNames[i] <<" "<< sampleName<<endl;
    if (allNames[i] == sampleName) {
      b_Found = true;
      sampleName  = allNames[i];
      sampleIndex = allSamples[i];
      break;
    }
  }
  if (not b_Found) {
    cout << "Unknown sample "<<sampleName<<endl;
    exit(1);
  }
  cout << "init tree "<<sampleIndex<<" "<<sampleName<<endl;


  double mcUnc = sqrt (0.02 * 0.02 + //Trigger
		       0.02 * 0.02 + //Lepton Efficiency
                       0.12 * 0.12); //Luminosity
  double qcdUnc = 0.40;
  double topUnc = 0.15;
  double vbUnc = 0.20;

  //Input files
  //Cross sections are from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SameSignDilepton2012
  //Numbers of events are from https://twiki.cern.ch/twiki/bin/viewauth/CMS/B2GTopLikeBSM53X
  //Taking the number of events processed is close, but imprecise because the pattuples already
  //cut on some things (like noise filtering)
  switch( sampleIndex )
    {
    case Data:	        break;

    case TTbar:	   		nEvents = 77229341.;	xSection = 831.76;   systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);     break; //nominal powheg (V2)
    case TTbar_scaleup:	   	nEvents = 29310620.;	xSection = 831.76;   systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);     break; //scaleup powheg (V2)
    case TTbar_scaledown:	nEvents = 28354188.;	xSection = 831.76;   systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);     break; //scaledown powheg (V2)

    //case ZJets:	   nEvents = 28968252.;	xSection = 6025.2;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc  );     break; // (V2)
    case ZJets:	   nEvents = 19403277.;	xSection = 6025.2;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc  );     break; // (V2)
    case WJets:	   nEvents = 1.;	xSection = 61526.7;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc  );     break; // (V2)
    case ZJets_HT100to200:	nEvents = 7856020.;	xSection = 213.5;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT200to400:	nEvents = 8691536.;	xSection = 57.41;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT400to600:	nEvents = 10008776.;	xSection = 7.194;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT600to800:	nEvents = 8292957.;	xSection = 1.727;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT800to1200:	nEvents = 2668730.;	xSection = 0.856;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT1200to2500:	nEvents = 596079.;	xSection = 0.147;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 
    case ZJets_HT2500toInf:	nEvents = 399492.;	xSection = 0.00328;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2) 

    case WJets_HT100to200:      nEvents = 39738898.;    xSection = 1627.45;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT200to400:      nEvents = 14815928.;    xSection = 435.237;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT400to600:      nEvents = 7759701.;     xSection = 59.1811;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT600to800:      nEvents = 18687480.;     xSection = 14.5805;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT800to1200:     nEvents = 6200954.;     xSection = 6.65621;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT1200to2500:    nEvents = 244532.;     xSection = 1.60809;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT2500toInf:     nEvents = 2637821.;      xSection = 0.0389136; systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
      
    case WJets_HT100to200_LF:      nEvents = 39738898.;    xSection = 1627.45;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT200to400_LF:      nEvents = 14815928.;    xSection = 435.237;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT400to600_LF:      nEvents = 7759701.;     xSection = 59.1811;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT600to800_LF:      nEvents = 18687480.;     xSection = 14.5805;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT800to1200_LF:     nEvents = 6200954.;     xSection = 6.65621;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT1200to2500_LF:    nEvents = 244532.;     xSection = 1.60809;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT2500toInf_LF:     nEvents = 2637821.;      xSection = 0.0389136; systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
      
    case WJets_HT100to200_HF:      nEvents = 39738898.;    xSection = 1627.45;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT200to400_HF:      nEvents = 14815928.;    xSection = 435.237;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT400to600_HF:      nEvents = 7759701.;     xSection = 59.1811;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT600to800_HF:      nEvents = 18687480.;     xSection = 14.5805;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT800to1200_HF:     nEvents = 6200954.;     xSection = 6.65621;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT1200to2500_HF:    nEvents = 244532.;     xSection = 1.60809;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_HT2500toInf_HF:     nEvents = 2637821.;      xSection = 0.0389136; systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)

    //case WJets_Pt100to250:      nEvents = 99945850.;    xSection = 676.3;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    //case WJets_Pt250to400:      nEvents = 12022587.;    xSection = 23.94;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    //case WJets_Pt400to600:      nEvents = 1939947.;     xSection = 3.031;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    //case WJets_Pt600toInf:      nEvents = 1974609.;     xSection = 0.4524;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt100to250:      nEvents = 36089998.;    xSection = 676.3;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt250to400:      nEvents = 4435411.;    xSection = 23.94;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt400to600:      nEvents = 740953.;     xSection = 3.031;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt600toInf:      nEvents = 794735.;     xSection = 0.4524;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)

    case WJets_Pt100to250_LF:      nEvents = 36089998.;    xSection = 676.3;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt250to400_LF:      nEvents = 4435411.;    xSection = 23.94;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt400to600_LF:      nEvents = 740953.;     xSection = 3.031;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt600toInf_LF:      nEvents = 794735.;     xSection = 0.4524;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)

    case WJets_Pt100to250_HF:      nEvents = 36089998.;    xSection = 676.3;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt250to400_HF:      nEvents = 4435411.;    xSection = 23.94;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt400to600_HF:      nEvents = 740953.;     xSection = 3.031;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WJets_Pt600toInf_HF:      nEvents = 794735.;     xSection = 0.4524;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)

    case WW:	   nEvents = 994012.;		xSection = 115.0;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case WZ:	   nEvents = 1000000.;		xSection = 47.3;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc);     break; // (V2)
    case ZZ:	   nEvents = 990064.;		xSection = 16.523;   systUnc = sqrt(mcUnc*mcUnc + vbUnc*vbUnc); break; // (V2)

    //case T_s:	   nEvents = 1000000.; 		xSection = 3.44;  systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);      break; // (V2)
    case T_s:	   nEvents = 622990.; 		xSection = 3.44;  systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);      break; // (V2)
    case T_t:	   nEvents = 67240808.; 		xSection = 45.34;   systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);      break; // (V2)
    case T_tW:	   nEvents = 6952830.;		xSection = 35.6;  systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);      break; // (V2)
      
    case Tbar_s:   nEvents = 1.; 		xSection = 10000.;    systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);    break; // (V2)
    case Tbar_t:   nEvents = 38811017.;  	xSection = 26.9833; systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);    break; // (V2)
    case Tbar_tW:  nEvents = 6933094.; 		xSection = 35.6;    systUnc = sqrt(mcUnc*mcUnc + topUnc*topUnc);    break; // (V2)

    //case QCD_Pt_20_Mu:	   nEvents = 4777926.; 	xSection = 866600000.*0.00044;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;

    //case QCD_Pt_20_30_EM:	   nEvents = 1987127.; 	xSection = 677300000.*0.007;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    //case QCD_Pt_30_80_EM:	   nEvents = 2000838.; 	xSection = 185900000.*0.056;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    //case QCD_Pt_80_170_EM:	   nEvents = 1959507.; 	xSection = 3529000.*0.158;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;

/*    case QCD_HT200to300:	   nEvents = 17891325.; 	xSection = 1735000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT300to500:	   nEvents = 20036151.; 	xSection = 366800.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT500to700:	   nEvents = 19722604.; 	xSection = 29370.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT700to1000:	   nEvents = 15416052.; 	xSection = 6524.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT1000to1500:	   nEvents = 4909636.; 		xSection = 1064.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
*/

    case QCD_Pt_5to10:	   nEvents = 1.; 		xSection = 61018300000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_10to15:	   nEvents = 1.; 		xSection = 5887580000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_15to30:	   nEvents = 1.; 		xSection = 1837410000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_30to50:	   nEvents = 1.; 		xSection = 140932000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_50to80:	   nEvents = 1.; 		xSection = 19204300.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_80to120:	   nEvents = 1.; 		xSection = 2762530.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_120to170:	   nEvents = 6708572.; 		xSection = 471100.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_170to300:	   nEvents = 6958708.; 		xSection = 117276.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_300to470:	   nEvents = 18253032.; 		xSection = 7823.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_470to600:	   nEvents = 3959986.; 		xSection = 648.2;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_600to800:	   nEvents = 9622896.; 		xSection = 186.9;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_800to1000:	   nEvents = 15704980.; 		xSection = 32.293;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_1000to1400:	   nEvents = 9981655.; 		xSection = 9.4183;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_1400to1800:	   nEvents = 1.; 		xSection = 0.84265;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_Pt_1800to2400:	   nEvents = 1.; 		xSection = 0.114943;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    //case QCD_Pt_2400to3200:	   nEvents = 198383.; 		xSection = 0.00682981;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    //case QCD_Pt_3200toInf:	   nEvents = 188696.; 		xSection = 0.000165445;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;

    case QCD_HT200to300:	   nEvents = 1.; 	xSection = 1735000.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT300to500:	   nEvents = 1.; 	xSection = 366800.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT500to700:	   nEvents = 1.; 	xSection = 29370.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT700to1000:	   nEvents = 1.; 	xSection = 6524.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT1000to1500:	   nEvents = 1.; 		xSection = 1064.;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT1500to2000:	   nEvents = 1.; 		xSection = 121.5;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;
    case QCD_HT2000toInf:	   nEvents = 1.; 		xSection = 25.42;  systUnc = sqrt(mcUnc*mcUnc + qcdUnc*qcdUnc);      break;

    case Wprime800Right:   nEvents = 1.;    xSection = 6.1959;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime900Right:   nEvents = 1.;    xSection = 3.9635;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1000Right:  nEvents = 198800.;    xSection = 2.6218;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1100Right:  nEvents = 199800.;    xSection = 1.7833;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1200Right:  nEvents = 200000.;    xSection = 1.2420;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1300Right:  nEvents = 200000.;    xSection = 0.88188;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1400Right:  nEvents = 199800.;    xSection = 0.63665;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1500Right:  nEvents = 199800.;    xSection = 0.46602;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1600Right:  nEvents = 200000.;    xSection = 0.34533;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1700Right:  nEvents = 200000.;    xSection = 0.25886;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1800Right:  nEvents = 198000.;    xSection = 0.19584;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1900Right:  nEvents = 200000.;    xSection = 0.14922;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2000Right:  nEvents = 200000.;    xSection = 0.11459;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2100Right:  nEvents = 196800.;    xSection = 0.088534;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2200Right:  nEvents = 200000.;    xSection = 0.068860;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2300Right:  nEvents = 199400.;    xSection = 0.053847;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2400Right:  nEvents = 200000.;    xSection = 0.042275;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2500Right:  nEvents = 200000.;    xSection = 0.033343;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2600Right:  nEvents = 185600.;    xSection = 0.026400;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2700Right:  nEvents = 200000.;    xSection = 0.020986;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2800Right:  nEvents = 200000.;    xSection = 0.016738;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2900Right:  nEvents = 200000.;    xSection = 0.013397;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3000Right:  nEvents = 199200.;    xSection = 0.010756;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3100Right:  nEvents = 200000.;    xSection = 0.008669;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3200Right:  nEvents = 200000.;    xSection = 0.006997;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3300Right:  nEvents = 200000.;    xSection = 0.005672;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3400Right:  nEvents = 200000.;    xSection = 0.004608;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3500Right:  nEvents = 199600.;    xSection = 0.003758;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3600Right:  nEvents = 199800.;    xSection = 0.003074;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3700Right:  nEvents = 199200.;    xSection = 0.002523;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3800Right:  nEvents = 200000.;    xSection = 0.002078;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3900Right:  nEvents = 200000.;    xSection = 0.001718;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime4000Right:  nEvents = 197600.;    xSection = 0.001425;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
	  
    case Wprime800Mix:   nEvents = 1.;    xSection = 14.029;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime900Mix:   nEvents = 1.;    xSection = 9.7876;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1000Mix:  nEvents = 399600.;    xSection = 4.8530;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1100Mix:  nEvents = 398400.;    xSection = 4.0787;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1200Mix:  nEvents = 397200.;    xSection = 3.5912;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1300Mix:  nEvents = 388775.;    xSection = 3.2735;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1400Mix:  nEvents = 393800.;    xSection = 3.0576;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1500Mix:  nEvents = 399000.;    xSection = 2.9107;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1600Mix:  nEvents = 384624.;    xSection = 2.8126;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1700Mix:  nEvents = 398768.;    xSection = 2.7431;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1800Mix:  nEvents = 399781.;    xSection = 2.6946;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1900Mix:  nEvents = 391145.;    xSection = 2.6614;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2000Mix:  nEvents = 387218.;    xSection = 2.6378;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2100Mix:  nEvents = 393834.;    xSection = 2.6209;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2200Mix:  nEvents = 400000.;    xSection = 2.6093;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2300Mix:  nEvents = 387545.;    xSection = 2.6019;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2400Mix:  nEvents = 392373.;    xSection = 2.5976;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2500Mix:  nEvents = 383142.;    xSection = 2.5948;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2600Mix:  nEvents = 395964.;    xSection = 2.5921;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2700Mix:  nEvents = 397600.;    xSection = 2.5917;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2800Mix:  nEvents = 392416.;    xSection = 2.5910;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2900Mix:  nEvents = 399206.;    xSection = 2.5918;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3000Mix:  nEvents = 396872.;    xSection = 2.5929;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3100Mix:  nEvents = 393638.;    xSection = 2.5931;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3200Mix:  nEvents = 394064.;    xSection = 2.5942;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3300Mix:  nEvents = 391573.;    xSection = 2.5977;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3400Mix:  nEvents = 400000.;    xSection = 2.5976;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3500Mix:  nEvents = 399206.;    xSection = 2.5993;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3600Mix:  nEvents = 400000.;    xSection = 2.6011;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3700Mix:  nEvents = 395672.;    xSection = 2.6024;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3800Mix:  nEvents = 395964.;    xSection = 2.6032;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3900Mix:  nEvents = 395964.;    xSection = 2.6048;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime4000Mix:  nEvents = 393638.;    xSection = 2.6066;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)


    case Wprime800Left:   nEvents = 1.;    xSection = 7.8894;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime900Left:   nEvents = 1.;    xSection = 5.8370;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1000Left:  nEvents = 400000.;    xSection = 4.6217;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1100Left:  nEvents = 393725.;    xSection = 3.8865;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1200Left:  nEvents = 390454.;    xSection = 3.4221;    systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1300Left:  nEvents = 399600.;    xSection = 3.1252;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1400Left:  nEvents = 400000.;    xSection = 2.9292;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1500Left:  nEvents = 396000.;    xSection = 2.7983;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1600Left:  nEvents = 398773.;    xSection = 2.7107;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1700Left:  nEvents = 399600.;    xSection = 2.6508;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1800Left:  nEvents = 396600.;    xSection = 2.6107;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime1900Left:  nEvents = 395768.;    xSection = 2.5848;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2000Left:  nEvents = 389186.;    xSection = 2.5677;   systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2100Left:  nEvents = 395964.;    xSection = 2.5567;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2200Left:  nEvents = 392434.;    xSection = 2.5502;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2300Left:  nEvents = 396800.;    xSection = 2.5469;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2400Left:  nEvents = 383545.;    xSection = 2.5454;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2500Left:  nEvents = 389942.;    xSection = 2.5457;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2600Left:  nEvents = 395364.;    xSection = 2.5474;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2700Left:  nEvents = 383199.;    xSection = 2.5493;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2800Left:  nEvents = 396406.;    xSection = 2.5519;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime2900Left:  nEvents = 384153.;    xSection = 2.5551;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3000Left:  nEvents = 397272.;    xSection = 2.5578;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3100Left:  nEvents = 396006.;    xSection = 2.5620;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3200Left:  nEvents = 395164.;    xSection = 2.5650;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3300Left:  nEvents = 392416.;    xSection = 2.5672;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3400Left:  nEvents = 394623.;    xSection = 2.5704;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3500Left:  nEvents = 395964.;    xSection = 2.5733;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3600Left:  nEvents = 382474.;    xSection = 2.5764;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3700Left:  nEvents = 382342.;    xSection = 2.5790;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3800Left:  nEvents = 389186.;    xSection = 2.5813;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime3900Left:  nEvents = 388143.;    xSection = 2.5837;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)
    case Wprime4000Left:  nEvents = 385674.;    xSection = 2.5861;  systUnc = sqrt(mcUnc*mcUnc); break; // (V2)


    default:	cout <<"Sample "<<sampleIndex<<" not listed for 2016\n";exit(1);
    }

  fexists(fileName.Data(), true);
  treeClass = new treetop(fileName);
  
  if   (sampleIndex == Data ){
    sScale[0] = 1.0;
    sScale[1] = 1.0;
  }
  else{
    sScale[0] = xSection * lumi[0] / nEvents;
    sScale[1] = xSection * lumi[1] / nEvents;
  }
}

#endif
