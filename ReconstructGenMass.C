#ifndef ReconstructGenMass_cxx
#define ReconstructGenMass_cxx

#include "GetT53GenParticles.C"
#include "T53LoadData2012.C"

using namespace std;

struct SortByDW {
  bool operator()(const TLorentzVector v1, const TLorentzVector v2) {
    double dw1 = fabs(v1.M() - MW);
    double dw2 = fabs(v2.M() - MW);
    return (dw1 < dw2);
  }
};

int main( int argc, const char* argv[] ){

  TString fileName   = "/home/avetisya/CMS/TopPartners/Sums/T53T53_750.root";
  TString sampleName = "T53m750";
  
  T53Event * teve = new T53Event();
  initAll_2012(fileName, sampleName);

  bool b_DoFakes  = (sampleIndex == FakeRate);
  bool b_RealData = (sampleIndex == Data or sampleIndex == ChargeMisID or sampleIndex == FakeRate);

  int sign = 1;

  TFile* outFile = new TFile("rgmOutFile_T53m750.root", "RECREATE");
  
  TH1F  *hnEvents    = new TH1F("hnEvents",     "hnEvents",      1,  0.5,    1.5);
  TH1F  *hn2rlEvents = new TH1F("hn2rlEvents",  "hn2rlEvents",   1,  0.5,    1.5);
  TH1F  *hn6pEvents  = new TH1F("hn6pEvents",   "hn6pEvents",    1,  0.5,    1.5);
  TH1F  *hGenT53Mass = new TH1F("hnGenT53Mass", "hnGenT53Mass", 80,  400,   1200.);

  TLorentzVector vtemp;

  for (Long64_t il = 0; il < treeClass->getEntries(); il++){
    treeClass->GetEntry(il);

    hnEvents->Fill(1);

    //Initialize T53Event class
    teve->clearEvent();
    teve->isam     = sampleIndex;
    teve->realData = b_RealData;
    teve->nPV      = treeClass->nPV;

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

    //Go to the next event if no leptons were found
    if (not b_LepCuts) continue;

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

    hn2rlEvents->Fill(1);
    
    //Determine the number of gen partons in the event and Lorentz vectors
    vector <TLorentzVector> genLV;
    vector <TLorentzVector> genPartons;
    for (unsigned int ui = 0; ui < treeClass->genID->size(); ui++){
      vtemp.SetPxPyPzE(treeClass->genPt->at(ui)*cos(treeClass->genPhi->at(ui)), treeClass->genPt->at(ui)*sin(treeClass->genPhi->at(ui)), 
		       treeClass->genPt->at(ui)*TMath::SinH(treeClass->genEta->at(ui)), treeClass->genEnergy->at(ui));
      genLV.push_back(vtemp);
      if (abs(treeClass->genID->at(ui)) < 6) genPartons.push_back(vtemp);      
    }
    //Only interested in events with 6 partons
    if (genPartons.size() != 6) continue;  

    hn6pEvents->Fill(1);
    
    vector <T53GenParticle> genParticles = GetT53GenParticles(genLV, (*treeClass->genID), (*treeClass->genMotherID), (*treeClass->genMotherIndex));

    vector <int> iUsedAK5;
    bool foundW1 = false;
    bool foundW2 = false;
    vector <TLorentzVector> vWs;
    TLorentzVector lvT53, lvTop, lvW;
    double bestMass = 1e8;
    bool foundT53Top = false;
    bool foundT53W   = false;
    for (unsigned int uij = 0; uij < genPartons.size(); uij++){
      bool bUsed = false;
      for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
	if (iUsedAK5.at(iused) == (int) uij) bUsed = true;
      }
      if (bUsed) continue;
      for (unsigned int ujj = uij + 1; ujj < genPartons.size(); ujj++){
	for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
	  if (iUsedAK5.at(iused) == (int) ujj) bUsed = true;
	}
	if (bUsed) continue;
	TLorentzVector lvtmp = genPartons.at(uij) + genPartons.at(ujj);
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
    
    sort(vWs.begin(), vWs.end(), SortByDW());
    
    if (vWs.size() > 1){
      //Two W's found (if there are more, use the two closest to M(W)
      //Need to use one to make a top using AK5 jets
      bestMass = 1e8;
      for (unsigned int uij = 0; uij < genPartons.size(); uij++){
	bool bUsed = false;
	for (unsigned int iused = 0; iused < iUsedAK5.size(); iused++){
	  if (iUsedAK5.at(iused) == (int) uij) bUsed = true;
	}
	if (bUsed) continue;

	TLorentzVector lvtmp = vWs.at(0) + genPartons.at(uij);
	if (fabs(lvtmp.M() - MTOP) < DMTOP and fabs(bestMass - MTOP) > fabs(lvtmp.M() - MTOP)){
	  bestMass    = lvtmp.M();
	  lvTop       = lvtmp;
	  foundT53Top = true;
	  
	  lvW         = vWs.at(1); //Set the T53W to be the other W
	  foundT53W   = true;
	}
	  
	//Once more, with the second W
	lvtmp = vWs.at(1) + genPartons.at(uij);
	if (fabs(lvtmp.M() - MTOP) < DMTOP and fabs(bestMass - MTOP) > fabs(lvtmp.M() - MTOP)){
	  bestMass    = lvtmp.M();
	  lvTop       = lvtmp;
	  foundT53Top = true;
	  
	  lvW         = vWs.at(0); //Set the T53W to be the other W
	  foundT53W   = true;
	}	    
      }//End loop over AK5 jets
    }//End more than one W if
    
    if (foundT53Top and foundT53W){ //Done!
      lvT53 = lvTop + lvW;
      hGenT53Mass->Fill(lvT53.M());
    }

  }//End loop over tree entries  

  outFile->Write();
  return 0;
}


#endif
