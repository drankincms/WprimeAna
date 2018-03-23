//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug 13 03:17:52 2012 by ROOT version 5.27/06b
// from TTree treetop/treetop
// found on file: TTbar_Aug13_all.root
//////////////////////////////////////////////////////////

#ifndef treetop_h
#define treetop_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include "TLepton.h"
#include <stdio.h>


class treetop {
 public :
   struct SortJetsByPt {
       bool operator()(const TJet *e1, const TJet *e2) {
           return (e1->lv.Pt() > e2->lv.Pt());
       }
   };


  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  vector <TLepton*> allMuons, allElecs, goodMuons, goodElecs;
  vector <TJet*> allAK4Jets, allAK8Jets, allJets; //allJets is obsolete! It's really all AK4 jets
  vector <TJet*> allCleanedLeptons;
  // The goodLeptons and leptonsForFakes are ordered in pT and overlaps are cleaned (dR<0.1)
  vector <TLepton*> goodLeptons;
  
  vector <TJet*> goodAK8Jets();
  vector <TJet*> goodAK4Jets();
  vector <TJet*> goodJets(); //goodJets is really good AK4 jets without considering AK jets
  //int goodLeptons();


  //to remove:
//
  Int_t           nPV;
  Int_t           run, lumi, event;
  
  Int_t           nAK4Jet;
  Int_t           nAK8Jet;
  Int_t           nPFjet;
  Int_t           nElectrons;
  Int_t           nMuons;
  Int_t           nMatchedMu;
  Int_t           nMatchedEl;

  Int_t           event_flavor; // Flavour of the leading jet
  Int_t           n_Bjets, n_Cjets, n_Lightjets;
  
// Event Weights 
  //Double_t        weight_PU_3D;
  //Double_t        weight_PU_RootFile;
  //Double_t        weight_PU;
  Double_t          weight_MC;
  vector<int>       *LHEweightids;
  vector<double>    *LHEweights;

// MET
  Double_t        PF_met_phi;
  Double_t        PF_met_pt;
  Double_t        PF_met_px;
  Double_t        PF_met_py;
  Double_t        corr_met;
  Double_t        corr_met_px;
  Double_t        corr_met_py;
  Double_t        corr_met_phi;


// Primary vertex
  Double_t        Nchi2PV;
  Double_t        PV1Z;
  Double_t        PV2Z;
  Double_t        PV3Z;
  Double_t        PV4Z;
  Double_t        chi2PV;
  Double_t        ndofPV;

  // Declaration of leaf types
  //Int_t           bunchXing_PileUpCalc;
  Int_t           event_CommonCalc;
  Int_t           nAllJets_CommonCalc;
  //Int_t           nInteractions_PileUpCalc;
  Double_t        nTrueInteractions;
  Int_t           nLooseMuons_CommonCalc;
  Int_t           nSelBtagJets_CommonCalc;
  Int_t           nSelElectrons_CommonCalc;
  Int_t           nSelJets_CommonCalc;
  Int_t           nTightMuons_CommonCalc;
  Int_t           run_CommonCalc;
  Int_t           nPV_singleLepCalc;
  vector<int>     *AK4JetBTag;
  vector<int>     *AK4JetBTag_bSFup;
  vector<int>     *AK4JetBTag_bSFdn;
  vector<int>     *AK4JetBTag_lSFup;
  vector<int>     *AK4JetBTag_lSFdn;
  vector<double>  *AK4JetBDisc;
  vector<int>     *AK4JetFlav;
  vector<int>     *AKWDaughterMotherIndex;
  vector<int>     *AKWJetIndex;
  vector<int>     *AKWJetnDaughters;
  vector<int>     *AKTopDaughterMotherIndex;
  vector<int>     *AKTopJetIndex;
  vector<int>     *AKTopJetnDaughters;
  vector<int>     *elChargeConsistent;
  vector<int>     *elCharge;
  vector<int>     *elIsEBEE;
  vector<int>     *elMHits;
  vector<int>     *elNotConversion;
  //Triggers
  vector<int>     *electron_1_hltmatched;
  vector<int>     *muon_1_hltmatched;
  vector<std::string>   *electron_hltfilters;
  vector<std::string>   *muon_hltfilters;
  vector<int>     *viSelMCTriggersEl;
  vector<int>     *viSelMCTriggersMu;
  vector<int>     *viSelTriggersEl;
  vector<int>     *viSelTriggersMu;
  vector<std::string>   *vsSelMCTriggersEl;
  vector<std::string>   *vsSelMCTriggersMu;
  vector<std::string>   *vsSelTriggersEl;
  vector<std::string>   *vsSelTriggersMu;
  //vector<int>     *elQuality;
  vector<int>     *elVtxFitConv;
  vector<int>     *genID;
  int             genTDLID;
  vector<int>     *genBSLID;
  vector<int>     *genIndex;
  vector<int>     *genMotherID;
  vector<int>     *genMotherIndex;
  vector<int>     *genStatus;
  vector<int>     *muCharge;
  vector<int>     *muGlobal;
  vector<int>     *muNMatchedStations;
  vector<int>     *muNTrackerLayers;
  vector<int>     *muNValMuHits;
  vector<int>     *muNValPixelHits;
  vector<double>  *AK4JetEnergy;
  vector<double>  *AK4JetEta;
  vector<double>  *AK4JetPhi;
  vector<double>  *AK4JetPt;
  /*vector<double>  *AK8JetEnergy;
  vector<double>  *AK8JetEta;
  vector<double>  *AK8JetPhi;
  vector<double>  *AK8JetPt;
  vector<double>  *AK8JetSoftDropMass;
  vector<double>  *AK8JetTrimmedMass;*/
  /*vector<double>  *AKWDaughterEnergy;
  vector<double>  *AKWDaughterEta;
  vector<double>  *AKWDaughterPhi;
  vector<double>  *AKWDaughterPt;
  vector<double>  *AKWJetEnergy;
  vector<double>  *AKWJetEta;
  vector<double>  *AKWJetMass;
  vector<double>  *AKWJetPhi;
  vector<double>  *AKWJetPt;
  vector<double>  *AKTopDaughterEnergy;
  vector<double>  *AKTopDaughterEta;
  vector<double>  *AKTopDaughterPhi;
  vector<double>  *AKTopDaughterPt;
  vector<double>  *AKTopJetEnergy;
  vector<double>  *AKTopJetEta;
  //vector<double>  *AKTopJetMinPairMass;
  vector<double>  *AKTopJetPhi;
  vector<double>  *AKTopJetPt;
  //vector<double>  *AKTopJetTopMass;*/
  vector<double>  *elD0;
  vector<double>  *elDZ;
  vector<double>  *elDeta;
  vector<double>  *elDphi;
  vector<double>  *elDxy;
  vector<double>  *elEnergy;
  vector<double>  *elEta;
  vector<double>  *elHoE;
  vector<double>  *elOoemoop;
  vector<double>  *elPhi;
  vector<double>  *elPt;
  vector<double>  *elRelIso;
  vector<double>  *elMiniIso;
  vector<double>  *elSihih;
  vector<double>  *genEnergy;
  vector<double>  *genEta;
  vector<double>  *genPhi;
  vector<double>  *genPt;
  double          genTDLEnergy;
  double          genTDLEta;
  double          genTDLPhi;
  double          genTDLPt;
  vector<double>  *genBSLEnergy;
  vector<double>  *genBSLEta;
  vector<double>  *genBSLPhi;
  vector<double>  *genBSLPt;
  vector<double>  *genJetEnergy;
  vector<double>  *genJetEta;
  vector<double>  *genJetPhi;
  vector<double>  *genJetPt;
  vector<double>  *muChi2;
  vector<double>  *muDxy;
  vector<double>  *muDz;
  vector<double>  *muEnergy;
  vector<double>  *muEta;
  vector<double>  *muPhi;
  vector<double>  *muPt;
  vector<double>  *muInnerEta;
  vector<double>  *muInnerPhi;
  vector<double>  *muInnerPt;
  vector<double>  *muRelIso;
  vector<double>  *muMiniIso;
  vector<double>  *muMatchedEnergy;
  vector<double>  *muMatchedEta;
  vector<double>  *muMatchedPhi;
  vector<double>  *muMatchedPt;
  vector<double>  *elMatchedEnergy;
  vector<double>  *elMatchedEta;
  vector<double>  *elMatchedPhi;
  vector<double>  *elMatchedPt;

  // List of branches
  //TBranch        *b_bunchXing_PileUpCalc;   //!
  TBranch        *b_event_CommonCalc;   //!
  TBranch        *b_lumi_CommonCalc;   //!
  TBranch        *b_nAllJets_CommonCalc;   //!
  //TBranch        *b_nInteractions_PileUpCalc;   //!
  TBranch        *b_nTrueInteractions_PileUpCalc;   //!
  TBranch        *b_nLooseMuons_CommonCalc;   //!
  TBranch        *b_nSelBtagJets_CommonCalc;   //!
  TBranch        *b_nSelElectrons_CommonCalc;   //!
  TBranch        *b_nSelJets_CommonCalc;   //!
  TBranch        *b_nTightMuons_CommonCalc;   //!
  TBranch        *b_run_CommonCalc;   //!
  TBranch        *b_corr_met_singleLepCalc;   //!
  TBranch        *b_corr_met_phi_singleLepCalc;   //!
  TBranch        *b_met_singleLepCalc;   //!
  TBranch        *b_met_phi_singleLepCalc;   //!
  //TBranch        *b_weight_PU_PileUpCalc;   //!
  TBranch        *b_MCWeight_singleLepCalc;   //!
  TBranch        *b_LHEweightids_singleLepCalc;   //!
  TBranch        *b_LHEweights_singleLepCalc;   //!
  TBranch        *b_AK4JetBTag_singleLepCalc;   //!
  TBranch        *b_AK4JetBTag_bSFup_singleLepCalc;   //!
  TBranch        *b_AK4JetBTag_bSFdn_singleLepCalc;   //!
  TBranch        *b_AK4JetBTag_lSFup_singleLepCalc;   //!
  TBranch        *b_AK4JetBTag_lSFdn_singleLepCalc;   //!
  TBranch        *b_AK4JetBDisc_singleLepCalc;   //!
  TBranch        *b_AK4JetFlav_singleLepCalc;   //!
  /*TBranch        *b_AKWDaughterMotherIndex_singleLepCalc;   //!
  TBranch        *b_AKWJetIndex_singleLepCalc;   //!
  TBranch        *b_AKWJetnDaughters_singleLepCalc;   //!
  TBranch        *b_AKTopDaughterMotherIndex_singleLepCalc;   //!
  TBranch        *b_AKTopJetIndex_singleLepCalc;   //!
  TBranch        *b_AKTopJetnDaughters_singleLepCalc;   //!*/

  TBranch        *b_electron_1_hltmatched_singleLepCalc;  //!
  TBranch        *b_muon_1_hltmatched_singleLepCalc;  //!
  TBranch        *b_electron_hltfilters_singleLepCalc;  //!
  TBranch        *b_muon_hltfilters_singleLepCalc;  //!
  TBranch        *b_viSelMCTriggersEl_singleLepCalc;  //!
  TBranch        *b_viSelMCTriggersMu_singleLepCalc;  //!
  TBranch        *b_viSelTriggersEl_singleLepCalc;  //!
  TBranch        *b_viSelTriggersMu_singleLepCalc;  //!
  TBranch        *b_vsSelMCTriggersEl_singleLepCalc;  //!
  TBranch        *b_vsSelMCTriggersMu_singleLepCalc;  //!
  TBranch        *b_vsSelTriggersEl_singleLepCalc;  //!
  TBranch        *b_vsSelTriggersMu_singleLepCalc;  //!

  TBranch        *b_elChargeConsistent_singleLepCalc;   //!
  TBranch        *b_elCharge_singleLepCalc;   //!
  TBranch        *b_elIsEBEE_singleLepCalc;   //!
  TBranch        *b_elMHits_singleLepCalc;   //!
  TBranch        *b_elNotConversion_singleLepCalc;   //!
  //TBranch        *b_elQuality_singleLepCalc;   //!
  TBranch        *b_elVtxFitConv_singleLepCalc;   //!
  TBranch        *b_genID_singleLepCalc;   //!
  TBranch        *b_genTDLID_singleLepCalc;   //!
  TBranch        *b_genBSLID_singleLepCalc;   //!
  TBranch        *b_genIndex_singleLepCalc;   //!
  TBranch        *b_genMotherID_singleLepCalc;   //!
  TBranch        *b_genMotherIndex_singleLepCalc;   //!
  TBranch        *b_genStatus_singleLepCalc;   //!
  TBranch        *b_muCharge_singleLepCalc;   //!
  TBranch        *b_muGlobal_singleLepCalc;   //!
  TBranch        *b_muNMatchedStations_singleLepCalc;   //!
  TBranch        *b_muNTrackerLayers_singleLepCalc;   //!
  TBranch        *b_muNValMuHits_singleLepCalc;   //!
  TBranch        *b_muNValPixelHits_singleLepCalc;   //!
  TBranch        *b_AK4JetEnergy_singleLepCalc;   //!
  TBranch        *b_AK4JetEta_singleLepCalc;   //!
  TBranch        *b_AK4JetPhi_singleLepCalc;   //!
  TBranch        *b_AK4JetPt_singleLepCalc;   //!
  /*TBranch        *b_theJetAK8Energy_JetSubCalc;   //!
  TBranch        *b_theJetAK8Eta_JetSubCalc;   //!
  TBranch        *b_theJetAK8Phi_JetSubCalc;   //!
  TBranch        *b_theJetAK8Pt_JetSubCalc;   //!
  TBranch        *b_theJetAK8SoftDropMass_JetSubCalc;   //!
  TBranch        *b_theJetAK8TrimmedMass_JetSubCalc;   //!*/
  /*TBranch        *b_AK8JetEnergy_singleLepCalc;   //!
  TBranch        *b_AK8JetEta_singleLepCalc;   //!
  TBranch        *b_AK8JetPhi_singleLepCalc;   //!
  TBranch        *b_AK8JetPt_singleLepCalc;   //!*/
  /*TBranch        *b_AKWDaughterEnergy_singleLepCalc;   //!
  TBranch        *b_AKWDaughterEta_singleLepCalc;   //!
  TBranch        *b_AKWDaughterPhi_singleLepCalc;   //!
  TBranch        *b_AKWDaughterPt_singleLepCalc;   //!
  TBranch        *b_AKWJetEnergy_singleLepCalc;   //!
  TBranch        *b_AKWJetEta_singleLepCalc;   //!
  TBranch        *b_AKWJetMass_singleLepCalc;   //!
  TBranch        *b_AKWJetPhi_singleLepCalc;   //!
  TBranch        *b_AKWJetPt_singleLepCalc;   //!
  TBranch        *b_AKTopDaughterEnergy_singleLepCalc;   //!
  TBranch        *b_AKTopDaughterEta_singleLepCalc;   //!
  TBranch        *b_AKTopDaughterPhi_singleLepCalc;   //!
  TBranch        *b_AKTopDaughterPt_singleLepCalc;   //!
  TBranch        *b_AKTopJetEnergy_singleLepCalc;   //!
  TBranch        *b_AKTopJetEta_singleLepCalc;   //!
  //TBranch        *b_AKTopJetMinPairMass_singleLepCalc;   //!
  TBranch        *b_AKTopJetPhi_singleLepCalc;   //!
  TBranch        *b_AKTopJetPt_singleLepCalc;   //!
  //TBranch        *b_AKTopJetTopMass_singleLepCalc;   //!*/
  TBranch        *b_elD0_singleLepCalc;   //!
  TBranch        *b_elDZ_singleLepCalc;   //!
  TBranch        *b_elDeta_singleLepCalc;   //!
  TBranch        *b_elDphi_singleLepCalc;   //!
  TBranch        *b_elDxy_singleLepCalc;   //!
  TBranch        *b_elEnergy_singleLepCalc;   //!
  TBranch        *b_elEta_singleLepCalc;   //!
  TBranch        *b_elHoE_singleLepCalc;   //!
  TBranch        *b_elOoemoop_singleLepCalc;   //!
  TBranch        *b_elPhi_singleLepCalc;   //!
  TBranch        *b_elPt_singleLepCalc;   //!
  TBranch        *b_elRelIso_singleLepCalc;   //!
  TBranch        *b_elMiniIso_singleLepCalc;   //!
  TBranch        *b_elSihih_singleLepCalc;   //!
  TBranch        *b_genEnergy_singleLepCalc;   //!
  TBranch        *b_genEta_singleLepCalc;   //!
  TBranch        *b_genPhi_singleLepCalc;   //!
  TBranch        *b_genPt_singleLepCalc;   //!
  TBranch        *b_genTDLEnergy_singleLepCalc;   //!
  TBranch        *b_genTDLEta_singleLepCalc;   //!
  TBranch        *b_genTDLPhi_singleLepCalc;   //!
  TBranch        *b_genTDLPt_singleLepCalc;   //!
  TBranch        *b_genBSLEnergy_singleLepCalc;   //!
  TBranch        *b_genBSLEta_singleLepCalc;   //!
  TBranch        *b_genBSLPhi_singleLepCalc;   //!
  TBranch        *b_genBSLPt_singleLepCalc;   //!
  TBranch        *b_genJetEnergy_singleLepCalc;   //!
  TBranch        *b_genJetEta_singleLepCalc;   //!
  TBranch        *b_genJetPhi_singleLepCalc;   //!
  TBranch        *b_genJetPt_singleLepCalc;   //!
  TBranch        *b_muChi2_singleLepCalc;   //!
  TBranch        *b_muDxy_singleLepCalc;   //!
  TBranch        *b_muDz_singleLepCalc;   //!
  TBranch        *b_muEnergy_singleLepCalc;   //!
  TBranch        *b_muEta_singleLepCalc;   //!
  TBranch        *b_muPhi_singleLepCalc;   //!
  TBranch        *b_muPt_singleLepCalc;   //!
  TBranch        *b_muInnerEta_singleLepCalc;   //!
  TBranch        *b_muInnerPhi_singleLepCalc;   //!
  TBranch        *b_muInnerPt_singleLepCalc;   //!
  TBranch        *b_muRelIso_singleLepCalc;   //!
  TBranch        *b_muMiniIso_singleLepCalc;   //!
  TBranch        *b_muMatchedEnergy_singleLepCalc;   //!
  TBranch        *b_muMatchedEta_singleLepCalc;   //!
  TBranch        *b_muMatchedPhi_singleLepCalc;   //!
  TBranch        *b_muMatchedPt_singleLepCalc;   //!
  TBranch        *b_elMatchedEnergy_singleLepCalc;   //!
  TBranch        *b_elMatchedEta_singleLepCalc;   //!
  TBranch        *b_elMatchedPhi_singleLepCalc;   //!
  TBranch        *b_elMatchedPt_singleLepCalc;   //!
  TBranch        *b_nPV_singleLepCalc;   //!


  treetop(TTree *tree);
  treetop(const TString &filename);
  virtual ~treetop();
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  double getEntries();
  void printEvent(int jentry);
  void printEvent();
  void checkLeptons();
  double entries;
};

#endif

#ifdef treetop_cxx

#endif
