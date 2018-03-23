#ifndef ClosureTestAnalyzer_cxx
#define ClosureTestAnalyzer_cxx

#include "T53LoadData2012.C"
#include "T53StandardHistos.C"
#include <iomanip>
#include "Objects/interface/HcalLaserEventFilter2012.h"
#include "Objects/interface/LumiReWeighting.h"
#include "Objects/interface/TableReader.h"

// Lepton ID

TableReader electronTightSF2012("electronTightSF2012.txt");
TableReader muTightSF2012AB("muTightSF2012AB.txt");
TableReader muTightSF2012C("muTightSF2012C.txt");

float getLeptonSF (TLepton * lepton, int run)
{
  if (lepton->isMuon()) {
    if (run < 198000) {
      return muTightSF2012AB.Val(lepton->pt, fabs(lepton->eta));
    } else {
      return muTightSF2012C.Val(lepton->pt, fabs(lepton->eta));
    }
  } else {
    return electronTightSF2012.Val(lepton->pt, fabs(lepton->eta));
  }
}

//This is from 2011! Replace it once we have the DoubleElectron dataset
double misIDprob[15] = {0.00130973,  0.00153928,  0.00154326,  0.000791874, 0.000406848,
			0.00029017,  0.000208527, 0.000161353, 0.000241185, 0.000291503,
			0.000408998, 0.000738134, 0.00144478,  0.00154651,  0.00137379
};

double getChargeMisIdProb(double eta);

int main( int argc, const char* argv[] ){

  int MIN_NBTAG =-1;
//   int MIN_HT =0;

 
  if (argc < 4) {
    cout<<"Need 3 arguments:\n";
    cout<<"- dilepton channel (MuMu, MuEl, ElEl)\n";
    cout<<"- sample"<<endl;
    cout<<"- root file name"<<endl;
    return 0;
  }
//   HcalLaserEventFilter2012 hcal("./AllBadHCALLaser.txt.gz");
 
  TString sampleName(argv[2]);
  TString fileName(argv[3]);
  
  cout<<"Processing "<<sampleName<<'\t'<<fileName<<endl;
  
  T53Event * teve = new T53Event();
  initAll_2012(fileName, sampleName);

  string signStr = "SS";
  if (argc > 4) signStr = argv[4];
  
  int sign = 1;
  if (signStr == "OS")            sign = -1;
  if (sampleIndex == ChargeMisID) sign *= -1;

  bool b_DoFakes  = (sampleIndex == FakeRate or sampleIndex == TTbar);
  bool b_RealData = (sampleIndex == Data or sampleIndex == ChargeMisID or sampleIndex == FakeRate);
  
  cout << "Init LumiReWeighting"<<endl;
  
  std::string generatedFile("pu_mc.root");
  std::string dataFile("pu_data.root");
  std::string genHistName("PUS10_Distr");
  std::string dataHistName("PUS10_Distr");
  fexists(generatedFile,true);
  fexists(dataFile,true);
  LumiReWeighting lumiReWeighting(generatedFile, dataFile, genHistName, dataHistName);

  //Output file
  char name[300];
  time_t rawtime;
  time ( &rawtime );
  
  if (b_RealData) sprintf(name,"ctaOutFile_%s_%s_%s_%lx.root", argv[2], argv[1], signStr.c_str(), rawtime);
  else            sprintf(name,"ctaOutFile_%s_%s_%lx.root",    argv[2],          signStr.c_str(), rawtime);

  TFile* outFile = new TFile(name, "RECREATE");

//Scale factors:
//   double sf=1.;
  
  // Trigger SF
  
  // Lepton ID
  TableReader electronTightSF2012("electronTightSF2012.txt");
  TableReader muTightSF2012AB("muTightSF2012AB.txt");
  TableReader muTightSF2012C("muTightSF2012C.txt");

  unsigned int nCutSteps = 6;

  TH1F* h_CutFlow;

  vector <StandardHistos*> allCuts_std_h(3);
  vector <StandardHistos*> twoSSLep_std_h(3);
  vector <StandardHistos*> twoSSLepZQV_std_h(3);
  vector <StandardHistos*> twoSSLepZQVTwoJ_std_h(3);

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  h_CutFlow = new TH1F("h_CutFlow_"+sampleName, "h_CutFlow_"+sampleName, nCutSteps, -0.5, nCutSteps - 0.5);
  h_CutFlow->Sumw2();

  //Make directories and initialize histograms
  initHistoDirThreeChannels(outFile, twoSSLep_std_h,        "SS_2L",        sampleName, sScale, true);
  initHistoDirThreeChannels(outFile, twoSSLepZQV_std_h,     "SS_2L_ZQV",    sampleName, sScale, true);
  initHistoDirThreeChannels(outFile, twoSSLepZQVTwoJ_std_h, "SS_2L_ZQV_2J", sampleName, sScale, true);
  initHistoDirThreeChannels(outFile, allCuts_std_h,         "AllCuts",      sampleName, sScale, true);
  
  outFile->cd();

  cout << "Looping over "<<sampleName<<" "<< treeClass->getEntries()<<endl;

  for (Long64_t il = 0; il < treeClass->getEntries(); il++){
//     for (Long64_t il = 0; il < 200000; il++){
    
    if (il % 100000 == 0) cout<<"At event "<<il<<endl;

    treeClass->GetEntry(il);
//     hcal.filter(treeClass->run,treeClass->lumi,treeClass->event);
    
    //Initialize T53Event class
    teve->clearEvent();
    teve->isam = sampleIndex;
    teve->realData = b_RealData;
    
    /// Review PU:
    if (teve->realData) teve->weight = 1.0;
    else                teve->weight = treeClass->weight_PU;
    
    //Tree-level cuts only (2 leptons > 20 GeV, 2 jets > 30 GeV)
    h_CutFlow->Fill(0.0, teve->weight);

    int  nPrompt = 0;
    for (unsigned int ig = 0; ig < treeClass->genID->size(); ig++){
      int pdgid = abs(treeClass->genID->at(ig));
      //I don't know what taus decay to so treat them as leptons
      if (treeClass->genStatus->at(ig) == 3 and (pdgid == 11 or pdgid == 13 or pdgid == 15)) nPrompt++;
    }
    if (nPrompt > 1) continue;

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

    //Determine channel
    if (    teve->lepton1->isMuon() and teve->lepton2->isMuon())           teve->suffix = "MuMu";
    else if ((teve->lepton1->isMuon() and teve->lepton2->isElectron()) or
	     (teve->lepton2->isMuon() and teve->lepton1->isElectron()))    teve->suffix = "ElMu";
    else if (teve->lepton1->isElectron() and teve->lepton2->isElectron())  teve->suffix = "ElEl";
    else{
      cout<<"Invalid channel!"<<endl;
      return 1;
    }

    teve->leptonSum  = teve->lepton1->lv + teve->lepton2->lv;
    teve->theLeptons = treeClass->goodLeptons;

    //Extend the lepton collection with selected loose leptons
    if (b_DoFakes){
      if (not teve->lepton1->goodLepton()) teve->theLeptons.push_back(teve->lepton1);
      if (not teve->lepton2->goodLepton()) teve->theLeptons.push_back(teve->lepton2);
    }
 
    //Get rid of events with 3 leptons where two of them come from a Z
    bool b_Z3L = false;

    for (unsigned int il = 0; il < teve->theLeptons.size(); il++){
      double mz = 0;
      if (teve->lepton1->charge *  teve->theLeptons.at(il)->charge == -1 and
	  ((teve->lepton1->isMuon()     and teve->theLeptons.at(il)->isMuon()) or
	   (teve->lepton1->isElectron() and teve->theLeptons.at(il)->isElectron())) and
	  teve->theLeptons.at(il)->lv.DeltaR(teve->lepton2->lv) > 0.1){
	
	mz = (teve->lepton1->lv + teve->theLeptons.at(il)->lv).M();
	if (mz < 106 and mz > 76) b_Z3L = true;
      }
      if (teve->lepton2->charge *  teve->theLeptons.at(il)->charge == -1 and
	  ((teve->lepton2->isMuon()     and teve->theLeptons.at(il)->isMuon()) or
	   (teve->lepton2->isElectron() and teve->theLeptons.at(il)->isElectron())) and
	  teve->theLeptons.at(il)->lv.DeltaR(teve->lepton1->lv) > 0.1){
	
	mz = (teve->lepton2->lv + teve->theLeptons.at(il)->lv).M();
	if (mz < 106 and mz > 76) b_Z3L = true;
      }
    }
    if (b_Z3L) continue;
    //End trilepton Z cleaning
    
    if (not teve->realData) teve->weight *= getLeptonSF(teve->lepton1, treeClass->run) *
			                    getLeptonSF(teve->lepton2, treeClass->run);   
    
    if (sampleIndex == ChargeMisID){
      teve->weight = 0;
      if (not teve->lepton1->isMuon()) teve->weight += getChargeMisIdProb(teve->lepton1->lv.Eta());
      if (not teve->lepton2->isMuon()) teve->weight += getChargeMisIdProb(teve->lepton2->lv.Eta());
    }
         
    //Jet Cuts
    teve->vGoodCATopJets = treeClass->goodCATopJets(teve->theLeptons);
    teve->vGoodCAWJets   = treeClass->goodCAWJets(teve->theLeptons, teve->vGoodCATopJets);
    teve->vGoodAK5Jets   = treeClass->goodAK5Jets(teve->theLeptons, teve->vGoodCATopJets, teve->vGoodCAWJets);

    for (unsigned int ij = 0; ij < teve->vGoodAK5Jets.size(); ij++){
      teve->ht += teve->vGoodAK5Jets.at(ij)->lv.Pt();
      if (teve->vGoodAK5Jets.at(ij)->csvMedium != 0) teve->nBTags++; //Medium
    }
    teve->met = treeClass->PF_met_pt;

    bool b_PassZVeto  = true;
    bool b_MuoniaVeto = true;
    
    //Muonia and Z vetoes
    if (teve->leptonSum.M() < 20) b_MuoniaVeto = false;
    if (teve->suffix == "MuMu" or teve->suffix == "ElEl"){
      if (teve->leptonSum.M() < 106 and teve->leptonSum.M() > 76) b_PassZVeto = false;
    }
   
    //Found good same-sign leptons
    fillHistosThreeChannels(twoSSLep_std_h, teve, eventType, b_DoFakes);

    //CutFlow for Thomas
    if (treeClass->goodLeptons.size() == 2){
      h_CutFlow->Fill(1.0, teve->weight);
      int nJets = teve->vGoodCATopJets.size() + teve->vGoodCAWJets.size() + teve->vGoodAK5Jets.size();
      if (nJets >= 2) h_CutFlow->Fill(2.0, teve->weight);
      if (nJets >= 3 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(3.0, teve->weight);
      if (nJets >= 4 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(4.0, teve->weight);
      if (nJets >= 5 and b_PassZVeto and b_MuoniaVeto) h_CutFlow->Fill(5.0, teve->weight);
    }
    
    if (b_PassZVeto and b_MuoniaVeto and teve->nBTags > MIN_NBTAG){
      fillHistosThreeChannels(twoSSLepZQV_std_h, teve, eventType, b_DoFakes);
      
      if (teve->vGoodAK5Jets.size() + 2*teve->vGoodCAWJets.size() + 3*teve->vGoodCATopJets.size() >= 2) {
	fillHistosThreeChannels(twoSSLepZQVTwoJ_std_h, teve, eventType, b_DoFakes);
      }
    }
  }//End loop over entries
  
  if (b_DoFakes){
    getTotFakesThreeChannels(twoSSLep_std_h);
    getTotFakesThreeChannels(twoSSLepZQV_std_h);
    getTotFakesThreeChannels(twoSSLepZQVTwoJ_std_h);
    getTotFakesThreeChannels(allCuts_std_h);
  }
  
  if (not teve->realData){
    scaleHistosThreeChannels(twoSSLep_std_h, b_DoFakes);
    scaleHistosThreeChannels(twoSSLepZQV_std_h, b_DoFakes);
    scaleHistosThreeChannels(twoSSLepZQVTwoJ_std_h, b_DoFakes);
    scaleHistosThreeChannels(allCuts_std_h, b_DoFakes);
  }
  
  outFile->Write();
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

#endif
