#!/bin/tcsh -f

set lp = "("
set rp = ")"
set po = "#"
set scale = 1.0

foreach dir (Run2016_HF_Feb20/)
  foreach sys (0 1 2 3 4)
    eval ./GetWJetsHF.o ${dir} '""' 0 ${scale} ${sys}
  end
end
echo "Done!"
