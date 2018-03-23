###Analysis code for W' -> tb (leptonic)

##Setup:

```
source setup.csh
cd tools/Objects
make clean
make all
cd ../..
make clean
make all
```

##Useful Scripts:

- `AnalyzerSubmit` [for bulk condor submission]
  - Uses `WprimeAnalyzer_Slim_Opt.C`, which is the actual script that implements cuts, applies weighting, etc. Exceptions are `_LHE` scripts for PDF uncertainties and `TopPt` scripts for Top Pt reweighting
- `AnalyzerHadd` [for pulling together condor output]
- `Draw` {`OneHistoThreeSig`, `OneHistoSigRatio`, `OneHistoSystUnc`, `OneHistoSystUncRatio`, `WJetsShape`} [for plotting]
- `makePlots` [for bulk plot production]
- `WprimeHistosForLimits` [for creating histos for `theta` usage]
  - `_AddSyst` [for adding in PDF uncertainties from dedicated calculations, `_LHE` scripts]
