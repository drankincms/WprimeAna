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

const unsigned int NHISTOS    = 1;

enum Histos_t {mWprime         //29
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

    if (teve->lvWprime . Pt() > 0) {
      myHistos.at(mWprime)  ->Fill(teve->lvWprime.M(),   teve->weight);
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

    vhd.at(mWprime)       = new HistoData("mWprime",     4000,   0.,  4000.);

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
