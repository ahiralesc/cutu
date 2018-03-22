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
# Output:
#
# Usage: log_split.sh <source> <target path> <target file name> 

TPATH=$2
CTLOG=${TPATH}${3%.*}-cmplt.${3#*.}     # Complete trace log
ITLOG=${TPATH}${3%.*}-icmplt.${3#*.}    # Incomplete trace log
SPATH=${TPATH}${3:0:24}.${3#*.}         # Log containing all events

awk -v ctl="$CTLOG" -v itl="$ITLOG"  -F "\"*,\"*" '
FNR==NR{   
    # Processes log $1. It contains close intervals
    key = $3-$4;
    h[key] = key;
    next
}
{
    # Processes log $2. It contains all events
    key = $4-$3;
    val = h[key];
    if(val != "" && val==key){
        print $0 > ctl;
    }else{
        print $0 > itl;
    }
}' $1 $SPATH
