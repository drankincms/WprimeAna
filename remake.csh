#!/bin/tcsh -f

foreach o ( `ls *.o` )
  eval make `echo ${o} | sed 's/\.o$//g'`
end
