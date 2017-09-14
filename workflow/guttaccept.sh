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

# Prepare the execution context

# Export the location of shared library
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/home/ahiralesc/mysrc/cpp/cutu/lib"

# Export path to binaries
export PATH="/home/ahiralesc/mysrc/cpp/cutu/bin:$PATH"

# Prepare I/O paths
SPATH=$1
TPATH=$2
ACCEPTED=${TPATH}${3%%.*}-${4%.*}.${4#*.}
REJECTED=${TPATH}${3%%.*}-${5%.*}.${5#*.}

zcat $SPATH | guttaccept -a $ACCEPTED -r $REJECTED
