#ifndef WprimeAnalyzer_cxx
#define WprimeAnalyzer_cxx

#include "WprimeLoadData2016.C"
#include "WprimeStandardHistos.C"
#include "GetWprimeGenParticles.C"
#include <iomanip>
#include <set>
#include "Objects/interface/HcalLaserEventFilter2012.h"
#include "Objects/interface/LumiReWeighting.h"
#include "Objects/interface/TableReader.h"
#include "TF1.h"

using namespace std;

// Lepton ID and Trig
TableReader elRECOSF("elRECOSF_2016.txt");
TableReader elIDSF("elIDSF_2016.txt");
TableReader elTrigSF("elTrigSF_2016.txt");
TableReader muTrkSF("muTrkSF_2016.txt");
TableReader muIDSF("muIDSF_2016.txt");
TableReader muTrigSF("muTrigSF_2016.txt");

TableReader  BtagEff("BtagEff_2015D.txt");
TableReader  MistagEff("MistagEff_2015D.txt");

TF1 *BtagSF = new TF1("BtagSF","0.908299+(2.70877e-06*(TMath::Log(x+370.144)*(TMath::Log(x+370.144)*(3-(-(104.614*TMath::Log(x+370.144)))))))",0,9000);
TF1 *MistagSF = new TF1("MistagSF","((1.07278+(0.000535714*x))+(-1.14886e-06*(x*x)))+(7.0636e-10*(x*(x*x)))",0,9000);
TF1 *MistagSF_DOWN = new TF1("MistagSF_DOWN","((1.01637+(0.000265653*x))+(-4.22531e-07*(x*x)))+(2.23396e-10*(x*(x*x)))",0,9000);
TF1 *MistagSF_UP = new TF1("MistagSF_UP","((1.12921+(0.000804962*x))+(-1.87332e-06*(x*x)))+(1.18864e-09*(x*(x*x)))",0,9000);

float getLeptonSF (TLepton * lepton, Long64_t evt, Long64_t tot)
{
  if (lepton->isMuon()) {
    return (muTrkSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * muIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * muTrigSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
  }
  else return (elRECOSF.Val(lepton->lv.Pt(), lepton->lv.Eta()) * elIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * elTrigSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
}

float getBTagSF (TJet * jet, std::string btagsys)
{
  double err = 0.;
  if (jet->hadronFlavor==5) {
    if (jet->lv.Pt()<50.)       err = 0.044655;
    else if (jet->lv.Pt()<70.)  err = 0.030661;
    else if (jet->lv.Pt()<100.) err = 0.048988;
    else if (jet->lv.Pt()<140.) err = 0.041866;
    else if (jet->lv.Pt()<200.) err = 0.058439;
    else if (jet->lv.Pt()<300.) err = 0.079143;
    else if (jet->lv.Pt()<670.) err = 0.074660;
    else                        err = 0.145320;
  }
  if (jet->hadronFlavor==4) {
    if (jet->lv.Pt()<50.)       err = 0.022328;
    else if (jet->lv.Pt()<70.)  err = 0.015330;
    else if (jet->lv.Pt()<100.) err = 0.024494;
    else if (jet->lv.Pt()<140.) err = 0.020933;
    else if (jet->lv.Pt()<200.) err = 0.029220;
    else if (jet->lv.Pt()<300.) err = 0.039571;
    else if (jet->lv.Pt()<670.) err = 0.037330;
    else                        err = 0.074660;
  }
  if (btagsys == "NOM") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF->Eval(std::min(670.,jet->lv.Pt()));
    else return MistagSF->Eval(std::min(1000.,jet->lv.Pt()));
  }
  else if (btagsys == "UP") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF->Eval(std::min(670.,jet->lv.Pt()))+err;
    else return MistagSF_UP->Eval(std::min(1000.,jet->lv.Pt()));
  }
  else if (btagsys == "DOWN") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF->Eval(std::min(670.,jet->lv.Pt()))-err;
    else return MistagSF_DOWN->Eval(std::min(1000.,jet->lv.Pt()));
  }
  return 0;
}

float getBTagEff (TJet * jet)
{
  if (jet->hadronFlavor==5 or jet->hadronFlavor==4) {
    return BtagEff.Val(jet->lv.Pt(), jet->lv.Eta());
  }
  else {
    return MistagEff.Val(jet->lv.Pt(), jet->lv.Eta());
  }
}

bool startsWith(const std::string str, const std::string pre)
{
    size_t lenpre = strlen(pre.c_str()),
           lenstr = strlen(str.c_str());
    return lenstr < lenpre ? false : strncmp(pre.c_str(), str.c_str(), lenpre) == 0;
}

struct SortByDW {
  bool operator()(const TLorentzVector v1, const TLorentzVector v2) {
    double dw1 = fabs(v1.M() - MW);
    double dw2 = fabs(v2.M() - MW);
    return (dw1 < dw2);
  }
};


int main( int argc, const char* argv[] ){

  //double TJSF = 1.0;
  double BTAGCUTLOOSE = 0.5426; // loose ||| see https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation76X
  double BTAGCUTMED = 0.8484; // medium
  double BTAGCUTTIGHT = 0.9535; // tight

  std::string BTAGSYS = "NOM";
 
  if (argc < 5) {
    cout<<"Need 4 arguments:\n";
    cout<<"- lepton channel (El, Mu)\n";
    cout<<"- sample"<<endl;
    cout<<"- root file name"<<endl;
    cout<<"- output file name"<<endl;
    return 0;
  }
  //HcalLaserEventFilter2012 elcsc("./cscbadee2015_Dec01.txt.gz");
 
  TString inSuffix(argv[1]);
  TString sampleName(argv[2]);
  TString fileName(argv[3]);
  TString outName(argv[4]);
  
  cout<<"Processing "<<sampleName<<'\t'<<fileName<<endl;
  
  WprimeEvent * teve = new WprimeEvent();
  initAll_2015(fileName, sampleName);

  bool b_Signal = Sig[sampleIndex];
  bool b_RealData = (sampleIndex == Data);

  // Init LumiReWeighting
  
  std::string generatedFile("Moriond17MC_25ns_PUScenarioV1.root");
  std::string dataFile("DataPU_Jan04_2016.root");
  //std::string dataFile("DataPU_Jul21_2016BCD.root");
  std::string genHistName("histo");
  std::string dataHistName("pileup");
  fexists(generatedFile,true);
  fexists(dataFile,true);
  LumiReWeighting lumiReWeighting(generatedFile, dataFile, genHistName, dataHistName);

  //File for synchronization
//   char syncName[300];
  time_t rawtime;
  time ( &rawtime );

  //Output file
  char name[300];
  
  if (b_RealData) sprintf(name,"anOutFile_%s_%s_%lx.root", argv[2], argv[4], rawtime);
//  else            sprintf(name,"anOutFile_%s_%lx.root",    argv[2], rawtime);
   else            sprintf(name,"anOutFile_%s_%s.root",    argv[2], argv[4]);

  TFile* outFile = new TFile(name, "RECREATE");

  unsigned int nCutSteps = 10;

  TH1F* h_MuCutFlow;
  TH1F* h_ElCutFlow;
  TH2F* h_ElIso_vs_nPV;
  TH2F* h_MuIso_vs_nPV;

  TH2F* h_genTpT_vs_genTbarpT;
  TH2F* h_genTpT_vs_genTbarpT_Final;

  TH2F* h_MinLepAK4JetDR_vs_GenLepTopBjetDR_El;
  TH2F* h_MinLepAK4JetDR_vs_GenLepTopBjetDR_Mu;
  TH2F* h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_El;
  TH2F* h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_Mu;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_0b_El;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_0b_Mu;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_1b_El;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_1b_Mu;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_2b_El;
  TH2F* h_MinLepAK4JetDR_vs_Mtb_2b_Mu;

  TH2F* h_ElMETdPhi_vs_MET_PreSel;
  TH2F* h_LepdR_vs_pTRel_El_PreSel;
  TH2F* h_LepdR_vs_pTRel_Mu_PreSel;
  TH2F* h_ElMETdPhi_vs_MET_PostSel;
  TH2F* h_LepdR_vs_pTRel_El_PostSel;
  TH2F* h_LepdR_vs_pTRel_Mu_PostSel;

  TH2F* h_topPt_vs_Mtb_El;
  TH2F* h_topPt_vs_Mtb_Mu;

  TH2F* h_topM_vs_MinLepAK4JetDR_El;
  TH2F* h_topM_vs_MinLepAK4JetDR_Mu;
  TH2F* h_topM_vs_MET_El;
  TH2F* h_topM_vs_MET_Mu;
  TH2F* h_topM_vs_LepPt_El;
  TH2F* h_topM_vs_LepPt_Mu;
  TH2F* h_topM_vs_LepEta_El;
  TH2F* h_topM_vs_LepEta_Mu;
  TH2F* h_topM_vs_J1Pt_El;
  TH2F* h_topM_vs_J1Pt_Mu;
  TH2F* h_topM_vs_J2Pt_El;
  TH2F* h_topM_vs_J2Pt_Mu;
  TH2F* h_topM_vs_TopJetPt_El;
  TH2F* h_topM_vs_TopJetPt_Mu;
  TH2F* h_topM_vs_TopJetEta_El;
  TH2F* h_topM_vs_TopJetEta_Mu;
  TH2F* h_topM_vs_Mtb_El;
  TH2F* h_topM_vs_Mtb_Mu;
  TH2F* h_topM_vs_Njets_El;
  TH2F* h_topM_vs_Njets_Mu;
  TH1F* h_TopJetClosestToLep_El;
  TH1F* h_TopJetClosestToLep_Mu;

  TH2F* h_dR_vs_dpT_J1J2;
  TH2F* h_dR_vs_dpT_J1El;
  TH2F* h_dR_vs_dpT_J1Mu;
  TH2F* h_dR_vs_dpT_J2El;
  TH2F* h_dR_vs_dpT_J2Mu;

  TH2F* h_EffBDen;
  TH2F* h_EffBNum;
  TH2F* h_EffCDen;
  TH2F* h_EffCNum;
  TH2F* h_EffLDen;
  TH2F* h_EffLNum;

  TH2F* h_ElPt_vs_Eta;
  TH2F* h_MuPt_vs_Eta;

  vector <StandardHistos*> PreselPU_std_h(2);
  vector <StandardHistos*> Presel_std_h(2);

  vector <StandardHistos*> Ex0BTags_std_h(2);
  vector <StandardHistos*> Ex1BTags_std_h(2);
  vector <StandardHistos*> Ex2BTags_std_h(2);

  vector <StandardHistos*> GE0BTags_std_h(2);
  vector <StandardHistos*> GE1BTags_std_h(2);

  vector <StandardHistos*> Ex0BTags_Final_std_h(2);
  vector <StandardHistos*> Ex1BTags_Final_std_h(2);
  vector <StandardHistos*> Ex2BTags_Final_std_h(2);

  vector <StandardHistos*> Ex1BTags_Soft_std_h(2);
  vector <StandardHistos*> Ex2BTags_Soft_std_h(2);
  vector <StandardHistos*> Ex1BTags_Hard_std_h(2);
  vector <StandardHistos*> Ex2BTags_Hard_std_h(2);

  vector <StandardHistos*> GE0BTags_Post2D_std_h(2);
  vector <StandardHistos*> GE0BTags_Fail2D_std_h(2);
  vector <StandardHistos*> GE0BTags_PostMET_std_h(2);
  vector <StandardHistos*> GE0BTags_FailMET_std_h(2);
  vector <StandardHistos*> GE0BTags_FailMETPass2D_std_h(2);
  vector <StandardHistos*> GE0BTags_PostJet1_std_h(2);
  vector <StandardHistos*> Ex0BTags_PostLowJet1LowLep_std_h(2);
  vector <StandardHistos*> GE1BTags_PostLowJet1LowLep_std_h(2);
  vector <StandardHistos*> GE0BTags_PostLowJet1LowLep_std_h(2);
  vector <StandardHistos*> GE0BTags_PostJet2_std_h(2);

  vector <StandardHistos*> GE0BTags_PostMTop_std_h(2);
  vector <StandardHistos*> GE0BTags_PostPtTop_std_h(2);
  vector <StandardHistos*> GE0BTags_PostPtJ1J2_std_h(2);

  vector <StandardHistos*> GE0BTags_Final_std_h(2);
  vector <StandardHistos*> GE1BTags_Final_std_h(2);

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  h_MuCutFlow = new TH1F("h_MuCutFlow_"+sampleName, "h_MuCutFlow_"+sampleName, nCutSteps+1, -0.5, nCutSteps + 0.5);
  h_MuCutFlow->Sumw2();
  h_ElCutFlow = new TH1F("h_ElCutFlow_"+sampleName, "h_ElCutFlow_"+sampleName, nCutSteps+1, -0.5, nCutSteps + 0.5);
  h_ElCutFlow->Sumw2();

  h_genTpT_vs_genTbarpT = new TH2F("h_genTpT_vs_genTbarpT_"+sampleName, "h_genTpT_vs_genTbarpT_"+sampleName, 1000,  0.,  1000., 1000, 0.,  1000.);
  h_genTpT_vs_genTbarpT_Final = new TH2F("h_genTpT_vs_genTbarpT_Final_"+sampleName, "h_genTpT_vs_genTbarpT_Final_"+sampleName, 1000,  0.,  1000., 1000, 0.,  1000.);

  h_ElIso_vs_nPV = new TH2F("h_ElIso_vs_nPV_"+sampleName, "h_ElIso_vs_nPV_"+sampleName, 100,  0.,  0.15, 50, -0.5,  49.5);
  h_MuIso_vs_nPV = new TH2F("h_MuIso_vs_nPV_"+sampleName, "h_MuIso_vs_nPV_"+sampleName, 100,  0.,  0.20, 50, -0.5,  49.5);

  h_MinLepAK4JetDR_vs_GenLepTopBjetDR_El = new TH2F("h_MinLepAK4JetDR_vs_GenLepTopBjetDR_El_"+sampleName, "h_MinLepAK4JetDR_vs_GenLepTopBjetDR_El_"+sampleName, 500,  0.,  5., 500, 0.,  5.);
  h_MinLepAK4JetDR_vs_GenLepTopBjetDR_Mu = new TH2F("h_MinLepAK4JetDR_vs_GenLepTopBjetDR_Mu_"+sampleName, "h_MinLepAK4JetDR_vs_GenLepTopBjetDR_Mu_"+sampleName, 500,  0.,  5., 500, 0.,  5.);
  h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_El = new TH2F("h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_El_"+sampleName, "h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_El_"+sampleName, 500,  0.,  5., 500, 0.,  5.);
  h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_Mu = new TH2F("h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_Mu_"+sampleName, "h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_Mu_"+sampleName, 500,  0.,  5., 500, 0.,  5.);
  h_MinLepAK4JetDR_vs_Mtb_0b_El = new TH2F("h_MinLepAK4JetDR_vs_Mtb_0b_El_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_0b_El_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);
  h_MinLepAK4JetDR_vs_Mtb_0b_Mu = new TH2F("h_MinLepAK4JetDR_vs_Mtb_0b_Mu_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_0b_Mu_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);
  h_MinLepAK4JetDR_vs_Mtb_1b_El = new TH2F("h_MinLepAK4JetDR_vs_Mtb_1b_El_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_1b_El_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);
  h_MinLepAK4JetDR_vs_Mtb_1b_Mu = new TH2F("h_MinLepAK4JetDR_vs_Mtb_1b_Mu_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_1b_Mu_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);
  h_MinLepAK4JetDR_vs_Mtb_2b_El = new TH2F("h_MinLepAK4JetDR_vs_Mtb_2b_El_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_2b_El_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);
  h_MinLepAK4JetDR_vs_Mtb_2b_Mu = new TH2F("h_MinLepAK4JetDR_vs_Mtb_2b_Mu_"+sampleName, "h_MinLepAK4JetDR_vs_Mtb_2b_Mu_"+sampleName, 25,  0.,  5., 25, 0.,  5000.);

  
  h_ElMETdPhi_vs_MET_PreSel = new TH2F("h_ElMETdPhi_vs_MET_PreSel_"+sampleName, "h_ElMETdPhi_vs_MET_PreSel_"+sampleName, 32,  0.,  3.2, 50, 0.,  250.);
  h_LepdR_vs_pTRel_El_PreSel = new TH2F("h_LepdR_vs_pTRel_El_PreSel_"+sampleName, "h_LepdR_vs_pTRel_El_PreSel_"+sampleName, 200,  0.,  2., 1000, 0.,  300.);
  h_LepdR_vs_pTRel_Mu_PreSel = new TH2F("h_LepdR_vs_pTRel_Mu_PreSel_"+sampleName, "h_LepdR_vs_pTRel_Mu_PreSel_"+sampleName, 200,  0.,  2., 1000, 0.,  300.);
  h_ElMETdPhi_vs_MET_PostSel = new TH2F("h_ElMETdPhi_vs_MET_PostSel_"+sampleName, "h_ElMETdPhi_vs_MET_PostSel_"+sampleName, 32,  0.,  3.2, 50, 0.,  250.);
  h_LepdR_vs_pTRel_El_PostSel = new TH2F("h_LepdR_vs_pTRel_El_PostSel_"+sampleName, "h_LepdR_vs_pTRel_El_PostSel_"+sampleName, 25,  0.,  5., 25, 0.,  500.);
  h_LepdR_vs_pTRel_Mu_PostSel = new TH2F("h_LepdR_vs_pTRel_Mu_PostSel_"+sampleName, "h_LepdR_vs_pTRel_Mu_PostSel_"+sampleName, 25,  0.,  5., 25, 0.,  500.);

  h_topPt_vs_Mtb_El = new TH2F("h_topPt_vs_Mtb_El_"+sampleName, "h_topPt_vs_Mtb_El_"+sampleName, 50,  0.,  2000., 25, 0.,  5000.);
  h_topPt_vs_Mtb_Mu = new TH2F("h_topPt_vs_Mtb_Mu_"+sampleName, "h_topPt_vs_Mtb_Mu_"+sampleName, 50,  0.,  2000., 25, 0.,  5000.);

  h_topM_vs_MinLepAK4JetDR_El = new TH2F("h_topM_vs_MinLepAK4JetDR_El_"+sampleName, "h_topM_vs_MinLepAK4JetDR_El_"+sampleName, 20, 0., 1000., 25, 0., 5.);
  h_topM_vs_MinLepAK4JetDR_Mu = new TH2F("h_topM_vs_MinLepAK4JetDR_Mu_"+sampleName, "h_topM_vs_MinLepAK4JetDR_Mu_"+sampleName, 20, 0., 1000., 25, 0., 5.);
  h_topM_vs_MET_El = new TH2F("h_topM_vs_MET_El_"+sampleName, "h_topM_vs_MET_El_"+sampleName, 20, 0., 1000., 24, 0., 1200.);
  h_topM_vs_MET_Mu = new TH2F("h_topM_vs_MET_Mu_"+sampleName, "h_topM_vs_MET_Mu_"+sampleName, 20, 0., 1000., 24, 0., 1200.);
  h_topM_vs_LepPt_El = new TH2F("h_topM_vs_LepPt_El_"+sampleName, "h_topM_vs_LepPt_El_"+sampleName, 20, 0., 1000., 25, 0., 1200.);
  h_topM_vs_LepPt_Mu = new TH2F("h_topM_vs_LepPt_Mu_"+sampleName, "h_topM_vs_LepPt_Mu_"+sampleName, 20, 0., 1000., 25, 0., 1200.);
  h_topM_vs_LepEta_El = new TH2F("h_topM_vs_LepEta_El_"+sampleName, "h_topM_vs_LepEta_El_"+sampleName, 20, 0., 1000., 25, -2.5, 2.5);
  h_topM_vs_LepEta_Mu = new TH2F("h_topM_vs_LepEta_Mu_"+sampleName, "h_topM_vs_LepEta_Mu_"+sampleName, 20, 0., 1000., 25, -2.5, 2.5);
  h_topM_vs_J1Pt_El = new TH2F("h_topM_vs_J1Pt_El_"+sampleName, "h_topM_vs_J1Pt_El_"+sampleName, 20, 0., 1000., 25, 0., 2000.);
  h_topM_vs_J1Pt_Mu = new TH2F("h_topM_vs_J1Pt_Mu_"+sampleName, "h_topM_vs_J1Pt_Mu_"+sampleName, 20, 0., 1000., 25, 0., 2000.);
  h_topM_vs_J2Pt_El = new TH2F("h_topM_vs_J2Pt_El_"+sampleName, "h_topM_vs_J2Pt_El_"+sampleName, 20, 0., 1000., 25, 0., 1500.);
  h_topM_vs_J2Pt_Mu = new TH2F("h_topM_vs_J2Pt_Mu_"+sampleName, "h_topM_vs_J2Pt_Mu_"+sampleName, 20, 0., 1000., 25, 0., 1500.);
  h_topM_vs_TopJetPt_El = new TH2F("h_topM_vs_TopJetPt_El_"+sampleName, "h_topM_vs_TopJetPt_El_"+sampleName, 20, 0., 1000., 25, 0., 1500.);
  h_topM_vs_TopJetPt_Mu = new TH2F("h_topM_vs_TopJetPt_Mu_"+sampleName, "h_topM_vs_TopJetPt_Mu_"+sampleName, 20, 0., 1000., 25, 0., 1500.);
  h_topM_vs_TopJetEta_El = new TH2F("h_topM_vs_TopJetEta_El_"+sampleName, "h_topM_vs_TopJetEta_El_"+sampleName, 20, 0., 1000., 25, -2.5, 2.5);
  h_topM_vs_TopJetEta_Mu = new TH2F("h_topM_vs_TopJetEta_Mu_"+sampleName, "h_topM_vs_TopJetEta_Mu_"+sampleName, 20, 0., 1000., 25, -2.5, 2.5);
  h_topM_vs_Mtb_El = new TH2F("h_topM_vs_Mtb_El_"+sampleName, "h_topM_vs_Mtb_El_"+sampleName, 20, 0., 1000., 25, 0., 5000.);
  h_topM_vs_Mtb_Mu = new TH2F("h_topM_vs_Mtb_Mu_"+sampleName, "h_topM_vs_Mtb_Mu_"+sampleName, 20, 0., 1000., 25, 0., 5000.);
  h_topM_vs_Njets_El = new TH2F("h_topM_vs_Njets_El_"+sampleName, "h_topM_vs_Njets_El_"+sampleName, 20, 0., 1000., 19, 1.5, 20.5);
  h_topM_vs_Njets_Mu = new TH2F("h_topM_vs_Njets_Mu_"+sampleName, "h_topM_vs_Njets_Mu_"+sampleName, 20, 0., 1000., 19, 1.5, 20.5);
  h_TopJetClosestToLep_El = new TH1F("h_TopJetClosestToLep_El_"+sampleName, "h_TopJetClosestToLep_El_"+sampleName, 2, -0.5, 1.5);
  h_TopJetClosestToLep_Mu = new TH1F("h_TopJetClosestToLep_Mu_"+sampleName, "h_TopJetClosestToLep_Mu_"+sampleName, 2, -0.5, 1.5);

  h_dR_vs_dpT_J1J2 = new TH2F("h_dR_vs_dpT_J1J2_"+sampleName, "h_dR_vs_dpT_J1J2_"+sampleName, 600,  0.,  6., 2000, 0.,  2000.);
  h_dR_vs_dpT_J1El = new TH2F("h_dR_vs_dpT_J1El_"+sampleName, "h_dR_vs_dpT_J1El_"+sampleName, 600,  0.,  6., 2000, 0.,  2000.);
  h_dR_vs_dpT_J1Mu = new TH2F("h_dR_vs_dpT_J1Mu_"+sampleName, "h_dR_vs_dpT_J1Mu_"+sampleName, 600,  0.,  6., 2000, 0.,  2000.);
  h_dR_vs_dpT_J2El = new TH2F("h_dR_vs_dpT_J2El_"+sampleName, "h_dR_vs_dpT_J2El_"+sampleName, 600,  0.,  6., 2000, 0.,  2000.);
  h_dR_vs_dpT_J2Mu = new TH2F("h_dR_vs_dpT_J2Mu_"+sampleName, "h_dR_vs_dpT_J2Mu_"+sampleName, 600,  0.,  6., 2000, 0.,  2000.);

  h_EffBDen = new TH2F("h_EffBDen_"+sampleName, "h_EffBDen_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);
  h_EffBNum = new TH2F("h_EffBNum_"+sampleName, "h_EffBNum_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);
  h_EffCDen = new TH2F("h_EffCDen_"+sampleName, "h_EffCDen_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);
  h_EffCNum = new TH2F("h_EffCNum_"+sampleName, "h_EffCNum_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);
  h_EffLDen = new TH2F("h_EffLDen_"+sampleName, "h_EffLDen_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);
  h_EffLNum = new TH2F("h_EffLNum_"+sampleName, "h_EffLNum_"+sampleName, 2000,  0.,  2000., 600, 0.,  3.);

  h_ElPt_vs_Eta = new TH2F("h_ElPt_vs_Eta_"+sampleName, "h_ElPt_vs_Eta_"+sampleName, 100,  0.,  1000., 50, -2.4,  2.4);
  h_MuPt_vs_Eta = new TH2F("h_MuPt_vs_Eta_"+sampleName, "h_MuPt_vs_Eta_"+sampleName, 100,  0.,  1000., 50, -2.4,  2.4);

  //Make directories and initialize histograms
  initHistoDirTwoChannels(outFile, PreselPU_std_h,           "PreselPU",        sampleName, sScale);
  initHistoDirTwoChannels(outFile, Presel_std_h,             "Presel",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, Ex0BTags_std_h,           "Ex0BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex1BTags_std_h,           "Ex1BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_std_h,           "Ex2BTags",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, GE0BTags_std_h,           "GE0BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_std_h,           "GE1BTags",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, Ex0BTags_Final_std_h,           "Ex0BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex1BTags_Final_std_h,           "Ex1BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_Final_std_h,           "Ex2BTags_Final",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, Ex1BTags_Soft_std_h,           "Ex1BTags_Soft",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_Soft_std_h,           "Ex2BTags_Soft",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex1BTags_Hard_std_h,           "Ex1BTags_Hard",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_Hard_std_h,           "Ex2BTags_Hard",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, GE0BTags_Post2D_std_h,          "GE0BTags_Post2D",         sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_Fail2D_std_h,          "GE0BTags_Fail2D",         sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostMET_std_h,         "GE0BTags_PostMET",        sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_FailMET_std_h,         "GE0BTags_FailMET",        sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_FailMETPass2D_std_h,   "GE0BTags_FailMETPass2D",  sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostJet1_std_h,        "GE0BTags_PostJet1",       sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostLowJet1LowLep_std_h,     "GE0BTags_PostLowJet1LowLep",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_PostLowJet1LowLep_std_h,     "Ex0BTags_PostLowJet1LowLep",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_PostLowJet1LowLep_std_h,     "GE1BTags_PostLowJet1LowLep",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostJet2_std_h,        "GE0BTags_PostJet2",       sampleName, sScale);

  initHistoDirTwoChannels(outFile, GE0BTags_PostMTop_std_h,        "GE0BTags_PostMTop",       sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostPtTop_std_h,       "GE0BTags_PostPtTop",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_PostPtJ1J2_std_h,      "GE0BTags_PostPtJ1J2",     sampleName, sScale);

  initHistoDirTwoChannels(outFile, GE0BTags_Final_std_h,           "GE0BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_Final_std_h,           "GE1BTags_Final",          sampleName, sScale);

  outFile->cd();

  cout << "Looping over "<<sampleName<<" "<< treeClass->getEntries()<<endl;

  set <long long> checkDuplicates;
  int nDup = 0;
  long long RUNPREF = 1000 * 1000; 
  RUNPREF *= 1000 * 1000;

  for (Long64_t il = 0; il < treeClass->getEntries(); il++){
  //for (Long64_t il = 0; il < 5; il++){
    
    if (il % 10000 == 0) cout<<"At event "<<il<<endl;

    treeClass->GetEntry(il);
    //Initialize WprimeEvent class
    teve->clearEvent();
    teve->isam     = sampleIndex;
    teve->realData = b_RealData;
    teve->nPV      = treeClass->nPV;

    //if (treeClass->run!=1 || treeClass->lumi!=1429 || treeClass->event!=285653) continue;
    //if (treeClass->run>276811) continue;

    //if (teve->realData) {
    //  elcsc.filter(treeClass->run,treeClass->lumi,treeClass->event);
    //}
    
    //if (teve->realData and treeClass->run<=275783) continue;
    
    // Review PU:
    teve->weight = 1.0;
    int nti = (int) treeClass->nTrueInteractions;
    if (teve->realData) teve->PUweight = 1.0;
    else                teve->PUweight = lumiReWeighting.weight(nti);
    //else                teve->PUweight = 1.0;
    //else                teve->weight = weightpu[nti];
    if (treeClass->weight_MC<0) teve->weight=(-1.0)*(teve->weight);
    if (b_Signal) teve->lim_weight = teve->weight*(1/xSection);
    else teve->lim_weight = teve->weight;

    /*if (il==0) {
      std::cout << "LHE weights:"<<std::endl;
      for (unsigned int it = 0; it<treeClass->LHEweightids->size(); it++)
        std::cout << "\tID = " << treeClass->LHEweightids->at(it) << " , weight = " << treeClass->LHEweights->at(it) << std::endl;
    }*/

    vector <WprimeGenParticle> genBSL;
    WprimeGenParticle genLtmp, genTDL;
    double gentpt = -1;
    double gentbarpt = -1;
    if (not teve->realData) {
      vector <TLorentzVector> genLV;
      vector <TLorentzVector> genJetLV;
      TLorentzVector vtemp;
      TLorentzVector dummy;
      dummy.SetPxPyPzE(0.1,0.,999998.,999999.);
      TLorentzVector genq1  = dummy;
      TLorentzVector genq2  = dummy;
      TLorentzVector gennu  = dummy;
      TLorentzVector genlep = dummy;
      for (unsigned int ui = 0; ui < treeClass->genID->size(); ui++){
        if (abs(treeClass->genID->at(ui))==6 && sampleName=="TTbar" && treeClass->genStatus->at(ui)==62) {
        //  std::cout<<"Top pT = "<<treeClass->genPt->at(ui)<<" , weight = "<<TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))<<std::endl;
          //teve->weight *= TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))/0.99638;
          //teve->lim_weight *= TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))/0.99638;
          teve->weight *= TMath::Sqrt(TMath::Exp(0.0615-0.0005*treeClass->genPt->at(ui)));
          teve->lim_weight *= TMath::Sqrt(TMath::Exp(0.0615-0.0005*treeClass->genPt->at(ui)));
          if (treeClass->genID->at(ui)>0) gentpt = treeClass->genPt->at(ui);
          else gentbarpt = treeClass->genPt->at(ui);
        }//TESTING
        vtemp.SetPtEtaPhiE(treeClass->genPt->at(ui), treeClass->genEta->at(ui),
		treeClass->genPhi->at(ui), treeClass->genEnergy->at(ui));
        genLV.push_back(vtemp);
      }

      vector <WprimeGenParticle> genParticles = GetWprimeGenParticles(genLV, (*treeClass->genID));

      for (unsigned int uj = 0; uj < genParticles.size(); uj++){
        if (abs(genParticles.at(uj).ID) == 34) teve->lvGenWprime = genParticles.at(uj).lv;
        else if (abs(genParticles.at(uj).ID) == 6 && genParticles.at(uj).lv.Pt()>1.) teve->lvGenTop = genParticles.at(uj).lv;
        else if (abs(genParticles.at(uj).ID) == 24 && genParticles.at(uj).lv.Pt()>1.) teve->lvGenW = genParticles.at(uj).lv;
        else if ((abs(genParticles.at(uj).ID) == 11 || abs(genParticles.at(uj).ID) == 13) && genParticles.at(uj).lv.Pt()>1.) {
          teve->lvGenLep = genParticles.at(uj).lv;
        }
        //else if ((abs(genParticles.at(uj).ID) == 15) && genParticles.at(uj).lv.Pt()>1.) gentau = true;
        if ((abs(genParticles.at(uj).ID) == 11 || abs(genParticles.at(uj).ID) == 13 || abs(genParticles.at(uj).ID) == 15) && genParticles.at(uj).lv.Pt()>genlep.Pt()) genlep = genParticles.at(uj).lv;
        if ((abs(genParticles.at(uj).ID) == 12 || abs(genParticles.at(uj).ID) == 14 || abs(genParticles.at(uj).ID) == 16) && genParticles.at(uj).lv.Pt()>gennu.Pt()) gennu = genParticles.at(uj).lv;
        if (abs(genParticles.at(uj).ID)<=5) {
          if (genParticles.at(uj).lv.Pt()>genq1.Pt()) {
            genq2 = genq1;
            genq1 = genParticles.at(uj).lv;
          }
          else if (genParticles.at(uj).lv.Pt()>genq2.Pt()) genq2 = genParticles.at(uj).lv;
        }
      }
      if (genlep.Pt()>1. && gennu.Pt()>1. && genq1.Pt()>1. && genq2.Pt()>1.) teve->lvGenWprimeReco = genlep+gennu+genq1+genq2;
      for (unsigned int uk = 0; uk < treeClass->genJetPt->size(); uk++){
        if (treeClass->genJetPt->at(uk) > 30. && fabs(treeClass->genJetEta->at(uk)) < 2.4) {
          vtemp.SetPtEtaPhiE(treeClass->genJetPt->at(uk), treeClass->genJetEta->at(uk),
		treeClass->genJetPhi->at(uk), treeClass->genJetEnergy->at(uk));
	  genJetLV.push_back(vtemp);
        }
      }
      teve->vGenJets = genJetLV;
      bool matched = false;
      for (unsigned int uk = 0; uk < treeClass->elMatchedPt->size(); uk++){
        if (treeClass->elMatchedPt->at(uk)>30.) {
          vtemp.SetPtEtaPhiE(treeClass->elMatchedPt->at(uk), treeClass->elMatchedEta->at(uk),
		treeClass->elMatchedPhi->at(uk), treeClass->elMatchedEnergy->at(uk));
	  teve->lvGenLepMatch = vtemp;
          matched = true;
        }
      }
      for (unsigned int uk = 0; uk < treeClass->muMatchedPt->size(); uk++){
        if (treeClass->muMatchedPt->at(uk)>30.) {
          vtemp.SetPtEtaPhiE(treeClass->muMatchedPt->at(uk), treeClass->muMatchedEta->at(uk),
		treeClass->muMatchedPhi->at(uk), treeClass->muMatchedEnergy->at(uk));
	  teve->lvGenLepMatch = vtemp;
          matched = true;
        }
      }
      if(!matched) teve->lvGenLepMatch = dummy;
      for (unsigned int ul = 0; ul < treeClass->genBSLID->size(); ul++) {
        if (treeClass->genBSLID->at(ul)!=0) {
          genLtmp.ID = treeClass->genBSLID->at(ul);
          vtemp.SetPtEtaPhiE(treeClass->genBSLPt->at(ul), treeClass->genBSLEta->at(ul),
                treeClass->genBSLPhi->at(ul), treeClass->genBSLEnergy->at(ul));
          genLtmp.lv = vtemp;
          genBSL.push_back(genLtmp);
        }
      }
      genTDL.ID = treeClass->genTDLID;
      if (genTDL.ID!=0) {
        vtemp.SetPtEtaPhiE(treeClass->genTDLPt, treeClass->genTDLEta,
              treeClass->genTDLPhi, treeClass->genTDLEnergy);
        genTDL.lv = vtemp;
      }
      else genTDL.lv = dummy;
    }//end of not data

    //check that there is only one lepton in the event
    if (treeClass->goodLeptons.size() == 1){
      teve->lepton1 = treeClass->goodLeptons.at(0);
    }//End of exactly one lepton check
    else continue;

    //Determine channel
    if (    teve->lepton1->isMuon())       teve->suffix = "Mu";
    else if (teve->lepton1->isElectron())  teve->suffix = "El";
    else{
      cout<<"Invalid channel!"<<endl;
      return 1;
    }

    //IP cuts
    //if (teve->suffix == "Mu" and (teve->lepton1->ipXY > 0.2 or teve->lepton1->ipZ > 0.5 or (teve->lepton1->lv.Pt()>500. and fabs(teve->lepton1->lv.Eta())>1.2))) {
    if (teve->suffix == "Mu" and (teve->lepton1->ipXY > 0.2 or teve->lepton1->ipZ > 0.5)) {
      continue;
    }

    if (not teve->realData) {
      teve->weight *= getLeptonSF(teve->lepton1, il, treeClass->getEntries());
      teve->lim_weight *= getLeptonSF(teve->lepton1, il, treeClass->getEntries());
    }
    //if (teve->lepton1->lv.Pt()>180 && teve->suffix == "Mu") std::cout<<"Muon SF: "<<getLeptonSF(teve->lepton1, il, treeClass->getEntries());
    
    if (teve->suffix == "Mu") h_MuCutFlow->Fill(0.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(0.0, teve->weight);
    
    //Select only events which pass the trigger for the channel we are looking at
    bool trigMuMC = false;
    bool trigElMC = false;
    bool trigMu = false;
    bool trigEl = false;
    bool trigMuMCMatch = false;
    bool trigElMCMatch = false;
    bool trigMuMatch = false;
    bool trigElMatch = false;
    /*if (not teve->realData) {
      trigMu = true;
      trigEl = true;
      trigMuMatch = true;
      trigElMatch = true;
      if (teve->suffix == "Mu") {
        trigElMC = true;
        trigElMCMatch = true;
        if (treeClass->viSelMCTriggersMu->at(1) == 1) trigMuMC = true;
        if (treeClass->muon_1_hltmatched->at(1) == 1) trigMuMCMatch = true;
      }
      if (teve->suffix == "El") {
        trigMuMC = true;
        trigMuMCMatch = true;
        if (treeClass->viSelMCTriggersEl->at(0) == 1) trigElMC = true;
        if (treeClass->electron_1_hltmatched->at(0) == 1) trigElMCMatch = true;
      }
    }
    else {
      trigMuMC = true;
      trigElMC = true;
      trigMuMCMatch = true;
      trigElMCMatch = true;
      if (teve->suffix == "Mu") {
        trigEl = true;
        trigElMatch = true;
        if (treeClass->viSelTriggersMu->at(1) == 1) trigMu = true;
        if (treeClass->muon_1_hltmatched->at(1) == 1) trigMuMatch = true;
      }
      if (teve->suffix == "El") {
        trigMu = true;
        trigMuMatch = true;
        if (treeClass->viSelTriggersEl->at(0) == 1) trigEl = true;
        if (treeClass->electron_1_hltmatched->at(0) == 1) trigElMatch = true;
      }
    }*/
    if (not teve->realData) {
      trigMu = true;
      trigEl = true;
      trigMuMatch = true;
      trigElMatch = true;
      if (teve->suffix == "Mu") {
        trigElMC = true;
        trigElMCMatch = true;
        for (unsigned int im = 0; im < treeClass->vsSelMCTriggersMu->size(); im++) {
          if (startsWith(treeClass->vsSelMCTriggersMu->at(im),"HLT_Mu50_v") or startsWith(treeClass->vsSelMCTriggersMu->at(im),"HLT_TkMu50_v")) {
          //if (treeClass->vsSelMCTriggersMu->at(im) == "HLT_IsoMu20_eta2p1_v1") {
            if (treeClass->viSelMCTriggersMu->at(im) == 1) trigMuMC = true;
          }
        }
        for (unsigned int im = 0; im < treeClass->muon_hltfilters->size(); im++) {
          if (treeClass->muon_hltfilters->at(im) == "hltL3fL1sMu16orMu25L1f0L2f16QL3Filtered45e2p1Q") {
            if (treeClass->muon_1_hltmatched->at(im) == 1) trigMuMCMatch = true;
          }
        }
      }
      if (teve->suffix == "El") {
        trigMuMC = true;
        trigMuMCMatch = true;
        for (unsigned int im = 0; im < treeClass->vsSelMCTriggersEl->size(); im++) {
          if (startsWith(treeClass->vsSelMCTriggersEl->at(im),"HLT_Ele115_CaloIdVT_GsfTrkIdT_v")) {
          //if (treeClass->vsSelMCTriggersEl->at(im) == "HLT_Ele27_eta2p1_WP75_Gsf_v1") {
            if (treeClass->viSelMCTriggersEl->at(im) == 1) trigElMC = true;
          }
        }
        for (unsigned int im = 0; im < treeClass->electron_hltfilters->size(); im++) {
          if (treeClass->electron_hltfilters->at(im) == "hltEle105CaloIdVTGsfTrkIdTGsfDphiFilter") {
            if (treeClass->electron_1_hltmatched->at(im) == 1) trigElMCMatch = true;
          }
        }
      }
    }
    else {
      trigMuMC = true;
      trigElMC = true;
      trigMuMCMatch = true;
      trigElMCMatch = true;
      if (teve->suffix == "Mu") {
        trigEl = true;
        trigElMatch = true;
        for (unsigned int im = 0; im < treeClass->vsSelTriggersMu->size(); im++) {
          if (startsWith(treeClass->vsSelTriggersMu->at(im),"HLT_Mu50_v") or startsWith(treeClass->vsSelTriggersMu->at(im),"HLT_TkMu50_v")) {
          //if (treeClass->vsSelTriggersMu->at(im) == "HLT_IsoMu20_eta2p1_v2") {
            if (treeClass->viSelTriggersMu->at(im) == 1) trigMu = true;
          }
        }
        for (unsigned int im = 0; im < treeClass->muon_hltfilters->size(); im++) {
          if (treeClass->muon_hltfilters->at(im) == "hltL3fL1sMu16orMu25L1f0L2f16QL3Filtered45e2p1Q") {
            if (treeClass->muon_1_hltmatched->at(im) == 1) trigMuMatch = true;
          }
        }
      }
      if (teve->suffix == "El") {
        trigMu = true;
        trigMuMatch = true;
        for (unsigned int im = 0; im < treeClass->vsSelTriggersEl->size(); im++) {
          if (startsWith(treeClass->vsSelTriggersEl->at(im),"HLT_Ele115_CaloIdVT_GsfTrkIdT_v")) {
          //if (treeClass->vsSelTriggersEl->at(im) == "HLT_Ele27_eta2p1_WPLoose_Gsf_v1") {
            if (treeClass->viSelTriggersEl->at(im) == 1) trigEl = true;
          }
        }
        for (unsigned int im = 0; im < treeClass->electron_hltfilters->size(); im++) {
          if (treeClass->electron_hltfilters->at(im) == "hltEle105CaloIdVTGsfTrkIdTGsfDphiFilter") {
            if (treeClass->electron_1_hltmatched->at(im) == 1) trigElMatch = true;
          }
        }
      }
    }

    if (!(trigElMC && trigEl && trigMuMC && trigMu)) {
      continue;
    }
    if (!(trigElMCMatch && trigElMatch && trigMuMCMatch && trigMuMatch)) {
      //continue;
    }

    //Check for duplicates
    //The CPU cost of this grows as nLog(n) with the number of 
    //events so do this as late as possible
    if (teve->realData){

//------------------- CHECKING FOR DUPLICATES (?) -------------------

      long long dupCheck = treeClass->run * RUNPREF + treeClass->event;
      if (checkDuplicates.find(dupCheck) != checkDuplicates.end()){
	nDup++;
	continue;
      }
      else checkDuplicates.insert(dupCheck);

    }
    //End of check for duplicates

    //Jet Cuts
    //teve->vGoodAK8Jets     = treeClass->goodAK8Jets();
    teve->vGoodAK4Jets     = treeClass->goodAK4Jets();
    //teve->vOnlyGoodAK4Jets = treeClass->goodJets();

    bool lep2Dcut = true;
    bool lepjetoverlap = false;

    teve->met = treeClass->corr_met;
    teve->met_x = treeClass->corr_met_px;
    teve->met_y = treeClass->corr_met_py;
    teve->met_phi = treeClass->corr_met_phi;

    if (teve->lepton1->isMuon() && fabs(teve->lepton1->eta)>1.2) {
      double metcx = (teve->lepton1->pt*TMath::Cos(teve->lepton1->phi))-teve->lepton1->lv.Px();
      double metcy = (teve->lepton1->pt*TMath::Sin(teve->lepton1->phi))-teve->lepton1->lv.Py();
      teve->met_x += metcx;
      teve->met_y += metcy;
      teve->met = TMath::Sqrt((teve->met_x*teve->met_x)+(teve->met_y*teve->met_y));
      teve->met_phi = TMath::ACos(teve->met_x/teve->met);
    }

    //if (teve->realData and teve->met>1000) {
    //    cout<<"***** Large MET event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<" *****"<<endl;
    //}

    double min2DjetPt;
    if (teve->suffix == "El") min2DjetPt = 25.;
    if (teve->suffix == "Mu") min2DjetPt = 25.;
    double minRecojetPt = 25.;
    double mindR = 999.;
    double ptrel = 999.;
    //for (unsigned int ij = 0; ij < teve->vGoodAK8Jets.size(); ij++){ teve->ht += teve->vGoodAK8Jets.at(ij)->lv.Pt(); }
    //std::vector<unsigned int> ljmatch;
    for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++){
      if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < min2DjetPt) continue;
      if ((teve->vGoodAK4Jets.at(ij)->lv.DeltaR(teve->lepton1->lv) < mindR)) {
        mindR = teve->vGoodAK4Jets.at(ij)->lv.DeltaR(teve->lepton1->lv);
        ptrel = teve->lepton1->lv.P()*(teve->vGoodAK4Jets.at(ij)->lv.Vect().Cross(teve->lepton1->lv.Vect()).Mag()/teve->vGoodAK4Jets.at(ij)->lv.P()/teve->lepton1->lv.P());
      }
      teve->ht        += teve->vGoodAK4Jets.at(ij)->lv.Pt();
      //if (teve->vGoodAK4Jets.at(ij)->csvDisc > BTAGCUTLOOSE) teve->nBTags++;
      if (teve->vGoodAK4Jets.at(ij)->csvTag[0] > 0) teve->nBTags++;
      if (teve->vGoodAK4Jets.at(ij)->hadronFlavor==5) teve->nBjs++;
      else if (teve->vGoodAK4Jets.at(ij)->hadronFlavor==4) teve->nCjs++;
      else teve->nLjs++;
      //if (teve->vGoodAK4Jets.at(ij)->lv.DeltaR(teve->lepton1->lv) < 0.4) ljmatch.push_back(ij);
    }
    //for (int lji = ljmatch.size()-1; lji >=0; lji--) {(teve->vGoodAK4Jets).erase((teve->vGoodAK4Jets).begin()+ljmatch[lji]);}
    if (sampleName.BeginsWith("WJets")) {
      if (teve->nBjs>0) {
        if (sampleName.EndsWith("LF")) continue;
        teve->weight *= WJHFSF;
        teve->lim_weight *= WJHFSF;
      }
      else if (teve->nBjs==0 && teve->nCjs>0) {
        if (sampleName.EndsWith("LF")) continue;
        teve->weight *= WJHFSF;
        teve->lim_weight *= WJHFSF;
      }
      else if (teve->nBjs==0 && teve->nCjs==0 && teve->nLjs>0) {
        if (sampleName.EndsWith("HF")) continue;
        teve->weight *= WJLFSF;
        teve->lim_weight *= WJLFSF;
      }
      /*for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++){
        if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < 250.) continue;
        double jetpt = std::min(teve->vGoodAK4Jets.at(ij)->lv.Pt(),2000.);
        teve->weight *= (1.094 - 0.0006798*jetpt + 0.0000001470*jetpt*jetpt);
        teve->lim_weight *= (1.094 - 0.0006798*jetpt + 0.0000001470*jetpt*jetpt);
      }*/
    }
    //std::cout<<"-----------"<<std::endl;
    double PMC = 1.;
    double PDATA = 1.;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE) {
      PMC *= getBTagEff(teve->vGoodAK4Jets.at(0));
      PDATA *= getBTagEff(teve->vGoodAK4Jets.at(0))*getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS);
      //std::cout<<"**TAGGED**"<<std::endl;
      //std::cout<<"EFF= "<<getBTagEff(teve->vGoodAK4Jets.at(0))<<"\tSF= "<<getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS)<<std::endl;
      //std::cout<<"MC= "<<PMC<<"\tDATA= "<<PDATA<<std::endl;
    }
    else {
      PMC *= (1-getBTagEff(teve->vGoodAK4Jets.at(0)));
      PDATA *= (1-getBTagEff(teve->vGoodAK4Jets.at(0))*getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS));
      //std::cout<<"**UNTAGGED**"<<std::endl;
      //std::cout<<"EFF= "<<getBTagEff(teve->vGoodAK4Jets.at(0))<<"\tSF= "<<getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS)<<std::endl;
      //std::cout<<"MC= "<<PMC<<"\tDATA= "<<PDATA<<std::endl;
    }
    if (teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) {
      PMC *= getBTagEff(teve->vGoodAK4Jets.at(1));
      PDATA *= getBTagEff(teve->vGoodAK4Jets.at(1))*getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS);
      //std::cout<<"**TAGGED**"<<std::endl;
      //std::cout<<"EFF= "<<getBTagEff(teve->vGoodAK4Jets.at(1))<<"\tSF= "<<getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS)<<std::endl;
      //std::cout<<"MC= "<<PMC<<"\tDATA= "<<PDATA<<std::endl;
    }
    else {
      PMC *= (1-getBTagEff(teve->vGoodAK4Jets.at(1)));
      PDATA *= (1-getBTagEff(teve->vGoodAK4Jets.at(1))*getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS));
      //std::cout<<"**UNTAGGED**"<<std::endl;
      //std::cout<<"EFF= "<<getBTagEff(teve->vGoodAK4Jets.at(1))<<"\tSF= "<<getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS)<<std::endl;
      //std::cout<<"MC= "<<PMC<<"\tDATA= "<<PDATA<<std::endl;
    }
    //if (PMC>0 && not teve->realData) {teve->weight *= (PDATA/PMC);}
    /*if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) std::cout<<"  Tag  Tag     ";
    else if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE) std::cout<<"  TagNoTag     ";
    else if (teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) std::cout<<"NoTag  Tag     ";
    else std::cout<<"NoTagNoTag     ";*/
    /*double bsftmp1 = 1;
    double bsftmp2 = 1;
    if (getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS)<=1.) bsftmp1 *= getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS);
    else bsftmp1 *= 1+(1-getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS))/(1-(1/getBTagEff(teve->vGoodAK4Jets.at(0))));
    if (getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS)<=1.) bsftmp1 *= getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS);
    else bsftmp2 *= 1+(1-getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS))/(1-(1/getBTagEff(teve->vGoodAK4Jets.at(1))));
    double bsftmp = bsftmp1*bsftmp2;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) {
      std::cout<<"        hex2a->Fill("<<PDATA/PMC<<");"<<std::endl;
      std::cout<<"        hex2b->Fill("<<bsftmp1<<");"<<std::endl;
      std::cout<<"        hex2b->Fill("<<bsftmp2<<");"<<std::endl;
    }
    else if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) {
      std::cout<<"        hex1a->Fill("<<PDATA/PMC<<");"<<std::endl;
      std::cout<<"        hex1b->Fill("<<bsftmp1<<");"<<std::endl;
      std::cout<<"        hex1b->Fill("<<bsftmp2<<");"<<std::endl;
    }
    else {
      std::cout<<"        hex0a->Fill("<<PDATA/PMC<<");"<<std::endl;
      std::cout<<"        hex0b->Fill("<<bsftmp1<<");"<<std::endl;
      std::cout<<"        hex0b->Fill("<<bsftmp2<<");"<<std::endl;
    }*/
    /*if (((float)((int)(bsftmp*1000))/1000.) != ((float)((int)((PDATA/PMC)*1000))/1000.)) {
      std::cout<<"vvvvv----";
      if (teve->vGoodAK4Jets.at(0)->hadronFlavor==5) std::cout<<"B";
      else if (teve->vGoodAK4Jets.at(0)->hadronFlavor==4) std::cout<<"C";
      else std::cout<<"L";
      if (teve->vGoodAK4Jets.at(1)->hadronFlavor==5) std::cout<<"B";
      else if (teve->vGoodAK4Jets.at(1)->hadronFlavor==4) std::cout<<"C";
      else std::cout<<"L";
      std::cout<<"----vvvvv"<<std::endl;
      std::cout<<"Method 1: "<<(PDATA/PMC)<<std::endl;
      std::cout<<"Method 2: "<<bsftmp<<std::endl;
      std::cout<<"J1 SF = "<<getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS)<<" , Eff = "<<getBTagEff(teve->vGoodAK4Jets.at(0))<<std::endl;
      std::cout<<"J2 SF = "<<getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS)<<" , Eff = "<<getBTagEff(teve->vGoodAK4Jets.at(1))<<std::endl;
    }
    else std::cout<<std::endl;*/
    //if (mindR <= 0.4) lepjetoverlap = true;
    if (mindR <= 0.4 && ptrel < 50 && teve->suffix == "Mu") lep2Dcut = false;
    if (mindR <= 0.4 && ptrel < 60 && teve->suffix == "El") lep2Dcut = false;
    /*bool firstbtagloose = false;
    bool first2btagloose = false;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) firstbtagloose = true;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) first2btagloose = true;
    bool firstbtagmed = false;
    bool first2btagmed = false;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTMED || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTMED) firstbtagmed = true;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTMED && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTMED) first2btagmed = true;
    bool firstbtagtight = false;
    bool first2btagtight = false;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTTIGHT || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTTIGHT) firstbtagtight = true;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTTIGHT && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTTIGHT) first2btagtight = true;*/

    /*if (not teve->realData) {
      if (teve->vGoodAK4Jets.at(0)->lv.Pt() > 1200.) {
        teve->weight *= 0.60852;
        teve->lim_weight *= 0.60852;
      }
      else {
        teve->weight *= (1.251-(0.0005354*teve->vGoodAK4Jets.at(0)->lv.Pt()));
        teve->lim_weight *= (1.251-(0.0005354*teve->vGoodAK4Jets.at(0)->lv.Pt()));
      }
    }//TESTING*/
    for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++) {
      if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < min2DjetPt) continue;
      if (teve->vGoodAK4Jets.at(ij)->hadronFlavor==5) {
        h_EffBDen->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
        if (teve->vGoodAK4Jets.at(ij)->csvDisc > BTAGCUTLOOSE) h_EffBNum->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
      }
      if (teve->vGoodAK4Jets.at(ij)->hadronFlavor==4) {
        h_EffCDen->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
        if (teve->vGoodAK4Jets.at(ij)->csvDisc > BTAGCUTLOOSE) h_EffCNum->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
      }
      else {
        h_EffLDen->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
        if (teve->vGoodAK4Jets.at(ij)->csvDisc > BTAGCUTLOOSE) h_EffLNum->Fill(teve->vGoodAK4Jets.at(ij)->lv.Pt(),fabs(teve->vGoodAK4Jets.at(ij)->lv.Eta()));
      }
    }

    //bool firstbtag = firstbtagloose;
    //bool first2btag = first2btagloose;
    bool firstbtag = false;
    bool first2btag = false;

    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) firstbtag = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 && teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) first2btag = true;
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) firstbtag = true;
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) first2btag = true;

    fillHistosTwoChannels(PreselPU_std_h, teve);
    teve->weight *= teve->PUweight;
    teve->lim_weight *= teve->PUweight;
    fillHistosTwoChannels(Presel_std_h, teve);

    double elmetdphi = 999.;
    if (teve->suffix == "El") {
      elmetdphi = teve->lepton1->lv.Phi()-teve->met_phi;
      if (elmetdphi>TMath::Pi()) elmetdphi -= 2*TMath::Pi();
      if (elmetdphi<=-TMath::Pi()) elmetdphi += 2*TMath::Pi();
      h_ElMETdPhi_vs_MET_PreSel->Fill(fabs(elmetdphi),teve->met);
    }
    if (teve->suffix == "El") h_LepdR_vs_pTRel_El_PreSel->Fill(mindR,ptrel);
    if (teve->suffix == "Mu") h_LepdR_vs_pTRel_Mu_PreSel->Fill(mindR,ptrel);

    if (teve->suffix == "Mu") h_MuCutFlow->Fill(1.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(1.0, teve->weight);

    if (teve->lepton1->lv.Pt() <= 120.) {
      //std::cout << "Failed lepton pt cut" << std::endl;
      continue;
    }

    if (teve->suffix == "Mu") h_MuCutFlow->Fill(2.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(2.0, teve->weight);
   
    if ( teve->suffix == "Mu" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 250. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else if ( teve->suffix == "El" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 250. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else {
      //std::cout << "Failed jet1 cuts" << std::endl;
      continue;
    }//End if lead jet pt>450

    if (teve->suffix == "Mu") h_MuCutFlow->Fill(3.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(3.0, teve->weight);

    fillHistosTwoChannels(GE0BTags_PostJet1_std_h, teve);
    if ( !(teve->vGoodAK4Jets.at(1)->lv.Pt() > 30 && fabs(teve->vGoodAK4Jets.at(1)->lv.Eta())<2.4) ){
      //std::cout << "Failed jet2 cuts" << std::endl;
      continue;
    }//End jet if >=2 jets, sublead jet pt>30
    for (int ti = teve->vGoodAK4Jets.size()-1; ti>=2; ti--) {
      if (teve->vGoodAK4Jets.at(ti)->lv.Pt()<20) teve->vGoodAK4Jets.erase(teve->vGoodAK4Jets.begin()+ti);
    }

    int nJets = teve->vGoodAK4Jets.size();

    fillHistosTwoChannels(GE0BTags_PostJet2_std_h, teve);
    if (teve->suffix == "Mu") h_MuCutFlow->Fill(4.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(4.0, teve->weight);

    if (teve->suffix == "El") {
      h_ElMETdPhi_vs_MET_PostSel->Fill(fabs(elmetdphi),teve->met);
    }
    if (lepjetoverlap) {
      //std::cout << "Failed lep-jet overlap cut" << std::endl;
      continue;
    }
    if (teve->lepton1->isMuon()) h_MuIso_vs_nPV->Fill(teve->lepton1->relIso, teve->nPV);
    else                         h_ElIso_vs_nPV->Fill(teve->lepton1->relIso, teve->nPV);

    //Try to reconstruct the Wprime -- need a W then a top then next leading quark

    //double bestTopMass = 1e8;

    /*std::cout << "---------------------------------" << endl;
    std::cout << "Lep E = " << teve->lepton1->lv.E() << endl;
    std::cout << "Lep Px = " << teve->lepton1->lv.Px() << endl;
    std::cout << "Lep Py = " << teve->lepton1->lv.Py() << endl;
    std::cout << "Lep Pz = " << teve->lepton1->lv.Pz() << endl;
    std::cout << "MET Px = " << teve->met_x << endl;
    std::cout << "MET Py = " << teve->met_y << endl;*/

    //Starting 
    double lepM;
    if (teve->suffix == "Mu") lepM = 0.105658367;
    else lepM = 0.00051099891;

    double Dtmp = (MW*MW)-(lepM*lepM)+2*((teve->lepton1->lv.Px())*(teve->met_x)+(teve->lepton1->lv.Py())*(teve->met_y));
    double Atmp = 4.0*((teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())-(teve->lepton1->lv.Pz())*(teve->lepton1->lv.Pz()));
    double Btmp = -4.0*Dtmp*(teve->lepton1->lv.Pz());
    double Ctmp = 4.0*(teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())*(teve->met)*(teve->met)-Dtmp*Dtmp;
    /*std::cout << "Dtmp = " << Dtmp << endl;
    std::cout << "Atmp = " << Atmp << endl;
    std::cout << "Btmp = " << Btmp << endl;
    std::cout << "Ctmp = " << Ctmp << endl;*/

    double nuPz_1;
    double nuPz_2;
    

    double DETtmp = Btmp*Btmp-4.0*Atmp*Ctmp;
    //std::cout << "DETtmp = " << DETtmp << endl;
    TLorentzVector Wlv_1, Wlv_2;
    //TLorentzVector Nulv;

    if (DETtmp >= 0.) {
      nuPz_1 = (-Btmp+TMath::Sqrt(DETtmp))/(2.0*Atmp);
      nuPz_2 = (-Btmp-TMath::Sqrt(DETtmp))/(2.0*Atmp);
      TLorentzVector Nulv_1(teve->met_x,teve->met_y,nuPz_1,TMath::Sqrt((teve->met)*(teve->met)+(nuPz_1)*(nuPz_1)));
      TLorentzVector Nulv_2(teve->met_x,teve->met_y,nuPz_2,TMath::Sqrt((teve->met)*(teve->met)+(nuPz_2)*(nuPz_2)));
      Wlv_1 = Nulv_1+(teve->lepton1->lv);
      Wlv_2 = Nulv_2+(teve->lepton1->lv);
      /*std::cout << "Nu_1_Pz = " << Nulv_1.Pz() << endl;
      std::cout << "Nu_1_Pt = " << Nulv_1.Pt() << endl;
      std::cout << "Nu_1_M = " << Nulv_1.M() << endl;
      std::cout << "Nu_2_Pz = " << Nulv_2.Pz() << endl;
      std::cout << "Nu_2_Pt = " << Nulv_2.Pt() << endl;
      std::cout << "Nu_2_M = " << Nulv_2.M() << endl;*/
    }
    if (DETtmp < 0.) {
      nuPz_1 = (-Btmp)/(2.0*Atmp);
      nuPz_2 = (-Btmp)/(2.0*Atmp);
      double alpha = (teve->lepton1->lv.Px())*(teve->met_x)/(teve->met)+(teve->lepton1->lv.Py())*(teve->met_y)/(teve->met);
      double Delta = (MW*MW)-(lepM*lepM);
      Atmp = 4.0*((teve->lepton1->lv.Pz())*(teve->lepton1->lv.Pz())-(teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())+(alpha*alpha));
      Btmp = 4.0*alpha*Delta;
      Ctmp = Delta*Delta;
      DETtmp = Btmp*Btmp-4.0*Atmp*Ctmp;
      /*std::cout << "alpha = " << alpha << endl;
      std::cout << "Delta = " << Delta << endl;
      std::cout << "Atmp = " << Atmp << endl;
      std::cout << "Btmp = " << Btmp << endl;
      std::cout << "Ctmp = " << Ctmp << endl;
      std::cout << "DETtmp = " << DETtmp << endl;*/
      double pTnu_1 = (-Btmp+TMath::Sqrt(DETtmp))/(2.0*Atmp);
      double pTnu_2 = (-Btmp-TMath::Sqrt(DETtmp))/(2.0*Atmp);
      //std::cout << "pTnu_1 = " << pTnu_1 << endl;
      //std::cout << "pTnu_2 = " << pTnu_2 << endl;
      TLorentzVector Nulv_1(teve->met_x*(pTnu_1)/(teve->met),teve->met_y*(pTnu_1)/(teve->met),nuPz_1,TMath::Sqrt((pTnu_1)*(pTnu_1)+(nuPz_1)*(nuPz_1)));
      TLorentzVector Nulv_2(teve->met_x*(pTnu_2)/(teve->met),teve->met_y*(pTnu_2)/(teve->met),nuPz_2,TMath::Sqrt((pTnu_2)*(pTnu_2)+(nuPz_2)*(nuPz_2)));
      Wlv_1 = Nulv_1+(teve->lepton1->lv);
      Wlv_2 = Nulv_2+(teve->lepton1->lv);
      if (fabs(Wlv_1.M()-MW) < fabs(Wlv_2.M()-MW)) Wlv_2 = Wlv_1;
      else Wlv_1 = Wlv_2;
      /*std::cout << "Nu_1_Pt = " << Nulv_1.Pt() << endl;
      std::cout << "Nu_1_M = " << Nulv_1.M() << endl;
      std::cout << "Nu_2_Pt = " << Nulv_2.Pt() << endl;
      std::cout << "Nu_2_M = " << Nulv_2.M() << endl;*/
    }
    /*std::cout << "W_1_Pt = " << Wlv_1.Pt() << endl;
    std::cout << "W_1_M = " << Wlv_1.M() << endl;
    std::cout << "W_2_Pt = " << Wlv_2.Pt() << endl;
    std::cout << "W_2_M = " << Wlv_2.M() << endl;*/

    double dMTOP = 1e8;
    unsigned int topIndex=0;
    bool firstW=true;
    double MTop_1, MTop_2;
    for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++){
      if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < minRecojetPt) continue;
      //if (teve->vGoodAK4Jets.at(ij)->lv.DeltaR(teve->lepton1->lv) < 0.5) continue;//overlap
      MTop_1 = ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_1).M();
      MTop_2 = ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_2).M();
      /*std::cout << "Top_1_Pt = " << ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_1).Pt() << endl;
      std::cout << "Top_1_M = " << MTop_1 << endl;
      std::cout << "Top_2_Pt = " << ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_2).Pt() << endl;
      std::cout << "Top_2_M = " << MTop_2 << endl;*/
      if (fabs(MTop_1-MTOP) < dMTOP) {
        if (fabs(MTop_1-MTOP) < fabs(MTop_2-MTOP)) {
          firstW = true;
          topIndex = ij;
          dMTOP = fabs(MTop_1-MTOP);
        }
        else {
          firstW = false;
          topIndex = ij;
          dMTOP = fabs(MTop_2-MTOP);
        }
      }
      else if (fabs(MTop_2-MTOP) < dMTOP) {
        firstW = false;
        topIndex = ij;
        dMTOP = fabs(MTop_2-MTOP);
      }
    }
    if (firstW) {
      teve->lvW = Wlv_1;
    }
    else {
      teve->lvW = Wlv_2;
    }
    if (dMTOP < 1e6) {
      teve->lvTop = (teve->vGoodAK4Jets.at(topIndex)->lv)+(teve->lvW);
      //std::cout << "Final Top_Pt = " << teve->lvTop.Pt() << endl;
      //std::cout << "Final Top_M = " << teve->lvTop.M() << endl;

      if (topIndex == 0) teve->lvWprime = ((teve->vGoodAK4Jets.at(1)->lv)+(teve->lvTop));
      else teve->lvWprime = ((teve->vGoodAK4Jets.at(0)->lv)+(teve->lvTop));
      //if (topIndex == 0 and teve->vGoodAK4Jets.at(1)->lv.DeltaR(teve->lepton1->lv)>0.5) teve->lvWprime = ((teve->vGoodAK4Jets.at(1)->lv)+(teve->lvTop));
      //else if (topIndex != 0 and teve->vGoodAK4Jets.at(0)->lv.DeltaR(teve->lepton1->lv)>0.5) teve->lvWprime = ((teve->vGoodAK4Jets.at(0)->lv)+(teve->lvTop));
      //std::cout << "W'_Pt = " << teve->lvWprime.Pt() << endl;
      //std::cout << "W'_M = " << teve->lvWprime.M() << endl;
    }

    if (!lep2Dcut) {
      fillHistosTwoChannels(GE0BTags_Fail2D_std_h, teve);
    }
    if (teve->suffix == "El" && teve->met <= 120.) {
      //std::cout << "Failed met cut" << std::endl;
      if (lep2Dcut) fillHistosTwoChannels(GE0BTags_FailMETPass2D_std_h, teve);
      continue;
    }
    if (teve->suffix == "Mu" && teve->met <= 50.) {
      //std::cout << "Failed met cut" << std::endl;
      if (lep2Dcut) fillHistosTwoChannels(GE0BTags_FailMETPass2D_std_h, teve);
      continue;
    }
    fillHistosTwoChannels(GE0BTags_PostMET_std_h, teve);

    if (teve->suffix == "El") h_LepdR_vs_pTRel_El_PostSel->Fill(mindR,ptrel);
    if (teve->suffix == "Mu") h_LepdR_vs_pTRel_Mu_PostSel->Fill(mindR,ptrel);

    if (!lep2Dcut) {
      //fillHistosTwoChannels(GE0BTags_Fail2D_std_h, teve);
      //std::cout << "Failed lepton 2D cut" << std::endl;
      continue;
    }
    fillHistosTwoChannels(GE0BTags_Post2D_std_h, teve);

    if (teve->suffix == "Mu") h_MuCutFlow->Fill(5.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(5.0, teve->weight);
    if (teve->suffix == "El" && (fabs(elmetdphi) > 1.5 + ((teve->met)/50.) || fabs(elmetdphi) < 1.5 - ((teve->met)/50.))) {
      //std::cout << "Failed electron triangle cut" << std::endl;
      //continue;
    }
    if (teve->suffix == "El" && fabs(elmetdphi) > 2.0) continue;
    //kill QCD where met comes from mismeasurement of electron/jet
    if (teve->suffix == "Mu") h_MuCutFlow->Fill(6.0, teve->weight);
    if (teve->suffix == "El") h_ElCutFlow->Fill(6.0, teve->weight);

    mindR = 1000;
    for (unsigned int ui = 0; ui < teve->vGoodAK4Jets.size(); ui++){
      if (teve->vGoodAK4Jets.at(ui)->lv.Pt() < min2DjetPt) continue;
      if (teve->vGoodAK4Jets.at(ui)->lv.DeltaR(teve->lepton1->lv) < mindR) mindR = teve->vGoodAK4Jets.at(ui)->lv.DeltaR(teve->lepton1->lv);
    }    
    double genmindR = 1000;
    if (teve->lvGenLepMatch.Pt()>30.) {
      for (unsigned int ui = 0; ui < teve->vGenJets.size(); ui++){
        if (teve->vGenJets.at(ui).Pt()<30.) continue;
        if (teve->vGenJets.at(ui).DeltaR(teve->lvGenLepMatch) < genmindR) genmindR = teve->vGenJets.at(ui).DeltaR(teve->lvGenLepMatch);
      }    
    }    

    if (teve->lepton1->isMuon()) h_MinLepAK4JetDR_vs_GenLepTopBjetDR_Mu->Fill(mindR , (teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep));
    else h_MinLepAK4JetDR_vs_GenLepTopBjetDR_El->Fill(mindR , (teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep));
    //if ((teve->lvGenTop-teve->lvGenW).Pt()>1 && (teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep)>0.38 && (teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep)<0.4) std::cout << "Found genDR=" << (teve->lvGenTop-teve->lvGenW).DeltaR(teve->lvGenLep) << " in : Run " << treeClass->run << ", Lumi " << treeClass->lumi << ", Event " << treeClass->event << std::endl;
    if (teve->lepton1->isMuon()) h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_Mu->Fill(mindR , genmindR);
    else h_MinLepAK4JetDR_vs_GenMinLepAK4JetDR_El->Fill(mindR , genmindR);

    if (teve->lepton1->isMuon() and !firstbtag) h_MinLepAK4JetDR_vs_Mtb_0b_Mu->Fill(mindR , teve->lvWprime.M());
    if (teve->lepton1->isElectron() and !firstbtag) h_MinLepAK4JetDR_vs_Mtb_0b_El->Fill(mindR , teve->lvWprime.M());
    if (teve->lepton1->isMuon() and firstbtag and !first2btag) h_MinLepAK4JetDR_vs_Mtb_1b_Mu->Fill(mindR , teve->lvWprime.M());
    if (teve->lepton1->isElectron() and firstbtag and !first2btag) h_MinLepAK4JetDR_vs_Mtb_1b_El->Fill(mindR , teve->lvWprime.M());
    if (teve->lepton1->isMuon() and first2btag) h_MinLepAK4JetDR_vs_Mtb_2b_Mu->Fill(mindR , teve->lvWprime.M());
    if (teve->lepton1->isElectron() and first2btag) h_MinLepAK4JetDR_vs_Mtb_2b_El->Fill(mindR , teve->lvWprime.M());
    //if ((teve->suffix == "El" && teve->lepton1->miniIso < 0.005) || (teve->suffix == "Mu" && teve->lepton1->miniIso < 0.005)) fillHistosTwoChannels(GE0BTags_PostMini_std_h, teve);

    bool finalcuts = false;
    //if (teve->suffix == "Mu" && (teve->lvTop.Pt() > 250 && teve->lvTop.M() > 100 && teve->lvTop.M() < 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;
    //if (teve->suffix == "El" && (teve->lvTop.Pt() > 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;
    if (teve->lvTop.Pt() > 250 && teve->lvTop.M() > 100 && teve->lvTop.M() < 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350) finalcuts = true;
    bool hardcuts = false;
    if (teve->lvTop.Pt() > 650 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 700) hardcuts = true;

    if (firstbtag) fillHistosTwoChannels(GE1BTags_PostLowJet1LowLep_std_h, teve);
    else fillHistosTwoChannels(Ex0BTags_PostLowJet1LowLep_std_h, teve);
    fillHistosTwoChannels(GE0BTags_PostLowJet1LowLep_std_h, teve);

    if (teve->lepton1->lv.Pt() < 180.) continue;

    //if (firstbtag) fillHistosTwoChannels(GE1BTags_PostLowJet1_std_h, teve);
    //else fillHistosTwoChannels(Ex0BTags_PostLowJet1_std_h, teve);
    //fillHistosTwoChannels(GE0BTags_PostLowJet1_std_h, teve);

    if ( teve->suffix == "Mu" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 450. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else if ( teve->suffix == "El" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 350. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else {
      //std::cout << "Failed jet1 cuts" << std::endl;
      continue;
    }//End if lead jet pt>450

    if (teve->lepton1->isMuon()) h_MuPt_vs_Eta->Fill(teve->lepton1->lv.Pt(), teve->lepton1->lv.Eta());
    else                         h_ElPt_vs_Eta->Fill(teve->lepton1->lv.Pt(), teve->lepton1->lv.Eta());

    fillHistosTwoChannels(GE0BTags_std_h, teve);


    if (!firstbtag) {
      if (teve->lepton1->isElectron()) {
        h_topM_vs_MinLepAK4JetDR_El->Fill(teve->lvTop.M(),mindR,teve->weight);
        h_topM_vs_MET_El->Fill(teve->lvTop.M(),teve->met,teve->weight);
        h_topM_vs_LepPt_El->Fill(teve->lvTop.M(),teve->lepton1->lv.Pt(),teve->weight);
        h_topM_vs_LepEta_El->Fill(teve->lvTop.M(),teve->lepton1->lv.Eta(),teve->weight);
        h_topM_vs_J1Pt_El->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(0)->lv.Pt(),teve->weight);
        h_topM_vs_J2Pt_El->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(1)->lv.Pt(),teve->weight);
        h_topM_vs_TopJetPt_El->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(topIndex)->lv.Pt(),teve->weight);
        h_topM_vs_TopJetEta_El->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(topIndex)->lv.Eta(),teve->weight);
        h_topM_vs_Mtb_El->Fill(teve->lvTop.M(),teve->lvWprime.M(),teve->weight);
        h_topM_vs_Njets_El->Fill(teve->lvTop.M(),nJets,teve->weight);
        h_TopJetClosestToLep_El->Fill(teve->lepton1->lv.DeltaR(teve->vGoodAK4Jets.at(topIndex)->lv) == mindR ? 1. : 0.,teve->weight);
      } else {
        h_topM_vs_MinLepAK4JetDR_Mu->Fill(teve->lvTop.M(),mindR,teve->weight);
        h_topM_vs_MET_Mu->Fill(teve->lvTop.M(),teve->met,teve->weight);
        h_topM_vs_LepPt_Mu->Fill(teve->lvTop.M(),teve->lepton1->lv.Pt(),teve->weight);
        h_topM_vs_LepEta_Mu->Fill(teve->lvTop.M(),teve->lepton1->lv.Eta(),teve->weight);
        h_topM_vs_J1Pt_Mu->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(0)->lv.Pt(),teve->weight);
        h_topM_vs_J2Pt_Mu->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(1)->lv.Pt(),teve->weight);
        h_topM_vs_TopJetPt_Mu->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(topIndex)->lv.Pt(),teve->weight);
        h_topM_vs_TopJetEta_Mu->Fill(teve->lvTop.M(),teve->vGoodAK4Jets.at(topIndex)->lv.Eta(),teve->weight);
        h_topM_vs_Mtb_Mu->Fill(teve->lvTop.M(),teve->lvWprime.M(),teve->weight);
        h_topM_vs_Njets_Mu->Fill(teve->lvTop.M(),nJets,teve->weight);
        h_TopJetClosestToLep_Mu->Fill(teve->lepton1->lv.DeltaR(teve->vGoodAK4Jets.at(topIndex)->lv) == mindR ? 1. : 0.,teve->weight);
      }
    }


    //cout<<"At event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
    //std::cout<<firstbtag<<finalcuts<<std::endl;
    //std::cout<<"1"<<std::endl;

    if (teve->suffix == "El") h_topPt_vs_Mtb_El->Fill(teve->lvTop.Pt(),teve->lvWprime.M());
    if (teve->suffix == "Mu") h_topPt_vs_Mtb_Mu->Fill(teve->lvTop.Pt(),teve->lvWprime.M());

    h_dR_vs_dpT_J1J2->Fill(teve->vGoodAK4Jets.at(0)->lv.DeltaR(teve->vGoodAK4Jets.at(1)->lv), ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt());
    if (teve->suffix=="El") h_dR_vs_dpT_J1El->Fill(teve->vGoodAK4Jets.at(0)->lv.DeltaR(teve->lepton1->lv), ((teve->vGoodAK4Jets.at(0)->lv)+(teve->lepton1->lv)).Pt());
    if (teve->suffix=="Mu") h_dR_vs_dpT_J1Mu->Fill(teve->vGoodAK4Jets.at(0)->lv.DeltaR(teve->lepton1->lv), ((teve->vGoodAK4Jets.at(0)->lv)+(teve->lepton1->lv)).Pt());
    if (teve->suffix=="El") h_dR_vs_dpT_J2El->Fill(teve->vGoodAK4Jets.at(1)->lv.DeltaR(teve->lepton1->lv), ((teve->vGoodAK4Jets.at(1)->lv)+(teve->lepton1->lv)).Pt());
    if (teve->suffix=="Mu") h_dR_vs_dpT_J2Mu->Fill(teve->vGoodAK4Jets.at(1)->lv.DeltaR(teve->lepton1->lv), ((teve->vGoodAK4Jets.at(1)->lv)+(teve->lepton1->lv)).Pt());

    if (teve->lvTop.M() > 100 && teve->lvTop.M() < 250) {
      fillHistosTwoChannels(GE0BTags_PostMTop_std_h, teve);
      if ( teve->lvTop.Pt() > 250 ) {
        fillHistosTwoChannels(GE0BTags_PostPtTop_std_h, teve);
        if ( ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350 ) {
          fillHistosTwoChannels(GE0BTags_PostPtJ1J2_std_h, teve);
        }
      }
    }

    //std::cout<<"2"<<std::endl;

    if (finalcuts) fillHistosTwoChannels(GE0BTags_Final_std_h, teve);
    if ( !firstbtag ) {
      fillHistosTwoChannels(Ex0BTags_std_h, teve);
      if (finalcuts) fillHistosTwoChannels(Ex0BTags_Final_std_h, teve);
    }
    if ( firstbtag && !first2btag) {
      fillHistosTwoChannels(Ex1BTags_std_h, teve);
      if (finalcuts) fillHistosTwoChannels(Ex1BTags_Final_std_h, teve);
      if (finalcuts && !hardcuts) fillHistosTwoChannels(Ex1BTags_Soft_std_h, teve);
      if (hardcuts) fillHistosTwoChannels(Ex1BTags_Hard_std_h, teve);
    }

    if ( firstbtag ) {
      fillHistosTwoChannels(GE1BTags_std_h, teve);
      h_genTpT_vs_genTbarpT->Fill(gentpt,gentbarpt);

      if (teve->suffix == "Mu") h_MuCutFlow->Fill(7.0, teve->weight);
      if (teve->suffix == "El") h_ElCutFlow->Fill(7.0, teve->weight);
      if (teve->lvTop.Pt() > 250) {
        if (teve->suffix == "Mu") h_MuCutFlow->Fill(8.0, teve->weight);
        if (teve->suffix == "El") h_ElCutFlow->Fill(8.0, teve->weight);
        if (teve->lvTop.M() > 100 && teve->lvTop.M() < 250) {
          if (teve->suffix == "Mu") h_MuCutFlow->Fill(9.0, teve->weight);
          if (teve->suffix == "El") h_ElCutFlow->Fill(9.0, teve->weight);
          if (finalcuts) {
            h_genTpT_vs_genTbarpT_Final->Fill(gentpt,gentbarpt);
            fillHistosTwoChannels(GE1BTags_Final_std_h, teve);
            //std::cout<<"3"<<std::endl;
            if (teve->realData and teve->lvWprime.M()>5000.){
              cout<<"-----------------------------------------------------"<<endl;
              cout<<"Found Wprime mass reco event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
              teve->printEvent();
              cout<<"-----------------------------------------------------"<<endl;
            } 

            if (teve->suffix == "Mu") h_MuCutFlow->Fill(10.0, teve->weight);
            if (teve->suffix == "El") h_ElCutFlow->Fill(10.0, teve->weight);
          }
        }
      }
    }
    if ( first2btag ) {
      fillHistosTwoChannels(Ex2BTags_std_h, teve);
      if (finalcuts) fillHistosTwoChannels(Ex2BTags_Final_std_h, teve);
      if (finalcuts && !hardcuts) fillHistosTwoChannels(Ex2BTags_Soft_std_h, teve);
      if (hardcuts) fillHistosTwoChannels(Ex2BTags_Hard_std_h, teve);
    }

	
  }//End loop over entries
  
  if (not teve->realData){
    scaleHistosTwoChannels(PreselPU_std_h);
    scaleHistosTwoChannels(Presel_std_h);

    scaleHistosTwoChannels(GE0BTags_std_h);
    scaleHistosTwoChannels(GE1BTags_std_h);

    scaleHistosTwoChannels(Ex0BTags_std_h);
    scaleHistosTwoChannels(Ex1BTags_std_h);
    scaleHistosTwoChannels(Ex2BTags_std_h);

    scaleHistosTwoChannels(GE0BTags_Post2D_std_h);
    scaleHistosTwoChannels(GE0BTags_Fail2D_std_h);
    scaleHistosTwoChannels(GE0BTags_PostMET_std_h);
    scaleHistosTwoChannels(GE0BTags_FailMET_std_h);
    scaleHistosTwoChannels(GE0BTags_FailMETPass2D_std_h);
    scaleHistosTwoChannels(GE0BTags_PostJet1_std_h);
    scaleHistosTwoChannels(GE0BTags_PostLowJet1LowLep_std_h);
    scaleHistosTwoChannels(GE1BTags_PostLowJet1LowLep_std_h);
    scaleHistosTwoChannels(Ex0BTags_PostLowJet1LowLep_std_h);
    scaleHistosTwoChannels(GE0BTags_PostJet2_std_h);

    scaleHistosTwoChannels(GE0BTags_PostMTop_std_h);
    scaleHistosTwoChannels(GE0BTags_PostPtTop_std_h);
    scaleHistosTwoChannels(GE0BTags_PostPtJ1J2_std_h);

    scaleHistosTwoChannels(GE0BTags_Final_std_h);
    scaleHistosTwoChannels(GE1BTags_Final_std_h);
    
    scaleHistosTwoChannels(Ex0BTags_Final_std_h);
    scaleHistosTwoChannels(Ex1BTags_Final_std_h);
    scaleHistosTwoChannels(Ex2BTags_Final_std_h);

    scaleHistosTwoChannels(Ex1BTags_Soft_std_h);
    scaleHistosTwoChannels(Ex2BTags_Soft_std_h);
    scaleHistosTwoChannels(Ex1BTags_Hard_std_h);
    scaleHistosTwoChannels(Ex2BTags_Hard_std_h);

  }

  h_MuCutFlow->GetXaxis()->SetBinLabel(1,"Preselection");
  h_MuCutFlow->GetXaxis()->SetBinLabel(2,"Trigger");
  h_MuCutFlow->GetXaxis()->SetBinLabel(3,"p_{T}^{lep}>180");
  h_MuCutFlow->GetXaxis()->SetBinLabel(4,"MET>50");
  h_MuCutFlow->GetXaxis()->SetBinLabel(5,"p_{T}^{jet1}>450, p_{T}^{jet2}>30");
  h_MuCutFlow->GetXaxis()->SetBinLabel(6,"Lepton 2D cut");
  h_MuCutFlow->GetXaxis()->SetBinLabel(7,"Electron triangle cuts");
  h_MuCutFlow->GetXaxis()->SetBinLabel(8,"nbtags#geq1");
  h_MuCutFlow->GetXaxis()->SetBinLabel(9,"p_{T}^{top}>250");
  h_MuCutFlow->GetXaxis()->SetBinLabel(10,"100<M_{top}<250");
  h_MuCutFlow->GetXaxis()->SetBinLabel(11,"p_{T}^{jet1,jet2}>350");
  h_ElCutFlow->GetXaxis()->SetBinLabel(1,"Preselection");
  h_ElCutFlow->GetXaxis()->SetBinLabel(2,"Trigger");
  h_ElCutFlow->GetXaxis()->SetBinLabel(3,"p_{T}^{lep}>180");
  h_ElCutFlow->GetXaxis()->SetBinLabel(4,"MET>120");
  h_ElCutFlow->GetXaxis()->SetBinLabel(5,"p_{T}^{jet1}>350, p_{T}^{jet2}>30");
  h_ElCutFlow->GetXaxis()->SetBinLabel(6,"Lepton 2D cut");
  h_ElCutFlow->GetXaxis()->SetBinLabel(7,"Electron triangle cuts");
  h_ElCutFlow->GetXaxis()->SetBinLabel(8,"nbtags#geq1");
  h_ElCutFlow->GetXaxis()->SetBinLabel(9,"p_{T}^{top}>250");
  h_ElCutFlow->GetXaxis()->SetBinLabel(10,"100<M_{top}<250");
  h_ElCutFlow->GetXaxis()->SetBinLabel(11,"p_{T}^{jet1,jet2}>350");

  cout<<"Number of duplicate events: "<<nDup<<endl;
  
  outFile->Write();
//   syncFile->Write();
  return 0;
}


#endif
