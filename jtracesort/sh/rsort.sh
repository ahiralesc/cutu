#!/bin/bash
./../../jtraceextract -i ../../bin/unsorted.json -o extracted.txt
sort -t ',' -k2n -k3n extracted.txt
