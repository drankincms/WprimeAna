#ifndef GetT53GenParticles_cxx
#define GetT53GenParticles_cxx

//The gen particles in the T53 root files have bizarre mother-daughter
//decay chains making the gen info difficult to understand
//With this class and function, I make TLorentzVectors of the gen particles
//and assign them the mothers and grandmothers expected from Feynman diagrams

#include <vector>
#include <iostream>
#include <TMath.h>
#include <TLorentzVector.h>

using namespace std;

class T53GenParticle{
  
public:
  TLorentzVector lv;
  int ID;
  int motherID;
  int grandMotherID;
  
  //Indicies needed for tracking
  int index1;
  int index2;

  bool leptonic;
  
  void initT53Gen(){
    lv.SetPxPyPzE(0, 0, 0, 0);
    ID            = 0;
    motherID      = 0;
    grandMotherID = 0;
    index1        = 0;
    index2        = 0;

    leptonic      = true;
  }

  void setLVandTracking(T53GenParticle p){
    lv       = p.lv;
    index1   = p.index1;
    index2   = p.index2;
    leptonic = p.leptonic;
  }

};

int chargex3FromPdgId(int pdgId){
  
  if (pdgId == 0){
    cout<<"Invalid pdgID: 0"<<endl;
    return 0;
  }
  int sign = pdgId / abs(pdgId);

  if (abs(pdgId) < 7){                                                //Quarks
    if (abs(pdgId) % 2 == 0)                      return ( 2 * sign); //Up, Charm and Top
    else                                          return (-1 * sign); //Down, Strange and Bottom
  }
  else if (abs(pdgId) > 10 and abs(pdgId) < 17){                      //Leptons
    if      (abs(pdgId) % 2 == 0)                 return 0;           //Neutrinos
    else                                          return (-3 * sign); //Charged Leptons
  }
  else if (abs(pdgId) == 24)                      return (3 * sign);  //W's 
  else if (abs(pdgId) == 8000001)                 return (5 * sign);  //T53

  return 0;
}

vector <T53GenParticle> GetT53GenParticles(vector <TLorentzVector> lv, vector <int> genID, vector <int> genMotherID, vector <int> genMotherIndex){

  const double MTOP  = 173.5;
  const double MW    = 80.4;

  vector <T53GenParticle> vT53Gen;
  
  unsigned int gSize = lv.size();
  if (gSize != genID.size() or gSize != genMotherIndex.size()){
    cout<<"Gen vector sizes don't match!"<<endl;
    cout<<"Returning empty vector"<<endl;
    return vT53Gen;
  }

  T53GenParticle T53;
  T53.initT53Gen();
  T53.ID            = 8000001;
  T53.motherID      = 0;
  T53.grandMotherID = 0;

  T53GenParticle T53bar;
  T53bar.initT53Gen();
  T53bar.ID            = -8000001;
  T53bar.motherID      = 0;
  T53bar.grandMotherID = 0;

  T53GenParticle Wp_from_T53;
  Wp_from_T53.initT53Gen();
  Wp_from_T53.ID            = 24;
  Wp_from_T53.motherID      = 8000001;
  Wp_from_T53.grandMotherID = 0;

  T53GenParticle Wm_from_T53bar;
  Wm_from_T53bar.initT53Gen();
  Wm_from_T53bar.ID            = -24;
  Wm_from_T53bar.motherID      = -8000001;
  Wm_from_T53bar.grandMotherID = 0;

  T53GenParticle t;
  t.initT53Gen();
  t.ID            = 6;
  t.motherID      = 8000001;
  t.grandMotherID = 0;

  T53GenParticle tbar;
  tbar.initT53Gen();
  tbar.ID            = -6;
  tbar.motherID      = -8000001;
  tbar.grandMotherID = 0;

  T53GenParticle Wp_from_t;
  Wp_from_t.initT53Gen();
  Wp_from_t.ID            = 24;
  Wp_from_t.motherID      = 6;
  Wp_from_t.grandMotherID = 8000001;

  T53GenParticle Wm_from_tbar;
  Wm_from_tbar.initT53Gen();
  Wm_from_tbar.ID            = -24;
  Wm_from_tbar.motherID      = -6;
  Wm_from_tbar.grandMotherID = -8000001;

  T53GenParticle b;
  b.initT53Gen();
  b.ID            = 5;
  b.motherID      = 6;
  b.grandMotherID = 8000001;

  T53GenParticle bbar;
  bbar.initT53Gen();
  bbar.ID            = -5;
  bbar.motherID      = -6;
  bbar.grandMotherID = -8000001;

  vector <T53GenParticle> genLeptons;
  vector <T53GenParticle> genWpartons;
  T53GenParticle tmpT53Gen;

  //Find the partons and leptons
  for (unsigned int ui = 0; ui < gSize; ui++){
    //The mother ID is not reliable: the b-quark can come from either the top or the T53
    //However, it is possible to use the mother ID to distinguish b-quarks
    //from top decays from the very rare b-quarks from W decays
    if      (genID.at(ui) ==  5 and (genMotherID.at(ui) ==  6 or genMotherID.at(ui) ==  8000001)){
      b.lv = lv.at(ui);
      continue;
    }
    else if (genID.at(ui) == -5 and (genMotherID.at(ui) == -6 or genMotherID.at(ui) == -8000001)){
      bbar.lv = lv.at(ui);
      continue;
    }
    tmpT53Gen.initT53Gen();
    tmpT53Gen.lv     = lv.at(ui);
    tmpT53Gen.ID     = genID.at(ui);
    tmpT53Gen.index1 = genMotherIndex.at(ui);
    
    if      (abs(genID.at(ui)) < 6)                             genWpartons.push_back(tmpT53Gen);
    else if (abs(genID.at(ui)) > 10 and abs(genID.at(ui)) < 17) genLeptons .push_back(tmpT53Gen);
  }

  vector <T53GenParticle> vWp;
  vector <T53GenParticle> vWm;

  //Make the hadronic W's
  for (unsigned int ui = 0; ui < genWpartons.size(); ui++){
    for (unsigned int uj = ui + 1; uj < genWpartons.size(); uj++){
      if (genWpartons.at(ui).motherID != genWpartons.at(uj).motherID) continue;

      int wCharge = chargex3FromPdgId(genWpartons.at(ui).ID) + chargex3FromPdgId(genWpartons.at(uj).ID);      
      if (abs(wCharge) != 3) continue; 

      tmpT53Gen.initT53Gen();
      tmpT53Gen.lv     = genWpartons.at(ui).lv + genWpartons.at(uj).lv;
      tmpT53Gen.index1 = ui;
      tmpT53Gen.index2 = uj;
      tmpT53Gen.leptonic = false;

      if (wCharge == 3) vWp.push_back(tmpT53Gen);
      else              vWm.push_back(tmpT53Gen);
    }
  }

  //Make the leptonic W's
  for (unsigned int ui = 0; ui < genLeptons.size(); ui++){
    for (unsigned int uj = ui + 1; uj < genLeptons.size(); uj++){
      if (genLeptons.at(ui).motherID != genLeptons.at(uj).motherID) continue;
      
      int wCharge = chargex3FromPdgId(genLeptons.at(ui).ID) + chargex3FromPdgId(genLeptons.at(uj).ID);      
      if (abs(wCharge) != 3) continue; 
      
      //Lepton number conservation
      if (abs(genLeptons.at(ui).ID + genLeptons.at(uj).ID) != 1) continue;

      tmpT53Gen.initT53Gen();
      tmpT53Gen.lv = genLeptons.at(ui).lv + genLeptons.at(uj).lv;
      tmpT53Gen.index1 = ui;
      tmpT53Gen.index2 = uj;
      tmpT53Gen.leptonic = true;

      if (wCharge == 3) vWp.push_back(tmpT53Gen);
      else              vWm.push_back(tmpT53Gen);     
    }
  }

  //There must be at least 2 W+ and 2 W- combinations in the event
  if (vWp.size() < 2 or vWm.size() < 2){
    cout<<"Cannot find 2 + 2 W's!"<<endl;
    cout<<"Wp size: "<<vWp.size()<<'\t'<<"Wm size: "<<vWm.size()<<endl;
    cout<<"Returning empty vector"<<endl;
    return vT53Gen;   
  }

  //There should be exactly two W's of each charge. Unfortunately, 
  //because of the mother-daughter problems, it is possible that
  //this is not the case. Work around it using the W mass.

  double dm = 10000;

  T53GenParticle Wp1, Wp2, Wm1, Wm2;
  Wp1.initT53Gen();
  Wp2.initT53Gen();
  Wm1.initT53Gen();
  Wm2.initT53Gen();

  //Plus
  for (unsigned int ui = 0; ui < vWp.size(); ui++){
    double dm1 = abs(vWp.at(ui).lv.M() - MW);
    for (unsigned int uj = ui + 1; uj < vWp.size(); uj++){
      //Consider only valid pairs of Ws -- they cannot share any daughters
      if (vWp.at(ui).leptonic != vWp.at(uj).leptonic or
	  (vWp.at(ui).index1 != vWp.at(uj).index1 and vWp.at(ui).index1 != vWp.at(uj).index2 and
	   vWp.at(ui).index2 != vWp.at(uj).index1 and vWp.at(ui).index2 != vWp.at(uj).index2)){

	double dm2 = abs(vWp.at(uj).lv.M() - MW);
	if (dm > dm1 + dm2){
	  Wp1 = vWp.at(ui);
	  Wp2 = vWp.at(uj);
	  dm = dm1 + dm2;
	}
      }
    }
  }

  dm = 10000;
  //Minus
  for (unsigned int ui = 0; ui < vWm.size(); ui++){
    double dm1 = abs(vWm.at(ui).lv.M() - MW);
    for (unsigned int uj = ui + 1; uj < vWm.size(); uj++){
      //Consider only valid pairs of Ws -- they cannot share any daughters
      if (vWm.at(ui).leptonic != vWm.at(uj).leptonic or
	  (vWm.at(ui).index1 != vWm.at(uj).index1 and vWm.at(ui).index1 != vWm.at(uj).index2 and
	   vWm.at(ui).index2 != vWm.at(uj).index1 and vWm.at(ui).index2 != vWm.at(uj).index2)){

	double dm2 = abs(vWm.at(uj).lv.M() - MW);
	if (dm > dm1 + dm2){
	  Wm1 = vWm.at(ui);
	  Wm2 = vWm.at(uj);
	  dm = dm1 + dm2;
	}
      }
    }
  }

  //Figure out which W's comes from the tops
  //Plus
  if (abs((Wp1.lv + b.lv).M() - MTOP) < abs((Wp2.lv + b.lv).M() - MTOP)){
    Wp_from_t   . setLVandTracking(Wp1);
    Wp_from_T53 . setLVandTracking(Wp2);
  }
  else{
    Wp_from_t   . setLVandTracking(Wp2);
    Wp_from_T53 . setLVandTracking(Wp1);
  }

  //Minus
  if (abs((Wm1.lv + b.lv).M() - MTOP) < abs((Wm2.lv + b.lv).M() - MTOP)){
    Wm_from_tbar   . setLVandTracking(Wm1);
    Wm_from_T53bar . setLVandTracking(Wm2);
  }
  else{
    Wm_from_tbar   . setLVandTracking(Wm2);
    Wm_from_T53bar . setLVandTracking(Wm1);
  }

  //Set top and T53 Lorentz vectors
  t    . lv = Wp_from_t    . lv + b    . lv;
  tbar . lv = Wm_from_tbar . lv + bbar . lv;

  T53bar . lv = tbar . lv + Wm_from_T53bar . lv;
  T53    . lv = t    . lv + Wp_from_T53    . lv;

  //Set parentage of partons and leptons from W's
  //Wp_from_t
  if (Wp_from_t.leptonic){
    genLeptons.at(Wp_from_t.index1).motherID      = 24;
    genLeptons.at(Wp_from_t.index1).grandMotherID = 6;

    genLeptons.at(Wp_from_t.index2).motherID      = 24;
    genLeptons.at(Wp_from_t.index2).grandMotherID = 6;
  }
  else{
    genWpartons.at(Wp_from_t.index1).motherID      = 24;
    genWpartons.at(Wp_from_t.index1).grandMotherID = 6;

    genWpartons.at(Wp_from_t.index2).motherID      = 24;
    genWpartons.at(Wp_from_t.index2).grandMotherID = 6;   
  }
  //Wm_from_tbar
  if (Wm_from_tbar.leptonic){
    genLeptons.at(Wm_from_tbar.index1).motherID      = -24;
    genLeptons.at(Wm_from_tbar.index1).grandMotherID = -6;

    genLeptons.at(Wm_from_tbar.index2).motherID      = -24;
    genLeptons.at(Wm_from_tbar.index2).grandMotherID = -6;
  }
  else{
    genWpartons.at(Wm_from_tbar.index1).motherID      = -24;
    genWpartons.at(Wm_from_tbar.index1).grandMotherID = -6;

    genWpartons.at(Wm_from_tbar.index2).motherID      = -24;
    genWpartons.at(Wm_from_tbar.index2).grandMotherID = -6;   
  }
  //Wp_from_T53
  if (Wp_from_T53.leptonic){
    genLeptons.at(Wp_from_T53.index1).motherID      = 24;
    genLeptons.at(Wp_from_T53.index1).grandMotherID = 8000001;

    genLeptons.at(Wp_from_T53.index2).motherID      = 24;
    genLeptons.at(Wp_from_T53.index2).grandMotherID = 8000001;
  }
  else{
    genWpartons.at(Wp_from_T53.index1).motherID      = 24;
    genWpartons.at(Wp_from_T53.index1).grandMotherID = 8000001;

    genWpartons.at(Wp_from_T53.index2).motherID      = 24;
    genWpartons.at(Wp_from_T53.index2).grandMotherID = 8000001;   
  }
  //Wm_from_T53bar
  if (Wm_from_T53bar.leptonic){
    genLeptons.at(Wm_from_T53bar.index1).motherID      = -24;
    genLeptons.at(Wm_from_T53bar.index1).grandMotherID = -8000001;

    genLeptons.at(Wm_from_T53bar.index2).motherID      = -24;
    genLeptons.at(Wm_from_T53bar.index2).grandMotherID = -8000001;
  }
  else{
    genWpartons.at(Wm_from_T53bar.index1).motherID      = -24;
    genWpartons.at(Wm_from_T53bar.index1).grandMotherID = -8000001;
    
    genWpartons.at(Wm_from_T53bar.index2).motherID      = -24;
    genWpartons.at(Wm_from_T53bar.index2).grandMotherID = -8000001;   
  }
  
  //Finally, fill the output vector
  for (unsigned int ui = 0; ui < genWpartons.size(); ui++){
    vT53Gen.push_back(genWpartons.at(ui));
  }
  for (unsigned int ui = 0; ui < genLeptons.size(); ui++){
    vT53Gen.push_back(genLeptons.at(ui));
  }

  vT53Gen.push_back(T53);
  vT53Gen.push_back(T53bar);

  vT53Gen.push_back(t);
  vT53Gen.push_back(tbar);
  
  vT53Gen.push_back(Wp_from_T53);
  vT53Gen.push_back(Wm_from_T53bar);

  vT53Gen.push_back(Wp_from_t);
  vT53Gen.push_back(Wm_from_tbar);

  return vT53Gen;

}

void printGenParticles(vector <T53GenParticle> vT53Gen){
  cout<<"Begin gen particles"<<endl;
  for (unsigned int ui = 0; ui < vT53Gen.size(); ui++){
    T53GenParticle p = vT53Gen.at(ui);
    cout<<p.ID<<'\t'<<p.motherID<<'\t'<<p.grandMotherID<<'\t'<<p.lv.Pt()<<'\t'<<p.lv.Eta()<<'\t'<<p.lv.Phi()<<'\t'<<p.lv.M()<<endl;
  }
  cout<<"End gen particles"<<endl<<endl;
}


#endif
