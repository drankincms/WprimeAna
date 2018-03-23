#ifndef GetWprimeGenParticles_cxx
#define GetWprimeGenParticles_cxx

//The gen particles in the Wprime root files have bizarre mother-daughter
//decay chains making the gen info difficult to understand
//With this class and function, I make TLorentzVectors of the gen particles
//and assign them the mothers and grandmothers expected from Feynman diagrams

#include <vector>
#include <iostream>
#include <TMath.h>
#include <TLorentzVector.h>

using namespace std;

class WprimeGenParticle{
  
public:
  TLorentzVector lv;
  int ID;
  
  void initWprimeGen(){
    lv.SetPxPyPzE(0, 0, 0, 0);
    ID            = 0;
  }

};

vector <WprimeGenParticle> GetWprimeGenParticles(vector <TLorentzVector> lv, vector <int> genID){

  vector <WprimeGenParticle> vWprimeGen;
  
  unsigned int gSize = lv.size();
  if (gSize != genID.size()){
    cout<<"Gen vector sizes don't match!"<<endl;
    cout<<"Returning empty vector"<<endl;
    return vWprimeGen;
  }

  WprimeGenParticle Wprime;
  Wprime.initWprimeGen();
  Wprime.ID            = 34;

  WprimeGenParticle Wprimebar;
  Wprimebar.initWprimeGen();
  Wprimebar.ID            = -34;

  WprimeGenParticle t;
  t.initWprimeGen();
  t.ID            = 6;

  WprimeGenParticle tbar;
  tbar.initWprimeGen();
  tbar.ID            = -6;

  WprimeGenParticle Wp;
  Wp.initWprimeGen();
  Wp.ID            = 24;

  WprimeGenParticle Wm;
  Wm.initWprimeGen();
  Wm.ID            = -24;

  WprimeGenParticle b;
  b.initWprimeGen();
  b.ID            = 5;

  WprimeGenParticle bbar;
  bbar.initWprimeGen();
  bbar.ID            = -5;

  vector <WprimeGenParticle> genLeptons;

  WprimeGenParticle tmpWprimeGen;

  //Find the partons and leptons
  for (unsigned int ui = 0; ui < gSize; ui++){
    //The mother ID is not reliable: the b-quark can come from either the top or the Wprime
    //However, it is possible to use the mother ID to distinguish b-quarks
    //from top decays from the very rare b-quarks from W decays
    if (genID.at(ui) == 5){
      b.lv = lv.at(ui);
      continue;
    }
    else if (genID.at(ui) == -5){
      bbar.lv = lv.at(ui);
      continue;
    }
    tmpWprimeGen.initWprimeGen();
    tmpWprimeGen.lv       = lv.at(ui);
    tmpWprimeGen.ID       = genID.at(ui);
    
    if (abs(genID.at(ui)) > 10 and abs(genID.at(ui)) < 17) genLeptons .push_back(tmpWprimeGen);
  }

  bool is_positive = true;

  //Make the leptonic W's
  for (unsigned int ui = 0; ui < genLeptons.size(); ui++){
    for (unsigned int uj = ui + 1; uj < genLeptons.size(); uj++){
      
      int lepidsum = genLeptons.at(ui).ID + genLeptons.at(uj).ID;
      //Lepton number conservation
      if (abs(lepidsum) != 1) continue;

      tmpWprimeGen.initWprimeGen();
      tmpWprimeGen.lv = genLeptons.at(ui).lv + genLeptons.at(uj).lv;

      if (lepidsum > 0) Wp.lv = tmpWprimeGen.lv;
      else {
        Wm.lv = tmpWprimeGen.lv;
        is_positive = false;
      }
      break;
    }
  }

  //Set top and Wprime Lorentz vectors
  if (is_positive) {
    t.lv = Wp.lv + b.lv;
    Wprime.lv = t.lv + bbar.lv;
  }
  else {
    tbar.lv = Wm.lv + bbar.lv;
    Wprimebar.lv = tbar.lv + b.lv;
  }

  //Finally, fill the output vector
  for (unsigned int ui = 0; ui < genLeptons.size(); ui++){
    vWprimeGen.push_back(genLeptons.at(ui));
  }

  vWprimeGen.push_back(Wprime);
  vWprimeGen.push_back(Wprimebar);

  vWprimeGen.push_back(t);
  vWprimeGen.push_back(tbar);
  
  vWprimeGen.push_back(b);
  vWprimeGen.push_back(bbar);

  vWprimeGen.push_back(Wp);
  vWprimeGen.push_back(Wm);

  return vWprimeGen;

}

void printGenParticles(vector <WprimeGenParticle> vWprimeGen){
  cout<<"Begin gen particles"<<endl;
  for (unsigned int ui = 0; ui < vWprimeGen.size(); ui++){
    WprimeGenParticle p = vWprimeGen.at(ui);
    cout<<p.ID<<'\t'<<'\t'<<p.lv.Pt()<<'\t'<<p.lv.Eta()<<'\t'<<p.lv.Phi()<<'\t'<<p.lv.M()<<endl;
  }
  cout<<"End gen particles"<<endl<<endl;
}


#endif
