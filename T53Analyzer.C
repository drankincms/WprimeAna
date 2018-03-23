#ifndef T53Analyzer_cxx
#define T53Analyzer_cxx

#include "T53LoadData2012.C"
#include "T53StandardHistos.C"
#include <iomanip>
#include <set>
#include "Objects/interface/HcalLaserEventFilter2012.h"
#include "Objects/interface/LumiReWeighting.h"
#include "Objects/interface/TableReader.h"

// Lepton ID and Iso
TableReader electronTightSF2012("electronTightSF2012.txt");
TableReader muTightSF2012ABCD("muTightSF2012ABCD.txt");

float getLeptonSF (TLepton * lepton, int run)
{
  if (lepton->isMuon()) return muTightSF2012ABCD  .Val(lepton->pt, fabs(lepton->eta));
  else                  return electronTightSF2012.Val(lepton->pt, fabs(lepton->eta));
}

//New 2012 values with electron dxy < 0.04 rather than 0.02
double misIDprob[15] = {0.00231008,  0.00233089,  0.00217188,  0.00101665,  0.00061021, 
			0.000385422, 0.000191425, 0.000133947, 0.000164017, 0.000294469, 
			0.000492558, 0.000906167, 0.00183267,  0.00214132,  0.00204466
};

// double misIDprob[15] = {0.0016076,   0.00149662,  0.00158111,  0.000685342, 0.000416127,
// 			0.000326186, 0.000127859, 0.000118712, 0.000117695, 0.000205299,
// 			0.000368622, 0.000684842, 0.00134407,  0.00135367,  0.00143893
// };


double getChargeMisIdProb(double eta);

double getSubstructureWeight(int nWJets, int nTopJets, int nCon, int nMinCon);

struct SortByDW {
  bool operator()(const TLorentzVector v1, const TLorentzVector v2) {
    double dw1 = fabs(v1.M() - MW);
    double dw2 = fabs(v2.M() - MW);
    return (dw1 < dw2);
  }
};


int main( int argc, const char* argv[] ){

  int MIN_NBTAG =-1;
//   int MIN_HT =0;

  double WJSF = 0.944;
  double TJSF = 0.926;
 
  if (argc < 4) {
    cout<<"Need 3 arguments:\n";
    cout<<"- dilepton channel (MuMu, MuEl, ElEl)\n";
    cout<<"- sample"<<endl;
    cout<<"- root file name"<<endl;
    return 0;
  }
//   HcalLaserEventFilter2012 hcal("./AllBadHCALLaser.txt.gz");
 
  TString inSuffix(argv[1]);
  TString sampleName(argv[2]);
  TString fileName(argv[3]);
  
  cout<<"Processing "<<sampleName<<'\t'<<fileName<<endl;
  
  WprimeEvent * teve = new WprimeEvent();
  initAll_2012(fileName, sampleName);

  string signStr = "SS";
  if (argc > 4) signStr = argv[4];

  int sign = 1;
  if (signStr == "OS")            sign = -1;
  if (sampleIndex == ChargeMisID) sign *= -1;

  bool b_DoFakes  = (sampleIndex == FakeRate);
  bool b_RealData = (sampleIndex == Data or sampleIndex == ChargeMisID or sampleIndex == FakeRate);
  
  cout << "Init LumiReWeighting"<<endl;
  
  std::string generatedFile("pu_mc.root");
  std::string dataFile("DataPileupHistogram_2012_ABCD.root");
  std::string genHistName("PUS10_Distr");
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
  
  if (b_RealData) sprintf(name,"anOutFile_%s_%s_%s_%lx.root", argv[2], argv[1], signStr.c_str(), rawtime);
  else            sprintf(name,"anOutFile_%s_%s_%lx.root",    argv[2],          signStr.c_str(), rawtime);
//   else            sprintf(name,"anOutFile_%s_%s.root",    argv[2],          signStr.c_str());

  TFile* outFile = new TFile(name, "RECREATE");

  unsigned int nCutSteps = 6;

  TH1F* h_CutFlow;
  TH2F* h_ElIso_vs_nPV;
  TH2F* h_MuIso_vs_nPV;
  TH2F* h_HT_vs_MET_PreSel;
  TH2F* h_HT_vs_MET_PostSel;

  vector <StandardHistos*> twoSSLep_std_h(3);
  vector <StandardHistos*> twoSSLepZQV_std_h(3);

  vector <StandardHistos*> twoSSLepZQV2J_std_h(3);
  vector <StandardHistos*> twoSSLepZQV5JL_std_h(3);
  vector <StandardHistos*> newCuts_std_h(3);

  vector <StandardHistos*> ak5TwoSSLepZQV2J_std_h(3);
  vector <StandardHistos*> ak5TwoSSLepZQV5JL_std_h(3);
  vector <StandardHistos*> ak5Cuts_std_h(3);

  vector <StandardHistos*> oldJetOnlyCuts_std_h(3);

  vector <StandardHistos*> massReco_std_h(3);
  vector <StandardHistos*> massRecoHE_std_h(3);

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  h_CutFlow = new TH1F("h_CutFlow_"+sampleName, "h_CutFlow_"+sampleName, nCutSteps, -0.5, nCutSteps - 0.5);
  h_CutFlow->Sumw2();

  h_ElIso_vs_nPV = new TH2F("h_ElIso_vs_nPV_"+sampleName, "h_ElIso_vs_nPV_"+sampleName, 100,  0.,  0.15, 50, -0.5,  49.5);
  h_MuIso_vs_nPV = new TH2F("h_MuIso_vs_nPV_"+sampleName, "h_MuIso_vs_nPV_"+sampleName, 100,  0.,  0.20, 50, -0.5,  49.5);

  h_HT_vs_MET_PreSel  = new TH2F("h_HT_vs_MET_PreSel_"+sampleName,  "h_HT_vs_MET_PreSel_"+sampleName,  100,  0.,  2000., 100, 0.,  1000.);
  h_HT_vs_MET_PostSel = new TH2F("h_HT_vs_MET_PostSel_"+sampleName, "h_HT_vs_MET_PostSel_"+sampleName, 100,  0.,  2000., 100, 0.,  1000.);
  
  //Make directories and initialize histograms
  initHistoDirThreeChannels(outFile, twoSSLep_std_h,          "SS_2L",            sampleName, sScale);
  initHistoDirThreeChannels(outFile, twoSSLepZQV_std_h,       "SS_2L_ZQV",        sampleName, sScale);

  initHistoDirThreeChannels(outFile, twoSSLepZQV2J_std_h,     "SS_2L_ZQV_2J",     sampleName, sScale);
  initHistoDirThreeChannels(outFile, twoSSLepZQV5JL_std_h,    "SS_2L_ZQV_5JL",    sampleName, sScale);
  initHistoDirThreeChannels(outFile, newCuts_std_h,           "NewCuts",          sampleName, sScale);
  
  initHistoDirThreeChannels(outFile, ak5TwoSSLepZQV2J_std_h,  "SS_2L_ZQV_2AK5J",  sampleName, sScale);
  initHistoDirThreeChannels(outFile, ak5TwoSSLepZQV5JL_std_h, "SS_2L_ZQV_5AK5JL", sampleName, sScale);
  initHistoDirThreeChannels(outFile, ak5Cuts_std_h,           "AK5Cuts",          sampleName, sScale);

  initHistoDirThreeChannels(outFile, oldJetOnlyCuts_std_h,    "OldJetOnlyCuts",   sampleName, sScale);
  
  initHistoDirThreeChannels(outFile, massReco_std_h,          "MassReco",         sampleName, sScale);
  initHistoDirThreeChannels(outFile, massRecoHE_std_h,        "MassRecoHE",       sampleName, sScale);
  
  outFile->cd();

  cout << "Looping over "<<sampleName<<" "<< treeClass->getEntries()<<endl;

  set <long long> checkDuplicates;
  int nDup = 0;
  long long RUNPREF = 1000 * 1000; 
  RUNPREF *= 1000 * 1000;

  //Skip the loop if the sample is MuMu charge misID
  if (not (sampleIndex == ChargeMisID and inSuffix == "MuMu")){

  for (Long64_t il = 0; il < treeClass->getEntries(); il++){
//     for (Long64_t il = 0; il < 200000; il++){
    
//     if (il % 100000 == 0) cout<<"At event "<<il<<endl;

    treeClass->GetEntry(il);
//     hcal.filter(treeClass->run,treeClass->lumi,treeClass->event);
    
    //Initialize WprimeEvent class
    teve->clearEvent();
    teve->isam     = sampleIndex;
    teve->realData = b_RealData;
    teve->nPV      = treeClass->nPV;
    
    /// Review PU:
    int nti = (int) treeClass->nTrueInteractions_PileUpCalc;
    if (teve->realData) teve->weight = 1.0;
    else                teve->weight = lumiReWeighting.weight(nti);
//     else                teve->weight = treeClass->weight_PU;
    
    teve->PUweight = teve->weight;

    //Tree-level cuts only (2 leptons > 20 GeV, 2 jets > 30 GeV)
    h_CutFlow->Fill(0.0, teve->weight);

    //Lepton cuts first    
    bool b_LepCuts = false;
    
    TString eventType = "";
        
    //Select only events which pass the trigger for the channel we are looking at and avoid overlaps for data
    bool b_SelTrigMM = ((not teve->realData or treeClass->dataMM > 0) and treeClass->trigMM > 0);
    bool b_SelTrigEM = ((not teve->realData or treeClass->dataEM > 0) and treeClass->trigEM > 0);
    bool b_SelTrigEE = ((not teve->realData or treeClass->dataEE > 0) and treeClass->trigEE > 0);
    
    //First, look for two good leptons with proper sign correlation
    //goodLeptons are already pt-sorted and cleaned of overlaps
    if (treeClass->goodLeptons.size() >= 2){
      bool b_Found = false;
      for (unsigned int ui = 0; ui < treeClass->goodLeptons.size(); ui++){
	for (unsigned int uj = ui + 1; uj < treeClass->goodLeptons.size(); uj++){
	  if (treeClass->goodLeptons.at(ui)->charge * treeClass->goodLeptons.at(uj)->charge != sign) continue;
	  if (//MuMu
	      (treeClass->goodLeptons.at(ui)->isMuon() and treeClass->goodLeptons.at(uj)->isMuon() and b_SelTrigMM) or
	      //ElMu
	      (((treeClass->goodLeptons.at(ui)->isElectron() and treeClass->goodLeptons.at(uj)->isMuon()) or
		(treeClass->goodLeptons.at(uj)->isElectron() and treeClass->goodLeptons.at(ui)->isMuon())) and b_SelTrigEM) or 
	      //ElEl
	      (treeClass->goodLeptons.at(ui)->isElectron() and treeClass->goodLeptons.at(uj)->isElectron() and b_SelTrigEE)){  
	    teve->lepton1 = treeClass->goodLeptons.at(ui);
	    teve->lepton2 = treeClass->goodLeptons.at(uj);	    
	    if (b_DoFakes) eventType = "Nt11";	    
	    b_LepCuts = true;
	    b_Found   = true;
	    break;
	  }
	}
	if (b_Found) break;
      }
    }//End of search for two good leptons with proper sign correlation

    //Search for fake leptons if no good ones found and fakes were asked for
    //General idea: start with a vector with all good and "loose-but-not-tight" leptons
    //sorted by pt just like goodLeptons. Then, take the first two leptons compatible with the
    //sign, selection and trigger requirements and assign the event type
    if (not b_LepCuts and b_DoFakes and treeClass->leptonsForFakes.size() >= 2){
      bool b_Found = false;
      for (unsigned int ui = 0; ui < treeClass->leptonsForFakes.size(); ui++){
	for (unsigned int uj = ui + 1; uj < treeClass->leptonsForFakes.size(); uj++){
	  if (treeClass->leptonsForFakes.at(ui)->charge * treeClass->leptonsForFakes.at(uj)->charge != sign) continue;

	  //MuMu or ElEl
	  if ((treeClass->leptonsForFakes.at(ui)->isMuon()     and treeClass->leptonsForFakes.at(uj)->isMuon()     and b_SelTrigMM) or
	      (treeClass->leptonsForFakes.at(ui)->isElectron() and treeClass->leptonsForFakes.at(uj)->isElectron() and b_SelTrigEE)){
	    b_LepCuts = true;
	    if      ((treeClass->leptonsForFakes.at(ui)->goodLepton() and treeClass->leptonsForFakes.at(uj)->lntLepton()) or
		     (treeClass->leptonsForFakes.at(uj)->goodLepton() and treeClass->leptonsForFakes.at(ui)->lntLepton()))    eventType = "Nt01";
	    else if (treeClass->leptonsForFakes.at(ui)->lntLepton()   and treeClass->leptonsForFakes.at(uj)->lntLepton())     eventType = "Nt00";
	    else {
	      cout<<"Invalid fakes eventType!"<<endl;
	      return 1;
	    }
	  }
	  //ElMu is special because it matters which lepton is loose-but-not-tight and which is tight
	  //Nt01 = Muon is tight, Electron is loose-but-not-tight;
	  //Nt10 = Muon is loose-but-not-tight, Electron is tight;
	  if (treeClass->leptonsForFakes.at(ui)->isElectron() and treeClass->leptonsForFakes.at(uj)->isMuon() and b_SelTrigEM){
	    b_LepCuts = true;
	    if      (treeClass->leptonsForFakes.at(ui)->goodLepton() and treeClass->leptonsForFakes.at(uj)->lntLepton()) eventType = "Nt10";
	    else if (treeClass->leptonsForFakes.at(uj)->goodLepton() and treeClass->leptonsForFakes.at(ui)->lntLepton()) eventType = "Nt01";
	    else if (treeClass->leptonsForFakes.at(ui)->lntLepton()  and treeClass->leptonsForFakes.at(uj)->lntLepton()) eventType = "Nt00";
	    else {
	      cout<<"Invalid fakes eventType!"<<endl;
	      return 1;
	    }	    
	  }
	  if (treeClass->leptonsForFakes.at(ui)->isMuon() and treeClass->leptonsForFakes.at(uj)->isElectron() and b_SelTrigEM){
	    b_LepCuts = true;
	    if      (treeClass->leptonsForFakes.at(ui)->goodLepton() and treeClass->leptonsForFakes.at(uj)->lntLepton()) eventType = "Nt01";
	    else if (treeClass->leptonsForFakes.at(uj)->goodLepton() and treeClass->leptonsForFakes.at(ui)->lntLepton()) eventType = "Nt10";
	    else if (treeClass->leptonsForFakes.at(ui)->lntLepton()  and treeClass->leptonsForFakes.at(uj)->lntLepton()) eventType = "Nt00";
	    else {
	      cout<<"Invalid fakes eventType!"<<endl;
	      return 1;
	    }	    
	  }
	
	  if (b_LepCuts){
	    teve->lepton1 = treeClass->leptonsForFakes.at(ui);
	    teve->lepton2 = treeClass->leptonsForFakes.at(uj);	    
	    b_Found = true;
	    break;
	  }	    
	}
	if (b_Found) break;
      }      
    }//End of search for two good or "loose-but-not-tight" leptons with proper sign correlation

    //Go to the next event if no leptons were found
    if (not b_LepCuts) continue;

    //Check for duplicates
    //The CPU cost of this grows as nLog(n) with the number of 
    //events so do this as late as possible
//     if (teve->realData or sampleIndex == WWp){
      long long dupCheck = treeClass->run * RUNPREF + treeClass->event;
      if (checkDuplicates.find(dupCheck) != checkDuplicates.end()){
	nDup++;
	continue;
      }
      else checkDuplicates.insert(dupCheck);
//     }
    //End of check for duplicates

    //Determine channel
    if (    teve->lepton1->isMuon() and teve->lepton2->isMuon())           teve->suffix = "MuMu";
    else if ((teve->lepton1->isMuon() and teve->lepton2->isElectron()) or
	     (teve->lepton2->isMuon() and teve->lepton1->isElectron()))    teve->suffix = "ElMu";
    else if (teve->lepton1->isElectron() and teve->lepton2->isElectron())  teve->suffix = "ElEl";
    else{
      cout<<"Invalid channel!"<<endl;
      return 1;
    }

    // Trigger SF
    if (not teve->realData){
      if (teve->suffix == "MuMu") teve->weight *= 0.965;
      if (teve->suffix == "ElMu") teve->weight *= 0.943;
      if (teve->suffix == "ElEl") teve->weight *= 0.962;
    }

    teve->leptonSum  = teve->lepton1->lv + teve->lepton2->lv;
    teve->theLeptons = treeClass->goodLeptons;

    //Extend the lepton collection with selected loose leptons
    if (b_DoFakes){
      if (not teve->lepton1->goodLepton()) teve->theLeptons.push_back(teve->lepton1);
      if (not teve->lepton2->goodLepton()) teve->theLeptons.push_back(teve->lepton2);
    }
 
    //Get rid of events with 3 leptons where two of them come from a Z
    //Also, count the number of other leptons with a different charge
    bool b_Z3L  = false;
    int  nOpLep = 0;

    vector <TLepton*> leptonsForZCheck;
    leptonsForZCheck.insert(leptonsForZCheck.end(), treeClass->allElecs.begin(), treeClass->allElecs.end());
    leptonsForZCheck.insert(leptonsForZCheck.end(), treeClass->allMuons.begin(), treeClass->allMuons.end());
  
    for (unsigned int il = 0; il < leptonsForZCheck.size(); il++){
      if (not leptonsForZCheck.at(il)->looseElectron(15) and not leptonsForZCheck.at(il)->looseMuon(15)) continue;
      //Must be a different lepton
      if (leptonsForZCheck.at(il)->lv.DeltaR(teve->lepton1->lv) > 0.1 and
	  leptonsForZCheck.at(il)->lv.DeltaR(teve->lepton2->lv) > 0.1){
	double mz = 0;
	if (teve->lepton1->charge *  leptonsForZCheck.at(il)->charge == -1 and
	    ((teve->lepton1->isMuon()     and leptonsForZCheck.at(il)->isMuon()) or
	     (teve->lepton1->isElectron() and leptonsForZCheck.at(il)->isElectron()))){
	  
	  mz = (teve->lepton1->lv + leptonsForZCheck.at(il)->lv).M();
	  if (mz < 106 and mz > 76) b_Z3L = true;
	}
	if (teve->lepton2->charge *  leptonsForZCheck.at(il)->charge == -1 and
	    ((teve->lepton2->isMuon()     and leptonsForZCheck.at(il)->isMuon()) or
	     (teve->lepton2->isElectron() and leptonsForZCheck.at(il)->isElectron()))){
	      
	  mz = (teve->lepton2->lv + leptonsForZCheck.at(il)->lv).M();
	  if (mz < 106 and mz > 76) b_Z3L = true;
	}
	if (leptonsForZCheck.at(il)->goodLepton() and //Don't want fakes or worse here
	    //They have the same sign, but this is necessary for the charge misID case
	    (teve->lepton1->charge * leptonsForZCheck.at(il)->charge == -1 * sign or
	     teve->lepton2->charge * leptonsForZCheck.at(il)->charge == -1 * sign)) nOpLep++;  
      }
    }
    //End trilepton Z cleaning
   
    if (not teve->realData) teve->weight *= getLeptonSF(teve->lepton1, treeClass->run) *
			                    getLeptonSF(teve->lepton2, treeClass->run);   
    
    if (sampleIndex == ChargeMisID){
      teve->weight = 0;
      if (not teve->lepton1->isMuon()) teve->weight += getChargeMisIdProb(teve->lepton1->lv.Eta());
      if (not teve->lepton2->isMuon()) teve->weight += getChargeMisIdProb(teve->lepton2->lv.Eta());
    }
        
    //Jet Cuts
    teve->vGoodCATopJets   = treeClass->goodCATopJets(teve->theLeptons);
    teve->vGoodCAWJets     = treeClass->goodCAWJets(teve->theLeptons, teve->vGoodCATopJets);
    teve->vGoodCA8Jets     = treeClass->goodCA8Jets(teve->theLeptons, teve->vGoodCATopJets, teve->vGoodCAWJets);
    teve->vGoodAK5Jets     = treeClass->goodAK5Jets(teve->theLeptons, teve->vGoodCATopJets, teve->vGoodCAWJets);
    //Only AK5 for cross-check
    teve->vOnlyGoodAK5Jets = treeClass->goodJets(teve->theLeptons);

    //Add missing "cleaned" leptons
     treeClass->addCleanedLeptons(teve->theLeptons, treeClass->allCleanedLeptons, 
				  teve->vGoodCATopJets, teve->vGoodCAWJets, teve->vGoodAK5Jets);
     
     treeClass->addCleanedLeptons(teve->theLeptons, treeClass->allCleanedLeptons, teve->vOnlyGoodAK5Jets);

    //Get rid of events with leptons inside CATop jets
    bool leptonInTopJet = false;
    for (unsigned int ij = 0; ij < teve->vGoodCATopJets.size(); ij++){ 
      if (teve->vGoodCATopJets.at(ij)->lv.DeltaR(teve->lepton1->lv) < 0.8 or
	  teve->vGoodCATopJets.at(ij)->lv.DeltaR(teve->lepton2->lv) < 0.8){
	leptonInTopJet = true;
	break;
      }
    }
    if (leptonInTopJet) continue; 


    //Cross-check for Freya
    teve->vAllCAWJets   = treeClass->allCAWJets;
    teve->vAllCATopJets = treeClass->allCATopJets;

    teve->met = treeClass->PF_met_pt;
    for (unsigned int ij = 0; ij < teve->vGoodCATopJets.size(); ij++){ 
      teve->ht        += teve->vGoodCATopJets.at(ij)->lv.Pt(); 
      teve->htJetOnly += teve->vGoodCATopJets.at(ij)->lv.Pt(); 
      teve->nBTags++;
    }
    for (unsigned int ij = 0; ij < teve->vGoodCAWJets.size(); ij++){ 
      teve->ht        += teve->vGoodCAWJets.at(ij)->lv.Pt(); 
      teve->htJetOnly += teve->vGoodCAWJets.at(ij)->lv.Pt(); 
    }
//     for (unsigned int ij = 0; ij < teve->vGoodCA8Jets.size(); ij++){ teve->ht += teve->vGoodCA8Jets.at(ij)->lv.Pt(); }
    for (unsigned int ij = 0; ij < teve->vGoodAK5Jets.size(); ij++){
      teve->ht        += teve->vGoodAK5Jets.at(ij)->lv.Pt();
      teve->htJetOnly += teve->vGoodAK5Jets.at(ij)->lv.Pt();
      if (teve->vGoodAK5Jets.at(ij)->csvMedium != 0) teve->nBTags++; //Medium
    }
    for (unsigned int ij = 0; ij < teve->theLeptons.size(); ij++){ 
      teve->ht    += teve->theLeptons.at(ij)->lv.Pt(); 
      teve->htAK5 += teve->theLeptons.at(ij)->lv.Pt(); 
    }
    for (unsigned int ij = 0; ij < teve->vOnlyGoodAK5Jets.size(); ij++){ teve->htAK5 += teve->vOnlyGoodAK5Jets.at(ij)->lv.Pt(); }

    bool b_PassZVeto  = true;
    bool b_MuoniaVeto = true;
    
    //Muonia and charge misID Z-veto (latter is for electrons only)
    if (teve->leptonSum.M() < 20)                                                          b_MuoniaVeto = false;
    if (teve->suffix == "ElEl" and teve->leptonSum.M() < 106 and teve->leptonSum.M() > 76) b_PassZVeto  = false;
   
    //Found good same-sign leptons
    fillHistosThreeChannels(twoSSLep_std_h, teve, eventType, b_DoFakes);

    if (b_Z3L) continue;

    int nJets = 3*teve->vGoodCATopJets.size() + 2*teve->vGoodCAWJets.size() + teve->vGoodAK5Jets.size();

    //CutFlow for Thomas
    if (treeClass->goodLeptons.size() == 2){
      h_CutFlow->Fill(1.0, teve->weight);
      if (nJets >= 2) h_CutFlow->Fill(2.0, teve->weight);
      if (nJets >= 3 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(3.0, teve->weight);
      if (nJets >= 4 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(4.0, teve->weight);
      if (nJets >= 5 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(5.0, teve->weight);
    }
    
    if (b_PassZVeto and b_MuoniaVeto and teve->nBTags > MIN_NBTAG){
      fillHistosThreeChannels(twoSSLepZQV_std_h, teve, eventType, b_DoFakes);

      if (teve->lepton1->isMuon()) h_MuIso_vs_nPV->Fill(teve->lepton1->relIso, teve->nPV);
      else                         h_ElIso_vs_nPV->Fill(teve->lepton1->relIso, teve->nPV);

      if (teve->lepton2->isMuon()) h_MuIso_vs_nPV->Fill(teve->lepton2->relIso, teve->nPV);
      else                         h_ElIso_vs_nPV->Fill(teve->lepton2->relIso, teve->nPV);

      h_HT_vs_MET_PreSel->Fill(teve->ht, teve->met);

      //---------------Begin AK5 jets only------------------
      if (teve->vOnlyGoodAK5Jets.size() >= 2) fillHistosThreeChannels(ak5TwoSSLepZQV2J_std_h, teve, eventType, b_DoFakes);
      if (teve->vOnlyGoodAK5Jets.size() + teve->theLeptons.size() - 2 >= 5){
  	fillHistosThreeChannels(ak5TwoSSLepZQV5JL_std_h, teve, eventType, b_DoFakes);
 	if (teve->htAK5 > 900) fillHistosThreeChannels(ak5Cuts_std_h, teve, eventType, b_DoFakes);
      }
      //----------------End AK5 jets only-------------------
      

      // if (not teve->realData){
      // 	//Scale factor for W and top jets
      // 	int nJoL = nJets + teve->theLeptons.size() - 2;
	
      // }

      double tmpWeight = 1;
      if (nJets >= 2){
	if (not teve->realData){
	  tmpWeight = getSubstructureWeight(teve->vGoodCAWJets.size(), teve->vGoodCATopJets.size(), nJets, 2);
	  teve->weight *= tmpWeight;
	  fillHistosThreeChannels(twoSSLepZQV2J_std_h, teve, eventType, b_DoFakes);
	  teve->weight /= tmpWeight;
	}
	else fillHistosThreeChannels(twoSSLepZQV2J_std_h, teve, eventType, b_DoFakes);
      }  
      if (nJets >= 4 and teve->htJetOnly > 700){
	if (not teve->realData){
	  tmpWeight = getSubstructureWeight(teve->vGoodCAWJets.size(), teve->vGoodCATopJets.size(), nJets, 4);
	  teve->weight *= tmpWeight;
	  fillHistosThreeChannels(oldJetOnlyCuts_std_h, teve, eventType, b_DoFakes);
	  teve->weight /= tmpWeight;
	}
	else fillHistosThreeChannels(oldJetOnlyCuts_std_h, teve, eventType, b_DoFakes);
      } 
      if (nJets + teve->theLeptons.size() - 2 >= 5){
	tmpWeight = getSubstructureWeight(teve->vGoodCAWJets.size(), teve->vGoodCATopJets.size(), nJets + teve->theLeptons.size() - 2, 5);
	if (not teve->realData) teve->weight *= tmpWeight;

	fillHistosThreeChannels(twoSSLepZQV5JL_std_h, teve, eventType, b_DoFakes);
	
	if (teve->ht > 900){
	  fillHistosThreeChannels(newCuts_std_h, teve, eventType, b_DoFakes);
	  h_HT_vs_MET_PostSel->Fill(teve->ht, teve->met);
	  if (sampleIndex == Data){
	    cout<<"-----------------------------------------------------"<<endl;
	    cout<<"Found T53 data event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
	    teve->printEvent();
	    cout<<"-----------------------------------------------------"<<endl;
	  }
	}
	if (not teve->realData) teve->weight /= tmpWeight;
      }

      //Try to reconstruct the T53 -- need a top and a W
      bool foundT53Top = false;
      bool foundT53W   = false;

      //If there are multiple CATopJets, pick the best one based on how close it is to the top mass
      double bestMass = 1e8;
      for (unsigned int uij = 0; uij < teve->vGoodCATopJets.size(); uij++){
	if (fabs(bestMass - MTOP) > fabs(teve->vGoodCATopJets.at(uij)->lv.M() - MTOP)){
	  bestMass    = teve->vGoodCATopJets.at(uij)->lv.M();
	  teve->lvTop = teve->vGoodCATopJets.at(uij)->lv;
	  foundT53Top = true;
	}
      }

      if (foundT53Top){
	if (not teve->realData) teve->weight *= TJSF; //Top jet scale factor
	//Look for one W or two AK5 jets that make a W
	bestMass = 1e8;
	for (unsigned int uij = 0; uij < teve->vGoodCAWJets.size(); uij++){
	  if (fabs(bestMass - MW) > fabs(teve->vGoodCAWJets.at(uij)->lv.M() - MW)){
	    bestMass  = teve->vGoodCAWJets.at(uij)->lv.M();
	    teve->lvW = teve->vGoodCAWJets.at(uij)->lv;
	    foundT53W = true;
	  }
	}
	if (foundT53W and not teve->realData) teve->weight *= WJSF; //WJet scale factor
	
	if (not foundT53W){ //No merged W -- try to find one among the AK5 jets
	  bestMass = 1e8;
	  for (unsigned int uij = 0; uij < teve->vGoodAK5Jets.size(); uij++){
	    for (unsigned int ujj = uij + 1; ujj < teve->vGoodAK5Jets.size(); ujj++){
	      TLorentzVector lvtmp = teve->vGoodAK5Jets.at(uij)->lv + teve->vGoodAK5Jets.at(ujj)->lv;
	      if (fabs(lvtmp.M() - MW) < DMW and fabs(bestMass - MW) > fabs(lvtmp.M() - MW)){
		bestMass  = lvtmp.M();
		teve->lvW = lvtmp;
		foundT53W = true;
	      }
	    }//End second loop over AK5 jets
	  }//End first loop over AK5 jets
	}//End not foundT53W if
      }//End existance of a CATopJet if
      else{
	vector <TLorentzVector> vWs;
	for (unsigned int uij = 0; uij < teve->vGoodCAWJets.size(); uij++){
	  vWs.push_back(teve->vGoodCAWJets.at(uij)->lv);
	}

	vector <int> iUsedAK5;
	//If there are no CAW W's, must make both from AK5 jets
	if (vWs.size() == 0){
	  //Do it the most straightforward way first; re-do later
	  bool foundW1 = false;
	  bool foundW2 = false;
	  for (unsigned int uij = 0; uij < teve->vGoodAK5Jets.size(); uij++){
	    bool bUsed = false;
	    for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
	      if (iUsedAK5.at(iused) == (int) uij) bUsed = true;
	    }
	    if (bUsed) continue;
	    for (unsigned int ujj = uij + 1; ujj < teve->vGoodAK5Jets.size(); ujj++){
	      for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
		if (iUsedAK5.at(iused) == (int) ujj) bUsed = true;
	      }
	      if (bUsed) continue;
	      TLorentzVector lvtmp = teve->vGoodAK5Jets.at(uij)->lv + teve->vGoodAK5Jets.at(ujj)->lv;
	      if (fabs(lvtmp.M() - MW) < DMW){
		iUsedAK5.push_back(uij);	 
		iUsedAK5.push_back(ujj);	 
		vWs.push_back(lvtmp);
		if (not foundW1) foundW1 = true;
		else             foundW2 = true;
		break;
	      }
	    }//End second loop over AK5 jets
	    if (foundW1 and foundW2) break;
	  }//End first loop over AK5 jets
	}
	//If there is only one CAW W, make the other W from AK5 jets
	else if (vWs.size() == 1){
	  if (not teve->realData) teve->weight *= WJSF;
	  bestMass = 1e8;
	  int ak5wj1 = -1;
	  int ak5wj2 = -1;	  
	  for (unsigned int uij = 0; uij < teve->vGoodAK5Jets.size(); uij++){
	    for (unsigned int ujj = uij + 1; ujj < teve->vGoodAK5Jets.size(); ujj++){
	      TLorentzVector lvtmp = teve->vGoodAK5Jets.at(uij)->lv + teve->vGoodAK5Jets.at(ujj)->lv;
	      if (fabs(lvtmp.M() - MW) < DMW and fabs(bestMass - MW) > fabs(lvtmp.M() - MW)){
		bestMass  = lvtmp.M();
		ak5wj1 = (int) uij;
		ak5wj2 = (int) ujj;
	      }
	    }//End second loop over AK5 jets
	  }//End first loop over AK5 jets	  
	  if (ak5wj1 != -1 and ak5wj2 != -1){
	    vWs.push_back(teve->vGoodAK5Jets.at(ak5wj1)->lv + teve->vGoodAK5Jets.at(ak5wj2)->lv);
	    iUsedAK5.push_back(ak5wj1);
	    iUsedAK5.push_back(ak5wj2);
	  }
	}	

	sort(vWs.begin(), vWs.end(), SortByDW());

	if (vWs.size() > 1){
	  if (not teve->realData) teve->weight *= WJSF*WJSF;
	  //Two W's found (if there are more, use the two closest to M(W)
	  //Need to use one to make a top using AK5 jets
	  bestMass = 1e8;
	  for (unsigned int uij = 0; uij < teve->vGoodAK5Jets.size(); uij++){
	    bool bUsed = false;
	    for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
	      if (iUsedAK5.at(iused) == (int) uij) bUsed = true;
	    }
	    if (bUsed) continue;

	    TLorentzVector lvtmp = vWs.at(0) + teve->vGoodAK5Jets.at(uij)->lv;
	    if (fabs(lvtmp.M() - MTOP) < DMTOP and fabs(bestMass - MTOP) > fabs(lvtmp.M() - MTOP)){
	      bestMass    = lvtmp.M();
	      teve->lvTop = lvtmp;
	      foundT53Top = true;
	      
	      teve->lvW   = vWs.at(1); //Set the T53W to be the other W
	      foundT53W   = true;
	    }
	  
	    //Once more, with the second W
	    lvtmp = vWs.at(1) + teve->vGoodAK5Jets.at(uij)->lv;
	    if (fabs(lvtmp.M() - MTOP) < DMTOP and fabs(bestMass - MTOP) > fabs(lvtmp.M() - MTOP)){
	      bestMass    = lvtmp.M();
	      teve->lvTop = lvtmp;
	      foundT53Top = true;
	      
	      teve->lvW   = vWs.at(0); //Set the T53W to be the other W
	      foundT53W   = true;
	    }	    
	  }//End loop over AK5 jets
	}//End more than one W if
      }//End no CATop else          
      
      if (foundT53Top and foundT53W){ //Done!
	teve->lvT53 = teve->lvTop + teve->lvW;
	fillHistosThreeChannels(massReco_std_h,   teve, eventType, b_DoFakes);
	fillHistosThreeChannels(massRecoHE_std_h, teve, eventType, b_DoFakes);

	if (sampleIndex == Data){
	  cout<<"-----------------------------------------------------"<<endl;
	  cout<<"Found T53 mass reco event: "<<treeClass->run<<" : "<<treeClass->lumi<<" : "<<treeClass->event<<endl;
	  teve->printEvent();
	  cout<<"-----------------------------------------------------"<<endl;
	}
	
      }
      else if (teve->ht > 600){
	//Stupidest T53 reconstruction possible: add the 5 highest pt jets
	teve->lvT53.SetPxPyPzE(0, 0, 0, 0);
	int nTotJets = 0;
	if (teve->vGoodCATopJets.size() > 0){
	  if (not teve->realData) teve->weight *= TJSF;
	  nTotJets += 3;
	  teve->lvT53 = teve->lvT53 + teve->vGoodCATopJets.at(0)->lv;
	}
	if (teve->vGoodCAWJets.size() > 0){
	  if (not teve->realData) teve->weight *= WJSF;
	  nTotJets += 2;
	  teve->lvT53 = teve->lvT53 + teve->vGoodCAWJets.at(0)->lv;
	}
	if (teve->vGoodCAWJets.size() > 1 and nTotJets < 3){
	  if (not teve->realData) teve->weight *= WJSF;
	  nTotJets += 2;
	  teve->lvT53 = teve->lvT53 + teve->vGoodCAWJets.at(1)->lv;	  
	}
	for (unsigned int uij = 0; uij < teve->vGoodAK5Jets.size(); uij++){
	  if (nTotJets > 4) break;
	  nTotJets++;
	  teve->lvT53 = teve->lvT53 + teve->vGoodAK5Jets.at(uij)->lv;	  	  
	}
	if (nTotJets > 4) fillHistosThreeChannels(massRecoHE_std_h, teve, eventType, b_DoFakes);
      }
    }//End pass Z/muonia veto if
  }//End loop over entries
  }//End skipping of loop for MuMu ChargeMisID
  
  if (b_DoFakes){
    getTotFakesThreeChannels(twoSSLep_std_h);
    getTotFakesThreeChannels(twoSSLepZQV_std_h);

    getTotFakesThreeChannels(twoSSLepZQV2J_std_h);
    getTotFakesThreeChannels(twoSSLepZQV5JL_std_h);
    getTotFakesThreeChannels(newCuts_std_h);

    getTotFakesThreeChannels(ak5Cuts_std_h);
    getTotFakesThreeChannels(ak5TwoSSLepZQV2J_std_h);
    getTotFakesThreeChannels(ak5TwoSSLepZQV5JL_std_h);

    getTotFakesThreeChannels(oldJetOnlyCuts_std_h);
    
    getTotFakesThreeChannels(massReco_std_h);
    getTotFakesThreeChannels(massRecoHE_std_h);
  } 
  if (not teve->realData){
    scaleHistosThreeChannels(twoSSLep_std_h,          b_DoFakes);
    scaleHistosThreeChannels(twoSSLepZQV_std_h,       b_DoFakes);

    scaleHistosThreeChannels(twoSSLepZQV2J_std_h,     b_DoFakes);
    scaleHistosThreeChannels(twoSSLepZQV5JL_std_h,    b_DoFakes);
    scaleHistosThreeChannels(newCuts_std_h,           b_DoFakes);

    scaleHistosThreeChannels(ak5TwoSSLepZQV2J_std_h,  b_DoFakes);
    scaleHistosThreeChannels(ak5TwoSSLepZQV5JL_std_h, b_DoFakes);
    scaleHistosThreeChannels(ak5Cuts_std_h,           b_DoFakes);

    scaleHistosThreeChannels(oldJetOnlyCuts_std_h,    b_DoFakes);
    
    scaleHistosThreeChannels(massReco_std_h,          b_DoFakes);
    scaleHistosThreeChannels(massRecoHE_std_h,        b_DoFakes);
  }

  cout<<"Number of duplicate events: "<<nDup<<endl;
  
  outFile->Write();
//   syncFile->Write();
  return 0;
}

  double getChargeMisIdProb(double eta){
  //Translation of eta to index: eta varies from -2.4 to 2.4 in intervals of 0.32
  int ib = (int) ((eta + 2.4) / 0.32);

  //Take care of rounding screwups
  if (ib > 14) ib = 14;
  if (ib < 0 ) ib = 0;

  return misIDprob[ib];
}

double getSubstructureWeight(int nWJets, int nTopJets, int nCon, int nMinCon){

  if (nCon < nMinCon){
    cerr<<"Fewer constituents than minimum. Returning weight 1, but this doesn't make sense."<<endl;
    return 1;
  }
  
  //The weight is NOT actually a probability. However, for the purpose of computing the effective
  //weight for multiple W/top jets, I treat it as such. It works out correctly in the one W jet and
  //one top jet cases and the impact of the others is practically irrelevant.
  double pTop = 0.926;
  double pW   = 0.944;

  //The minimum number of constituents. Counts each W and each top jet as one constituent
  int baseNCon = nCon - 2*nTopJets - nWJets;

  if (baseNCon >= nMinCon) return 1; 
  
  //The above covers the case of 0 and 1 jets.
  int minFromSubstr = nMinCon - baseNCon;

  double weight = 1;
  if      (minFromSubstr == 1) weight = 1 - pow(1 - pTop, nTopJets)*pow(1 - pW, nWJets);
  else if (minFromSubstr == 2) weight = 1 - pow(1 - pTop, nTopJets)*(pow(1 - pW, nWJets) + nWJets*pW*pow(1 - pW, nWJets - 1));
  else if (minFromSubstr == 3)
    weight = 1 - (pow(1 - pTop, nTopJets)*(pow(1 - pW, nWJets) + nWJets*pW*pow(1 - pW, nWJets - 1) 
					   + (nWJets*(nWJets - 1)/ 2)*pW*pW*pow(1 - pW, nWJets - 2)) +
		  pTop*nTopJets*pow(1 - pTop, nTopJets - 1)*pow(1 - pW, nWJets));
  else{
    cerr<<"minFromSubstr > 3. How is this possible? Asking for more than 5 jets? Returning 1";
    return 1;
  }

  return weight;
}


#endif
