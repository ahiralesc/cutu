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

IFILE=$1
SE=${1%.*}-sta.${1#*.}  # Start event log
EE=${1%.*}-end.${1#*.}  # End event log
TI=${1%.*}-int.${1#*.}  # Task intervals
event_start_make.sh $IFILE > $SE
event_end_make.sh $IFILE > $EE
event_concatenate.sh $SE $EE $TI
rm $SE
rm $EE
