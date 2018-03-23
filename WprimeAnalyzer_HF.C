#ifndef WprimeAnalyzer_HF_cxx
#define WprimeAnalyzer_HF_cxx

#include "WprimeLoadData2016.C"
#include "WprimeStandardHistos_HF.C"
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
    if (jet->lv.Pt() < 670.) return BtagEff.Val(jet->lv.Pt(), jet->lv.Eta());
    else return 0.8635025-0.0002091*jet->lv.Pt();
  }
  else {
    if (jet->lv.Pt() < 670.) return MistagEff.Val(jet->lv.Pt(), jet->lv.Eta());
    else return 0.174574;
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

  //int MIN_NBTAG =1;
//   int MIN_HT =0;

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
  
  if (b_RealData) sprintf(name,"aHFOutFile_%s_%s_%lx.root", argv[2], argv[4], rawtime);
//  else            sprintf(name,"aHFOutFile_%s_%lx.root",    argv[2], rawtime);
   else            sprintf(name,"aHFOutFile_%s_%s.root",    argv[2], argv[4]);

  TFile* outFile = new TFile(name, "RECREATE");

  vector <StandardHistos*> Ex0BTags_LF_std_h(2);
  vector <StandardHistos*> GE0BTags_LF_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_std_h(2);
  vector <StandardHistos*> GE0BTags_HF_std_h(2);
  vector <StandardHistos*> Ex0BTags_LF_Boost_std_h(2);
  vector <StandardHistos*> GE0BTags_LF_Boost_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_Boost_std_h(2);
  vector <StandardHistos*> GE0BTags_HF_Boost_std_h(2);
  vector <StandardHistos*> Ex0BTags_LF_BUP_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_BUP_std_h(2);
  vector <StandardHistos*> Ex0BTags_LF_BDOWN_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_BDOWN_std_h(2);
  vector <StandardHistos*> Ex0BTags_LF_MUP_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_MUP_std_h(2);
  vector <StandardHistos*> Ex0BTags_LF_MDOWN_std_h(2);
  vector <StandardHistos*> Ex0BTags_HF_MDOWN_std_h(2);

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_std_h,           "Ex0BTags_LF",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_LF_std_h,           "GE0BTags_LF",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_std_h,           "Ex0BTags_HF",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_HF_std_h,           "GE0BTags_HF",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_Boost_std_h,     "Ex0BTags_LF_Boost",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_LF_Boost_std_h,     "GE0BTags_LF_Boost",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_Boost_std_h,     "Ex0BTags_HF_Boost",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_HF_Boost_std_h,     "GE0BTags_HF_Boost",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_BUP_std_h,       "Ex0BTags_LF_BUP",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_BUP_std_h,       "Ex0BTags_HF_BUP",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_BDOWN_std_h,     "Ex0BTags_LF_BDOWN",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_BDOWN_std_h,     "Ex0BTags_HF_BDOWN",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_MUP_std_h,       "Ex0BTags_LF_MUP",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_MUP_std_h,       "Ex0BTags_HF_MUP",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_LF_MDOWN_std_h,     "Ex0BTags_LF_MDOWN",    sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex0BTags_HF_MDOWN_std_h,     "Ex0BTags_HF_MDOWN",    sampleName, sScale);

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

    if (teve->realData) {
      //elcsc.filter(treeClass->run,treeClass->lumi,treeClass->event);
    }
    // Review PU:
    int nti = (int) treeClass->nTrueInteractions;
    if (teve->realData) teve->weight = 1.0;
    else                teve->weight = lumiReWeighting.weight(nti);
    //else                teve->weight = 1.0;
    //else                teve->weight = weightpu[teve->nPV];
//     else                teve->weight = treeClass->weight_PU;
    if (treeClass->weight_MC<0) teve->weight=(-1.0)*(teve->weight);


    if (sampleName.BeginsWith("WJets_HT100to200")) teve->weight *= wjlhe_100200;
    if (sampleName.BeginsWith("WJets_HT200to400")) teve->weight *= wjlhe_200400;
    if (sampleName.BeginsWith("WJets_HT400to600")) teve->weight *= wjlhe_400600;
    if (sampleName.BeginsWith("WJets_HT600to800")) teve->weight *= wjlhe_600800;
    if (sampleName.BeginsWith("WJets_HT800to1200")) teve->weight *= wjlhe_8001200;
    if (sampleName.BeginsWith("WJets_HT1200to2500")) teve->weight *= wjlhe_12002500;
    if (sampleName.BeginsWith("WJets_HT2500toInf")) teve->weight *= wjlhe_2500inf;


    if (b_Signal) teve->lim_weight = teve->weight*(1/xSection);
    else teve->lim_weight = teve->weight;
    
    teve->PUweight = teve->weight;
    if (sampleName=="TTbar") {
      for (unsigned int ui = 0; ui < treeClass->genID->size(); ui++){
        if (abs(treeClass->genID->at(ui))==6 && treeClass->genStatus->at(ui)==62) {
        //  std::cout<<"Top pT = "<<treeClass->genPt->at(ui)<<" , weight = "<<TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))<<std::endl;
          teve->weight *= TMath::Sqrt(TMath::Exp(0.0615-0.0005*treeClass->genPt->at(ui)));
          teve->lim_weight *= TMath::Sqrt(TMath::Exp(0.0615-0.0005*treeClass->genPt->at(ui)));
        }
      }
    }

    //check that there is only one lepton in the event
    if (treeClass->goodLeptons.size() == 1){
      teve->lepton1 = treeClass->goodLeptons.at(0);
    }//End of exactly one lepton check
    else continue;

    if (teve->lepton1->lv.Pt() <= 180.) {
      //std::cout << "Failed lepton pt cut" << std::endl;
      continue;
    }

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

    //Select only events which pass the trigger for the channel we are looking at
    bool trigMuMC = false;
    bool trigElMC = false;
    bool trigMu = false;
    bool trigEl = false;
    bool trigMuMCMatch = false;
    bool trigElMCMatch = false;
    bool trigMuMatch = false;
    bool trigElMatch = false;
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
    int nJets = teve->vGoodAK4Jets.size();

    if ( teve->suffix == "Mu" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 450. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else if ( teve->suffix == "El" && teve->vGoodAK4Jets.at(0)->lv.Pt() > 350. && fabs(teve->vGoodAK4Jets.at(0)->lv.Eta())<2.4 ){}
    else {
      //std::cout << "Failed jet1 cuts" << std::endl;
      continue;
    }//End if lead jet pt>450
    if ( !(teve->vGoodAK4Jets.at(1)->lv.Pt() > 30 && fabs(teve->vGoodAK4Jets.at(1)->lv.Eta())<2.4) ){
      //std::cout << "Failed jet2 cuts" << std::endl;
      continue;
    }//End jet if >=2 jets, sublead jet pt>30
    for (int ti = teve->vGoodAK4Jets.size()-1; ti>=2; ti--) {
      if (teve->vGoodAK4Jets.at(ti)->lv.Pt()<20) teve->vGoodAK4Jets.erase(teve->vGoodAK4Jets.begin()+ti);
    }

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

    if (teve->suffix == "El" && teve->met <= 120.) {
      //std::cout << "Failed met cut" << std::endl;
      continue;
    }
    if (teve->suffix == "Mu" && teve->met <= 50.) {
      //std::cout << "Failed met cut" << std::endl;
      continue;
    }

    double min2DjetPt;
    if (teve->suffix == "El") min2DjetPt = 25.;
    if (teve->suffix == "Mu") min2DjetPt = 25.;
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
      if (teve->nBjs>0) teve->weight *= 1.0;
      else if (teve->nBjs==0 && teve->nCjs>0) teve->weight *= 1.0;
      else if (teve->nBjs==0 && teve->nCjs==0 && teve->nLjs>0) teve->weight *= 1.0;
      /*for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++){
        if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < 250.) continue;
        double jetpt = teve->vGoodAK4Jets.at(ij)->lv.Pt();
        teve->weight *= (1.216 - 0.0008272*jetpt + 0.0000001934*jetpt*jetpt);
        teve->lim_weight *= (1.216 - 0.0008272*jetpt + 0.0000001934*jetpt*jetpt);
      }*/
    }
    /*double PMC = 1.;
    double PDATA = 1.;
    if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE) {
      PMC *= getBTagEff(teve->vGoodAK4Jets.at(0));
      PDATA *= getBTagEff(teve->vGoodAK4Jets.at(0))*getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS);
    }
    else {
      PMC *= (1-getBTagEff(teve->vGoodAK4Jets.at(0)));
      PDATA *= (1-getBTagEff(teve->vGoodAK4Jets.at(0))*getBTagSF(teve->vGoodAK4Jets.at(0),BTAGSYS));
    }
    if (teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) {
      PMC *= getBTagEff(teve->vGoodAK4Jets.at(1));
      PDATA *= getBTagEff(teve->vGoodAK4Jets.at(1))*getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS);
    }
    else {
      PMC *= (1-getBTagEff(teve->vGoodAK4Jets.at(1)));
      PDATA *= (1-getBTagEff(teve->vGoodAK4Jets.at(1))*getBTagSF(teve->vGoodAK4Jets.at(1),BTAGSYS));
    }
    if (PMC>0. && not teve->realData) teve->weight *= (PDATA/PMC);*/
    //std::cout<<"BTag weight = "<<PDATA/PMC<<std::endl;
    //if (mindR <= 0.4) lepjetoverlap = true;
    if (mindR <= 0.4 && ptrel < 50 && teve->suffix == "Mu") continue;
    if (mindR <= 0.4 && ptrel < 60 && teve->suffix == "El") continue;
    bool firstbtag = false;
    bool firstbtagBUP = false;
    bool firstbtagBDOWN = false;
    bool firstbtagMUP = false;
    bool firstbtagMDOWN = false;
    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) firstbtag = true;
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) firstbtag = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[1] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[1] > 0) firstbtagBUP = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[2] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[2] > 0) firstbtagBDOWN = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[3] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[3] > 0) firstbtagMUP = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[4] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[4] > 0) firstbtagMDOWN = true;

    // Trigger SF
    if (not teve->realData){
      if (teve->suffix == "Mu") teve->weight *= 1.0;
      if (teve->suffix == "El") teve->weight *= 1.0;
    }

    double elmetdphi = 999.;
    if (teve->suffix == "El") {
      elmetdphi = teve->lepton1->lv.Phi()-teve->met_phi;
      if (elmetdphi>TMath::Pi()) elmetdphi -= 2*TMath::Pi();
      if (elmetdphi<=-TMath::Pi()) elmetdphi += 2*TMath::Pi();
      if (fabs(elmetdphi) > 2.0) continue;
    }

    //Try to reconstruct the Wprime -- need a W then a top then next leading quark

    //double bestTopMass = 1e8;

    //std::cout << "---------------------------------" << endl;
    //std::cout << "Lep E = " << teve->lepton1->lv.E() << endl;
    //std::cout << "Lep Px = " << teve->lepton1->lv.Px() << endl;
    //std::cout << "Lep Py = " << teve->lepton1->lv.Py() << endl;
    //std::cout << "Lep Pz = " << teve->lepton1->lv.Pz() << endl;
    //std::cout << "MET Px = " << teve->met_x << endl;
    //std::cout << "MET Py = " << teve->met_y << endl;      

    //Starting 
    double lepM;
    if (teve->suffix == "Mu") lepM = 0.105658367;
    else lepM = 0.00051099891;

    double Dtmp = (MW*MW)-(lepM*lepM)+2*((teve->lepton1->lv.Px())*(teve->met_x)+(teve->lepton1->lv.Py())*(teve->met_y));
    double Atmp = 4.0*((teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())-(teve->lepton1->lv.Pz())*(teve->lepton1->lv.Pz()));
    double Btmp = -4.0*Dtmp*(teve->lepton1->lv.Pz());
    double Ctmp = 4.0*(teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())*(teve->met)*(teve->met)-Dtmp*Dtmp;
    //std::cout << "Dtmp = " << Dtmp << endl;
    //std::cout << "Atmp = " << Atmp << endl;
    //std::cout << "Btmp = " << Btmp << endl;
    //std::cout << "Ctmp = " << Ctmp << endl;

    double nuPz_1;
    double nuPz_2;
    

    double DETtmp = Btmp*Btmp-4.0*Atmp*Ctmp;
    //std::cout << "DETtmp = " << DETtmp << endl;
    TLorentzVector Wlv_1, Wlv_2;
    //TLorentzVector Nulv;

    if (DETtmp >= 0) {
      nuPz_1 = (-Btmp+TMath::Sqrt(DETtmp))/(2.0*Atmp);
      nuPz_2 = (-Btmp-TMath::Sqrt(DETtmp))/(2.0*Atmp);
      TLorentzVector Nulv_1(teve->met_x,teve->met_y,nuPz_1,TMath::Sqrt((teve->met)*(teve->met)+(nuPz_1)*(nuPz_1)));
      TLorentzVector Nulv_2(teve->met_x,teve->met_y,nuPz_2,TMath::Sqrt((teve->met)*(teve->met)+(nuPz_2)*(nuPz_2)));
      Wlv_1 = Nulv_1+(teve->lepton1->lv);
      Wlv_2 = Nulv_2+(teve->lepton1->lv);
      //std::cout << "Nu_1_Pz = " << Nulv_1.Pz() << endl;
      //std::cout << "Nu_1_Pt = " << Nulv_1.Pt() << endl;
      //std::cout << "Nu_1_M = " << Nulv_1.M() << endl;
      //std::cout << "Nu_2_Pz = " << Nulv_2.Pz() << endl;
      //std::cout << "Nu_2_Pt = " << Nulv_2.Pt() << endl;
      //std::cout << "Nu_2_M = " << Nulv_2.M() << endl;
    }
    if (DETtmp < 0) {
      nuPz_1 = (-Btmp)/(2.0*Atmp);
      nuPz_2 = (-Btmp)/(2.0*Atmp);
      double alpha = (teve->lepton1->lv.Px())*(teve->met_x)/(teve->met)+(teve->lepton1->lv.Py())*(teve->met_y)/(teve->met);
      double Delta = (MW*MW)-(lepM*lepM);
      Atmp = 4.0*((teve->lepton1->lv.Pz())*(teve->lepton1->lv.Pz())-(teve->lepton1->lv.Energy())*(teve->lepton1->lv.Energy())+(alpha*alpha));
      Btmp = 4.0*alpha*Delta;
      Ctmp = Delta*Delta;
      DETtmp = Btmp*Btmp-4.0*Atmp*Ctmp;
      //std::cout << "alpha = " << alpha << endl;
      //std::cout << "Delta = " << Delta << endl;
      //std::cout << "Atmp = " << Atmp << endl;
      //std::cout << "Btmp = " << Btmp << endl;
      //std::cout << "Ctmp = " << Ctmp << endl;
      //std::cout << "DETtmp = " << DETtmp << endl;
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
      //std::cout << "Nu_1_Pt = " << Nulv_1.Pt() << endl;
      //std::cout << "Nu_1_M = " << Nulv_1.M() << endl;
      //std::cout << "Nu_2_Pt = " << Nulv_2.Pt() << endl;
      //std::cout << "Nu_2_M = " << Nulv_2.M() << endl;
    }
    //std::cout << "W_1_Pt = " << Wlv_1.Pt() << endl;
    //std::cout << "W_1_M = " << Wlv_1.M() << endl;
    //std::cout << "W_2_Pt = " << Wlv_2.Pt() << endl;
    //std::cout << "W_2_M = " << Wlv_2.M() << endl;

    double dMTOP = 1e8;
    unsigned int topIndex=0;
    bool firstW=true;
    double MTop_1, MTop_2;
    for (unsigned int ij = 0; ij < teve->vGoodAK4Jets.size(); ij++){
      if (teve->vGoodAK4Jets.at(ij)->lv.Pt() < min2DjetPt) continue;
      MTop_1 = ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_1).M();
      MTop_2 = ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_2).M();
      //std::cout << "Top_1_Pt = " << ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_1).Pt() << endl;
      //std::cout << "Top_1_M = " << MTop_1 << endl;
      //std::cout << "Top_2_Pt = " << ((teve->vGoodAK4Jets.at(ij)->lv)+Wlv_2).Pt() << endl;
      //std::cout << "Top_2_M = " << MTop_2 << endl;
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
    teve->lvTop = (teve->vGoodAK4Jets.at(topIndex)->lv)+(teve->lvW);
    //std::cout << "Final Top_Pt = " << teve->lvTop.Pt() << endl;
    //std::cout << "Final Top_M = " << teve->lvTop.M() << endl;

    if (topIndex == 0) teve->lvWprime = ((teve->vGoodAK4Jets.at(1)->lv)+(teve->lvTop));
    else teve->lvWprime = ((teve->vGoodAK4Jets.at(0)->lv)+(teve->lvTop));
    //std::cout << "W'_Pt = " << teve->lvWprime.Pt() << endl;
    //std::cout << "W'_M = " << teve->lvWprime.M() << endl;
    
    bool finalcuts = false;
    //if (teve->suffix == "Mu" && (teve->lvTop.Pt() > 250 && teve->lvTop.M() > 100 && teve->lvTop.M() < 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;
    //if (teve->suffix == "El" && (teve->lvTop.Pt() > 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;
    if (teve->lvTop.Pt() > 250 && teve->lvTop.M() > 100 && teve->lvTop.M() < 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350) finalcuts = true;

    if (sampleIndex == Data and teve->lvWprime.M()>5000.){
	cout<<"-----------------------------------------------------"<<endl;
	cout<<"Found Wprime mass reco event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
	teve->printEvent();
	cout<<"-----------------------------------------------------"<<endl;
    }

    //if (!finalcuts) continue;

    if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(GE0BTags_HF_std_h, teve);
    else fillHistosTwoChannels(GE0BTags_LF_std_h, teve);
    if ( !firstbtag) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_std_h, teve);
      else fillHistosTwoChannels(Ex0BTags_LF_std_h, teve);
    }
    if ( !firstbtagBUP) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_BUP_std_h, teve);
      else fillHistosTwoChannels(Ex0BTags_LF_BUP_std_h, teve);
    }
    if ( !firstbtagBDOWN) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_BDOWN_std_h, teve);
      else fillHistosTwoChannels(Ex0BTags_LF_BDOWN_std_h, teve);
    }
    if ( !firstbtagMUP) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_MUP_std_h, teve);
      else fillHistosTwoChannels(Ex0BTags_LF_MUP_std_h, teve);
    }
    if ( !firstbtagMDOWN) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_MDOWN_std_h, teve);
      else fillHistosTwoChannels(Ex0BTags_LF_MDOWN_std_h, teve);
    }
    //if (teve->vGoodAK4Jets.at(0)->lv.Pt() > 600. ) {
    if (finalcuts) {
      if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(GE0BTags_HF_Boost_std_h, teve);
      else fillHistosTwoChannels(GE0BTags_LF_Boost_std_h, teve);
      if ( !firstbtag) {
        if (teve->nBjs>0 || teve->nCjs>0) fillHistosTwoChannels(Ex0BTags_HF_Boost_std_h, teve);
        else fillHistosTwoChannels(Ex0BTags_LF_Boost_std_h, teve);
      }
    }
	
  }//End loop over entries
  
  if (not teve->realData){
    scaleHistosTwoChannels(Ex0BTags_LF_std_h);
    scaleHistosTwoChannels(GE0BTags_LF_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_std_h);
    scaleHistosTwoChannels(GE0BTags_HF_std_h);
    scaleHistosTwoChannels(Ex0BTags_LF_Boost_std_h);
    scaleHistosTwoChannels(GE0BTags_LF_Boost_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_Boost_std_h);
    scaleHistosTwoChannels(GE0BTags_HF_Boost_std_h);
    scaleHistosTwoChannels(Ex0BTags_LF_BUP_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_BUP_std_h);
    scaleHistosTwoChannels(Ex0BTags_LF_BDOWN_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_BDOWN_std_h);
    scaleHistosTwoChannels(Ex0BTags_LF_MUP_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_MUP_std_h);
    scaleHistosTwoChannels(Ex0BTags_LF_MDOWN_std_h);
    scaleHistosTwoChannels(Ex0BTags_HF_MDOWN_std_h);
  }

  cout<<"Number of duplicate events: "<<nDup<<endl;
  
  outFile->Write();
//   syncFile->Write();
  return 0;
}


#endif
