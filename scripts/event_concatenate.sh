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
# 2 - task index
# 3 - job id
# 4 - tuple: task index - job id 
#
# Output:
# 1 - event time stamp (start)
# 2 - event time stamp (end)
# 3 - task index
# 4 - job id

awk -F "\"*,\"*" '
FNR==NR{ 
    # procesa bitacora 1
    h[$4] = $1",NA,"$2","$3;
    next 
}
{   # procesa bitacora 2
    value=h[$4];    
    if(value=="") {
        h[$4]="NA,"$1","$2","$3;
    } else {
        # Es posible una tarea tenga multiples eventos de finalizacin. Cuando esto ocura,
        # Es importante solo considerar el ultimo evento de finalizacion. Esto se debe corregir.
        gsub("NA",$1,value);
        h[$4]=value;
    }; 
}
END{for(k in h)
    print h[k]}
' $1 $2 > $3
