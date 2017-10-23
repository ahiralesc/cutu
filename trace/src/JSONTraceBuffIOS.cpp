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

#include <iostream>
#include <cstddef>
#include "JSONTraceBuffIOS.h"

using namespace std;
using namespace ETrace;


JSONTraceBuffIOS::JSONTraceBuffIOS(string infile, int ntraces) :
    in_file{infile}, num_traces{ntraces} {
    traces.clear();
    initialized = false;
    finalized = false;
}


void JSONTraceBuffIOS::initialize() {
    if( !initialized ) {
        stream = ifstream(in_file);

        if(!stream) {
            cerr << "Error: could not open file" << endl;
            exit(EXIT_FAILURE);
        }

        // Locate start location of the first trace
        char next;
        do{ stream.get(next); }while( next != '[');
        initialized = true;
    }
}


string JSONTraceBuffIOS::readNextTrace() {
    int balance = 0;
    char next;
    string buffer = "";
    
    if( !stream.is_open() )
        return buffer;

    // Discard padding characters
    do{ stream.get(next);
        if( next == ']' || stream.eof() ) {
            finalized = true;
            stream.close();
            return buffer;
        }
    }while(next != '{');
        
    // Trace back a single character
    stream.unget();

    // Extract the target pattern

    do{ stream.get(next);
        switch (next){
            case ('{'):
                ++balance;
                break;
            case ('}'):
                --balance;
                break;
            default:
                break;
        }
        buffer += next;
    } while (balance > 0);

    return buffer;
}


void JSONTraceBuffIOS::ensureBufferRefill(){
    if(traces.empty() && !finalized) {
        initialize();
        for(int i=0; i<num_traces; i++) {
            string trace = readNextTrace();
            if(trace.size() > 0)
                traces.push_back(trace);
            else
                break;
        }
    }
}


Trace JSONTraceBuffIOS::next() {
    Trace empty{};
    string json{};

    ensureBufferRefill();
    if(traces.size() == 0)
        return empty;
    else{
        json = traces.front();
        traces.erase(traces.begin());
        Trace trace{json};
        return trace;
    }
}


Trace JSONTraceBuffIOS::peek() {
    Trace empty{};
    string json{};

    ensureBufferRefill();
    if(traces.size() == 0)
        return empty;
    else{
        json = traces.front();
        Trace trace{json};
        return trace;
    }
}


bool JSONTraceBuffIOS::empty() {
    return finalized && traces.empty(); 
}
