#Author Omar.Zapata@cern.ch 2017
#!/bin/bash
root -l -q  parsenames.C  | grep Br | gawk -F ":" '{print "\""$3}' | sed -e 's/\/F/","F"/g;' -e 's/*//g' -e 's/ //g'
