#ifndef StandardHistos_Slim_cxx
#define StandardHistos_Slim_cxx

#include "TLorentzVector.h"
#include "TVector2.h"
#include <TMath.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>

#include <vector>
#include <algorithm>

#include "Objects/interface/WprimeEvent.h"

const unsigned int NHISTOS    = 4;

enum Histos_t {LepPt,                       //3
	       mWprime,         //29
               mWprime_Limits,                 //30
               hnPV
};

class HistoData{

public:

  TString histoName;
  int     nBins;
  double  min;
  double  max;
  double *binArr;
  bool useArr= false;

  HistoData();

  HistoData(TString hn, int nb, double mn, double mx){
    histoName = hn;
    nBins     = nb;
    min       = mn;
    max       = mx;
  }

  HistoData(TString hn, int nb, double *ba, bool ua){
    histoName = hn;
    nBins     = nb;
    binArr    = ba;
    useArr    = ua;
  }

};

class StandardHistos{
  
public:
  
  StandardHistos(const TString & aName, const TString & aSuffix, const TString & sampleName, const double scale) :
	hname(aName), suffix(aSuffix), sampleName_(sampleName), scale_(scale)
  {
    initAllHistos();
  }

  void fillVHD(WprimeEvent* teve, vector <TH1F*> myHistos){

    myHistos.at(hnPV)    ->Fill(teve->nPV,      teve->weight);
    myHistos.at(LepPt) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    if (teve->lvWprime . Pt() > 0) {
      myHistos.at(mWprime)  ->Fill(teve->lvWprime.M(),   teve->weight);
      myHistos.at(mWprime_Limits)  ->Fill(teve->lvWprime.M(),   teve->lim_weight);
    }

  }

  void fillAllHistos(WprimeEvent* teve){
    fillVHD(teve, goodHistos);
  }

  void scaleAllHistos(){
    for (unsigned int ih = 0; ih < NHISTOS; ih++){
      goodHistos.at(ih)->Scale(scale_);
    }
  }

private:
  TString hname;
  TString suffix;
  TString sampleName_;
  double scale_;
  vector <TH1F*> goodHistos;

  void initAllHistos(){

    vector <HistoData*> vhd(NHISTOS);

    vhd.at(hnPV)          = new HistoData("nPV",            100, -0.5,  99.5);
    vhd.at(LepPt)         = new HistoData("LepPt",         1200,  0.,  1200.);
    vhd.at(mWprime)       = new HistoData("mWprime",     5000,   0.,  5000.);
    // What was used for 8 TeV analysis
    //double xlow  = [ 100, 300,  400, 500, 600, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 4000]

    // Total guess for 13 TeV testing
    //double xlow[23]  = { 100, 300, 500, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2400, 2600, 2800, 4000 };

    // Using binning script
    //double xlow[27]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,2900.0,4000.0};
    double xlow[27]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,3000.,3600.0,4000.0,5000.0};
    vhd.at(mWprime_Limits)= new HistoData("mWprime_Limits",     26,   xlow,  true);

    if (hname != "") hname = "_"+hname;
    
    goodHistos.resize(NHISTOS);
    
    for (unsigned int ih = 0; ih < NHISTOS; ih++){
      if (!vhd.at(ih)->useArr) goodHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, 
				   vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      else goodHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->nBins, vhd.at(ih)->binArr);
      goodHistos.at(ih)->Sumw2();
    }

  }

};

class StandardHistosLimits{
  
public:
  
  StandardHistosLimits(const TString & aName, const TString & aSuffix, const TString & sampleName, const double scale) :
	hname(aName), suffix(aSuffix), sampleName_(sampleName), scale_(scale)
  {
    initAllHistosLimits();
  }

  void fillVHDLimits(WprimeEvent* teve, vector <TH1F*> myHistos){
    myHistos.at(0)  ->Fill(teve->lvWprime.M(),   teve->lim_weight);
  }

  void fillAllHistosLimits(WprimeEvent* teve){
    fillVHDLimits(teve, goodHistos);
  }

  void scaleAllHistosLimits(){
    goodHistos.at(0)->Scale(scale_);
  }

private:
  TString hname;
  TString suffix;
  TString sampleName_;
  double scale_;
  vector <TH1F*> goodHistos;

  void initAllHistosLimits(){

    vector <HistoData*> vhd(1);
    // What was used for 8 TeV analysis
    //double xlow  = [ 100, 300,  400, 500, 600, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 4000]

    // Total guess for 13 TeV optimization
    double xlow[24]  = { 100, 300, 500, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2400, 2600, 2800, 4000, 5000 };

    // Using binning script
    //double xlow[25]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,2900.0,4000.0};
    vhd.at(0)= new HistoData("mWprime_Limits",     23,   xlow,  true);
    
    if (hname != "") hname = "_"+hname;
    
    goodHistos.resize(1);

    goodHistos.at(0) = new TH1F(vhd.at(0)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(0)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(0)->nBins, vhd.at(0)->binArr);

    goodHistos.at(0)->Sumw2();

  }

};

void initHistoDirTwoChannels(TFile* outFile, vector <StandardHistos*> & stanHist, TString dirName, TString sampleName, vector <double> sScale){

  if (stanHist.size() != 2){
    cout<<"Number of channels is not 2!";
    return;
  }

  outFile->cd();
  TDirectory* topdir = outFile->mkdir(dirName);
  TDirectory* muDir  = topdir->mkdir("Mu");
  TDirectory* elDir  = topdir->mkdir("El");

  muDir->cd(); 
  stanHist[0] = new StandardHistos(dirName, "Mu", sampleName, sScale[0]);

  elDir->cd();
  stanHist[1] = new StandardHistos(dirName, "El", sampleName, sScale[1]); 
}

void fillHistosTwoChannels(vector <StandardHistos*> & stanHist, WprimeEvent* teve){
  if (teve->suffix == "Mu") stanHist[0]->fillAllHistos(teve);
  if (teve->suffix == "El") stanHist[1]->fillAllHistos(teve);
}

void scaleHistosTwoChannels(vector <StandardHistos*> & stanHist){
  stanHist[0]->scaleAllHistos();
  stanHist[1]->scaleAllHistos();
}

void initHistoLimitsDirTwoChannels(TFile* outFile, vector <StandardHistosLimits*> & stanHist, TString dirName, TString sampleName, vector <double> sScale){

  if (stanHist.size() != 2){
    cout<<"Number of channels is not 2!";
    return;
  }

  outFile->cd();
  TDirectory* topdir = outFile->mkdir(dirName);
  TDirectory* muDir  = topdir->mkdir("Mu");
  TDirectory* elDir  = topdir->mkdir("El");

  muDir->cd(); 
  stanHist[0] = new StandardHistosLimits(dirName, "Mu", sampleName, sScale[0]);

  elDir->cd();
  stanHist[1] = new StandardHistosLimits(dirName, "El", sampleName, sScale[1]); 
}

void fillHistosLimitsTwoChannels(vector <StandardHistosLimits*> & stanHist, WprimeEvent* teve){
  if (teve->suffix == "Mu") stanHist[0]->fillAllHistosLimits(teve);
  if (teve->suffix == "El") stanHist[1]->fillAllHistosLimits(teve);
}

void scaleHistosLimitsTwoChannels(vector <StandardHistosLimits*> & stanHist){
  stanHist[0]->scaleAllHistosLimits();
  stanHist[1]->scaleAllHistosLimits();
}


#endif
