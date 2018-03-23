CXX=g++

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

ROOTIL      = $(ROOTCFLAGS) $(ROOTLIBS)

LIBS        = libObjects.so

PWD         = $(shell pwd)

TOOLDIR     = $(PWD)/tools

WprimeAnalyzer:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer.o WprimeAnalyzer.C

WprimeAnalyzer_LHE:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_LHE.o WprimeAnalyzer_LHE.C

WprimeAnalyzer_Slim:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim.o WprimeAnalyzer_Slim.C

WprimeAnalyzer_Slim_Opt:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_Opt.o WprimeAnalyzer_Slim_Opt.C

WprimeAnalyzer_Slim_btp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_btp.o WprimeAnalyzer_Slim_btp.C

WprimeAnalyzer_Slim_btm:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_btm.o WprimeAnalyzer_Slim_btm.C

WprimeAnalyzer_Slim_mtp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_mtp.o WprimeAnalyzer_Slim_mtp.C

WprimeAnalyzer_Slim_mtm:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_mtm.o WprimeAnalyzer_Slim_mtm.C

WprimeAnalyzer_Slim_tpm:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_tpm.o WprimeAnalyzer_Slim_tpm.C

WprimeAnalyzer_Slim_tpp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_tpp.o WprimeAnalyzer_Slim_tpp.C

WprimeAnalyzer_Slim_wjp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_wjp.o WprimeAnalyzer_Slim_wjp.C

WprimeAnalyzer_Slim_wjm:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_wjm.o WprimeAnalyzer_Slim_wjm.C

WprimeAnalyzer_Slim_mup:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_mup.o WprimeAnalyzer_Slim_mup.C

WprimeAnalyzer_Slim_mum:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_mum.o WprimeAnalyzer_Slim_mum.C

WprimeAnalyzer_Slim_pup:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_pup.o WprimeAnalyzer_Slim_pup.C

WprimeAnalyzer_Slim_pum:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_pum.o WprimeAnalyzer_Slim_pum.C

WprimeAnalyzer_Slim_msp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_msp.o WprimeAnalyzer_Slim_msp.C

WprimeAnalyzer_Slim_msm:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_msm.o WprimeAnalyzer_Slim_msm.C

WprimeAnalyzer_Slim_mrp:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Slim_mrp.o WprimeAnalyzer_Slim_mrp.C

WprimeAnalyzer_Test:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_Test.o WprimeAnalyzer_Test.C

WprimeAnalyzer_HF:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeAnalyzer_HF.o WprimeAnalyzer_HF.C

WprimeTopPt:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o WprimeTopPt.o WprimeTopPt.C

DrawOneHistoWithSyst:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoWithSyst.o DrawOneHistoWithSyst.C

DrawOneHistoWithSystSignal:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoWithSystSignal.o DrawOneHistoWithSystSignal.C

DrawOneHistoForPaper:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoForPaper.o DrawOneHistoForPaper.C

DrawOneHistoThreeSig:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoThreeSig.o DrawOneHistoThreeSig.C

DrawOneHistoSystUnc:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoSystUnc.o DrawOneHistoSystUnc.C

DrawOneHistoSystUncRatio:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoSystUncRatio.o DrawOneHistoSystUncRatio.C

DrawOneHistoPU:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoPU.o DrawOneHistoPU.C

DrawOneHistoECFA:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoECFA.o DrawOneHistoECFA.C

DrawOneHistoRatio:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoRatio.o DrawOneHistoRatio.C

DrawOneHistoFit:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoFit.o DrawOneHistoFit.C

DrawOneHistoSigRatio:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoSigRatio.o DrawOneHistoSigRatio.C

DrawWJetsShape:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawWJetsShape.o DrawWJetsShape.C

DrawOneHistoSignal:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o DrawOneHistoSignal.o DrawOneHistoSignal.C

GetWJetsHF:
	$(CXX) $(ROOTIL) -L$(TOOLDIR)/lib -I$(TOOLDIR) -lObjects -lz -O2 -Wall -o GetWJetsHF.o GetWJetsHF.C


clean: 
	rm *.o *.so *.d

.PHONY: WprimeAnalyzer
.PHONY: WprimeAnalyzer_LHE
.PHONY: WprimeAnalyzer_Slim
.PHONY: WprimeAnalyzer_Slim_Opt
.PHONY: WprimeAnalyzer_Slim_btp
.PHONY: WprimeAnalyzer_Slim_btm
.PHONY: WprimeAnalyzer_Slim_mtp
.PHONY: WprimeAnalyzer_Slim_mtm
.PHONY: WprimeAnalyzer_Slim_wjp
.PHONY: WprimeAnalyzer_Slim_wjm
.PHONY: WprimeAnalyzer_Slim_pup
.PHONY: WprimeAnalyzer_Slim_pum
.PHONY: WprimeAnalyzer_Slim_msp
.PHONY: WprimeAnalyzer_Slim_msm
.PHONY: WprimeAnalyzer_Slim_mrp
.PHONY: WprimeAnalyzer_Slim_tpm
.PHONY: WprimeAnalyzer_Slim_tpp
.PHONY: WprimeAnalyzer_Slim_mup
.PHONY: WprimeAnalyzer_Slim_mum
.PHONY: WprimeAnalyzer_Test
.PHONY: WprimeAnalyzer_HF
.PHONY: WprimeTopPt
.PHONY: DrawOneHistoSystUnc
.PHONY: DrawOneHistoSystUncRatio
.PHONY: DrawOneHistoWithSyst
.PHONY: DrawOneHistoWithSystSignal
.PHONY: DrawOneHistoForPaper
.PHONY: DrawWJetsShape
.PHONY: DrawOneHistoThreeSig
.PHONY: DrawOneHistoPU
.PHONY: DrawOneHistoECFA
.PHONY: DrawOneHistoSigRatio
.PHONY: DrawOneHistoRatio
.PHONY: DrawOneHistoFit
.PHONY: DrawOneHistoSignal
.PHONY: GetWJetsHF
.PHONY: clean
