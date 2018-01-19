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


#ifndef _JEVENT_MERGE_
#define _JEVENT_MERGE_

#include <thread>
#include <condition_variable>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "barrier.h"
#include "onlinedatastrmdup.h"
#include "threadsafe_lookup_table.h"

class JEventMerge{
private:
    // Command line arguments
    std::string ipath;
    std::string opath;
    std::string atl;
    std::string rtl;

    std::mutex fdm;                 // Load trace ids mutex
    std::condition_variable fdcv;   // Load trace ids condition variable
    bool available;

    std::mutex rsm;                 // Reader state mutex
    std::mutex csm;                 // Coordinator syncronization mutex
    std::condition_variable ccv;    // Coordinator contion variable
    Barrier barrier{0};             // log reader barrier


    // Data stream duplicates
    // MapDataStream<unsigned long long> dsd; 
    threadsafe_lookup_table<unsigned long long, unsigned long long> duplicates;
    

    // Log and log reader state
    boost::dynamic_bitset<> state;

    std::unique_ptr<MapDataStream<unsigned long long>> loadTraceIDs(std::string);
    void buildLookupTable(std::unique_ptr<MapDataStream<unsigned long long>>);
    void uniqueEventFilter(std::string);
    void logReader(std::string, int);

public:
    void master();
    
    JEventMerge(std::string _ipath, std::string _opath, std::string _atl, std::string _rtl) : ipath{_ipath}, opath{_opath}, atl{_atl}, rtl{_rtl} { available = true; };
};

#endif
