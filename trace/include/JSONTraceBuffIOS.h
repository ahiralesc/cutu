/* ==========================================================================
Copyright 2017 The CUTU Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
============================================================================= */


#ifndef __JSONBIOS_
#define __JSONBIOS_

#include <vector>
#include <fstream>
#include "Trace.h"

class JSONTraceBuffIOS {
    public:
        JSONTraceBuffIOS(std::string, int sz = 300);
        ETrace::Trace next();
        ETrace::Trace peek();
        bool empty();

    private:
        std::string in_file;
        int num_traces;
        bool initialized, finalized;
        std::vector<std::string> traces;
        std::ifstream stream;       
 
        void initialize();
        void ensureBufferRefill();
        std::string readNextTrace();
        void parse();
};

#endif
