#ifndef WprimeEvent_h
#define WprimeEvent_h
#include <TString.h>
#include "TLepton.h"

class WprimeEvent{

public:

  TString suffix;

  unsigned int isam;
  bool   realData;

  double weight;
  double lim_weight;
  double PUweight;
  int    nPV;
 
  TLepton* lepton1;
  vector <TLepton *> theLeptons;
  TLorentzVector leptonSum;

  vector <TJet*>        vGoodAK4Jets;
  vector <TJet*>        vOnlyGoodAK4Jets;
  vector <TJet*>        vGoodAK8Jets;
  vector <unsigned int> vGoodCsvs;

  int nBjs;
  int nCjs;
  int nLjs;

  double ht;
  double htAK4;
  double htJetOnly;
  double met;
  double met_x;
  double met_y;
  double met_phi;
  double mt;
  
  int nBTags;

  int nAK4inAKTop;
  int nAK4inAKW;

  TLorentzVector lvW;
  TLorentzVector lvTop;
  TLorentzVector lvWprime;

  TLorentzVector lvGenW;
  TLorentzVector lvGenTop;
  TLorentzVector lvGenWprime;
  TLorentzVector lvGenWprimeReco;
  TLorentzVector lvGenLep;
  TLorentzVector lvGenLepMatch;

  vector <TLorentzVector> vGenJets;

  WprimeEvent()  { clearEvent(); }

  void clearEvent();
  void printEvent();

  vector<double> drBJetLepton () const;
  double mindrBJetLepton() const;

  vector<double> mlb () const;
  double minMlb() const;
  
  vector <TJet*> selectJetsForMass();
};

#endif
