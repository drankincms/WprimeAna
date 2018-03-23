#include "../interface/TLepton.h"

TLepton::TLepton(int chargeTemp, double ptTemp, double etaTemp, double phiTemp, double energyTemp, 
	double relIsoTemp, double miniIsoTemp, double ipXYTemp, double ipzTemp, int muonTypeTemp, 
        double innerptTemp, double inneretaTemp, double innerphiTemp, double innerenergyTemp,
	int innerTrackLayersTemp, double nChi2Temp, int MuonHitsTemp, int PixelHitsTemp, int nMatchTemp) :
  charge(chargeTemp), pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp),
  relIso(relIsoTemp), miniIso(miniIsoTemp), ipXY(ipXYTemp), ipZ(ipzTemp),
  isElectron_(false), isMuon_(true),
  innerpt(innerptTemp), innereta(inneretaTemp), innerphi(innerphiTemp), innerenergy(innerenergyTemp),
  innerTrackLayers(innerTrackLayersTemp), 
  nChi2(nChi2Temp), MuonHits(MuonHitsTemp), PixelHits(PixelHitsTemp), 
  nMatch(nMatchTemp)
{

  defaults();

  if (muonTypeTemp==8) {
    GlobalMuon = true;
    TrackerMuon= true;
  } 
  else{  
    GlobalMuon  = (muonTypeTemp & 0x4) >> 1;
    TrackerMuon =  muonTypeTemp & 0x1;
  }
  setLV();
}

TLepton::TLepton(int chargeTemp, double ptTemp, double etaTemp, double phiTemp, double energyTemp, 
	double relIsoTemp, double miniIsoTemp, double ipXYTemp, double ipzTemp,
	int eEEBtypes, int chargeConsistencyTemp, int notConvTemp,
	double elDeta, double elDphi,
	double elSihih, double elHoE, double elD0, double elDZ, double elOoemoop,
	int elMHits, int elVtxFitConv):
  charge(chargeTemp), pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp),
  relIso(relIsoTemp), miniIso(miniIsoTemp), ipXY(ipXYTemp), ipZ(ipzTemp), 
  isElectron_(true), isMuon_(false),
  chargeConsistency(chargeConsistencyTemp), notConv(notConvTemp),
  deltaEtaSuperClusterTrackAtVtx(elDeta), deltaPhiSuperClusterTrackAtVtx(elDphi),
  sigmaIetaIeta(elSihih), hadronicOverEm(elHoE), dB(elD0), dZ(elDZ), ooemoop(elOoemoop),
  trackerExpectedHitsInner(elMHits), passConversionVeto(elVtxFitConv)
{
  defaults();
  notEEEBGap_ = (eEEBtypes & 0x4) >> 2;
  isEE_       = (eEEBtypes & 0x2) >> 1;
  isEB_       =  eEEBtypes & 0x1;

  setLV();
}


int TLepton::origin() const
{
  if (!matched) return 0;
  int i = 0;
  while (i<10) {
    if (abs(mother[i].pdgID)==24) return 24; // The mother is a W
    if ((abs(mother[i].pdgID)!=11) && (abs(mother[i].pdgID)!=13) && (abs(mother[i].pdgID)!=15)) break; 
    ++i; // The mother is a lepton, continue checking whether the next mother is a W.
  }
  // So it is not a W
  for (i = 0;i<10;++i ) {
    if ((thousand(fabs(mother[i].pdgID))== 5) || (hundred(fabs(mother[i].pdgID))== 5)) return 5;
  }
  for (i = 0;i<10;++i ) {
    if ((thousand(fabs(mother[i].pdgID))== 4) || (hundred(fabs(mother[i].pdgID))== 4)) return 4;
  }
  for (i = 0;i<10;++i ) {
    int t = thousand(fabs(mother[i].pdgID));
    int h = hundred(fabs(mother[i].pdgID));
    if ((t==1)||(t==2)|| (t==3) || (h==1)||(h==2)|| (h==3)) return 1;
  }
  return -1;
}

void TLepton::setLV() {
  if (isElectron_) lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
  else {
    if (fabs(eta)>1.2) lv.SetPxPyPzE(innerpt*cos(innerphi), innerpt*sin(innerphi), innerpt*TMath::SinH(innereta), innerenergy);
    else lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
  }
  //lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
//     if (matched) {
//       matchedLepton.setLV();
//       for (int i = 0;i<10;++i ) if (mother[i].pdgID!=0) mother[i].setLV();
//     }

}

bool TLepton::looseElectron(double minPt){
  //Common to good and loose
  if (isMuon_)                                     return false;
  if (lv.Pt() < minPt)                             return false;
  if (fabs(lv.Eta()) > MAX_EL_ETA)                 return false;
  if (relIso > MAX_EL_ISO_LOOSE)                   return false;

  return true;
}

bool TLepton::goodElectron(double minPt){
  if (not looseElectron(minPt))                   return false;
  //Barrel
/*  if(fabs(lv.Eta()) <= 1.479){
    if(sigmaIetaIeta >= 0.0101) return false;
    if(fabs(deltaEtaSuperClusterTrackAtVtx) >= 0.0095)    return false;
    if(fabs(deltaPhiSuperClusterTrackAtVtx) >= 0.0291)     return false;
    if(hadronicOverEm >= 0.0372)        return false;
    if(relIso > 0.0468)        return false;
    if(ooemoop >= 0.0174) return false;
    if(fabs(dB) >= 0.0144)      return false;
    if(fabs(dZ) >= 0.323)      return false;
    if(trackerExpectedHitsInner > 2)              return false;
    if(!passConversionVeto)        return false;
  }
  //Endcap
  else{
    if(sigmaIetaIeta >= 0.0287) return false;
    if(fabs(deltaEtaSuperClusterTrackAtVtx) >= 0.00762)    return false;
    if(fabs(deltaPhiSuperClusterTrackAtVtx) >= 0.0439)    return false;
    if(hadronicOverEm >= 0.0544)        return false;
    if(relIso >= 0.0759)        return false; 
    if(ooemoop >= 0.01) return false;
    if(fabs(dB) >= 0.0377)      return false;
    if(fabs(dZ) >= 0.571)      return false;
    if(trackerExpectedHitsInner > 1)              return false;
    if(!passConversionVeto)        return false;
  }*/
  if (relIso > MAX_EL_ISO)                         return false;  

  return true;
}


bool TLepton::looseMuon(double minPt){
  //Common to good and loose
  if (isElectron_)                  return false;
  if (lv.Pt() < minPt)              return false;
  if (fabs(lv.Eta()) > MAX_MU_ETA)  return false;
  if (relIso > MAX_MU_ISO_LOOSE)    return false;

  return true;
}

bool TLepton::goodMuon(double minPt){
  if (not looseMuon(minPt))         return false;

  if (relIso > MAX_MU_ISO)          return false;

  return true;
}


//For measurement of fake contribution

bool TLepton::goodLepton(){
  if (goodElectron()) return true;
  if (goodMuon())     return true;
  return false;
}

// float TLepton::leptonIDsfLoose(int run)
// {
//   if (isElectron_){
//   } else {
//     if (run < 198000) { // 2012 A & B
//       if (fabs(eta)< 0.9) {
//         return 0.9988;
//       } else if (fabs(eta)< 1.2) {
//         return 0.9993;
//       } else {
//         return 0.9982;
//       }
//     } else {		// 2012 C
//       if (fabs(eta)< 0.9) {
//         return 0.9985;
//       } else if (fabs(eta)< 1.2) {
//         return 0.9992;
//       } else {
//         return 0.9986;
//       }
//     }
//   }
// }

void TLepton::printLepton(bool fullMatching){

  if (lv.Pt() < 1){
    cout<<"Lepton with pT < 1"<<endl;
//      return;
  }

  if (isElectron_) cout<<"ele ";
  else cout<<"mu ";
  cout<<"pt: "<<lv.Pt()<<" eta: "<<lv.Eta()<<" phi: "<<lv.Phi()<<endl;
  //cout<<"charge:            "<<charge<<endl;
  //cout<<"iso:               "<<relIso<<endl;
  //cout<<"ipXY / ipZ:        "<<ipXY<<" / "<< ipZ<<  endl;

  if (isElectron_){
    //Electrons only
    //cout<<"EE/EB/notGap:        "<<isEE_<<" / "<< isEB_<<" / "<<  notEEEBGap_<<endl;
//     cout<<"Cut based-id L/M/T   "<<elQualityL_<<" / "<< elQualityM_<<" / "<<  elQualityT_<<endl;

//     cout<<"hyperTightID:      "<<hyperTightID<<endl;
    //cout<<"chargeConsistency:   "<<chargeConsistency<<endl;
    //cout<<"Conversion veto:     "<<passConversionVeto<<endl;
    //cout<<"trackerExpectedHitsInner:   "<<trackerExpectedHitsInner<<endl;
    //cout<<"chargeConsistency:   "<<chargeConsistency<<endl;
    //cout<<"delta Eta/Phi SuperClusterTrackAtVtx: "<<
	//deltaEtaSuperClusterTrackAtVtx<<" / "<< deltaPhiSuperClusterTrackAtVtx<<endl;
    //cout<<"sigmaIetaIeta:   "<<sigmaIetaIeta<<endl;
    //cout<<"hadronicOverEm:   "<<hadronicOverEm<<endl;
    //cout<<"dB:   "<<dB<<endl;
    //cout<<"ooemoop:   "<<ooemoop<<endl;

  }
  else{
    //Muons only
    //cout<<"Global / Tracker   "<<GlobalMuon<<" / "<< TrackerMuon<<endl;
    //cout<<"innerTrackLayers:    "<<innerTrackLayers<<endl;
    //cout<<"NormalizedChi2:    "<<nChi2<<endl;
    //cout<<"MuonHits:          "<<MuonHits<<endl;
    //cout<<"PixelHits:         "<<PixelHits<<endl;
    //cout<<"nMatch:            "<<nMatch<<endl;

//     if (goodMuon(MIN_LEP_PT)) cout << "Good muon\n";
//     else if (looseMuon(MIN_LEP_PT)) cout << "Loose muon\n";
  }

//   cout<<"MC match:          "<<matched<<" "<< origin()<<endl;
  if (fullMatching && matched) {
    cout<<"Matched lepton pt: "<<matchedLepton.lv.Pt()<<" eta: "<<matchedLepton.lv.Eta()
        <<" phi: "<<matchedLepton.lv.Phi()<<endl;
    cout <<"Mothers\n";
    for (int i = 0;i<10;++i )
      if (mother[i].pdgID!=0) cout<<i<<" : pdgID: "<< mother[i].pdgID<<" pt: "
	<< mother[i].lv.Pt()<<" eta: "<<mother[i].lv.Eta()
        <<" phi: "<<mother[i].lv.Phi()<<endl;
  }
  cout<<endl;
}

void TLepton::defaults(){
  MIN_LEP_PT	 = 30.;

  MAX_EL_ISO  = 999999.;
  MAX_MU_ISO  = 999999.;

  MAX_EL_ETA  = 2.5;
  MAX_MU_ETA  = 2.1;
}

// void TLepton::initLepton(){
//   lv.SetPxPyPzE(0,0,0,0);
//   relIso = SEN_HIGH;
//   ipXY = SEN_HIGH;
//   ipZ = SEN_HIGH;
//   charge = 0;
// 
//   //Electrons only
//   notEEEBGap_ = true;
//   chargeConsistency = false;
//   notConv = false;
// 
//   //Muons only
//   GlobalMuon = false;
//   TrackerMuon = false;
//   innerTrackLayers = SEN_LOW;
//   nChi2 = SEN_HIGH;
//   MuonHits = SEN_LOW;
//   PixelHits = SEN_LOW;
//   nMatch = SEN_LOW;
// //     relPtUnc = SEN_HIGH;
// }

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, std::vector<int> bTagTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), csvTag(bTagTemp)
{
  setLV();
  defaults();
}

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, double bTagDiscTemp, int jetFlavorTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), csvDisc(bTagDiscTemp), hadronFlavor(jetFlavorTemp)
{
  setLV();
  defaults();
}

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, std::vector<int> bTagTemp, double bTagDiscTemp, int jetFlavorTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), csvTag(bTagTemp), csvDisc(bTagDiscTemp), hadronFlavor(jetFlavorTemp)
{
  setLV();
  defaults();
}

/*TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, bool CAWDauTemp, 
	   int motherIndexTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), CAWDau(CAWDauTemp), 
  motherIndex(motherIndexTemp)
{
  setLV();
  defaults();
}

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, int indexTemp, 
	   int nDaughtersTemp, double massTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), index(indexTemp), 
  nDaughters(nDaughtersTemp), mass(massTemp)
{
  setLV();
  defaults();
}

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp, int indexTemp, 
	   int nDaughtersTemp, double topMassTemp, double minPairMassTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp), index(indexTemp), 
  nDaughters(nDaughtersTemp), topMass(topMassTemp), minPairMass(minPairMassTemp)
{
  setLV();
  defaults();
}*/

TJet::TJet(double ptTemp, double etaTemp, double phiTemp, double energyTemp) :
  pt(ptTemp), eta(etaTemp), phi(phiTemp), energy(energyTemp)
{
  setLV();
  defaults();
}

void TJet::setLV() {
  lv.SetPxPyPzE(pt*cos(phi), pt*sin(phi), pt*TMath::SinH(eta), energy);
}

void TJet::printJet(){

  if (lv.Pt() < 1){
    cout<<"Jet with pT < 1"<<endl;
//      return;
  }

  cout<<"pt: "<<lv.Pt()<<" eta: "<<lv.Eta()<<" phi: "<<lv.Phi()<<" energy: "<<lv.E()<<endl;
  cout<<"CSV:       "<<csvTag[0]  <<endl;
  cout<<"CSV Disc:  "<<csvDisc<<" Flavor: "<< hadronFlavor  <<endl;
  cout<<endl;
}


bool TJet::checkJetLeptondR(const TLepton & lep1){

  bool dRcut = true; 

  std::cout << "Lepton : Pt = " << lep1.lv.Pt() << " , Eta = " << lep1.lv.Eta() << " , Phi = " << lep1.lv.Phi() << std::endl;
  std::cout << "Jet : Pt = " << lv.Pt() << " , Eta = " << lv.Eta() << " , Phi = " << lv.Phi() << std::endl;

  if ((lv.Pt() < MIN_JET_PT) || fabs(lv.Eta()) > MAX_JET_ETA) return false; 

  if (lep1.lv.Pt() > MIN_LEP_PT){
    if (lep1.lv.DeltaR(lv) < 0.3) dRcut = false;
  }

  return dRcut;
}

bool TJet::checkJetJetdR(const TJet & jet1){

  bool dRcut = true; 

  if ((lv.Pt() < MIN_JET_PT) || fabs(lv.Eta()) > MAX_JET_ETA) return false; 

  if (jet1.lv.Pt() > MIN_LEP_PT){
    if (jet1.lv.DeltaR(lv) < 0.8) dRcut = false;
  }

  return dRcut;
}

void TJet::defaults(){
  MIN_JET_PT  = 30.;
  MAX_JET_ETA = 2.4;
  MIN_LEP_PT  = 30.;
}

