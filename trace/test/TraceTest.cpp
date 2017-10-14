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

//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "trace test"

#include <iostream>
#include <string>
#include <boost/test/unit_test.hpp>
#include "Trace.h"
#include "Event.h"

using namespace ETrace;
using namespace Event;
using namespace std;

BOOST_AUTO_TEST_SUITE(trace)

/**
*   Test cases:
*   1. Trace, evaluate default object initialization
*   2. Trace(stirng), evaluate object initialization
*   3. empty, evaluate the container is/not empty 
*   4. size, evaluate the number of stored events. Expected: 1
*/
BOOST_AUTO_TEST_CASE( constructor_capacity_tests )
{
    string json =  "{"
    "\"job_id\": 3689348814741910300,"
    "\"user_name\": \"3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
    "\"timestamp\": 3689348814741910301,"
    "\"events\": ["
        "{"
        "\"timestamp\": 3689348814741910301,"
        "\"missing_info\": 2,"
        "\"job_id\": 3689348814741910300,"
        "\"task_index\": 3689348814741910300,"
        "\"machine_id\": 3689348814741910300,"
        "\"event_type\": 0,"
        "\"user_name\": \"3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.99,"
        "\"norm_req_disk\": 1.0,"
        "\"constraints\": true"
        "}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\": 9.87654321,"
    "\"avg_norm_alloc_ram\": 9.87654321,"
    "\"avg_norm_alloc_disk\": 9.87654321"
    "},"
    "\"factors\": {"
        "\"class\": \"abc123\","
        "\"released_time\": 3689348814741910300,"
        "\"waiting_time\": 3689348814741910300,"
        "\"start_deadline\": 3689348814741910300,"
        "\"completion_deadline\": 3689348814741910300,"
        "\"system_time\": 3689348814741910300,"
        "\"req_time\": 3689348814741910300,"
        "\"req_cores\": 8589934591,"
        "\"req_ram\": 8589934591,"
        "\"req_disk\": 3689348814741910300"
    "}"
    "}";

    bitset<4> states;
    states.reset();

    // Test 1
    Trace t1{};
    if(t1.empty() == true)
        states.set(0); 
    
    // Test 2
    Trace t2{json};
    bool eval = (t2.get_jid() == 3689348814741910300) &&
                (t2.get_uuid() == "5c319860-5f51-4093-aacb-778cfcab65cc") &&
                (t2.get_user() == "3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3"); 
    if(eval)
        states.set(1);
    // Test 3
    if(!t2.empty() && t1.empty())
        states.set(2);

    // Test 4
    if(t2.size() == 1)
        states.set(3);

    BOOST_CHECK_EQUAL(states.all(),true);
}


/**
*   Test cases:
*   1. insert, 
*       We assume events belong to the same trace. However, they are inserted in two separate traces: trace1 and 
*       trace2. Traces are disjoint sets. 
*       Events will be inserted out of order in terms of event timestamp and event sequence.
*       Expected outcome: non-empty traces, trace timestamp equal to the minimum event timestamp,
*                         events ordered by timestamp.
*
*
*    2. merge,
*       Trace 1 and 2 are merged. No enforcement is made to verify events are disjoint prior merging.
*       
*/
BOOST_AUTO_TEST_CASE( modifier_tests )
{    
    string trace1 =  "{"
    "\"job_id\": 1,"
    "\"user_name\": \"3A\","
    "\"uuid\":\"5c\","
    "\"timestamp\": 10,"
    "\"events\": ["
        "{"
        "\"timestamp\": 10,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 2,"
        "\"machine_id\": 1,"
        "\"event_type\": 7,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.1,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.1,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 15,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 4,"
        "\"machine_id\": 1,"
        "\"event_type\": 5,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.2,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.1,"
        "\"constraints\": true"
        "}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\": 9.87654321,"
    "\"avg_norm_alloc_ram\": 9.87654321,"
    "\"avg_norm_alloc_disk\": 9.87654321"
    "},"
    "\"factors\": {"
        "\"class\": \"abc123\","
        "\"released_time\": 3689348814741910300,"
        "\"waiting_time\": 3689348814741910300,"
        "\"start_deadline\": 3689348814741910300,"
        "\"completion_deadline\": 3689348814741910300,"
        "\"system_time\": 3689348814741910300,"
        "\"req_time\": 3689348814741910300,"
        "\"req_cores\": 8589934591,"
        "\"req_ram\": 8589934591,"
        "\"req_disk\": 3689348814741910300"
    "}"
    "}";

    string e1 = "{"
        "\"timestamp\": 5,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 1,"
        "\"machine_id\": 1,"
        "\"event_type\": 0,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.3,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 1.0,"
        "\"constraints\": true"
        "}";

    string e2 = "{"
        "\"timestamp\": 20,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 5,"
        "\"machine_id\": 1,"
        "\"event_type\": 0,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.4,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.9,"
        "\"constraints\": true"
        "}";

    string e3 = "{"
        "\"timestamp\": 25,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 6,"
        "\"machine_id\": 1,"
        "\"event_type\": 1,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.5,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.8,"
        "\"constraints\": true"
        "}";

    string e4 = "{"
        "\"timestamp\": 12,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 3,"
        "\"machine_id\": 1,"
        "\"event_type\": 1,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.6,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.7,"
        "\"constraints\": true"
        "}";


    string e5 = "{"
        "\"timestamp\": 30,"
        "\"missing_info\": 2,"
        "\"job_id\": 1,"
        "\"task_index\": 7,"
        "\"machine_id\": 1,"
        "\"event_type\": 4,"
        "\"user_name\": \"3A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.7,"
        "\"norm_req_ram\": 0.5,"
        "\"norm_req_disk\": 0.6,"
        "\"constraints\": true"
        "}";


    std::bitset<3> states;
    states.reset();
    
    // Test 1
    Trace t1{trace1};
    t1.insert(TaskEvent{e1,"json"});
    t1.insert(TaskEvent{e2,"json"});
    if(t1.time_stamp() == 5)
        states.set(0);
    
    Trace t2{};
    t2.insert(TaskEvent{e5,"json"});
    t2.insert(TaskEvent{e4,"json"});
    t2.insert(TaskEvent{e3,"json"});
    if(t2.time_stamp() == 12)
        states.set(1);

    // Test 2
    t1 += t2;
    if(t1.time_stamp() == 5 && t1.size() == 7)
        states.set(2);


    BOOST_CHECK_EQUAL(states.all(),true);
}


/** 
*   Test cases (using an incomplete trace):
*   1. last_evet, evaluates event equality. Expected last event timestamp: 20
*   2. get_jid, evaluates job id equality. Expected: 10
*   3. get_tid, evaluates tid equality. Expected: 1
*   4. get_uuid, evaluates uuid equality. Expected: 5c319860-5f51-4093-aacb-778cfcab65cc
*   5. get_user, evaluates user name equality. Expected: A
*   6. get_start_time, evaluates trace start time. Expected: 10
*   7. evicted, evaluates if the trace was evicted. Expected: false
*   8. _failed, evaluates if the trace failed. Expected: false
*   9. finished, evaluates if the trace finished. Expected: false
*   10. completed, evaluates if the trace completed. Expected: false
*   11. killed, evaluates if the trace was killed. Expected: false
*   12. lost, evaluates if the trace was lost. Expected: false
*   13. isComplete, evaluates if the trace is complete. Expected: false 
*   14. isResbmitted, evaluates is the trace was resubmitted. Expected: false
*
*   Test are re-evaluated under the following assumtions:
*   - The trace is complete (a complete trace is used)
*   - The traces was resubmitted
*   - The trace failed
*   Thus, tests 8, 9, 10, 13, 14 are true
*/ 
BOOST_AUTO_TEST_CASE( operation_tests )
{
    string incomplete_trace =  "{"
    "\"job_id\": 10,"
    "\"user_name\": \"A\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
    "\"timestamp\":10,"
    "\"events\": ["
        "{"
        "\"timestamp\": 20,"
        "\"missing_info\": 2,"
        "\"job_id\": 10,"
        "\"task_index\": 20,"
        "\"machine_id\": 20,"
        "\"event_type\": 1,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.2,"
        "\"norm_req_ram\": 0.2,"
        "\"norm_req_disk\": 0.2,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 10,"
        "\"missing_info\": 1,"
        "\"job_id\": 10,"
        "\"task_index\": 10,"
        "\"machine_id\": 10,"
        "\"event_type\": 0,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\": 9.87654321,"
    "\"avg_norm_alloc_ram\": 9.87654321,"
    "\"avg_norm_alloc_disk\": 9.87654321"
    "},"
    "\"factors\": {"
        "\"class\": \"abc123\","
        "\"released_time\": 3689348814741910300,"
        "\"waiting_time\": 3689348814741910300,"
        "\"start_deadline\": 3689348814741910300,"
        "\"completion_deadline\": 3689348814741910300,"
        "\"system_time\": 3689348814741910300,"
        "\"req_time\": 3689348814741910300,"
        "\"req_cores\": 8589934591,"
        "\"req_ram\": 8589934591,"
        "\"req_disk\": 3689348814741910300"
    "}"
    "}";


string complete_trace =  "{"
    "\"job_id\": 11,"
    "\"user_name\": \"A\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
    "\"timestamp\":10,"
    "\"events\": ["
        "{"
        "\"timestamp\": 10,"
        "\"missing_info\": 2,"
        "\"job_id\": 11,"
        "\"task_index\": 1,"
        "\"machine_id\": 1,"
        "\"event_type\": 0,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 3,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.2,"
        "\"norm_req_ram\": 0.2,"
        "\"norm_req_disk\": 0.2,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 20,"
        "\"missing_info\": 1,"
        "\"job_id\": 11,"
        "\"task_index\": 2,"
        "\"machine_id\": 1,"
        "\"event_type\": 1,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 30,"
        "\"missing_info\": 1,"
        "\"job_id\": 11,"
        "\"task_index\": 3,"
        "\"machine_id\": 1,"
        "\"event_type\": 3,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 40,"
        "\"missing_info\": 1,"
        "\"job_id\": 11,"
        "\"task_index\": 4,"
        "\"machine_id\": 1,"
        "\"event_type\": 0,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 50,"
        "\"missing_info\": 1,"
        "\"job_id\": 11,"
        "\"task_index\": 5,"
        "\"machine_id\": 1,"
        "\"event_type\": 1,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "},"
        "{"
        "\"timestamp\": 60,"
        "\"missing_info\": 1,"
        "\"job_id\": 11,"
        "\"task_index\": 2,"
        "\"machine_id\": 1,"
        "\"event_type\": 4,"
        "\"user_name\": \"A\","
        "\"scheduling_class\": 1,"
        "\"priority\": 255,"
        "\"norm_req_cores\": 0.01,"
        "\"norm_req_ram\": 0.01,"
        "\"norm_req_disk\": 0.01,"
        "\"constraints\": true"
        "}"
    "],"
    "\"avg_alloc_resources\": {"
    "\"avg_norm_alloc_cores\": 9.87654321,"
    "\"avg_norm_alloc_ram\": 9.87654321,"
    "\"avg_norm_alloc_disk\": 9.87654321"
    "},"
    "\"factors\": {"
        "\"class\": \"abc123\","
        "\"released_time\": 3689348814741910300,"
        "\"waiting_time\": 3689348814741910300,"
        "\"start_deadline\": 3689348814741910300,"
        "\"completion_deadline\": 3689348814741910300,"
        "\"system_time\": 3689348814741910300,"
        "\"req_time\": 3689348814741910300,"
        "\"req_cores\": 8589934591,"
        "\"req_ram\": 8589934591,"
        "\"req_disk\": 3689348814741910300"
    "}"
    "}";

    bitset<18> states;
    states.reset();

    // Test 1
    Trace trace{incomplete_trace};
    TaskEvent ev = trace.last_event();
    bool eval =
        (ev.timestamp == 20 ) &&
        (ev.missing_info == 2) &&
        (ev.job_id == 10) &&
        (ev.task_index == 20) &&
        (ev.machine_id == 20) &&
        (ev.event_type == EventType::schedule) &&
        (ev.user_name == "A") &&
        (ev.scheduling_class == 3)  &&
        (ev.priority == 255) &&
        (ev.norm_req_cores == 0.2f)&&
        (ev.norm_req_ram == 0.2f) &&
        (ev.norm_req_disk == 0.2f) &&
        (ev.constraints == true);
    if(eval)
        states.set(0);

    // Test 2
    if(trace.get_jid() == 10)
        states.set(1);

    // Test 3 
    if(trace.get_uuid() == "5c319860-5f51-4093-aacb-778cfcab65cc")
        states.set(2);

    // Test 4
    if(trace.get_user() == "A")
        states.set(3);
    
    // Test 5
    if(trace.time_stamp() == 10)
        states.set(4);

    // Test 6
    if(trace.evicted() == false)
        states.set(5);

    // Test 7
    if(trace._failed() == false)
        states.set(6);

    // Test 8
    if(trace.finished() == false)
        states.set(7);

    // Test 9
    if(trace.completed() == false)
        states.set(8);

    // Test 10
    if(trace.killed() == false)
        states.set(9);

    // Test 11
    if(trace.lost() == false)
        states.set(10);

    // Test 12
    if(trace.isComplete() == false)
        states.set(11);

    // Test 13
    if(trace.isResubmitted() == false)
        states.set(12);

    // Tests  8, 9, 10, 13, 14 are reevaluated using a trace that failes, was resubmitted and completed
    Trace ctrace{complete_trace};
    if(ctrace._failed() == true)
        states.set(13);
    if(ctrace.finished() == true)
        states.set(14);
    if(ctrace.completed() == true)
        states.set(15);
    if(ctrace.isComplete() == true)
        states.set(16);
    if(ctrace.isResubmitted() == true)
        states.set(17);

    //std::cout << std::endl << states << std::endl;
    BOOST_CHECK_EQUAL(states.all(),true);
}

BOOST_AUTO_TEST_SUITE_END()
