#ifndef ObjectID_cxx
#define ObjectID_cxx

#include <TLorentzVector.h>
#include <cmath>
#include <iostream>


using namespace std;

//Particle masses and acceptable deltas
//Do they belong here or somewhere else?
const double MTOP  = 172.5;
const double DMTOP = 30.0;
const double MW    = 80.4;
const double DMW   = 20.0;

class MCLepton{

public:
  
  TLorentzVector lv;

  double pt;
  double eta;
  double phi;
  double energy;
  int pdgID;

  void setLV() {
    //lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
  }

};

class TLepton{

public:
  
  TLepton(int chargeTemp, double ptTemp, double etaTemp, double phiTemp, double energyTemp, 
	double relIsoTemp, double miniIsoTemp, double ipXYTemp, double ipzTemp, int muonTypeTemp,
        double innerptTemp, double inneretaTemp, double innerphiTemp, double innerenergyTemp,
	int innerTrackLayersTemp, double nChi2Temp, int MuonHitsTemp, int PixelHitsTemp, int nMatchTemp);

  TLepton(int chargeTemp, double ptTemp, double etaTemp, double phiTemp, double energyTemp, 
	double relIsoTemp, double miniIsoTemp, double ipXYTemp, double ipzTemp,
	int eEEBtypes, int chargeConsistencyTemp, int notConvTemp,
	double elDeta, double elDphi, 
	double elSihih, double elHoE, double elD0, double elDZ, double elOoemoop,
	int elMHits, int elVtxFitConv);

  bool isElectron() { return isElectron_;}
  bool isMuon(){ return isMuon_;}
  
  TLorentzVector lv;
  int charge;
  double pt;
  double eta;
  double phi;
  double energy;
  double relIso;
  double miniIso;
  double ipXY, ipZ;
  bool   isElectron_, isMuon_;

  //Electrons only
  bool notEEEBGap_, isEE_, isEB_;
  int    chargeConsistency;
  int    notConv;
  double deltaEtaSuperClusterTrackAtVtx, deltaPhiSuperClusterTrackAtVtx;
  double sigmaIetaIeta, hadronicOverEm, dB, dZ, ooemoop; 
  int trackerExpectedHitsInner;
  int passConversionVeto;


  //Muons only
  int    GlobalMuon;
  int    TrackerMuon;
  double innerpt, innereta, innerphi, innerenergy;
  int    innerTrackLayers;
  double nChi2;
  int    MuonHits; // number of valid muon hits
  int    PixelHits;
  int    nMatch;
//   double relPtUnc;
  //mc
  MCLepton mother[10];
  MCLepton matchedLepton;
  bool matched;
  double gen_reco_DR;

  int origin() const;
  bool goodElectron(double minPt = 30);
  bool looseElectron(double minPt = 30);
  bool goodMuon(double minPt = 30);
  bool looseMuon(double minPt = 30);
  bool goodLepton();
  void printLepton(bool fullMatching = false);



  inline int hundred (int i) const
  {
    int j = (int) i % 1000;
    return (int) j/100;
  }
  inline int thousand (int i) const
  {
    int j = (int) i % 10000;
    return (int) j/1000;
  }

private :

  void setLV() ;
  
  double MIN_LEP_PT    ;

  double MAX_EL_ISO;
  double MAX_MU_ISO;

  double MAX_EL_ISO_LOOSE;
  double MAX_MU_ISO_LOOSE;

  double MAX_EL_ETA;
  double MAX_MU_ETA;

  TLepton(){defaults();}
  void defaults();
//   void initLepton();
};

class TJet{

public:
  
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, std::vector<int> bTagTemp);     //AK4 jets
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, double bTagDiscTemp, int jetFlavTemp);     //AK4 jets
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, std::vector<int> bTagTemp, double bTagDiscTemp, int jetFlavTemp);     //AK4 jets
  /*TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, bool CAWDauTemp, 
       int motherIndexTemp);                                                                 //For daughters
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, int indexTemp, 
       int nDaughtersTemp, double massTemp);                                                 //CAW jets
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, int indexTemp, 
       int nDaughtersTemp, double topMassTemp, double minPairMassTemp);*/                      //CATop jets
  TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp);                    //CA8 jets and cleaned leptons

  TLorentzVector lv;

  double pt;
  double eta;
  double phi;
  double energy;
  
  std::vector<int> csvTag;//, csvLoose;
//   double combinedSecondaryVertexBJetTags, combinedSecondaryVertexMVABJetTags,
// 	jetBProbabilityBJetTags, jetProbabilityBJetTags, simpleSecondaryVertexBJetTags,
// 	simpleSecondaryVertexHighEffBJetTags, simpleSecondaryVertexHighPurBJetTags,
// 	trackCountingHighEffBJetTags, trackCountingHighPurBJetTags;
  double csvDisc;
  int hadronFlavor;

  //For jet substructure
  bool   CAWDau;
  int    index;
  int    nDaughters;
  int    motherIndex;
  double mass;
  double topMass;
  double minPairMass;

  void setLV();
  void printJet();
  bool checkJetLeptondR(const TLepton & lep1, const TLepton & lep2);
  bool checkJetLeptondR(const TLepton & lep1);
  bool checkJetJetdR(const TJet & jet1);
private:
  double MIN_LEP_PT;
  double MIN_JET_PT;
  double MAX_JET_ETA;

  TJet(){defaults();}
  void defaults();
};

#endif
