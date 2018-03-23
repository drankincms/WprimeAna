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

#include "Objects/interface/WprimeEvent.h"

const unsigned int NHISTOS    = 31;

enum Histos_t {LepPt,       LepEta,     LepPhi,                       //3
 	       AK4Jet1Pt,    AK4Jet2Pt,   AK4Jet3Pt,   nAK4Jets,        //7
 	       AK4Jet1Eta,    AK4Jet2Eta, AK4Jet3Eta,        //10
 	       AK4Jet1Phi,    AK4Jet2Phi, AK4Jet3Phi,        //13
               AK4Jet1CSV,    AK4Jet2CSV, AK4Jet3CSV,       //16
	       HT,                           //17
	       MinLepAK4dR,  LepAK4dR,     //19
               LepJetpTrel,                 //20
	       mW,           mTop,        mWprime,         //23
	       ptW,          ptTop,         //25
	       hnPV,         hnEvents,                     //27
	       /*hPUwgt,*/      MET,          //28
	       nBTags,                        //29
	       nBTags_First2,                        //30
	       //nBjs,    nCjs,    nLjs,                      
               mTW,				//31
};

class HistoData{

public:

  TString histoName;
  int     nBins;
  double  min;
  double  max;
  double *binArr;

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
  
  StandardHistos(const TString & aName, const TString & aSuffix, const TString & sampleName, const double scale) :
	hname(aName), suffix(aSuffix), sampleName_(sampleName), scale_(scale)
  {
    initAllHistos();
  }

  void fillVHD(WprimeEvent* teve, vector <TH1F*> myHistos){

    myHistos.at(hnEvents)->Fill(1, teve->weight);

    myHistos.at(hnPV)    ->Fill(teve->nPV,      teve->weight);
  //  myHistos.at(hPUwgt)  ->Fill(teve->PUweight);

    myHistos.at(LepPt) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    myHistos.at(LepEta)->Fill(teve->lepton1->lv.Eta(), teve->weight);
    myHistos.at(LepPhi)->Fill(teve->lepton1->lv.Phi(), teve->weight);


    /*myHistos.at(ipXY)->Fill(teve->lepton1->ipXY, teve->weight);
    myHistos.at(ipZ)->Fill(teve->lepton1->ipZ, teve->weight);
    if (teve->suffix == "El") {
      myHistos.at(Deta)->Fill(teve->lepton1->deltaEtaSuperClusterTrackAtVtx, teve->weight);
      myHistos.at(Dphi)->Fill(teve->lepton1->deltaPhiSuperClusterTrackAtVtx, teve->weight);
      myHistos.at(sihih)->Fill(teve->lepton1->sigmaIetaIeta, teve->weight);
      myHistos.at(HoE)->Fill(teve->lepton1->hadronicOverEm, teve->weight);
      myHistos.at(Ooemoop)->Fill(teve->lepton1->ooemoop, teve->weight);
      myHistos.at(mHits)->Fill(teve->lepton1->trackerExpectedHitsInner, teve->weight);
      myHistos.at(passConVeto)->Fill(teve->lepton1->passConversionVeto, teve->weight);
    }
    if (teve->suffix == "Mu") {
      myHistos.at(globMu)->Fill(teve->lepton1->GlobalMuon, teve->weight);
      myHistos.at(trkMu)->Fill(teve->lepton1->TrackerMuon, teve->weight);
      myHistos.at(inTrkLay)->Fill(teve->lepton1->innerTrackLayers, teve->weight);
      myHistos.at(nChi2)->Fill(teve->lepton1->nChi2, teve->weight);
      myHistos.at(muHits)->Fill(teve->lepton1->MuonHits, teve->weight);
      myHistos.at(pixHits)->Fill(teve->lepton1->PixelHits, teve->weight);
      myHistos.at(nMatch)->Fill(teve->lepton1->nMatch, teve->weight);
    }*/

    if (teve->vGoodAK4Jets.size() > 0)  {
      myHistos.at(AK4Jet1Pt)->Fill(teve->vGoodAK4Jets.at(0)->lv.Pt(), teve->weight);
      myHistos.at(AK4Jet1Eta)->Fill(teve->vGoodAK4Jets.at(0)->lv.Eta(), teve->weight);
      myHistos.at(AK4Jet1Phi)->Fill(teve->vGoodAK4Jets.at(0)->lv.Phi(), teve->weight);
      myHistos.at(AK4Jet1CSV)->Fill(teve->vGoodAK4Jets.at(0)->csvDisc, teve->weight);
    }
    if (teve->vGoodAK4Jets.size() > 1)  {
      myHistos.at(AK4Jet2Pt)->Fill(teve->vGoodAK4Jets.at(1)->lv.Pt(), teve->weight); 
      myHistos.at(AK4Jet2Eta)->Fill(teve->vGoodAK4Jets.at(1)->lv.Eta(), teve->weight);
      myHistos.at(AK4Jet2Phi)->Fill(teve->vGoodAK4Jets.at(1)->lv.Phi(), teve->weight);
      myHistos.at(AK4Jet2CSV)->Fill(teve->vGoodAK4Jets.at(1)->csvDisc, teve->weight);
    }
    if (teve->vGoodAK4Jets.size() > 2)  {
      myHistos.at(AK4Jet3Pt)->Fill(teve->vGoodAK4Jets.at(2)->lv.Pt(), teve->weight); 
      myHistos.at(AK4Jet3Eta)->Fill(teve->vGoodAK4Jets.at(2)->lv.Eta(), teve->weight);
      myHistos.at(AK4Jet3Phi)->Fill(teve->vGoodAK4Jets.at(2)->lv.Phi(), teve->weight);
      myHistos.at(AK4Jet3CSV)->Fill(teve->vGoodAK4Jets.at(2)->csvDisc, teve->weight);
    }

    myHistos.at(nAK4Jets)->Fill(teve->vGoodAK4Jets.size(), teve->weight);

  
    myHistos.at(HT)      ->Fill(teve->ht, teve->weight);

    /*myHistos.at(nBjs)    ->Fill(teve->nBjs, teve->weight);
    myHistos.at(nCjs)    ->Fill(teve->nCjs, teve->weight);
    myHistos.at(nLjs)    ->Fill(teve->nLjs, teve->weight);*/

    //myHistos.at(LepInvM)->Fill(teve->leptonSum.M(), teve->weight);

    double minJetPt = 25.;
    double mindR = 1000;
    double ptrel = 1000;
    for (unsigned int ui = 0; ui < teve->vGoodAK4Jets.size(); ui++){
      if (teve->vGoodAK4Jets.at(ui)->lv.DeltaR(teve->lepton1->lv) < mindR && teve->vGoodAK4Jets.at(ui)->lv.Pt() > minJetPt) {
        mindR = teve->vGoodAK4Jets.at(ui)->lv.DeltaR(teve->lepton1->lv);
        ptrel = teve->lepton1->lv.P()*(teve->vGoodAK4Jets.at(ui)->lv.Vect().Cross(teve->lepton1->lv.Vect()).Mag()/teve->vGoodAK4Jets.at(ui)->lv.P()/teve->lepton1->lv.P());
      }
      if (teve->vGoodAK4Jets.at(ui)->lv.Pt() > minJetPt) myHistos.at(LepAK4dR)->Fill(teve->vGoodAK4Jets.at(ui)->lv.DeltaR(teve->lepton1->lv), teve->weight);
    }    
    double BTAGCUT = 0.605; // loose ||| see https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
    //double BTAGCUT = 0.89; // medium
    //double BTAGCUT = 0.97; // tight
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUT || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUT) {
      if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUT && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUT) myHistos.at(nBTags_First2)->Fill(2., teve->weight);
      else myHistos.at(nBTags_First2)->Fill(1., teve->weight);
    }
    else myHistos.at(nBTags_First2)->Fill(0., teve->weight);
    if (mindR < 10.) {
      myHistos.at(MinLepAK4dR)->Fill(mindR, teve->weight);
      myHistos.at(LepJetpTrel)->Fill(ptrel, teve->weight);
    }
    
    if (teve->lvW      . Pt() > 1) {
      myHistos.at(mW)       ->Fill(teve->lvW.M(),        teve->weight);
      double delta_phi = (teve->met_phi)-(teve->lepton1->lv.Pt());
      if (delta_phi>TMath::Pi()) delta_phi-=2*TMath::Pi();
      if (delta_phi<=-TMath::Pi()) delta_phi+=2*TMath::Pi();
      myHistos.at(mTW)       ->Fill(TMath::Sqrt(2*(teve->met)*(teve->lepton1->lv.Pt())*(1-TMath::Cos(delta_phi))),        teve->weight);
      myHistos.at(ptW)      ->Fill(teve->lvW.Pt(),       teve->weight);
    }
    if (teve->lvTop    . Pt() > 1) {
      myHistos.at(mTop)     ->Fill(teve->lvTop.M(),      teve->weight);
      myHistos.at(ptTop)    ->Fill(teve->lvTop.Pt(),     teve->weight);
    }
    if (teve->lvWprime . Pt() > 0) {
      myHistos.at(mWprime)  ->Fill(teve->lvWprime.M(),   teve->weight);
    }

    myHistos.at(MET)->Fill(teve->met, teve->weight);
    
    myHistos.at(nBTags)->Fill(teve->nBTags, teve->weight);

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
    vhd.at(hnEvents)      = new HistoData("nEvents",         1,  0.5,  1.5);

    vhd.at(hnPV)          = new HistoData("nPV",            50, -0.5,  49.5);
    //vhd.at(hPUwgt)        = new HistoData("PUwght",         50,  0.,  3);

    vhd.at(LepPt)         = new HistoData("LepPt",         1200,  0.,  1200.);
    vhd.at(LepEta)        = new HistoData("LepEta",        1000,  -3., 3.);
    vhd.at(LepPhi)        = new HistoData("LepPhi",        1000,  -3.5, 3.5);
    //vhd.at(Lep2Pt)        = new HistoData("Lep2Pt",        480,  0.,  480.);

    /*vhd.at(ElPt)          = new HistoData("ElPt",          1000,  0.,  1000.);
    vhd.at(MuPt)          = new HistoData("MuPt",          1000,  0.,  1000.);

    vhd.at(ElEta)         = new HistoData("ElEta",          60, -2.4, 2.4);
    vhd.at(MuEta)         = new HistoData("MuEta",          60, -2.4, 2.4);*/


    /*vhd.at(ipXY)          = new HistoData("ipXY",        1000,  -0.05, 0.05);
    vhd.at(ipZ)           = new HistoData("ipZ",        2000,  -0.2, 0.2);
    vhd.at(passConVeto)   = new HistoData("passConVeto",        2,  0, 1.5);
    vhd.at(Deta)          = new HistoData("Deta",        400,  -0.01, 0.01);
    vhd.at(Dphi)          = new HistoData("Dphi",        1000,  -0.05, 0.05);
    vhd.at(sihih)         = new HistoData("sihih",        2000,  0., 0.05);
    vhd.at(HoE)           = new HistoData("HoE",        2000,  0., 0.1);
    vhd.at(Ooemoop)       = new HistoData("Ooemoop",        1000,  0., 0.1);
    vhd.at(mHits)         = new HistoData("mHits",        4,  -0.5, 3.5);
    vhd.at(globMu)        = new HistoData("globMu",        10,  -0.5, 9.5);
    vhd.at(trkMu)         = new HistoData("trkMu",        10,  -0.5, 9.5);
    vhd.at(nChi2)         = new HistoData("nChi2",        2400,  0., 12.);
    vhd.at(inTrkLay)      = new HistoData("inTrkLay",        31,  -0.5, 30.5);
    vhd.at(muHits)        = new HistoData("muHits",        61,  -0.5, 60.5);
    vhd.at(pixHits)       = new HistoData("pixHits",        10,  -0.5, 9.5);
    vhd.at(nMatch)        = new HistoData("nMatch",        10,  -0.5, 9.5);*/

    vhd.at(AK4Jet1Pt)     = new HistoData("AK4Jet1Pt",     2000,  0.,  2000.);
    vhd.at(AK4Jet2Pt)     = new HistoData("AK4Jet2Pt",     1500,  0.,  1500.);
    vhd.at(AK4Jet3Pt)     = new HistoData("AK4Jet3Pt",     2000,  0.,  1000.);
    vhd.at(AK4Jet1Eta)    = new HistoData("AK4Jet1Eta",    1000, -3.,  3.);
    vhd.at(AK4Jet2Eta)    = new HistoData("AK4Jet2Eta",    1000, -3.,  3.);
    vhd.at(AK4Jet3Eta)    = new HistoData("AK4Jet3Eta",    1000, -3.,  3.);
    vhd.at(AK4Jet1Phi)    = new HistoData("AK4Jet1Phi",    1000, -3.5,  3.5);
    vhd.at(AK4Jet2Phi)    = new HistoData("AK4Jet2Phi",    1000, -3.5,  3.5);
    vhd.at(AK4Jet3Phi)    = new HistoData("AK4Jet3Phi",    1000, -3.5,  3.5);
    vhd.at(AK4Jet1CSV)    = new HistoData("AK4Jet1CSV",    1200, -0.1,  1.1);
    vhd.at(AK4Jet2CSV)    = new HistoData("AK4Jet2CSV",    1200, -0.1,  1.1);
    vhd.at(AK4Jet3CSV)    = new HistoData("AK4Jet3CSV",    1200, -0.1,  1.1);
    vhd.at(nAK4Jets)      = new HistoData("nAK4Jets",       9,  -0.5, 8.5);

    vhd.at(HT)            = new HistoData("HT",           4000,  0.,  4000.);
    
    /*vhd.at(nBjs)          = new HistoData("nBjs",         10,  -0.5,  9.5);
    vhd.at(nCjs)          = new HistoData("nCjs",         10,  -0.5,  9.5);
    vhd.at(nLjs)          = new HistoData("nLjs",         10,  -0.5,  9.5);*/

    //vhd.at(LepInvM)       = new HistoData("LepInvM",       240,  0.,  240.);
    //vhd.at(LepdR)         = new HistoData("LepdR",         100,  0.,  5.);

    vhd.at(MinLepAK4dR)   = new HistoData("MinLepAK4dR",   500,  0.,  5.);
    vhd.at(LepAK4dR)   = new HistoData("LepAK4dR",   500,  0.,  5.);

    vhd.at(LepJetpTrel)    = new HistoData("LepJetpTrel",   500, 0., 100.);

//     vhd.at(nAK8inAKTop)   = new HistoData("nAK8inAKTop",    4,  -0.5, 3.5);
//     vhd.at(nAK8inAKW)     = new HistoData("nAK8inAKW",      4,  -0.5, 3.5);

    vhd.at(mW)            = new HistoData("mW",            120,  70.,  100.);
    vhd.at(mTW)           = new HistoData("mTW",           1000,  0.,  500.);
    vhd.at(mTop)          = new HistoData("mTop",         1000, 0.,  1000.);
    vhd.at(mWprime)       = new HistoData("mWprime",     4000,   0.,  4000.);
    // What was used for 8 TeV analysis
    //double xlow  = [ 100, 300,  400, 500, 600, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 4000]

    vhd.at(ptW)           = new HistoData("ptW",      2000,  0.,  2000.);
    vhd.at(ptTop)         = new HistoData("ptTop",    1800,  0.,  1800.);

    vhd.at(MET)           = new HistoData("MET",          1200,   0.,  1200.);

    vhd.at(nBTags)        = new HistoData("nBTags",            8,  -0.5, 7.5);
    vhd.at(nBTags_First2) = new HistoData("nBTags_First2",     3,  -0.5, 2.5);
    
    
    if (hname != "") hname = "_"+hname;
    
    goodHistos.resize(NHISTOS);
    
    for (unsigned int ih = 0; ih < NHISTOS; ih++){
      goodHistos.at(ih) = new TH1F(vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, vhd.at(ih)->histoName+hname+"_"+sampleName_+"_"+suffix, 
				   vhd.at(ih)->nBins, vhd.at(ih)->min, vhd.at(ih)->max);
      goodHistos.at(ih)->Sumw2();
    }

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


#endif
