#ifndef FindEvent_cxx
#define FindEvent_cxx

#include <TFile.h>
#include <TTree.h>

int main( int argc, const char* argv[] ){


  TFile* myfile();
  
  Int_t           run, lumi, event;

  TBranch        *b_run_CommonCalc;   //!
  TBranch        *b_lumi_CommonCalc;   //!
  TBranch        *b_event_CommonCalc;   //!
   
  fChain->SetBranchAddress("run_CommonCalc", &run, &b_run_CommonCalc);
  fChain->SetBranchAddress("lumi_CommonCalc", &lumi, &b_lumi_CommonCalc);
  fChain->SetBranchAddress("event_CommonCalc", &event, &b_event_CommonCalc);



}

#endif
