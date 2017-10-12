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

/**
*   @file Trace.cpp
*   @section DESCRIPTION
*    
*   A container for task events. A trace can empty or contain one or more events. 
*   A trace is complete if it begins with a start event, ends with an finalization 
*   event, and all event transitions are valid.
*
*   @author Adan Hirales Carbajal, Vincent Bernard Bloom
*   @version 1.0 
*   @date 10/01/17 
*/

#ifndef _TRACE__
#define _TRACE__

#include <set>
#include <bitset>
#include "Event.h"


namespace ETrace {

/**
* @brief Class that maintains the Avrg. Allocated resources 
*/
class AvgAllocResources {
public :
    unsigned int num_req_cores{};
    unsigned int num_req_ram{};
    unsigned int num_req_disk{};
    float sum_norm_req_cores{};
    float sum_norm_req_ram{};
    float sum_norm_req_disk{};
    
    //AvgAllocResources() = default;
    void add(const Event::TaskEvent&);
    AvgAllocResources& operator+=(AvgAllocResources&);
    void clear();
    std::string to_json() const;
};
 
/**
* @brief Class that stores task events
*/
class Trace {
private:
    unsigned long long jid{};       // Job ID (the task container) 
    std::string user;               // Job owner
    std::string uuid;               // The trace UUID. Merging traces may replace the uuid
    unsigned long long startTime;   // Timestamp of the first trace event
    AvgAllocResources resources{};  // Avrg. Allocated resources
    std::set<Event::TaskEvent,Event::Event_Comparator> events; // Task events. They are assumed to be in temporal order

    bool validateEvent(Event::EventType);

public:
    // Constructor: Constructs an empty Trace
    Trace() = default;

    // Constructor: Transforms a JSON formatted string to a Trace
    Trace(const std::string&);

    // Capacity: Test if the container is empty
    bool empty();

    // Capacity: Return the number of stored events
    unsigned int size();

    // Modifier: insert an element (event)
    void insert(const Event::TaskEvent& );

    // Modifier: erase 
    void clear();

    // Modifier: merge events of this trace with those of another
    Trace& operator+=(Trace&);

    // Operation: gets the last inserted event
    Event::TaskEvent& last_event();

    // Operation: gets the trace job id
     unsigned long long get_jid() { return jid; }

    // Operation: gets the trace uuid
    std::string get_uuid() { return uuid; } 

    // Operation: get trace user
    std::string get_user() { return user; }

    // Operation: gets the trace start time
    unsigned long long time_stamp() { return startTime; }

    // Operation: returns true if the trace was evicted, otherwise false
    bool evicted();

    // Operation: returns true if the trace failed, otherswise false
    bool _failed();

    // Operation: returns true if the trace finished, otherwise false
    bool finished();

    // Operation: returns true if the trace completed, otherwise false
    //            Completion means the trace reached a final event
    bool completed();

    // Operation: returns true if the trace was killed, otherwise false
    bool killed();

    // Operation: returns true if the trace was lost, otherwise false
    bool lost();

    // Operation: returns true if the trace is complete.
    // A trace is complete if its first event is an initial, reaches a final event, and all state transitions are valid
    bool isComplete();

    // Operation: returns true is the trace was resubmitted, otherwise false
    bool isResubmitted();
        
    // Operation: transforms the object to a JSON formatted string
    std::string to_json() const;

};

    
    struct StartTimeComparator {
        bool operator()(Trace& t1, Trace& t2){
            return t1.time_stamp() < t2.time_stamp();
        }
    };
}
#endif
