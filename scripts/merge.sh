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


# Extract the log number
n=$1                            # Numbers of files 
i=${2:5:5}                      # File ID
l=$3                            # Current level

# Prepare the file name of the adyacent file in the sequence of files
j=$(python -c 'import sys; val=int(sys.argv[1]); print val + 1' $i)
m=$((${#i}-${#j}))              # Computes the difference in prefix zero characters
for ((k=1; k<=m; k++)); do      # Concatenates missing prefix zero characters
    j="0"${j}
done    
fname=${2:0:5}$j${2:10:${#2}}   # ${#2} is the varibale length

# Computes the depth of the binary tree (stopping criterion)
depth=$(python -c 'import math; import sys; val=int(sys.argv[1]); print math.ceil(math.log(val)/math.log(2))' $n)

# Evaluates if the ID is odd or even
p=$(python -c 'import sys; val=int(sys.argv[1]); print val % 2' $i)

if [ "$p" == "0" ]
then
    echo "PAR toco trabajar"
    if [ -f $fname ] 
    then
        echo "Existe el archivo"
        
    else
        echo "No existe el archivo"
    fi
fi
