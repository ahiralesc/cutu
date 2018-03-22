#!/bin/bash

# Copyright 2017 The CUTU Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


###########################################################################
#
# Author: Adan Hirales Carbal
# Email : ahiralesc@gmail.com
#
###########################################################################

# Input:
# 1 - event time stamp (start)
# 2 - event time stamp (end)
# 3 - task index
# 4 - job id
#
# Three outputs are produced. I and II, open and close intervals. Both have 
# equal format as the input. III, log stats format is:
# 1 - interval count
# 2 - open interval count
# 3 - open interval ratio
#
# Usage: interval_split.sh <source> <target path> <target file name> 

TPATH=$2
OINTLOG=${TPATH}${3%.*}-open.${3#*.}     # Open interval log
CINTLOG=${TPATH}${3%.*}-closed.${3#*.}   # Close interval log
SINTLOG=${TPATH}${3%.*}-stat.${3#*.}     # Stat interval log

awk -v oi="$OINTLOG" -v ci="$CINTLOG" -v si="$SINTLOG"  -F "\"*,\"*" '
BEGIN{
    ocnt=0;
    acnt=0;
}
{
    acnt++;
    if($1=="NA" || $2=="NA") {
        print $0 > oi;
        ocnt++;
    } else {
        print $0 > ci;
    }
} 
END{
    print "all intervals, open intervals, open interval ratio\n" > si;
    print acnt","ocnt","acnt/ocnt > si;
}' $1
