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
# 1 - event time stamp
# 2 - missing info
# 3 - job id
# 4 - task index
# 5 - machine id
# 6 - event type
# 7 - user name
# 8 - scheduling class
# 9 - priority
# 10 - resource request CPU
# 11 - resource request RAM
# 12 - resource request DISK space
# 13 - machine constraints
#
# Output:
# 1 - event time stamp (start)
# 2 - task index
# 3 - job id
# 4 - tuple: task index - job id

awk -F "\"*,\"*" '
BEGIN { 
    sta[0]=0;}
{ 
    if( $6 in sta) {
        key = $4 "-" $3;
        printf "%d,%d,%d,%s\n",$1,$4,$3,key; 
        cnt++
    }
}' $1

#END { print "count=",cnt}' $1
