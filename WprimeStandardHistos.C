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

const unsigned int NHISTOS    = 71;

enum Histos_t {LepPt,       LepEta,     LepPhi,     LepEnergy,             //3
 	       AK4Jet1Pt,    AK4Jet2Pt,   AK4Jet3Pt,   nAK4Jets,        //7
 	       AK4Jet1Eta,    AK4Jet2Eta, AK4Jet3Eta,        //10
 	       AK4Jet1Phi,    AK4Jet2Phi, AK4Jet3Phi,        //13
               AK4Jet1CSV,    AK4Jet2CSV, AK4Jet3CSV,       //16
               AK4Jet1Jet2Pt,                               //17
	       HT, AllJetPt,                          //18 (+1)
	       MinLepAK4dR,  LepAK4dR,     //20
               LepJetpTrel,                 //21
	       MinGenLepAK4dR,  GenLepAK4dR, LepTopBjetdR,   //24
	       LepTopBjetdR_Match,   //25
	       LepIso,                      //26
	       mW,           mTop,        mWprime,         //29
               mWprime_J1J2,                 //30
               mWprime_Limits,                 //31
	       ptW,          ptTop,         //33
	       mGenW,        mGenTop,     mGenWprime,         //36
               mGenWprimeReco,
	       ptGenW,       ptGenTop,         //38
               ptGenLep,     etaGenLep,   phiGenLep,      //41
 	       GenJet1Pt,    GenJet2Pt,   GenJet3Pt,       //44
 	       GenJet1Eta,    GenJet2Eta,   GenJet3Eta,       //47
 	       GenJet1Phi,    GenJet2Phi,   GenJet3Phi,       //50
               TopRecoGenDR, WprRecoGenDR,
               TopGenJ2DR, WprGenJ1DR,
	       hnPV,         hnEvents,                     //52
	       /*hPUwgt,*/      MET,     METLepdPhi,   METJetdPhi,               //55
	       nLeptons,                         //56
	       nBTags,                        //57
	       nBTags_First2,                        //58
	       //nBjs,    nCjs,    nLjs,                      
               mTW,				//59
               dPhiWTop,     dPhiWWprime, //61
               dRJet1Jet2,   dRLepJet1,   dRLepJet2, //64
               /*ipXY,    ipZ,
               passConVeto, Deta, Dphi,
               sihih, HoE, Ooemoop, mHits,
               globMu, trkMu, nChi2,
               inTrkLay, muHits, pixHits, nMatch*/
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

    myHistos.at(hnEvents)->Fill(1, teve->weight);

    myHistos.at(hnPV)    ->Fill(teve->nPV,      teve->weight);
  //  myHistos.at(hPUwgt)  ->Fill(teve->PUweight);

    myHistos.at(LepPt) ->Fill(teve->lepton1->lv.Pt(), teve->weight);
    myHistos.at(LepEta)->Fill(teve->lepton1->lv.Eta(), teve->weight);
    myHistos.at(LepPhi)->Fill(teve->lepton1->lv.Phi(), teve->weight);
    myHistos.at(LepEnergy) ->Fill(teve->lepton1->lv.Energy(), teve->weight);

    myHistos.at(LepIso)->Fill(teve->lepton1->relIso, teve->weight);

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

    for (unsigned int i = 0; i<teve->vGoodAK4Jets.size(); i++) myHistos.at(AllJetPt)->Fill(teve->vGoodAK4Jets.at(i)->lv.Pt(), teve->weight);

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
      myHistos.at(AK4Jet1Jet2Pt)->Fill((teve->vGoodAK4Jets.at(0)->lv+teve->vGoodAK4Jets.at(1)->lv).Pt(), teve->weight);
      if (teve->lvW      . Pt() > 1) myHistos.at(mWprime_J1J2)  ->Fill((teve->vGoodAK4Jets.at(0)->lv+teve->vGoodAK4Jets.at(1)->lv+teve->lvW).M(),   teve->weight);
      myHistos.at(dRJet1Jet2)->Fill(teve->vGoodAK4Jets.at(1)->lv.DeltaR(teve->vGoodAK4Jets.at(0)->lv), teve->weight);
      myHistos.at(dRLepJet1)->Fill(teve->vGoodAK4Jets.at(0)->lv.DeltaR(teve->lepton1->lv), teve->weight);
      myHistos.at(dRLepJet2)->Fill(teve->vGoodAK4Jets.at(1)->lv.DeltaR(teve->lepton1->lv), teve->weight);
    }
    if (teve->vGoodAK4Jets.size() > 2)  {
      myHistos.at(AK4Jet3Pt)->Fill(teve->vGoodAK4Jets.at(2)->lv.Pt(), teve->weight); 
      myHistos.at(AK4Jet3Eta)->Fill(teve->vGoodAK4Jets.at(2)->lv.Eta(), teve->weight);
      myHistos.at(AK4Jet3Phi)->Fill(teve->vGoodAK4Jets.at(2)->lv.Phi(), teve->weight);
      myHistos.at(AK4Jet3CSV)->Fill(teve->vGoodAK4Jets.at(2)->csvDisc, teve->weight);
    }

    myHistos.at(nAK4Jets)->Fill(teve->vGoodAK4Jets.size(), teve->weight);

    if (teve->vGenJets.size() > 0) {
      myHistos.at(GenJet1Pt)->Fill(teve->vGenJets.at(0).Pt(), teve->weight);
      myHistos.at(GenJet1Eta)->Fill(teve->vGenJets.at(0).Eta(), teve->weight);
      myHistos.at(GenJet1Phi)->Fill(teve->vGenJets.at(0).Phi(), teve->weight);
    }
    if (teve->vGenJets.size() > 1) {
      myHistos.at(GenJet2Pt)->Fill(teve->vGenJets.at(1).Pt(), teve->weight);
      myHistos.at(GenJet2Eta)->Fill(teve->vGenJets.at(1).Eta(), teve->weight);
      myHistos.at(GenJet2Phi)->Fill(teve->vGenJets.at(1).Phi(), teve->weight);
    }
    if (teve->vGenJets.size() > 2) {
      myHistos.at(GenJet3Pt)->Fill(teve->vGenJets.at(2).Pt(), teve->weight);
      myHistos.at(GenJet3Eta)->Fill(teve->vGenJets.at(2).Eta(), teve->weight);
      myHistos.at(GenJet3Phi)->Fill(teve->vGenJets.at(2).Phi(), teve->weight);
    }
  
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
    //double BTAGCUT = 0.605; // loose ||| see https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
    //double BTAGCUT = 0.89; // medium
    //double BTAGCUT = 0.97; // tight
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUT || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUT) {
      //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUT && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUT) myHistos.at(nBTags_First2)->Fill(2., teve->weight);
    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) {
      if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 && teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) myHistos.at(nBTags_First2)->Fill(2., teve->weight);
      else myHistos.at(nBTags_First2)->Fill(1., teve->weight);
    }
    else myHistos.at(nBTags_First2)->Fill(0., teve->weight);
    if (mindR < 10.) {
      myHistos.at(MinLepAK4dR)->Fill(mindR, teve->weight);
      myHistos.at(LepJetpTrel)->Fill(ptrel, teve->weight);
    }
    mindR = 1000;
    if (teve->lvGenLepMatch.Pt()>30.) {
      if ((teve->lvGenTop-teve->lvGenW).Pt()>1) myHistos.at(LepTopBjetdR_Match)->Fill((teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLepMatch), teve->weight);
      for (unsigned int ui = 0; ui < teve->vGenJets.size(); ui++){
        if (teve->vGenJets.at(ui).Pt()<30.) continue;
        if (teve->vGenJets.at(ui).DeltaR(teve->lvGenLepMatch) < mindR) mindR = teve->vGenJets.at(ui).DeltaR(teve->lvGenLepMatch);
        myHistos.at(GenLepAK4dR)->Fill(teve->vGenJets.at(ui).DeltaR(teve->lvGenLepMatch), teve->weight);
      }    
    }
    if (mindR < 10.) myHistos.at(MinGenLepAK4dR)->Fill(mindR, teve->weight);

    
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
      myHistos.at(dPhiWTop) ->Fill(fabs(teve->lvW.DeltaPhi(teve->lvTop-teve->lvW)),     teve->weight);
    }
    if (teve->lvWprime . Pt() > 0) {
      myHistos.at(mWprime)  ->Fill(teve->lvWprime.M(),   teve->weight);
      myHistos.at(mWprime_Limits)  ->Fill(teve->lvWprime.M(),   teve->lim_weight);
      myHistos.at(dPhiWWprime) ->Fill(fabs(teve->lvW.DeltaPhi(teve->lvWprime-teve->lvTop)),     teve->weight);
    }

    if (teve->lvGenW      . Pt() > 1) {
      myHistos.at(mGenW)       ->Fill(teve->lvGenW.M(),        teve->weight);
      myHistos.at(ptGenW)      ->Fill(teve->lvGenW.Pt(),       teve->weight);
    }
    if (teve->lvGenTop    . Pt() > 1) {
      myHistos.at(mGenTop)     ->Fill(teve->lvGenTop.M(),      teve->weight);
      myHistos.at(ptGenTop)    ->Fill(teve->lvGenTop.Pt(),     teve->weight);
    }
    if (teve->lvGenWprime . Pt() > 0) {
      myHistos.at(mGenWprime)  ->Fill(teve->lvGenWprime.M(),   teve->weight);
      myHistos.at(TopRecoGenDR)->Fill((teve->lvTop-teve->lvW).DeltaR(teve->lvGenTop-teve->lvGenW), teve->weight);
      myHistos.at(WprRecoGenDR)->Fill((teve->lvTop-teve->lvWprime).DeltaR(teve->lvGenTop-teve->lvGenWprime), teve->weight);
      myHistos.at(TopGenJ2DR)->Fill((teve->vGoodAK4Jets.at(1)->lv).DeltaR(teve->lvGenTop-teve->lvGenW), teve->weight);
      myHistos.at(WprGenJ1DR)->Fill((teve->vGoodAK4Jets.at(0)->lv).DeltaR(teve->lvGenWprime-teve->lvGenTop), teve->weight);
    }
    if (teve->lvGenWprimeReco . Pt() > 0) {
      myHistos.at(mGenWprimeReco)  ->Fill(teve->lvGenWprimeReco.M(),   teve->weight);
    }
    if (teve->lvGenLep . Pt() > 1) {
      myHistos.at(ptGenLep)  ->Fill(teve->lvGenLep.Pt(),   teve->weight);
      myHistos.at(etaGenLep)  ->Fill(teve->lvGenLep.Eta(),   teve->weight);
      myHistos.at(phiGenLep)  ->Fill(teve->lvGenLep.Phi(),   teve->weight);
    }

    if ((teve->lvGenTop-teve->lvGenW).Pt()>1) myHistos.at(LepTopBjetdR)->Fill((teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep), teve->weight);

    myHistos.at(MET)->Fill(teve->met, teve->weight);
    double lepmetdphi = 999.;
    double jetmetdphi = 999.;
    lepmetdphi = teve->lepton1->lv.Phi()-teve->met_phi;
    if (lepmetdphi>TMath::Pi()) lepmetdphi -= 2*TMath::Pi();
    if (lepmetdphi<=-TMath::Pi()) lepmetdphi += 2*TMath::Pi();
    myHistos.at(METLepdPhi)->Fill(fabs(lepmetdphi), teve->weight);
    jetmetdphi = teve->vGoodAK4Jets.at(0)->lv.Phi()-teve->met_phi;
    if (jetmetdphi>TMath::Pi()) jetmetdphi -= 2*TMath::Pi();
    if (jetmetdphi<=-TMath::Pi()) jetmetdphi += 2*TMath::Pi();
    myHistos.at(METJetdPhi)->Fill(fabs(jetmetdphi), teve->weight);

    myHistos.at(nLeptons)->Fill(teve->theLeptons.size(), teve->weight);
    
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

    vhd.at(hnPV)          = new HistoData("nPV",            100, -0.5,  99.5);
    //vhd.at(hPUwgt)        = new HistoData("PUwght",         50,  0.,  3);

    vhd.at(LepPt)         = new HistoData("LepPt",         1200,  0.,  1200.);
    vhd.at(LepEta)        = new HistoData("LepEta",        1000,  -3., 3.);
    vhd.at(LepPhi)        = new HistoData("LepPhi",        1000,  -3.5, 3.5);
    vhd.at(LepEnergy)     = new HistoData("LepEnergy",     2000,  0.,  2000.);
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
    vhd.at(nAK4Jets)      = new HistoData("nAK4Jets",       16,  -0.5, 15.5);

    vhd.at(AK4Jet1Jet2Pt) = new HistoData("AK4Jet1Jet2Pt",     1800,  0.,  1800.);

    vhd.at(HT)            = new HistoData("HT",           5000,  0.,  5000.);
    vhd.at(AllJetPt)     = new HistoData("AllJetPt",      2000,  0.,  2000.);
    
    /*vhd.at(nBjs)          = new HistoData("nBjs",         10,  -0.5,  9.5);
    vhd.at(nCjs)          = new HistoData("nCjs",         10,  -0.5,  9.5);
    vhd.at(nLjs)          = new HistoData("nLjs",         10,  -0.5,  9.5);*/

    //vhd.at(LepInvM)       = new HistoData("LepInvM",       240,  0.,  240.);
    //vhd.at(LepdR)         = new HistoData("LepdR",         100,  0.,  5.);

    vhd.at(MinLepAK4dR)   = new HistoData("MinLepAK4dR",   500,  0.,  5.);
    vhd.at(LepAK4dR)   = new HistoData("LepAK4dR",   500,  0.,  5.);

    vhd.at(MinGenLepAK4dR)   = new HistoData("MinGenLepAK4dR",   500,  0.,  5.);
    vhd.at(GenLepAK4dR)   = new HistoData("GenLepAK4dR",   500,  0.,  5.);

    vhd.at(LepTopBjetdR)   = new HistoData("LepTopBjetdR",   500,  0.,  5.);
    vhd.at(LepTopBjetdR_Match)   = new HistoData("LepTopBjetdR_Match",   500,  0.,  5.);

    vhd.at(LepJetpTrel)    = new HistoData("LepJetpTrel",   500, 0., 100.);

//     vhd.at(nAK8inAKTop)   = new HistoData("nAK8inAKTop",    4,  -0.5, 3.5);
//     vhd.at(nAK8inAKW)     = new HistoData("nAK8inAKW",      4,  -0.5, 3.5);

    vhd.at(LepIso)       = new HistoData("LepIso",       100,  0.,  0.2);

    vhd.at(mW)            = new HistoData("mW",            120,  70.,  100.);
    vhd.at(mTW)           = new HistoData("mTW",           1000,  0.,  500.);
    vhd.at(mTop)          = new HistoData("mTop",         1000, 0.,  1000.);
    vhd.at(mWprime)       = new HistoData("mWprime",     5000,   0.,  5000.);
    vhd.at(mWprime_J1J2)  = new HistoData("mWprime_J1J2",5000,   0.,  5000.);
    // What was used for 8 TeV analysis
    //double xlow  = [ 100, 300,  400, 500, 600, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 4000]

    // Total guess for 13 TeV testing
    //double xlow[23]  = { 100, 300, 500, 700, 800, 900,  1000, 1100, 1200,  1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2400, 2600, 2800, 4000 };

    // Using binning script
    //double xlow[25]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,2900.0,4000.0};
    //double xlow[27]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,3000.,3600.0,4000.0,5000.0};
    double xlow[25]  = {100.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,3000.0,3600.0,5000.0};
    vhd.at(mWprime_Limits)= new HistoData("mWprime_Limits",     24,   xlow,  true);

    vhd.at(ptW)           = new HistoData("ptW",      2000,  0.,  2000.);
    vhd.at(ptTop)         = new HistoData("ptTop",    1800,  0.,  1800.);

    vhd.at(dPhiWTop)      = new HistoData("dPhiWTop",    1000,  0.,  3.5);
    vhd.at(dPhiWWprime)   = new HistoData("dPhiWWprime", 1000,  0.,  3.5);

    vhd.at(dRJet1Jet2)    = new HistoData("dRJet1Jet2",  500,  0.,  5.);
    vhd.at(dRLepJet1)     = new HistoData("dRLepJet1",   500,  0.,  5.);
    vhd.at(dRLepJet2)     = new HistoData("dRLepJet2",   500,  0.,  5.);

    vhd.at(mGenW)         = new HistoData("mGenW",         60,  50.,  130.);
    vhd.at(mGenTop)       = new HistoData("mGenTop",      240, 120.,  240.);
    vhd.at(mGenWprime)    = new HistoData("mGenWprime",  4500,   500.,  5000.);
    vhd.at(mGenWprimeReco)= new HistoData("mGenWprimeReco",  4500,   500.,  5000.);

    vhd.at(ptGenW)        = new HistoData("ptGenW",      2000,  0.,  2000.);
    vhd.at(ptGenTop)      = new HistoData("ptGenTop",    2000,  0.,  2000.);

    vhd.at(ptGenLep)      = new HistoData("ptGenLep",    2000,  0.  ,  2000.);
    vhd.at(etaGenLep)     = new HistoData("etaGenLep",   1000,  -3. ,  3.);
    vhd.at(phiGenLep)     = new HistoData("phiGenLep",   1000,  -3.5,  3.5);

    vhd.at(GenJet1Pt)     = new HistoData("GenJet1Pt",     2000,  0.,  2000.);
    vhd.at(GenJet2Pt)     = new HistoData("GenJet2Pt",     1500,  0.,  1500.);
    vhd.at(GenJet3Pt)     = new HistoData("GenJet3Pt",     2000,  0.,  1000.);
    vhd.at(GenJet1Eta)    = new HistoData("GenJet1Eta",    1000, -3.,  3.);
    vhd.at(GenJet2Eta)    = new HistoData("GenJet2Eta",    1000, -3.,  3.);
    vhd.at(GenJet3Eta)    = new HistoData("GenJet3Eta",    1000, -3.,  3.);
    vhd.at(GenJet1Phi)    = new HistoData("GenJet1Phi",    1000, -3.5,  3.5);
    vhd.at(GenJet2Phi)    = new HistoData("GenJet2Phi",    1000, -3.5,  3.5);
    vhd.at(GenJet3Phi)    = new HistoData("GenJet3Phi",    1000, -3.5,  3.5);

    vhd.at(TopRecoGenDR)  = new HistoData("TopRecoGenDR",    1000, 0,  3.5);
    vhd.at(WprRecoGenDR)  = new HistoData("WprRecoGenDR",    1000, 0,  3.5);
    vhd.at(TopGenJ2DR)    = new HistoData("TopGenJ2DR",    1000, 0,  3.5);
    vhd.at(WprGenJ1DR)    = new HistoData("WprGenJ1DR",    1000, 0,  3.5);

    vhd.at(MET)           = new HistoData("MET",          1200,   0.,  1200.);
    vhd.at(METLepdPhi)    = new HistoData("METLepdPhi",   1000,   0.,  3.5);
    vhd.at(METJetdPhi)    = new HistoData("METJetdPhi",   1000,   0.,  3.5);

    vhd.at(nLeptons)      = new HistoData("nLeptons",      3,   0.5, 3.5);

    vhd.at(nBTags)        = new HistoData("nBTags",            8,  -0.5, 7.5);
    vhd.at(nBTags_First2) = new HistoData("nBTags_First2",     3,  -0.5, 2.5);
    
    
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
    //double xlow[27]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,3000.,3600.0,4000.0,5000.0};
    double xlow[25]  = {100.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,3000.0,3600.0,5000.0};
    // Using binning script
    //double xlow[25]  = {100.0,300.0,400.0,500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0,1300.0,1400.0,1500.0,1600.0,1700.0,1800.0,1900.0,2000.0,2100.0,2200.0,2400.0,2600.0,2900.0,4000.0};
    vhd.at(0)= new HistoData("mWprime_Limits",     24,   xlow,  true);
    
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
