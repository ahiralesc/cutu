#!/bin/bash
../build/jtraceextract -i ../../bin/unsorted.json -o extracted_traces.csv
#sort -t ',' -k2n -k3n extracted_traces.csv > sorted_traces.csv
#../build/jtracesort -i ../../bin/unsorted.json -s sorted_traces.csv -o sorted.json
