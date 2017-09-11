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
# Note: You must change some paths to use this code.
#
###########################################################################


#!/bin/bash
INPUT=$1
ACCEPT=${2%.*}-${1%%.*}.${2#*.}
REJECT=${3%.*}-${1%%.*}.${3#*.}
zcat /home/ahiralesc/workload/google/task_events/$INPUT | \
/home/ahiralesc/mysrc/cpp/guct/bin/guttaccept \
-a /home/ahiralesc/workload/guct/accepted/$ACCEPT \
-r /home/ahiralesc/workload/guct/rejected/$REJECT
