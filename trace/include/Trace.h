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

#ifndef _TRACE__
#define _TRACE__

#include <set>
#include <bitset>
#include "Event.h"


namespace ETrace {

class AvgAllocResources {
    public :
        // For book keeping. They dont represent allocated resources
        int num_req_cores;
        int num_req_ram;
        int num_req_disk;
        float sum_norm_req_cores;
        float sum_norm_req_ram;
        float sum_norm_req_disk;

        AvgAllocResources();
        void add(const Event::TaskEvent&);
        std::string to_json() const;
};


class Trace {
    /* Bitwise representations of the registered events */ 
    std::bitset<5> event_bs;
    
    /* The last registered event (lre) */
    Event::TaskEvent lre{};

    /* The trace events */
    std::set<Event::TaskEvent,Event::Event_Comparator> events;

    AvgAllocResources resources{};

    bool _empty;

    public :

        unsigned long long jid;
        unsigned long long startTime;
        std::string tid;
        std::string user;
        std::string uuid;
        
        /* Class constructor */
        Trace(const Event::TaskEvent&);
        Trace(const std::string&);
        Trace();
        /* Elapse time between two task events */
        long distance(const Event::TaskEvent&);
        
        /* Returns the last registered event */
        Event::TaskEvent& last_event();
        
        /* Adds an event */
        void addEvent(const Event::TaskEvent&);
        
        /* Coverterts */
        std::string to_json() const;
        
        /* Returns the trace id */
        std::string id() const;
        
        /* Merge events of this trace with that given */
        bool merge(Trace&);
        
        /* Validates if a trace is complete or incomplete */
        bool isComplete();

        bool empty();
    };



    struct StartTimeComparator {
        bool operator()(const Trace& t1, const Trace& t2){
            return t1.startTime < t2.startTime;
        }
    };
}
#endif
