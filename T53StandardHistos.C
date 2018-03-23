#ifndef StandardHistos_cxx
#define StandardHistos_cxx

#include "TLorentzVector.h"
#include "TVector2.h"
#include <TMath.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>

#include <vector>
#include <algorithm>

#include "Objects/interface/T53Event.h"

const unsigned int NHISTOS    = 52;

enum Histos_t {Lep1Pt,       Lep2Pt,                       //2
	       ElPt,         MuPt,                         //4
	       ElEta,        MuEta,                        //6
	       CA8Jet1Pt,    CA8Jet2Pt,   nCA8Jets,        //9
	       CAWJet1Pt,    CAWJet2Pt,   nCAWJets,        //12
	       CATopJet1Pt,  CATopJet2Pt, nCATopJets,      //15  
 	       AK5Jet1Pt,    AK5Jet2Pt,   nAK5Jets,        //18
	       nTotJets,     HT,                           //20
	       LepInvM,      LepdR,                        //22
	       MinLepAK5dR,  MinLepCAWdR, MinLepCATopdR,   //25
	       Lep1Iso,      Lep2Iso,                      //27
	       ElIso,        MuIso,                        //29
	       mW,           mTop,        mT53,            //33
	       hnPV,         hnEvents,                     //34
	       hPUwgt,       MET,                          //36
	       nCATopDau,    nSelCATopDau,                 //38
	       CAWMass,      SelCAWMass,                   //40
	       CATopMass,    SelCATopMass,                 //42
	       CATopMinMass, SelCATopMinMass,              //44
	       nOnlyAK5Jets, OnlyAK5Jet1Pt, OnlyAK5Jet2Pt, //47
	       OnlyAK5HT,                                  //48
	       nLeptons,     nJoL,                         //50
	       LepCharge,    nBTags                        //52
};

const unsigned int NHISTOS_FR = 10;

enum Histos_FR_t {Lep1Pt_FR, Lep1Eta_FR, Jet1Pt_FR, nJets_FR, nEvents_FR, 
		  nPV_FR,    mJL_FR,    dRJL_FR,  MET_FR,
		  MT_FR
};

//Fake rates
//Data
// const double PR_MU = 1.0;
const double PR_MU = 0.937;
const double FR_MU = 0.340;

// const double PR_EL = 1.0;
const double PR_EL = 0.869;
const double FR_EL = 0.251;

//TTbar
// const double PR_MU = 0.949;
// const double FR_MU = 0.221;

// const double PR_EL = 0.910;
// const double FR_EL = 0.097;

class HistoData{

public:

  TString histoName;
  int     nBins;
  double  min;
  double  max;

  HistoData();

  HistoData(TString hn, int nb, double mn, double mx){
    histoName = hn;
    nBins     = nb;
    min       = mn;
    max       = mx;
  }

};

class StandardHistos{
  
public:
  
  StandardHistos(const TString & aName, const TString & aSuffix, const TString & sampleName, const double scale, bool doFakes=false) :
	hname(aName), suffix(aSuffix), sampleName_(sampleName), scale_(scale)
  {
    initAllHistos(sampleName == "FakeRate" or doFakes);
  }
  void fillVHD(T53Event* teve, vector <TH1F*> myHistos){

    myHistos.at(hnEvents)->Fill(1, teve->weight);

    myHistos.at(hnPV)    ->Fill(teve->nPV,      teve->weight);
    myHistos.at(hPUwgt)  ->Fill(teve->PUweight);

    myHistos.at(Lep1Pt) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    myHistos.at(Lep2Pt) ->Fill(teve->lepton2->lv.Pt(), teve->weight);

    myHistos.at(Lep1Iso)->Fill(teve->lepton1->relIso, teve->weight);
    myHistos.at(Lep2Iso)->Fill(teve->lepton2->relIso, teve->weight);

    myHistos.at(LepCharge)->Fill(teve->lepton1->charge, teve->weight);
    myHistos.at(LepCharge)->Fill(teve->lepton2->charge, teve->weight);

    //Fill histos split by lepton flavor
    if (teve->lepton1->isMuon()){
      myHistos.at(MuPt) ->Fill(teve->lepton1->lv.Pt(),  teve->weight);
      myHistos.at(MuEta)->Fill(teve->lepton1->lv.Eta(), teve->weight);
      myHistos.at(MuIso)->Fill(teve->lepton1->relIso, teve->weight);
    }
    else{
      myHistos.at(ElPt) ->Fill(teve->lepton1->lv.Pt(),  teve->weight);
      myHistos.at(ElEta)->Fill(teve->lepton1->lv.Eta(), teve->weight);
      myHistos.at(ElIso)->Fill(teve->lepton1->relIso, teve->weight);
    }
    if (teve->lepton2->isMuon()){
      myHistos.at(MuPt) ->Fill(teve->lepton2->lv.Pt(),  teve->weight);
      myHistos.at(MuEta)->Fill(teve->lepton2->lv.Eta(), teve->weight);
      myHistos.at(MuIso)->Fill(teve->lepton2->relIso, teve->weight);
    }
    else{
      myHistos.at(ElPt) ->Fill(teve->lepton2->lv.Pt(),  teve->weight);
      myHistos.at(ElEta)->Fill(teve->lepton2->lv.Eta(), teve->weight);
      myHistos.at(ElIso)->Fill(teve->lepton2->relIso, teve->weight);
    }

    if (teve->vGoodAK5Jets.size() > 0)  myHistos.at(AK5Jet1Pt)->Fill(teve->vGoodAK5Jets.at(0)->lv.Pt(), teve->weight);
    if (teve->vGoodAK5Jets.size() > 1)  myHistos.at(AK5Jet2Pt)->Fill(teve->vGoodAK5Jets.at(1)->lv.Pt(), teve->weight); 
    myHistos.at(nAK5Jets)->Fill(teve->vGoodAK5Jets.size(), teve->weight);
  
    if (teve->vGoodCA8Jets.size() > 0)  myHistos.at(CA8Jet1Pt)->Fill(teve->vGoodCA8Jets.at(0)->lv.Pt(), teve->weight);
    if (teve->vGoodCA8Jets.size() > 1)  myHistos.at(CA8Jet2Pt)->Fill(teve->vGoodCA8Jets.at(1)->lv.Pt(), teve->weight); 
    myHistos.at(nCA8Jets)->Fill(teve->vGoodCA8Jets.size(), teve->weight);

    if (teve->vGoodCAWJets.size() > 0)  myHistos.at(CAWJet1Pt)->Fill(teve->vGoodCAWJets.at(0)->lv.Pt(), teve->weight);
    if (teve->vGoodCAWJets.size() > 1)  myHistos.at(CAWJet2Pt)->Fill(teve->vGoodCAWJets.at(1)->lv.Pt(), teve->weight); 
    myHistos.at(nCAWJets)->Fill(teve->vGoodCAWJets.size(), teve->weight);

    if (teve->vGoodCATopJets.size() > 0)  myHistos.at(CATopJet1Pt)->Fill(teve->vGoodCATopJets.at(0)->lv.Pt(), teve->weight);
    if (teve->vGoodCATopJets.size() > 1)  myHistos.at(CATopJet2Pt)->Fill(teve->vGoodCATopJets.at(1)->lv.Pt(), teve->weight); 
    myHistos.at(nCATopJets)->Fill(teve->vGoodCATopJets.size(), teve->weight);

    if (teve->vOnlyGoodAK5Jets.size() > 0)  myHistos.at(OnlyAK5Jet1Pt)->Fill(teve->vOnlyGoodAK5Jets.at(0)->lv.Pt(), teve->weight);
    if (teve->vOnlyGoodAK5Jets.size() > 1)  myHistos.at(OnlyAK5Jet2Pt)->Fill(teve->vOnlyGoodAK5Jets.at(1)->lv.Pt(), teve->weight); 
    myHistos.at(nOnlyAK5Jets)->Fill(teve->vOnlyGoodAK5Jets.size(), teve->weight);

    int nJets = 3*teve->vGoodCATopJets.size() + 2*teve->vGoodCAWJets.size() + teve->vGoodAK5Jets.size();
    myHistos.at(nTotJets)->Fill(nJets, teve->weight);
    myHistos.at(HT)      ->Fill(teve->ht, teve->weight);

    myHistos.at(OnlyAK5HT)->Fill(teve->htAK5, teve->weight);

    myHistos.at(LepInvM)->Fill(teve->leptonSum.M(), teve->weight);
    myHistos.at(LepdR)->Fill(teve->lepton1->lv.DeltaR(teve->lepton2->lv), teve->weight);

    double mindR = 1000;
    for (unsigned int ui = 0; ui < teve->vGoodAK5Jets.size(); ui++){
      if (teve->vGoodAK5Jets.at(ui)->lv.DeltaR(teve->lepton1->lv) < mindR) mindR = teve->vGoodAK5Jets.at(ui)->lv.DeltaR(teve->lepton1->lv);
      if (teve->vGoodAK5Jets.at(ui)->lv.DeltaR(teve->lepton2->lv) < mindR) mindR = teve->vGoodAK5Jets.at(ui)->lv.DeltaR(teve->lepton2->lv);
    }    
    if (mindR < 100) myHistos.at(MinLepAK5dR)->Fill(mindR, teve->weight);

    mindR = 1000;
    for (unsigned int ui = 0; ui < teve->vGoodCAWJets.size(); ui++){
      if (teve->vGoodCAWJets.at(ui)->lv.DeltaR(teve->lepton1->lv) < mindR) mindR = teve->vGoodCAWJets.at(ui)->lv.DeltaR(teve->lepton1->lv);
      if (teve->vGoodCAWJets.at(ui)->lv.DeltaR(teve->lepton2->lv) < mindR) mindR = teve->vGoodCAWJets.at(ui)->lv.DeltaR(teve->lepton2->lv);

      myHistos.at(SelCAWMass)->Fill(teve->vGoodCAWJets.at(ui)->lv.M(), teve->weight);
    }   
    if (mindR < 100) myHistos.at(MinLepCAWdR)->Fill(mindR, teve->weight);

    mindR = 1000;
    for (unsigned int ui = 0; ui < teve->vGoodCATopJets.size(); ui++){
      if (teve->vGoodCATopJets.at(ui)->lv.DeltaR(teve->lepton1->lv) < mindR) mindR = teve->vGoodCATopJets.at(ui)->lv.DeltaR(teve->lepton1->lv);
      if (teve->vGoodCATopJets.at(ui)->lv.DeltaR(teve->lepton2->lv) < mindR) mindR = teve->vGoodCATopJets.at(ui)->lv.DeltaR(teve->lepton2->lv);
      
      myHistos.at(nSelCATopDau)   ->Fill(teve->vGoodCATopJets.at(ui)->nDaughters, teve->weight);
      myHistos.at(SelCATopMass)   ->Fill(teve->vGoodCATopJets.at(ui)->lv.M(), teve->weight);
      myHistos.at(SelCATopMinMass)->Fill(teve->vGoodCATopJets.at(ui)->minPairMass, teve->weight);
    }   
    if (mindR < 100) myHistos.at(MinLepCATopdR)->Fill(mindR, teve->weight);

//     if (teve->nCA8inCATop >= 0) myHistos.at(nCA8inCATop)->Fill(teve->nCA8inCATop, teve->weight);
//     if (teve->nCA8inCAW   >= 0) myHistos.at(nCA8inCAW)  ->Fill(teve->nCA8inCAW,   teve->weight);
    
    if (teve->lvW   . Pt() > 1) myHistos.at(mW)  ->Fill(teve->lvW.M(),   teve->weight);
    if (teve->lvTop . Pt() > 1) myHistos.at(mTop)->Fill(teve->lvTop.M(), teve->weight);
    if (teve->lvT53 . Pt() > 0) myHistos.at(mT53)->Fill(teve->lvT53.M(), teve->weight);

    myHistos.at(MET)->Fill(teve->met, teve->weight);

    for (unsigned int ui = 0; ui < teve->vAllCATopJets.size(); ui++){
      myHistos.at(nCATopDau)   ->Fill(teve->vAllCATopJets.at(ui)->nDaughters, teve->weight);
      myHistos.at(CATopMass)   ->Fill(teve->vAllCATopJets.at(ui)->lv.M(), teve->weight);
      myHistos.at(CATopMinMass)->Fill(teve->vAllCATopJets.at(ui)->minPairMass, teve->weight);     
    }
    for (unsigned int ui = 0; ui < teve->vAllCAWJets.size(); ui++){
      myHistos.at(CAWMass)->Fill(teve->vAllCAWJets.at(ui)->lv.M(), teve->weight);
    }

    myHistos.at(nLeptons)->Fill(teve->theLeptons.size(), teve->weight);
    myHistos.at(nJoL)->Fill(teve->theLeptons.size() + nJets - 2, teve->weight);
    
    myHistos.at(nBTags)->Fill(teve->nBTags, teve->weight);

  }

  void fillAllHistos(T53Event* teve, TString type="", bool doFakes=false){
    if (not doFakes) fillVHD(teve, goodHistos);
    else{
      if (type == "Nt00") fillVHD(teve, Nt00Histos);
      if (type == "Nt01") fillVHD(teve, Nt01Histos);
      if (type == "Nt10") fillVHD(teve, Nt10Histos);
      if (type == "Nt11") fillVHD(teve, Nt11Histos);
    }
  }

  void scaleAllHistos(bool doFakes=false){
    for (unsigned int ih = 0; ih < NHISTOS; ih++){
      goodHistos.at(ih)->Scale(scale_);

      if (doFakes){
	Nt00Histos.at(ih)->Scale(scale_);
	Nt01Histos.at(ih)->Scale(scale_);
	Nt10Histos.at(ih)->Scale(scale_);
	Nt11Histos.at(ih)->Scale(scale_);
      }
    }
  }

  void getTotFakes(double f1, double f2, double p1, double p2){
    for (unsigned int ih = 0; ih < NHISTOS; ih++){

      double det = 1.0 / ((p1 - f1)*(p2 - f2)); 

      TH1F* hNff = (TH1F*)Nt00Histos.at(ih)->Clone("hNff");
      hNff->Scale(p1*p2);
      hNff->Add(Nt10Histos.at(ih), -1*(1-p1)*p2);
      hNff->Add(Nt01Histos.at(ih), -1*p1*(1-p2));
      hNff->Add(Nt11Histos.at(ih), (1-p1)*(1-p2));
      hNff->Scale(det*f1*f2);

      TH1F* hNpf = (TH1F*)Nt00Histos.at(ih)->Clone("hNpf");
      hNpf->Scale(-1*f1*p2);
      hNpf->Add(Nt10Histos.at(ih), (1-f1)*p2);
      hNpf->Add(Nt01Histos.at(ih), f1*(1-p2));
      hNpf->Add(Nt11Histos.at(ih), -1*(1-f1)*(1-p2));
      hNpf->Scale(det*p1*f2);

      TH1F* hNfp = (TH1F*)Nt00Histos.at(ih)->Clone("hNfp");
      hNfp->Scale(-1*p1*f2);
      hNfp->Add(Nt10Histos.at(ih), (1-p1)*f2);
      hNfp->Add(Nt01Histos.at(ih), p1*(1-f2));
      hNfp->Add(Nt11Histos.at(ih), -1*(1-p1)*(1-f2));
      hNfp->Scale(det*f1*p2);

      goodHistos.at(ih)->Add(hNff);
      goodHistos.at(ih)->Add(hNpf);
      goodHistos.at(ih)->Add(hNfp);

      delete hNff;
      delete hNpf;
      delete hNfp;
    }
  }

private:
  TString hname;
  TString suffix;
  TString sampleName_;
  double scale_;
  vector <TH1F*> goodHistos;
  vector <TH1F*> Nt00Histos;
  vector <TH1F*> Nt01Histos;
  vector <TH1F*> Nt10Histos;
  vector <TH1F*> Nt11Histos;
  void initAllHistos(bool doFakes=false){

    vector <HistoData*> vhd(NHISTOS);
    vhd.at(hnEvents)      = new HistoData("nEvents",         1,  0.5,  1.5);

    vhd.at(hnPV)          = new HistoData("nPV",            50, -0.5,  49.5);
    vhd.at(hPUwgt)        = new HistoData("PUwght",         50,  0.,  3);

    vhd.at(Lep1Pt)        = new HistoData("Lep1Pt",        480,  0.,  480.);
    vhd.at(Lep2Pt)        = new HistoData("Lep2Pt",        480,  0.,  480.);

    vhd.at(LepCharge)     = new HistoData("LepCharge",     3,   -1.5, 1.5);

    vhd.at(ElPt)          = new HistoData("ElPt",          240,  0.,  240.);
    vhd.at(MuPt)          = new HistoData("MuPt",          240,  0.,  240.);

    vhd.at(ElEta)         = new HistoData("ElEta",          60, -2.4, 2.4);
    vhd.at(MuEta)         = new HistoData("MuEta",          60, -2.4, 2.4);

    vhd.at(AK5Jet1Pt)     = new HistoData("AK5Jet1Pt",     480,  0.,  480.);
    vhd.at(AK5Jet2Pt)     = new HistoData("AK5Jet2Pt",     480,  0.,  480.);
    vhd.at(nAK5Jets)      = new HistoData("nAK5Jets",       9,  -0.5, 8.5);

    vhd.at(CA8Jet1Pt)     = new HistoData("CA8Jet1Pt",     480,  0.,  480.);
    vhd.at(CA8Jet2Pt)     = new HistoData("CA8Jet2Pt",     480,  0.,  480.);
    vhd.at(nCA8Jets)      = new HistoData("nCA8Jets",       9,  -0.5, 8.5);

    vhd.at(CAWJet1Pt)     = new HistoData("CAWJet1Pt",     480,  0.,  960.);
    vhd.at(CAWJet2Pt)     = new HistoData("CAWJet2Pt",     480,  0.,  960.);
    vhd.at(nCAWJets)      = new HistoData("nCAWJets",       9,  -0.5, 8.5);

    vhd.at(CATopJet1Pt)   = new HistoData("CATopJet1Pt",   480,  0.,  960.);
    vhd.at(CATopJet2Pt)   = new HistoData("CATopJet2Pt",   480,  0.,  960.);
    vhd.at(nCATopJets)    = new HistoData("nCATopJets",     9,  -0.5, 8.5);

    vhd.at(OnlyAK5Jet1Pt) = new HistoData("OnlyAK5Jet1Pt", 480,  0.,  480.);
    vhd.at(OnlyAK5Jet2Pt) = new HistoData("OnlyAK5Jet2Pt", 480,  0.,  480.);
    vhd.at(nOnlyAK5Jets)  = new HistoData("nOnlyAK5Jets",   9,  -0.5, 8.5);

    vhd.at(nTotJets)      = new HistoData("nTotJets",       9,  -0.5, 8.5);
    vhd.at(HT)            = new HistoData("HT",           2000,  0.,  2000.);

    vhd.at(OnlyAK5HT)     = new HistoData("OnlyAK5HT",    2000,  0.,  2000.);

    vhd.at(LepInvM)       = new HistoData("LepInvM",       240,  0.,  240.);
    vhd.at(LepdR)         = new HistoData("LepdR",         100,  0.,  5.);

    vhd.at(MinLepCATopdR) = new HistoData("MinLepCATopdR", 100,  0.,  5.);
    vhd.at(MinLepCAWdR)   = new HistoData("MinLepCAWdR",   100,  0.,  5.);
    vhd.at(MinLepAK5dR)   = new HistoData("MinLepAK5dR",   100,  0.,  5.);

//     vhd.at(nCA8inCATop)   = new HistoData("nCA8inCATop",    4,  -0.5, 3.5);
//     vhd.at(nCA8inCAW)     = new HistoData("nCA8inCAW",      4,  -0.5, 3.5);

    vhd.at(Lep1Iso)       = new HistoData("Lep1Iso",       100,  0.,  0.2);
    vhd.at(Lep2Iso)       = new HistoData("Lep2Iso",       100,  0.,  0.2);

    vhd.at(ElIso)         = new HistoData("ElIso",         100,  0.,  0.15);
    vhd.at(MuIso)         = new HistoData("MuIso",         100,  0.,  0.2);

    vhd.at(mW)            = new HistoData("mW",            60,  50.,  130.);
    vhd.at(mTop)          = new HistoData("mTop",          80, 135.,  215.);
    vhd.at(mT53)          = new HistoData("mT53",        1800,   0.,  1800.);

    vhd.at(MET)           = new HistoData("MET",          400,   0.,  400.);

    vhd.at(nLeptons)      = new HistoData("nLeptons",      4,   1.5, 5.5);
    vhd.at(nJoL)          = new HistoData("nJoL",         10,  -0.5, 9.5);

    //Cross-check variables for Freya
    vhd.at(nCATopDau)       = new HistoData("nCATopDau",         8,  -0.5,  7.5);
    vhd.at(CATopMass)       = new HistoData("CATopMass",       240,   0.,  240.);
    vhd.at(CATopMinMass)    = new HistoData("CATopMinMass",    240,   0.,  240.);
    vhd.at(CAWMass)         = new HistoData("CAWMass",         240,   0.,  240.);

    vhd.at(nSelCATopDau)    = new HistoData("nSelCATopDau",      8,  -0.5,  7.5);
    vhd.at(SelCATopMass)    = new HistoData("SelCATopMass",    240,   0.,  240.);
    vhd.at(SelCATopMinMass) = new HistoData("SelCATopMinMass", 240,   0.,  240.);
    vhd.at(SelCAWMass)      = new HistoData("SelCAWMass",      240,   0.,  240.);
    
    vhd.at(nBTags)          = new HistoData("nBTags",            6,  -0.5, 5.5);
    

    if (hname != "") hname = "_"+hname;
    
    goodHistos.resize(NHISTOS);
    
    for (unsigned int ih = 0; ih < NHISTOS; ih++){
      goodHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, 
				   vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      goodHistos.at(ih)->Sumw2();
    }

    if (doFakes){
      Nt00Histos.resize(NHISTOS);
      Nt10Histos.resize(NHISTOS);
      Nt11Histos.resize(NHISTOS);
      Nt01Histos.resize(NHISTOS);

      for (unsigned int ih = 0; ih < NHISTOS; ih++){
	Nt00Histos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_Nt00"+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+"_Nt00"+hname+"_"+sampleName_+"_"+suffix, 
				     vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
	Nt00Histos.at(ih)->Sumw2();

	Nt01Histos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_Nt01"+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+"_Nt01"+hname+"_"+sampleName_+"_"+suffix, 
				     vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
	Nt01Histos.at(ih)->Sumw2();

	Nt10Histos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_Nt10"+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+"_Nt10"+hname+"_"+sampleName_+"_"+suffix, 
				     vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
	Nt10Histos.at(ih)->Sumw2();

	Nt11Histos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_Nt11"+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+"_Nt11"+hname+"_"+sampleName_+"_"+suffix, 
				     vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
	Nt11Histos.at(ih)->Sumw2();
      }
    }
  }

};

void initHistoDirThreeChannels(TFile* outFile, vector <StandardHistos*> & stanHist, TString dirName, TString sampleName, vector <double> sScale, bool DoFakes=false){

  if (stanHist.size() != 3){
    cout<<"Number of channels is not 3!";
    return;
  }

  outFile->cd();
  TDirectory* topdir = outFile->mkdir(dirName);
  TDirectory* mmDir  = topdir->mkdir("MuMu");
  TDirectory* emDir  = topdir->mkdir("ElMu");
  TDirectory* eeDir  = topdir->mkdir("ElEl");

  mmDir->cd(); 
  stanHist[0] = new StandardHistos(dirName, "MuMu", sampleName, sScale[0], DoFakes);

  emDir->cd();
  stanHist[1] = new StandardHistos(dirName, "ElMu", sampleName, sScale[1], DoFakes);
  
  eeDir->cd();
  stanHist[2] = new StandardHistos(dirName, "ElEl", sampleName, sScale[2], DoFakes); 
}

void fillHistosThreeChannels(vector <StandardHistos*> & stanHist, T53Event* teve, TString type="", bool doFakes=false){
  if (teve->suffix == "MuMu") stanHist[0]->fillAllHistos(teve, type, doFakes);
  if (teve->suffix == "ElMu") stanHist[1]->fillAllHistos(teve, type, doFakes);
  if (teve->suffix == "ElEl") stanHist[2]->fillAllHistos(teve, type, doFakes);
}

void scaleHistosThreeChannels(vector <StandardHistos*> & stanHist, bool doFakes=false){
  stanHist[0]->scaleAllHistos(doFakes);
  stanHist[1]->scaleAllHistos(doFakes);
  stanHist[2]->scaleAllHistos(doFakes);
}

void getTotFakesThreeChannels(vector <StandardHistos*> & stanHist){
  stanHist[0]->getTotFakes(FR_MU, FR_MU, PR_MU, PR_MU);
  stanHist[1]->getTotFakes(FR_EL, FR_MU, PR_EL, PR_MU);
  stanHist[2]->getTotFakes(FR_EL, FR_EL, PR_EL, PR_EL);
}

//Second class for fake rate studies
class StandardHistosFR{
  
private:
  StandardHistosFR(){}
public:
  
  StandardHistosFR(const TString & aName, const TString & aSuffix, const TString & aLabel, const double scale) :
	hname(aName), suffix(aSuffix), outLabel(aLabel), scale_(scale)
  {
    initAllHistos();
  }

  TString hname;
  
  TString suffix;
  TString outLabel;
  double scale_;

  vector <TH1F*> looseHistos;
  vector <TH1F*> tightHistos;
  vector <TH1F*> ratioHistos;
  
  void initAllHistos(){
    
    vector <HistoData*> vhd(NHISTOS_FR);
    vhd.at(Lep1Pt_FR)   = new HistoData("Lep1Pt",   480,  0.,  480.);
    vhd.at(Lep1Eta_FR)  = new HistoData("Lep1Eta",  480, -2.4, 2.4);
    vhd.at(Jet1Pt_FR)   = new HistoData("Jet1Pt",   480,  0.,  480.);
    vhd.at(nJets_FR)    = new HistoData("nJets",    9,   -0.5, 8.5);
    vhd.at(nEvents_FR)  = new HistoData("nEvents",  1,    0.5, 1.5);
    vhd.at(nPV_FR)      = new HistoData("nPV",      30,  -0.5, 29.5);
    vhd.at(mJL_FR)      = new HistoData("mJL",      240,  0.,  240.);
    vhd.at(dRJL_FR)     = new HistoData("dRJL",     50,   0.,  10);
    vhd.at(MET_FR)      = new HistoData("MET",      250,  0.,  250);
    vhd.at(MT_FR)       = new HistoData("MT",       250,  0.,  250);

    looseHistos.resize(NHISTOS_FR);
    tightHistos.resize(NHISTOS_FR);
    ratioHistos.resize(NHISTOS_FR);
    
    for (unsigned int ih = 0; ih < NHISTOS_FR; ih++){
      looseHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_loose_"+outLabel+"_"+suffix, vhd.at(ih)->histoName+"_loose_"+outLabel+"_"+suffix, 
				    vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      looseHistos.at(ih)->Sumw2();
      
      tightHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_tight_"+outLabel+"_"+suffix, vhd.at(ih)->histoName+"_tight_"+outLabel+"_"+suffix, 
				    vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      tightHistos.at(ih)->Sumw2();
      
      ratioHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+"_ratio_"+outLabel+"_"+suffix, vhd.at(ih)->histoName+"_ratio_"+outLabel+"_"+suffix, 
				    vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      ratioHistos.at(ih)->Sumw2();
    }
  }
 
  void fillLooseHistos(T53Event* teve){
    looseHistos.at(Lep1Pt_FR) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    looseHistos.at(Lep1Eta_FR)->Fill(teve->lepton1->lv.Eta(), teve->weight);
    looseHistos.at(nJets_FR)  ->Fill(teve->vGoodCA8Jets.size(), teve->weight);
    looseHistos.at(nEvents_FR)->Fill(1, teve->weight);
    looseHistos.at(nPV_FR)    ->Fill(teve->nPV, teve->weight);
    looseHistos.at(MET_FR)    ->Fill(teve->met, teve->weight);
    looseHistos.at(MT_FR)     ->Fill(teve->mt, teve->weight);
    
    if (teve->vGoodCA8Jets.size() > 0){
      looseHistos.at(Jet1Pt_FR)->Fill(teve->vGoodCA8Jets.at(0)->lv.Pt(), teve->weight);
      for (unsigned int ij = 0; ij < teve->vGoodCA8Jets.size(); ij++){
	looseHistos.at(mJL_FR)->Fill((teve->lepton1->lv + teve->vGoodCA8Jets.at(ij)->lv).M(), teve->weight);
	looseHistos.at(dRJL_FR)->Fill(teve->lepton1->lv.DeltaR(teve->vGoodCA8Jets.at(ij)->lv), teve->weight);
      }
    }
  }

  void fillTightHistos(T53Event* teve){
    tightHistos.at(Lep1Pt_FR) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    tightHistos.at(Lep1Eta_FR)->Fill(teve->lepton1->lv.Eta(), teve->weight);
    tightHistos.at(nJets_FR)  ->Fill(teve->vGoodCA8Jets.size(), teve->weight);
    tightHistos.at(nEvents_FR)->Fill(1, teve->weight);
    tightHistos.at(nPV_FR)    ->Fill(teve->nPV, teve->weight);
    tightHistos.at(MET_FR)    ->Fill(teve->met, teve->weight);
    tightHistos.at(MT_FR)     ->Fill(teve->mt, teve->weight);

    if (teve->vGoodCA8Jets.size() > 0){
      tightHistos.at(Jet1Pt_FR)->Fill(teve->vGoodCA8Jets.at(0)->lv.Pt(), teve->weight);
      for (unsigned int ij = 0; ij < teve->vGoodCA8Jets.size(); ij++){
	tightHistos.at(mJL_FR)->Fill((teve->lepton1->lv + teve->vGoodCA8Jets.at(ij)->lv).M(), teve->weight);
	tightHistos.at(dRJL_FR)->Fill(teve->lepton1->lv.DeltaR(teve->vGoodCA8Jets.at(ij)->lv), teve->weight);
      }
    }
  }

  void scaleTLHistos(){
    for (unsigned int ih = 0; ih < NHISTOS_FR; ih++){
      looseHistos.at(ih)->Scale(scale_);
      tightHistos.at(ih)->Scale(scale_);
    }
  }

  void getRatioHistos(){
    for (unsigned int ih = 0; ih < NHISTOS_FR; ih++){
      ratioHistos.at(ih)->Add(tightHistos.at(ih));
      ratioHistos.at(ih)->Divide(looseHistos.at(ih));
    }
  }

  void fillTLHistos(T53Event* teve, bool b_Tight){
    fillLooseHistos(teve);
    if (b_Tight) fillTightHistos(teve);
  }
};


#endif
