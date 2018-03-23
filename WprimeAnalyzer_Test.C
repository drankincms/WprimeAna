#ifndef WprimeAnalyzer_Slim_cxx
#define WprimeAnalyzer_Slim_cxx

#include "WprimeLoadData2016.C"
#include "WprimeStandardHistos_Slim.C"
#include "GetWprimeGenParticles.C"
#include <iomanip>
#include <set>
#include "Objects/interface/HcalLaserEventFilter2012.h"
#include "Objects/interface/LumiReWeighting.h"
#include "Objects/interface/TableReader.h"

using namespace std;

// Lepton ID and Trig
TableReader elIDSF("elIDSF_2016B.txt");
TableReader elTrigSF("elTrigSF_2016B.txt");
TableReader muIDSF("muIDSF_2016B.txt");
TableReader muTrigSF_b1("muTrigSF_2016B_b1.txt");
TableReader muTrigSF_b2("muTrigSF_2016B_b2.txt");
TableReader muTrigSF_b3("muTrigSF_2016B_b1.txt");

TableReader  BtagEff("BtagEff_2015D.txt");
TableReader  MistagEff("MistagEff_2015D.txt");
TableReader  BtagSF("BtagSF_2015D.txt");
TableReader  MistagSF("MistagSF_2015D.txt");
TableReader  BtagSF_UP("BtagSF_2015D_UP.txt");
TableReader  MistagSF_UP("MistagSF_2015D_UP.txt");
TableReader  BtagSF_DOWN("BtagSF_2015D_DOWN.txt");
TableReader  MistagSF_DOWN("MistagSF_2015D_DOWN.txt");

float getLeptonSF (TLepton * lepton, Long64_t evt, Long64_t tot)
{
  float lumi = 36150.0;
  if (lepton->isMuon()) {
    if (evt < (Long64_t)(tot*94./lumi)) return (muIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * muTrigSF_b1.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
    else if (evt < (Long64_t)(tot*589./lumi)) return (muIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * muTrigSF_b2.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
    else return (muIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * muTrigSF_b3.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
  }
  else return (elIDSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())) * elTrigSF.Val(lepton->lv.Pt(), fabs(lepton->lv.Eta())));
}

float getBTagSF (TJet * jet, std::string btagsys)
{
  if (btagsys == "NOM") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
    else return MistagSF.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
  }
  else if (btagsys == "UP") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF_UP.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
    else return MistagSF_UP.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
  }
  else if (btagsys == "DOWN") {
    if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagSF_DOWN.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
    else return MistagSF_DOWN.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
  }
  return 0;
}

float getBTagEff (TJet * jet)
{
  if (jet->hadronFlavor==5 or jet->hadronFlavor==4) return BtagEff.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
  else return MistagEff.Val(jet->lv.Pt(), fabs(jet->lv.Eta()));
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

  double WJHFSF = 2.09269;
  double WJLFSF = 0.519313;
  //double WJHFSF = 1.523;//nom   tpr
  //double WJLFSF = 0.758;//nom,  tpr
  //double WJHFSF = 0.869;//nom
  //double WJLFSF = 1.062;//nom

  double BTAGCUTLOOSE = 0.605; // loose ||| see https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation74X
  double BTAGCUTMED = 0.89; // medium
  double BTAGCUTTIGHT = 0.97; // tight

  std::string BTAGSYS = "NOM";
 
 
  if (argc < 5) {
    cout<<"Need 4 arguments:\n";
    cout<<"- lepton channel (El, Mu)\n";
    cout<<"- sample"<<endl;
    cout<<"- root file name"<<endl;
    cout<<"- output file name"<<endl;
    return 0;
  }


  //CSC Tight Halo Muon Filter (despite name)
  //HcalLaserEventFilter2012 elcsc("./cscbadee2015_Dec01.txt.gz");
 
  TString inSuffix(argv[1]);
  TString sampleName(argv[2]);
  TString fileName(argv[3]);
  TString outName(argv[4]);
  
  cout<<"Processing "<<sampleName<<'\t'<<fileName<<endl;

  if (sampleName.BeginsWith("WJets") && WJHFSF!=1.) cout<<"Applying W+Jets Heavy Flavor Scale Factor"<<endl;
  
  WprimeEvent * teve = new WprimeEvent();
  initAll_2015(fileName, sampleName);

  bool b_Signal = Sig[sampleIndex];
  bool b_RealData = (sampleIndex == Data);

  // Init LumiReWeighting
  
  std::string generatedFile("Spring16MC_25ns_PUScenarioV1.root");
  std::string dataFile("DataPU_Jul21_2016BCD.root");
  std::string dataFileUp("DataPU_Jul21_2016BCD_plus.root");//pileup up
  std::string genHistName("histo");
  std::string dataHistName("pileup");
  fexists(generatedFile,true);
  fexists(dataFile,true);
  LumiReWeighting lumiReWeighting(generatedFile, dataFile, genHistName, dataHistName);
  LumiReWeighting lumiReWeightingUp(generatedFile, dataFileUp, genHistName, dataHistName);

  //File for synchronization
//   char syncName[300];
  time_t rawtime;
  time ( &rawtime );

  //Output file
  char name[300];
  
  if (b_RealData) sprintf(name,"aSlimOutFile_msp_%s_%s_%lx.root", argv[2], argv[4], rawtime);
//  else            sprintf(name,"aSlimOutFile_%s_%lx.root",    argv[2], rawtime);
   else            sprintf(name,"aSlimOutFile_msp_%s_%s.root",    argv[2], argv[4]);

  TFile* outFile = new TFile(name, "RECREATE");

  vector <StandardHistos*> Ex0BTags_std_h(2);
  vector <StandardHistos*> Ex1BTags_std_h(2);
  vector <StandardHistos*> Ex2BTags_std_h(2);
  vector <StandardHistos*> GE1BTags_std_h(2);
  vector <StandardHistos*> GE0BTags_std_h(2);

  vector <StandardHistos*> Ex0BTags_Final_std_h(2);
  vector <StandardHistos*> Ex1BTags_Final_std_h(2);
  vector <StandardHistos*> Ex2BTags_Final_std_h(2);
  vector <StandardHistos*> GE1BTags_Final_std_h(2);
  vector <StandardHistos*> GE1BTags_Final_Pos_std_h(2);
  vector <StandardHistos*> GE1BTags_Final_Neg_std_h(2);
  vector <StandardHistos*> GE0BTags_Final_std_h(2);

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  initHistoDirTwoChannels(outFile, Ex0BTags_std_h,           "Ex0BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex1BTags_std_h,           "Ex1BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_std_h,           "Ex2BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_std_h,           "GE1BTags",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_std_h,           "GE0BTags",          sampleName, sScale);

  initHistoDirTwoChannels(outFile, Ex0BTags_Final_std_h,           "Ex0BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex1BTags_Final_std_h,           "Ex1BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, Ex2BTags_Final_std_h,           "Ex2BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_Final_std_h,           "GE1BTags_Final",          sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_Final_Pos_std_h,       "GE1BTags_Final_Pos",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE1BTags_Final_Neg_std_h,       "GE1BTags_Final_Neg",      sampleName, sScale);
  initHistoDirTwoChannels(outFile, GE0BTags_Final_std_h,           "GE0BTags_Final",          sampleName, sScale);

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
    //if (teve->realData) {
    //  elcsc.filter(treeClass->run,treeClass->lumi,treeClass->event);
    //}

    
    //Initialize WprimeEvent class
    teve->clearEvent();
    teve->isam     = sampleIndex;
    teve->realData = b_RealData;
    teve->nPV      = treeClass->nPV;


    //double weightpu[50];  std::fill_n(weightpu, 50, 1.);
    // Review PU:
    int nti = (int) treeClass->nTrueInteractions;
    if (teve->realData) teve->weight = 1.0;
    else                teve->weight = lumiReWeighting.weight(nti);
    //else                teve->weight = 1.0;
    //else                teve->weight = weightpu[teve->nPV];
//     else                teve->weight = treeClass->weight_PU;
    if (treeClass->weight_MC<0) teve->weight=(-1.0)*(teve->weight);
    /*for (unsigned int it = 0; it<treeClass->LHEweightids->size(); it++) {
      if (treeClass->LHEweightids->at(it)==1005 || treeClass->LHEweightids->at(it)==5) murmufweightdown = treeClass->LHEweights->at(it);//correlated down
      if (treeClass->LHEweightids->at(it)==1009 || treeClass->LHEweightids->at(it)==9) murmufweightup = treeClass->LHEweights->at(it);//correlated up
      if (treeClass->LHEweightids->at(it)>1010) break;
    }*/
    if (b_Signal) teve->lim_weight = teve->weight*(1/xSection);
    else teve->lim_weight = teve->weight;

    double puweightup = lumiReWeightingUp.weight(nti);
    
    teve->PUweight = teve->weight;

    if (sampleName=="TTbar" or sampleName=="TTbar_scaleup" or sampleName=="TTbar_scaledown") {
      for (unsigned int ui = 0; ui < treeClass->genID->size(); ui++){
        if (abs(treeClass->genID->at(ui))==6 and treeClass->genStatus->at(ui)==62) {
          //std::cout<<"Top pT = "<<treeClass->genPt->at(ui)<<" , weight = "<<TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))<<std::endl;
          teve->weight *= TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))/0.99659;//0.99626(up), 0.99638(nom), 0.99659(down)
          teve->lim_weight *= TMath::Sqrt(TMath::Exp(0.156-0.00137*treeClass->genPt->at(ui)))/0.99659;
        }
      }
    }

    //check that there is only one lepton in the event
    if (treeClass->goodLeptons.size() == 1){
      teve->lepton1 = treeClass->goodLeptons.at(0);
    }//End of exactly one lepton check
    else continue;

    teve->met = treeClass->corr_met;
    teve->met_x = treeClass->corr_met_px;
    teve->met_y = treeClass->corr_met_py;
    teve->met_phi = treeClass->corr_met_phi;

    if (teve->lepton1->isMuon()) {
      double temppt = 1./((1./teve->lepton1->lv.Pt())+(teve->lepton1->charge)*(fabs(teve->lepton1->lv.Eta()) >= 1.2 ? 0.0001 : 0.00005));
      double metcx = (teve->lepton1->lv.Pt()-temppt)*TMath::Cos(teve->lepton1->lv.Phi());
      double metcy = (teve->lepton1->lv.Pt()-temppt)*TMath::Sin(teve->lepton1->lv.Phi());
      teve->met_x += metcx;
      teve->met_y += metcy;
      teve->met = TMath::Sqrt((teve->met_x*teve->met_x)+(teve->met_y*teve->met_y));
      teve->met_phi = TMath::ACos(teve->met_x/teve->met);
      
      teve->lepton1->lv.SetPtEtaPhiE(temppt,teve->lepton1->lv.Eta(),teve->lepton1->lv.Phi(),temppt*TMath::CosH(teve->lepton1->lv.Eta()));
    }

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
    if (teve->suffix == "Mu" and (teve->lepton1->ipXY > 0.2 or teve->lepton1->ipZ > 0.5 or (teve->lepton1->lv.Pt()>500. and fabs(teve->lepton1->lv.Eta())>1.2))) {
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
      trigMuMC = true;
      trigElMC = true;
      trigMuMCMatch = true;
      trigElMCMatch = true;
      /*if (teve->suffix == "Mu") {
        trigElMC = true;
        trigElMCMatch = true;
        for (unsigned int im = 0; im < treeClass->vsSelMCTriggersMu->size(); im++) {
          if (startsWith(treeClass->vsSelMCTriggersMu->at(im),"HLT_Mu45_eta2p1_v")) {
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
          if (startsWith(treeClass->vsSelMCTriggersEl->at(im),"HLT_Ele105_CaloIdVT_GsfTrkIdT_v")) {
          //if (treeClass->vsSelMCTriggersEl->at(im) == "HLT_Ele27_eta2p1_WP75_Gsf_v1") {
            if (treeClass->viSelMCTriggersEl->at(im) == 1) trigElMC = true;
          }
        }
        for (unsigned int im = 0; im < treeClass->electron_hltfilters->size(); im++) {
          if (treeClass->electron_hltfilters->at(im) == "hltEle105CaloIdVTGsfTrkIdTGsfDphiFilter") {
            if (treeClass->electron_1_hltmatched->at(im) == 1) trigElMCMatch = true;
          }
        }
      }*/
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
          if (startsWith(treeClass->vsSelTriggersMu->at(im),"HLT_Mu45_eta2p1_v")) {
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
          if (startsWith(treeClass->vsSelTriggersEl->at(im),"HLT_Ele105_CaloIdVT_GsfTrkIdT_v")) {
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
    teve->weight *= (PDATA/PMC);*/
    //std::cout<<"BTag weight = "<<PDATA/PMC<<std::endl;
    //if (mindR <= 0.4) lepjetoverlap = true;
    if (mindR <= 0.4 && ptrel < 50 && teve->suffix == "Mu") continue;
    if (mindR <= 0.4 && ptrel < 60 && teve->suffix == "El") continue;
    bool firstbtag = false;
    bool first2btag = false;
    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 || teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) firstbtag = true;
    if (teve->vGoodAK4Jets.at(0)->csvTag[0] > 0 && teve->vGoodAK4Jets.at(1)->csvTag[0] > 0) first2btag = true;
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE || teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) firstbtag = true;
    //if (teve->vGoodAK4Jets.at(0)->csvDisc > BTAGCUTLOOSE && teve->vGoodAK4Jets.at(1)->csvDisc > BTAGCUTLOOSE) first2btag = true;


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
    
    if (sampleIndex == Data and teve->lvWprime.M()>3000.){
	cout<<"-----------------------------------------------------"<<endl;
	cout<<"Found Wprime mass reco event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
	teve->printEvent();
	cout<<"-----------------------------------------------------"<<endl;
    }

    bool finalcuts = false;
    if (teve->suffix == "Mu" && (teve->lvTop.Pt() > 250 && teve->lvTop.M() > 100 && teve->lvTop.M() < 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;
    if (teve->suffix == "El" && (teve->lvTop.Pt() > 250 && ((teve->vGoodAK4Jets.at(0)->lv)+(teve->vGoodAK4Jets.at(1)->lv)).Pt() > 350)) finalcuts = true;

    fillHistosTwoChannels(GE0BTags_std_h, teve);
    if (finalcuts) fillHistosTwoChannels(GE0BTags_Final_std_h, teve);
    if ( !firstbtag) {
      fillHistosTwoChannels(Ex0BTags_std_h, teve);
      if (finalcuts) fillHistosTwoChannels(Ex0BTags_Final_std_h, teve);
    }
    if ( firstbtag && !first2btag) {
      fillHistosTwoChannels(Ex1BTags_std_h, teve);
      if (finalcuts) {
        fillHistosTwoChannels(Ex1BTags_Final_std_h, teve);
      }
    }
    if ( firstbtag ) {
      fillHistosTwoChannels(GE1BTags_std_h, teve);
      if (finalcuts) {
        fillHistosTwoChannels(GE1BTags_Final_std_h, teve);
        if (teve->lepton1->charge>0.) fillHistosTwoChannels(GE1BTags_Final_Pos_std_h, teve);
        else fillHistosTwoChannels(GE1BTags_Final_Neg_std_h, teve);
      }
    }
    if ( first2btag ) {
      fillHistosTwoChannels(Ex2BTags_std_h, teve);
      if (finalcuts) {
        fillHistosTwoChannels(Ex2BTags_Final_std_h, teve);
      }
    }
	
  }//End loop over entries
  
  if (not teve->realData){
    scaleHistosTwoChannels(Ex0BTags_std_h);
    scaleHistosTwoChannels(Ex1BTags_std_h);
    scaleHistosTwoChannels(Ex2BTags_std_h);
    scaleHistosTwoChannels(GE1BTags_std_h);
    scaleHistosTwoChannels(GE0BTags_std_h);

    scaleHistosTwoChannels(Ex0BTags_Final_std_h);
    scaleHistosTwoChannels(Ex1BTags_Final_std_h);
    scaleHistosTwoChannels(Ex2BTags_Final_std_h);
    scaleHistosTwoChannels(GE1BTags_Final_std_h);
    scaleHistosTwoChannels(GE0BTags_Final_std_h);
  }

  cout<<"Number of duplicate events: "<<nDup<<endl;
  
  outFile->Write();
//   syncFile->Write();
  return 0;
}


#endif
