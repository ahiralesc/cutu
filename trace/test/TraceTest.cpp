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

BOOST_AUTO_TEST_CASE( Trace_creation ) 
{
    // Only events and the main trace properties are stored.

    string json =  "{"
    "\"trace_id\": \"3689348814741910300\","
    "\"job_id\": 3689348814741910300,"
    "\"user_name\": \"3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
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

    Trace trace{json};

    TaskEvent ev = trace.last_event();

    bool eval = 
        (trace.get_jid() == 3689348814741910300) &&
        (trace.get_tid() == "3689348814741910300") &&
        (trace.get_user() == "3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3") &&
        (ev.timestamp == 3689348814741910301) &&
        (ev.missing_info == 2) &&
        (ev.job_id == 3689348814741910300) &&
        (ev.task_index == 3689348814741910300) &&
        (ev.machine_id == 3689348814741910300) &&
        (ev.event_type == EventType::submit) && 
        (ev.user_name == "3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3") &&
        (ev.scheduling_class == 3)  &&
        (ev.priority == 255) && 
        (ev.norm_req_cores == 0.01f)&& 
        (ev.norm_req_ram == 0.99f) && 
        (ev.norm_req_disk == 1.0f) &&
        (ev.constraints == true);
    
    BOOST_CHECK_EQUAL(eval,true);
}


BOOST_AUTO_TEST_CASE( emptynes_test )
{
    // Only events and the main trace properties are stored.

    string json =  "{"
    "\"trace_id\": \"3689348814741910300\","
    "\"job_id\": 3689348814741910300,"
    "\"user_name\": \"3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
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

    bitset<2> states;
    states.reset();

    Trace t1{};
    if(t1.empty() == true)
        states.set(0); 
    Trace t2{json};
    if(t2.empty() == false)
        states.set(1);

    BOOST_CHECK_EQUAL(states.all(),true);
}





BOOST_AUTO_TEST_CASE( Trace_creation_two_events )
{
    // Only events and the main trace properties are stored.

    string json =  "{"
    "\"trace_id\": \"3689348814741910300\","
    "\"job_id\": 3689348814741910300,"
    "\"user_name\": \"3Adsf4#%Zzkd/32SKkfAk3Adsf4#%Zzkd/32SKkfAkw3\","
    "\"uuid\":\"5c319860-5f51-4093-aacb-778cfcab65cc\","
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
        "},"
        "{"
        "\"timestamp\": 123,"
        "\"missing_info\": 1,"
        "\"job_id\": 123,"
        "\"task_index\": 123,"
        "\"machine_id\": 123,"
        "\"event_type\": 0,"
        "\"user_name\": \"AAA\","
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

    Trace trace{json};

    TaskEvent ev = trace.last_event();


    bool eval =
        (ev.timestamp == 123) &&
        (ev.missing_info == 1) &&
        (ev.job_id == 123) &&
        (ev.task_index == 123) &&
        (ev.machine_id == 123) &&
        (ev.event_type == EventType::submit) &&
        (ev.user_name == "AAA") &&
        (ev.scheduling_class == 1)  &&
        (ev.priority == 255) &&
        (ev.norm_req_cores == 0.01f)&&
        (ev.norm_req_ram == 0.01f) &&
        (ev.norm_req_disk == 0.01f) &&
        (ev.constraints == true);

    //cout << "Value : " << ev.constraints << std::endl;
    BOOST_CHECK_EQUAL(eval,true);
}

BOOST_AUTO_TEST_SUITE_END()
