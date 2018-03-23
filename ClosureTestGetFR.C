#ifndef ClosureTestGetFR_cxx
#define ClosureTestGetFR_cxx

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

int main( int argc, const char* argv[] ){
 
  if (argc < 4) {
    cout<<"Need 3 arguments:\n";
    cout<<"- lepton channel (Mu or El)\n";
    cout<<"- sample"<<endl;
    cout<<"- root file name"<<endl;
    return 0;
  }
//   HcalLaserEventFilter2012 hcal("./AllBadHCALLaser.txt.gz");
  
  TString sChan(argv[1]);
  
  bool bMu;
  if      (sChan == "El") bMu = false;
  else if (sChan == "Mu") bMu = true;
  else{
    cout<<"Invalid channel! Must be Mu or El"<<endl;
    return 1;
  }

  TString sampleName(argv[2]);
  TString fileName(argv[3]);
  
  cout<<"Processing "<<sampleName<<'\t'<<fileName<<endl;
  
  T53Event * teve = new T53Event();
  initAll_2012(fileName, sampleName);

  TString signStr = "";
  if (argc > 4) signStr = argv[4];
  
  int sign = 1;
  if (signStr == "OS")            sign = -1;
  if (sampleIndex == ChargeMisID) sign *= -1;

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
  
  sprintf(name,"ctfOutFile_%s_%s.root", argv[2], argv[1]);

  TFile* outFile = new TFile(name, "RECREATE");

//Scale factors:
//   double sf=1.;
  
  // Trigger SF
  
  // Lepton ID
  TableReader electronTightSF2012("electronTightSF2012.txt");
  TableReader muTightSF2012AB("muTightSF2012AB.txt");
  TableReader muTightSF2012C("muTightSF2012C.txt");

  unsigned int nCutSteps = 6;

  double fScale = 1.0;
  //DoubleElectron up to the end of C1: 5.882pb
  //DoubleMu up to the end of C2:       15.286pb

  // double       lumi = 15.286;
  // if (not bMu) lumi = 5.882;

  // if (sampleIndex == TTbar) fScale = lumi * 225.197 / 6923750.;
  // if (sampleIndex == ZJets) fScale = lumi * 3503.71 / 30459503.;
  // if (sampleIndex == WJets) fScale = lumi * 37509.0 / 18331570.;
 
  outFile->mkdir("Fakes");
  outFile->cd("Fakes");
  StandardHistosFR* Fakes_stdfr_h  = new StandardHistosFR("Fakes",  sChan, sampleName, fScale);

  outFile->mkdir("Prompt");
  outFile->cd("Prompt");
  StandardHistosFR* Prompt_stdfr_h = new StandardHistosFR("Prompt", sChan, sampleName, fScale);
  

  cout << "init histos "<<sampleIndex<<" "<<sampleName<<endl;

  outFile->cd();
  
  TH1F* h_CutFlowLoose = new TH1F("h_CutFlowLoose_"+sampleName+"_"+sChan, "h_CutFlowLoose_"+sampleName+"_"+sChan, nCutSteps, -0.5, nCutSteps - 0.5);
  h_CutFlowLoose->Sumw2();

  TH1F* h_CutFlowTight = new TH1F("h_CutFlowTight_"+sampleName+"_"+sChan, "h_CutFlowTight_"+sampleName+"_"+sChan, nCutSteps, -0.5, nCutSteps - 0.5);
  h_CutFlowTight->Sumw2();

  outFile->cd();

  cout << "Looping over "<<sampleName<<" "<< treeClass->getEntries()<<endl;

  const double MINPT = 30.;
  // const double MAXPT = 1000.;

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
 
    //Lepton cuts first    
    bool b_LepCuts = false;
    bool b_GoodLep = false;
          
    //Select only events which pass the trigger for the channel we are looking at and avoid overlaps for data
    bool b_SelTrigMM = ((not teve->realData or treeClass->dataMM > 0) and treeClass->trigMM > 0 and bMu);
    bool b_SelTrigEE = ((not teve->realData or treeClass->dataEE > 0) and treeClass->trigEE > 0 and not bMu);

    bool b_Tau    = false;
    bool b_Lepton = false;
    vector <TLorentzVector> genLeptons;
    TLorentzVector lvtmp;
    for (unsigned int ig = 0; ig < treeClass->genID->size(); ig++){
      int pdgid = abs(treeClass->genID->at(ig));
      //I don't know what taus decay to so ignore events with them altogether
      if (pdgid == 15 and treeClass->genStatus->at(ig) == 3){
	b_Tau = true;
	break;
      }
      if ((pdgid == 11 or pdgid == 13) and treeClass->genStatus->at(ig) == 3){
	b_Lepton = true;
	lvtmp.SetPxPyPzE(treeClass->genPt->at(ig)*cos(treeClass->genPhi->at(ig)),         treeClass->genPt->at(ig)*sin(treeClass->genPhi->at(ig)), 
			 treeClass->genPt->at(ig)*TMath::SinH(treeClass->genEta->at(ig)), treeClass->genEnergy->at(ig));
	genLeptons.push_back(lvtmp);
      }
    }
    if (b_Tau) continue;
    
    bool b_Prompt = false;
    if (b_SelTrigMM){
      for (unsigned int il = 0; il < treeClass->allMuons.size(); il++){
	for (unsigned int ig = 0; ig < genLeptons.size(); ig++){
	  if (genLeptons.at(ig).DeltaR(treeClass->allMuons.at(il)->lv) < 0.3){
	    b_Prompt = true;
	    break;
	  }
	}
	if (treeClass->allMuons.at(il)->looseMuon(MINPT) and (not b_Lepton or b_Prompt)){
	  b_LepCuts = true;
	  teve->lepton1 = treeClass->allMuons.at(il);
	  if (teve->lepton1->goodMuon(MINPT)) b_GoodLep = true;
	  break;
	}
      }
    }

    if (b_SelTrigEE){
      for (unsigned int il = 0; il < treeClass->allElecs.size(); il++){
	for (unsigned int ig = 0; ig < genLeptons.size(); ig++){
	  if (genLeptons.at(ig).DeltaR( treeClass->allElecs.at(il)->lv) < 0.3){
	    b_Prompt = true;
	    break;
	  }
	}
	if (treeClass->allElecs.at(il)->looseElectron(MINPT) and (not b_Lepton or b_Prompt)){
	  b_LepCuts = true;
	  teve->lepton1 = treeClass->allElecs.at(il);
	  if (teve->lepton1->goodElectron(MINPT)) b_GoodLep = true;
	  break;
	}
      }
    }

    if (not b_LepCuts) continue;

    if (not teve->realData) teve->weight *= getLeptonSF(teve->lepton1, treeClass->run);   
           
    //JetMET
    teve->vGoodAK5Jets = treeClass->allAK5Jets;
    teve->met          = treeClass->PF_met_pt;
    
    double dphi = fabs(treeClass->PF_met_phi - teve->lepton1->lv.Phi());
    if (dphi > TMath::Pi()) dphi = TMath::TwoPi() - dphi;
    
    double mt2 = 2 * teve->lepton1->lv.Pt() * teve->met * (1 - cos(dphi));;
    teve->mt  = sqrt(mt2);
    
    if (not b_Lepton) Fakes_stdfr_h ->fillTLHistos(teve, b_GoodLep);
    if (b_Prompt)     Prompt_stdfr_h->fillTLHistos(teve, b_GoodLep);
  }//End loop over entries

  Fakes_stdfr_h->scaleTLHistos();
  Fakes_stdfr_h->getRatioHistos();

  Prompt_stdfr_h->scaleTLHistos();
  Prompt_stdfr_h->getRatioHistos();
  
  
  outFile->Write();
  return 0;
}

#endif
