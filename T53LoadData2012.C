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
#include "Objects/interface/T53Event.h"
#include "Objects/interface/FileExists.h"

using namespace std;

//Global variables

enum Samples_t {Data,        ChargeMisID, FakeRate,
		TTbar,       ZJets,       WJets,     
		WZ,          ZZ,      
		WWm,         WWp,         WWW,  
		TTW,         TTWW,        TTZ,
		T53m350,     T53m450,
		T53m550,     T53m600,     T53m650,    T53m700,    T53m750,
		T53m800,     T53m850,     T53m900,    T53m950,    T53m1000
};

const unsigned int totalSamples = 26;

Samples_t allSamples[totalSamples] = {Data,    ChargeMisID, FakeRate,
				      TTbar,   ZJets,       WJets,     
				      WZ,      ZZ,      
				      WWm,     WWp,     WWW,  
				      TTW,     TTWW,    TTZ,
				      T53m350,     T53m450,
				      T53m550,     T53m600,     T53m650,    T53m700,    T53m750,
				      T53m800,     T53m850,     T53m900,    T53m950,    T53m1000
};


TString allNames[totalSamples] = {"Data",    "ChargeMisID", "FakeRate",
				  "TTbar",   "ZJets",       "WJets",     
				  "WZ",      "ZZ", 
				  "WWm",     "WWp",     "WWW",  
				  "TTW",     "TTWW",    "TTZ",
				  "T53m350",     "T53m450",
				  "T53m550",     "T53m600",     "T53m650",    "T53m700",    "T53m750",
				  "T53m800",     "T53m850",     "T53m900",    "T53m950",    "T53m1000"
};
   
Color_t color[totalSamples] = {kBlack,     kBlack,   kBlack,    //Data, ChargeMisID, FakeRate
			       kBlack,     kBlack,   kBlack,    //TTbar, WJets and ZJets -- they should not be in plots
			       kViolet+1,  kAzure-2,            //WZ, ZZ       
			       kGreen-9,   kGreen-9, kGreen+2,  //WWm, WWp, WWW
			       kRed+1,     kOrange,  kOrange+1, //TTW, TTWW, TTZ
			       kAzure,     kAzure-1,
			       kAzure+10,  kAzure+9, kAzure+8, kAzure+7, kAzure+6,  
			       kAzure+5,   kAzure+4, kAzure+3, kAzure+2, kAzure+1
};


TString      sampleName;
Samples_t    sampleIndex;

treetop* treeClass;

double xSection;
double nEvents;
vector <double> sScale(3);
double systUnc;

vector <TH1D*>  Histos;

void initAll_2012(TString fileName, TString sampleName){

  double lumi[3] = {19620., 19620., 19620.}; // MuMu, ElMu, ElEl
  
  bool b_Found = false;
  for (unsigned int i = 0; i<totalSamples;++i){
    cout << allNames[i] <<" "<< sampleName<<endl;
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


  double mcUnc = sqrt (2 * 0.006 * 0.006 + //Trigger
		       2 * 0.03  * 0.03);  //Lepton Efficiency
  
  //Input files
  //Cross sections are from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SameSignDilepton2012
  //Numbers of events are from https://twiki.cern.ch/twiki/bin/viewauth/CMS/B2GTopLikeBSM53X
  //Taking the number of events processed is close, but imprecise because the pattuples already
  //cut on some things (like noise filtering)
  switch( sampleIndex )
    {
    case Data:	        break;
    case ChargeMisID:	break;
    case FakeRate:	break;

    case TTbar:	   nEvents = 6923750.;	xSection = 225.197;   systUnc = sqrt(mcUnc*mcUnc + 0.05*0.05  + 0.018*0.018 + 0.17*0.17);     break; 
    case ZJets:	   nEvents = 30459503.;	xSection = 3503.71;   systUnc = sqrt(mcUnc*mcUnc + 0.05*0.05  + 0.018*0.018 + 0.17*0.17);     break; 
    case WJets:	   nEvents = 18331570.;	xSection = 37509.0;   systUnc = sqrt(mcUnc*mcUnc + 0.05*0.05  + 0.018*0.018 + 0.17*0.17);     break; 
      
    //Uncertainties need to be adjusted for 2012!
    case WZ:	   nEvents = 2017979.;	xSection = 1.0575;   systUnc = sqrt(mcUnc*mcUnc + 0.05*0.05  + 0.018*0.018 + 0.17*0.17);     break; 
    case ZZ:	   nEvents = 4807893.;	xSection = 0.1769;   systUnc = sqrt(mcUnc*mcUnc + 0.011*0.011 + 0.006*0.006 + 0.075* 0.075); break; 
    case WWm:	   nEvents = 96392.; 	xSection = 0.08888;  systUnc = sqrt(mcUnc*mcUnc + 0.045*0.045 + 0.024*0.024 + 0.5*0.5);      break;
    case WWp:	   nEvents = 99985.; 	xSection = 0.2482;   systUnc = sqrt(mcUnc*mcUnc + 0.045*0.045 + 0.024*0.024 + 0.5*0.5);      break;
    case WWW:	   nEvents = 220549.;	xSection = 0.08217;  systUnc = sqrt(mcUnc*mcUnc + 0.037*0.037 + 0.005*0.005 + 0.5*0.5);      break;
      
    case TTW:	   nEvents = 196046.; 	xSection = 0.232;    systUnc = sqrt(mcUnc*mcUnc + 0.034*0.034 + 0.0094*0.0094 + 0.5*0.5);    break;
    case TTWW:	   nEvents = 217820.;  	xSection = 0.002037; systUnc = sqrt(mcUnc*mcUnc + 0.037*0.037 + 0.0025*0.0025 + 0.5*0.5);    break;
    case TTZ:	   nEvents = 210160.; 	xSection = 0.208;    systUnc = sqrt(mcUnc*mcUnc + 0.034*0.034 + 0.0094*0.0094 + 0.5*0.5);    break;

    case T53m350:  nEvents = 82498.;    xSection = 5.086   * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m450:  nEvents = 81853.;    xSection = 1.112   * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m550:  nEvents = 76552.;    xSection = 0.305   * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m600:  nEvents = 72779.;    xSection = 0.170   * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m650:  nEvents = 76442.;    xSection = 0.097   * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m700:  nEvents = 76704.;    xSection = 0.0569  * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m750:  nEvents = 76413.;    xSection = 0.0341  * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;

    case T53m800:  nEvents = 75069.;    xSection = 0.0208  * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m850:  nEvents = 76020.;    xSection = 0.0129  * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m900:  nEvents = 74479.;    xSection = 0.00809 * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m950:  nEvents = 75235.;    xSection = 0.00513 * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
    case T53m1000: nEvents = 75045.;    xSection = 0.00327 * 0.21;  systUnc = sqrt(mcUnc*mcUnc); break;
	  
    default:	cout <<"Sample "<<sampleIndex<<" not listed for 2012\n";exit(1);
    }

  fexists(fileName.Data(), true);
  treeClass = new treetop(fileName);
  
  if   (sampleIndex == Data || sampleIndex == FakeRate || sampleIndex == ChargeMisID){
    sScale[0] = 1.0;
    sScale[1] = 1.0;
    sScale[2] = 1.0;
  }
  else{
    sScale[0] = xSection * lumi[0] / nEvents;
    sScale[1] = xSection * lumi[1] / nEvents;
    sScale[2] = xSection * lumi[2] / nEvents;
  }
}

#endif
