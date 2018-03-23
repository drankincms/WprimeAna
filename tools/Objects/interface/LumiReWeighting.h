#ifndef PhysicsTools_Utilities_interface_LumiReWeighting_cc
#define PhysicsTools_Utilities_interface_LumiReWeighting_cc

#include "TH1.h"
#include "TFile.h"
#include <string>

class LumiReWeighting {
public:
  LumiReWeighting( std::string generatedFile,
		   std::string dataFile,
		   std::string GenHistName,
		   std::string DataHistName);

  double weight( int npv ) ;

  double weight( float npv ) ;

//   double weight( const edm::EventBase &e ) ;
// 
//   double weightOOT( const edm::EventBase &e ) ;
// 
//   void weightOOT_init(); 

protected:

  std::string generatedFileName_;
  std::string dataFileName_;
  std::string GenHistName_;
  std::string DataHistName_;
  TFile*     generatedFile_;
  TFile*     dataFile_;
  TH1*	  weights_;

  //keep copies of normalized distributions:

  TH1*	   MC_distr_;
  TH1*	   Data_distr_;
};

#endif
