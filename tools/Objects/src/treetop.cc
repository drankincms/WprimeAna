#define treetop_cxx
#include "../interface/treetop.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <algorithm>

void treetop::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L treetop.C
//      Root > treetop t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

//    Long64_t nentries = fChain->GetEntriesFast();

//    Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<1;jentry++) {
//    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
cout << "Get\n";

   }
}

double treetop::getEntries()
{
  if (entries<0) entries = fChain->GetEntriesFast();
  return entries;
}; 


void treetop::printEvent(int jentry)
{
  GetEntry(jentry);
  printEvent();
}

void treetop::printEvent()
{
  cout << "run / ls / event = " << run << " / "<< lumi<< " / "<< event<<endl;
  cout << "npv = " << nPV << endl;
  cout << "nElectrons = " << nElectrons << endl;
  for (unsigned int i = 0;i<allElecs.size();++i ) allElecs[i]->printLepton();
  cout << "nMuons = " << nMuons << endl;  
  for (unsigned int i = 0;i<allMuons.size();++i ) allMuons[i]->printLepton();
  cout << "nAK4Jet = " << allAK4Jets.size() << endl;  
  for (unsigned int i = 0;i<allAK4Jets.size();++i ) allAK4Jets[i]->printJet();
cout <<"=======================\n";
}

void treetop::checkLeptons()
{
  goodMuons.clear();
  goodElecs.clear();

  goodLeptons.clear();


  for (unsigned int i = 0;i<allElecs.size();++i ) {
    goodElecs.push_back(allElecs[i]);
  }

  for (unsigned int i = 0;i<allMuons.size();++i ) {
    goodMuons.push_back(allMuons[i]);
  }

  vector <TLepton*> intLeptons;
  intLeptons.insert(intLeptons.end(), goodElecs.begin(), goodElecs.end());
  intLeptons.insert(intLeptons.end(), goodMuons.begin(), goodMuons.end());

  for (unsigned int i = 0;i<intLeptons.size();++i ) {
    bool pass=true;
    for (unsigned int j = 0;j<goodLeptons.size();++j ) 
      if (intLeptons[i]->lv.DeltaR(goodLeptons[j]->lv) < 0.1) pass=false; 
    if (pass) goodLeptons.push_back(intLeptons[i]);
  }

}

vector <TJet*> treetop::goodAK4Jets()
{
  vector <TJet*>  theJets;
  for (unsigned int i = 0; i < allAK4Jets.size(); i++) theJets.push_back(allAK4Jets[i]);
  sort(theJets.begin(), theJets.end(), SortJetsByPt());

  return theJets;
}

vector <TJet*> treetop::goodAK8Jets()
{
  vector <TJet*>  theJets;
  for (unsigned int i = 0; i < allAK8Jets.size(); i++) theJets.push_back(allAK8Jets[i]);
  sort(theJets.begin(), theJets.end(), SortJetsByPt());

  return theJets;
}


//I keep this for compatibility, but it is confusing
vector <TJet*> treetop::goodJets()
{
  vector <TJet*>  theJets;
  for (unsigned int i = 0;i<allJets.size();++i ) theJets.push_back(allJets[i]);
  sort(theJets.begin(), theJets.end(), SortJetsByPt());
  
  return theJets;

}


treetop::treetop(const TString &filename)
{
 cout << "Opening: "<<filename<<endl;
  TFile *f = new TFile(filename);
  f->cd();
  TTree *tree = (TTree*)gDirectory->Get("ljmet");
  Init(tree);
}

treetop::treetop(TTree *tree)
{
  Init(tree);
}
treetop::~treetop()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t treetop::GetEntry(Long64_t entry)
{
//Clear previous containers:
  for (unsigned int i = 0;i<allMuons.size();++i ) delete allMuons[i];
  for (unsigned int i = 0;i<allElecs.size();++i ) delete allElecs[i];
  for (unsigned int i = 0;i<allAK4Jets.size();++i ) delete allAK4Jets[i];
  for (unsigned int i = 0;i<allAK8Jets.size();++i ) delete allAK8Jets[i];
  for (unsigned int i = 0;i<allCleanedLeptons.size();++i ) delete allCleanedLeptons[i];
  allMuons.clear();
  allElecs.clear();
  allAK4Jets.clear();
  allAK8Jets.clear();
  allCleanedLeptons.clear();

  //I keep this for compatibility, but it is confusing and we should get rid of it
  for (unsigned int i = 0;i<allJets.size();++i ) delete allJets[i];
  allJets.clear();

  // Read contents of entry.
  if (!fChain) return 0;
  int stat =  fChain->GetEntry(entry);
  nMuons = muCharge->size();
  nElectrons = elPt->size();
  nAK4Jet = AK4JetBTag->size();
  //nAK8Jet = AK8JetPt->size();
  nMatchedMu = muMatchedPt->size();
  nMatchedEl = elMatchedPt->size();

  nPFjet = AK4JetBTag->size(); //It's not really all PF jets; it's all AK4 jets without considering AKW

  for (int i = 0;i<nMuons;++i ) 
    allMuons.push_back(new TLepton((*muCharge)[i], (*muPt)[i],
      (*muEta)[i], (*muPhi)[i], (*muEnergy)[i],
      (*muRelIso)[i], (*muMiniIso)[i], (*muDxy)[i], (*muDz)[i],
      (*muGlobal)[i],
      (*muInnerPt)[i], (*muInnerEta)[i], (*muInnerPhi)[i], ((*muInnerPt)[i])*TMath::CosH((*muInnerEta)[i]),
      (*muNTrackerLayers)[i],
      (*muChi2)[i], (*muNValMuHits)[i], (*muNValPixelHits)[i], (*muNMatchedStations)[i]));

  for (int i = 0;i<nElectrons;++i)
    allElecs.push_back(new TLepton((*elCharge)[i], (*elPt)[i],
      (*elEta)[i], (*elPhi)[i], (*elEnergy)[i],
      (*elRelIso)[i], (*elMiniIso)[i], (*elDxy)[i], (*elDZ)[i],
      (*elIsEBEE)[i], (*elChargeConsistent)[i],
      (*elVtxFitConv)[i], (*elDeta)[i], (*elDphi)[i], 
      (*elSihih)[i], (*elHoE)[i], (*elD0)[i], (*elDZ)[i], (*elOoemoop)[i],
      (*elMHits)[i], (*elVtxFitConv)[i]));

  for (int i = 0;i<nAK4Jet;++i ) {
    //std::cout << (*AK4JetPt)[i] << std::endl;
    std::vector<int> temp = {(*AK4JetBTag)[i], (*AK4JetBTag_bSFup)[i], (*AK4JetBTag_bSFdn)[i], (*AK4JetBTag_lSFup)[i], (*AK4JetBTag_lSFdn)[i]};
    allAK4Jets.push_back(new TJet((*AK4JetPt)[i],
      (*AK4JetEta)[i], (*AK4JetPhi)[i], (*AK4JetEnergy)[i], temp, (*AK4JetBDisc)[i], (*AK4JetFlav)[i]));
  }

  /*for (int i = 0;i<nAK8Jet;++i ) 
    allAK8Jets.push_back(new TJet((*AK8JetPt)[i],
				  (*AK8JetEta)[i], (*AK8JetPhi)[i], (*AK8JetEnergy)[i], (*AK8JetSoftDropMass)[i], (*AK8JetTrimmedMass)[i]));*/

  /*for (int i = 0;i<nAKTopJet;++i ) 
    allAKTopJets.push_back(new TJet((*AKTopJetPt)[i], (*AKTopJetEta)[i], (*AKTopJetPhi)[i], 
				    (*AKTopJetEnergy)[i], (*AKTopJetIndex)[i], (*AKTopJetnDaughters)[i],
				    //(*AKTopJetTopMass)[i], (*AKTopJetMinPairMass)[i]));
				    -1,-1));

  for (int i = 0;i<nAKWJet;++i ) 
    allAKWJets.push_back(new TJet((*AKWJetPt)[i], (*AKWJetEta)[i], (*AKWJetPhi)[i], 
				  (*AKWJetEnergy)[i], (*AKWJetIndex)[i], (*AKWJetnDaughters)[i],
				  (*AKWJetMass)[i]));*/

  /*for (int i = 0;i<nCleanedMu;++i ) 
    allCleanedLeptons.push_back(new TJet((*cleanedMuPt)[i],
      (*cleanedMuEta)[i], (*cleanedMuPhi)[i], (*cleanedMuEnergy)[i]));

  for (int i = 0;i<nCleanedEl;++i ) 
    allCleanedLeptons.push_back(new TJet((*cleanedElPt)[i],
      (*cleanedElEta)[i], (*cleanedElPhi)[i], (*cleanedElEnergy)[i]));*/

  //I keep this for compatibility, but it is confusing and we should get rid of it
  for (int i = 0;i<nAK4Jet;++i ) {
    std::vector<int> temp = {(*AK4JetBTag)[i], (*AK4JetBTag_bSFup)[i], (*AK4JetBTag_bSFdn)[i], (*AK4JetBTag_lSFup)[i], (*AK4JetBTag_lSFdn)[i]};
    allJets.push_back(new TJet((*AK4JetPt)[i],
      (*AK4JetEta)[i], (*AK4JetPhi)[i], (*AK4JetEnergy)[i], temp));
  }

  PF_met_px = PF_met_pt*cos(PF_met_phi);
  PF_met_py = PF_met_pt*sin(PF_met_phi);
  corr_met_px = corr_met*cos(corr_met_phi);
  corr_met_py = corr_met*sin(corr_met_phi);

  checkLeptons();
  return stat;
}

Long64_t treetop::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void treetop::Init(TTree *tree)
{

  if (!tree) return;

  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  entries = -1;

  weight_MC = 0;
  LHEweightids = 0;
  LHEweights = 0;

  nPV = -1;

  nTrueInteractions = 0;

  AK4JetBTag = 0;
  AK4JetBTag_bSFup = 0;
  AK4JetBTag_bSFdn = 0;
  AK4JetBTag_lSFup = 0;
  AK4JetBTag_lSFdn = 0;
  AK4JetBDisc = 0;
  AK4JetFlav = 0;
  /*AKWDaughterMotherIndex = 0;
  AKWJetIndex = 0;
  AKWJetnDaughters = 0;
  AKTopDaughterMotherIndex = 0;
  AKTopJetIndex = 0;
  AKTopJetnDaughters = 0;*/
  elChargeConsistent = 0;
  elCharge = 0;
  elIsEBEE = 0;
  elMHits = 0;
  elNotConversion = 0;
  elVtxFitConv = 0;

  genID = 0;
  genTDLID = 0;
  genBSLID = 0;
  genIndex = 0;
  genMotherID = 0;
  genMotherIndex = 0;
  genStatus = 0;
  muCharge = 0;
  muGlobal = 0;
  muNMatchedStations = 0;
  muNTrackerLayers = 0;
  muNValMuHits = 0;
  muNValPixelHits = 0;
  AK4JetEnergy = 0;
  AK4JetEta = 0;
  AK4JetPhi = 0;
  AK4JetPt = 0;
  /*AK8JetEnergy = 0;
  AK8JetEta = 0;
  AK8JetPhi = 0;
  AK8JetPt = 0;
  AK8JetSoftDropMass = 0;
  AK8JetTrimmedMass = 0;*/
  /*AKWDaughterEnergy = 0;
  AKWDaughterEta = 0;
  AKWDaughterPhi = 0;
  AKWDaughterPt = 0;
  AKWJetEnergy = 0;
  AKWJetEta = 0;
  AKWJetMass = 0;
  AKWJetPhi = 0;
  AKWJetPt = 0;
  AKTopDaughterEnergy = 0;
  AKTopDaughterEta = 0;
  AKTopDaughterPhi = 0;
  AKTopDaughterPt = 0;
  AKTopJetEnergy = 0;
  AKTopJetEta = 0;
  //AKTopJetMinPairMass = 0;
  AKTopJetPhi = 0;
  AKTopJetPt = 0;
  //AKTopJetTopMass = 0;*/
  elD0 = 0;
  elDZ = 0;
  elDeta = 0;
  elDphi = 0;
  elDxy = 0;
  elEnergy = 0;
  elEta = 0;
  elHoE = 0;
  elOoemoop = 0;
  elPhi = 0;
  elPt = 0;
  elRelIso = 0;
  elMiniIso = 0;
  elSihih = 0;
  genEnergy = 0;
  genEta = 0;
  genPhi = 0;
  genPt = 0;
  genTDLEnergy = 0;
  genTDLEta = 0;
  genTDLPhi = 0;
  genTDLPt = 0;
  genBSLEnergy = 0;
  genBSLEta = 0;
  genBSLPhi = 0;
  genBSLPt = 0;
  genJetEnergy = 0;
  genJetEta = 0;
  genJetPhi = 0;
  genJetPt = 0;
  muChi2 = 0;
  muDxy = 0;
  muDz = 0;
  muEnergy = 0;
  muEta = 0;
  muPhi = 0;
  muPt = 0;
  muInnerEta = 0;
  muInnerPhi = 0;
  muInnerPt = 0;
  muRelIso = 0;
  muMiniIso = 0;
  muMatchedEnergy = 0;
  muMatchedEta = 0;
  muMatchedPhi = 0;
  muMatchedPt = 0;
  elMatchedEnergy = 0;
  elMatchedEta = 0;
  elMatchedPhi = 0;
  elMatchedPt = 0;

  electron_1_hltmatched = 0;
  muon_1_hltmatched = 0;
  electron_hltfilters = 0;
  muon_hltfilters = 0;
  viSelMCTriggersMu = 0;
  viSelMCTriggersEl = 0;
  viSelTriggersMu = 0;
  viSelTriggersEl = 0;
  vsSelMCTriggersMu = 0;
  vsSelMCTriggersEl = 0;
  vsSelTriggersMu = 0;
  vsSelTriggersEl = 0;

//    // Set branch addresses and branch pointers
//    if (!tree) return;
//    fChain = tree;
//    fCurrent = -1;
//    fChain->SetMakeClass(1);
// 

  fChain->SetBranchAddress("run_CommonCalc", &run, &b_run_CommonCalc);
  fChain->SetBranchAddress("lumi_CommonCalc", &lumi, &b_lumi_CommonCalc);
  fChain->SetBranchAddress("event_CommonCalc", &event, &b_event_CommonCalc);

  //fChain->SetBranchAddress("bunchXing_PileUpCalc", &bunchXing_PileUpCalc, &b_bunchXing_PileUpCalc);

// 
// 
//    fChain->SetBranchAddress("nAllJets_CommonCalc", &nAllJets_CommonCalc, &b_nAllJets_CommonCalc);
//   fChain->SetBranchAddress("nInteractions_PileUpCalc", &nInteractions_PileUpCalc, &b_nInteractions_PileUpCalc);
  fChain->SetBranchAddress("nPV_singleLepCalc", &nPV, &b_nPV_singleLepCalc);

  fChain->SetBranchAddress("nTrueInteractions_PileUpCalc", &nTrueInteractions, &b_nTrueInteractions_PileUpCalc);
//    fChain->SetBranchAddress("nLooseMuons_CommonCalc", &nLooseMuons_CommonCalc, &b_nLooseMuons_CommonCalc);
// 
//    fChain->SetBranchAddress("nSelBtagJets_CommonCalc", &nSelBtagJets_CommonCalc, &b_nSelBtagJets_CommonCalc);
//    fChain->SetBranchAddress("nSelElectrons_CommonCalc", &nSelElectrons_CommonCalc, &b_nSelElectrons_CommonCalc);
//    fChain->SetBranchAddress("nSelJets_CommonCalc", &nSelJets_CommonCalc, &b_nSelJets_CommonCalc);
//    fChain->SetBranchAddress("nTightMuons_CommonCalc", &nTightMuons_CommonCalc, &b_nTightMuons_CommonCalc);
// 

  fChain->SetBranchAddress("corr_met_singleLepCalc", &corr_met, &b_corr_met_singleLepCalc);
  fChain->SetBranchAddress("corr_met_phi_singleLepCalc", &corr_met_phi, &b_corr_met_phi_singleLepCalc);

  fChain->SetBranchAddress("met_singleLepCalc", &PF_met_pt, &b_met_singleLepCalc);
  fChain->SetBranchAddress("met_phi_singleLepCalc", &PF_met_phi, &b_met_phi_singleLepCalc);

  //fChain->SetBranchAddress("weight_PU_PileUpCalc", &weight_PU, &b_weight_PU_PileUpCalc);
  fChain->SetBranchAddress("MCWeight_singleLepCalc", &weight_MC, &b_MCWeight_singleLepCalc);
  fChain->SetBranchAddress("LHEweightids_singleLepCalc", &LHEweightids, &b_LHEweightids_singleLepCalc);
  fChain->SetBranchAddress("LHEweights_singleLepCalc", &LHEweights, &b_LHEweights_singleLepCalc);
/*
  //AKW Daughter Lorentz vector
  fChain->SetBranchAddress("AKWDaughterEnergy_singleLepCalc", &AKWDaughterEnergy, &b_AKWDaughterEnergy_singleLepCalc);
  fChain->SetBranchAddress("AKWDaughterEta_singleLepCalc", &AKWDaughterEta, &b_AKWDaughterEta_singleLepCalc);
  fChain->SetBranchAddress("AKWDaughterPhi_singleLepCalc", &AKWDaughterPhi, &b_AKWDaughterPhi_singleLepCalc);
  fChain->SetBranchAddress("AKWDaughterPt_singleLepCalc", &AKWDaughterPt, &b_AKWDaughterPt_singleLepCalc);

  //AKW Mother Lorentz vector
  fChain->SetBranchAddress("AKWJetEnergy_singleLepCalc", &AKWJetEnergy, &b_AKWJetEnergy_singleLepCalc);
  fChain->SetBranchAddress("AKWJetEta_singleLepCalc", &AKWJetEta, &b_AKWJetEta_singleLepCalc);
  fChain->SetBranchAddress("AKWJetPhi_singleLepCalc", &AKWJetPhi, &b_AKWJetPhi_singleLepCalc);
  fChain->SetBranchAddress("AKWJetPt_singleLepCalc", &AKWJetPt, &b_AKWJetPt_singleLepCalc);

  //Other AKW quantities
  fChain->SetBranchAddress("AKWJetMass_singleLepCalc", &AKWJetMass, &b_AKWJetMass_singleLepCalc);
  fChain->SetBranchAddress("AKWDaughterMotherIndex_singleLepCalc", &AKWDaughterMotherIndex, &b_AKWDaughterMotherIndex_singleLepCalc);
  fChain->SetBranchAddress("AKWJetIndex_singleLepCalc", &AKWJetIndex, &b_AKWJetIndex_singleLepCalc);
  fChain->SetBranchAddress("AKWJetnDaughters_singleLepCalc", &AKWJetnDaughters, &b_AKWJetnDaughters_singleLepCalc);
 
  //AKTop Daughter Lorentz vector
  fChain->SetBranchAddress("AKTopDaughterEnergy_singleLepCalc", &AKTopDaughterEnergy, &b_AKTopDaughterEnergy_singleLepCalc);
  fChain->SetBranchAddress("AKTopDaughterEta_singleLepCalc", &AKTopDaughterEta, &b_AKTopDaughterEta_singleLepCalc);
  fChain->SetBranchAddress("AKTopDaughterPhi_singleLepCalc", &AKTopDaughterPhi, &b_AKTopDaughterPhi_singleLepCalc);
  fChain->SetBranchAddress("AKTopDaughterPt_singleLepCalc", &AKTopDaughterPt, &b_AKTopDaughterPt_singleLepCalc);

  //AKTop Mother Lorentz vector
  fChain->SetBranchAddress("AKTopJetEnergy_singleLepCalc", &AKTopJetEnergy, &b_AKTopJetEnergy_singleLepCalc);
  fChain->SetBranchAddress("AKTopJetEta_singleLepCalc", &AKTopJetEta, &b_AKTopJetEta_singleLepCalc);
  fChain->SetBranchAddress("AKTopJetPhi_singleLepCalc", &AKTopJetPhi, &b_AKTopJetPhi_singleLepCalc);
  fChain->SetBranchAddress("AKTopJetPt_singleLepCalc", &AKTopJetPt, &b_AKTopJetPt_singleLepCalc);

  //Other AKTop quantities
  //fChain->SetBranchAddress("AKTopJetMinPairMass_singleLepCalc", &AKTopJetMinPairMass, &b_AKTopJetMinPairMass_singleLepCalc);
  //fChain->SetBranchAddress("AKTopJetTopMass_singleLepCalc", &AKTopJetTopMass, &b_AKTopJetTopMass_singleLepCalc);

  fChain->SetBranchAddress("AKTopDaughterMotherIndex_singleLepCalc", &AKTopDaughterMotherIndex, &b_AKTopDaughterMotherIndex_singleLepCalc);
  fChain->SetBranchAddress("AKTopJetIndex_singleLepCalc", &AKTopJetIndex, &b_AKTopJetIndex_singleLepCalc);
  fChain->SetBranchAddress("AKTopJetnDaughters_singleLepCalc", &AKTopJetnDaughters, &b_AKTopJetnDaughters_singleLepCalc);*/

  //Trigger

  fChain->SetBranchAddress("electron_1_hltmatched_singleLepCalc", &electron_1_hltmatched, &b_electron_1_hltmatched_singleLepCalc);
  fChain->SetBranchAddress("muon_1_hltmatched_singleLepCalc", &muon_1_hltmatched, &b_muon_1_hltmatched_singleLepCalc);
  fChain->SetBranchAddress("electron_hltfilters_singleLepCalc", &electron_hltfilters, &b_electron_hltfilters_singleLepCalc);
  fChain->SetBranchAddress("muon_hltfilters_singleLepCalc", &muon_hltfilters, &b_muon_hltfilters_singleLepCalc);
  fChain->SetBranchAddress("viSelMCTriggersEl_singleLepCalc", &viSelMCTriggersEl, &b_viSelMCTriggersEl_singleLepCalc);
  fChain->SetBranchAddress("viSelMCTriggersMu_singleLepCalc", &viSelMCTriggersMu, &b_viSelMCTriggersMu_singleLepCalc);
  fChain->SetBranchAddress("viSelTriggersEl_singleLepCalc", &viSelTriggersEl, &b_viSelTriggersEl_singleLepCalc);
  fChain->SetBranchAddress("viSelTriggersMu_singleLepCalc", &viSelTriggersMu, &b_viSelTriggersMu_singleLepCalc);
  fChain->SetBranchAddress("vsSelMCTriggersEl_singleLepCalc", &vsSelMCTriggersEl, &b_vsSelMCTriggersEl_singleLepCalc);
  fChain->SetBranchAddress("vsSelMCTriggersMu_singleLepCalc", &vsSelMCTriggersMu, &b_vsSelMCTriggersMu_singleLepCalc);
  fChain->SetBranchAddress("vsSelTriggersEl_singleLepCalc", &vsSelTriggersEl, &b_vsSelTriggersEl_singleLepCalc);
  fChain->SetBranchAddress("vsSelTriggersMu_singleLepCalc", &vsSelTriggersMu, &b_vsSelTriggersMu_singleLepCalc);

  //Electrons
  fChain->SetBranchAddress("elChargeConsistent_singleLepCalc", &elChargeConsistent, &b_elChargeConsistent_singleLepCalc);
  fChain->SetBranchAddress("elCharge_singleLepCalc", &elCharge, &b_elCharge_singleLepCalc);
  fChain->SetBranchAddress("elIsEBEE_singleLepCalc", &elIsEBEE, &b_elIsEBEE_singleLepCalc);
  fChain->SetBranchAddress("elMHits_singleLepCalc", &elMHits, &b_elMHits_singleLepCalc);
  fChain->SetBranchAddress("elNotConversion_singleLepCalc", &elNotConversion, &b_elNotConversion_singleLepCalc);
  fChain->SetBranchAddress("elVtxFitConv_singleLepCalc", &elVtxFitConv, &b_elVtxFitConv_singleLepCalc);
  fChain->SetBranchAddress("elD0_singleLepCalc", &elD0, &b_elD0_singleLepCalc);
  fChain->SetBranchAddress("elDZ_singleLepCalc", &elDZ, &b_elDZ_singleLepCalc);
  fChain->SetBranchAddress("elDeta_singleLepCalc", &elDeta, &b_elDeta_singleLepCalc);
  fChain->SetBranchAddress("elDphi_singleLepCalc", &elDphi, &b_elDphi_singleLepCalc);
  fChain->SetBranchAddress("elDxy_singleLepCalc", &elDxy, &b_elDxy_singleLepCalc);
  fChain->SetBranchAddress("elEnergy_singleLepCalc", &elEnergy, &b_elEnergy_singleLepCalc);
  fChain->SetBranchAddress("elEta_singleLepCalc", &elEta, &b_elEta_singleLepCalc);
  fChain->SetBranchAddress("elHoE_singleLepCalc", &elHoE, &b_elHoE_singleLepCalc);
  fChain->SetBranchAddress("elOoemoop_singleLepCalc", &elOoemoop, &b_elOoemoop_singleLepCalc);
  fChain->SetBranchAddress("elPhi_singleLepCalc", &elPhi, &b_elPhi_singleLepCalc);
  fChain->SetBranchAddress("elPt_singleLepCalc", &elPt, &b_elPt_singleLepCalc);
  fChain->SetBranchAddress("elRelIso_singleLepCalc", &elRelIso, &b_elRelIso_singleLepCalc);
  fChain->SetBranchAddress("elMiniIso_singleLepCalc", &elMiniIso, &b_elMiniIso_singleLepCalc);
  fChain->SetBranchAddress("elSihih_singleLepCalc", &elSihih, &b_elSihih_singleLepCalc);

  //Muons
  fChain->SetBranchAddress("muCharge_singleLepCalc", &muCharge, &b_muCharge_singleLepCalc);
  fChain->SetBranchAddress("muGlobal_singleLepCalc", &muGlobal, &b_muGlobal_singleLepCalc);
  fChain->SetBranchAddress("muNMatchedStations_singleLepCalc", &muNMatchedStations, &b_muNMatchedStations_singleLepCalc);
  fChain->SetBranchAddress("muNTrackerLayers_singleLepCalc", &muNTrackerLayers, &b_muNTrackerLayers_singleLepCalc);
  fChain->SetBranchAddress("muNValMuHits_singleLepCalc", &muNValMuHits, &b_muNValMuHits_singleLepCalc);
  fChain->SetBranchAddress("muNValPixelHits_singleLepCalc", &muNValPixelHits, &b_muNValPixelHits_singleLepCalc);
  fChain->SetBranchAddress("muChi2_singleLepCalc", &muChi2, &b_muChi2_singleLepCalc);
  fChain->SetBranchAddress("muDxy_singleLepCalc", &muDxy, &b_muDxy_singleLepCalc);
  fChain->SetBranchAddress("muDz_singleLepCalc", &muDz, &b_muDz_singleLepCalc);
  fChain->SetBranchAddress("muEnergy_singleLepCalc", &muEnergy, &b_muEnergy_singleLepCalc);
  fChain->SetBranchAddress("muEta_singleLepCalc", &muEta, &b_muEta_singleLepCalc);
  fChain->SetBranchAddress("muPhi_singleLepCalc", &muPhi, &b_muPhi_singleLepCalc);
  fChain->SetBranchAddress("muPt_singleLepCalc", &muPt, &b_muPt_singleLepCalc);
  fChain->SetBranchAddress("muInnerEta_singleLepCalc", &muInnerEta, &b_muInnerEta_singleLepCalc);
  fChain->SetBranchAddress("muInnerPhi_singleLepCalc", &muInnerPhi, &b_muInnerPhi_singleLepCalc);
  fChain->SetBranchAddress("muInnerPt_singleLepCalc", &muInnerPt, &b_muInnerPt_singleLepCalc);
  fChain->SetBranchAddress("muRelIso_singleLepCalc", &muRelIso, &b_muRelIso_singleLepCalc);
  fChain->SetBranchAddress("muMiniIso_singleLepCalc", &muMiniIso, &b_muMiniIso_singleLepCalc);

  fChain->SetBranchAddress("AK4JetBTag_singleLepCalc", &AK4JetBTag, &b_AK4JetBTag_singleLepCalc);
  fChain->SetBranchAddress("AK4JetBTag_bSFup_singleLepCalc", &AK4JetBTag_bSFup, &b_AK4JetBTag_bSFup_singleLepCalc);
  fChain->SetBranchAddress("AK4JetBTag_bSFdn_singleLepCalc", &AK4JetBTag_bSFdn, &b_AK4JetBTag_bSFdn_singleLepCalc);
  fChain->SetBranchAddress("AK4JetBTag_lSFup_singleLepCalc", &AK4JetBTag_lSFup, &b_AK4JetBTag_lSFup_singleLepCalc);
  fChain->SetBranchAddress("AK4JetBTag_lSFdn_singleLepCalc", &AK4JetBTag_lSFdn, &b_AK4JetBTag_lSFdn_singleLepCalc);
  fChain->SetBranchAddress("AK4JetBDisc_singleLepCalc", &AK4JetBDisc, &b_AK4JetBDisc_singleLepCalc);
  fChain->SetBranchAddress("AK4JetFlav_singleLepCalc", &AK4JetFlav, &b_AK4JetFlav_singleLepCalc);
  fChain->SetBranchAddress("AK4JetEnergy_singleLepCalc", &AK4JetEnergy, &b_AK4JetEnergy_singleLepCalc);
  fChain->SetBranchAddress("AK4JetEta_singleLepCalc", &AK4JetEta, &b_AK4JetEta_singleLepCalc);
  fChain->SetBranchAddress("AK4JetPhi_singleLepCalc", &AK4JetPhi, &b_AK4JetPhi_singleLepCalc);
  fChain->SetBranchAddress("AK4JetPt_singleLepCalc", &AK4JetPt, &b_AK4JetPt_singleLepCalc);

  /*fChain->SetBranchAddress("theJetAK8Energy_JetSubCalc", &AK8JetEnergy, &b_theJetAK8Energy_JetSubCalc);
  fChain->SetBranchAddress("theJetAK8Eta_JetSubCalc", &AK8JetEta, &b_theJetAK8Eta_JetSubCalc);
  fChain->SetBranchAddress("theJetAK8Phi_JetSubCalc", &AK8JetPhi, &b_theJetAK8Phi_JetSubCalc);
  fChain->SetBranchAddress("theJetAK8Pt_JetSubCalc", &AK8JetPt, &b_theJetAK8Pt_JetSubCalc);
  fChain->SetBranchAddress("theJetAK8SoftDropMass_JetSubCalc", &AK8JetSoftDropMass, &b_theJetAK8SoftDropMass_JetSubCalc);
  fChain->SetBranchAddress("theJetAK8TrimmedMass_JetSubCalc", &AK8JetTrimmedMass, &b_theJetAK8TrimmedMass_JetSubCalc);*/

  //Gen Info
  fChain->SetBranchAddress("genID_singleLepCalc", &genID, &b_genID_singleLepCalc);
  fChain->SetBranchAddress("genTDLID_singleLepCalc", &genTDLID, &b_genTDLID_singleLepCalc);
  fChain->SetBranchAddress("genBSLID_singleLepCalc", &genBSLID, &b_genBSLID_singleLepCalc);
  fChain->SetBranchAddress("genIndex_singleLepCalc", &genIndex, &b_genIndex_singleLepCalc);
  fChain->SetBranchAddress("genMotherID_singleLepCalc", &genMotherID, &b_genMotherID_singleLepCalc);
  fChain->SetBranchAddress("genMotherIndex_singleLepCalc", &genMotherIndex, &b_genMotherIndex_singleLepCalc);
  fChain->SetBranchAddress("genStatus_singleLepCalc", &genStatus, &b_genStatus_singleLepCalc);

  fChain->SetBranchAddress("genEnergy_singleLepCalc", &genEnergy, &b_genEnergy_singleLepCalc);
  fChain->SetBranchAddress("genEta_singleLepCalc", &genEta, &b_genEta_singleLepCalc);
  fChain->SetBranchAddress("genPhi_singleLepCalc", &genPhi, &b_genPhi_singleLepCalc);
  fChain->SetBranchAddress("genPt_singleLepCalc", &genPt, &b_genPt_singleLepCalc);

  fChain->SetBranchAddress("genTDLEnergy_singleLepCalc", &genTDLEnergy, &b_genTDLEnergy_singleLepCalc);
  fChain->SetBranchAddress("genTDLEta_singleLepCalc", &genTDLEta, &b_genTDLEta_singleLepCalc);
  fChain->SetBranchAddress("genTDLPhi_singleLepCalc", &genTDLPhi, &b_genTDLPhi_singleLepCalc);
  fChain->SetBranchAddress("genTDLPt_singleLepCalc", &genTDLPt, &b_genTDLPt_singleLepCalc);

  fChain->SetBranchAddress("genBSLEnergy_singleLepCalc", &genBSLEnergy, &b_genBSLEnergy_singleLepCalc);
  fChain->SetBranchAddress("genBSLEta_singleLepCalc", &genBSLEta, &b_genBSLEta_singleLepCalc);
  fChain->SetBranchAddress("genBSLPhi_singleLepCalc", &genBSLPhi, &b_genBSLPhi_singleLepCalc);
  fChain->SetBranchAddress("genBSLPt_singleLepCalc", &genBSLPt, &b_genBSLPt_singleLepCalc);

  fChain->SetBranchAddress("genJetEnergy_singleLepCalc", &genJetEnergy, &b_genJetEnergy_singleLepCalc);
  fChain->SetBranchAddress("genJetEta_singleLepCalc", &genJetEta, &b_genJetEta_singleLepCalc);
  fChain->SetBranchAddress("genJetPhi_singleLepCalc", &genJetPhi, &b_genJetPhi_singleLepCalc);
  fChain->SetBranchAddress("genJetPt_singleLepCalc", &genJetPt, &b_genJetPt_singleLepCalc);

  //Cleaned leptons
  fChain->SetBranchAddress("muMatchedEnergy_singleLepCalc", &muMatchedEnergy, &b_muMatchedEnergy_singleLepCalc);
  fChain->SetBranchAddress("muMatchedEta_singleLepCalc", &muMatchedEta, &b_muMatchedEta_singleLepCalc);
  fChain->SetBranchAddress("muMatchedPhi_singleLepCalc", &muMatchedPhi, &b_muMatchedPhi_singleLepCalc);
  fChain->SetBranchAddress("muMatchedPt_singleLepCalc", &muMatchedPt, &b_muMatchedPt_singleLepCalc);

  fChain->SetBranchAddress("elMatchedEnergy_singleLepCalc", &elMatchedEnergy, &b_elMatchedEnergy_singleLepCalc);
  fChain->SetBranchAddress("elMatchedEta_singleLepCalc", &elMatchedEta, &b_elMatchedEta_singleLepCalc);
  fChain->SetBranchAddress("elMatchedPhi_singleLepCalc", &elMatchedPhi, &b_elMatchedPhi_singleLepCalc);
  fChain->SetBranchAddress("elMatchedPt_singleLepCalc", &elMatchedPt, &b_elMatchedPt_singleLepCalc);

  Notify();
}

Bool_t treetop::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

void treetop::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
