#!/bin/bash
INFILE=${1}
FILENAME=${1##*/}

jtraceextract -i ${INFILE} -o ${INFILE}.csv
#sort -t ',' -k3n -k2n ${INFILE}.csv > ${INFILE}.csv
#jtracesort -i $INFILE -s $(INFILE)_sorted.csv -o $INFILE.json
