#include "../interface/WprimeEvent.h"

vector<double> WprimeEvent::drBJetLepton () const {
  vector<double> dr;
  for (unsigned int i=0;i<vGoodAK4Jets.size();++i) {
    if (vGoodAK4Jets[i]->csvTag[0] != 0) {
      dr.push_back(lepton1->lv.DeltaR(vGoodAK4Jets[i]->lv));
    }
  }
  return dr;
}
double WprimeEvent::mindrBJetLepton() const {
     if (vGoodAK4Jets.size()==0) return -1;
     vector<double> dr(drBJetLepton());
     if (dr.size()==0) return -1;
     return *(min_element(dr.begin() ,dr.end()));

}

vector<double> WprimeEvent::mlb () const {
  vector<double> m;
  for (unsigned int i=0;i<vGoodAK4Jets.size();++i) {
    if (vGoodAK4Jets[i]->csvTag[0] != 0) {

      m.push_back((lepton1->lv+vGoodAK4Jets[i]->lv).M());
    }
  }
  return m;
}
double WprimeEvent::minMlb() const {
     if (vGoodAK4Jets.size()==0) return -1;
     vector<double> m(mlb());
     if (m.size()==0) return -1;
     return *(min_element(m.begin() ,m.end()));

}

void WprimeEvent::clearEvent(){
  weight = -1000.0;
  lim_weight = -1000.0;
  PUweight = 0.0;
  lepton1=0;
  theLeptons.clear();
  leptonSum.SetPxPyPzE(0,0,0,0);
  vGoodAK4Jets.clear();
  vOnlyGoodAK4Jets.clear();
  vGoodCsvs.clear();
  nBTags = 0;
  nBjs  = 0;
  nCjs  = 0;
  nLjs  = 0;
  ht  = 0;
  htAK4 = 0;
  htJetOnly = 0;
  met = 0;
  met_x = 0;
  met_y = 0;
  met_phi = 0;
  mt  = 0;
  nPV = -1;
  lvW         . SetPxPyPzE(0,0,0,0);
  lvTop       . SetPxPyPzE(0,0,0,0);
  lvWprime    . SetPxPyPzE(0,0,0,0);
  lvGenW      . SetPxPyPzE(0,0,0,0);
  lvGenTop    . SetPxPyPzE(0,0,0,0);
  lvGenWprime . SetPxPyPzE(0,0,0,0);
  lvGenWprimeReco . SetPxPyPzE(0,0,0,0);

  realData = false;
}

void WprimeEvent::printEvent(){
  cout<<"--------------------- Lepton ------------------------"<<endl;
  lepton1->printLepton();

  cout<<"--------------------- AK4 Jets ----------------------"<<endl;
  for (unsigned int ij = 0; ij < vGoodAK4Jets.size(); ij++){
    cout<<"Jet"<<ij<<endl;
    cout<<"Pt: "<<vGoodAK4Jets.at(ij)->lv.Pt()<<'\t'<<"Eta: "<<vGoodAK4Jets.at(ij)->lv.Eta()<<'\t'<<"Phi: "<<vGoodAK4Jets.at(ij)->lv.Phi()<<'\t'<<"CSV Disc: "<<vGoodAK4Jets.at(ij)->csvDisc<<endl;
  }

  cout<<"--------------------- MET ---------------------------"<<endl;
  cout<<"MET:       "<<met<<endl;
  cout<<"MET Phi:   "<<met_phi<<endl;
  cout<<endl;

  cout<<"--------------------- Hadronic Properties ----------------------"<<endl;
  cout<<"HT:        "<<ht<<endl;
  cout<<"M(W):      "<<lvW.M()<<"\tpT(W):   "<<lvW.Pt()<<endl;
  cout<<"M(Top):    "<<lvTop.M()<<"\tpT(Top): "<<lvTop.Pt()<<endl;
  cout<<"M(W'):     "<<lvWprime.M()<<"\tpT(W'):  "<<lvWprime.Pt()<<endl;
  cout<<endl;

}

vector<TJet*> WprimeEvent::selectJetsForMass()
{
  vector <TJet*> jets;
  if (vGoodAK4Jets.size()>2) return jets;
  unsigned int j[2];
  
  unsigned int nj=0;
  for (unsigned int ij = 0; ij < vGoodAK4Jets.size(); ij++){
    if ((vGoodAK4Jets.at(ij)->csvTag[0] != 0)&&(nj<2)) j[nj++]=ij;
  }
  if (nj<2) {
    for (unsigned int ij = 0; ij < vGoodAK4Jets.size(); ij++){
      if ((nj==1)&&(j[0]!=ij)) j[nj++]=ij;
      if (nj==0) j[nj++]=ij;
    }
  }
  jets.push_back(vGoodAK4Jets.at(j[0]));
  jets.push_back(vGoodAK4Jets.at(j[1]));
  return jets;
}
